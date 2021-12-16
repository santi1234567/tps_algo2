class Grafo:
    def __init__(self, vertices, esDirigido = bool):
        self.esDirigido = esDirigido
        self.vertices = {}
        self.cantidadVertices = len(vertices)
        for v in vertices:
            self.vertices[v] = {}

    def agregarVertice(self, nuevo):
        if nuevo not in self.vertices:
            self.cantidadVertices += 1
            self.vertices[nuevo] = {}

    def borrarVertice(self, vertice):
        if vertice in self.vertices:
            self.cantidadVertices -= 1
            del self.vertices[vertice]

    def agregarArista(self, origen, destino, peso):
        if origen != destino and destino not in self.adyacentes(origen):
            self.vertices[origen][destino] = peso
            if not self.esDirigido:
                self.vertices[destino][origen] = peso

    def borrarArista(self, origen, destino):
        if destino in self.adyacentes(origen):
            del self.vertices[origen][destino]
            if not self.esDirigido:
                del self.vertices[destino][origen]

    def obtenerVertices(self):
        return list(self.vertices.keys())

    def obtenerVertices_(self):
        return list(self.vertices.values())

    def adyacentes(self, vertice):
        return list(self.vertices[vertice])

    def grafoAristas(self):
        aristas = []
        vertices = self.obtenerVertices_()
        for i in vertices:
            aristas += self.adyacentes(i)
        return aristas

    def obtenerAristas(self):
        lista = []
        for v in self.obtenerVertices():
            for w in self.vertices[v]:
                lista.append((v, w, self.vertices[v][w]))
        return lista

    def verticePertenece(self, vertice):
        if vertice in self.obtenerVertices():
            return True
        else:
            return False

    def sonAdyacentes(self, origen, destino):
        if destino in self.adyacentes(origen):
            return True
        else:
            return False

    def cantVertices(self):
        return self.cantidadVertices

    def cantAristas(self, vertice):
        return len(self.vertices[vertice])

    def obtenerPeso(self, origen, destino):
        return self.vertices[origen][destino]

    def vertice_aleatorio(self):
        return self.obtenerVertices()[0]



