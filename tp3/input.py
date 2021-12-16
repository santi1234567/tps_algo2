from grafo import Grafo
from io import open
import biblioFunciones
import string
import numpy as np
import csv


def readinputdata(filename):
    grafo = Grafo([], True)
    fichero = open(filename, 'r', encoding='utf-8')
    line = '0'
    n = 0
    while len(line[0]) > 0:
        line = fichero.readline().split('\t')
        largo = len(line)
        line[largo-1] = line[largo-1].rstrip('\n')
        if largo > 0:
            vertice = line[0]
            grafo.agregarVertice(vertice)
            for i in range(1, largo):
                grafo.agregarArista(vertice, line[i], 1)
        n += 1
    grafo.borrarVertice('')
    fichero.close()
    return grafo


# Entrada de Datos
#grafo = readinputdata("wiki-reducido-75000.tsv")  # Aqui pones el nombre del fichero de entrada"""
#biblioFunciones.rango(grafo, "Tokio", 8)
#biblioFunciones.rango(grafo, "Tokio", 3)
#biblioFunciones.rango(grafo, "Perón", 4)

#biblioFunciones.camino(grafo, "Argentina", "Zinedine Zidane")
#biblioFunciones.camino(grafo, "Jeremy Irons", "pejerrey")
#biblioFunciones.camino(grafo, "handball", "Napoleón Bonaparte")

#biblioFunciones.diametro(grafo)

#biblioFunciones.navegacion(grafo, "Argentina")
#biblioFunciones.navegacion(grafo, "Alemania")
#biblioFunciones.navegacion(grafo, "queso")
#biblioFunciones.navegacion(grafo, "Bolivia")
#biblioFunciones.navegacion(grafo, "Brasil")

#paginas = ["Hockey sobre hielo", "Roma", "Japón", "árbol", "Guerra", "Dios", "universo", "Himalaya", "otoño"]
#paginas2 = ["Buenos Aires", "Roma"]
#biblioFunciones.lectura(grafo, paginas)
#biblioFunciones.lectura(grafo, paginas2)


#print(biblioFunciones.clustering(grafo, "River Plate"))
#print(biblioFunciones.clustering(grafo, "Club Atlético River Plate"))
#print(biblioFunciones.clustering(grafo, "Juan Domingo Perón"))
#print(biblioFunciones.clustering(grafo, "Ámsterdam"))
#print(biblioFunciones.clustering(grafo, None))

#biblioFunciones.comunidades(grafo, "Chile")

#cfc = None
#cfc = biblioFunciones.conectividad(grafo, "Boca Juniors", cfc)
#biblioFunciones.conectividad(grafo, "Argentina", cfc)
