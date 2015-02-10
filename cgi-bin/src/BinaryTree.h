#ifndef _BINARYTREE_H
#define _BINARYTREE_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <set>
#include <queue>
#include <stack>
#include "Graph.h"
#include "GraphUtil.h"


using namespace std;


struct dp_table {
	dp_table():max(0), l(0), r(0){};
	double max;
	int l;
	int r;
};

struct treenode{
	treenode():parent(-1), left_child(-1), right_child(-1), community(-1), level(-1), in(0), tot(0), lrange(0), rrange(0), label_name(""), max_num(-1){};
	int parent;
	int left_child;
	int right_child;
	int community;
	int level;
	EdgeWeight in;
	EdgeWeight tot;
	int lrange;
	int rrange;
	string label_name;
	int max_num;
};

typedef vector<treenode> BinaryTreeStructure;	// tree index

class BinaryTree {
	private:
		BinaryTreeStructure tree_nlist;
		vector<vector<dp_table> > table;
		int tree_size;
		int base_size;
		vector<int> roots;
		int Root;
		vector<int> rangeToValue;
		int largest_table_content;
	public:
		BinaryTree();
		BinaryTree(int);
		BinaryTree(BinaryTreeStructure&);
		~BinaryTree();
		int num_nodes();
		int num_edges();
		treenode& operator[](const int);
		void setSize(int);
		int getSibling(int);
		void insert(int vid1, int vid2);
		void insert(Graph& g, int vid1, int vid2, EdgeWeight in, EdgeWeight tot);
		void add(treenode tn);		
		void addNode(Graph& g, int, int, map<int, EdgeWeight>&, vector<EdgeWeight>& in, vector<EdgeWeight>& tot);
		double addNode(Graph& g, int i, double max_threshold, EdgeWeight in, EdgeWeight tot);
		void deleteNode(int);
		double removeNode(Graph& g, int i);
		void freeNode(int);
		friend void Engraft(BinaryTree&, BinaryTree&);
		void writeTree(ostream&);
		void writeTree();
		void writeTree(int i);
		void clear();
		double modularitySelection(Graph& g, int, int, int&, map<int, EdgeWeight>&, vector<EdgeWeight>& in, vector<EdgeWeight>& tot);
		double modularitySelection_debug(Graph& g, int, int, int&, map<int, EdgeWeight>&, vector<EdgeWeight>& in, vector<EdgeWeight>& tot);
		double countModularityGain(Graph& g, Node i, int cur);
		pair<double, double> modularitySelection(Graph& g, int comm_v, int i, double max_threshold, int& selected);
		vector<int>& getRoots();
		void reorder();
		void mergeRoot(int v1, int v2, vector<EdgeWeight>& in, vector<EdgeWeight>& tot);
		void traverseToLevel(int root, int level);
		void traverseToLabel(int id, int& num);
		int getRoot();
		int setRoot(int r);
		bool checkNeighborComm(Graph& g, int i, int root);
		void readTree(string file_name);
		void getChildrenNode(int i, set<int>& children);
		double getDensity(Graph& g);
		void getValues(int cluster, set<int>& values);
		int countEdge(int cluster1, int cluster2, Graph& g);
		int countEdgeSlow(int cluster1, int cluster2, Graph& g);
		void constructDPTable(Graph& g);
		void writeDPTable();
		void setLargestTableCount(int count);
		int getFixInput(int v, int users_input_low, int users_input_high);
		void getVisualizationNodes(int root, int users_input_num, vector<int>& children);
		void labelCluster(int i, int cluster, Graph& g);
		void setClusterLableName(Graph& g);
};	

// get a specified vertex property
inline
treenode& BinaryTree::operator[](const int vid){
	return tree_nlist[vid];
}

inline
int BinaryTree::num_nodes(){
	return tree_nlist.size();
}

inline
int BinaryTree::num_edges(){
	BinaryTreeStructure::iterator git;
	int num = 0;
	for (git = tree_nlist.begin(); git != tree_nlist.end(); git++) {
		if (git->parent>= 0)
			num++;
		if (git->left_child>=0)
			num++;
		if (git->right_child>=0)
			num++;
	}
	return num/2;//each edge is calculated twice
}

inline
void BinaryTree::clear(){
	tree_size = 0;
	tree_nlist.clear();
	roots.clear();
}

inline
void BinaryTree::freeNode(int vid){
//cout<<"free node "<<vid<<endl;
	tree_nlist[vid].parent = -1;
	tree_nlist[vid].left_child = -1;
	tree_nlist[vid].right_child = -1;
	tree_nlist[vid].community = -1;	
}

inline
int BinaryTree::getRoot(){
	return Root;
}

inline
int BinaryTree::setRoot(int r){
	return Root = r;
}

inline
void BinaryTree::setLargestTableCount(int count){
	largest_table_content = count;
}

#endif

