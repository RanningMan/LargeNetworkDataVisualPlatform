#ifndef _GRAPH_H
#define _GRAPH_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <utility>
#include <cmath>
#include <algorithm>
#include <set>
#include <map>

#define MAX_BASE 1000000000

using namespace std;

typedef double EdgeWeight;
typedef int Node;

struct Vertex {
	Vertex(): community(-1), weighted_degree(0), cluster(-1), label_name(""){}
	int community;
	EdgeWeight weighted_degree;
	int cluster;
	string label_name;
};

struct Compare {
	bool operator () (pair<Node, EdgeWeight> p1, pair<Node, EdgeWeight> p2) {
		return (p1.first < p2.first);
	}
} ;

typedef vector<Vertex> VertexList;
typedef vector<pair<Node, EdgeWeight> > EdgeList;
typedef vector<EdgeList> GRA;

class Graph{
	private:
		GRA graph;
		VertexList vl;
		int vsize;
		int esize;
		EdgeWeight total_weight;
	public:
		Graph();
		Graph(int num, EdgeWeight total_weight);
		Graph(istream& in, istream& label, bool preprocess);
		~Graph();
		void readGraph(istream&, istream& label, bool preprocess);
		void readGraph(istream& in, bool preprocess);
		void writeGraph();
		void addVertex(Vertex);
		void addEdge(Node, Node, const EdgeWeight&);
		const int& getNumVertices() const;
		const int& getNumEdges() const;
		const EdgeWeight& getTotalWeight() const;
		VertexList& getVertices();
		int getDegree(Node src);
		EdgeList& getNeighbors(Node src);
		void setCommunity(Node src, int community);
		int getCommunity(Node src);
		void setCluster(Node src, int cluster);
		int getCluster(Node src);
		EdgeWeight getWeightedDegree(Node src);
		void setWeightedDegree(Node src, EdgeWeight weight);
		void resize(int size);
		Graph& operator=(const Graph&);
		bool compare(pair<Node, EdgeWeight> p1, pair<Node, EdgeWeight> p2);
		string getLabelName(int i);
};

inline
string Graph::getLabelName(int i) {
	return vl[i].label_name;
}

inline
void Graph::addVertex(Vertex v){
	vl.push_back(v);
	graph.push_back(EdgeList());
	vsize++;
}

inline
void Graph::addEdge(Node sid, Node tid, const EdgeWeight& weight) {
	graph[sid].push_back(make_pair(tid, weight));
	esize++;
}

inline
const int& Graph::getNumVertices() const{
	return vsize;
}

inline
const int& Graph::getNumEdges() const{
	return esize;
}

inline
const EdgeWeight& Graph::getTotalWeight() const{
	return total_weight;
}

inline
VertexList& Graph::getVertices(){
	return vl;
}

inline
int Graph::getDegree(Node src) {
	return graph[src].size();
}

inline
EdgeList& Graph::getNeighbors(Node src){
	return graph[src];
}

inline
void Graph::setCommunity(Node src, int comm){
	vl[src].community = comm;
}

inline
int Graph::getCommunity(Node src){
	return vl[src].community;
}

inline
void Graph::setCluster(Node src, int cluster){
	vl[src].cluster = cluster;
}

inline
int Graph::getCluster(Node src){
	return vl[src].cluster;
}

inline
EdgeWeight Graph::getWeightedDegree(Node src){
	return vl[src].weighted_degree;
}

inline
void Graph::setWeightedDegree(Node src, EdgeWeight weight){
	vl[src].weighted_degree = weight;
}


#endif
