#include <iostream>
#include <vector>
using namespace std;

template <typename T> class Heap {
private:
	int size;
	bool isMaxHeap;
	vector<T> heap;

	void heapifyUp(int index) {
		if (index == 0) return;

		int parentIndex = (index - 1) / 2;

		T child = heap[index];
		T parent = heap[parentIndex];

		if ((isMaxHeap && child > parent) || (!isMaxHeap && child < parent)) {
			swap(heap[index], heap[parentIndex]);
			heapifyUp(parentIndex);
		}
	}

	void heapifyDown(int index) {
		int leftChildIndex = (index * 2) + 1;
		
		if (leftChildIndex >= size) return;

		int rightChildIndex = (index * 2) + 2;
		int requiredIndex = index;

		if (leftChildIndex < size && (isMaxHeap && heap[leftChildIndex] > heap[requiredIndex]) ||
			(!isMaxHeap && heap[leftChildIndex] < heap[requiredIndex])) 
			requiredIndex = leftChildIndex;

		if (rightChildIndex < size && (isMaxHeap && heap[rightChildIndex] > heap[requiredIndex]) ||
			(!isMaxHeap && heap[rightChildIndex] < heap[requiredIndex]))
			requiredIndex = rightChildIndex;

		if (index != requiredIndex) {
			swap(heap[index], heap[requiredIndex]);
			heapifyDown(requiredIndex);
		}
	}

public:
	Heap(bool isMaxHeap) : isMaxHeap(isMaxHeap), size(0) {}

	~Heap() { 
		heap.clear();
		size = 0;
	}

	T at(int index) {
		if (index < size) return heap[index];
	}

	int getSize() {
		return size;
	}

	bool empty() {
		return (size == 0);
	}

	void print() {
		if (empty()) {
			cout << "Heap is empty" << endl;
			return;
		}

		cout << "{ ";
		for (const auto& elem : heap) {
			cout << elem << " ";
		}
		cout << "}" << endl;
	}

	void insert(T node) {
		size++;
		heap.push_back(node);
		if (size > 1) heapifyUp(size - 1);
	}

	void remove() {
		if (heap.empty()) return;

		--size;
		heap[0] = heap.back();
		heap.pop_back();

		if (size > 1) heapifyDown(0);
	}

	void heapSort(vector<T>& arr) {
		vector<T> temp = heap;

		heap.clear();
		size = 0;
		
		for (const auto& elem : arr) {
			insert(elem);
		}

		int origSize = size;

		for (int i = origSize - 1; i > 0; i--) {
			swap(heap[0], heap[i]);
			--size;
			heapifyDown(0);
		}

		arr = heap;
		heap = temp;
	}
};

int main() {
	Heap<int> heap(true);

	heap.insert(1);
	heap.insert(2);
	heap.insert(3);
	heap.insert(4);
	heap.insert(5);
	heap.insert(6);
	heap.insert(7);
	
	heap.print();

	vector<int> nums = {-5, 4, 30, -79, 69};
	heap.heapSort(nums);

	for (const auto& elem : nums) {
		cout << elem << " ";
	}

	heap.print();
}