//
// Created by Maciej on 21.05.2025.
//

#include "kruskal.h"

#include <chrono>
#include <climits>
#include <iostream>
#include <ostream>
#include <tuple>
#include <tuple>
#include <tuple>
#include <tuple>
#include <tuple>
#include <tuple>

#include "../model/dsu.h"

std::tuple<int, Edge*, double> Kruskal::kruskalIncidenceMatrix(int** incidenceMatrix, const int V, const int E) {
      const auto start = std::chrono::high_resolution_clock::now();

      // Utworzenie listy krawędzi z macierzy incydencji
      auto* edges = new Edge[E];
      for (int e = 0; e < E; e++) {
            int u = -1, v = -1, w = -1;
            for (int i = 0; i < V; i++) {
                  if (incidenceMatrix[i][e] > 0) {
                        if (u == -1) {
                              u = i;
                              w = incidenceMatrix[i][e];
                        }
                        v = i;
                  }
            }
            edges[e] = Edge(u, v, w);
      }

      // Sortowanie krawędzi grafu według najmniejszej wagi
      heapSort(edges, E);

      // Inicjalizacja Disjoint Set Union
      const DSU dsu(V);

      // Inicjalizacja MST
      int mstCost = 0, mstEdgesCount = 0;
      auto* mstEdges = new Edge[V - 1];

      // Przejście przez wszystkie krawędzie grafu i dodanie odpowiednich krawędzi do MST
      for (int i = 0; i < E; i++) {
            const int u = edges[i].u, v = edges[i].v, w = edges[i].w;

            // Sprawdzenie, czy wierzchołki należą do dwóch różnych zbiorów i łączenie je w jeden
            if (dsu.findSet(u) != dsu.findSet(v)) {
                  dsu.unionSet(u, v);
                  mstCost += w;
                  mstEdges[mstEdgesCount++] = edges[i];
                  if (mstEdgesCount == V - 1)
                        break;
            }
      }

      const auto end = std::chrono::high_resolution_clock::now();
      const std::chrono::duration<double, std::milli> duration = end - start;

      delete[] edges;
      return std::make_tuple(mstCost, mstEdges, duration.count());
}

std::tuple<int, Edge*, double> Kruskal::kruskalSuccessorList(Node** successorList, const int V, const int E) {
      const auto start = std::chrono::high_resolution_clock::now();

      // Utworzenie listy krawędzi z listy następników
      int count = 0;
      auto* edges = new Edge[E];
      for (int u = 0; u < V; u++) {
            const Node* current = successorList[u];
            while (current != nullptr) {
                  const int v = current->v;
                  const int w = current->w;
                  // Dodawanie tylko jednej krawędzi dla pary wierzchołków
                  if (u < v) {
                        edges[count++] = Edge(u, v, w);
                  }

                  current = current->next;
            }
      }

      // Sortowanie krawędzi grafu według najmniejszej wagi
      heapSort(edges, E);

      // Inicjalizacja Disjoint Set Union
      const DSU dsu(V);

      // Inicjalizacja MST
      int mstCost = 0, mstEdgesCount = 0;
      auto* mstEdges = new Edge[V - 1];

      // Przejście przez wszystkie krawędzie grafu i dodanie odpowiednich krawędzi do MST
      for (int i = 0; i < E; i++) {
            const int u = edges[i].u, v = edges[i].v, w = edges[i].w;

            // Sprawdzenie, czy wierzchołki należą do dwóch różnych zbiorów i łączenie je w jeden
            if (dsu.findSet(u) != dsu.findSet(v)) {
                  dsu.unionSet(u, v);
                  mstCost += w;
                  mstEdges[mstEdgesCount++] = edges[i];
                  if (mstEdgesCount == V - 1)
                        break;
            }
      }

      const auto end = std::chrono::high_resolution_clock::now();
      const std::chrono::duration<double, std::milli> duration = end - start;

      delete[] edges;
      return std::make_tuple(mstCost, mstEdges, duration.count());
}

void Kruskal::heapSort(Edge *array, const int n) {
      // Budowanie kopca
      for (int i = n / 2 - 1; i >= 0; --i) {
            heapify(array, n, i);
      }

      // Sortowanie
      for (int i = n - 1; i >= 0; --i) {
            // Przeniesienia korzenia na koniec tablicy
            swap(array[0], array[i]);

            // Przywrócenie własności kopca
            heapify(array, i, 0);
      }
}

void Kruskal::heapify(Edge *array, const int n, const int i) {
      int largest = i;

      const int left = 2 * i + 1;
      const int right = 2 * i + 2;

      if (left < n && array[left].w > array[largest].w) {
            largest = left;
      }

      if (right < n && array[right].w > array[largest].w) {
            largest = right;
      }

      if (largest != i) {
            swap(array[i], array[largest]);
            heapify(array, n, largest);
      }
}

void Kruskal::swap(Edge &a, Edge &b) noexcept {
      const Edge temp = a;
      a = b;
      b = temp;
}


