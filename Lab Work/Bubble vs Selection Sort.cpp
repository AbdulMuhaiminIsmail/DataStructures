#include <iostream>
#include <time.h>
using namespace std;

class Application {
	int size;
	int* arr;

public:
	Application(int size) : size(size) {
		arr = new int[size];
	}

	int* bubbleSort(int*& arr) {
		for (int i = 0; i < size; i++) {
			for (int j = 0; j < size-1; j++) {
				if (arr[j + 1] < arr[j]) {
					swap(arr[j], arr[j + 1]);
				}
			}
		}
		return arr;
	}

	int* selectionSort(int*& arr) {
		for (int i = 0; i < size - 1; i++) {
			for (int j = i + 1; j < size; j++) {
				if (arr[j] < arr[i]) {
					swap(arr[j], arr[i]);
				}
			}
		}
		return arr;
	}

	void printArray(int* arr) {
		for (int i = 0; i < size; i++) {
			cout << arr[i] << "\t";
		}
		cout << endl;
	}

	void addElement(int element) {
		int* temp = new int[size + 1];
		for (int i = 0; i < size; i++) {
			temp[i] = arr[i];
		}
		temp[size] = element;

		delete[] arr;
		arr = temp;
		size++;
	}

	void process() {
		for (int i = 0; i < size; i++) {
			cout << "Enter element #" << i+1 << ": ";
			cin >> arr[i];
		}


		cout << "Original array: ";
		printArray(arr);

		int* temp = new int[size];
		for (int i = 0; i < size; i++) {
			temp[i] = arr[i];
		}

		clock_t start, end;
		double cpu_time_used;
		start = clock();
		int* bubbleTemp = bubbleSort(temp);
		end = clock();
		cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC * 100;
		cout << "Array after bubble sort: ";
		printArray(bubbleTemp);
		cout << "Time taken by bubble sort: ";
		cout << cpu_time_used << " ms" << endl;

		int* temp2 = new int[size];
		for (int i = 0; i < size; i++) {
			temp2[i] = arr[i];
		}

		clock_t start2, end2;
		double cpu_time_used2;
		start2 = clock();
		int* selectionTemp = selectionSort(temp2);
		end2 = clock();
		cpu_time_used2 = ((double)(end2 - start2)) / CLOCKS_PER_SEC * 100;
		cout << "Array after selection sort: ";
		printArray(selectionTemp);
		cout << "Time taken by selection sort: ";
		cout << cpu_time_used2 << " ms" << endl;

		addElement(-999);
		printArray(arr);

		delete[] temp;
		delete[] temp2;
	}
};

int main() {
	int size;
	cout << "Enter the size of the array: ";
	cin >> size;
	Application app(size);
	app.process();


}