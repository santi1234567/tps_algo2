from grafo import Grafo
from collections import deque
import random
import heapq
from pila import Pila
from enum import Enum

"""
Todos en Rango (★)
Camino más corto (★)
Diametro (★)
Navegacion por primer link (★)
Lectura a las 2 a.m. (★★)
Coeficiente de Clustering (★★)
Comunidades (★★)
Conectividad (★★)
Artículos más importantes (★★★)
Ciclo de n artículos (★★★)
---------------------------------
Total 18 (★★★★★★★★★★★★★★★★★★)
"""
cfc = {}

class tipo_devolucion(Enum):
	LISTADO = 1
	RECORRIDO = 2
	VALOR = 3


# Devuelve diccionario con las funciones de la Biblioteca. Como valor está la función y un enumerador que indica el tipo de dato que devuelve (valor numérico, recorrido o listado)
def obtener_dict_funciones():
	return {'rango': (rango, tipo_devolucion.VALOR), 'camino': (camino, tipo_devolucion.RECORRIDO), 'diametro': (diametro, tipo_devolucion.RECORRIDO), 'navegacion': (navegacion, tipo_devolucion.RECORRIDO), 'lectura': (lectura, tipo_devolucion.LISTADO), 'clustering': (clustering, tipo_devolucion.VALOR), 'comunidad': (comunidades, tipo_devolucion.LISTADO), 'conectados': (conectividad, tipo_devolucion.LISTADO), 'mas_importantes': (mas_importantes, tipo_devolucion.LISTADO), 'ciclo': (ciclo, tipo_devolucion.RECORRIDO)}

# Recibe un diccionario de padres, el último elemento y un contador tope del recorrido y devuelve una lista con los vértices
# ordenados desde el indicado por el tope hasta último
def obtener_recorrido(padres, ultimo, tope_recorrido):
	contador = tope_recorrido + 1
	recorrido = []
	actual = ultimo
	while actual and contador:
		recorrido.append(actual)
		actual = padres[actual]
		contador -= 1
	return recorrido

"""
Todos en Rango (★)
Comando: rango.
Parámetros: página y n.
Utilidad: permite obtener la cantidad de páginas que se encuenten a exactamente n links/saltos desde la página pasada por parámetro.
Complejidad: Este comando debe ejecutar en O(P+L).

Ejemplo: Entrada:
  rango Tokio,8
  rango Tokio,3
  rango Perón,4

Salida:
  0
  39347
  53346
"""


def bfs_a_distancia_n(grafo, origen, n):
	paginas_a_distancia_n = 0
	visitados = set()
	orden = {}
	orden[origen] = 0
	visitados.add(origen)
	q = deque()
	q.append(origen)
	while q:
		v = q.popleft()
		for w in grafo.adyacentes(v):
			if w not in visitados:
				orden[w] = orden[v] + 1
				if orden[w] == n:
					paginas_a_distancia_n += 1
				else:
					if orden[w] > n:
						return paginas_a_distancia_n
				visitados.add(w)
				q.append(w)
	return paginas_a_distancia_n

def rango(grafo, origen, n):
	n = int(n)
	if origen in grafo.obtenerVertices():
		return (bfs_a_distancia_n(grafo, origen, n), None, None)
	else:
		return (None, "No existe esa página", None)

"""
Camino más corto (★)
Comando: camino.
Parámetros: origen y destino. Origen y destino son páginas.
Utilidad: nos imprime una lista con las páginas con los cuales navegamos de la página origen a la página destino, navegando lo menos posible.
Complejidad: Este comando debe ejecutar en O(P+L), siendo P la cantidad de páginas, y L la cantidad de Links en toda la red.
Ejemplos: Entrada:
  camino Argentina,Zinedine Zidane
  camino Jeremy Irons,pejerrey
  camino handball,Napoleón Bonaparte
Salida:
  Argentina -> Francia -> Zinedine Zidane
  Costo: 2
  Jeremy Irons -> Reino Unido -> Patagonia argentina -> Río Grande (Tierra del Fuego) -> pejerrey
  Costo: 4
  No se encontro recorrido
"""


def camino(grafo, origen, destino):
	if origen not in grafo.obtenerVertices():
		return (None, "Vértice  " + origen + " no está en el grafo", None)
	if destino not in grafo.obtenerVertices():
		return (None, "Vértice  " + destino + " no está en el grafo", None)
	padre, orden, ultimo = bfs(grafo, origen, destino)
	if destino not in padre:
		return (None, "No se encontro recorrido", None)
	return (obtener_recorrido(padre, destino, orden[destino])[::-1], None, orden[destino])

"""
Diametro (★)
Comando: diametro.
Parámetros: ninguno.
Utilidad: permite obtener el diámetro de toda la red. Esto es, obtener el camino mínimo más grande de toda la red.
Nota: Puede haber más de uno de estos, pero todos tendrán el mismo largo.
Complejidad: Este comando debe ejecutar en O(P(P+L)).

Ejemplo: Entrada:
	diametro
Salida:
	Huésped (biología) -> Agente biológico patógeno -> Animalia -> Carlos Linneo -> Finlandia -> Unión Europea -> Robert Schuman -> Aristide Briand
	Costo: 7
"""


def bfs(grafo, origen, destino):
	q = deque()
	visitados = set()
	distancia = {}
	distancia[origen] = 0
	visitados.add(origen)
	padres = {}
	padres[origen] = None
	ultimo = None
	q.append(origen)
	while q:
		v = q.popleft()
		for w in grafo.adyacentes(v):
			if w not in visitados and w != v:
				padres[w] = v
				distancia[w] = distancia[v] + 1
				q.append(w)
				visitados.add(w)
				if destino and w == destino:
					return padres, distancia, ultimo
		ultimo = v
	return padres, distancia, ultimo


def diametro(grafo):
	max_min_dist = 0
	padres_max = None
	ult = None
	for v in grafo.obtenerVertices():
		padres, distancias, ultimo = bfs(grafo, v, None)
		for w in distancias:
			if distancias[w] > max_min_dist:
				max_min_dist = distancias[w]
				ult = ultimo
				padres_max = padres
	return (obtener_recorrido(padres_max, ult, max_min_dist)[::-1], None, max_min_dist)


"""
Navegacion por primer link (★)
Comando: navegación.
Parámetros: origen.
Utilidad: Se dice que si comenzamos en cualquier artículo de wikipedia, y navegamos únicamente utilizando el primer link,
eventualmente llegaremos al artículo de Filosofía. Por lo tanto, queremos implementar un comando que navegue usando el primer link
(de los que tengamos reportados) desde la página origen y navegando usando siempre el primer link. Debemos continuar accediendo
al primer link hasta que la página ya no tenga links, o bien hasta que hayamos llegado a 20 páginas.
Complejidad: este comando debe ejecutar en O(n).
Ejemplo: Entrada:
  navegacion Argentina
  navegacion Alemania
  navegacion queso
  navegacion Bolivia
  navegacion Brasil
Salida:

  Argentina -> polacos
  Alemania -> Derecho penal -> complicidad
  queso
  Bolivia -> Guerra del Acre -> Hevea brasiliensis -> Guerra del Acre -> Hevea brasiliensis -> Guerra del Acre -> Hevea brasiliensis -> Guerra del Acre -> Hevea brasiliensis -> Guerra del Acre -> Hevea brasiliensis -> Guerra del Acre -> Hevea brasiliensis -> Guerra del Acre -> Hevea brasiliensis -> Guerra del Acre -> Hevea brasiliensis -> Guerra del Acre -> Hevea brasiliensis -> Guerra del Acre -> Hevea brasiliensis
  Brasil -> Hermeto Pascoal -> Portugués brasileño -> caña de azúcar
"""


def navegacion_primer_link(grafo, v, contador, recorrido):
	if contador >= 20 or v == "Filosofía":
		return recorrido, contador
	ady = grafo.adyacentes(v)
	if len(ady) == 0:
		return recorrido, contador
	recorrido.append(ady[0])
	return navegacion_primer_link(grafo, ady[0], contador + 1, recorrido)


def navegacion(grafo, origen):
	contador = 0
	recorrido = [origen]
	recorrido, contador = navegacion_primer_link(grafo, origen, contador, recorrido)
	return (recorrido, None, None)

"""
Lectura a las 2 a.m. (★★)
Comando: lectura.
Parámetros: página1, página2, …, página_n.
Utilidad: Permite obtener un orden en el que es válido leer las páginas indicados. Para que un orden sea válido, si página_i tiene un link a página_j,
entonces es necesario primero leer página_j. Solo se debe tener en cuenta los artículos mencionados en los parámetros. Esto, por supuesto, puede implicar que
no podamos cumplir con lo pedido por encontrarnos con un ciclo.
Complejidad: Este comando debe ejecutar en O(n + L_n) siendo n la cantidad de páginas indicadas, y L_n la cantidad de links entre estas.
Ejemplo: Entrada:
  lectura Buenos Aires,Roma
  lectura Hockey sobre hielo,Roma,Japón,árbol,Guerra,Dios,universo,Himalaya,otoño
Salida:

  No existe forma de leer las paginas en orden
  otoño, Himalaya, universo, Dios, Guerra, árbol, Hockey sobre hielo, Japón, Roma
Importante: considerar lo indicado en el enunciado. Si quiero saber un orden válido para leer página1 y página2,
y hay un link de página1 a página2, un orden válido es página2 y luego página1.
"""


def lectura_(grafo, visitadas, actual, paginas, orden):
	visitadas.append(actual)
	for w in grafo.adyacentes(actual):
		if w in paginas:
			for i in grafo.adyacentes(w):
				if i in visitadas:
					visitadas.remove(actual)
					return False
			if not lectura_(grafo, visitadas, w, paginas, orden):
				visitadas.remove(actual)
				return False
	if actual not in orden:
		orden.append(actual)
	visitadas.remove(actual)
	return True


def lectura(grafo, *paginas):
	orden = deque()
	visitadas = []
	lista_lectura = []
	for v in paginas:
		if not lectura_(grafo, visitadas, v, paginas, orden):
			return (None, "No existe forma de leer las paginas en orden", None)
	while orden:
		pagina = orden.popleft()
		lista_lectura.append(pagina)
	return (lista_lectura, None, None)


"""
Coeficiente de Clustering (★★)
El Coeficiente de Clustering es una métrica que nos permite entender cuán agrupados se encuentran los vértices de un grafo.
Para explicarla de manera simplificada, es similar a plantear la proporción en la que se cumple al regla de transitividad:
Cuántos de mis adyacentes son adyacentes entre sí.
"""


def clustering_general(grafo, pagina):
	adyacentes = grafo.adyacentes(pagina)
	grado_salida = len(adyacentes)
	if pagina in adyacentes:
		grado_salida -= 1
	if grado_salida < 2:
		return 0.000
	aristas = 0
	for v in adyacentes:
		for w in adyacentes:
			if v != w and grafo.sonAdyacentes(v, w) and v != pagina and w != pagina:
				aristas += 1
	resultado = aristas / (grado_salida * (grado_salida - 1))
	return resultado


def clustering(grafo, pagina=None):
	if not pagina or pagina not in grafo.obtenerVertices():
		res = 0
		contador = 0
		for v in grafo.obtenerVertices():
			res += clustering_general(grafo, v)
			contador += 1
		return (round(res / contador, 3), None, None)
	else:
		return round(clustering_general(grafo, pagina), 3), None, None


"""
Comunidades (★★)
Comando: comunidad.
Parámetros: página.
Utilidad: permite obtener la comunidad dentro de la red a la que pertenezca la página pasada por parámetro. Para esto, utilizaremos el sencillo algoritmo de Label Propagation.
Ejemplo: Entrada:
  comunidad Chile
Salida: Dado que la salida puede ser muy grande adjuntamos una salida de ejemplo. No es requisito que la salida sea tal cual está (ni en orden ni los artículos),
y se tendrá consideración las diferencias que se puedan llegar a tener. La idea de implementar este comando es que tengan un primer acercamiento a un algoritmo
muy sencillo para detectar comunidades. En este caso, pueden ver que muchas páginas relacionadas a Chile y ciudades de Chile han quedado en la misma comunidad,
lo cual sería un resultado esperable.

A quién le interese este tema puede ver otro tipo de algoritmos, como por ejemplo el Algoritmo de Louvain.
"""


def comunidades(grafo, origen):
	labels = {}
	entradas = {}
	i = 0
	for v in grafo.obtenerVertices():
		entradas[v] = []
		labels[v] = i
		i = i + 1
		for w in grafo.adyacentes(v):
			if w != v:
				if w in entradas:
					entradas[w].append(v)
				else:
					entradas[w] = [v]
	N_it = 6
	for i in range(0, N_it-1):  # Recorro N_it iteraciones
		orden_vertices = grafo.obtenerVertices()
		random.shuffle(orden_vertices)
		for v in orden_vertices:  # Aplico label propagation por cada vertice
			frecuencias = {}
			max = None
			for w in entradas[v]:
				if labels[w] in frecuencias:
					frecuencias[labels[w]] += 1
				else:
					frecuencias[labels[w]] = 1
				if not max:
					max = labels[w]
				else:
					if frecuencias[labels[w]] > frecuencias[max]:
						max = labels[w]
			labels[v] = max  # Actualizo el label de v de acuerdo al label con más apariciones

	comunidad = set()
	for v in grafo.obtenerVertices():
		if labels[v] == labels[origen]:
			comunidad.add(v)
	return comunidad, None, None


"""
Conectividad (★★)
Comando: conectados.
Parámetros: página, la página que se le quiere obtener la conectividad.
Utilidad: nos muestra todos las páginas a los que podemos llegar desde la página pasado por parámetro y que, a su vez,
puedan también volver a dicha página.
Complejidad: Este comando debe ejecutar en O(P+L). Considerar que a todas las páginas a las que lleguemos también se
conectan entre sí, y con el tamaño del set de datos puede convenir guardar los resultados.

Ejemplo: Entrada:
  conectados Boca Juniors
  conectados Argentina
Salida: En ambos casos la CFC está compuesta por los mismos artículos. Dejamos acá un archivo con la salida esperada
(no necesariamente en ese orden, pero sí esos artículos), dado que la misma consta de 44764 de los 75000 artículos.
Es importante notar que la segunda consulta debería obtener un resultado en tiempo constante.
"""


def bfs_contador(grafo, origen, conectados):
	conectados_desde_origen = set()
	conectados_desde_origen.add(origen)
	visitados = set()
	visitados.add(origen)
	q = deque()
	q.append(origen)
	while q:
		v = q.popleft()
		for w in grafo.adyacentes(v):
			if w not in visitados:
				visitados.add(w)
				q.append(w)
				if w in conectados:
					conectados_desde_origen.add(w)
	return conectados_desde_origen


def llegan_a_actual(actual, entradas, conectados, pila):
	for w in entradas[actual]:
		pila.apilar(w)
	while not pila.estaVacia():
		v = pila.desapilar()
		for w in entradas[v]:
			if w not in conectados:
				conectados.add(w)
				pila.apilar(w)
	return conectados


def conectividad(grafo, origen):
	global cfc
	if cfc and (origen in cfc):
		return cfc, None, None
	entradas = {}
	for v in grafo.obtenerVertices():
		if v not in entradas:
			entradas[v] = []
		for w in grafo.adyacentes(v):
			if w in entradas:
				entradas[w].append(v)
			else:
				entradas[w] = [v]
	if len(entradas[origen]) == 0:
		conectados = set()
		conectados.add(origen)
	else:
		conectados = set()
		pila = Pila()
		llegan_a_actual(origen, entradas, conectados, pila)
	conectados_desde_origen = bfs_contador(grafo, origen, conectados)
	cfc = conectados_desde_origen
	return conectados_desde_origen, None, None


"""
Artículos más importantes (★★★)
Utlizaremos el algoritmo de PageRank para implementar este comando, dado que además fue pensado primordialmente para este escenario.

Comando: mas_importantes.
Parámetros: n, la cantidad de páginas más importantes a mostrar.
Utilidad: nos muestra las n páginas más centrales/importantes del mundo según el algoritmo de pagerank, ordenadas de mayor importancia a menor importancia.
Complejidad: Este comando debe ejecutar en O(K(P+L) + P log(n)), siendo KK la cantidad de iteraciones a realizar para llegar a la convergencia
(puede simplificarse a O(Plogn+L) (El término O(P logn) proviene de obtener los Top-n luego de haber aplicado el algoritmo).
Ejemplo: Entrada:
  mas_importantes 20
Salida:
Argentina, Estados Unidos, Buenos Aires, España, Francia, Provincias de la Argentina, Magnoliophyta, Alemania, México, Reino Unido, Europa, Italia, Brasil, Chile, Perú, Segunda Guerra Mundial, Uruguay, Inglaterra, Venezuela, Colombia
"""


def mas_importantes_(grafo, k, d, rank, N_iteraciones, largo, entradas):
	for v in grafo.obtenerVertices():
		rank[v] = (1 - d)/largo
	k_mas_importantes = []
	for _ in range(N_iteraciones):
		for v in grafo.obtenerVertices():
			sumatoria = 0
			if v not in entradas:
				entradas[v] = []
			for w in entradas[v]:
				len_adyacentes = len(grafo.adyacentes(w))
				if len_adyacentes == 0:
					len_adyacentes = 1
				sumatoria += rank[w]/len_adyacentes
			rank[v] = (1 - d)/largo + d * sumatoria

	contador = 0
	for v in grafo.obtenerVertices():
		if contador < k:
			heapq.heappush(k_mas_importantes, (rank[v], v))
			contador += 1
		else:
			if rank[v] > k_mas_importantes[0][0]:
				heapq.heappushpop(k_mas_importantes, (rank[v], v))
	listado_mas_importantes = []
	while k_mas_importantes:
		listado_mas_importantes.insert(0, heapq.heappop(k_mas_importantes)[1])
	return listado_mas_importantes


def mas_importantes(grafo, k):
	k = int(k)
	entradas = {}
	for v in grafo.obtenerVertices():
		for w in grafo.adyacentes(v):
			if w in entradas:
				entradas[w].append(v)
			else:
				entradas[w] = [v]
	d = 0.85
	N_iteraciones = 5
	rank = {}
	largo = grafo.cantidadVertices
	return (mas_importantes_(grafo, k, d, rank, N_iteraciones, largo, entradas), None, None)

"""
Ciclo de n artículos (★★★)
Comando: ciclo.
Parámetros: página y n.
Utilidad: permite obtener un ciclo de largo n que comience en la página indicada.
Complejidad: Este comando debe ejecutar en O(P**n), pero realizando una buena poda puede reducirse sustancialmente el tiempo de ejecución.
Ejemplo: Entrada:
  ciclo Jeremy Irons,4
  ciclo Polonia,20

Jeremy Irons -> The Silence of the Lambs (película) -> Dances with Wolves -> Premios Óscar -> Jeremy Irons

Polonia -> Unión Europea Occidental -> Unión Europea -> Idioma italiano -> Florencia -> Cannes -> Festival de Cannes -> Biarritz -> Pirineos Atlánticos -> Pirineos -> Alpes -> Dolomitas ->
  Marmolada -> Italia -> Pier Paolo Pasolini -> Roma -> Alfabeto fonético internacional -> Alfabeto Fonético Internacional -> Idioma ruso -> Moscú -> Polonia
"""


def ciclo_(grafo, origen, actual, contador, n, padres, visitados):
	if contador == n and actual == origen:
		recorrido = obtener_recorrido(padres, actual, n)[::-1]
		return recorrido
	if contador >= n and actual != origen:
		return None

	for w in grafo.adyacentes(actual):
		if w == origen and contador != n - 1:
			continue
		if w in visitados and w != origen:
			continue
		padres[w] = actual
		visitados.add(w)
		lista = ciclo_(grafo, origen, w, contador + 1, n, padres, visitados)
		if lista:
			return lista
		visitados.remove(w)
	return None


def ciclo(grafo, origen, n):
	n = int(n)
	padres = {}
	visitados = set()
	padres[origen] = None
	contador = 0
	recorrido = ciclo_(grafo, origen, origen, contador, n, padres, visitados)
	if recorrido:
		return recorrido, None, None
	else:
		return None, "No se encontro recorrido", None
