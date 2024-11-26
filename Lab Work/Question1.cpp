#include <iostream>
#include <vector>
#include <list>
#include <string>
using namespace std;

template <typename T> class Graph {
private:
	int vertices;
	bool undirectedGraph;
	vector<list<int>> adj;

public:
	Graph(bool undirectedGraph) : vertices(0), adj(vertices), undirectedGraph(undirectedGraph) {}

	void addEdge(int vertex1, int vertex2) {
		if (edgeExists(vertex1, vertex2)) return;
		if (vertex1 < 0 || vertex1 >= vertices || vertex2 < 0 || vertex2 >= vertices) return;
		if (undirectedGraph && vertex1 != vertex2) adj[vertex2].push_back(vertex1);
		adj[vertex1].push_back(vertex2);
	}

	bool edgeExists(int vertex1, int vertex2) {
		list<int>& adjList = adj[vertex1];
		for (const auto& edge : adjList) {
			if (edge == vertex2) return true;
		}
		return false;
	}

	void inputGraph() {
		int vertices, edges;
		vector<pair<int, int>> uvPairs;

		cout << "Enter the number of vertices: ";
		cin >> vertices;
		cout << "Enter the number of edges: ";
		cin >> edges;

		this->vertices = vertices;
		string pair;
		cin.ignore();
		for (int i = 0; i < edges; i++) {
			cout << "Enter Pair (U,V) for Edge " << i+1 << " ";
			getline(cin, pair);
			uvPairs.push_back({ static_cast<int>(pair[0]), static_cast<int>(pair[2]) });
		}

		for (const auto& pair : uvPairs) {
			addEdge((pair.first), (pair.second));
		}
	}

	void printAdjList() {
		for (int i = 0; i < vertices; i++) {
			cout << "V" << i << "\t|\t";
			for (auto it2 = adj[i].begin(); it2 != adj[i].end();) {
				cout << *it2;
				if (++it2 != adj[i].end()) cout << ", ";
			}
			cout << endl;
		}
	}
};

int main() {
	Graph<int> graph(true);
	graph.inputGraph();
	graph.printAdjList();
}