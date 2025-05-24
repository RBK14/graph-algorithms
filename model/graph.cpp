//
// Created by Maciej on 14.05.2025.
//

#include "graph.h"

#include <iostream>
#include <stdexcept>

Graph::Graph(const int V, const int E) {
    this->V = V;
    this->E = E;

    // Inicjalizacja macierzy incydencji
    incidenceMatrix = new int*[V];
    for (int i = 0; i < V; i++) {
        incidenceMatrix[i] = new int[E]();
    }

    // Inicjalizacja listy następników
    successorList = new Node*[V];
    for (int i = 0; i < V; i++) {
        successorList[i] = nullptr;
    }
}

Graph::~Graph() {
    for (int i = 0; i < V; i++) {
        delete[] incidenceMatrix[i];
    }
    delete[] incidenceMatrix;

    for (int i = 0; i < V; i++) {
        const Node* current = successorList[i];
        while (current != nullptr) {
            const Node* toDelete = current;
            current = current->next;
            delete toDelete;
        }
    }
    delete[] successorList;
}

int Graph::getVerticesNumber() const {
    return V;
}

int Graph::getEdgesNumber() const {
    return E;
}

int **Graph::getIncidenceMatrix() const {
    return incidenceMatrix;
}

Node** Graph::getSuccessorList() const {
    return successorList;
}


void Graph::insertEdge(const int u, const int v, const int w, const int i, const bool directed) const {
    if (u >= V || v >= V || u < 0 || v < 0 || i < 0 || i >= E) {
        throw std::invalid_argument("Invalid edge");
    }

    if (!directed) {
        incidenceMatrix[u][i] = w;
        incidenceMatrix[v][i] = w;

        auto node = new Node{v, w, successorList[u]};
        successorList[u] = node;

        node = new Node{u, w, successorList[v]};
        successorList[v] = node;
    } else {
        incidenceMatrix[u][i] = w;
        incidenceMatrix[v][i] = -w;

        const auto node = new Node{v, w, successorList[u]};
        successorList[u] = node;
    }
}

void Graph::printMatrix() const {
    printf("    ");
    for (int i = 0; i < E; i++) {
        printf("%3d ", i);
    }
    printf("\n");

    for (int i = 0; i < V; i++) {
        printf("%3d ", i);
        for (int j = 0; j < E; j++) {
            printf("%3d ", incidenceMatrix[i][j]);
        }
        printf("\n");
    }
}

void Graph::printList() const {
    for (int i = 0; i < V; i++) {
        std::cout << i << ": ";
        const Node* current = successorList[i];
        while (current != nullptr) {
            std::cout << "[v:" << current->v << ", w:" << current->w << "] ";
            current = current->next;
        }
        std::cout << std::endl;
    }
}