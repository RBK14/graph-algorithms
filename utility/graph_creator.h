//
// Created by Maciej on 16.05.2025.
//

#ifndef FILE_READER_H
#define FILE_READER_H
#include <string>

#include "../model/graph.h"


class GraphCreator {
public:
    // Metoda wczytująca graf z pliku .txt
    static Graph* readGraphFromFile(const std::string& path, bool directed);

    // Metoda do tworzenia grafu o zadanej liczbie wierzchołków i gęstości
    static Graph* generateGraph(int V, double density, bool directed);
};

#endif //FILE_READER_H
