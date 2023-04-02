
import matplotlib.pyplot as plt
from typing import List

from mpl_toolkits.mplot3d import Axes3D
import matplotlib.pyplot as plt
from sklearn.decomposition import PCA
from scipy.interpolate import interp1d

import numpy as np


colors = ['red', 'green', 'blue']

def plot_vectors(vectors: List[List[List[int]]], colors: List[str]):
    fig, ax = plt.subplots()
    unique_colors = np.unique(colors)
    color_map = {color: idx for idx, color in enumerate(unique_colors)}
    for idx, sub_list in enumerate(vectors):
        for vec in sub_list:
            x = [j for j in range(len(vec)) if vec[j] == 1]
            y = [i + idx for j in range(len(x))]
            c = color_map[colors[idx]]
            ax.scatter(x, y, c=c)
    ax.set_xticks(np.arange(len(vectors[0][0])))
    ax.set_yticks(np.arange(len(vectors)))
    ax.set_xticklabels([str(i) for i in range(len(vectors[0][0]))])
    ax.set_yticklabels([str(i) for i in range(len(vectors))])
    ax.tick_params(axis='x', labelrotation=90)
    plt.show()


def plot_clusters(clusters):
    colors = ['b', 'g', 'r', 'c', 'm', 'y', 'k'] # elenco dei colori
    num_colors = len(colors)
    num_clusters = len(clusters)
    
    # per ogni cluster, disegna tutti i suoi punti con lo stesso colore
    for i, cluster in enumerate(clusters):
        color = colors[i % num_colors] # seleziona un colore dalla lista dei colori
        for point in cluster:
            # disegna il punto con il colore selezionato
            plt.plot(point[0], point[1], color=color, marker='o')
            
    plt.show() # mostra il grafico


def plot_clusters_3d(clusters):
    # Calcola l'algoritmo PCA per ridurre lo spazio di dimensione superiore a 3 dimensioni
    pca = PCA(n_components=3)
    points = [point for cluster in clusters for point in cluster] # crea un elenco di tutti i punti nel dataset
    reduced_points = pca.fit_transform(points)
    
    # Inizializza il grafico 3D
    fig = plt.figure()
    ax = fig.add_subplot(111, projection='3d')
    
    # Disegna ogni cluster con un colore casuale
    for i, cluster in enumerate(clusters):
        color = np.random.rand(3,) # seleziona un colore casuale
        for point in cluster:
            index = points.index(point) # trova l'indice del punto nell'elenco originale
            # disegna il punto ridotto nello spazio 3D con il colore selezionato
            ax.scatter(reduced_points[index, 0], reduced_points[index, 1], reduced_points[index, 2], c=color, marker='o')
    
    plt.show() # mostra il grafico


def plot_clusters_3d_2(clusters):
    pca = PCA(n_components=3)
    points = [point for cluster in clusters for point in cluster] # crea un elenco di tutti i punti nel dataset
    reduced_points = pca.fit_transform(points)
    
    # Inizializza il grafico 3D
    fig = plt.figure()
    ax = fig.add_subplot(111, projection='3d')
    
    # Disegna ogni cluster con un colore differente
    colors = plt.cm.rainbow(np.linspace(0, 1, len(clusters))) # seleziona un elenco di colori differenti
    for i, cluster in enumerate(clusters):
        # riduci la dimensionalità dei punti nel cluster utilizzando PCA
        reduced_cluster = pca.transform(cluster)
        # Disegna i punti nel cluster nello spazio 3D con il colore selezionato
        ax.scatter(reduced_cluster[:, 0], reduced_cluster[:, 1], reduced_cluster[:, 2], c=[colors[i]], marker='o')
    
    plt.show() # mostra il grafico



def plot_clusters_2d(clusters):
    # Calcola l'algoritmo PCA per ridurre lo spazio di dimensione superiore a 2 dimensioni
    pca = PCA(n_components=2)
    points = [point for cluster in clusters for point in cluster] # crea un elenco di tutti i punti nel dataset
    reduced_points = pca.fit_transform(points)
    
    # Disegna ogni cluster con un colore differente
    colors = plt.cm.rainbow(np.linspace(0, 1, len(clusters))) # seleziona un elenco di colori differenti
    for i, cluster in enumerate(clusters):
        # riduci la dimensionalità dei punti nel cluster utilizzando PCA
        reduced_cluster = pca.transform(cluster)
        # Disegna i punti nel cluster nello spazio 2D con il colore selezionato
        plt.scatter(reduced_cluster[:, 0], reduced_cluster[:, 1], c=[colors[i]], marker='o')
    
    plt.show() # mostra il grafico


data = []
with open('../OutputCluster/test_2.txt', 'r') as f:
    vectors = []
    for line in f:
        if line.strip() == '===':
            data.append(vectors)
            vectors = []
        else:
            innerList = [float(x) for x in line.strip().split()]
            vectors.append(innerList)
    if vectors:
        data.append(vectors)

#plot_vectors(data,colors)
#plot_clusters(data)
#plot_clusters_3d(data)
#plot_clusters_3d_2(data)
plot_clusters_2d(data)
