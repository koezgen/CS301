#include <iostream>
#include <set>
#include <tuple>
#include <vector>
#include <algorithm>

#include <numeric>
#include <random>
using namespace std;

const int MATRIX_SIZE = 5;
const int MATRIX_AMOUNT = 100;

struct nodeInfo {
    int sat;
    int deg;
    int vertex;
};

struct maxSat {
    bool operator()(const nodeInfo& lhs, const nodeInfo& rhs) const {
        return tie(lhs.sat, lhs.deg, lhs.vertex)
               > tie(rhs.sat, rhs.deg, rhs.vertex);
    }
};

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
    int DSatur();
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
    vector<int> color(n);
    iota(color.begin(), color.end(), 0);  // Initialize colors to 0, 1, 2, ..., n-1

    int chromaticNumber = n;
    for (int numColors = 1; numColors <= n; ++numColors) {
        bool found = false;
        vector<int> color(numColors, 0);
        do {
            if (isValidColoring(color)) {
                chromaticNumber = numColors;
                found = true;
                break;
            }
            int pos = 0;
            while (pos < n && ++color[pos] == numColors) {
                color[pos] = 0;
                ++pos;
            }
            if (pos == n) break;
        } while (true);
        if (found) break;
    }
    return chromaticNumber;
}

int Graph::DSatur() {
    int u, i;
    vector<bool> used(n, false);
    vector<int> c(n), d(n);
    vector<set<int>> adjCols(n);
    set<nodeInfo, maxSat> Q;

    for (u = 0; u < n; u++) {
        c[u] = -1;
        d[u] = adj[u].size();
        adjCols[u] = set<int>();
        Q.emplace(nodeInfo{0, d[u], u});
    }

    while (!Q.empty()) {
        auto maxPtr = Q.begin();
        u = (*maxPtr).vertex;
        Q.erase(maxPtr);

        for (int v : adj[u]) {
            if (c[v] != -1) {
                used[c[v]] = true;
            }
        }
        for (i = 0; i < used.size(); i++) {
            if (!used[i]) {
                break;
            }
        }
        for (int v : adj[u]) {
            if (c[v] != -1) {
                used[c[v]] = false;
            }
        }
        c[u] = i;

        for (int v : adj[u]) {
            if (c[v] == -1) {
                Q.erase({int(adjCols[v].size()), d[v], v});
                adjCols[v].insert(i);
                d[v]--;
                Q.emplace(nodeInfo{int(adjCols[v].size()), d[v], v});
            }
        }
    }

    int chromaticNumber = 0;
    for (u = 0; u < n; u++) {
        chromaticNumber = max(chromaticNumber, c[u] + 1);
    }
    return chromaticNumber;
}

// Function to generate a random graph adjacency matrix
vector<vector<int>> generate_random_graph_adj_matrix(int size, double edge_probability) {
    vector<vector<int>> matrix(size, vector<int>(size, 0));
    random_device rd;  // Seed for the random number engine
    mt19937 gen(rd()); // Mersenne Twister engine seeded with rd()
    uniform_real_distribution<> dis(0.0, 1.0);

    for (int i = 0; i < size; ++i) {
        for (int j = i + 1; j < size; ++j) {
            if (dis(gen) < edge_probability) {
                matrix[i][j] = 1;
                matrix[j][i] = 1;
            }
        }
    }

    return matrix;
}

// Function to convert adjacency matrix to adjacency list
Graph adj_matrix_to_graph(const vector<vector<int>>& matrix) {
    int size = matrix.size();
    Graph graph(size);
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            if (matrix[i][j] == 1) {
                graph.addEdge(i, j);
            }
        }
    }
    return graph;
}

// Driver Code
int main() {
    for (int i = 0; i < MATRIX_AMOUNT; ++i) {
        auto matrix = generate_random_graph_adj_matrix(MATRIX_SIZE, 0.5);
        Graph graph = adj_matrix_to_graph(matrix);

        cout << "Adjacency matrix of graph " << i + 1 << ":\n";
        graph.printAdjMatrix();
        cout << "Chromatic number of graph " << i + 1 << " (Brute Force): " << graph.bruteForceColoring() << "\n";
        cout << "Chromatic number of graph " << i + 1 << " (DSatur): " << graph.DSatur() << "\n";
        cout << "-----------------------\n";
    }
    return 0;
}
