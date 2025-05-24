//
// Created by Maciej on 23.05.2025.
//

#include "dijkstra.h"

#include <chrono>
#include <iostream>

#include "../model/min_heap.h"

std::tuple<int*, Node**, double> Dijkstra::dijkstraSuccessorList(Node **successorList, const int V, const int s) {
    const auto start = std::chrono::high_resolution_clock::now();

    auto* distance = new int[V];
    auto* parent = new int[V];

    // Inicjalizacja kolejki priorytetowej
    MinHeap heap(V);

    // Inicjalizacja drogi do wierzchołków i dodanie wierzchołków do kolejki priorytetowej
    for (int v = 0; v < V; v++) {
        distance[v] = INT_MAX;
        parent[v] = -1;
        heap.insert(v, distance[v]);
    }

    // Inicjalizacja wierzchołka początkowego
    distance[s] = 0;
    heap.decreaseKey(s, distance[s]);

    while (!heap.isEmpty()) {
        // Wybranie wierzchołka o najmniejszej drodze
        const int u = heap.extractMin().v;

        const Node* current = successorList[u];
        while (current != nullptr) {
            const int v = current->v;
            const int w = current->w;

            // Relaksacja
            if (w > 0 && heap.isInHeap(v) && distance[u] != INT_MAX && distance[u] + w < distance[v]) {
                distance[v] = distance[u] + w;
                parent[v] = u;
                heap.decreaseKey(v, distance[v]);
            }

            current = current->next;
        }
    }

    const auto end = std::chrono::high_resolution_clock::now();
    const auto duration = std::chrono::duration<double, std::milli>(end - start);

    auto** path = buildPaths(parent, V);

    delete[] parent;

    return std::make_tuple(distance, path, duration.count());
}

std::tuple<int*, Node**, double> Dijkstra::dijkstraIncidenceMatrix(int** incidenceMatrix, const int V, const int E, const int s) {
    const auto start = std::chrono::high_resolution_clock::now();

    auto* distance = new int[V];
    auto* parent = new int[V];

    // Inicjalizacja kolejki priorytetowej
    MinHeap heap(V);

    // Inicjalizacja drogi do wierzchołków i dodanie wierzchołków do kolejki priorytetowej
    for (int v = 0; v < V; v++) {
        distance[v] = INT_MAX;
        parent[v] = -1;
        heap.insert(v, distance[v]);
    }

    // Inicjalizacja wierzchołka początkowego
    distance[s] = 0;
    heap.decreaseKey(s, distance[s]);

    while (!heap.isEmpty()) {
        // Wybranie wierzchołka o najmniejszej drodze
        const int u = heap.extractMin().v;

        for (int e = 0; e < E; e++) {
            if (incidenceMatrix[u][e] <= 0) continue;

            // Określenie wagi krawędzi
            const int w = incidenceMatrix[u][e];

            int v = -1;
            for (int k = 0; k < V; k++) {
                if (k != u && incidenceMatrix[k][e] == -w) {
                    v = k;
                    break;
                }
            }

            if (v == -1) continue;  // Nie znaleziono krawędzi

            // Relaksacja
            if (w > 0 && heap.isInHeap(v) && distance[u] != INT_MAX && distance[u] + w < distance[v]) {
                distance[v] = distance[u] + w;
                parent[v] = u;
                heap.decreaseKey(v, distance[v]);
            }
        }
    }

    const auto end = std::chrono::high_resolution_clock::now();
    const auto duration = std::chrono::duration<double, std::milli>(end - start);

    auto** path = buildPaths(parent, V);

    delete[] distance;
    delete[] parent;

    return std::make_tuple(distance, path, duration.count());
}

Node** Dijkstra::buildPaths(const int *parent, const int V) {
    auto** path = new Node*[V];

    for (int v = 0; v < V; v++) {
        path[v] = nullptr;

        int current = v;
        Node* head = nullptr;

        while (current != -1) {
            auto* newNode = new Node{current, 0 , head};
            head = newNode;
            current = parent[current];
        }

        path[v] = head;
    }

    return path;
}

