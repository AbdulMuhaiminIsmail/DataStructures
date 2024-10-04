#include <iostream>
#include <vector>
using namespace std;

template <typename T> class CLL {
private:
	class Node {
	public:
		T value;
		Node* next;

		Node(T value, Node* next = nullptr) : value(value), next(next) {}
	};

	Node* tail;

public:
	CLL() : tail(nullptr) {}

	void insert(T element) {
		if (!tail) {
			Node* node = new Node(element);
			tail = node;
			tail->next = tail;
			node->next = tail;
			return;
		}

		Node* head = tail->next;
		tail->next = new Node(element, head);
		tail = tail->next;
	}

	void insertBefore(T v1, T v2) {
		Node *iter = tail->next;
		Node *head = iter;
		do {
			if (head->value == v2) {
				insert(v1);
				return;
			}

			if (iter->next->value == v2) {
				Node* temp = iter->next;
				iter->next = new Node(v1);
				iter->next->next = temp;
				return;
			}

			iter = iter->next;
		} while (iter != tail->next);
	}

	void update(T e1, T e2) {
		Node* iter = tail->next;
		do {
			if (iter->value == e1) iter->value = e2;
		} while (iter != tail->next);
	}

	//O(n)
	void printList(Node* curr) {
		cout << "(" << curr->value << ") -> ";
		
		if (curr->next == tail->next) return;

		printList(curr->next);

		cout << endl;
	}

	//O(1)
	bool isEmpty() const {
		return tail == nullptr;
	}

	//O(n)
	bool search(T element) {
		Node* iter = tail->next;
		do {
			if (iter->value == element) return true;
			iter = iter->next;
		} while (iter != tail->next);
		return false;
	}

	bool check() {
		vector<Node*> ptrs;
		Node* iter = tail->next;

		do {
			for (const auto& ptr : ptrs) {
				if (iter == ptr) return true;
			}
			ptrs.push_back(iter);
		} while (iter != tail->next);
	}

	Node* getHead() {
		return tail->next;
	}

	~CLL() {
		// Breaking the chain
		Node* curr = tail->next;
		tail->next = nullptr;

		while (curr) {
			Node* temp = curr;
			delete curr;
			curr = temp;
		}
	}
};

int main() {
	CLL<int> nums;
	nums.insert(1);
	nums.insert(2);
	nums.insert(3);
	nums.insert(4);
	nums.insert(5);

	nums.printList(nums.getHead());

	nums.insertBefore(6, 3);
	nums.printList(nums.getHead());
	
	nums.insertBefore(6, 1);
	nums.printList(nums.getHead());

	nums.check() ? cout << "CLL" : cout << "Not CLL";

	nums.search(6) ? cout << "Found" : cout << "Not Found";

	return 0;
}