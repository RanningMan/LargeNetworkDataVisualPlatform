
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <stdlib.h> 
#include <algorithm>
#include <cmath>
#include <ctime>
#include "Graph.h"

using namespace std;

struct Edge {
	int v1, v2;
	Edge(int V1, int V2) : v1(V1), v2(V2) {}
};

Graph g_down, g_up;

bool isEdge() {
	double val = (double)rand() / (double)RAND_MAX;
	if (val >= 0.6)
		return true;
	return false;
}

void getRandEdgeList(vector<int>& current_vertices, vector<Edge>& current_edges) {
	// for each combination of vertex pairs
	for (size_t i = 0; i < current_vertices.size()-1; i++) {
		for (size_t j = i+1; j < current_vertices.size(); j++) {
			if (isEdge()) {
				Edge e(current_vertices[i], current_vertices[j]);
				current_edges.push_back(e);
			}
		}
	}
}

// side effect: current_vertices and current_edges are altered
void queryDown(vector<int>& current_vertices, int v) {
	// add v's children to vertices
	EdgeList el = g_down.get_neighbors(v);
	if (el.size() > 0) {  // only can go down if has children
		for (EdgeList::iterator eit = el.begin(); eit != el.end(); eit++) {
			current_vertices.push_back(*eit);
		}
		
		// remove v itself
		vector<int>::iterator position = find(current_vertices.begin(), current_vertices.end(), v);
		if (position != current_vertices.end()) {
			current_vertices.erase(position);
		}
	}
}

void queryUp(vector<int>& current_vertices, int v) {
	// add v's parent to vertices
	EdgeList el = g_up.get_neighbors(v);
	if (el.size() > 0) {  // only can go up if has parent
		for (EdgeList::iterator eit = el.begin(); eit != el.end(); eit++) {
			current_vertices.push_back(*eit);
			
			// get children of parent (should include v and another vertex)
			EdgeList el_remove = g_down.get_neighbors(*eit);
			
			// remove those children (including v) from list
			for (EdgeList::iterator eit_remove = el_remove.begin(); eit_remove != el_remove.end(); eit_remove++) {
				vector<int>::iterator position = find(current_vertices.begin(), current_vertices.end(), *eit_remove);
				if (position != current_vertices.end())
					current_vertices.erase(position);
			}
		}
	}
}

void queryDownAll(vector<int>& current_vertices) {
	vector<int>::iterator it;
	EdgeList el;
	
	vector<int> new_vertices;
	
	for (it = current_vertices.begin(); it != current_vertices.end();) {
		// add v's children to vertices
		EdgeList el = g_down.get_neighbors(*it);
		if (el.size() > 0) {  // only can go down if has children
			for (EdgeList::iterator eit = el.begin(); eit != el.end(); eit++) {
				new_vertices.push_back(*eit);
			}
			
			// remove v itself
			bool erased = false;
			vector<int>::iterator position = find(current_vertices.begin(), current_vertices.end(), *it);
			if (position != current_vertices.end()) {
				it = current_vertices.erase(position);
				erased = true;
			}
			
			if (!erased)
				++it;
		}
		else {
			new_vertices.push_back(*it);  // doesn't have children, keep node
			++it;
		}
	}
	
	current_vertices = new_vertices;
}

void queryUpAll(vector<int>& current_vertices) {
	vector<int>::iterator it;
	EdgeList el;
	
	vector<int> new_vertices;
	
	for (it = current_vertices.begin(); it != current_vertices.end(); it++) {
		// add v's parent to vertices
		EdgeList el = g_up.get_neighbors(*it);
		if (el.size() > 0) {  // only can go up if has parent
			for (EdgeList::iterator eit = el.begin(); eit != el.end(); eit++) {
				if (find(new_vertices.begin(), new_vertices.end(), *eit) == new_vertices.end())
					new_vertices.push_back(*eit);
			}
		}
		else {
			new_vertices.push_back(*it);  // doesn't have parent, keep node
		}
	}
	
	current_vertices = new_vertices;
}

void generateFullTree(Graph& g_down, Graph& g_up, const int& iter) {
	for (int i = 0; i < iter; i++) {
		int left = 2 * i + 1;
		int right = 2 * i + 2;
		g_down.addEdge(i, left);
		g_down.addEdge(i, right);
		g_up.addEdge(left, i);
		g_up.addEdge(right, i);
	}
}

void run(const int height) {
	srand( time(NULL) );
	int node_size = pow(2, height+1) - 1;
	int iter = node_size - pow(2, height);  // total node_size - leaves (will be generated with left/right)
	g_down.init(node_size);
	g_up.init(node_size);
	generateFullTree(g_down, g_up, iter);
	/*
	string out_down = "down.gra";
	string out_up = "up.gra";
	g_down.writeGraph(out_down);
	g_up.writeGraph(out_up);
	*/
	vector<int> current_vertices;
	vector<Edge> current_edges;
	
	current_vertices.push_back(0);
	//current_vertices.push_back(5);
	//current_vertices.push_back(6);
	queryUpAll(current_vertices);
	
	// generate random edgelist
	getRandEdgeList(current_vertices, current_edges);
	
	vector<int>::iterator eit;
	for (eit = current_vertices.begin(); eit != current_vertices.end(); eit++) {
		cout << *eit << endl;
	}
	
}

int main(int argc, char* argv[]) {
	run(2);
	return 0;
}

