from random import seed
from random import randrange
from random import random
from csv import reader
from math import exp

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

def convertir2Int(dataset, columna):
	class_values = [celda[columna] for celda in dataset]
	unique = set(class_values)
	lookup = dict()
	for i, value in enumerate(unique):
		lookup[value] = i
	for celda in dataset:
		celda[columna] = lookup[celda[columna]]
	return lookup

def valorMaxYMin(dataset):
	minmax = list()
	stats = [[min(columna), max(columna)] for columna in zip(*dataset)]
	return stats

def normalizar(dataset, minmax):
	for celda in dataset:
		for i in range(len(celda)-1):
			celda[i] = (celda[i] - minmax[i][0]) / (minmax[i][1] - minmax[i][0])

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

def evaluacion(dataset, algorithm, n_folds, *args):
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

def funcionActivacion(weights, inputs):
	activation = weights[-1]
	for i in range(len(weights)-1):
		activation += weights[i] * inputs[i]
	return activation

def transferencia(activation):
	return 1.0 / (1.0 + exp(-activation))

def propagacion(network, celda):
	inputs = celda
	for layer in network:
		new_inputs = []
		for neuron in layer:
			activation = funcionActivacion(neuron['weights'], inputs)
			neuron['output'] = transferencia(activation)
			new_inputs.append(neuron['output'])
		inputs = new_inputs
	return inputs

def derivada(output):
	return output * (1.0 - output)

def errorBackprop(network, expected):
	for i in reversed(range(len(network))):
		layer = network[i]
		errors = list()
		if i != len(network)-1:
			for j in range(len(layer)):
				error = 0.0
				for neuron in network[i + 1]:
					error += (neuron['weights'][j] * neuron['delta'])
				errors.append(error)
		else:
			for j in range(len(layer)):
				neuron = layer[j]
				errors.append(expected[j] - neuron['output'])
		for j in range(len(layer)):
			neuron = layer[j]
			neuron['delta'] = errors[j] * derivada(neuron['output'])

def cambiarPesos(network, celda, l_rate):
	for i in range(len(network)):
		inputs = celda[:-1]
		if i != 0:
			inputs = [neuron['output'] for neuron in network[i - 1]]
		for neuron in network[i]:
			for j in range(len(inputs)):
				neuron['weights'][j] += l_rate * neuron['delta'] * inputs[j]
			neuron['weights'][-1] += l_rate * neuron['delta']

def entrenar(network, train, l_rate, n_epoch, n_outputs):
	for epoch in range(n_epoch):
		for celda in train:
			outputs = propagacion(network, celda)
			expected = [0 for i in range(n_outputs)]
			expected[celda[-1]] = 1
			errorBackprop(network, expected)
			cambiarPesos(network, celda, l_rate)

def inicializacion(n_inputs, n_hidden, n_outputs):
	network = list()
	hidden_layer = [{'weights':[random() for i in range(n_inputs + 1)]} for i in range(n_hidden)]
	network.append(hidden_layer)
	output_layer = [{'weights':[random() for i in range(n_hidden + 1)]} for i in range(n_outputs)]
	network.append(output_layer)
	return network

def prediccion(network, celda):
	outputs = propagacion(network, celda)
	return outputs.index(max(outputs))

def propagacionB(train, test, l_rate, n_epoch, n_hidden):
	n_inputs = len(train[0]) - 1
	n_outputs = len(set([celda[-1] for celda in train]))
	network = inicializacion(n_inputs, n_hidden, n_outputs)
	entrenar(network, train, l_rate, n_epoch, n_outputs)
	prediccionions = list()
	for celda in test:
		prediccionion = prediccion(network, celda)
		prediccionions.append(prediccionion)
	return(prediccionions)

seed(1)
filename = 'Iris.csv'
dataset = cargarArchivo(filename)
for i in range(len(dataset[0])-1):
	convertir2Float(dataset, i)
convertir2Int(dataset, len(dataset[0])-1)
minmax = valorMaxYMin(dataset)
normalizar(dataset, minmax)
n_folds = 3
l_rate = 0.3
n_epoch = 500
n_hidden = 5
scores = evaluacion(dataset, propagacionB, n_folds, l_rate, n_epoch, n_hidden)
print('Valores Obtenidos: %s' % scores)
print('Media de Porcentajes: %.3f%%' % (sum(scores)/float(len(scores))))
