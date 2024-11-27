#include <list>
#include <queue>
#include <string>
#include <conio.h>
#include <iomanip>
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
    User() {}
    User(int userID, string userName, string userEmail, string userCountry, Type userType)
        : userID(userID), userName(userName), userEmail(userEmail), userCountry(userCountry), userType(userType) {}

    User(string userName, string userEmail, string userCountry, Type userType)
        : userID(++currentUserID), userName(userName), userEmail(userEmail), userCountry(userCountry), userType(userType) {}

    // Getters
    static int getCurrentUserId() {
        return currentUserID;
    }

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

    void deleteComplaint(int complaintId) {
        if (head == nullptr || tail == nullptr) return;

        if (complaintId == head->value->getComplaintId()) {
            deleteStart();
            return;
        }

        if (complaintId == tail->value->getComplaintId()) {
            deleteEnd();
            return;
        }

        for (Node*& iter = head; iter && iter->next; iter = iter->next) {
            if (iter->next->value->getUserId() == complaintId) {
                Node* node = iter->next;
                iter->next = node->next;
                delete node;
                node = nullptr;
                return;
            }
        }
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
class Complaint;

template <typename T> class AVLTree {
private:
    class AVLNode {
    public:
        T data;
        int height;
        LinkedList<Node<User>*> userAddresses;
        LinkedList<Complaint*> complaintAddresses;
        AVLNode* left;
        AVLNode* right;

        AVLNode(T data, Node<User>* userAddress = nullptr, Complaint* complaintAddress = nullptr, AVLNode* left = nullptr, AVLNode* right = nullptr) : data(data), height(0), left(left), right(right) {
            if (userAddress) userAddresses.insertAtEnd(userAddress);
            if (complaintAddress) complaintAddresses.insertAtEnd(complaintAddress);
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
//       HashMap Class
// ===========================

template <typename KeyType, typename ValueType> class HashMap {
private:
    struct Node {
        KeyType key;
        ValueType value;
        Node(pair<KeyType, ValueType> kvPair) : key(kvPair.first), value(kvPair.second) {}
    };

    bool grpFlag;
    int collisionMethod;
    vector<list<Node*>> hashMap;

public:
    HashMap(int tableSize, int collisionMethod, bool grpFlag = false) : collisionMethod(collisionMethod), hashMap(tableSize), grpFlag(grpFlag) {}

    // Hash Functions for Integer Keys
    int hash1(int key) const {
        const int a = 123, b = 456, k = 77, p = 10007;
        return ((((a * k) + b) % p) % hashMap.size());
    }

    int hash2(int key) const {
        int prime = 7; // A prime number less than the table size
        return prime - (key % prime);
    }

    // Primary Hash Function for Strings (DJB2)
    int hash1(string str) const {
        unsigned long hash = 5381;
        for (char c : str) {
            hash = ((hash << 5) + hash) + c; // hash * 33 + c
        }
        return static_cast<int>(hash % hashMap.size());
    }

    int hash2(string str) const {
        unsigned long hash = 0;
        for (char c : str) {
            hash = c + (hash << 6) + (hash << 16) - hash;
        }
        return static_cast<int>((hash % (hashMap.size() - 1)) + 1); // Ensure it's non-zero
    }

    // Collision Resolution Methods (II - Insertion Index)
    int linearProbingII(KeyType key) {
        int hashIndex = hash1(key);
        for (int i = 0; i < hashMap.size(); ++i) {
            int index = (hashIndex + i) % hashMap.size();
            if (hashMap[index].empty()) {
                return index;
            }
        }
        return -1;
    }

    int quadraticProbingII(KeyType key) {
        int hashIndex = hash1(key);
        for (int i = 0; i < hashMap.size(); ++i) {
            int index = (hashIndex + (i * i)) % hashMap.size();
            if (hashMap[index].empty()) {
                return index;
            }
        }
        return -1;
    }

    int doubleHashingII(KeyType key) {
        int hashIndex = hash1(key);
        for (int i = 0; i < hashMap.size(); ++i) {
            int index = (hashIndex + (i * hash2(key))) % hashMap.size();
            if (hashMap[index].empty()) {
                return index;
            }
        }
        return -1;
    }

    int resolveCollision(KeyType key) {
        switch (collisionMethod) {
        case 1:
            return linearProbingII(key);
        case 2:
            return quadraticProbingII(key);
        case 3:
            return doubleHashingII(key);
        case 4:
            return chainingTechnique(key);
        }
    }

    // Searching
    int linearProbing(KeyType key) {
        int hashIndex = hash1(key);
        for (int i = 0; i < hashMap.size(); ++i) {
            int index = (hashIndex + i) % hashMap.size();
            if (!hashMap[index].empty() && hashMap[index].front()->key == key) {
                return index;
            }
        }
        return -1;
    }

    int quadraticProbing(KeyType key) {
        int hashIndex = hash1(key);
        for (int i = 0; i < hashMap.size(); ++i) {
            int index = (hashIndex + (i * i)) % hashMap.size();
            if (!hashMap[index].empty() && hashMap[index].front()->key == key) {
                return index;
            }
        }
        return -1;
    }

    int doubleHashing(KeyType key) {
        int hashIndex = hash1(key);
        for (int i = 0; i < hashMap.size(); ++i) {
            int index = (hashIndex + (i * hash2(key))) % hashMap.size();
            if (!hashMap[index].empty() && hashMap[index].front()->key == key) {
                return index;
            }
        }
        return -1;
    }

    int chainingTechnique(KeyType key) {
        return hash1(key);
    }

    // Helper functions
    int search(KeyType key) {
        switch (collisionMethod) {
        case 1:
            return linearProbing(key);
        case 2:
            return quadraticProbing(key);
        case 3:
            return doubleHashing(key);
        case 4:
            return chainingTechnique(key);
        }
    }

    // Main Functions
    void print() {
        cout << "HashMap Contents:" << endl;
        cout << "-------------------" << endl;
        cout << setw(5) << "Index" << " | " << setw(5) << "Key" << " | " << setw(5) << "Value" << endl;
        cout << "-------------------" << endl;
        for (int i = 0; i < hashMap.size(); ++i) {
            list<Node*>& list = hashMap[i];
            cout << setw(5) << i << " | ";
            if (list.empty()) {
                cout << setw(5) << "No key" << " | " << setw(5) << "No Value" << endl;
            }
            else {
                for (auto it = list.begin(); it != list.end();) {
                    cout << "<" << (*it)->key << "," << (*it)->value << ">";
                    if (++it != list.end()) cout << ", ";
                }
                cout << endl;
            }
        }
        cout << "-------------------" << endl;
    }

    void insert(KeyType key, ValueType value) {
        int index = resolveCollision(key);
        Node* node = new Node({ key, value });
        if (collisionMethod == 4 && grpFlag) {
            if (hashMap[index].empty() || hashMap[index].front()->key == key) {
                hashMap[index].push_back(node);
                return;
            }
            for (int i = 1; i < hashMap.size(); i++) {
                int index2 = (index + i) % hashMap.size();
                if (hashMap[index2].empty() || hashMap[index2].front()->key == key) {
                    hashMap[index2].push_back(node);
                    return;
                }
            }
        }
        else {
            hashMap[index].push_back(node);
        }
    }

    void del(KeyType key, ValueType value = ValueType()) {
        int index = search(key);

        if (index == -1) {
            cout << key << " could not be found in the HashMap for deletion" << endl;
            return;
        }

        if (collisionMethod == 4) {
            list<Node*>& list = hashMap[index];
            for (auto it = list.begin(); it != list.end(); ++it) {
                if (grpFlag) {
                    if ((*it)->key == key && (*it)->value == value) {
                        delete* it;
                        it = list.erase(it);
                        cout << key << " has been deleted from the HashMap" << endl;
                        return;
                    }
                }
                else if ((*it)->key == key) {
                    delete* it;
                    it = list.erase(it);
                    cout << key << " has been deleted from the HashMap" << endl;
                    return;
                }

            }
            cout << key << " could not be found in the HashMap for deletion" << endl;
            return;
        }

        delete hashMap[index].front();
        hashMap[index].pop_front();
        cout << key << " has been deleted from the HashMap" << endl;
    }

    void update(KeyType key, ValueType newValue) {
        int index = search(key);

        if (index == -1) {
            cout << key << " could not be found in the HashMap for updation" << endl;
            return;
        }

        if (collisionMethod == 4) {
            list<Node*>& list = hashMap[index];
            for (auto it = list.begin(); it != list.end(); ++it) {
                if ((*it)->key == key) {
                    (*it)->value = newValue;
                    cout << key << " has been updated in the HashMap" << endl;
                    return;
                }
            }
        }

        hashMap[index].front()->value = newValue;
        cout << key << " has been updated in the HashMap" << endl;
    }

    vector<pair<KeyType, ValueType>> access(KeyType key) {
        int index = search(key);
        vector<pair<KeyType, ValueType>> pairs;

        if (index == -1) {
            cout << key << " could not be found in the HashMap for access" << endl;
            return pairs;
        }

        if (collisionMethod == 4 && grpFlag) {
            for (int i = 0; i < hashMap.size(); i++) {
                int index2 = (index + i) % hashMap.size();
                if (!hashMap[index2].empty() && hashMap[index2].front()->key == key) {
                    index = index2;
                    break;
                }
            }
        }

        list<Node*>& list = hashMap[index];
        for (auto it = list.begin(); it != list.end(); ++it) {
            if ((*it)->key == key) {
                pairs.push_back({ (*it)->key, (*it)->value });
            }
        }

        return pairs;
    }

    // Destructor
    ~HashMap() {
        for (int i = 0; i < hashMap.size(); i++) {
            list<Node*>& list = hashMap[i];
            for (auto it = list.begin(); it != list.end();) {
                delete* it;
                it = list.erase(it);
            }
        }
    }
};

// ===========================
//       UserList Class
// ===========================

class UserList {
private:
    DLL<User> users;
    AVLTree<int> idIdx;
    HashMap<int, Node<User>*> idMap;
    HashMap<string, Node<User>*> nameMap;
    HashMap<string, Node<User>*> emailMap;
    HashMap<string, Node<User>*> countryMap;
    HashMap<string, Node<User>*> typeMap;

    void updateIndices(User user, Node<User>* userAddressInDLL) {
        idIdx.insert(user.getUserID(), userAddressInDLL);
        idMap.insert(user.getUserID(), userAddressInDLL);
        nameMap.insert(user.getUserName(), userAddressInDLL);
        emailMap.insert(user.getUserEmail(), userAddressInDLL);
        countryMap.insert(user.getUserCountry(), userAddressInDLL);
        typeMap.insert(returnUserType(user.getUserType()), userAddressInDLL);
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
        auto userAddressInDLL = idMap.access(_user.getUserID()).front().second;
        auto userCountry = _user.getUserCountry();
        countryMap.del(userCountry, userAddressInDLL);
    }

    void delUserFromTypeIdx(User _user) {
        auto userAddressInDLL = idMap.access(_user.getUserID()).front().second;
        auto userType = returnUserType(_user.getUserType());
        typeMap.del(userType, userAddressInDLL);
    }

    void printUser(User user) {
        cout << "------------------------------------------" << endl;
        cout << "User Id: " << user.getUserID() << endl;
        cout << "Username: " << user.getUserName() << endl;
        cout << "Email: " << user.getUserEmail() << endl;
        cout << "Country: " << user.getUserCountry() << endl;
        cout << "Type: " << returnUserType(user.getUserType()) << endl;
        cout << "------------------------------------------" << endl;
    }

public:
    UserList() : idIdx(), users(), idMap(101, 3), nameMap(101, 4), emailMap(101, 4), countryMap(101, 4, true), typeMap(5, 4, true) {}

    void addUser(User user) {
        Node<User>* node = idIdx.getParentForInsertion(user.getUserID());
        Node<User>* userAddressInDLL = nullptr;

        if (!node) {
            userAddressInDLL = users.insertHead(user);
            updateIndices(user, userAddressInDLL);
            return;
        }

        (user.getUserID() > node->data.getUserID())
        ? userAddressInDLL = users.insertAfter(node, user)
        : userAddressInDLL = users.insertBefore(node, user);

        updateIndices(user, userAddressInDLL);
    }

    User getUserById(int userID) {
        auto user = idMap.access(userID);
        if (user.empty()) {
            cout << "User with Id " << userID << " does not exist" << endl;
            return User();
        }
        else {
            return user.front().second->data;
        }
    }

    vector<pair<string, Node<User>*>> getUsersByCountry(string country) {
        return countryMap.access(country);
    }

    vector<pair<string, Node<User>*>> getUsersByType(Type type) {
        return typeMap.access(returnUserType(type));
    }

    void searchUserById(int userID) {
        auto user = idMap.access(userID);
        if (user.empty()) {
            cout << "User with Id " << userID << " does not exist" << endl;
            return;
        }
        else {
            printUser(user.front().second->data);
        }
    }

    void searchUserByName(string userName) {
        auto user = nameMap.access(userName);
        if (user.empty()) {
            cout << "User with Name " << userName << " does not exist" << endl;
            return;
        }
        else {
            printUser(user.front().second->data);
        }
    }

    void searchUserByEmail(string userEmail) {
        auto user = emailMap.access(userEmail);
        if (user.empty()) {
            cout << "User with Email " << userEmail << " does not exist" << endl;
            return;
        }
        else {
            printUser(user.front().second->data);
        }
    }

    void searchUsersByCountry(string userCountry) {
        auto users = getUsersByCountry(userCountry);
        if (users.empty()) {
            cout << "There is no user in the database who lives in " << userCountry << endl;
            return;
        }
        else {
            cout << "------------------------------------------" << endl;
            cout << "Users filtered by country " << userCountry << ": " << endl;
            int i = 1;
            for (auto it = users.begin(); it != users.end(); ++it, ++i)
                cout << i << ". " << it->second->data.getUserName() << endl;
            cout << "------------------------------------------" << endl;
        }
    }

    void searchUsersByType(Type userType) {
        auto users = getUsersByType(userType);
        string type = returnUserType(userType);
        if (users.empty()) {
            cout << "There is no " << type << " user in the database" << endl;
            return;
        }
        else {
            cout << "------------------------------------------" << endl;
            cout << "Users filtered by type " << type << ": " << endl;
            int i = 1;
            for (auto it = users.begin(); it != users.end(); ++it, ++i)
                cout << i << ". " << it->second->data.getUserName() << endl;
            cout << "------------------------------------------" << endl;
        }
    }

    int delUserById(int userID) {
        auto user = idMap.access(userID);

        if (user.empty()) {
            cout << "User with userId " << userID << " does not exist" << endl;
            return -1;
        }

        string userName = user.front().second->data.getUserName();
        string userEmail = user.front().second->data.getUserEmail();

        idIdx.remove(userID);
        nameMap.del(userName);
        emailMap.del(userEmail);
        delUserFromCountryIdx(user.front().second->data);
        delUserFromTypeIdx(user.front().second->data);
        idMap.del(userID);
        users.deleteNode(user.front().second);

        cout << "User with userId " << userID << " has been sucessfully removed" << endl;
        return userID;
    }

    int delUserByName(string userName) {
        auto user = nameMap.access(userName);
        if (user.empty()) {
            cout << "User with userName " << userName << " does not exist" << endl;
            return -1;
        }

        int userID = user.front().second->data.getUserID();
        string userEmail = user.front().second->data.getUserEmail();

        idIdx.remove(userID);
        nameMap.del(userName);
        emailMap.del(userEmail);
        delUserFromCountryIdx(user.front().second->data);
        delUserFromTypeIdx(user.front().second->data);
        idMap.del(userID);
        users.deleteNode(user.front().second);

        cout << "User with userId " << userID << " has been sucessfully removed" << endl;
        return userID;
    }

    int delUserByEmail(string userEmail) {
        auto user = emailMap.access(userEmail);
        if (user.empty()) {
            cout << "User with userEmail " << userEmail << " does not exist" << endl;
            return -1;
        }

        int userID = user.front().second->data.getUserID();
        string userName = user.front().second->data.getUserName();

        idIdx.remove(userID);
        nameMap.del(userName);
        emailMap.del(userEmail);
        delUserFromCountryIdx(user.front().second->data);
        delUserFromTypeIdx(user.front().second->data);
        idMap.del(userID);
        users.deleteNode(user.front().second);

        cout << "User with userId " << userID << " has been sucessfully removed" << endl;
        return userID;
    }

    vector<int> delUsersByCountry(string country) {
        auto users = getUsersByCountry(country);

        if (users.empty()) {
            cout << "There are no users in " << country << " to be deleted" << endl;
            return vector<int>();
        }

        vector<int> userIdsToDelete;

        // Collect user IDs to delete
        for (auto iter = users.begin(); iter != users.end(); ++iter) {
            userIdsToDelete.push_back(iter->second->data.getUserID());
        }

        // Delete users by ID
        for (int userId : userIdsToDelete) {
            delUserById(userId);
        }

        cout << "All users living in " << country << " have been deleted" << endl;
        return userIdsToDelete;
    }

    vector<int> delUsersByType(Type type) {
        auto users = getUsersByType(type);
        if (users.empty()) {
            cout << "There are no " << returnUserType(type) << " users to be deleted" << endl;
            return vector<int>();
        }

        vector<int> userIdsToDelete;

        // Collect user IDs to delete
        for (auto iter = users.begin(); iter != users.end(); ++iter) {
            userIdsToDelete.push_back(iter->second->data.getUserID());
        }

        // Delete users by ID
        for (int userId : userIdsToDelete) {
            delUserById(userId);
        }

        cout << "All " << returnUserType(type) << " users have been deleted" << endl;
        return userIdsToDelete;
    }

    void printUserList() {
        if (users.size() == 0) {
            cout << "There are no users currently in the database" << endl;
            return;
        }

        for (auto it = users.fbegin(); it != users.fend(); ++it) {
            printUser(*it);
        }
    }
};

// ===========================
//      Complaint Class
// ===========================

class Complaint {
private:
    User user;
    int userId;
    int complaintId;
    string description;
    static int currentId;

public:
    // Parameterized constructor
    Complaint(int _userId, const string& _description, UserList& userList)
        : userId(_userId), complaintId(++currentId), description(_description) {
        setUser(userList);
    }

    // Getters
    int getUserId() const {
        return userId;
    }

    int getComplaintId() const {
        return complaintId;
    }

    string getDescription() const {
        return description;
    }

    static int getCurrentId() {
        return currentId;
    }

    // Methods
    void setUser(UserList& userList) {
        this->user = userList.getUserById(userId);
    }

    bool operator > (const Complaint& complaint) const {
        return (this->user.getUserType() == complaint.user.getUserType())
            ? (this->complaintId < complaint.complaintId)
            : (this->user.getUserType() > complaint.user.getUserType());
    }

    bool operator < (const Complaint& complaint) const {
        return (this->user.getUserType() == complaint.user.getUserType())
            ? (this->complaintId > complaint.complaintId)
            : (this->user.getUserType() < complaint.user.getUserType());
    }

    void print() const {
        cout << "------------------------------------------" << endl;
        cout << "User Id: " << userId << endl;
        cout << "Complaint Id: " << complaintId << endl;
        cout << "Description: " << description << endl;
        cout << "------------------------------------------" << endl;
    }
};

// ===========================
//        Heap Class
// ===========================

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

        if ((isMaxHeap && *child > *parent) || (!isMaxHeap && *child < *parent)) {
            swap(heap[index], heap[parentIndex]);
            heapifyUp(parentIndex);
        }
    }

    void heapifyDown(int index) {
        int leftChildIndex = (index * 2) + 1;

        if (leftChildIndex >= size) return;

        int rightChildIndex = leftChildIndex + 1;
        int requiredIndex = index;

        if (leftChildIndex < size && (isMaxHeap && *heap[leftChildIndex] > *heap[requiredIndex]) ||
            (!isMaxHeap && *heap[leftChildIndex] < *heap[requiredIndex]))
            requiredIndex = leftChildIndex;

        if (rightChildIndex < size && (isMaxHeap && *heap[rightChildIndex] > *heap[requiredIndex]) ||
            (!isMaxHeap && *heap[rightChildIndex] < *heap[requiredIndex]))
            requiredIndex = rightChildIndex;

        if (index != requiredIndex) {
            swap(heap[index], heap[requiredIndex]);
            heapifyDown(requiredIndex);
        }
    }

public:
    Heap(bool isMaxHeap = true) : isMaxHeap(isMaxHeap), size(0) {}

    ~Heap() {
        for (auto& elem : heap) {
            if (elem != nullptr) {
                delete elem;
                elem = nullptr;
            }
        }

        heap.clear();
        size = 0;
    }

    T search(int id) {
        for (const auto& node : heap) {
            if (node->getComplaintId() == id) return node;
        }
        return nullptr;
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

        cout << "[ ";
        for (const auto& elem : heap) {
            cout << elem << " ";
        }
        cout << "]" << endl;
    }

    void insert(T node) {
        ++size;
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

    void increasePriority(T& elem) {
        auto it = find(heap.begin(), heap.end(), elem);
        if (it != heap.end()) {
            int index = distance(heap.begin(), it);
            heap[index] = heap.back();
            heap.pop_back();
            --size;
            heapifyDown(index);
        }
        else {
            cout << "Complaint not found" << endl;
        }
    }
};

// ===========================
//         CMS Class
// ===========================

class CMS {
private:
    UserList userList;
    Heap<Complaint*> complaints;
    HashMap<int, Complaint*> complaintsMap;

    void addSampleUsers() {
        User user1("Muhaimin", "abdul.muhaimin@example.com", "USA", Platinum);
        User user2("John Doe", "john.doe@example.com", "USA", New);
        User user3("Alice Johnson", "alice.johnson@example.com", "UK", Gold);
        User user4("Bob Brown", "bob.brown@example.com", "Australia", Gold);
        User user5("Charlie Davis", "charlie.davis@example.com", "Australia", Gold);
        User user6("Dylan Minette", "dylan.minette@example.com", "Australia", Platinum);

        userList.addUser(user1);
        userList.addUser(user2);
        userList.addUser(user3);
        userList.addUser(user4);
        userList.addUser(user5);
        userList.addUser(user6);
    }

    Complaint* inputComplain() {
        int userId;
        string description;
        cout << "Enter the user id: ";
        cin >> userId;
        if (complaintsMap.access(userId).empty()) return nullptr;
        cin.ignore();
        cout << "Enter the complaint description: ";
        getline(cin, description);
        return new Complaint(userId, description, userList);
    }

public:
    CMS() : complaintsMap(101, 4, true) { addSampleUsers(); }
    ~CMS() {}

    void insertUser() {
        string userName, userEmail, userCountry;
        int userTypeInt;
        Type userType;

        cout << "Enter username: ";
        cin.ignore();
        getline(cin, userName);

        cout << "Enter email: ";
        getline(cin, userEmail);

        cout << "Enter country: ";
        getline(cin, userCountry);

        cout << "Select user type:" << endl;
        cout << "1. New" << endl;
        cout << "2. Regular" << endl;
        cout << "3. Silver" << endl;
        cout << "4. Gold" << endl;
        cout << "5. Platinum" << endl;
        cout << "Enter choice (1-5): ";
        cin >> userTypeInt;

        switch (userTypeInt) {
        case 1:
            userType = New;
            break;
        case 2:
            userType = Regular;
            break;
        case 3:
            userType = Silver;
            break;
        case 4:
            userType = Gold;
            break;
        case 5:
            userType = Platinum;
            break;
        default:
            cout << "Invalid choice. Defaulting to 'New'." << endl;
            userType = New;
            break;
        }

        User user(userName, userEmail, userCountry, userType);
        userList.addUser(user);
    }

    void registerComplaint() {
        Complaint* complaint = inputComplain();

        if (complaint == nullptr) {
            cout << "Complaint can not be registered against a non-existent User Id" << endl;
            return;
        }

        complaints.insert(complaint);
        complaintsMap.insert(complaint->getUserId(), complaint);
        cout << "Complaint has been registered successfully" << endl;
    }

    void serviceComplaint() {
        if (complaints.empty()) {
            cout << "There are no complaints to be serviced" << endl;
            return;
        }

        Complaint* complaint = complaints.at(0);
        complaintsMap.del(complaint->getUserId(), complaint);
        complaints.remove();

        delete complaint;
        complaint = nullptr;

        cout << "Top complaint has been serviced" << endl;
    }

    void increasePriority(int complaintId) {
        Complaint* complaint = complaints.search(complaintId);

        if (complaint == nullptr) {
            cout << "No complaint exists with the given complaint Id" << endl;
            return;
        }

        complaintsMap.del(complaint->getUserId(), complaint);
        complaints.increasePriority(complaint);

        delete complaint;
        complaint = nullptr;

        cout << "Given complaint has been serviced as highest priority" << endl;
    }

    void displayComplaints(int userId) {
        int index = complaintsMap.search(userId);
        if (index == -1) {
            cout << "User with Id " << userId << " does not exist so he does not complaint" << endl;
            return;
        }

        auto list = complaintsMap.access(userId);
        if (list.empty()) {
            cout << "The user with Id " << userId << " does not have any complaints" << endl;
            return;
        }

        cout << "Complaints by User Id " << userId << " are following: " << endl;
        for (auto iter = list.begin(); iter != list.end(); ++iter) {
            iter->second->print();
        }
    }

    void displayComplaintsByCountry(string country) {
        vector<pair<string, Node<User>*>> users = userList.getUsersByCountry(country);
        if (users.empty()) {
            cout << "There are no complaints from " << country << endl;
            return;
        }

        cout << "Complaints in " << country << " are following: " << endl;
        for (auto iter = users.begin(); iter != users.end(); ++iter) {
            displayComplaints(iter->second->data.getUserID());
        }
    }

    void deleteComplaints(int userID) {
        auto complaints = complaintsMap.access(userID);
        for (auto it = complaints.begin(); it != complaints.end();) {
            auto temp = ++it; --it;
            increasePriority(it->second->getComplaintId());
            it = temp;
        }
    }

    // Search and delete user by ID
    void searchUserById(int userID) {
        userList.searchUserById(userID);
    }

    void delUserById(int userID) {
        int id = userList.delUserById(userID);
        if (id == -1) return;
        deleteComplaints(userID);
    }

    // Search and delete user by Name
    void searchUserByName(const string& userName) {
        userList.searchUserByName(userName);
    }

    void delUserByName(const string& userName) {
        int userID = userList.delUserByName(userName);
        if (userID == -1) return;
        deleteComplaints(userID);
    }

    // Search and delete user by Email
    void searchUserByEmail(const string& userEmail) {
        userList.searchUserByEmail(userEmail);
    }

    void delUserByEmail(const string& userEmail) {
        int userID = userList.delUserByEmail(userEmail);
        if (userID == -1) return;
        deleteComplaints(userID);
    }

    // Search and delete all users by Country
    void searchUsersByCountry(const string& usersCountry) {
        userList.searchUsersByCountry(usersCountry);
    }

    void delUsersByCountry(const string& usersCountry) {
        vector<int> usersToDelete = userList.delUsersByCountry(usersCountry);
        for (const auto& userId : usersToDelete) {
            deleteComplaints(userId);
        }
    }

    // Search and delete all users by Type
    void searchUsersByType(Type userType) {
        userList.searchUsersByType(userType);
    }

    void delUsersByType(Type userType) {
        vector<int> usersToDelete = userList.delUsersByType(userType);
        for (const auto& userId : usersToDelete) {
            deleteComplaints(userId);
        }
    }

    // Print all users
    void printAllUsers() {
        userList.printUserList();
    }

    // Display menu
    void displayMenu() {
        // Clear screen
        cout << "\033[2J\033[H";
        cout << "\033[0m"; // Reset color

        // Menu border
        cout << "\033[1;33m" << string(70, '=') << "\033[0m" << endl; // Yellow border

        // Menu options
        cout << "\033[1;00m" // Cyan text
            << "Complaint Management System Menu:\n"
            << "\033[0m" // Reset color
            << "\033[1;32m" // Green numbers
            << "[01] Insert User\n"
            << "[02] Register Complaint\n"
            << "[03] Service Complaint\n"
            << "[04] Increase Complaint Priority\n"
            << "[05] Display Complaints by User ID\n"
            << "[06] Display Complaints by Country\n"
            << "[07] Search User by ID\n"
            << "[08] Delete User by ID\n"
            << "[09] Search User by Name\n"
            << "[10] Delete User by Name\n"
            << "[11] Search User by Email\n"
            << "[12] Delete User by Email\n"
            << "[13] Search Users by Country\n"
            << "[14] Delete Users by Country\n"
            << "[15] Search Users by Type\n"
            << "[16] Delete Users by Type\n"
            << "[17] Print All Users\n"
            << "[18] Exit\n"
            << "\033[0m"; // Reset color

        // Footer border
        cout << "\033[1;33m" << string(70, '=') << "\033[0m" << endl; // Yellow border

        // Prompt
        cout << "\033[1;31m" // Bold red for prompt
            << "Enter your choice: "
            << "\033[0m"; // Reset color
    }

    // Main menu
    void mainMenu() {
        int choice;
        do {
            _getch();
            displayMenu();
            cin >> choice;
            switch (choice) {
            case 1:
                insertUser();
                break;
            case 2:
                registerComplaint();
                break;
            case 3:
                serviceComplaint();
                break;
            case 4: {
                int complaintId;
                cout << "\033[1;31m Enter complaint ID: \033[0m";
                cin >> complaintId;
                increasePriority(complaintId);
                break;
            }
            case 5: {
                int userId;
                cout << "\033[1;31mEnter user ID: \033[0m";
                cin >> userId;
                displayComplaints(userId);
                break;
            }
            case 6: {
                string country;
                cout << "\033[1;31mEnter country: \033[0m";
                cin.ignore();
                getline(cin, country);
                displayComplaintsByCountry(country);
                break;
            }
            case 7: {
                int userId;
                cout << "\033[1;31mEnter user ID: \033[0m";
                cin >> userId;
                searchUserById(userId);
                break;
            }
            case 8: {
                int userId;
                cout << "\033[1;31mEnter user ID: \033[0m";
                cin >> userId;
                delUserById(userId);
                break;
            }
            case 9: {
                string userName;
                cout << "\033[1;31mEnter user name: \033[0m";
                cin.ignore();
                getline(cin, userName);
                searchUserByName(userName);
                break;
            }
            case 10: {
                string userName;
                cout << "\033[1;31mEnter user name: \033[0m";
                cin.ignore();
                getline(cin, userName);
                delUserByName(userName);
                break;
            }
            case 11: {
                string userEmail;
                cout << "\033[1;31mEnter user email: \033[0m";
                cin.ignore();
                getline(cin, userEmail);
                searchUserByEmail(userEmail);
                break;
            }
            case 12: {
                string userEmail;
                cout << "\033[1;31mEnter user email: \033[0m";
                cin.ignore();
                getline(cin, userEmail);
                delUserByEmail(userEmail);
                break;
            }
            case 13: {
                string usersCountry;
                cout << "\033[1;31mEnter country: \033[0m";
                cin.ignore();
                getline(cin, usersCountry);
                searchUsersByCountry(usersCountry);
                break;
            }
            case 14: {
                string usersCountry;
                cout << "\033[1;31mEnter country: \033[0m";
                cin.ignore();
                getline(cin, usersCountry);
                delUsersByCountry(usersCountry);
                break;
            }
            case 15: {
                int userTypeInt;
                Type userType;
                cout << "\033[1;31mSelect user type:\033[0m" << endl;
                cout << "\033[1;31m1. New\033[0m" << endl;
                cout << "\033[1;31m2. Regular\033[0m" << endl;
                cout << "\033[1;31m3. Silver\033[0m" << endl;
                cout << "\033[1;31m4. Gold\033[0m" << endl;
                cout << "\033[1;31m5. Platinum\033[0m" << endl;
                cout << "\033[1;31mEnter choice (1-5): \033[0m";
                cin >> userTypeInt;
                switch (userTypeInt) {
                case 1:
                    userType = New;
                    break;
                case 2:
                    userType = Regular;
                    break;
                case 3:
                    userType = Silver;
                    break;
                case 4:
                    userType = Gold;
                    break;
                case 5:
                    userType = Platinum;
                    break;
                default:
                    cout << "\033[1;31mInvalid choice.\033[0m" << endl;
                    continue;
                }
                searchUsersByType(userType);
                break;
            }
            case 16: {
                int userTypeInt;
                Type userType;
                cout << "\033[1;31mSelect user type:\033[0m" << endl;
                cout << "\033[1;31m1. New\033[0m" << endl;
                cout << "\033[1;31m2. Regular\033[0m" << endl;
                cout << "\033[1;31m3. Silver\033[0m" << endl;
                cout << "\033[1;31m4. Gold\033[0m" << endl;
                cout << "\033[1;31m5. Platinum\033[0m" << endl;
                cout << "\033[1;31mEnter choice (1-5): \033[0m";
                cin >> userTypeInt;
                switch (userTypeInt) {
                case 1:
                    userType = New;
                    break;
                case 2:
                    userType = Regular;
                    break;
                case 3:
                    userType = Silver;
                    break;
                case 4:
                    userType = Gold;
                    break;
                case 5:
                    userType = Platinum;
                    break;
                default:
                    cout << "\033[1;31mInvalid choice.\033[0m" << endl;
                    continue;
                }
                delUsersByType(userType);
                break;
            }
            case 17:
                printAllUsers();
                break;
            case 18:
                cout << "\033[1;31mExiting...\033[0m" << endl;
                break;
            default:
                cout << "\033[1;31mInvalid choice. Please try again.\033[0m" << endl;
            }

        } while (choice != 18);
    }
};

// ===========================
//        Static Data
// ===========================

int User::currentUserID = 0;
int Complaint::currentId = 0;

// ===========================
//        Main Function
// ===========================

int main() {
    {
        CMS().mainMenu();
    }

    _CrtDumpMemoryLeaks() ? cout << "Leaks Found\n" : cout << "No Leaks Found\n";

    return 0;
}