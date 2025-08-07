#ifndef BELLMANFORD_H
#define BELLMANFORD_H

#include <iostream>
#include <vector>
#include <string>
#include <limits> 

using namespace std;


// --- Implementação do Algoritmo Bellman-Ford ---

void bellmanFord(const Grafo& grafo) {
    int numVertices = grafo.numVertices;
    if (numVertices == 0) {
        return;
    }

    int origem = 0; // Apenas vértice 0 como origem
    vector<int> distancias(numVertices, numeric_limits<int>::max());
    distancias[origem] = 0;
    long long comparacoesOrigem = 0;

    // Relaxamento das arestas (V-1 vezes)
    for (int k = 0; k < numVertices - 1; ++k) {
        for (int u = 0; u < numVertices; ++u) {
            // Se a distância para u não for infinita, explore suas arestas
            comparacoesOrigem++; // Comparação: distancias[u] != numeric_limits<int>::max()
            if (distancias[u] != numeric_limits<int>::max()) {
                for (const auto& aresta : grafo.vertices[u].arestas) {
                    int v = aresta.destino;
                    int peso = aresta.peso;
                    comparacoesOrigem++; // Comparação: distancias[u] + peso < distancias[v]
                    if (distancias[u] + peso < distancias[v]) {
                        distancias[v] = distancias[u] + peso;
                    }
                }
            }
        }
    }

    // Verificação de ciclos de peso negativo
    bool cicloNegativoEncontrado = false;
    for (int u = 0; u < numVertices; ++u) {
        comparacoesOrigem++; // Comparação: distancias[u] != numeric_limits<int>::max()
        if (distancias[u] != numeric_limits<int>::max()) {
            for (const auto& aresta : grafo.vertices[u].arestas) {
                int v = aresta.destino;
                int peso = aresta.peso;
                comparacoesOrigem++; // Comparação: distancias[u] + peso < distancias[v]
                if (distancias[u] + peso < distancias[v]) {
                    cicloNegativoEncontrado = true;
                    break;
                }
            }
        }
        if (cicloNegativoEncontrado) {
            break;
        }
    }

    if (cicloNegativoEncontrado) {
        cout << "O grafo contém um ciclo de peso negativo acessível a partir da origem " << origem << endl;
        return;
    }

    
    cout << "Comparações para origem " << origem << ": " << comparacoesOrigem << endl;
    cout << "Executando o algoritmo de Bellman-Ford com " << grafo.numVertices << " vértices." << endl;
    cout << "Distâncias a partir do vértice " << origem << ":" << endl;
    for (int i = 0; i < numVertices; ++i) {
        if (distancias[i] == numeric_limits<int>::max()) {
            cout << "INF ";
        } else {
            cout << distancias[i] << " ";
        }
    }
}


#endif // BELLMANFORD_H