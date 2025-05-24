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
