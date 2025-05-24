//
// Created by Maciej on 23.05.2025.
//

#ifndef MIN_HEAP_H
#define MIN_HEAP_H

struct MinHeapNode {
    int v;
    int key;
};

class MinHeap {
    MinHeapNode* heapArray;
    int* position;
    int size;
    int capacity;

public:
    explicit MinHeap(int capacity);
    ~MinHeap();
    void insert(int v, int key);
    MinHeapNode extractMin();
    void decreaseKey(int v, int key) const;
    [[nodiscard]] bool isInHeap(int v) const;
    [[nodiscard]] bool isEmpty() const;

private:
    void heapify(int i);
    void swapHeapNodes(int i, int j) const;
};

#endif //MIN_HEAP_H
