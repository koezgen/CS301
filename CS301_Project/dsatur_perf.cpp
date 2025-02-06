#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <numeric>
#include <fstream>

using namespace std;

vector<vector<int> > generate_random_graph_adj_matrix(int size, double edge_probability) {
    vector<vector<int> > matrix(size, vector<int>(size, 0));
    random_device rd;
    mt19937 gen(rd());
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

vector<int> dsatur(const vector<vector<int> >& graph, int num_colors) {
    int n = graph.size();
    vector<int> degrees(n);
    vector<int> coloring(n, -1);
    vector<int> saturation_degrees(n, 0);

    for (int i = 0; i < n; ++i) {
        degrees[i] = count(graph[i].begin(), graph[i].end(), 1);
    }

    for (int _ = 0; _ < n; ++_) {
        int max_saturation = -1;
        int vertex_to_color = -1;
        for (int i = 0; i < n; ++i) {
            if (coloring[i] == -1) {
                if (saturation_degrees[i] > max_saturation || (saturation_degrees[i] == max_saturation && degrees[i] > degrees[vertex_to_color])) {
                    max_saturation = saturation_degrees[i];
                    vertex_to_color = i;
                }
            }
        }

        for (int color = 0; color < num_colors; ++color) {
            bool can_color = true;
            for (int j = 0; j < n; ++j) {
                if (graph[vertex_to_color][j] == 1 && coloring[j] == color) {
                    can_color = false;
                    break;
                }
            }
            if (can_color) {
                coloring[vertex_to_color] = color;
                break;
            }
        }

        for (int i = 0; i < n; ++i) {
            if (graph[vertex_to_color][i] == 1 && coloring[i] == -1) {
                saturation_degrees[i]++;
            }
        }
    }

    return coloring;
}

void measure_performance() {
    const int k = 200;
    const int min_size = 5;
    const int max_size = 30;
    const double edge_probability = 0.5;

    ofstream output_file("performance_data.csv");
    output_file << "InputSize,MeanTime\n";

    for (int size = min_size; size <= max_size; ++size) {
        vector<double> times;
        for (int i = 0; i < k; ++i) {
            vector<vector<int> > graph = generate_random_graph_adj_matrix(size, edge_probability);
            auto start = chrono::high_resolution_clock::now();
            dsatur(graph, size);
            auto end = chrono::high_resolution_clock::now();
            chrono::duration<double> duration = end - start;
            times.push_back(duration.count());
        }

        double mean_time = accumulate(times.begin(), times.end(), 0.0) / times.size();
        output_file << size << "," << mean_time << "\n";
    }

    output_file.close();
}

int main() {
    measure_performance();
    return 0;
}
