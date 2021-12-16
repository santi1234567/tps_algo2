from grafo import Grafo
import input
import sys
import biblioFunciones

def main(argv):
	operaciones = {'rango': biblioFunciones.rango, 'camino': biblioFunciones.camino, 'diametro': biblioFunciones.diametro, 'navegacion': biblioFunciones.navegacion, 'lectura': biblioFunciones.lectura, 'clustering': biblioFunciones.clustering, 'comunidades': biblioFunciones.comunidades, 'conectados': biblioFunciones.conectividad, 'mas_importantes': biblioFunciones.mas_importantes}
	def call_func(func, params):
		operaciones[func](*params) # *params is the magic.
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
			argumentos.append(palabras[1].split(","))
		argumentos = tuple(argumentos)
		operacion = palabras[0]
		if 'listar_operaciones' == line.rstrip():
			biblioFunciones.listar_operaciones()
		elif operacion in operaciones:
			call_func(operacion, argumentos)
		else:
			print("Comando no válido")

if __name__ == "__main__":
	main(sys.argv)
