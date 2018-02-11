# http://ccy.dd.ncu.edu.tw/~chen/resource/pso/pso.htm

# macro for definition here
macro for(expr)
  {{expr.args.first.args.first}}.each do |{{expr.name.id}}|
    {{expr.args.first.block.body}}
  end
end

Dim = 2 # Dimension of problem's search space
NumParticle = 40 # Number of particles in population
max_cycle = 100 # Maximum iteration cycle
C1 = 1.5
C2 = 1.5

# Variable Declaration

# Lower bound and upper bound for each dimension
xLo = Array(Float64).new(Dim)
xHi = Array(Float64).new(Dim)

# current velocity of particle
v = Array(Float64).new(Dim * NumParticle)

# current position of particle
x = Array(Float64).new(Dim * NumParticle)

# individual optimal fitness
individual_optimal_fit = Array(Float64).new(NumParticle)

# individual optimal position
individual_optimal_x = Array(Float64).new(Dim * NumParticle)

# social optimal fitness
social_optimal_fit = 0.0

# social optimal position
social_optimal_x = Array(Float64).new(Dim)

# rand number generate here
fRan = Random.new.rand(1.0..123456789.0)

puts "initialization done"

# Initialization
xLo[0] = xLo[1] = -5.0
xHi[0] = xHi[1] = 5.0

#for i in 0...Dim do
#  for j in 0...NumParticle do
#    x[(i * NumParticle) + j] = individual_optimal_x[(i * NumParticle) + j] = Random.new.rand(xLo[i], xHi[i])
#    v[(i * NumParticle) + j] = Random.new.rand(xLo[i] / 20.0, xHi[i] / 20.0)
#  end 
#end

#Dim.times do |i| {
#  {
#  NumParticle.times do |j| {
#    {
#    x[(i * NumParticle) + j] = individual_optimal_x[(i * NumParticle) + j] = Random.new.rand(xLo[i], xHi[i])
#    v[(i * NumParticle) + j] = Random.new.rand(xLo[i] / 20.0, xHi[i] / 20.0)
#    }
#  }
#  end
#  }
#}
#end

(0...Dim).each do |i| {
  (0...NumParticle).each do |j| {
    x[(i * NumParticle) + j] = individual_optimal_x[(i * NumParticle) + j] = Random.new.rand(xLo[i], xHi[i])
    v[(i * NumParticle) + j] = Random.new.rand(xLo[i] / 20.0, xHi[i] / 20.0)
  }
  end
}
end

for j in 0...NumParticle do
  individual_optimal_fit[j] = x[(0 * NumParticle) + j] * x[(0 * NumParticle) + j] + x[(1 * NumParticle) + j] * x[(1 * NumParticle) + j]
end
social_optimal_fit = individual_optimal_fit[0]

# Optimization
fitness = 0.0

for cycle in 0...max_cycle do
  for j in 0...NumParticle do # Evaluate each particle
    fitness =  x[(0 * NumParticle) + j] * x[(0 * NumParticle) + j] + x[(1 * NumParticle) + j] * x[(1 * NumParticle) + j] # evaluation function
    if fitness > individual_optimal_fit[j] # Replace the individual optimal position
      individual_optimal_fit[j] = fitness
      individual_optimal_x[(0 * NumParticle) + j] = x[(0 * NumParticle) + j]
      individual_optimal_x[(1 * NumParticle) + j] = x[(1 * NumParticle) + j]
    end
  end

  for j in 0...NumParticle do # Replace the social optimal position
    if individual_optimal_fit[j] > social_optimal_fit
      social_optimal_fit = individual_optimal_fit[j]
      social_optimal_x[0] = individual_optimal_x[(0 * NumParticle) + j]
      social_optimal_x[1] = individual_optimal_x[(1 * NumParticle) + j]
    end
  end

  for i in 0...Dim do
    for j in 0...NumParticle do # Modify the velocity and position of particle
      v[(i * NumParticle) + j] = v[(i * NumParticle) + j] + C1 * Random.new.rand(-1.0, 1.0) * (individual_optimal_x[(i * NumParticle) + j]-x[(i * NumParticle) + j])
        + C2 * Random.new.rand(-1.0, 1.0) * (social_optimal_x[i]-x[(i * NumParticle) + j])
      x[(i * NumParticle) + j]=x[(i * NumParticle) + j]+v[(i * NumParticle) + j]
      if x[(i * NumParticle) + j] > xHi[i]
        x[(i * NumParticle) + j] = xHi[i]
      end
      if x[(i * NumParticle) + j] < xLo[i]
        x[(i * NumParticle) + j] = xLo[i]
      end
    end
  end
end
# write file here
f = File.open("result.txt", "w")
print social_optimal_x[0], "\t", social_optimal_x[1], "\t", "\n"
f.write(social_optimal_x[0])
f.write("\t")
f.write(social_optimal_x[1])
f.write("\t")
f.write("\n")


