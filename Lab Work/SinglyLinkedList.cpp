#include <iostream>
using namespace std;

template <typename T> class LinkedList {
private:
	class Node {
	public:
		T value;
		Node* next;

		Node(T value, Node* next = nullptr) : value(value), next(next) {}
	};

	Node* head;
	Node* tail;

public:
	LinkedList() : head(nullptr), tail(nullptr) {}

	T getHead() const {
		return head->value;
	}

	T getTail() const {
		return tail->value;
	}
	
	void insertAtStart(T element) {
		head = new Node(element, head);
		if (!head->next) tail = head;
	}

	void insertAtEnd(T element) {
		Node* node = new Node(element, nullptr);

		if (!head) {
			head = tail = node;
			return;
		}

		tail->next = node;
		tail = node;
	}

	void printList() {
		Node* currentNode = head;
		while (currentNode) {
			if (currentNode->next) {
				cout << "(" << currentNode->value << ") -> ";
			}
			else {
				cout << "(" << currentNode->value << ")";
			}
			currentNode = currentNode->next;
		}
		cout << endl;
	}

	//O(1)
	bool isEmpty() const {
		return head == nullptr;
	}

	//O(n)
	bool findElement(T element) {
		for (Node* iter = head; iter; iter = iter->next) {
			if (iter->value == element) {
				return true;
			}
		}
		return false;
	}

	//O(1)
	void deleteStart() {
		if (head) {
			Node* temp = head;
			head = head->next;
			delete temp;
		}
	}

	void deleteEnd() {
		if (head == tail) {
			delete head;
			head = tail = nullptr;
			return;
		}

		if (tail) {
			Node* iter = head;
			for (iter; iter->next != tail; iter = iter->next);
			delete tail;
			tail = iter;
		}
	}

	Node* accessNode(int n) {
		Node* iter = head;
		for (int i = 0, iter; iter && i < n; i++, iter = iter->next);
		return iter;
	}
	
	//O(n)
	T findMax() {
		T max = head->value;
		for (Node* iter = head->next; iter; iter = iter->next) {
			if (iter->value > max) {
				max = iter->value;
			}
		}
		return max;
	}

	bool compare(LinkedList<T>& list) {
		for (Node* iter = head, *iter2 = list.head; iter && iter2; iter = iter->next, iter2 = iter2->next) {
			if (iter->value != iter2->value) {
				return false;
			}
		}
		return true;
	}

	void merge(LinkedList<T>& list) {
		tail->next = list.head;
	}

	~LinkedList() {
		Node* iter = head;
		while (iter) {
			Node* temp = iter->next;
			delete iter;
			iter = temp;
		}
	}
};