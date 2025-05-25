//
// Created by Maciej on 25.05.2025.
//

#include "result_parser.h"

#include <fstream>
#include <iostream>
#include <numeric>
#include <ostream>

void ResultParser::printMST(const Edge* mstEdges, const int V) {
    for (int v = 0; v < V - 1; v++) {
        std::cout << mstEdges[v].u << " --- " << mstEdges[v].v << "; " << mstEdges[v].w << std::endl;
    }
    std::cout << std::endl;
}

void ResultParser::printPath(const int* distance, Node** path, const int V) {
    for (int v = 0; v < V; ++v) {
        std::cout << "path: ";

        const Node* current = path[v];
        while (current) {
            std::cout << current->v;
            if (current->next) std::cout << " -> ";
            current = current->next;
        }

        std::cout << "; distance: " << distance[v] << std::endl;
    }
}

void ResultParser::addScore(const double score, const Algorithm algorithm) {
    if (algorithm == Algorithm::PRIM) primScores.push_back(score);
    if (algorithm == Algorithm::KRUSKAL) kruskalScores.push_back(score);
    if (algorithm == Algorithm::DIJKSTRA) dijkstraScores.push_back(score);
    if (algorithm == Algorithm::BELLMAN_FORD) bellmanFordScores.push_back(score);
}

void ResultParser::calculateAverageScores(const int V, const double density) {
    std::map<Algorithm, double> scores;

    scores[Algorithm::PRIM] = calculateAverageScore(primScores);
    scores[Algorithm::KRUSKAL] = calculateAverageScore(kruskalScores);
    scores[Algorithm::DIJKSTRA] = calculateAverageScore(dijkstraScores);
    scores[Algorithm::BELLMAN_FORD] = calculateAverageScore(bellmanFordScores);

    averageScores[V][density] = scores;
}

void ResultParser::saveScoresToFile(const std::string &filename) {
    std::ofstream file("../files/results/" + filename);
    if (!file.is_open()) {
        std::cerr << "[ERROR] Cannot open the file: " << filename << std::endl;
        return;
    }

    file << "V;density;kruskal;prim;dijkstra;bellman_ford\n";

    for (const auto& [V, densityMap] : averageScores) {
        for (const auto& [density, algorithmScores] : densityMap) {
            file << V << ";" << density << ";"
                 << algorithmScores.at(Algorithm::PRIM) << ";"
                 << algorithmScores.at(Algorithm::KRUSKAL) << ";"
                 << algorithmScores.at(Algorithm::DIJKSTRA) << ";"
                 << algorithmScores.at(Algorithm::BELLMAN_FORD) << "\n";
        }
    }

    file.close();
    std::cout << "[SUCCESS] Results have been saved to file " << filename << std::endl;
}


double ResultParser::calculateAverageScore(const std::vector<double>& scores) {
    if (scores.empty()) return 0.0;

    const double sum = std::accumulate(scores.begin(), scores.end(), 0.0);
    return sum / static_cast<double>(scores.size());
}
