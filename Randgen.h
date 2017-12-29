#if !defined(RANDGEN_H)
#define RANDGEN_H
#include <stddef.h>
#include <time.h>
#include <float.h>
static const long   IM1 = 2147483563L;
static const long   IM2 = 2147483399L;
static const long  IMM1 = IM1 - 1L;
static const long   IA1 = 40014L;
static const long   IA2 = 40692L;
static const long   IQ1 = 53668L;
static const long   IQ2 = 52774L;
static const long   IR1 = 12211L;
static const long   IR2 =  3791L;
static const long  NTAB =    32L;
static const long  NDIV = 1L + IMM1 / long(NTAB);
static const float RNMX = 1.0F - FLT_EPSILON;
static const float   AM = 1.0F / 2147483563.0F;

/*-------------------------------------------------------------*/
//      integer random number generator
//      1998.1.1 by CHEN Ching-Han
//-------------------------------------------------------------
class Ran32 {
protected:
  static const long a, m, q, r;
  long seed;
public:
  Ran32(){};
  Ran32(long s){Reset(s);}
  void Reset(long s);
  long Curr() const{return seed;}
  long Next();
  // Returns the current value cv scaled such that 0 <= cv < n.
  long Curr(long n) const {return Curr() % n;}
  long Next(long n){return Next() % n;}
  long Curr(long lo, long hi) const;
  long Next(long lo, long hi);
};



/*---------------------------------------------------------*/
//          float point random number generator
//      1998.1.1 by CHEN Ching-Han
//-------------------------------------------------------------

class fRan32 : private Ran32 {
protected:
  static const float d; // d = 1/m
public:
  fRan32(long s): Ran32(s) {}
  Ran32::Reset; // This can be public
  long Seed(){return seed;}
  float Curr() const { return float(Ran32::Curr()) * d;}
  float Next(){return float(Ran32::Next()) * d;}
  float Curr(float n) const { return (n * float(Ran32::Curr())) * d;}
  float Next(float n) {return (n * float(Ran32::Next())) * d;}
  float Curr(float lo, float hi) const;
  float Next(float lo, float hi);
};


/*--------------------------------------------------------------------*/
//        bit-wise random number generator
//      1998.1.1 by CHEN Ching-Han
//-------------------------------------------------------------

class RanBit32 {
  unsigned long seed; // Assume 32 bits
public:
  RanBit32(unsigned long s);
  void Reset(unsigned long s);
  unsigned long Seed() const;
  int Curr() const;
  int Next();
};

const long Ran32::a = 16807L;       // The multiplier
const long Ran32::m = 2147483647L;  // The modulus
const long Ran32::q = m / a;
const long Ran32::r = m % a;
inline RanBit32::RanBit32(unsigned long s)
// Constructs a random bit sequencer using the
// seed s, assumed to be > 0.
{
  Reset(s);
}

inline unsigned long RanBit32::Seed() const
{
  return seed;
}

inline int RanBit32::Curr() const
// Returns the current random bit
{
  return (seed & 0x80000000L) != 0;
}


void Ran32::Reset(long s)
// Resets the random number sequence by starting
// with a new seed. Note that seed must be between
// [1 .. m-1] in order for things to work properly.
// In particular, DON'T SET THE SEED TO ZERO.
{
  seed = s;
  if (seed < 1) seed = 1;
  else if (seed >= m) seed = m-1;
}

long Ran32::Next()
// Advances to the next random number, having a uniform
// distribution, with a range of [1..m-1]. The period of
// the sequence is m-2, or roughly 2.15x10^9 for a m = 2^31-1.
{
  long k = seed/q;
  seed = a * (seed - k*q) - k*r;
  if (seed < 0) seed += m;
  return seed;
}

long Ran32::Curr(long lo, long hi) const
// Returns the current value of the sequence, cv, and
// restricts it's range to be lo <= cv <= hi.
{
  return lo + (Curr() % (hi-lo+1));
}

long Ran32::Next(long lo, long hi)
// Returns the next value of the sequence, nv, and
// restricts it's range to be lo <= nv <= hi.
{
  return lo + (Next() % (hi-lo+1));
}

const float fRan32::d = 1.0 / float(Ran32::m);
float fRan32::Curr(float lo, float hi) const
// Takes current value, and limits its range from 0 <= cv <= hi.
{
  return lo + ((hi-lo) * float(Ran32::Curr())) * d;
}

float fRan32::Next(float lo, float hi)
// Advances to next value, and limits its range from 0 <= nv <= hi.
{
  return lo + ((hi-lo) * float(Ran32::Next())) * d;
}



void RanBit32::Reset(unsigned long s)
// Initializes seed with s. Note that the seed must be
// non-zero to make any sense.
{
  seed = s;
  if (seed == 0) seed = 421755L; // Arbitrary value
}

int RanBit32::Next()
// Advances to the next random bit, with a cycle
// length of 2^32-1. It assumes longs are 32 bits.
{
  const unsigned long mask = 0xAF; // Bits 7,5,3,2,1,0
  if (seed & 0x80000000L) {
     seed <<= 1;
     seed ^= mask;
     return 1;
  }
  else {
    seed <<= 1;
    return 0;
  }
}

/*---------------------------------------------------------*/
//          Deviate random number generator
//      1998.2.1 by CHEN Ching-Han
//-------------------------------------------------------------

class RandDev
{
    protected:
        // sets default seed argument from system time
        static long TimeSeed(){return (long)time(NULL); }
    public:
        // constructor
        RandDev(long initSeed = TimeSeed());
        // set seed value
        void SetSeed( long newSeed = TimeSeed() );
        // get a uniform deviate between 0.0 and 1.0
        float operator () ();
		// get a uniform deviate between 0 and max
		size_t operator ()(size_t max);
		int operator ()(int max);
    private:
        long Seed;
};

inline RandDev::RandDev ( long initSeed )
{
    if (initSeed < 0) Seed = initSeed;
    else Seed = -initSeed;
}

inline void RandDev::SetSeed(long initSeed )
{
    if (initSeed < 0) Seed = initSeed;
    else Seed = -initSeed;
}

inline size_t RandDev::operator ()(size_t max )
{
    return size_t(float(max) * (*this)());
}

inline int RandDev::operator () ( int max )
{
    return int(float(max) * (*this)());
}

float RandDev::operator () ()
{
    long j, k;
    static long idum2 = 123456789L;
    static long iy    = 0L;
    static long iv[NTAB];
    float temp;

    if (Seed <= 0L)
    {
        if (-Seed < 1L)  Seed = 1L;
        else Seed = -Seed;
        idum2 = Seed;
        for (j = NTAB + 7; j >= 0; --j)
        {
            k = Seed / IQ1;
            Seed = IA1 * (Seed - k * IQ1) - k * IR1;
            if (Seed < 0L) Seed += IM1;

            if (j < NTAB) iv[size_t(j)] = Seed;
        }
        iy = iv[0];
    }
    k = Seed / IQ1;
    Seed = IA1 * (Seed - k * IQ1) - k * IR1;
    if (Seed < 0L)Seed += IM1;
    k = idum2 / IQ2;
    idum2 = IA2 * (idum2 - k * IQ2) - k * IR2;
    if (idum2 < 0L)idum2 += IM2;
    j  = iy / NDIV;
    iy = iv[size_t(j)] - idum2;
    iv[size_t(j)] = Seed;
    if (iy < 1L)iy += IMM1;
    temp = AM * float(iy);
    if (temp > RNMX) return RNMX;
    else  return temp;
}


//
// test integer random number generator program
//
/*
main()
{
	ofstream out("c:/temp/ran_int.dat");
	long seed, hi=0, lo=50, n=10;
  	seed = (unsigned long)time(NULL);
	out << "seed: "<<seed<<"\n";
	out << "low bound: "<<lo<<"\n";
	out << "hi bound: "<<hi<<"\n";
	out << "nb of random number: "<< n<<"\n";
   Ran32 rg(seed);
   for (long i = 0; i<n; i++) out << rg.Next(lo, hi) << ' ';
   out << '\n';
}
*/

//
//  test real random numbr generator
//
/*
main()
{
	ofstream out("c:/temp/ran_real.dat");
	long seed, hi=0, lo=1, n=10;
	seed = (unsigned long)time(NULL);
	out << "seed: "<<seed<<"\n";
	out << "low bound: "<<lo<<"\n";
	out << "hi bound: "<<hi<<"\n";
	out << "nb of random number: "<< n<<"\n";
   fRan32 rg(seed);
   for (long i = 0; i<n; i++) out << rg.Next(lo, hi) << ' ';
   out << '\n';
}
*/

/*
//
// test bit-wise random number generator
//

main()
{
	long numbits=50;
	unsigned long seed;
	ofstream out("c:/temp/ran_bit.dat");
	seed = (unsigned long)time(NULL);
	out << "Seed is: " << seed << '\n';
	RanBit32 rbg(seed);
	for(long i = 0; i<numbits; i++) out << rbg.Next();
	out << '\n';
}
*/
#endif
