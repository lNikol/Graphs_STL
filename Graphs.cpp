#include <iostream>
#include <vector>
#include <stack>
#include <set>
#include <algorithm>
using namespace std;
using ln = long long int;
using Graph = vector<vector<ln>>;

void DFS(const Graph& gr, vector<bool>& visited, ln start) {
    stack<ln> s;
    s.push(start);
    while (!s.empty()) {
        ln node = s.top();
        s.pop();
        if (!visited[node]) {
            visited[node] = true;
            for (ln n : gr[node]) {
                ln temp = n - 1 >= 0 ? n - 1 : n;
                if (!visited[temp]) {
                    s.push(temp);
                }
            }
        }
    }
}

bool iterativeDFS(const Graph& gr, vector<ln>& color, ln start) {
    stack<ln> s;
    s.push(start);
    color[start] = 0;  // Start coloring with 0

    while (!s.empty()) {
        ln node = s.top();
        s.pop();

        for (ln n : gr[node]) {
            ln temp = n - 1 >= 0 ? n - 1 : n;
            if (color[temp] == -1) {  // If not colored
                color[temp] = 1 - color[node];  // Color with opposite color
                s.push(temp);
            }
            else if (color[temp] == color[node]) {
                // If the neighbor has the same color, the graph is not bipartite
                return false;
            }
        }
    }
    return true;
}

void coloring(const Graph& gr, vector<ln>& color, ln start) {
    stack<ln> s;
    s.push(start);
    color[start] = 0;  // Start coloring with 0

    while (!s.empty()) {
        ln node = s.top();
        s.pop();

        for (ln n : gr[node]) {
            ln temp = n - 1 >= 0 ? n - 1 : n;
            if (color[temp] == -1) {  // If not colored
                color[temp] = 1 - color[node];  // Color with opposite color
                s.push(temp);
            }
            else if (color[temp] == color[node]) {
                // If the neighbor has the same color, the graph is not bipartite
                break;
            }
        }
    }
}

bool isBipartite(const Graph& gr, ln& size) {
    vector<ln> color(size, -1);  // -1 means uncolored, 0 and 1 are the two colors

    for (ln start = 0; start < size; ++start) {
        if (color[start] == -1) {  // Not yet colored
            if (!iterativeDFS(gr, color, start)) {
                return false;
            }
        }
    }
    return true;
}

ln countComponents(const Graph& gr, const ln& size) {
    ln components = 0;
    vector<bool> visited(size, false);

    for (ln i = 0; i < size; ++i) {
        if (!visited[i]) {
            DFS(gr, visited, i);
            ++components;
        }
    }

    return components;
}

// Zmienić kopie parametrów na referencje

void DFSCountC4(const Graph& gr, ln start, ln current, ln depth, ln& count, ln parent) {
    if (depth == 4) {
        if (current == start) {
            count++;
        }
        return;
    }

    for (ln n : gr[current]) {
        ln temp = n - 1 >= 0 ? n - 1 : n;
        if (temp != parent) {
            DFSCountC4(gr, start, temp, depth + 1, count, current);
        }
    }
}

ln countC4Subgraphs(const Graph& gr) {
    ln V = gr.size();
    ln count = 0;

    for (ln start = 0; start < V; ++start) {
        DFSCountC4(gr, start, start, 0, count, -1);
    }

    return count / 8; // Each C4 cycle is counted 8 times (4 vertices and 2 directions)
}

vector<ln> bfs(const Graph& gr, ln start) {
    vector<ln> distance(gr.size(), -1);
    ln* queue = new ln[gr.size()];
    ln front = 0, back = 0;

    queue[back++] = start;
    distance[start] = 0;

    while (front != back) {
        ln node = queue[front++];
        for (ln neighbor : gr[node]) {
            ln temp = neighbor - 1 >= 0 ? neighbor - 1 : neighbor;
            if (distance[temp] == -1) {
                distance[temp] = distance[node] + 1;
                queue[back++] = temp;
            }
        }
    }
    delete[] queue;
    return distance;
}

vector<ln> eccentricities(const Graph& gr) {
    vector<ln> ecc(gr.size(), -1);

    for (ln i = 0; i < gr.size(); ++i) {
        vector<ln> distances = bfs(gr, i);
        ecc[i] = *max_element(distances.begin(), distances.end());
    }

    return ecc;
}

// Funkcja kolorująca graf metodą zachłanną
void greedyColoring(Graph& gr, vector<ln>& result, const ln& method) {
    ln V = gr.size();
    vector<ln> order(V);
    for (ln i = 0; i < V; ++i) {
        order[i] = i;
    }

    result.assign(V, -1);  // Initialize all vertices as unassigned (-1)

    if (method == 1) {  // LF method
        sort(order.begin(), order.end(), [&gr](ln a, ln b) {
            if (gr[a].size() == gr[b].size()) return a < b;
            return gr[a].size() > gr[b].size();
            });
    }
    else if (method == 2) {  // SLF method (DSatur)
        vector<ln> degree(V);
        for (ln i = 0; i < V; ++i) {
            degree[i] = gr[i].size();
        }

        vector<ln> saturation(V, 0);
        vector<bool> usedColors(V, false);

        for (ln i = 0; i < V; ++i) {
            ln maxSaturation = -1, maxDegree = -1, selectedVertex = -1;
            for (ln j = 0; j < V; ++j) {
                if (result[j] == -1) {  // Only consider uncolored vertices
                    if (saturation[j] > maxSaturation ||
                        (saturation[j] == maxSaturation && degree[j] > maxDegree) ||
                        (saturation[j] == maxSaturation && degree[j] == maxDegree && j < selectedVertex)) {
                        maxSaturation = saturation[j];
                        maxDegree = degree[j];
                        selectedVertex = j;
                    }
                }
            }

            ln u = selectedVertex;
            result[u] = 0;  // Start coloring with 0

            // Mark colors of adjacent vertices as unavailable
            fill(usedColors.begin(), usedColors.end(), false);
            for (ln n : gr[u]) {
                ln temp = n - 1 >= 0 ? n - 1 : n;
                if (result[temp] != -1) {
                    usedColors[result[temp]] = true;
                }
            }

            // Find the first available color
            while (usedColors[result[u]]) {
                result[u]++;
            }

            // Update saturation degrees
            for (ln n : gr[u]) {
                ln temp = n - 1 >= 0 ? n - 1 : n;
                if (result[temp] == -1) {
                    set<ln> adjColors;
                    for (ln nn : gr[temp]) {
                        ln temp2 = nn - 1 >= 0 ? nn - 1 : nn;
                        if (result[temp2] != -1) {
                            adjColors.insert(result[temp2]);
                        }
                    }
                    saturation[temp] = adjColors.size();
                }
            }
        }
    }
    if (method != 2) {  // Default method
       
        result[order[0]] = 0;  // Assign the first color to the first vertex

        // A temporary array to store the available colors. False value means color is available.
        vector<bool> available(V, true);

        // Assign colors to remaining V-1 vertices
        for (ln i = 1; i < V; ++i) {
            ln u = order[i];
            // Process all adjacent vertices and mark their colors as unavailable
            for (ln n : gr[u]) {
                ln v = n - 1 >= 0 ? n - 1 : n;
                if (result[v] != -1) {
                    available[result[v]] = false;
                }
            }

            // Find the first available color
            ln cr;
            for (cr = 0; cr < V; ++cr) {
                if (available[cr]) {
                    break;
                }
            }

            result[u] = cr;  // Assign the found color to vertex u

            // Reset the values back to true for the next iteration
            for (ln n : gr[u]) {
                ln v = n - 1 >= 0 ? n - 1 : n;
                if (result[v] != -1) {
                    available[result[v]] = true;
                }
            }
        }
    }
}

int main() {
    ln graph_numbers;
    cin >> graph_numbers;
    for (ln i = 0; i < graph_numbers; ++i) {
        ln edge_numbers;
        ln edges = 0;
        cin >> edge_numbers;
        Graph gr(edge_numbers, vector<ln>());
        vector<ln> degrees;
        for (ln j = 0; j < edge_numbers; ++j) {
            ln neighbors;
            cin >> neighbors;
            edges += neighbors;
            degrees.push_back(neighbors);
            for (ln k = 0; k < neighbors; ++k) {
                ln num;
                cin >> num;
                gr[j].push_back(num);
            }
        }

        // 1. DEGREE
        sort(degrees.begin(), degrees.end(), greater<ln>());
        for (ln j = 0; j < edge_numbers; ++j) {
            cout << degrees[j] << " ";
        }
        cout << endl;
        degrees.clear();

        // 2. COMPONENTS (Liczba składowych spójności)
        cout << countComponents(gr, edge_numbers) << endl;
        // 3.
        cout << (isBipartite(gr, edge_numbers) ? "T" : "F") << endl;
        // 4. Acentryczność wierzchołków
        //vector<ln> ecc = eccentricities(gr);
        //for (ln e : ecc) {
        //    cout << e << " ";
        //}
        //cout << endl;
        cout << "?" << endl;

        // 5.
        cout << "?" << endl;
        // 6a.
        vector<ln> result;
        greedyColoring(gr, result, 0);
        // PRINT COLORING RESULT
        for (ln u = 0; u < result.size(); ++u) {
            cout << result[u] + 1 << " ";
        }
        cout << endl;
        result.clear();

        // 6b.
        greedyColoring(gr, result, 1);
        // PRINT COLORING RESULT
        for (ln u = 0; u < result.size(); ++u) {
            cout << result[u] + 1 << " ";
        }
        cout << endl;
        result.clear();
        // 6c.
        greedyColoring(gr, result, 2);
        for (ln u = 0; u < result.size(); ++u) {
            cout << result[u] + 1 << " ";
        }
        cout << endl;
        result.clear();
        // 7.
        cout << "?" << endl;

        // 8. liczba krawędzi dopełnienia grafu 
        cout << (edge_numbers * (edge_numbers - 1) / 2) - edges / 2 << endl;

        cout << "\n\n";
        gr.clear();
    }
}