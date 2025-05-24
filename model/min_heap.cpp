//
// Created by Maciej on 23.05.2025.
//

#include "min_heap.h"

MinHeap::MinHeap(const int capacity) {
    this->capacity = capacity;
    this->size = 0;
    heapArray = new MinHeapNode[capacity];
    position = new int[capacity];
}

MinHeap::~MinHeap() {
    delete[] heapArray;
    delete[] position;
}

void MinHeap::insert(const int v, const int key) {
    int i = size++;
    heapArray[i].v = v;
    heapArray[i].key = key;
    position[v] = i;

    while (i && heapArray[i].key < heapArray[(i - 1) / 2].key) {
        swapHeapNodes(i, (i - 1) / 2);
        i = (i - 1) / 2;
    }
}

MinHeapNode MinHeap::extractMin() {
    if (size == 0)
        return { -1, -1 };

    const MinHeapNode root = heapArray[0];
    heapArray[0] = heapArray[--size];
    position[heapArray[0].v] = 0;
    heapify(0);

    return root;
}

void MinHeap::decreaseKey(const int v, const int key) const {
    int i = position[v];
    heapArray[i].key = key;

    while (i && heapArray[i].key < heapArray[(i - 1) / 2].key) {
        swapHeapNodes(i, (i - 1) / 2);
        i = (i - 1) / 2;
    }
}

bool MinHeap::isInHeap(const int v) const {
    return position[v] < size;
}

bool MinHeap::isEmpty() const {
    return size == 0;
}


void MinHeap::heapify(const int i) {
    int smallest = i;
    const int left = 2 * i + 1;
    const int right = 2 * i + 2;

    if (left < size && heapArray[left].key < heapArray[smallest].key)
        smallest = left;

    if (right < size && heapArray[right].key < heapArray[smallest].key)
        smallest = right;

    if (smallest != i) {
        swapHeapNodes(i, smallest);
        heapify(smallest);
    }
}

void MinHeap::swapHeapNodes(const int i, const int j) const {
    position[heapArray[i].v] = j;
    position[heapArray[j].v] = i;

    const MinHeapNode temp = heapArray[i];
    heapArray[i] = heapArray[j];
    heapArray[j] = temp;
}

