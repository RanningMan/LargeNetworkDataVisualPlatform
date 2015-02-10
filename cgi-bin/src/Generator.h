#ifndef _GENERATOR_H
#define _GENERATOR_H

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <stdlib.h> 
#include <algorithm>
#include <cmath>
#include <ctime>
#include "Graph.h"
#include "Utils.h"
#include "BinaryTree.h"
#include "GraphUtil.h"
#include "hash_map.h"
#include <stdio.h>
#include <queue>
#include <stack>

#define MIN 0
#define PER 0.2

using namespace std;

struct Edge {
	int v1, v2;
	double weight;
	Edge(int V1, int V2, double W) : v1(V1), v2(V2), weight(W) {}
};

class Generator
{
	public:
		Generator();
		~Generator();
		void init();
		void run(DBJob& job, string& new_vertices_str, string& new_edges_str, int& vertices_num, int& edges_num, double& mod, bool& flag);
	
	private:
		Graph original_g;
		BinaryTree tree;
		stack<int> pre_query_v;
		void generateFullTree(Graph& g_down, Graph& g_up, const int& iter);
		bool queryDown(vector<int>& current_vertices, int v);
		bool queryUp(vector<int>& current_vertices, int v);
		void queryDownAll(vector<int>& current_vertices);
		void queryUpAll(vector<int>& current_vertices);
		void makeUniqueVertices(vector<int>& current_vertices);
		void parseVertexList(const string& current_v_list, vector<int>& current_vertices);
		void buildVertexStr(const vector<int>& current_vertices, string& new_vertices_str);
		void buildEdgeStr(const vector<Edge>& new_edges, string& new_edges_str);
		void getRandEdgeList(vector<int>& current_vertices, vector<Edge>& current_edges);
		bool isEdge();
		
		double constructVisGraph(Graph& g, BinaryTree& tree, vector<Edge>& edges, vector<int>& nodes);
		void ClusterMerge(Graph& cur_g, Graph& original_g, BinaryTree& original_t);
		int LouvainAlgorithm(Graph& g, Graph& original_g, BinaryTree& original_t, vector<vector<int> >& community, float EPSILON, int PASS_NUM, vector<EdgeWeight>& in, vector<EdgeWeight>& tot);
		int constructNewGraph(Graph& original_g, BinaryTree& original_t, Graph& g, vector<EdgeWeight>& in, vector<EdgeWeight>& tot);
		double memberAdjustUp(Graph& original_g, BinaryTree& original_t, vector<EdgeWeight>& original_in, vector<EdgeWeight>& original_tot, float EPSILON, int PASS_NUM);
		double detectCommunity(Graph& g, BinaryTree& t, vector<EdgeWeight>& in, vector<EdgeWeight>& tot, float EPSILON, int PASS_NUM);
		double getModThres(Graph& g, vector<EdgeWeight>& in, vector<EdgeWeight>& tot, vector<bool>& active, double p);
		double sortThres(vector<double>& a, double p);
		void partition(vector<double>& a, int& n, double& pivot, bool smaller);
		void add(Graph& g, Node i, int comm, vector<EdgeWeight>& in, vector<EdgeWeight>& tot);
		void remove(Graph& g, Node i, vector<EdgeWeight>& in, vector<EdgeWeight>& tot);
		EdgeWeight getEdgeWeightBetweenCommunity(Graph& g, Node i, int comm);
		EdgeWeight getEdgeWeightBetweenVertices(Graph& g, Node i, Node j);
		bool query(vector<int>& current_vertices, int v, int inputlow, int inputhigh);
		void buildLabelName(const vector<int>& current_vertices, string& label_name);
};

#endif

