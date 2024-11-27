#include <list>
#include <vector>
#include <iomanip>
#include <iostream>
using namespace std;

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

public:
    HashMap(int tableSize, int collisionMethod, bool grpFlag = false) : collisionMethod(collisionMethod), hashMap(tableSize), grpFlag(grpFlag) {}

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
