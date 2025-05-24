//
// Created by Maciej on 23.05.2025.
//

#ifndef PRIM_H
#define PRIM_H

#include <string>
#include <tuple>

#include "../model/graph.h"

struct Edge;

class Prim {
public:
    // Metoda realizująca algorytm MST-Prim dla grafu w postaci listy następników
    static std::tuple<int, Edge*, double> primIncidenceMatrix(int** incidenceMatrix, int V, int E);

    // Metoda realizująca algorytm MST-Prim dla grafu w postaci listy następników
    static std::tuple<int, Edge*, double> primSuccessorList(Node** successorList, int V);
};

#endif //PRIM_H
