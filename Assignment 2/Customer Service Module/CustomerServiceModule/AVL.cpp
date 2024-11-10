#pragma once
#include <iostream>
#include <queue>
#include "DLL.cpp"
#include "User.cpp"
using namespace std;

// ===========================
//          AVL Class
// ===========================

template <typename T> class AVLTree {
private:
	class AVLNode {
	public:
		T data;
		int height;	
		Node* userAddress;
		AVLNode* left;
		AVLNode* right;

		AVLNode(T data, Node* userAddress, AVLNode* left = nullptr, AVLNode* right = nullptr) : data(data), userAddress(userAddress), height(0), left(left), right(right) {}
	};

	AVLNode* root;

	AVLNode* insert(AVLNode*& curr, T data, Node* userAddress) {
		if (!curr) {
			return new AVLNode(data, userAddress, nullptr, nullptr);
		}

		(data < curr->data) ? curr->left = insert(curr->left, data, userAddress) : curr->right = insert(curr->right, data, userAddress);
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

	void leftRotate(AVLNode*& node) {
		bool isXRoot = (node == root);

		AVLNode* X = node;
		AVLNode* Y = node->right;
		AVLNode* temp = Y->left;

		Y->left = X;
		X->right = temp;

		X->height = max(height(X->left), height(X->right)) + 1;
		Y->height = max(height(Y->left), height(Y->right)) + 1;

		node = Y;
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

		node = Y;
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
			(balanceFactor(node->left) < 0) ? rightRotate(node) : leftRightRotate(node);
		}

		if (isRightHeavy(node)) {
			(balanceFactor(node->right) > 0) ? leftRotate(node) : rightLeftRotate(node);
		}
	}

	AVLNode* findSuccessor(AVLNode* root) {
		if (!root->right) return root->right;
		findSuccessor(root->right, 1);
	}

	AVLNode* findSuccessor(AVLNode* root, int x) {
		if (!root->left) return root;
		findSuccessor(root->left, 1);
	}

	void levelOrderTraversal(AVLNode* root) {
		if (!root) return;  // If the tree is empty, return

		std::queue<AVLNode*> q;
		q.push(root);

		while (!q.empty()) {
			int size = q.size();
			for (int i = 0; i < size; i++) {
				AVLNode* current = q.front();
				q.pop();

				// Process the current node (print its value)
				std::cout << current->data << " ";

				// Enqueue the left child if it exists
				if (current->left) q.push(current->left);

				// Enqueue the right child if it exists
				if (current->right) q.push(current->right);
			}
			std::cout << std::endl;  // Newline after each level traversal
		}
		std::cout << std::endl;  // Newline after complete traversal
	}

	AVLNode* remove(AVLNode*& root, T key) {
		// base case (key not found)
		if (!root) return root;

		// search for the key
		if (key < root->data) {
			root->left = remove(root->left, key);
		}
		else if (key > root->data) {
			root->right = remove(root->right, key);
		}
		else {
			if (root->left && root->right) {
				AVLNode* succ = findSuccessor(root);
				root->data = succ->data;
				root->right = remove(root->right, succ->data);
			}
			else {
				AVLNode* temp = root;
				(!root->left) ? root = root->right : root = root->left;
				delete temp;
			}
		}

		balance(root);
		return root;
	}

	AVLNode* getParent(AVLNode* root, T key) {
		if (!root) return nullptr;

		if (root->left == key || root->right == key) return root;

		(key > root->data) ? getParent(root->right) : getParent(root->left);
	}

	void destructor(AVLNode*& root) {
		if (!root) return;

		destructor(root->left);
		destructor(root->right);

		delete root;
		root = nullptr;
	}

	Node* getParentForInsertion(AVLNode* root, T key) {
		if (!root || (!root->left && !root->right)) return root->userAddress;

		return (key > root->data) ? getParentForInsertion(root->right) : getParentForInsertion(root->left);
	}

public:
	AVLTree() : root(nullptr) {}

	void insert(T data, Node* userAddress) {
		root = insert(root, data, userAddress);
	}

	void remove(T data) {
		remove(root, data);
	}

	void levelOrderTraversal() {
		levelOrderTraversal(root);
	}

	AVLNode* getParent(T key) {
		return getParent(root, key);
	}

	Node* getParentForInsertion(T key) {
		return getParentForInsertion(root, key);
	}

	~AVLTree() {
		destructor(root);
	}
};