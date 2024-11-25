#include <list>
#include <vector>
#include <iomanip>
#include <iostream>
using namespace std;

template <typename KeyType, typename ValueType> class HashMap {
private:
	struct Node {
		KeyType key;
		ValueType value;
		Node(pair<KeyType, ValueType> kvPair) : key(kvPair.first), value(kvPair.second) {}
	};

	int collisionMethod;
	vector<list<Node*>> hashMap;

public:
	HashMap(int tableSize, int collisionMethod) : collisionMethod(collisionMethod), hashMap(tableSize) {}

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
	void Print() {
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
				for (auto it = list.begin(); it != list.end(); ++it) {
					cout << setw(5) << (*it)->key << " | " << setw(5) << (*it)->value;
					if (++it != list.end()) cout << " --> ";
					--it;
				}
				cout << endl;
			}
		}
		cout << "-------------------" << endl;
	}

	void Insert(KeyType key, ValueType value) {
		int index = resolveCollision(key);
		Node* node = new Node({ key, value });
		hashMap[index].push_back(node);
		cout << "<" << key << "," << value << ">" << " has been inserted into the HashMap" << endl;
	}

	void Delete(int key) {
		int index = search(key);

		if (index == -1) {
			cout << key << " could not be found in the HashMap for deletion" << endl;
			return;
		}

		if (collisionMethod == 4) {
			list<Node*>& list = hashMap[index];
			for (auto it = list.begin(); it != list.end(); ++it) {
				if ((*it)->key == key) {
					delete *it;
					list.erase(it);
					cout << key << " has been deleted from the HashMap" << endl;
					return;
				}
			}
		}

		delete hashMap[index].front();
		hashMap[index].pop_front();
		cout << key << " has been deleted from the HashMap" << endl;
	}

	void Update(int key, int newValue) {
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

	vector<pair<KeyType, ValueType>> Access(int key) {
		int index = search(key);
		vector<pair<KeyType, ValueType>> pairs;

		if (index == -1) {
			cout << key << " could not be found in the HashMap for access" << endl;
			return pairs;
		}

		list<Node*>& list = hashMap[index];
		for (auto it = list.begin(); it != list.end(); ++it) {
			if ((*it)->key == key) pairs.push_back({ (*it)->key, (*it)->value });
		}

		return pairs;
	}

	// Destructor
	~HashMap() {
		for (int i = 0; i < hashMap.size(); i++) {
			list<Node*>& list = hashMap[i];
			for (auto it = list.begin(); it != list.end();) {
				delete *it;
				it = list.erase(it);
			}
		}
	}
};

class Point {
public:
	int x, y;

	Point(int x = 0, int y = 0) : x(x), y(y) {}

	friend ostream& operator<<(ostream& os, const Point& point) {
		os << "(" << point.x << ", " << point.y << ")";
		return os;
	}
};


int main() {
	// Test with string keys and int values
	for (int method = 1; method <= 3; ++method) {
		cout << "Testing with collision method " << method << " (string keys, int values)" << endl;
		HashMap<string, int> hashMap(11, method);

		hashMap.Insert("one", 1);
		hashMap.Insert("two", 2);
		hashMap.Insert("three", 3);
		hashMap.Insert("four", 4);
		hashMap.Insert("five", 5);

		hashMap.Print();
		cout << endl;
	}

	// Test with string keys and Point values
	for (int method = 1; method <= 3; ++method) {
		cout << "Testing with collision method " << method << " (string keys, Point values)" << endl;
		HashMap<string, Point> hashMap(11, method);

		hashMap.Insert("A", Point(1, 2));
		hashMap.Insert("B", Point(3, 4));
		hashMap.Insert("C", Point(5, 6));
		hashMap.Insert("D", Point(7, 8));
		hashMap.Insert("E", Point(9, 10));

		hashMap.Print();
		cout << endl;
	}

	// Test with int keys and Point values
	for (int method = 1; method <= 4; ++method) {
		cout << "Testing with collision method " << method << " (int keys, Point values)" << endl;
		HashMap<int, Point> hashMap(11, method);

		hashMap.Insert(1, Point(1, 2));
		hashMap.Insert(2, Point(3, 4));
		hashMap.Insert(3, Point(5, 6));
		hashMap.Insert(4, Point(7, 8));
		hashMap.Insert(5, Point(9, 10));

		hashMap.Print();
		cout << endl;
	}

	_CrtDumpMemoryLeaks() ? cout << "Leaks Found" : cout << "No Leaks Found";
	return 0;
}


