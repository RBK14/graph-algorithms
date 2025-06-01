//
// Created by Maciej on 25.05.2025.
//

#ifndef CONFIG_PARSER_H
#define CONFIG_PARSER_H

#include <vector>

#include "result_parser.h"

enum class Representation {
    MATRIX,
    LIST
};

struct Config {
    bool isModeSimulation;                  // Czy aplikacja ma być uruchomiona w trybie symulacji
    Representation representation;          // Reprezentacja grafy
    std::vector<Algorithm> algorithms;      // Algorytmy, które mają być uruchomione
    std::vector<int> vertices;              // Liczba wierzchołków w grafie
    std::vector<double> densities;          // Gęstość generowanego grafu
    bool isReadFromFile;                    // Czy graf ma być wczytany z pliku
    std::string testDataPath;               // Ścieżka do pliku, z którego ma być wczytany graf
    bool isGraphPrinting;                   // Czy graf ma być wypisywany po wczytaniu i po działaniu algorytmu
    int startVertex;                        // Wierzchołek startowy dla algorytmów SP
};

class ConfigParser {
public:
    // Metoda do wczytania konfiguracji aplikacji
    static Config readConfig(const std::string& configPath);

    // Metoda do wypisania konfiguracji
    static void printConfig(const Config& config);

private:
    static std::string toString(Representation repr);
    static std::string toString(Algorithm alg);
};

#endif //CONFIG_PARSER_H
