from random import seed
from random import randrange
from csv import reader
 
def cargarArchivo(filename):
	dataset = list()
	with open(filename, 'r') as file:
		csv_reader = reader(file)
		for celda in csv_reader:
			if not celda:
				continue
			dataset.append(celda)
	return dataset

def convertir2Float(dataset, columna):
	for celda in dataset:
		celda[columna] = float(celda[columna].strip())
 
def manejoInfo(dataset, columna):
	class_values = [celda[columna] for celda in dataset]
	unique = set(class_values)
	lookup = dict()
	for i, value in enumerate(unique):
		lookup[value] = i
	for celda in dataset:
		celda[columna] = lookup[celda[columna]]
	return lookup

def validacion(dataset, n_folds):
	dataset_split = list()
	dataset_copy = list(dataset)
	fold_size = int(len(dataset) / n_folds)
	for i in range(n_folds):
		fold = list()
		while len(fold) < fold_size:
			index = randrange(len(dataset_copy))
			fold.append(dataset_copy.pop(index))
		dataset_split.append(fold)
	return dataset_split
 

def porcentaje(actual, prediccioned):
	correct = 0
	for i in range(len(actual)):
		if actual[i] == prediccioned[i]:
			correct += 1
	return correct / float(len(actual)) * 100.0
 
def algoritmoEvaluacion(dataset, algorithm, n_folds, *args):
	folds = validacion(dataset, n_folds)
	scores = list()
	for fold in folds:
		train_set = list(folds)
		train_set.remove(fold)
		train_set = sum(train_set, [])
		test_set = list()
		for celda in fold:
			celda_copy = list(celda)
			test_set.append(celda_copy)
			celda_copy[-1] = None
		prediccioned = algorithm(train_set, test_set, *args)
		actual = [celda[-1] for celda in fold]
		accuracy = porcentaje(actual, prediccioned)
		scores.append(accuracy)
	return scores
 
def prediccion(celda, weights):
	activation = weights[0]
	for i in range(len(celda)-1):
		activation += weights[i + 1] * celda[i]
	return 1.0 if activation >= 0.0 else 0.0
 
def entrenamientoPesos(train, l_rate, n_repeticion):
	weights = [0.0 for i in range(len(train[0]))]
	for repeticion in range(n_repeticion):
		for celda in train:
			prediccionion = prediccion(celda, weights)
			error = celda[-1] - prediccionion
			weights[0] = weights[0] + l_rate * error
			for i in range(len(celda)-1):
				weights[i + 1] = weights[i + 1] + l_rate * error * celda[i]
	return weights
 
def perceptron(train, test, l_rate, n_repeticion):
	prediccionions = list()
	weights = entrenamientoPesos(train, l_rate, n_repeticion)
	for celda in test:
		prediccionion = prediccion(celda, weights)
		prediccionions.append(prediccionion)
	return(prediccionions)
 
seed(1)

filename = 'Iris.csv'
dataset = cargarArchivo(filename)
for i in range(len(dataset[0])-1):
	convertir2Float(dataset, i)

manejoInfo(dataset, len(dataset[0])-1)

n_folds = 3
l_rate = 0.01
n_repeticion = 500
scores = algoritmoEvaluacion(dataset, perceptron, n_folds, l_rate, n_repeticion)
print('Aprendizaje por columnaa: %s' % scores)
print('Porcentaje de Aprendizaje: %.3f%%' % (sum(scores)/float(len(scores))))
