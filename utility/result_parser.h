//
// Created by Maciej on 25.05.2025.
//

#ifndef RESULT_PARSER_H
#define RESULT_PARSER_H

#include <string>
#include <map>
#include <vector>

#include "../model/graph.h"

enum class Algorithm {
    PRIM,
    KRUSKAL,
    DIJKSTRA,
    BELLMAN_FORD
};

class ResultParser {
    std::map<int, std::map<double, std::map<Algorithm, double>>> averageScores;
    std::vector<double> kruskalScores;
    std::vector<double> primScores;
    std::vector<double> dijkstraScores;
    std::vector<double> bellmanFordScores;

public:
    // Metoda wypisująca listę krawędzi drzewa MST
    static void printMST(const Edge* mstEdges, int V);

    // Metoda wypisująca najkrótsze ścieżki z danego wierzchołka początkowego
    static void printPath(const int* distance, Node** path, int V);

    // Metoda dodająca wynik pomiaru
    void addScore(double score, Algorithm algorithm);

    // Metoda wyliczająca średni wynik dla liczby wierzchołków i gęstości
    void calculateAverageScores(int V, double density);

    // Metoda zapisująca wyniki do pliku
    void saveScoresToFile(const std::string& filename);

private:
    // Metoda wyliczająca średni wynik z pomiaru
    static double calculateAverageScore(const std::vector<double>& scores);
};

#endif //RESULT_PARSER_H
