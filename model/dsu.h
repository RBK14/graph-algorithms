//
// Created by Maciej on 22.05.2025.
//

#ifndef DSU_H
#define DSU_H

class DSU {
    int* parent;
    int* rank;

public:
    // Konstruktor inicjalizujacy tablice parent i rank
    explicit DSU(int V);

    // Destruktor usuwający tablice parent i rank
    ~DSU();

    // Metoda znajdująca zbiór, w którym znajduje się dany wierzchołek
    [[nodiscard]] int findSet(int v) const;

    // Metoda łącząca dwa wierzchołki we wspólny zbiór
    void unionSet(int u, int v) const;
};

#endif //DSU_H
