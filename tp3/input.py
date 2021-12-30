from grafo import Grafo
from io import open
import biblioFunciones
import string
import csv

# Lee el archivo de nombre "filename" con formato tsv y crea un grafo dirigido a partir de este.
def readinputdata(filename):
	grafo = Grafo(True, [])
	fichero = open(filename, 'r', encoding='utf-8')
	line = '0'
	while len(line[0]) > 0:
		line = fichero.readline().split('\t')
		largo = len(line)
		line[largo-1] = line[largo-1].rstrip('\n')
		if largo > 0:
			vertice = line[0]
			grafo.agregarVertice(vertice)
			for i in range(1, largo):
				grafo.agregarArista(vertice, line[i], 1)
	grafo.borrarVertice('')
	fichero.close()
	return grafo
