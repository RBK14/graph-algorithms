//
// Created by Maciej on 14.05.2025.
//

#ifndef GRAPH_H
#define GRAPH_H

struct Edge {
    int u;          // Numer wierzchołka początkowego
    int v;          // Numer wierzchołka docelowego
    int w;          // Waga krawędzi
};

struct Node {
    int v;          // Numer wierzchołka docelowego
    int w;          // Waga krawędzi
    Node* next;     // Wskaźnik do następnego sąsiada
};

class Graph {
    int V;                      // Liczba wierzchołków
    int E;                      // Liczba krawędzi
    int** incidenceMatrix;      // Macierz incydencji
    Node** successorList;       // Lista następników

public:
    // Konstruktor do inicjalizacji grafu posiadającego v wierzchołków i e krawędzi
    Graph(int V, int E);

    // Destruktor grafu usuwający macierz incydencji i listę następników
    ~Graph();

    // Metoda zwracająca liczbę wierzchołków
    [[nodiscard]] int getVerticesNumber() const;

    // Metoda zwracająca liczbę krawędzi
    [[nodiscard]] int getEdgesNumber() const;

    // Metoda zwracająca macierz incydencji
    [[nodiscard]] int** getIncidenceMatrix() const;

    // Metoda zwracająca listę następników
    [[nodiscard]] Node** getSuccessorList() const;

    // Metoda dodająca krawędź do grafu
    void insertEdge(int u, int v, int w, int i, bool directed) const;

    // Wypisanie grafu w postaci macierzy incydencji
    void printMatrix() const;

    // Wypisywanie grafu w postaci listy następników
    void printList() const;
};

#endif //GRAPH_H
