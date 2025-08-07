#include <vector>
#include <iostream>
#include <fstream>

using namespace std;

class Aresta
{
public:
    int destino;
    int peso;

    Aresta(int dest, int p) : destino(dest), peso(p) {}
};

class Vertice
{
public:
    int id;
    vector<Aresta> arestas;

    Vertice(int i) : id(i) {}

    void adicionarAresta(int destino, int peso)
    {
        arestas.push_back(Aresta(destino, peso));
    }

    void mostrarArestas() const
    {
        cout << "Vértice " << id << ": ";
        for (const auto &aresta : arestas)
        {
            cout << "[" << aresta.destino << "," << aresta.peso << "] ";
        }
        cout << endl;
    }
};

class Grafo
{
public:
    vector<Vertice> vertices;
    int numVertices;

    Grafo(int n) : numVertices(n)
    {
        for (int i = 0; i < n; i++)
        {
            vertices.push_back(Vertice(i));
        }
    }

    void adicionarAresta(int origem, int destino, int peso)
    {
        vertices[origem].adicionarAresta(destino, peso);
    }

    void mostrarGrafo()
    {
        for (const auto &vertice : vertices)
        {
            if (!vertice.arestas.empty())
            {
                vertice.mostrarArestas();
            }
        }
    }

    static Grafo lerGrafo(const string &nomeArquivo)
    {
        ifstream arquivo(nomeArquivo);

        if (!arquivo.is_open())
        {
            cerr << "Erro ao abrir o arquivo!" << endl;
            exit(1);
        }

        int numVertices;
        arquivo >> numVertices;

        Grafo grafo(numVertices);

        for (int i = 0; i < numVertices; i++)
        {
            for (int j = 0; j < numVertices; j++)
            {
                int peso;
                arquivo >> peso;

                // Se peso != 0, existe uma aresta
                if (peso != 0)
                {
                    grafo.adicionarAresta(i, j, peso);
                }
            }
        }

        arquivo.close();
        return grafo;
    }
};