#include <iostream>
#include <unordered_map>
#include "Heap.cpp"

using namespace std;

class HuffmanCoder {
private:
	class Node {
	private:
		int freqCount;
		char character;
		Node* left;
		Node* right;

	public:
		Node(char c, int f, Node* l = nullptr, Node* r = nullptr) : character(c), freqCount(f), left(l), right(r) {}

		// Getters
		int getFreqCount() const { return freqCount; }
		char getCharacter() const { return character; }
		Node* getLeft() const { return left; }
		Node* getRight() const { return right; }

		// Setters
		void setFreqCount(int f) { freqCount = f; }
		void setCharacter(char c) { character = c; }
		void setLeft(Node* l) { left = l; }
		void setRight(Node* r) { right = r; }

		// Operator overloads for Node
		bool operator < (const Node& node) {
			return this->freqCount < node.freqCount;
		}

		bool operator > (const Node& node) {
			return this->freqCount > node.freqCount;
		}
	};


	string text;
	string encodedText;
	Heap<Node*> priorityQueue;
	unordered_map<char, int> freqMap;
	unordered_map<char, string> encoder;
	unordered_map<string, char> decoder;

	void buildFreqMap() {
		for (const auto& ch : text) {
			++freqMap[ch];
		}
	}
	
	void buildPriorityQueue() {
		for (const auto& pair : freqMap) {
			Node* node = new Node(pair.first, pair.second);
			priorityQueue.insert(node);
		}
	}

	void buildHuffmanTree() {
		while (priorityQueue.getSize() > 1) {
			Node* nodeX = priorityQueue.at(0);
			priorityQueue.remove();
			Node* nodeY = priorityQueue.at(0);
			priorityQueue.remove();

			Node* nodeZ = new Node('*', nodeX->getFreqCount() + nodeY->getFreqCount(), nodeX, nodeY);
			priorityQueue.insert(nodeZ);
		}
	}

	void buildEncoder() {
		buildEncoder(priorityQueue.at(0), "");
	}

	void buildEncoder(Node* root, string code) {
		if (!root) return;

		if (root->getLeft() == nullptr && root->getRight() == nullptr) {
			encoder[root->getCharacter()] = code;
		}

		buildEncoder(root->getLeft(), code + '0');
		buildEncoder(root->getRight(), code + '1');
	}

public:
	HuffmanCoder(string text) : text(text) { encodedText = ""; }
	~HuffmanCoder() {}

	void encodeString() {
		cout << "Building frequency map ..." << endl;
		buildFreqMap(); // Step 1: Build the frequency map from the input text

		cout << "Building priority queue ..." << endl;
		buildPriorityQueue(); // Step 2: Build the priority queue (min-heap) from the frequency map

		cout << "Building Huffman tree ..." << endl;
		buildHuffmanTree(); // Step 3: Build the Huffman tree from the priority queue

		cout << "Building encoder ..." << endl;
		buildEncoder(); // Step 4: Build the encoder map from the Huffman tree


		for (const auto& ch : text) {
			encodedText = encodedText + encoder[ch];
		}
	}

	void printCharFreqTable() {
		cout << "Character Frequency Table:" << endl;
		cout << "--------------------------" << endl;
		cout << "Character | Frequency" << endl;
		cout << "--------------------------" << endl;
		for (const auto& pair : freqMap) {
			cout << "    " << pair.first << "     |     " << pair.second << endl;
		}
		cout << "--------------------------" << endl;
	}

	void printCharCodesTable() {
		cout << "Character Codes Table:" << endl;
		cout << "----------------------" << endl;
		cout << "Character | Code" << endl;
		cout << "----------------------" << endl;
		for (const auto& pair : encoder) {
			cout << "    " << pair.first << "     | " << pair.second << endl;
		}
		cout << "----------------------" << endl;
	}


	void printCompressedBinaryString() {
		cout << "Compressed Binary String: " << encodedText << endl;
	}
};

int main() {
	string text;
	cout << "Enter string to be compressed: ";
	cin >> text;

	HuffmanCoder hmc(text);
	hmc.encodeString();
	hmc.printCharFreqTable();
	hmc.printCharCodesTable();
	hmc.printCompressedBinaryString();

	return 0;
}