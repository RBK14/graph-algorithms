//
// Created by Maciej on 24.05.2025.
//

#include "bellman_ford.h"

#include <chrono>

#include "dijkstra.h"

std::tuple<int*, Node**, double> BellmanFord::bellmanFordIncidenceMatrix(int** incidenceMatrix, const int V, const int E, const int s) {
    const auto start = std::chrono::high_resolution_clock::now();

    auto* distance = new int[V];
    auto* parent = new int[V];

    for (int v = 0; v < V; v++) {
        distance[v] = INT_MAX;
        parent[v] = -1;
    }

    distance[s] = 0;

    // Przejście przez krawędzie V-1 razy i sprawdzenie cykli ujemnych przy V-tej iteracji
    for (int i = 0; i < V; i++) {
        for (int e = 0; e < E; e++) {
            int u = -1, v = -1, w = 0;

            // Wyłuskanie wierzchołków i wag krawędzi
            for (int k = 0; k < V; k++) {
                if (incidenceMatrix[k][e] > 0) {
                    u = k;
                    w = incidenceMatrix[k][e];
                } else if (incidenceMatrix[k][e] < 0) {
                    v = k;
                }
            }

            if (u == -1 || v == -1) continue;

            // Sprawdzenie, czy istnieje tańsza ścieżka
            if (distance[u] != INT_MAX && distance[u] + w < distance[v]) {
                if (i < V - 1) {
                    // Relaksacja
                    distance[v] = distance[u] + w;
                    parent[v] = u;
                } else {
                    // Cykl ujemny (dla V-tej iteracji)
                    delete[] distance;
                    delete[] parent;
                    return std::make_tuple(nullptr, nullptr, -1.0);
                }
            }
        }
    }

    const auto end = std::chrono::high_resolution_clock::now();
    const auto duration = std::chrono::duration<double, std::milli>(end - start);

    auto** path = Dijkstra::buildPaths(parent, V);

    delete[] parent;

    return std::make_tuple(distance, path, duration.count());
}

std::tuple<int *, Node **, double> BellmanFord::bellmanFordSuccessorList(Node **successorList, const int V, const int s) {
    const auto start = std::chrono::high_resolution_clock::now();

    auto* distance = new int[V];
    auto* parent = new int[V];

    for (int v = 0; v < V; v++) {
        distance[v] = INT_MAX;
        parent[v] = -1;
    }

    distance[s] = 0;

    // Przejście przez krawędzie V-1 razy i sprawdzenie cykli ujemnych przy V-tej iteracji
    for (int i = 0; i < V; i++) {
        for (int u = 0; u < V; u++) {

            const Node* current = successorList[u];
            while (current != nullptr) {
                const int v = current->v;
                const int w = current->w;

                // Sprawdzenie, czy istnieje tańsza ścieżka
                if (distance[u] != INT_MAX && distance[u] + w < distance[v]) {
                    if (i < V - 1) {
                        // Relaksacja
                        distance[v] = distance[u] + w;
                        parent[v] = u;
                    } else {
                        // Cykl ujemny (dla V-tej iteracji)
                        delete[] distance;
                        delete[] parent;
                        return std::make_tuple(nullptr, nullptr, -1.0);
                    }
                }

                current = current->next;
            }
        }
    }

    const auto end = std::chrono::high_resolution_clock::now();
    const auto duration = std::chrono::duration<double, std::milli>(end - start);

    auto** path = Dijkstra::buildPaths(parent, V);

    delete[] parent;

    return std::make_tuple(distance, path, duration.count());
}

