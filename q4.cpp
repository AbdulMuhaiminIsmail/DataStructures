#include <queue>
#include <string>
#include <iostream>
#include <conio.h>

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

    friend class Complaint;

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

        countryAVLNode->userAddresses.insertAtEnd(userAddressInDLL);

        // Updating type group index
        Type userType = user.getUserType();
        auto typeAVLNode = typeGrpIdx.search(userType);

        if (!typeAVLNode) {
            typeGrpIdx.insert(userType, nullptr);
            typeAVLNode = typeGrpIdx.search(userType);
        }

        typeAVLNode->userAddresses.insertAtEnd(userAddressInDLL);
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
        }
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

    User getUserById(int userID) {
        auto user = idIdx.search(userID);
        if (!user) {
            cout << "User with Id " << userID << " does not exist" << endl;
            return User();
        }
        else {
            return (user->userAddresses.getHead()->value->data);
        }
    }

    LinkedList<Node<User>*>& getUsersByCountry(string country) {
        auto users = countryGrpIdx.search(country);
        if (users == nullptr) {
            LinkedList<Node<User>*> list;
            return list;
        }
        return users->userAddresses;
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
            cout << "There is no " << returnUserType(userType) << " user in the database" << endl;
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

    int delUserById(int userID) {
        auto user = idIdx.search(userID);
        if (!user) {
            cout << "User with userId " << userID << " does not exist" << endl;
            return -1;
        }

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
        return userID;
    }

    int delUserByName(string userName) {
        auto user = nameIdx.search(userName);
        if (!user) {
            cout << "User with userName " << userName << " does not exist" << endl;
            return -1;
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
            return userID;
        }
    }

    int delUserByEmail(string userEmail) {
        auto user = emailIdx.search(userEmail);
        if (!user) {
            cout << "User with userEmail " << userEmail << " does not exist" << endl;
            return -1;
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
            return userID;
        }
    }

    vector<int> delUsersByCountry(string country) {
        auto users = countryGrpIdx.search(country);
        if (!users) {
            cout << "There are no users in " << country << " to be deleted" << endl;
            return vector<int>();
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
            return userIdsToDelete;
        }
    }

    vector<int> delUsersByType(Type type) {
        auto users = typeGrpIdx.search(type);
        if (!users) {
            cout << "There are no " << returnUserType(type) << " users to be deleted" << endl;
            return vector<int>();
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
            return userIdsToDelete;
        }
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
//        CMS Class
// ===========================

class CMS {
private:
    UserList userList;
    Heap<Complaint*> complaints;
    AVLTree<int> userGrpIdx;

    void addIdx(Complaint* complaint) {
        int userId = complaint->getUserId();
        auto userAVLNode = userGrpIdx.search(userId);

        if (!userAVLNode) {
            userGrpIdx.insert(userId, nullptr);
            userAVLNode = userGrpIdx.search(userId);
        }

        userAVLNode->complaintAddresses.insertAtEnd(complaint);
    }

    void delIdx(int userId, int complaintId) {
        auto userAVLNode = userGrpIdx.search(userId);
        userAVLNode->complaintAddresses.deleteComplaint(complaintId);
    }

    Complaint* inputComplain() {
        int userId;
        string description;
        cout << "Enter the user id: ";
        cin >> userId;
        if (userId > User::getCurrentUserId()) return nullptr;
        cin.ignore();
        cout << "Enter the complaint description: ";
        getline(cin, description);
        return new Complaint(userId, description, userList);
    }

public:
    CMS() {}
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
        userGrpIdx.insert(user.getUserID(), nullptr);
    }

    void registerComplaint() {
        Complaint* complaint = inputComplain();

        if (complaint == nullptr) {
            cout << "Complaint can not be registered against a non-existent User Id" << endl;
            return;
        }

        complaints.insert(complaint);
        addIdx(complaint);
        cout << "Complaint has been registered successfully" << endl;
    }

    void serviceComplaint() {
        if (complaints.getSize() == 0) {
            cout << "There are no complaints to be serviced" << endl;
            return;
        }

        Complaint* complaint = complaints.at(0);
        delIdx(complaint->getUserId(), complaint->getComplaintId());
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

        delIdx(complaint->getUserId(), complaint->getComplaintId());
        complaints.increasePriority(complaint);

        delete complaint;
        complaint = nullptr;

        cout << "Given complaint has been serviced as highest priority" << endl;
    }

    void displayComplaints(int userId) {
        auto userAVLNode = userGrpIdx.search(userId);
        if (userAVLNode == nullptr) {
            cout << "User with Id " << userId << " does not exist so he does not complaint" << endl;
            return;
        }

        LinkedList<Complaint*>& list = userAVLNode->complaintAddresses;
        if (list.getHead() == nullptr) {
            cout << "The user with Id " << userId << " does not have any complaints" << endl;
            return;
        }

        cout << "Complaints by User Id " << userId << " are following: " << endl;
        for (auto iter = list.getHead(); iter; iter = iter->next) {
            iter->value->print();
        }
    }

    void displayComplaintsByCountry(string country) {
        LinkedList<Node<User>*>& users = userList.getUsersByCountry(country);
        if (users.isEmpty()) {
            cout << "There are no complaints from " << country << endl;
            return;
        }

        cout << "Complaints in " << country << " are following: " << endl;
        for (auto iter = users.getHead(); iter; iter = iter->next) {
            displayComplaints(iter->value->data.getUserID());
        }
    }

    // Search and delete user by ID
    void searchUserById(int userID) {
        userList.searchUserById(userID);
    }

    void delUserById(int userID) {
        int id = userList.delUserById(userID);
        if (id == -1) return;

        LinkedList<Complaint*>& complaints = userGrpIdx.search(userID)->complaintAddresses;
        for (auto it = complaints.getHead(); it;) {
            auto temp = it->next;
            increasePriority(it->value->getComplaintId());
            it = temp;
        }

        userGrpIdx.remove(userID);
    }

    // Search and delete user by Name
    void searchUserByName(const string& userName) {
        userList.searchUserByName(userName);
    }

    void delUserByName(const string& userName) {
        int userID = userList.delUserByName(userName);
        if (userID == -1) return;

        LinkedList<Complaint*>& complaints = userGrpIdx.search(userID)->complaintAddresses;
        for (auto it = complaints.getHead(); it;) {
            auto temp = it->next;
            increasePriority(it->value->getComplaintId());
            it = temp;
        }

        userGrpIdx.remove(userID);
    }

    // Search and delete user by Email
    void searchUserByEmail(const string& userEmail) {
        userList.searchUserByEmail(userEmail);
    }

    void delUserByEmail(const string& userEmail) {
        int userID = userList.delUserByEmail(userEmail);
        if (userID == -1) return;

        LinkedList<Complaint*>& complaints = userGrpIdx.search(userID)->complaintAddresses;
        for (auto it = complaints.getHead(); it;) {
            auto temp = it->next;
            increasePriority(it->value->getComplaintId());
            it = temp;
        }

        userGrpIdx.remove(userID);
    }

    // Search and delete all users by Country

    void searchUsersByCountry(const string& usersCountry) {
        userList.searchUsersByCountry(usersCountry);
    }

    void delUsersByCountry(const string& usersCountry) {
        vector<int> usersToDelete = userList.delUsersByCountry(usersCountry);
        for (const auto& userId : usersToDelete) {
            delUserById(userId);
            userGrpIdx.remove(userId);
        }
    }

    // Search and delete all users by Type

    void searchUsersByType(Type userType) {
        userList.searchUsersByType(userType);
    }
    void delUsersByType(Type userType) {
        vector<int> usersToDelete = userList.delUsersByType(userType);
        for (const auto& userId : usersToDelete) {
            delUserById(userId);
            userGrpIdx.remove(userId);
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