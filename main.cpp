#include <iostream>
#include <ostream>
#include <random>

#include "algorithm/bellman_ford.h"
#include "algorithm/dijkstra.h"
#include "algorithm/kruskal.h"
#include "algorithm/prim.h"
#include "model/graph.h"
#include "utility/config_parser.h"
#include "utility/graph_creator.h"
#include "utility/result_parser.h"

// Metoda uruchamiająca program w trybie symulacji
void launchSimulation(const Config& config);

// Metoda uruchamiająca program w trybie testowym
void launchTest(const Config& config);

int main() {
    // Wczytanie i wypisanie konfiguracji startowej
    const Config config = ConfigParser::readConfig("../config.cfg");
    ConfigParser::printConfig(config);

    // Wybranie odpowiedniego trybu działania programu
    if (config.isModeSimulation) {
        launchSimulation(config);
    } else {
        launchTest(config);
    }

    return 0;
}

void launchSimulation(const Config& config) {
    // Ustalenie problemu
    bool isMst = true;
    if (config.algorithms[0] == Algorithm::DIJKSTRA || config.algorithms[0] == Algorithm::BELLMAN_FORD) {
        isMst = false;
    }
    // Ilość symulacji
    constexpr int trails = 100;

    Graph* graph;
    double result = 0;
    ResultParser resultParser;

    // Symulacja i zapisanie wyników
    for (const auto ver : config.vertices) {
        for (const auto den : config.densities) {
            std::cout << "[INFO] Running for vertices: " << ver << " density: " << den << "..." << std::endl;
            for (int i = 0; i < trails; i++) {
                if (config.representation == Representation::LIST) {
                    if (isMst) {
                        graph = GraphCreator::generateGraph(ver, den, false);
                        const int V = graph->getVerticesNumber();
                        const int E = graph->getEdgesNumber();
                        Node** sList = graph->getSuccessorList();

                        for (const auto& alg : config.algorithms) {
                            if (alg == Algorithm::PRIM)
                                std::tie(std::ignore, std::ignore, result) = Prim::primSuccessorList(sList, V);
                            else if (alg == Algorithm::KRUSKAL)
                                std::tie(std::ignore, std::ignore, result) = Kruskal::kruskalSuccessorList(sList, V, E);
                            else continue;
                            resultParser.addScore(result, alg);
                        }
                        delete graph;
                    } else {
                        graph = GraphCreator::generateGraph(ver, den, true);
                        const int V = graph->getVerticesNumber();
                        Node** sList = graph->getSuccessorList();

                        std::random_device rd;
                        std::mt19937 gen(rd());
                        std::uniform_int_distribution<int> vertexDistribution(0, V - 1);
                        const int s = vertexDistribution(gen);

                        for (const auto& alg : config.algorithms) {
                            if (alg == Algorithm::DIJKSTRA)
                                std::tie(std::ignore, std::ignore, result) = Dijkstra::dijkstraSuccessorList(sList, V, s);
                            else if (alg == Algorithm::BELLMAN_FORD)
                                std::tie(std::ignore, std::ignore, result) = BellmanFord::bellmanFordSuccessorList(sList, V, s);
                            else continue;
                            resultParser.addScore(result, alg);
                        }
                        delete graph;
                    }
                } else if (config.representation == Representation::MATRIX) {
                    if (isMst) {
                        graph = GraphCreator::generateGraph(ver, den, false);
                        const int V = graph->getVerticesNumber();
                        const int E = graph->getEdgesNumber();
                        int** iMatrix = graph->getIncidenceMatrix();

                        for (const auto& alg : config.algorithms) {
                            if (alg == Algorithm::PRIM)
                                std::tie(std::ignore, std::ignore, result) = Prim::primIncidenceMatrix(iMatrix, V, E);
                            else if (alg == Algorithm::KRUSKAL)
                                std::tie(std::ignore, std::ignore, result) = Kruskal::kruskalIncidenceMatrix(iMatrix, V, E);
                            resultParser.addScore(result, alg);
                        }
                        delete graph;
                    } else {
                        graph = GraphCreator::generateGraph(ver, den, true);
                        const int V = graph->getVerticesNumber();
                        const int E = graph->getEdgesNumber();
                        int** iMatrix = graph->getIncidenceMatrix();

                        std::random_device rd;
                        std::mt19937 gen(rd());
                        std::uniform_int_distribution<int> vertexDistribution(0, V - 1);
                        const int s = vertexDistribution(gen);

                        for (const auto& alg : config.algorithms) {
                            if (alg == Algorithm::DIJKSTRA)
                                std::tie(std::ignore, std::ignore, result) = Dijkstra::dijkstraIncidenceMatrix(iMatrix, V, E, s);
                            else if (alg == Algorithm::BELLMAN_FORD)
                                std::tie(std::ignore, std::ignore, result) = BellmanFord::bellmanFordIncidenceMatrix(iMatrix, V, E, s);
                            resultParser.addScore(result, alg);
                        }
                        delete graph;
                    }
                }
            }

            resultParser.calculateAverageScores(ver, den);
        }
    }
    std::string suffix = isMst ? "_mst" : "_sp";
    suffix += config.representation == Representation::LIST ? "_list" : "_matrix";
    suffix += "_" + std::to_string(trails);
    resultParser.saveScoresToFile("results" + suffix + ".csv");

    std::cin.get();
}

void launchTest(const Config &config) {
    Graph* graph = nullptr;
    Edge* mstEdges = nullptr;
    int mstCost = -1;
    int* distance = nullptr;
    Node** paths = nullptr;
    double duration = 0.0;

    // Ustalenie problemu
    bool isMst = true;
    if (config.algorithms[0] == Algorithm::DIJKSTRA || config.algorithms[0] == Algorithm::BELLMAN_FORD) {
        isMst = false;
    }

    if (isMst) {
        if (config.isReadFromFile) {
            graph = GraphCreator::readGraphFromFile(config.testDataPath, false);
        } else {
            graph = GraphCreator::generateGraph(config.vertices[0], config.densities[0], false);
        }

        if (config.isGraphPrinting) {
            std::cout << "LIST REPRESENTATION:\n";
            graph->printList();
            std::cout << "\nMATRIX REPRESENTATION:\n";
            graph->printMatrix();
        }

        std::cout << "--------------------------------------------\n\n";

        if (graph == nullptr) {
            throw std::runtime_error("Cannot read graph from file");
        }
        const int V = graph->getVerticesNumber();
        const int E = graph->getEdgesNumber();
        int** iMatrix = graph->getIncidenceMatrix();
        Node** sList = graph->getSuccessorList();

        for (const auto& alg : config.algorithms) {
            if (alg == Algorithm::PRIM) {
                std::cout << "ALGORITHM: PRIM \nREPRESENTATION: LIST\n";
                std::tie(mstCost, mstEdges, duration) = Prim::primSuccessorList(sList, V);
                std::cout << "MST EDGES:\n";
                ResultParser::printMST(mstEdges, V);
                std::cout << "COST: " << mstCost << "\nDURATION: " << duration << "\n\n";

                std::cout << "ALGORITHM: PRIM \nREPRESENTATION: MATRIX\n";
                std::tie(mstCost, mstEdges, duration) = Prim::primIncidenceMatrix(iMatrix, V, E);
                std::cout << "MST EDGES:\n";
                ResultParser::printMST(mstEdges, V);
                std::cout << "COST: " << mstCost << "\nDURATION: " << duration << "\n\n";
            } else if (alg == Algorithm::KRUSKAL) {
                std::cout << "ALGORITHM: KRUSKAL \nREPRESENTATION: LIST\n";
                std::tie(mstCost, mstEdges, duration) = Kruskal::kruskalSuccessorList(sList, V, E);
                std::cout << "MST EDGES:\n";
                ResultParser::printMST(mstEdges, V);
                std::cout << "COST: " << mstCost << "\nDURATION: " << duration << "\n\n";

                std::cout << "ALGORITHM: KRUSKAL \nREPRESENTATION: MATRIX\n";
                std::tie(mstCost, mstEdges, duration) = Kruskal::kruskalIncidenceMatrix(iMatrix, V, E);
                std::cout << "MST EDGES:\n";
                ResultParser::printMST(mstEdges, V);
                std::cout << "COST: " << mstCost << "\nDURATION: " << duration << "\n\n";
            }
        }
    } else {
        if (config.isReadFromFile) {
            graph = GraphCreator::readGraphFromFile(config.testDataPath, true);
        } else {
            graph = GraphCreator::generateGraph(config.vertices[0], config.densities[0], true);
        }

        if (config.isGraphPrinting) {
            std::cout << "LIST REPRESENTATION:\n";
            graph->printList();
            std::cout << "\nMATRIX REPRESENTATION:\n";
            graph->printMatrix();
        }

        std::cout << "--------------------------------------------\n\n";

        if (graph == nullptr) {
            throw std::runtime_error("Cannot read graph from file");
        }
        const int V = graph->getVerticesNumber();
        const int E = graph->getEdgesNumber();
        int** iMatrix = graph->getIncidenceMatrix();
        Node** sList = graph->getSuccessorList();
        const int s = config.startVertex;

        for (const auto& alg : config.algorithms) {
            if (alg == Algorithm::DIJKSTRA) {
                std::cout << "ALGORITHM: DIJKSTRA \nREPRESENTATION: LIST\n";
                std::tie(distance, paths, duration) = Dijkstra::dijkstraSuccessorList(sList, V, s);
                std::cout << "PATHS:\n";
                ResultParser::printPath(distance, paths, V);
                std::cout << "DURATION: " << duration << "\n\n";

                std::cout << "ALGORITHM: DIJKSTRA \nREPRESENTATION: MATRIX\n";
                std::tie(distance, paths, duration) = Dijkstra::dijkstraIncidenceMatrix(iMatrix, V, E, s);
                std::cout << "PATHS:\n";
                ResultParser::printPath(distance, paths, V);
                std::cout << "DURATION: " << duration << "\n\n";
            } else if (alg == Algorithm::BELLMAN_FORD) {
                std::cout << "ALGORITHM: BELLMAN-FORD \nREPRESENTATION: LIST\n";
                std::tie(distance, paths, duration) = BellmanFord::bellmanFordSuccessorList(sList, V, s);
                std::cout << "PATHS:\n";
                ResultParser::printPath(distance, paths, V);
                std::cout << "DURATION: " << duration << "\n\n";

                std::cout << "ALGORITHM: BELLMAN-FORD \nREPRESENTATION: MATRIX\n";
                std::tie(distance, paths, duration) = BellmanFord::bellmanFordIncidenceMatrix(iMatrix, V, E, s);
                std::cout << "PATHS:\n";
                ResultParser::printPath(distance, paths, V);
                std::cout << "DURATION: " << duration << "\n\n";
            }
        }
    }
    std::cin.get();
}

