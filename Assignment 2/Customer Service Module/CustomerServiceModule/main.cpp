#pragma once
#include <iostream>
#include <queue>

using namespace std;

// ===========================
//         Type Enum
// ===========================

enum Type { New = 1, Regular, Silver, Gold, Platinum };

// ===========================
//       DLL Node Class
// ===========================

template <typename T> class Node {
public:
	T data;
	Node<T>* prev;
	Node<T>* next;

	Node(T data, Node<T>* prev, Node<T>* next) : data(data), prev(prev), next(next) {}
};

// ===========================
//         User Class
// ===========================

class User {
	int userID;
	string userName;
	string userEmail;
	string userCountry;
	Type userType;
	static int currentUserID;

public:
	User(string userName, string userEmail, string userCountry, Type userType)
		: userID(++currentUserID), userName(userName), userEmail(userEmail), userCountry(userCountry), userType(userType) {}

	// Getters
	int getUserID() const {
		return userID;
	}

	string getUserName() const {
		return userName;
	}

	string getUserEmail() const {
		return userEmail;
	}

	string getUserCountry() const {
		return userCountry;
	}

	Type getUserType() const {
		return userType;
	}
};

// ===========================
//          AVL Class
// ===========================

template <typename T> class AVLTree {
private:
	class AVLNode {
	public:
		T data;
		int height;
		Node<User>* userAddress;
		AVLNode* left;
		AVLNode* right;

		AVLNode(T data, Node<User>* userAddress, AVLNode* left = nullptr, AVLNode* right = nullptr) : data(data), userAddress(userAddress), height(0), left(left), right(right) {}
	};

	AVLNode* root;

	AVLNode* insert(AVLNode*& curr, T data, Node<User>* userAddress) {
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

		return (key > root->data) ? getParent(root->right) : getParent(root->left);
	}

	void destructor(AVLNode*& root) {
		if (!root) return;

		destructor(root->left);
		destructor(root->right);

		delete root;
		root = nullptr;
	}

	Node<User>* getParentForInsertion(AVLNode* root, T key) {
		// If the tree is empty, there's no parent, return nullptr
		if (!root) return nullptr;

		// If the node is a leaf (no children), return this node's user address
		if (!root->left && !root->right) return root->userAddress;

		// Traverse to the right if the key is greater than the current node's data
		if (key > root->data) {
			// If there's no right child, return the current node's user address
			if (!root->right) return root->userAddress;
			return getParentForInsertion(root->right, key);
		}
		// Traverse to the left if the key is less than or equal to the current node's data
		else {
			// If there's no left child, return the current node's user address
			if (!root->left) return root->userAddress;
			return getParentForInsertion(root->left, key);
		}
	}

public:
	AVLTree() : root(nullptr) {}

	void insert(T data, Node<User>* userAddress) {
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

	Node<User>* getParentForInsertion(T key) {
		return getParentForInsertion(root, key);
	}

	~AVLTree() {
		destructor(root);
	}
};

// ===========================
//         DLL Class
// ===========================

template <typename T> class DLL {
private:

	Node<T>* head;
	Node<T>* tail;

public:
	class ListIterator {
	private:
		Node<T>* iter;

	public:
		ListIterator(Node<T>* iter = nullptr) : iter(iter) {}

		ListIterator& operator ++ () {
			(!iter || !iter->next) ? iter = nullptr : iter = iter->next;
			return *this;
		}

		ListIterator& operator -- () {
			(!iter || !iter->prev) ? iter = nullptr : iter = iter->prev;
			return *this;
		}

		T& operator *() {
			return iter->data;
		}

		bool operator == (const ListIterator& iter2) const {
			return (this->iter == iter2.iter);
		}

		bool operator != (const ListIterator& iter2) const {
			return (this->iter != iter2.iter);
		}
	};

	DLL() : head(nullptr), tail(nullptr) {}
	DLL(Node<T>* head, Node<T>* tail) : head(head), tail(tail) {}

	Node<T>* getHead() {
		return head;
	}

	Node<T>* getTail() {
		return tail;
	}

	Node<T>* insertHead(const T element) {
		if (!head) {
			head = tail = new Node<T>(element, nullptr, nullptr);
			return head;
		}

		Node<T>* node = new Node<T>(element, nullptr, head);
		head->prev = node;
		head = head->prev;
		if (!tail) tail = head;
		return node;
	}

	Node<T>* insertTail(T element) {
		if (!head) {
			head = tail = new Node<T>(element, nullptr, nullptr);
			return tail;
		}

		Node<T>* node = new Node<T>(element, tail, nullptr);
		tail->next = node;
		tail = tail->next;
		return node;
	}

	Node<T>* insertAfter(Node<T>*& node, T element) {
		if (node == nullptr || node == tail) {
			return insertTail(element);
		}

		Node<T>* newNode = new Node<T>(element, node, node->next);
		if (node->next) node->next->prev = newNode;
		node->next = newNode;
		return newNode;
	}

	Node<T>* insertBefore(Node<T>*& node, T element) {
		if (node == nullptr || node == head) {
			return insertHead(element);
		}

		Node<T>* newNode = new Node<T>(element, node->prev, node);
		if (node->prev) node->prev->next = newNode;
		node->prev = newNode;
		return newNode;
	}

	void deleteHead() {
		if (!head) return;
		Node<T>* temp = head;
		head = head->next;
		delete temp;
		if (head) head->prev = nullptr;
	}

	void deleteTail() {
		if (!tail) return;
		Node<T>* temp = tail;
		tail = tail->prev;
		delete temp;
		if (tail) tail->next = nullptr;
	}

	Node<T>*& removeTail() {
		Node<T>* temp = tail;
		tail = tail->prev;
		if (tail) tail->next = nullptr;
		else head = tail = nullptr;
		return temp;
	}

	void printForward() const {
		if (!head) return;
		for (auto iter = fbegin(); iter != fend(); ++iter) {
			(iter == ListIterator(tail)) ? cout << *iter : cout << *iter << " -> ";
		}
		cout << endl;
	}

	void printReverse() const {
		if (!head) return;
		for (auto iter = rbegin(); iter != rend(); --iter) {
			(iter == ListIterator(head)) ? cout << *iter : cout << *iter << " -> ";
		}
		cout << endl;
	}

	bool isEmpty() const {
		return head == nullptr;
	}

	int size() const {
		int size = 0;
		for (Node<T>* iter = head; iter; iter = iter->next, size++);
		return size;
	}

	typename ListIterator fbegin() const {
		return ListIterator(head);
	}

	typename ListIterator fend() const {
		return ListIterator(nullptr);
	}

	typename ListIterator rbegin() const {
		return ListIterator(tail);
	}

	typename ListIterator rend() const {
		return ListIterator(nullptr);
	}

	~DLL() {
		for (Node<T>* iter = head; iter;) {
			Node<T>* temp = iter->next;
			delete iter;
			iter = temp;
		}
		head = tail = nullptr;
	}

};

// ===========================
//       UserList Class
// ===========================

class UserList {
private:
	DLL<User> users;
	AVLTree<int> indexID;

public:
	UserList() : indexID(), users() {}

	void addUser(User user) {
		Node<User>* node = indexID.getParentForInsertion(user.getUserID());

		if (!node) {  // Check if node is nullptr
			auto userAddressInDLL = users.insertHead(user);  // Insert at head if no parent found
			indexID.insert(user.getUserID(), userAddressInDLL);
			return;
		}

		if (user.getUserID() > node->data.getUserID()) {
			auto userAddressInDLL = users.insertAfter(node, user);
			indexID.insert(user.getUserID(), userAddressInDLL);
		}
		else {
			auto userAddressInDLL = users.insertBefore(node, user);
			indexID.insert(user.getUserID(), userAddressInDLL);
		}
	}

	string returnUserType(Type type) {
		switch (type) {
		case 1:
			return "New";
			break;
		case 2:
			return "Regular";
			break; 
		case 3:
			return "Silver";
			break; 
		case 4:
			return "Gold";
			break; 
		case 5:
			return "Platinum";
			break;
		}
	}

	void printList() {
		cout << "------------------------------------------" << endl;
		for (auto it = users.fbegin(); it != users.fend(); ++it) {
			cout << "User Id: " << (*it).getUserID() << endl;
			cout << "Username: " << (*it).getUserName() << endl;
			cout << "Email: " << (*it).getUserEmail() << endl;
			cout << "Country: " << (*it).getUserCountry() << endl;
			cout << "Type: " << returnUserType((*it).getUserType()) << endl;
			cout << "------------------------------------------" << endl;
		}
	}
};

// static data
int User::currentUserID = 0;

int main() {
	{
		UserList userList;

		User user1("Muhaimin", "muhaimin1@gmail.com", "Pakistan", Platinum);
		userList.addUser(user1);

		User user2("Ahmed", "ahmed2@gmail.com", "Pakistan", Gold);
		userList.addUser(user2);

		User user3("Sara", "sara3@gmail.com", "Pakistan", Silver);
		userList.addUser(user3);

		User user4("Ali", "ali4@gmail.com", "Pakistan", Regular);
		userList.addUser(user4);

		User user5("Fatima", "fatima5@gmail.com", "Pakistan", New);
		userList.addUser(user5);

		userList.printList();
	} 
	
	_CrtDumpMemoryLeaks() ? cout << "Leaks Found\n" : cout << "No Leaks Found\n";

	return 0;
}