#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include <string>
#include <limits>
#include <sstream>
#include "Grafo.h"
#include "Floyd.h"
#include "BellmanFord.h"
#include "Johnson.h"

using namespace std;

void medirDesempenho(string nomeAlgoritmo, const Grafo &grafo) // Assinatura alterada
{
    // Extrai os valores de dentro do grafo se precisar deles para outros algoritmos
    int numVertices = grafo.numVertices;
    int numArestas = 0;
    for (const auto &vertice : grafo.vertices)
    {
        numArestas += vertice.arestas.size();
    }

    if (nomeAlgoritmo == "Floyd-Warshall")
    {
        auto start = chrono::high_resolution_clock::now();
        floydWarshall(grafo); // Chamada corrigida para passar o grafo
        auto end = chrono::high_resolution_clock::now();
        auto duration = chrono::duration_cast<chrono::milliseconds>(end - start).count();
        cout << "Tempo de execução do algoritmo " << nomeAlgoritmo << ": " << duration << " ms" << endl;
    }
    else if (nomeAlgoritmo == "Johnson")
    {
        auto start = chrono::high_resolution_clock::now();
        // A função jhonson original ainda pode ser chamada assim
        // jhonson(numVertices, numArestas);
        auto end = chrono::high_resolution_clock::now();
        auto duration = chrono::duration_cast<chrono::milliseconds>(end - start).count();
        cout << "Tempo de execução do algoritmo " << nomeAlgoritmo << ": " << duration << " ms" << endl;
    }
    else if (nomeAlgoritmo == "Bellman-Ford")
    {
        auto start = chrono::high_resolution_clock::now();
        bellmanFord(grafo); // Chamada corrigida para passar o grafo
        auto end = chrono::high_resolution_clock::now();
        auto duration = chrono::duration_cast<chrono::milliseconds>(end - start).count();
        cout << "Tempo de execução do algoritmo " << nomeAlgoritmo << ": " << duration << " ms" << endl;
    }
    else
    {
        cout << "Algoritmo desconhecido!" << endl;
        return;
    }
}

int main()
{
    // Lê o grafo do arquivo
    vector<string> arquivos = {
        "../../GrafosPonderados_NaoDirecionados/Entrada10.txt",
        "../../GrafosPonderados_NaoDirecionados/Entrada25.txt"
    };

    // Medir desempenho dos algoritmos
    cout << "Medição de desempenho dos algoritmos:" << endl;
    for (const string &arquivo : arquivos)
    {
        cout << "Arquivo: " << arquivo << endl;
        Grafo grafo = Grafo::lerGrafo(arquivo);
        int numVertices = grafo.numVertices;
        int numArestas = 0;

        for (const auto &vertice : grafo.vertices)
        {
            numArestas += vertice.arestas.size();
        }

        //medirDesempenho("Floyd-Warshall", grafo);
        // medirDesempenho("Johnson", numVertices, numArestas);
        medirDesempenho("Bellman-Ford", grafo);
        cout << endl;
    }

    return 0;
}