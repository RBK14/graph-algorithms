//
// Created by Maciej on 23.05.2025.
//

#include "prim.h"

#include <chrono>
#include <iostream>
#include <tuple>

#include <random>
#include <ratio>
#include <tuple>
#include <tuple>
#include <tuple>
#include <tuple>

#include "../model/min_heap.h"

std::tuple<int, Edge*, double> Prim::primSuccessorList(Node** successorList, const int V) {
    const auto start = std::chrono::high_resolution_clock::now();

    auto* parent = new int[V];
    auto* key = new int[V];

    // Inicjalizacja kolejki priorytetowej
    MinHeap heap(V);

    // Dodanie wierzchołków do kolejki priorytetowej
    for (int v =0; v < V; v++) {
        key[v] = INT_MAX;
        parent[v] = -1;
        heap.insert(v, key[v]);
    }

    // Inicjalizacja wierzchołka początkowego
    key[0] = 0;
    heap.decreaseKey(0, key[0]);

    // Inicjalizacja MST
    int mstCost = 0, mstEdgesCount = 0;
    auto* mstEdges = new Edge[V - 1];

    while (!heap.isEmpty()) {
        // Wybieranie najtańszego wierzchołka
        const int u = heap.extractMin().v;

        // Dodawanie krawędzi do MST (jeżeli nie jest to wierzchołek początkowy)
        if (parent[u] != -1) {
            mstEdges[mstEdgesCount++] = { parent[u], u, key[u] };
            mstCost += key[u];
        }

        // Sprawdzenie następników
        const Node* current = successorList[u];
        while (current != nullptr) {
            const int v = current->v;
            const int w = current->w;

            // Aktualizacja najtańszej krawędzi do następnika, który nie jest jeszcze w MST
            if (heap.isInHeap(v) && w < key[v]) {
                key[v] = w;
                parent[v] = u;
                heap.decreaseKey(v, key[v]);
            }

            current = current->next;
        }
    }

    const auto end = std::chrono::high_resolution_clock::now();
    const std::chrono::duration<double, std::milli> duration = end - start;

    delete[] parent;
    delete[] key;

    return std::make_tuple(mstCost, mstEdges, duration.count());
}

std::tuple<int, Edge*, double> Prim::primIncidenceMatrix(int **incidenceMatrix, const int V, const int E) {
    const auto start = std::chrono::high_resolution_clock::now();

    auto* parent = new int[V];
    auto* key = new int[V];

    // Inicjalizacja kolejki priorytetowej
    MinHeap heap(V);

    // Dodanie wierzchołków do kolejki priorytetowej
    for (int v =0; v < V; v++) {
        key[v] = INT_MAX;
        parent[v] = -1;
        heap.insert(v, key[v]);
    }

    // Inicjalizacja wierzchołka początkowego
    key[0] = 0;
    heap.decreaseKey(0, key[0]);

    // Inicjalizacja MST
    int mstCost = 0, mstEdgesCount = 0;
    auto* mstEdges = new Edge[V - 1];

    while (!heap.isEmpty()) {
        const int u = heap.extractMin().v;

        // Dodawanie krawędzi do MST (jeżeli nie jest to wierzchołek początkowy)
        if (parent[u] != -1) {
            mstEdges[mstEdgesCount++] = { parent[u], u, key[u] };
            mstCost += key[u];
        }

        // Przejście po wszystkich krawędziach
        for (int e = 0; e < E; e++) {
            if (incidenceMatrix[u][e] == 0) continue;

            // Określenie wagi krawędzi
            const int w = incidenceMatrix[u][e];

            // Wyszukanie drugiego wierzchołka krawędzi e
            int v = -1;
            for (int k = 0; k < V; k++) {
                if (k != u && incidenceMatrix[k][e] > 0) {
                    v = k;
                    break;
                }
            }

            // Nie znaleziono drugiego wierzchołka
            if (v == -1) continue;

            // Zapobieganie duplikowanie się krawędzi
            if (parent[u] == v || parent[v] == u) continue;

            // Aktualizacja krawędzi, jeżeli v nie jest jeszcze w MST i ta krawędź jest tańsza
            if (heap.isInHeap(v) && w < key[v]) {
                key[v] = w;
                parent[v] = u;
                heap.decreaseKey(v, key[v]);
            }
        }
    }

    const auto end = std::chrono::high_resolution_clock::now();
    const std::chrono::duration<double, std::milli> duration = end - start;

    delete[] parent;
    delete[] key;

    return std::make_tuple(mstCost, mstEdges, duration.count());
}

