//#include <iostream>
//using namespace std;
//
//template <typename T> class Container {
//	T* elements;
//	int capacity;
//	int size;
//
//public:
//	Container(int capacity) : capacity(capacity), size(0) {
//		elements = new T[capacity];
//	}
//
//	~Container() {
//		if (elements != nullptr) {
//			delete[] elements;
//			elements = nullptr;
//		}
//	}
//
//	void addElement(const T& element) {
//		if (size == capacity) {
//			cout << "No more capacity in Container" << endl;
//			return;
//		}
//		
//		elements[size++] = element;
//	}
//
//	void displayElements() const {
//		if (size == 0) {
//			cout << "No Elements in the Container." << endl;
//			return;
//		}
//		
//		cout << "Elements in the container: \n";
//		for (int i = 0; i < size; i++) {
//			cout << elements[i] << endl;
//		}
//	}
//
//};
//
//int main() {
//	Container<int> container(5);
//	container.addElement(5);
//	container.addElement(6);
//	container.addElement(7);
//	container.addElement(8);
//	container.addElement(99);
//	container.displayElements();
//	container.addElement(5);
//	container.displayElements();
//
//
//	Container<string> containerString(5);
//	containerString.addElement("Abdul Muhaimin");
//	containerString.addElement("Abdul Rafay");
//	containerString.addElement("Abdul Nafay");
//	containerString.addElement("Abdul Momin");
//	containerString.addElement("Abdul Majeed");
//	containerString.displayElements();
//}