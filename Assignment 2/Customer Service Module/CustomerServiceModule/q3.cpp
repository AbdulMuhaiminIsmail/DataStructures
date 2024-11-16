#pragma once
#include <queue>
#include <string>
#include <iostream>

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
	User(int userID, string userName, string userEmail, string userCountry, Type userType)
		: userID(userID), userName(userName), userEmail(userEmail), userCountry(userCountry), userType(userType) {}

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
//         SLL Class
// ===========================

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

	Node* getHead() const {
		return head;
	}

	Node* getTail() const {
		return tail;
	}

	void clear() {
		while (head) deleteStart();
	}

	void operator = (LinkedList<T>& list) {
		clear();
		for (Node* iter = list.getHead(); iter; iter = iter->next) {
			insertAtEnd(iter->value);
		}
	}
	
	void insertAtStart(T element) {
		head = new Node(element, head);
		if (!head->next) tail = head;
	}

	Node* insertAtEnd(T element) {
		Node* node = new Node(element, nullptr);

		if (!head) {
			head = tail = node;
			return node;
		}

		tail->next = node;
		tail = node;
		return node;
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

	bool isEmpty() const {
		return head == nullptr;
	}

	void deleteNode(int userId) {
		if (head == nullptr || tail == nullptr) return;

		if (userId == head->value->data.getUserID()) {
			deleteStart();
			return;
		}
		
		if (userId == tail->value->data.getUserID()) {
			deleteEnd();
			return;
		}

		for (Node*& iter = head; iter && iter->next; iter = iter->next) {
			if (iter->next->value->data.getUserID() == userId) {
				Node* node = iter->next;
				iter->next = node->next;
				delete node;
				node = nullptr;
				return;
			}
		}
	}

	void deleteStart() {
		if (head == nullptr) return;

		if (head == tail) {
			delete head;
			head = tail = nullptr;
			return;
		}

		Node* temp = head;
		head = head->next;
		delete temp;
	}

	void deleteEnd() {
		if (tail == nullptr) return;

		if (tail == head) {
			delete tail;
			head = tail = nullptr;
			return;
		}

		Node* iter = head;
		for (iter; iter->next != tail; iter = iter->next);
		delete tail;
		tail = iter;
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

// ===========================
//          AVL Class
// ===========================

template <typename T> class AVLTree {
private:
	class AVLNode {
	public:
		T data;
		int height;
		LinkedList<Node<User>*> userAddresses;
		AVLNode* left;
		AVLNode* right;

		AVLNode(T data, Node<User>* userAddress = nullptr, AVLNode* left = nullptr, AVLNode* right = nullptr) : data(data), height(0), left(left), right(right) {
			if (userAddress) userAddresses.insertAtEnd(userAddress);
		}
	};

	AVLNode* root;

	AVLNode* search(AVLNode* root, T key) {
		if (!root) return nullptr;

		if (root->data == key) return root;

		return (key > root->data) ? search(root->right, key) : search(root->left, key);
	}

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

		if (isXRoot) {
			root = Y;
		}
		else {
			AVLNode* parent = searchParent(X);
			if (parent->left == X) {
				parent->left = Y;
			}
			else {
				parent->right = Y;
			}
		}
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

		if (isXRoot) {
			root = Y;
		}
		else {
			AVLNode* parent = searchParent(X);
			if (parent->left == X) {
				parent->left = Y;
			}
			else {
				parent->right = Y;
			}
		}
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
				root->userAddresses = succ->userAddresses;
				succ->userAddresses.clear();
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

	AVLNode*& searchParent(AVLNode* child) {
		AVLNode* curr = root;

		while (curr) {
			if (curr->right == child || curr->left == child) {
				return curr;
			}

			(child->data > curr->data) ? curr = curr->right : curr = curr->left;
		}

		return curr;
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
		if (!root->left && !root->right) return root->userAddresses.getHead()->value;

		// Traverse to the right if the key is greater than the current node's data
		if (key > root->data) {
			// If there's no right child, return the current node's user address
			if (!root->right) return root->userAddresses.getHead()->value;
			return getParentForInsertion(root->right, key);
		}
		// Traverse to the left if the key is less than or equal to the current node's data
		else {
			// If there's no left child, return the current node's user address
			if (!root->left) return root->userAddresses.getHead()->value;
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

	AVLNode* search(T key) {
		return search(root, key);
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

	void deleteNode(Node<T>*& node) {
		if (!node) return;

		if (node == head) {
			deleteHead();
			return;
		}

		if (node == tail) {
			deleteTail();
			return;
		}

		if (node->prev) node->prev->next = node->next;
		if (node->next) node->next->prev = node->prev;
		node->next = node->prev = nullptr;

		delete node;
		node = nullptr;
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
	AVLTree<int> idIdx;
	AVLTree<string> nameIdx;
	AVLTree<string> emailIdx;
	AVLTree<string> countryGrpIdx;
	AVLTree<Type> typeGrpIdx;

	void updateIndices(User user, Node<User>* userAddressInDLL) {
		idIdx.insert(user.getUserID(), userAddressInDLL);
		nameIdx.insert(user.getUserName(), userAddressInDLL);
		emailIdx.insert(user.getUserEmail(), userAddressInDLL);
	}

	void updateGrpIndices(User user, Node<User>* userAddressInDLL) {
		// Updating country group index
		string userCountry = user.getUserCountry();
		auto countryAVLNode = countryGrpIdx.search(userCountry);

		if (!countryAVLNode) {
			countryGrpIdx.insert(userCountry, nullptr);
			countryAVLNode = countryGrpIdx.search(userCountry);
		}

		auto addressInCountryIdxList = countryAVLNode->userAddresses.insertAtEnd(userAddressInDLL);
		//user.setAddressInCountryIdxList(addressInCountryIdxList);

		// Updating type group index
		Type userType = user.getUserType();
		auto typeAVLNode = typeGrpIdx.search(userType);

		if (!typeAVLNode) {
			typeGrpIdx.insert(userType, nullptr);
			typeAVLNode = typeGrpIdx.search(userType);
		}

		auto addressInTypeIdxList = typeAVLNode->userAddresses.insertAtEnd(userAddressInDLL);
		//user.setAddressInTypeIdxList(addressInTypeIdxList);
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

	void delUserFromCountryIdx(User _user) {
		auto user = countryGrpIdx.search(_user.getUserCountry());
		if (!user) {
			return;
		}
		else {
			LinkedList<Node<User>*>& list = user->userAddresses;
			vector<int> userIdsToDelete;

			// Collect user IDs to delete
			for (auto iter = list.getHead(); iter; iter = iter->next) {
				userIdsToDelete.push_back(iter->value->data.getUserID());
			}

			// Delete users by ID
			for (int userId : userIdsToDelete) {
				list.deleteNode(userId);
			}

			cout << "The user " << _user.getUserName() << " has been deleted from " << _user.getUserCountry() << " index" << endl;
		}
	}

	void delUserFromTypeIdx(User _user) {
		auto user = typeGrpIdx.search(_user.getUserType());
		if (!user) {
			return;
		}
		else {
			LinkedList<Node<User>*>& list = user->userAddresses;
			vector<int> userIdsToDelete;

			// Collect user IDs to delete
			for (auto iter = list.getHead(); iter; iter = iter->next) {
				userIdsToDelete.push_back(iter->value->data.getUserID());
			}

			// Delete users by ID
			for (int userId : userIdsToDelete) {
				list.deleteNode(userId);
			}

			cout << "The user " << _user.getUserName() << " has been deleted from " << returnUserType(_user.getUserType()) << " index" << endl;
		}
	}

public:
	UserList() : idIdx(), users() {}

	void addUser(User user) {
		Node<User>* node = idIdx.getParentForInsertion(user.getUserID());
		Node<User>* userAddressInDLL = nullptr;

		if (!node) {
			userAddressInDLL = users.insertHead(user);
			updateIndices(user, userAddressInDLL);
			updateGrpIndices(user, userAddressInDLL);
			return;
		}

		(user.getUserID() > node->data.getUserID()) 
		? userAddressInDLL = users.insertAfter(node, user) 
		: userAddressInDLL = users.insertBefore(node, user);

		updateIndices(user, userAddressInDLL);
		updateGrpIndices(user, userAddressInDLL);
	}

	void searchUserById(int userID) {
		auto user = idIdx.search(userID);
		if (!user) {
			cout << "User with Id " << userID << " does not exist" << endl;
			return;
		}
		else {
			printUser(user->userAddresses.getHead()->value->data);	
		}
	}

	void searchUserByName(string userName) {
		auto user = nameIdx.search(userName);
		if (!user) {
			cout << "User with name " << userName << " does not exist" << endl;
			return;
		}
		else {
			printUser(user->userAddresses.getHead()->value->data);
		}
	}

	void searchUserByEmail(string userEmail) {
		auto user = emailIdx.search(userEmail);
		if (!user) {
			cout << "User with email " << userEmail << " does not exist" << endl;
			return;
		}
		else {
			printUser(user->userAddresses.getHead()->value->data);
		}
	}

	void searchUsersByCountry(string userCountry) {
		auto users = countryGrpIdx.search(userCountry);
		if (!users) {
			cout << "There is no user in the database who lives in " << userCountry << endl;
			return;
		}
		else {
			LinkedList<Node<User>*>& list = users->userAddresses;
			cout << "------------------------------------------" << endl;
			cout << "Users filtered by country " << userCountry << ": " << endl;
			int i = 1;
			for (auto iter = list.getHead(); iter; iter = iter->next, i++)
				cout << i << ". " << iter->value->data.getUserName() << endl;
			cout << "------------------------------------------" << endl;
		}
	}

	void searchUsersByType(Type userType) {
		auto users = typeGrpIdx.search(userType);
		if (!users) {
			cout << "There is no user in the database who lives in " << userType << endl;
			return;
		}
		else {
			LinkedList<Node<User>*>& list = users->userAddresses;
			cout << "------------------------------------------" << endl;
			cout << "Users filtered by type " << returnUserType(userType) << ": " << endl;
			int i = 1;
			for (auto iter = list.getHead(); iter; iter = iter->next, i++)
				cout << i << ". " << iter->value->data.getUserName() << endl;
			cout << "------------------------------------------" << endl;
		}
	}

	void delUserById(int userID) {
		auto user = idIdx.search(userID);
		if (!user) {
			cout << "User with userId " << userID << " does not exist" << endl;
			return;
		}
		else {
			Node<User>* userNode = user->userAddresses.getHead()->value;
			string userName = user->userAddresses.getHead()->value->data.getUserName();
			string userEmail = user->userAddresses.getHead()->value->data.getUserEmail();

			idIdx.remove(userID);
			nameIdx.remove(userName);
			emailIdx.remove(userEmail);
			delUserFromCountryIdx(userNode->data);
			delUserFromTypeIdx(userNode->data);
			users.deleteNode(userNode);

			cout << "User with userId " << userID << " has been sucessfully removed" << endl;
		}
	}

	void delUserByName(string userName) {
		auto user = nameIdx.search(userName);
		if (!user) {
			cout << "User with userName " << userName << " does not exist" << endl;
			return;
		}
		else {
			Node<User>* userNode = user->userAddresses.getHead()->value;
			int userID = user->userAddresses.getHead()->value->data.getUserID();
			string userEmail = user->userAddresses.getHead()->value->data.getUserEmail();

			idIdx.remove(userID);
			delUserFromCountryIdx(userNode->data);
			delUserFromTypeIdx(userNode->data);
			users.deleteNode(userNode);
			nameIdx.remove(userName);
			emailIdx.remove(userEmail);

			cout << "User with userName " << userName << " has been sucessfully removed" << endl;
		}
	}

	void delUserByEmail(string userEmail) {
		auto user = emailIdx.search(userEmail);
		if (!user) {
			cout << "User with userEmail " << userEmail << " does not exist" << endl;
			return;
		}
		else {
			Node<User>* userNode = user->userAddresses.getHead()->value;
			int userID = user->userAddresses.getHead()->value->data.getUserID();
			string userName = user->userAddresses.getHead()->value->data.getUserName();

			idIdx.remove(userID);
			nameIdx.remove(userName);
			emailIdx.remove(userEmail);
			delUserFromCountryIdx(userNode->data);
			delUserFromTypeIdx(userNode->data);
			users.deleteNode(userNode);

			cout << "User with userEmail " << userEmail << " has been sucessfully removed" << endl;
		}
	}

	void delUsersByCountry(string country) {
		auto users = countryGrpIdx.search(country);
		if (!users) {
			return;
		}
		else {
			LinkedList<Node<User>*>& list = users->userAddresses;
			vector<int> userIdsToDelete;

			// Collect user IDs to delete
			for (auto iter = list.getHead(); iter; iter = iter->next) {
				userIdsToDelete.push_back(iter->value->data.getUserID());
			}

			// Delete users by ID
			for (int userId : userIdsToDelete) {
				delUserById(userId);
			}

			cout << "All users living in " << country << " have been deleted" << endl;
		}
	}

	void delUsersByType(Type type) {
		auto users = typeGrpIdx.search(type);
		if (!users) {
			return;
		}
		else {
			LinkedList<Node<User>*>& list = users->userAddresses;
			vector<int> userIdsToDelete;

			// Collect user IDs to delete
			for (auto iter = list.getHead(); iter; iter = iter->next) {
				userIdsToDelete.push_back(iter->value->data.getUserID());
			}

			// Delete users by ID
			for (int userId : userIdsToDelete) {
				delUserById(userId);
			}

			cout << "All " << returnUserType(type) << " users have been deleted" << endl;
		}
	}

	void printUser(User user) {
		cout << "User Id: " << user.getUserID() << endl;
		cout << "Username: " << user.getUserName() << endl;
		cout << "Email: " << user.getUserEmail() << endl;
		cout << "Country: " << user.getUserCountry() << endl;
		cout << "Type: " << returnUserType(user.getUserType()) << endl;
		cout << "------------------------------------------" << endl;
	}

	void printUserList() {
		cout << "------------------------------------------" << endl;
		for (auto it = users.fbegin(); it != users.fend(); ++it) {
			printUser(*it);
		}
	}
};

// ===========================
//        Static Data
// ===========================

int User::currentUserID = 0;

// ===========================
//        Main Function
// ===========================

int main() {
	{
		UserList userList;

		User user1(2, "Muhaimin", "muhaimin1@gmail.com", "Pakistan", Platinum);
		userList.addUser(user1);

		User user2(5, "Ahmed", "ahmed2@gmail.com", "Canada", Gold);
		userList.addUser(user2);

		User user3(3, "Sara", "sara3@gmail.com", "Australia", Silver);
		userList.addUser(user3);

		User user4(1, "Ali", "ali4@gmail.com", "Canada", Regular);
		userList.addUser(user4);

		User user5(4, "Fatima", "fatima5@gmail.com", "Pakistan", New);
		userList.addUser(user5);

		userList.printUserList();

		/*userList.searchUserById(1);
		userList.searchUserByName("Ahmed");
		userList.searchUserByEmail("muhaimin1@gmail.com");
		userList.searchUsersByCountry("Pakistan");
		userList.searchUsersByCountry("Canada");
		userList.searchUsersByCountry("Australia");
		userList.searchUsersByCountry("Denmark");

		userList.searchUsersByType(Platinum);
		userList.searchUsersByType(Regular);
		userList.searchUsersByType(New);
		userList.searchUsersByType(Silver);
		userList.searchUsersByType(Gold);*/

		//userList.delUserById(2);
		//userList.delUserByName("Sara");
		//userList.delUserByEmail("fatima5@gmail.com");

		//userList.searchUserById(2);
		//userList.searchUserByName("Sara");
		//userList.searchUserByEmail("fatima5@gmail.com");

		userList.delUsersByCountry("Canada");
		userList.delUsersByCountry("Pakistan");
		userList.delUsersByCountry("Australia");
		userList.delUsersByType(Platinum);
		userList.delUsersByType(New);

		userList.searchUsersByCountry("Pakistan");
		userList.searchUsersByCountry("Canada");
		userList.searchUsersByCountry("Australia");
		userList.searchUsersByType(Platinum);
		userList.searchUsersByType(New);

		//userList.printUserList();
	}

	_CrtDumpMemoryLeaks() ? cout << "Leaks Found\n" : cout << "No Leaks Found\n";

	return 0;
}
