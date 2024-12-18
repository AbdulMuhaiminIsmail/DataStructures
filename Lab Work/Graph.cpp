#include <iostream>
#include <vector>
#include <queue>
#include <list>
#include <string>
#include <utility>

using namespace std;

template <typename T> class Graph {
private:
	int vertices;
	bool undirectedGraph;
	vector<list<int>> adjList;

	void dfs(const int& vertex, vector<bool>& visited, vector<int>& dfsTraversal) const {
		if (visited[vertex] == false) {
			dfsTraversal.push_back(vertex);
			visited[vertex] = true;
		}

		for (int neighbour : adjList[vertex]) {
			if (visited[neighbour] == false) {
				dfs(neighbour, visited, dfsTraversal);
			}
		}
	}

	void dfsSpanningTree(const int& vertex, vector<bool>& visited) const {
		if (visited[vertex] == false) {
			visited[vertex] = true;
		}

		for (int neighbour : adjList[vertex]) {
			if (visited[neighbour] == false) {
				cout << "(" << vertex << "," << neighbour << ")" << endl;
				dfsSpanningTree(neighbour, visited);
			}
		}
	}

	vector<int> bfs(const int& vertex, vector<bool>& visited) const {
		vector<int> bfsTraversal;
		queue<int> queue;

		queue.push(vertex);
		visited[vertex] = true;

		while (!queue.empty()) {
			int temp = queue.front();
			queue.pop();
			bfsTraversal.push_back(temp);

			for (int neighbour : adjList[temp]) {
				if (visited[neighbour] == false) {
					queue.push(neighbour);
					visited[neighbour] = true;
				}
			}
		}

		return bfsTraversal;
	}

	void bfsSpanningTree(const int& vertex, vector<bool>& visited) const {
		queue<int> queue;

		queue.push(vertex);
		visited[vertex] = true;

		while (!queue.empty()) {
			int temp = queue.front();
			queue.pop();

			for (int neighbour : adjList[temp]) {
				if (visited[neighbour] == false) {
					cout << "(" << temp << "," << neighbour << ")" << endl;
					queue.push(neighbour);
					visited[neighbour] = true;
				}
			}
		}
	}

	bool hasCycleDFS(const int& vertex, const int& parent, vector<bool>& visited) {
		visited[vertex] = true;
		for (int neighbour : adjList[vertex]) {
			if (visited[neighbour] == false) {
				if (hasCycleDFS(neighbour, vertex, visited) == true) {
					return true;
				}
			}
			else if (neighbour != parent) {
				return true;
			}
		}
		return false;
	}

public:
	Graph(bool undirectedGraph) : vertices(0), undirectedGraph(undirectedGraph) {}

	void addEdge(const int& vertex1, const int& vertex2) {
		if (vertex1 < 0 || vertex1 >= vertices || vertex2 < 0 || vertex2 >= vertices) return;
		if (edgeExists(vertex1, vertex2)) return;

		adjList[vertex1].push_back(vertex2);
		if (undirectedGraph && vertex1 != vertex2) {
			adjList[vertex2].push_back(vertex1);
		}
	}

	void removeEdge(const int& vertex1, const int& vertex2) {
		if (vertex1 < 0 || vertex1 >= vertices || vertex2 < 0 || vertex2 >= vertices) return;
		if (!edgeExists(vertex1, vertex2)) return;

		adjList[vertex1].remove(vertex2);
		if (undirectedGraph && vertex1 != vertex2) {
			adjList[vertex2].remove(vertex1);
		}
	}

	void addVertex() {
		adjList.push_back(list<int>());
		vertices = vertices + 1;
	}

	 the list shifts if some element is removed so if data vector is mapped handle it aswell
	void removeVertex(const int& vertex) {
		vector<list<int>>::iterator iter;
		int i = 0;
		for (auto it = adjList.begin(); it != adjList.end(); ++it, ++i) {
			if (i == vertex) iter = it;
			removeEdge(i, vertex);
		}
		adjList.erase(iter);
		vertices = vertices - 1;
	}

	bool edgeExists(const int& vertex1, const int& vertex2) {
		list<int>::iterator it = find(adjList[vertex1].begin(), adjList[vertex1].end(), vertex2);
		return (it != adjList[vertex1].end());
	}

	bool isConnected() {
		vector<bool> visited(vertices, false);
		bfs(0, visited);

		for (int vertex : visited) {
			if (vertex == false) return false;
		}
		return true;
	}

	int connectedComponentsCount() {
		if (isConnected()) return 1;

		int count = 1;
		vector<bool> visited(vertices, false);
		bfs(0, visited);

		for (int i = 0; i < vertices; i++) {
			if (visited[i] == false) {
				count++;
				bfs(i, visited);
			}
		}

		return count;
	}

	vector<int> dfs(const int& vertex) const {
		vector<bool> visited(vertices, false);
		vector<int> dfsTraversal;
		dfs(vertex, visited, dfsTraversal);
		return dfsTraversal;
	}

	vector<int> bfs(const int& vertex) const {
		vector<bool> visited(vertices, false);
		return bfs(vertex, visited);
	}

	void bfsTree(const int& vertex) {

	}

	void bfsSpanningTree(const int& start) {
		vector<bool> visited(vertices, false);
		bfsSpanningTree(start, visited);
	}

	void dfsSpanningTree(const int& start) {
		vector<bool> visited(vertices, false);
		dfsSpanningTree(start, visited);
	}

	bool hasCycleDFS(const int& vertex, const int& parent) {
		vector<bool> visited(vertices, false);
		return hasCycleDFS(vertex, parent, visited);
	}

	void inputGraph() {
		int vertices, edges;

		cout << "Enter the number of vertices: ";
		cin >> vertices;
		cout << "Enter the number of edges: ";
		cin >> edges;

		this->vertices = vertices;
		adjList.resize(vertices);
		cin.ignore();

		for (int i = 0; i < edges; i++) {
			string pair = "";
			cout << "Enter Pair (U,V) for Edge " << i + 1 << ": ";
			getline(cin, pair);
			addEdge((pair[0] - '0'), (pair[2] - '0'));
		}
	}

	void makeSampleGraph() {
		//	0 -- 1
		//	|    |
		//	3 -- 2

		int v = 4;
		vertices = v;
		adjList.resize(vertices);
		addEdge(0, 1);
		addEdge(1, 2);
		addEdge(2, 3);
		addEdge(3, 0);

		 1 -- 2 -- 3 -- 4

		int v = 4;
		vertices = v;
		adjList.resize(vertices);
		addEdge(0, 1);
		addEdge(1, 2);
		addEdge(2, 3);
	}

	void bfsTraversals() {
		for (int i = 0; i < vertices; ++i) {
			cout << "BFS Traversal starting from vertex :" << i << endl;
			vector<int> bfsTraversal = bfs(i);
			for (int vertex : bfsTraversal) {
				cout << vertex << " ";
			}
			cout << endl;
		}
	}

	void dfsTraversals() {
		for (int i = 0; i < vertices; ++i) {
			cout << "DFS Traversal starting from vertex :" << i << endl;
			vector<int> dfsTraversal = dfs(i);
			for (int vertex : dfsTraversal) {
				cout << vertex << " ";
			}
			cout << endl;
		}
	}

	void bfsSpanningTrees() {
		for (int i = 0; i < vertices; ++i) {
			cout << "BFS Spanning Tree starting from vertex :" << i << endl;
			bfsSpanningTree(i);
		}		
	}

	void dfsSpanningTrees() {
		for (int i = 0; i < vertices; ++i) {
			cout << "DFS Spanning Tree starting from vertex :" << i << endl;
			dfsSpanningTree(i);
		}
	}

	void printAdjList() {
		cout << "Adjacency List: " << endl;
		for (int i = 0; i < vertices; i++) {
			cout << "V" << i << "\t|\t";

			if (adjList[i].begin() == adjList[i].end()) {
				cout << "-" << endl;
				continue;
			}

			for (auto it2 = adjList[i].begin(); it2 != adjList[i].end();) {
				cout << *it2;
				if (++it2 != adjList[i].end()) cout << ", ";
			}

			cout << endl;
		}
	}
};

int main() {
	 Create an undirected graph
	Graph<int> undirectedGraph(true);
	undirectedGraph.makeSampleGraph();
	undirectedGraph.addEdge(3, 4);
	undirectedGraph.printAdjList();
	undirectedGraph.addVertex();
	undirectedGraph.addEdge(3, 4);
	undirectedGraph.printAdjList();
	undirectedGraph.removeVertex(1);
	undirectedGraph.printAdjList();

	// Print adjacency list
	cout << "Undirected Graph:" << endl;
	undirectedGraph.printAdjList();

	// Check if the graph is connected
	cout << "Is the graph connected? " << (undirectedGraph.isConnected() ? "Yes" : "No") << endl;

	// Get the number of connected components
	cout << "Number of connected components: " << undirectedGraph.connectedComponentsCount() << endl;

	// Check if the graph has a cycle
	cout << "Does the graph has a cycle? " << (undirectedGraph.hasCycleDFS(0, -1) ? "Yes" : "No") << endl;

	// Perform BFS traversals
	cout << "BFS Traversals starting from each vertex:" << endl;
	undirectedGraph.bfsTraversals();

	// Perform BFS traversals
	cout << "DFS Traversals starting from each vertex:" << endl;
	undirectedGraph.dfsTraversals();

	// Print BFS spanning trees
	cout << "BFS Spanning Trees starting from each vertex:" << endl;
	undirectedGraph.bfsSpanningTrees();

	// Print DFS spanning trees
	cout << "DFS Spanning Trees starting from each vertex:" << endl;
	undirectedGraph.dfsSpanningTrees();

	return 0;
}

