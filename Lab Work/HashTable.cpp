#include "HashTable.h"

// Constructor
HashTable::HashTable(int step, int collisionMethod, bool rehashFlag) {
	this->rehashFlag = rehashFlag;
	this->collisionMethod = collisionMethod;
	this->step = step;
	this->tableSize = 11;
	this->currentLoad = 0;

	arr = new int[tableSize];

	for (int i = 0; i < tableSize; i++) {
		arr[i] = -1;
	}
}

// Hash Functions
int HashTable::hash1(int key) const {
	return (key % tableSize);
}

int HashTable::hash2(int key) const {
	return (1 + key % (tableSize - 2));
}

// Collision Resolution + Searching
pair<int, int> HashTable::linearProbing(int hashValue, int valueToSearch = -1) {
	int index = -1, newIndex = -1, i = 0;
	do {
		newIndex = hash1(hashValue + i++);
		if (arr[newIndex] == valueToSearch) index = newIndex;
	} while (index == -1 && i < tableSize);
	return make_pair(index, i);
}

pair<int, int> HashTable::linearProbingWithSteps(int hashValue, int step, int valueToSearch = -1) {
	int index = -1, newIndex = -1, i = 0;
	do {
		newIndex = hash1(hashValue + (step * i++));
		if (arr[newIndex] == valueToSearch) index = newIndex;
	} while (index == -1 && i < tableSize);
	return make_pair(index, i);
}

pair<int, int> HashTable::pseudoRandomProbing(int hashValue, int valueToSearch = -1) {
	srand(time(0));
	vector<int> perm;

	// Generating random vector
	for (int i = 0; i < tableSize; i++) {
		perm.push_back(rand() % tableSize);
	}

	// Probing empty bucket
	int index = -1, newIndex = -1, i = 0;
	do {
		newIndex = hash1(hashValue + perm[i++]);
		if (arr[newIndex] == valueToSearch) index = newIndex;
	} while (index == -1 && i < perm.size());
	return make_pair(index, i);
}

pair<int, int> HashTable::doubleHashing(int hashValue, int key, int valueToSearch = -1) {
	int index = -1, newIndex = -1, i = 0;
	do {
		newIndex = hash1(hashValue + i++ * hash2(key));
		if (arr[newIndex] == valueToSearch) index = newIndex;
	} while (index == -1 && i < tableSize);
	return make_pair(index, i);
}

pair<int, int> HashTable::resolveCollision(int hashValue, int key) {
	switch (collisionMethod) {
	case 1:
		return linearProbing(hashValue);
	case 2:
		return linearProbingWithSteps(hashValue, step);
	case 3:
		return pseudoRandomProbing(hashValue);
	case 4:
		return doubleHashing(hashValue, key);
	}
}

// Helper functions
pair<int, int> HashTable::search(int hashValue, int key) {
	switch (collisionMethod) {
	case 1:
		return linearProbing(hashValue, key);
	case 2:
		return linearProbingWithSteps(hashValue, step, key);
	case 3:
		return pseudoRandomProbing(hashValue, key);
	case 4:
		return doubleHashing(hashValue, key, key);
	}
}

void HashTable::rehash() {
	int* oldArr = arr;
	int oldSize = tableSize;
	tableSize = oldSize * 2;
	arr = new int[tableSize];

	for (int i = 0; i < tableSize; i++) {
		arr[i] = -1;
	}

	int oldLoad = currentLoad;
	currentLoad = 0;

	for (int i = 0; i < oldSize; i++) {
		if (oldArr[i] != -1) Insert(oldArr[i]);
	}

	currentLoad = oldLoad;

	delete[] oldArr;
	oldArr = nullptr;
}

// Main Functions
void HashTable::Print() {
	cout << "HashTable Contents:" << endl;
	cout << "-------------------" << endl;
	cout << "Index | Value" << endl;
	cout << "-------------------" << endl;
	for (int i = 0; i < tableSize; ++i) {
		cout << "  " << i << "   | ";
		if (arr[i] == -1) {
			cout << "Empty" << endl;
		}
		else {
			cout << arr[i] << endl;
		}
	}
	cout << "-------------------" << endl;
}

void HashTable::Insert(int key) {
	int hashValue = hash1(key);

	pair<int, int> Pair = resolveCollision(hashValue, key);

	if (Pair.first == -1) {
		cout << key << " can't be inserted because there is no empty bucket in the HashTable" << endl;
		return;
	}

	arr[Pair.first] = key;
	++currentLoad;
	float loadFactor = currentLoad / tableSize;
	if (rehashFlag && loadFactor > 0.5) rehash();
	cout << key << " has been inserted into the HashTable" << endl;
}

void HashTable::Delete(int key) {
	int hashValue = hash1(key);

	pair<int, int> Pair = search(hashValue, key);

	if (Pair.first == -1) {
		cout << key << " could not be found in the HashTable for deletion" << endl;
		return;
	}

	arr[Pair.first] = -1;
	--currentLoad;
	cout << key << " has been deleted from the HashTable" << endl;
}

void HashTable::Update(int key, int newValue) {
	Delete(key);
	Insert(newValue);
}

pair<int, int> HashTable::Access(int key) {
	int hashValue = hash1(key);

	pair<int, int> Pair = search(hashValue, key);

	if (Pair.first == -1) {
		cout << key << " could not be found in the HashTable for access" << endl;
	}

	return Pair;
}

// Destructor
HashTable::~HashTable() {
	if (arr != nullptr) {
		delete[] arr;
		arr = nullptr;
	}
}

int main() {
	// Create hash tables with different collision resolution methods
	HashTable h1(1, 1, true); // Linear probing
	HashTable h2(1, 2, true); // Linear probing with steps
	HashTable h3(1, 3, true); // Pseudo-random probing
	HashTable h4(1, 4, true); // Double hashing

	// Insert keys
	vector<int> keys = { 10, 22, 31, 44, 59, 67, 72, 89 };
	for (int key : keys) {
		h1.Insert(key);
		h2.Insert(key);
		h3.Insert(key);
		h4.Insert(key);
	}

	// Function to test a hash table
	auto testHashTable = [](HashTable& ht, const string& name) {
		cout << "Testing " << name << ":\n";
		ht.Print();

		// Access
		cout << "Accessing 31: " << ht.Access(31).first << endl;

		// Update
		ht.Update(31, 100);
		ht.Print();
		cout << "Accessing updated 31 (now 100): " << ht.Access(100).first << endl;

		// Delete
		ht.Delete(10);
		ht.Print();
		cout << "Accessing 59: " << ht.Access(59).first << endl;
		ht.Delete(72);
		ht.Print();
		cout << "Accessing 72: " << ht.Access(72).first << endl;
		cout << endl;

		// Calculate average access cost
		int totalAccessCost = 0;
		int numAccesses = 0;
		vector<int> keys = { 10, 22, 31, 44, 59, 67, 72, 89 };
		for (int key : keys) {
			pair<int, int> result = ht.Access(key);
			if (result.first != -1) {
				totalAccessCost += result.second;
				numAccesses++;
			}
		}
		double avgAccessCost = static_cast<double>(totalAccessCost) / numAccesses;
		cout << "Average access cost for " << name << ": " << avgAccessCost << endl;
		};

	// Test each hash table
	testHashTable(h1, "Linear Probing");
	testHashTable(h2, "Linear Probing with Steps");
	testHashTable(h3, "Pseudo-Random Probing");
	testHashTable(h4, "Double Hashing");

	return 0;
}





