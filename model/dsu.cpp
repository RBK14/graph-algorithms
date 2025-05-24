//
// Created by Maciej on 22.05.2025.
//

#include "dsu.h"

DSU::DSU(const int V) {
    this->parent = new int[V];
    this->rank = new int[V];

    for (int i = 0; i < V; i++) {
        this->parent[i] = i;
        this->rank[i] = 1;
    }
}

DSU::~DSU() {
    delete[] parent;
    delete[] rank;
}

int DSU::findSet(const int v) const {
    if (v != parent[v])
        parent[v] = findSet(parent[v]);
    return parent[v];
}

void DSU::unionSet(const int u, const int v) const {
    int s1 = findSet(u), s2 = findSet(v);
    if (s1 != s2) {
        if (rank[s1] < rank[s2]) {
            parent[s1] = s2;
        } else if (rank[s1] > rank[s2]) {
            parent[s2] = s1;
        } else {
            parent[s2] = s1;
            rank[s1]++;
        }
    }
}



