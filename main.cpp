#include <iostream>
#include <ostream>

#include "algorithm/bellman_ford.h"
#include "algorithm/dijkstra.h"
#include "algorithm/kruskal.h"
#include "algorithm/prim.h"
#include "model/graph.h"
#include "utility/graph_creator.h"
#include "utility/result_parser.h"

int main() {
    // const Graph* g = GraphCreator::readGraphFromFile("../data/dane_droga_sk2.txt", true);
    const Graph* g = GraphCreator::generateGraph(280, 0.5, true);

    // g->printList();
    // g->printMatrix();

    Node** successorList = g->getSuccessorList();
    int** incidenceMatrix = g->getIncidenceMatrix();
    const int V = g->getVerticesNumber();
    const int E = g->getEdgesNumber();

    // auto [cost, mst, duration] = Kruskal::kruskalIncidenceMatrix(incidenceMatrix, V, E);
    // auto [cost, mst, duration] = Kruskal::kruskalSuccessorList(successorList, V, E);

    // auto [cost, mst, duration] = Prim::primSuccessorList(successorList, V);
    // auto [cost, mst, duration] = Prim::primIncidenceMatrix(incidenceMatrix, V, E);

    // ResultParser::printMST(mst, V);
    // std::cout << cost << " " << duration << std::endl;

    // auto [distance, path, duration] = Dijkstra::dijkstraSuccessorList(successorList, V, 0);
    // auto [distance, path, duration] = Dijkstra::dijkstraIncidenceMatrix(incidenceMatrix, V, E, 0);

    auto [distance, path, duration] = BellmanFord::bellmanFordIncidenceMatrix(incidenceMatrix, V, E, 0);
    // auto [distance, path, duration] = BellmanFord::bellmanFordSuccessorList(successorList, V, 0);

    ResultParser::printPath(distance, path, V);
    std::cout << std::endl << "duration: " << duration << std::endl;

    return 0;
}
