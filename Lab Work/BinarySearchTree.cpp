#include <iostream>
#include <stack>
#include <queue>
using namespace std;

template <typename T> class BST {
private:
	class Node {
	public:
		T data;
		Node* left;
		Node* right;

		Node(int data, Node* left = nullptr, Node* right = nullptr) : data(data), left(left), right(right) {}
	};

	Node* root;

public:
	BST() : root(nullptr) {}
		
	~BST() { destructor(root); }

	void destructor(Node* curr) {
		if (!curr) return;

		destructor(curr->left);
		destructor(curr->right);

		delete curr;
		curr = nullptr;
	}

	BST(BST& tree) { copy(tree.getRoot()); }

	void copy(Node* curr) {
		if (!curr) return;

		insert(curr->data);
		copy(curr->left);
		copy(curr->right);
	}

	Node* getRoot() {
		return root;
	}

	bool insert(T data) {
		if (!root) {
			root = new Node(data, nullptr, nullptr);
			return true;
		}

		Node* curr = root;
		while (curr) {
			if (data == curr->data) return false;

			if (curr->data > data) {
				if (!curr->left) {
					curr->left = new Node(data, nullptr, nullptr);
					return true;
				}
				curr = curr->left;
			}
			else {
				if (!curr->right) {
					curr->right = new Node(data, nullptr, nullptr);
					return true;
				}
				curr = curr->right;
			}
		}
	}

	Node* searchI(T key) {
		Node* curr = root;

		while (curr) {
			if (curr->data == key) return curr;

			if (curr->data > key) curr = curr->left;
			else curr = curr->right;
		}

		return nullptr;
	}

	Node* searchR(T key) {
		return searchR(root, key);
	}

	Node* searchR(Node* curr, T key) {
		if (!curr) return nullptr;
		
		if (curr->data == key) return curr;

		return (curr->data > key) ? searchR(curr->left, key) : searchR(curr->right, key); 
	}

	void inorderR() {
		inorderR(root);
		cout << endl;
	}

	void inorderR(Node* curr) {
		if (!curr) return;

		inorderR(curr->left);
		cout << curr->data << " ";
		inorderR(curr->right);
	}

	void inorderI() {
		Node* curr = root;
		stack<Node*> st;

		while (curr || !st.empty()) {
			if (curr) {
				st.push(curr);
				curr = curr->left;
			}
			else {
				curr = st.top();
				st.pop();
				cout << curr->data << " ";
				curr = curr->right;
			}
		}
		cout << endl;
	}

	int countNodes() {
		return countNodes(root);
	}

	int countNodes(Node* curr) {
		if (!curr) return 0;

		return (countNodes(curr->left) + countNodes(curr->right) + 1);
	}

	int countLeaves() {
		return countLeaves(root);
	}

	int countLeaves(Node* curr) {
		if (!curr) return 0;

		if (!curr->left && !curr->right) return 1;

		return (countLeaves(curr->left) + countLeaves(curr->right));
	}

	void levelOrderI() {
		queue<Node*> queue;
		Node* curr = root;
		queue.push(root);

		while (!queue.empty()) {
			Node* front = queue.front();
			if (!front) return;

			cout << front->data << " ";
			if (front->left) queue.push(front->left);
			if (front->right) queue.push(front->right);
			queue.pop();
		}

		cout << endl;
	}
};

int main() {
	{
		BST<int> tree;
		tree.insert(10);
		tree.insert(7);
		tree.insert(9);
		tree.insert(5);
		tree.insert(30);
		tree.insert(40);
		tree.insert(50);
		tree.insert(60);

		tree.inorderR();

		BST<int> tree2 = tree;

		tree2.inorderR();

		cout << "Number of nodes in the tree: " << tree.countNodes() << endl;
		cout << "Number of leaves in the tree: " << tree.countLeaves() << endl;

		tree.inorderI();

		tree.levelOrderI();			// Uses BFS
	}
	_CrtDumpMemoryLeaks() ? cout << "Memory leaks found\n" : cout << "No memory leaks found\n";
}
