#include <iostream>
#include <vector>
#include <stack>
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
				coloring(gr, color, temp);
				// If the neighbor has the same color, the graph is not bipartite
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

int main()
{
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
		// 4.
		cout << "?" << endl;
		// 5.
		cout << "?" << endl;
		// 6a.
		//isBipartite(gr, edge_numbers, true); cout << endl;
		cout << "?" << endl;

		vector<ln> color(edge_numbers, -1);  // -1 means uncolored, 0 and 1 are the two colors

		for (ln start = 0; start < edge_numbers; ++start) {
			if (color[start] == -1) {  // Not yet colored
				coloring(gr, color, start);
			}
		}


		// 6b.
		cout << "?" << endl;
		// 6c.
		cout << "?" << endl;
		// 7.
		cout << "?" << endl;
		// 8. liczba krawędzi dopełnienia grafu 
		cout << (edge_numbers * (edge_numbers - 1) / 2) - edges / 2 << endl;

		cout << "\n\n";
	}
}
