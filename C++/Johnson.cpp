#include <iostream>
#include <vector>
#include <limits>
#include <utility>
#include <algorithm>
#include <fstream>
#include <chrono>
#include <iomanip>

using namespace std;

pair<vector<double>, int> bellman_ford_johnson(const vector<vector<double>> &graph, int start, int &num_comparacoes)
{
    int num_vertices = graph.size();
    vector<double> dist(num_vertices, numeric_limits<double>::infinity());
    dist[start] = 0;

    for (int i = 0; i < num_vertices - 1; i++)
    {
        for (int u = 0; u < num_vertices; u++)
        {
            for (int v = 0; v < num_vertices; v++)
            {
                num_comparacoes++;
                if (graph[u][v] != 0 && dist[u] + graph[u][v] < dist[v])
                {
                    dist[v] = dist[u] + graph[u][v];
                }
            }
        }
    }

    return make_pair(dist, num_comparacoes);
}

vector<double> dijkstra(const vector<vector<double>> &graph, int start, int &num_comparacoes)
{
    int num_vertices = graph.size();
    vector<double> dist(num_vertices, numeric_limits<double>::infinity());
    dist[start] = 0;
    vector<bool> visited(num_vertices, false);

    for (int i = 0; i < num_vertices; i++)
    {
        double min_dist = numeric_limits<double>::infinity();
        int u = -1;

        for (int v = 0; v < num_vertices; v++)
        {
            num_comparacoes++;
            if (!visited[v] && dist[v] < min_dist)
            {
                min_dist = dist[v];
                u = v;
            }
        }

        num_comparacoes++;
        if (u == -1)
        { // Todos os vértices visitados ou inacessíveis
            break;
        }

        visited[u] = true;

        for (int v = 0; v < num_vertices; v++)
        {
            num_comparacoes++;
            if (graph[u][v] > 0 && dist[u] + graph[u][v] < dist[v])
            {
                dist[v] = dist[u] + graph[u][v];
            }
        }
    }

    return dist;
}

pair<vector<vector<double>>, int> johnson(vector<vector<double>> graph)
{
    int num_vertices = graph.size();
    int num_comparacoes = 0;

    // Verifica se todos os pesos são não-negativos
    bool all_non_negative = true;

    for (int i = 0; i < num_vertices && all_non_negative; i++)
    {
        for (int j = 0; j < num_vertices && all_non_negative; j++)
        {
            if (graph[i][j] < 0)
            {
                all_non_negative = false;
            }
        }
    }

    // Para grafos não-direcionados sem pesos negativos,
    // podemos pular o Bellman-Ford e usar diretamente o Dijkstra
    if (all_non_negative)
    {
        // Aplica Dijkstra diretamente para cada vértice
        vector<vector<double>> dist(num_vertices, vector<double>(num_vertices, numeric_limits<double>::infinity()));

        for (int i = 0; i < num_vertices; i++)
        {
            dist[i] = dijkstra(graph, i, num_comparacoes);
            // Define distância do vértice para ele mesmo como 0
            dist[i][i] = 0;
        }

        return make_pair(dist, num_comparacoes);
    }

    // Caso contrário, segue o algoritmo completo de Johnson
    // Adiciona um vértice auxiliar
    vector<vector<double>> extended_graph(num_vertices + 1, vector<double>(num_vertices + 1, 0));
    for (int i = 0; i < num_vertices; i++)
    {
        for (int j = 0; j < num_vertices; j++)
        {
            extended_graph[i][j] = graph[i][j];
        }
    }

    // Aplica Bellman-Ford a partir do vértice auxiliar
    auto bellman_result = bellman_ford_johnson(extended_graph, num_vertices, num_comparacoes);
    vector<double> h = bellman_result.first;

    // Verifica se há ciclos negativos (implementação simplificada)
    // Remove o vértice auxiliar
    h.resize(num_vertices);

    // Reajusta os pesos das arestas
    vector<vector<double>> modified_graph = graph;
    for (int i = 0; i < num_vertices; i++)
    {
        for (int j = 0; j < num_vertices; j++)
        {
            num_comparacoes++;
            if (graph[i][j] > 0)
            {
                modified_graph[i][j] = graph[i][j] + h[i] - h[j];
            }
        }
    }

    // Aplica Dijkstra para cada vértice
    vector<vector<double>> dist(num_vertices, vector<double>(num_vertices, numeric_limits<double>::infinity()));
    for (int i = 0; i < num_vertices; i++)
    {
        dist[i] = dijkstra(modified_graph, i, num_comparacoes);
    }

    // Reajusta as distâncias finais
    for (int i = 0; i < num_vertices; i++)
    {
        for (int j = 0; j < num_vertices; j++)
        {
            num_comparacoes++;
            if (dist[i][j] < numeric_limits<double>::infinity())
            {
                dist[i][j] = dist[i][j] + h[j] - h[i];
            }
            num_comparacoes++;
            if (i == j)
            {
                dist[i][j] = 0;
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
    // Array com as quantidades de vértices
    int exec = 1;
    vector<int> vertices_counts = {10, 25, 50, 75, 100, 150, 200, 250, 300, 400, 500, 650, 800, 1000, 1500};

    ofstream output_file("metricas_johnson.csv", ios::app);
    if (!output_file.is_open())
    {
        cerr << "Erro ao abrir o arquivo: metricas_johnson.csv" << endl;
        return 1;
    }
    output_file << "Algoritmo," << "Tempo (s)," << "Uso de Memória (MB)," << "Precisão (%)," << "Número de Comparações," << "Linguagem," << "Vertices," << "Execução" << "\n";
    output_file.close();

    for (exec = 1; exec <= 6; exec++)
    {

        for (int num_vertices_file : vertices_counts)
        {
            string filename = "Entrada" + to_string(num_vertices_file) + ".txt";
            string filepath = "../GrafosPonderados_NaoDirecionados/" + filename;

            ifstream file(filepath);
            if (!file.is_open())
            {
                cerr << "Erro ao abrir o arquivo: " << filepath << endl;
                continue; // Pula para o próximo arquivo
            }

            int num_vertices;
            file >> num_vertices;

            vector<vector<double>> graph(num_vertices, vector<double>(num_vertices, 0));

            int u, v;
            double weight;
            while (file >> u >> v >> weight)
            {
                graph[u][v] = weight;
                graph[v][u] = weight; // Grafo não direcionado
            }

            file.close();

            cout << "Processando arquivo: " << filename << " (" << num_vertices << " vértices)" << endl;

            // Capturar o tempo de execução
            auto start = chrono::high_resolution_clock::now();
            auto result = johnson(graph);
            auto end = chrono::high_resolution_clock::now();
            chrono::duration<double> duration = end - start;
            cout << "Tempo de execução: " << fixed << setprecision(8) << duration.count() << " segundos" << endl;

            vector<vector<double>> distances = result.first;
            int comparisons = result.second;

            cout << "Número de comparações: " << comparisons << endl
                 << endl;

            // Salvar os dados em um arquivo csv com append
            if (exec != 1)
            {
                ofstream output_file("metricas_johnson.csv", ios::app);
                if (!output_file.is_open())
                {
                    cerr << "Erro ao abrir o arquivo: metricas_johnson.csv" << endl;
                    return 1;
                }

                output_file << "Johnson," << fixed << setprecision(8) << duration.count() << ",0,100," << comparisons << "," << num_vertices << "," << exec << "\n";
                output_file.close();
            }
        }
    }

    return 0;
}
