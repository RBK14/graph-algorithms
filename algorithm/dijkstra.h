//
// Created by Maciej on 23.05.2025.
//

#ifndef DIJKSTRA_H
#define DIJKSTRA_H

#include <tuple>

#include "../model/graph.h"

class Dijkstra {
public:
    // Metoda realizująca algorytm SP-Dijkstra dla grafu w postaci listy następników
    static std::tuple<int*, Node**, double> dijkstraSuccessorList(Node** successorList, int V, int s);

    // Metoda realizująca algorytm SP-Dijkstra dla grafu w postaci macierzy incydencji
    static std::tuple<int*, Node**, double> dijkstraIncidenceMatrix(int** incidenceMatrix, int V, int E, int s);

    // Metoda budująca najkrótsze ścieżki w postaci listy korzystając z tablicy rodzica
    static Node** buildPaths(const int* parent, int V);
};



#endif //DIJKSTRA_H
