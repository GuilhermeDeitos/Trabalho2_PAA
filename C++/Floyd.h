#ifndef FLOYD_H
#define FLOYD_H

#include <iostream>
#include <vector>
#include <string>
#include <limits>

using namespace std;
// --- Implementação do Algoritmo Floyd-Warshall ---

void floydWarshall(const Grafo &grafo)
{
    int numVertices = grafo.numVertices;
    if (numVertices == 0)
    {
        return;
    }

    const int INFINITO = numeric_limits<int>::max();
    vector<vector<int>> dist(numVertices, vector<int>(numVertices, INFINITO));

    // Passo 1: Inicializar a matriz de distâncias
    for (int i = 0; i < numVertices; ++i)
    {
        // A distância para si mesmo é 0
        dist[i][i] = 0;
        // Para as outras arestas, use o peso direto do grafo
        for (const auto &aresta : grafo.vertices[i].arestas)
        {
            dist[i][aresta.destino] = aresta.peso;
        }
    }

    int numComparacoes = 0; // Contador de comparações
    // Passo 2: Computar os caminhos mais curtos
    for (int k = 0; k < numVertices; ++k)
    {
        for (int i = 0; i < numVertices; ++i)
        {
            for (int j = 0; j < numVertices; ++j)
            {
                // Se k é um vértice intermediário no caminho de i para j,
                // e o caminho através de k é mais curto, atualize a distância.
                numComparacoes++; // Incrementa o contador de comparações
                if (dist[i][k] != INFINITO && dist[k][j] != INFINITO && dist[i][k] + dist[k][j] < dist[i][j])
                {
                    dist[i][j] = dist[i][k] + dist[k][j];
                }
            }
        }
    }

    cout << "Executando o algoritmo de Floyd-Warshall com " << numVertices << " vértices." << endl;
    cout << "Número de comparações realizadas: " << numComparacoes << endl;
    cout << "Matriz de distâncias mais curtas:" << endl;
    for (int i = 0; i < numVertices; ++i)
    {
        for (int j = 0; j < numVertices; ++j)
        {
            if (dist[i][j] == INFINITO)
            {
                cout << "INF ";
            }
            else
            {
                cout << dist[i][j] << " ";
            }
        }
        cout << endl;
    }

    // Verificar ciclos negativos
    for (int i = 0; i < numVertices; ++i)
    {
        if (dist[i][i] < 0)
        {
            cout << "O grafo contém um ciclo de peso negativo!" << endl;
            break;
        }
    }
}

#endif // FLOYD_H