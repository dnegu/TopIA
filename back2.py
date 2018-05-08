from math import exp

inputs = [0.05,0.1]
pesos=[[0.15,0.2,0.25,0.3],[0.40,0.45,0.50,0.55]]
bias= [0.35,0.60]
red1=inputs[0]*pesos[0][0]+inputs[1]*pesos[0][1]+bias[0]*1
red2=inputs[0]*pesos[0][2]+inputs[1]*pesos[0][3]+bias[1]*1

def calcRed(pesos,inputs,bias):
	for i in range(len(pesos)):
		inputs[0]*pesos[0][0]+inputs[1]*pesos[0][1]+bias[0]*1
def logistica(val):
	return 1/(1+exp(-val)) 

print logistica(red1),logistica(red2)


