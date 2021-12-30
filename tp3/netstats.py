#!/usr/bin/python3
from grafo import Grafo
import input
import sys
import biblioFunciones


def main(argv):

	operaciones = biblioFunciones.obtener_dict_funciones()
	def imprimir_resultado(resultado, costo, operacion):
		if operaciones[operacion][1] == biblioFunciones.tipo_devolucion.VALOR:
			print(resultado)
		else:
			if operaciones[operacion][1] == biblioFunciones.tipo_devolucion.LISTADO:
				separador = ', '
			else:
				separador = ' -> '
			print((separador).join([str(item) for item in resultado]))
		if costo:
			print('Costo: ' + str(costo))

	def call_func(func, params):
		return operaciones[func][0](*params)

	if(len(sys.argv) != 2):
		print("Error de argumentos")

	paginas = input.readinputdata(sys.argv[1])

	if not paginas:
		print("Error en nombre de archivo")
		exit()

	for line in sys.stdin:
		palabras = line.rstrip().split(" ", 1)
		argumentos = [paginas]
		if len(palabras) > 1:
			argumentos += palabras[1].split(",")
		argumentos = tuple(argumentos)
		operacion = palabras[0]
		if 'listar_operaciones' == line.rstrip():
			print('\n'.join([str(item) for item in operaciones]))
		elif operacion in operaciones:
			(resultado, error, costo) = call_func(operacion, argumentos)
			if error:
				print(error)
			else:
				imprimir_resultado(resultado, costo, operacion)
		else:
			print("Comando no válido")

if __name__ == "__main__":
	main(sys.argv)
