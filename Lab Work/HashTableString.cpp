//#include <iostream>
//#include <vector>
//#include <iomanip>
//using namespace std;
//
//class HashTable {
//private:
//	struct Node {
//		string key; // City name
//		int value; // Air pollution index
//		Node* next; // Pointer for chaining
//
//		Node(string key, int value, Node* next) : key(key), value(value), next(next) {}
//	};
//
//	int tableSize; // Size of the hash table
//	Node** table; // Array of linked lists
//
//	int hash(string key) const;
//public:
//	HashTable(int size); // Constructor
//	~HashTable();		 // Destructor
//	void Insert(std::string key, int value);
//	void Delete(std::string key);
//	void Update(std::string key, int newValue);
//	int Access(std::string key);
//	void Print();
//};
//
//// Constructor
//HashTable::HashTable(int size = 11) {
//	this->tableSize = size;
//	table = new Node*[tableSize];
//
//	for (int i = 0; i < tableSize; i++) {
//		table[i] = nullptr;
//	}
//}
//
//// Hash Functions
//int HashTable::hash(string key) const {
//	const int a = 33;
//	int hashCode = 0, currCode;
//	for (int i = 0; i < key.size(); i++) {
//		currCode = (key[i] * pow(a, i));
//		hashCode += currCode % INT_MAX;
//	}
//	return hashCode % tableSize;
//}
//
//#include <iomanip> // Include the iomanip library for setw
//
//void HashTable::Print() {
//	cout << "HashTable Contents:" << endl;
//	cout << "-------------------" << endl;
//	cout << left << setw(20) << "Key" << " | " << "Value" << endl;
//	cout << "-------------------" << endl;
//	for (int i = 0; i < tableSize; ++i) {
//		if (table[i] == nullptr) {
//			cout << left << setw(20) << "No key" << " | " << "No value" << endl;
//		}
//		else {
//			Node* iter = table[i];
//			while (iter != nullptr) {
//				cout << left << setw(20) << iter->key << " | " << iter->value << endl;
//				iter = iter->next;
//			}
//		}
//	}
//	cout << "-------------------" << endl;
//}
//
//
//// Main Functions
//void HashTable::Insert(string key, int value) {
//	int hashValue = hash(key);
//	Node* node = new Node(key, value, nullptr);
//
//	if (table[hashValue] == nullptr) {
//		table[hashValue] = node;
//		return;
//	}
//
//	Node* iter;
//	for (iter = table[hashValue]; iter->next; iter = iter->next);
//	iter->next = node;
//}
//
//void HashTable::Delete(string key) {
//	int hashValue = hash(key);
//
//	if (table[hashValue] == nullptr) return;
//
//	if (table[hashValue]->key == key) {
//		Node* node = table[hashValue];
//		table[hashValue] = node->next;
//		delete node;
//		node = nullptr;
//		return;
//	}
//
//	Node* iter;
//	for (iter = table[hashValue]; iter->next && iter->next->key != key; iter = iter->next);
//	
//	if (iter->next != nullptr) {
//		Node* node = iter->next;
//		iter->next = node->next;
//		delete node;
//		node = nullptr;
//	}
//}
//
//void HashTable::Update(string key, int newValue) {
//	int hashValue = hash(key);
//
//	if (table[hashValue] == nullptr) return;
//
//	for (Node* iter = table[hashValue]; iter; iter = iter->next) {
//		if (iter->key == key) iter->value = newValue;
//		return;
//	}
//}
//
//int HashTable::Access(string key) {
//	int hashValue = hash(key);
//
//	if (table[hashValue] == nullptr) return -1;
//
//	for (Node* iter = table[hashValue]; iter; iter = iter->next) {
//		if (iter->key == key) return iter->value;
//	}
//
//	return -1;
//}
//
//// Destructor
//HashTable::~HashTable() {
//	for (int i = 0; i < tableSize; i++) {
//		if (table[i] != nullptr) {
//			Node* iter = table[i];
//			while (iter) {
//				Node* temp = iter->next;
//				delete iter;
//				iter = temp;
//			}
//		}
//	}
//	delete table;
//	table = nullptr;
//}
//
//void insertCityData(HashTable& hashTable) {
//	// Example data for 50 cities with arbitrary pollution indices
//	vector<pair<string, int>> cities = {
//		{"Karachi", 45}, {"Lahore", 60}, {"Islamabad", 30}, {"Peshawar", 50},
//		{"Quetta", 40}, {"Multan", 55}, {"Faisalabad", 65}, {"Hyderabad", 48},
//		{"Rawalpindi", 35}, {"Sialkot", 38}, {"Gujranwala", 52}, {"Sargodha", 49},
//		{"Bahawalpur", 42}, {"Sukkur", 57}, {"Abbottabad", 25}, {"Mardan", 37},
//		{"Sheikhupura", 39}, {"Jhelum", 29}, {"Sahiwal", 41}, {"Kasur", 44},
//		{"Dera Ghazi Khan", 51}, {"Nawabshah", 43}, {"Mansehra", 28}, {"Rahim Yar Khan", 54},
//		{"Chiniot", 31}, {"Mirpur Khas", 36}, {"Swat", 33}, {"Muzaffarabad", 27},
//		{"Okara", 46}, {"Larkana", 50}, {"Gilgit", 20}, {"Skardu", 22},
//		{"Hunza", 18}, {"Gwadar", 21}, {"Khuzdar", 34}, {"Turbat", 32},
//		{"Chitral", 23}, {"Zhob", 24}, {"Khairpur", 26}, {"Shikarpur", 30},
//		{"Dera Ismail Khan", 39}, {"Vehari", 35}, {"Mianwali", 40}, {"Narowal", 38},
//		{"Kohat", 29}, {"Hafizabad", 33}, {"Pakpattan", 31}, {"Badin", 34},
//		{"Kotli", 28}, {"Attock", 25}
//	};
//
//	// Insert all city data into the hash table
//	for (const auto& city : cities) {
//		hashTable.Insert(city.first, city.second);
//	}
//}
//
//int main() {
//	// Initialize the hash table with a reasonable size
//	HashTable hashTable(50);
//
//	// Insert data for 50 cities using the helper function
//	insertCityData(hashTable);
//
//	// Print the hash table after insertion
//	hashTable.Print();
//
//	// Test Access: Retrieve pollution index for multiple cities
//	std::cout << "Air pollution index of Karachi: " << hashTable.Access("Karachi") << std::endl;
//	std::cout << "Air pollution index of Lahore: " << hashTable.Access("Lahore") << std::endl;
//	std::cout << "Air pollution index of Gilgit: " << hashTable.Access("Gilgit") << std::endl;
//
//	// Test Update: Update pollution index of specific cities
//	hashTable.Update("Karachi", 55);
//	hashTable.Update("Quetta", 45);
//	std::cout << "Updated air pollution index of Karachi: " << hashTable.Access("Karachi") << std::endl;
//	std::cout << "Updated air pollution index of Quetta: " << hashTable.Access("Quetta") << std::endl;
//
//	// Print the hash table after updates
//	hashTable.Print();
//
//	// Test Deletion: Remove data for a city
//	hashTable.Delete("Peshawar");
//	std::cout << "Air pollution index of Peshawar after deletion: " << hashTable.Access("Peshawar") << std::endl;
//
//	// Print the hash table after deletion
//	hashTable.Print();
//
//	// Test Edge Cases:
//	// 1. Accessing a city that doesn't exist
//	std::cout << "Air pollution index of NonExistentCity: " << hashTable.Access("NonExistentCity") << std::endl;
//
//	// 2. Deleting a city that doesn't exist
//	hashTable.Delete("NonExistentCity");
//
//	// 3. Updating a city that doesn't exist
//	hashTable.Update("NonExistentCity", 100);
//
//	// 4. Inserting a city with an existing key (should overwrite or handle appropriately)
//	hashTable.Insert("Karachi", 65); // Reinserting Karachi with a new value
//	std::cout << "Reinserted air pollution index of Karachi: " << hashTable.Access("Karachi") << std::endl;
//
//	// Print the hash table after reinserting Karachi
//	hashTable.Print();
//
//	return 0;
//}
//
