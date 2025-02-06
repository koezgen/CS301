#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

// Class representing an undirected graph
class Graph {
    int n; // Number of vertices
    vector<vector<int>> adj; // Adjacency list

public:
    Graph(int numNodes) {
        n = numNodes;
        adj.resize(n, vector<int>());
    }

    ~Graph() { adj.clear(); }

    void addEdge(int u, int v);
    void printAdjMatrix();
    bool isValidColoring(const vector<int>& color);
    int bruteForceColoring();
};

void Graph::addEdge(int u, int v) {
    adj[u].push_back(v);
    adj[v].push_back(u);
}

void Graph::printAdjMatrix() {
    vector<vector<int>> matrix(n, vector<int>(n, 0));
    for (int i = 0; i < n; ++i) {
        for (int j : adj[i]) {
            matrix[i][j] = 1;
        }
    }

    for (const auto& row : matrix) {
        for (int val : row) {
            cout << val << " ";
        }
        cout << endl;
    }
}

bool Graph::isValidColoring(const vector<int>& color) {
    for (int u = 0; u < n; ++u) {
        for (int v : adj[u]) {
            if (color[u] == color[v]) {
                return false;
            }
        }
    }
    return true;
}

int Graph::bruteForceColoring() {
    vector<int> color(n, 0);
    int chromaticNumber = n;

    for (int numColors = 1; numColors <= n; ++numColors) {
        fill(color.begin(), color.end(), 0);
        bool found = false;
        do {
            if (isValidColoring(color)) {
                chromaticNumber = numColors;
                found = true;
                break;
            }
        } while (next_permutation(color.begin(), color.end()));
        
        if (found) break;
    }
    return chromaticNumber;
}

// Driver Code
int main() {
    Graph G1(5);
    G1.addEdge(0, 1);
    G1.addEdge(0, 2);
    G1.addEdge(1, 2);
    G1.addEdge(1, 3);
    G1.addEdge(2, 3);
    G1.addEdge(3, 4);

    cout << "Adjacency matrix of graph G1:\n";
    G1.printAdjMatrix();
    cout << "Chromatic number of graph G1: " << G1.bruteForceColoring() << "\n";
    cout << "-----------------------\n";

    Graph G2(5);
    G2.addEdge(0, 1);
    G2.addEdge(0, 2);
    G2.addEdge(1, 2);
    G2.addEdge(1, 4);
    G2.addEdge(2, 4);
    G2.addEdge(4, 3);

    cout << "Adjacency matrix of graph G2:\n";
    G2.printAdjMatrix();
    cout << "Chromatic number of graph G2: " << G2.bruteForceColoring() << "\n";
    cout << "-----------------------\n";

    return 0;
}
