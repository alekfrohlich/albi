import simplesbml

model = simplesbml.sbmlModel()
model.addSpecies('x', 0.96259)
model.addSpecies('y', 2.07272)
model.addSpecies('z', 18.65888)
model.addParameter('sigma', 20.0)
model.addParameter('rho', 28)
model.addParameter('beta', 2.67)
model.addRateRule('x', 'sigma *( y - x)')
model.addRateRule('x', 'x *( rho - z) - y')
model.addRateRule('x', 'x* y - beta * z')
print(model.toSBML())