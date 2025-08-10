#!/bin/bash

# Compilar o Johnson
echo "Compilando Johnson..."
g++ -std=c++17 -O2 Johnson.cpp -o johnson
if [ $? -ne 0 ]; then
    echo "Erro ao compilar johnson.cpp"
    exit 1
fi

# Compilar o Floyd-Warshall
echo "Compilando Floyd-Warshall..."
g++ -std=c++17 -O2 Floyd.cpp -o floyd
if [ $? -ne 0 ]; then
    echo "Erro ao compilar floyd.cpp"
    exit 1
fi

# Executar Johnson
echo "Executando Johnson..."
./johnson

# Executar Floyd-Warshall
echo "Executando Floyd-Warshall..."
./floyd

echo "Execução concluída."
