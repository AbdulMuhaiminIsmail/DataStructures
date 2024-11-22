#pragma once
#include <iostream>
#include <vector>
using namespace std;

class HashTable {
private:
	int* arr;				// Array to store keys
	int step;				// Step of linear probing
	int tableSize;			// Size of the hash table (odd number)
	bool rehashFlag;		// Doubles table size when load factor > 0.5
	int currentLoad;		// Number of stored elements
	int collisionMethod;	// Specifies the collision resolution method

	// Hash Functions
	int hash1(int) const;
	int hash2(int) const;

	// Collision Resolution Methods
	pair<int, int> resolveCollision(int, int);
	pair<int, int> linearProbing(int, int);
	pair<int, int> linearProbingWithSteps(int, int, int);
	pair<int, int> pseudoRandomProbing(int, int);
	pair<int, int> doubleHashing(int, int, int);

	// Helper Functions
	pair<int, int> search(int, int);
	void rehash();


public:
	//Constructor
	HashTable(int, int, bool);

	//Destructor
	~HashTable();

	// Hash Table Functions
	void Insert(int);
	void Delete(int);
	void Update(int, int);
	pair<int, int> Access(int);
	void Print();
};

