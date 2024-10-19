#include <algorithm>
#include <iostream>
#include <vector>

template <typename T> class Heap {
private:
  std::vector<T> heap;

  void heapifyDown(int index) {
    int largest = index;
    int left = 2 * index + 1;
    int right = 2 * index + 2;

    if (left < heap.size() && heap[left] > heap[largest]) largest = left;

    if (right < heap.size() && heap[right] > heap[largest]) largest = right;

    if (largest != index) {
      std::swap(heap[index], heap[largest]);
      heapifyDown(largest);
    }
  }

  void heapifyUp(int index) {
    int parent = (index - 1) / 2;

    if (index > 0 && heap[index] > heap[parent]) {
      std::swap(heap[index], heap[parent]);
      heapifyUp(parent);
    }
  }

public:
  void insert(T value) {
    heap.push_back(value);

    heapifyUp(heap.size() - 1);
  }

  T extractMax() {
    if (heap.size() == 0) throw std::runtime_error("Heap is empty");

    T root = heap[0];

    heap[0] = heap.back();
    heap.pop_back();

    heapifyDown(0);

    return root;
  }

  void printHeap() {
    for (T value : heap) {
      std::cout << value << " ";
    }
    std::cout << std::endl;
  }
};
