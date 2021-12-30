class Grafo:

	# Crea un grafo (dirigido o no) que puede poseer vertices recibidos por parámetro o estar vacío
	def __init__(self, esDirigido=bool, vertices=None):
		if vertices is None:
			vertices = []
		self.esDirigido = esDirigido
		self.vertices = {}
		self.cantidadVertices = len(vertices)
		for v in vertices:
			self.vertices[v] = {}


	# Agrega un vértice al grafo. En caso de que el vértice ya existía no hay cambios
	def agregarVertice(self, nuevo):
		if nuevo not in self.vertices:
			self.cantidadVertices += 1
			self.vertices[nuevo] = {}


	# Borra el vértice recibido por parámetro. En caso de que el vértice no existiese no hay cambios
	def borrarVertice(self, vertice):
		if vertice in self.vertices:
			self.cantidadVertices -= 1
			del self.vertices[vertice]

	# Agrega una arista entre destino y origen con peso recibido por parámetro.
	# En caso de agregar una arista en la que una de sus puntas no termine existiendo, no quedará notificado y fallará eventualmente

	def agregarArista(self, origen, destino, peso):
		if destino not in self.adyacentes(origen):
			self.vertices[origen][destino] = peso
			if not self.esDirigido:
				self.vertices[destino][origen] = peso

	# Modifica el peso de la arista entre origen y destino.
	# Si origen o destino no están en grafo, u origen y destino no son adyacentes devuelve False,
	# de lo contrario modifica el peso y devuelve True
	def modificarPeso(self, origen, destino, peso):
		if destino not in self.vertices or origen not in self.vertices:
			return False
		if self.sonAdyacentes(origen, destino):
			self.vertices[origen][destino] = peso
			if not self.esDirigido:
				self.vertices[destino][origen] = peso
			return True
		return False

	# Borra la arista entre origen y destino.
	# Si origen o destino no están en grafo, u origen y destino no son adyacentes devuelve False,
	# de lo contrario borra la arista y devuelve True
	def borrarArista(self, origen, destino):
		if destino not in self.vertices or origen not in self.vertices:
			return False
		if destino in self.adyacentes(origen):
			del self.vertices[origen][destino]
			if not self.esDirigido:
				del self.vertices[destino][origen]
			return True
		return False

	# Devuelve una lista con las cadenas que representan los vértices
	def obtenerVertices(self):
		return list(self.vertices.keys())

	# Funcion auxiliar, devuelve una lista con las cadenas que representan los adyacentes a todos los vértices
	def obtenerVertices_(self):
		return list(self.vertices.values())

	# Devuelve una lista con todos los adyacentes al vértice recibido por parámetro. Si vértice no pertenece a grafo
	# se devuelve None
	def adyacentes(self, vertice):
		if vertice in self.vertices:
			return list(self.vertices[vertice])
		return None

	def grafoAristas(self):
		aristas = []
		vertices = self.obtenerVertices_()
		for i in vertices:
			aristas += self.adyacentes(i)
		return aristas

	# Devuelve una lista con todas las aristas del grafo
	def obtenerAristas(self):
		lista = []
		for v in self.obtenerVertices():
			for w in self.vertices[v]:
				lista.append((v, w, self.vertices[v][w]))
		return lista

	# Devuelve si vertice pertenece al grafo
	def verticePertenece(self, vertice):
		return vertice in self.obtenerVertices()

	# Devuelve si origen y destino son adyacentes
	def sonAdyacentes(self, origen, destino):
		return destino in self.adyacentes(origen)

	# Devuelve la cantidad de vértices en el grafo
	def cantVertices(self):
		return self.cantidadVertices

	# Devuelve la cantidad de aristas en el grafo
	def cantAristas(self, vertice):
		return len(self.vertices[vertice])

	# Devuelve el peso de la arista entre origen y destino. Si origen y destino no son adyacentes, u origen y
	# destino no pertenecen al grafo devuelve False
	def obtenerPeso(self, origen, destino):
		if origen not in self.vertices or destino not in self.vertices or not self.sonAdyacentes(origen, destino):
			return False
		return self.vertices[origen][destino]

	# Devuelve un vértice aleatorio del grafo
	def vertice_aleatorio(self):
		return random.choice(self.obtenerVertices())
