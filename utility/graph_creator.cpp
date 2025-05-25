//
// Created by Maciej on 16.05.2025.
//

#include "graph_creator.h"

#include <fstream>
#include <iostream>
#include <random>

Graph* GraphCreator::readGraphFromFile(const std::string& path, const bool directed) {
    std::ifstream file(path);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file");
    }

    // Wczytanie rozmiaru grafu
    int V, E;
    file >> E >> V;

    // Inicjalizacja grafu dla podanego rozmiaru
    auto* graph = new Graph(V, E);

    // Wczytanie krawędzi
    int u, v, w;
    for (int i = 0; i < E; i++) {
        file >> u >> v >> w;
        graph->insertEdge(u, v, w, i, directed);
    }

    file.close();
    return graph;
}

Graph* GraphCreator::generateGraph(const int V, const double density, const bool directed) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> vertexDistribution(0, V - 1);
    std::uniform_int_distribution<int> weightDistribution(1, 99);

    const int maxEdges = directed ? V * (V - 1) : V * (V - 1) / 2;
    // Sprawdzenie, czy dla podanej gęstości możemy zbudować graf spójny
    const int targetEdges = maxEdges * density > V - 1 ? static_cast<int>(maxEdges * density) : V - 1;

    // Inicjalizacja grafu
    auto* graph = new Graph(V, targetEdges);

    // Inicjalizacja macierzy sprawdzającej, czy krawędź istnieje
    auto** isEdgeExists = new bool*[V];
    for (int i = 0; i < V; i++) {
        isEdgeExists[i] = new bool[V];
        for (int j = 0; j < V; j++) {
            isEdgeExists[i][j] = false;
        }
    }

    // Łączymy losowo kolejne wierzchołki z już dołączonymi
    auto* isConnected = new bool[V];
    for (int i = 0; i < V; ++i) isConnected[i] = false;

    isConnected[0] = true;
    int edgesCount = 0;

    while (edgesCount < V - 1) {
        int u, v;

        do {
            u = vertexDistribution(gen);
        } while (!isConnected[u]);

        do {
            v = vertexDistribution(gen);
        } while (isConnected[v] || u == v);

        const int w = weightDistribution(gen);
        graph->insertEdge(u, v, w, edgesCount, directed);

        // Jeżeli graf jest nieskierowany dodajemy krawędź w obu kierunkach
        isEdgeExists[u][v] = true;
        if (!directed) {
            isEdgeExists[v][u] = true;
        }

        isConnected[v] = true;
        edgesCount++;
    }

    // Dodawanie pozostałych krawędzi
    while (edgesCount < targetEdges) {
        const int u = vertexDistribution(gen);
        const int v = vertexDistribution(gen);

        if (u != v && !isEdgeExists[u][v]) {
            const int w = weightDistribution(gen);
            graph->insertEdge(u, v, w, edgesCount, directed);

            // Jeżeli graf jest nieskierowany dodajemy krawędź w obu kierunkach
            isEdgeExists[u][v] = true;
            if (!directed) {
                isEdgeExists[v][u] = true;
            }

            edgesCount++;
        }
    }

    // Usuwanie macierzy pomocniczych
    for (int i = 0; i < V; i++)  delete[] isEdgeExists[i];
    delete[] isEdgeExists;
    delete[] isConnected;

    return graph;
}