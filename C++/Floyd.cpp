#include <iostream>
#include <vector>
#include <limits>
#include <utility>
#include <fstream>
#include <chrono>
#include <iomanip>

using namespace std;

pair<vector<vector<double>>, int> floyd_warshall(const vector<vector<double>> &graph)
{
    // Número de vértices no grafo
    int num_vertices = graph.size();

    unsigned long long int num_comparacoes = 0;

    // Inicializa a matriz de distâncias com os pesos das arestas
    vector<vector<double>> dist(num_vertices, vector<double>(num_vertices, numeric_limits<double>::infinity()));

    for (int i = 0; i < num_vertices; i++)
    {
        for (int j = 0; j < num_vertices; j++)
        {
            num_comparacoes++;
            if (i == j)
            {
                dist[i][j] = 0;
            }
            else if (graph[i][j] != 0)
            {
                dist[i][j] = graph[i][j];
            }
        }
    }

    // Aplica o algoritmo de Floyd-Warshall
    for (int k = 0; k < num_vertices; k++)
    {
        for (int i = 0; i < num_vertices; i++)
        {
            for (int j = 0; j < num_vertices; j++)
            {
                num_comparacoes++;
                if (dist[i][j] > dist[i][k] + dist[k][j])
                {
                    dist[i][j] = dist[i][k] + dist[k][j];
                }
            }
        }
    }

    return make_pair(dist, num_comparacoes);
}

// Função auxiliar para imprimir a matriz
void print_matrix(const vector<vector<double>> &matrix)
{
    for (const auto &row : matrix)
    {
        for (double val : row)
        {
            if (val == numeric_limits<double>::infinity())
            {
                cout << "INF\t";
            }
            else
            {
                cout << val << "\t";
            }
        }
        cout << endl;
    }
}

int main()
{
    int exec = 1;
    vector<int> vertex_counts = { 10, 25, 50, 75, 100, 150, 200, 250, 300, 400, 500, 650, 800, 1000, 1500};

    ofstream output_file("metricas_floyd.csv", ios::app);
    if (!output_file.is_open())
    {
        cerr << "Erro ao abrir o arquivo: metricas_floyd.csv" << endl;
        return 1;
    }
    output_file << "Algoritmo," << "Tempo (s)," << "Uso de Memória (MB)," << "Precisão (%)," << "Número de Comparações," << "Vertices," << "Execução" << "\n";
    output_file.close();

    for (exec = 1; exec <= 6; exec++)
    {
        for (int num_vertices : vertex_counts)
        {
            string filename = "Entrada" + to_string(num_vertices) + ".txt";
            string filepath = "../GrafosPonderados_NaoDirecionados/" + filename;

            ifstream file(filepath);
            if (!file.is_open())
            {
                cerr << "Erro ao abrir o arquivo: " << filepath << endl;
                continue;
            }

            int vertices;
            file >> vertices;

            vector<vector<double>> graph(vertices, vector<double>(vertices, 0));

            int u, v;
            double weight;
            while (file >> u >> v >> weight)
            {
                graph[u][v] = weight;
                graph[v][u] = weight; // Grafo não direcionado
            }

            file.close();

            auto start = chrono::high_resolution_clock::now();
            auto result = floyd_warshall(graph);
            auto end = chrono::high_resolution_clock::now();

            chrono::duration<double> duration = end - start;
            cout << "Arquivo: " << filename << " - Tempo de execução: " << fixed << setprecision(8) << duration.count() << " segundos" << endl;

            vector<vector<double>> distances = result.first;
            unsigned long long int comparisons = result.second;

            cout << "Número de comparações: " << comparisons << endl;

            if (exec != 1)
            {
                ofstream output_file("metricas_floyd.csv", ios::app);
                if (!output_file.is_open())
                {
                    cerr << "Erro ao abrir o arquivo: metricas_floyd.csv" << endl;
                    continue;
                }

                output_file << "Floyd," << fixed << setprecision(8) << duration.count() << ",0,100," << comparisons << "," << vertices << "," << exec << "\n";
                output_file.close();
            }
        }
    }
    return 0;
}
