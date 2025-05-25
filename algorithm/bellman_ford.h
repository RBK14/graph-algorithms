//
// Created by Maciej on 24.05.2025.
//

#ifndef BELLMAN_FORD_H
#define BELLMAN_FORD_H

#include <tuple>

#include "../model/graph.h"

class BellmanFord {
public:
    // Metoda realizująca algorytm SP-Bellman-Ford dla grafu w postaci macierzy incydencji
    static std::tuple<int*, Node**, double> bellmanFordIncidenceMatrix(int** incidenceMatrix, int V, int E, int s);

    // Metoda realizująca algorytm SP-Bellman-Ford dla grafu w postaci listy następników
    static std::tuple<int*, Node**, double> bellmanFordSuccessorList(Node** successorList, int V, int s);
};



#endif //BELLMAN_FORD_H
