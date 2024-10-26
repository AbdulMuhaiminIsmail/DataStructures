#include <iostream>
#include <queue>
using namespace std;

template <typename T> class AVLTree {
private:
	class AVLNode {
	public:
		T data;
		int height;
		AVLNode* left;
		AVLNode* right;

		AVLNode(T data, AVLNode* left = nullptr, AVLNode* right = nullptr) : data(data), height(0), left(left), right(right) {}
	};

	AVLNode* root;

public:
	AVLTree() : root(nullptr) {}

	void insert(T data) {
		root = insert(root, data);
	}

	AVLNode* insert(AVLNode*& curr, T data) {
		if (!curr) {
			return new AVLNode(data, nullptr, nullptr);
		}

		(data < curr->data) ? curr->left = insert(curr->left, data) : curr->right = insert(curr->right, data);
		curr->height = max(height(curr->left), height(curr->right)) + 1;

		balance(curr);

		return curr;
	}

	int height(AVLNode* node) {
		return (!node) ? -1 : node->height;
	}

	int balanceFactor(AVLNode* node) {
		return (!node) ? 0 : (height(node->right) - height(node->left));
	}

	bool isLeftHeavy(AVLNode* node) {
		return (balanceFactor(node) < -1);
	}

	bool isRightHeavy(AVLNode* node) {
		return (balanceFactor(node) > 1);
	}

	AVLNode* searchParent(AVLNode* child) {
		AVLNode* curr = root;

		while (curr) {
			if (curr->right == child || curr->left == child) {
				return curr;
			}

			(child->data > curr->data) ? curr = curr->right : curr = curr->left;
		}

		return curr;
	}

	void leftRotate(AVLNode*& node) {
		bool isXRoot = (node == root);

		AVLNode* X = node;
		AVLNode* Y = node->right;
		AVLNode* temp = Y->left;

		Y->left = X;
		X->right = temp;

		X->height = max(height(X->left), height(X->right)) + 1;
		Y->height = max(height(Y->left), height(Y->right)) + 1;

		(isXRoot) ? root = Y : searchParent(X)->right = Y;
	}

	void rightRotate(AVLNode*& node) {
		bool isXRoot = (node == root);

		AVLNode* X = node;
		AVLNode* Y = node->left;
		AVLNode* temp = Y->right;

		Y->right = X;
		X->left = temp;

		X->height = max(height(X->left), height(X->right)) + 1;
		Y->height = max(height(Y->left), height(Y->right)) + 1;

		(isXRoot) ? root = Y : searchParent(X)->left = Y;
	}

	void leftRightRotate(AVLNode*& node) {
		AVLNode* X = node;
		AVLNode* Y = X->left;
		
		leftRotate(Y);
		rightRotate(X);
	}

	void rightLeftRotate(AVLNode*& node) {
		AVLNode* X = node;
		AVLNode* Y = X->right;

		rightRotate(Y);
		leftRotate(X);
	}

	void balance(AVLNode*& node) {
		if (isLeftHeavy(node)) {
			cout << node->data << " is left heavy" << endl;
			(balanceFactor(node->left) < 0) ? rightRotate(node) : leftRightRotate(node);
		}

		if (isRightHeavy(node)) {
			cout << node->data << " is right heavy" << endl;
			(balanceFactor(node->right) > 0) ? leftRotate(node) : rightLeftRotate(node);
		}
	}

	void levelOrderTraversal() {
		levelOrderTraversal(root);
	}

	void levelOrderTraversal(AVLNode* root) {
		if (!root) return;  // If the tree is empty, return

		std::queue<AVLNode*> q;
		q.push(root);

		while (!q.empty()) {
			AVLNode* current = q.front();
			q.pop();

			// Process the current node (print its value)
			std::cout << current->data << " ";

			// Enqueue the left child if it exists
			if (current->left) q.push(current->left);

			// Enqueue the right child if it exists
			if (current->right) q.push(current->right);
		}
		std::cout << std::endl;  // Newline after traversal
	}


};


int main() {
	AVLTree<int> tree;

	tree.insert(1);
	tree.levelOrderTraversal();
	tree.insert(2);
	tree.levelOrderTraversal();
	tree.insert(3);
	tree.levelOrderTraversal();
	tree.insert(4);
	tree.levelOrderTraversal();
	tree.insert(5);
	tree.levelOrderTraversal();
	tree.insert(6);
	tree.levelOrderTraversal();
	tree.insert(7);
	tree.levelOrderTraversal();
	tree.insert(8);
	tree.levelOrderTraversal();
	tree.insert(9);
	tree.levelOrderTraversal();
	tree.insert(10);
	tree.levelOrderTraversal();

}