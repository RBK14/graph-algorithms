//
// Created by Maciej on 21.05.2025.
//

#ifndef KRUSKAL_H
#define KRUSKAL_H
#include <string>
#include <tuple>

#include "../model/graph.h"

class Kruskal {
public:
    // Metoda realizująca algorytm MST-Kruskal dla grafu w postaci macierzy incydencji
    static std::tuple<int, Edge*, double> kruskalIncidenceMatrix(int **incidenceMatrix, int V, int E);

    // Metoda realizująca algorytm MST-Kruskal dla grafu w postaci list następników
    static std::tuple<int, Edge*, double> kruskalSuccessorList(Node **successorList, int V, int E);

private:
    // Metoda realizująca sortowanie tablicy za pomocą algorytmu Heap Sort
    static void heapSort(Edge* array, int n);

    // Metoda przywracająca własność kopca
    static void heapify(Edge* array, int n, int i);

    // Metoda pomocnicza do zamiany dwóch wartości w kopcu
    static void swap(Edge& a, Edge& b) noexcept;
};

#endif //KRUSKAL_H
