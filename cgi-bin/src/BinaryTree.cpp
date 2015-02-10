#include "BinaryTree.h"

BinaryTree::BinaryTree(){
	tree_nlist = BinaryTreeStructure();
	rangeToValue = vector<int>();
	setLargestTableCount(100);
}

BinaryTree::BinaryTree(int size){ //O(n)
	tree_size = size;
	base_size = size;
	tree_nlist = BinaryTreeStructure(size);
	for(int i = 0; i < size; i++){
		tree_nlist[i].community = i;
	}
	rangeToValue = vector<int>(size);
	setLargestTableCount(100);
}

BinaryTree::BinaryTree(BinaryTreeStructure& tree_nlist){
	tree_nlist = tree_nlist;
	tree_size = tree_nlist.size();
	setLargestTableCount(100);
}

BinaryTree::~BinaryTree(){}

void BinaryTree::setSize(int size) {
	tree_size = size;
	base_size = size;
	tree_nlist.resize(size);
	for(int i = 0; i < size; i++){
		tree_nlist[i].community = i;
	}
	rangeToValue.resize(size);
}

int BinaryTree::getSibling(int vid){ //O(1)
	int pid = tree_nlist[vid].parent;
	if(vid == tree_nlist[pid].left_child)
		return tree_nlist[pid].right_child;
	else
		return tree_nlist[pid].left_child;
}

void BinaryTree::writeTree(ostream& out){
	cout << "Tree size = " << tree_nlist.size() << endl;
	out << "Tree Print out: " << endl;

	BinaryTreeStructure::iterator git;
	size_t i = 0;
	for (i = 0; i < tree_nlist.size(); i++){
		out << i << ": ";
		out << "parent: " <<tree_nlist[i].parent<<"; left_child: "<<tree_nlist[i].left_child<<"; right_child: "<<tree_nlist[i].right_child<<"; community: "<<tree_nlist[i].community<<"; level: "<<tree_nlist[i].level<<"; in: "<<tree_nlist[i].in<<"; tot: "<<tree_nlist[i].tot<<"; range: ["<<tree_nlist[i].lrange<<", "<<tree_nlist[i].rrange<<"]";
		out << " label_name: " << tree_nlist[i].label_name << " #" << endl;
	}
}

void BinaryTree::writeDPTable() {
	cout << "table content: " << endl;
	for (int i = 0; i < tree_nlist.size(); i++){
		cout << i << ": " << endl;
		for(int j = 0; j < (largest_table_content + 1); j++) {
			cout << "	table " << j << ": max-> " << table[i][j].max << "; l->" << table[i][j].l << "; r->" << table[i][j].r << " #" << endl;
		}
	}
}

void BinaryTree::writeTree(){
	cout << "Tree size = " << tree_nlist.size() << endl;
	cout << "Tree Print out: " << endl;

	BinaryTreeStructure::iterator git;
	size_t i = 0;
	for (i = 0; i < tree_nlist.size(); i++){
		cout << i << ": ";
		cout << "parent: " <<tree_nlist[i].parent<<"; left_child: "<<tree_nlist[i].left_child<<"; right_child: "<<tree_nlist[i].right_child<<"; community: "<<tree_nlist[i].community<<"; level: "<<tree_nlist[i].level<<"; in: "<<tree_nlist[i].in<<"; tot: "<<tree_nlist[i].tot<<"; range: ["<<tree_nlist[i].lrange<<", "<<tree_nlist[i].rrange<<"]";
		cout << " label_name: " << tree_nlist[i].label_name << " #" << endl;
	}
}

void BinaryTree::writeTree(int i){
	cout << i << ": ";
	cout << "parent: " <<tree_nlist[i].parent<<"; left_child: "<<tree_nlist[i].left_child<<"; right_child: "<<tree_nlist[i].right_child<<"; community: "<<tree_nlist[i].community<<"; level: "<<tree_nlist[i].level<<"; in: "<<tree_nlist[i].in<<"; tot: "<<tree_nlist[i].tot<<"; range: ["<<tree_nlist[i].lrange<<", "<<tree_nlist[i].rrange<<"] ";
}

//insert vid1 to vid2
void BinaryTree::insert(int vid1, int vid2){ 
//cout<<"insert "<<vid1<<" and "<<vid2<<endl;	
	tree_nlist.push_back(treenode());
	int nid = tree_nlist.size() - 1;
	tree_nlist[nid].left_child = vid1;
	tree_nlist[nid].right_child = vid2;
	if(tree_nlist[vid2].parent != -1){
		int pid = tree_nlist[vid2].parent;
		if(tree_nlist[pid].left_child == vid2)
			tree_nlist[pid].left_child = nid;
		else
			tree_nlist[pid].right_child = nid;
		tree_nlist[nid].parent = pid;		
	}
	tree_nlist[vid1].parent = nid;
	tree_nlist[vid2].parent = nid;
	tree_nlist[nid].community = tree_nlist[vid2].community;
	tree_nlist[vid1].community = tree_nlist[vid2].community;
	tree_nlist[nid].lrange = tree_nlist[vid2].lrange;
	tree_nlist[nid].rrange = tree_nlist[vid2].rrange;
	tree_nlist[nid].in = tree_nlist[vid2].in;
	tree_nlist[nid].tot = tree_nlist[vid2].tot;
}

//insert vid1 to vid2
void BinaryTree::insert(Graph& g, int vid1, int vid2, EdgeWeight in, EdgeWeight tot){ //O(degree * lgn)
	insert(vid1, vid2);
	tree_nlist[vid1].in = in;
	tree_nlist[vid1].tot = tot;
	int cur	= 0;
	for(EdgeList::iterator eit = g.getNeighbors(vid1).begin(); eit != g.getNeighbors(vid1).end(); eit++){
		int neighbor_label = tree_nlist[eit->first].lrange;
		cur = vid1;
		while(cur != -1){
			cur = tree_nlist[cur].parent;
			if((neighbor_label >= tree_nlist[cur].lrange) && (neighbor_label <= tree_nlist[cur].rrange)){
				tree_nlist[cur].in += (2* (eit->second));
			}		
		}
	}
	cur = vid1;
	while(cur != -1){
		cur = tree_nlist[cur].parent;
		tree_nlist[cur].in += tree_nlist[vid1].in;
		tree_nlist[cur].tot += tree_nlist[vid1].tot;
	}
}

void BinaryTree::add(treenode tn){ //O(1)
	tree_nlist.push_back(tn);
}

void BinaryTree::addNode(Graph& g, int i, int comm, map<int, EdgeWeight>& neighbor_comm_i, vector<EdgeWeight>& in, vector<EdgeWeight>& tot){		
	if(tree_nlist[comm].left_child != -1 && tree_nlist[comm].right_child != -1){
		int left_selected = comm;
		int right_selected = comm;
		double comm_left_value = 0, comm_right_value = 0;
		comm_left_value = modularitySelection(g, tree_nlist[comm].left_child, i, left_selected, neighbor_comm_i, in, tot);	
		comm_right_value = modularitySelection(g, tree_nlist[comm].right_child, i, right_selected, neighbor_comm_i, in, tot);
//cout << "add node " << i << " to " << comm << endl;
//cout << "left_selected is " << left_selected << ";right_selected is " << right_selected << endl;	
		if(comm_left_value > comm_right_value)
			insert(i, left_selected);
		else
			insert(i, right_selected);
	}
	else{
		insert(i, comm);
	}
}


/* double BinaryTree::modularitySelection_debug(Graph& g, int comm_v, int i, int& selected, map<int, EdgeWeight>& neighbor_comm_i, vector<EdgeWeight>& in, vector<EdgeWeight>& tot){ //O(lgn)
cout << "node " << i << " select with node " << comm_v <<" 's community which is "<< tree_nlist[comm_v].community << endl;
	if(checkNeighborComm(g, i, comm)){
		if(tree_nlist[comm_v].left_child == -1 && tree_nlist[comm_v].right_child == -1){
cout	<< "get down into if "<< comm_v << endl;		
			selected = comm_v;
		}
		else{
cout	<< "get down into else " << comm_v << endl;		
			int left = tree_nlist[comm_v].left_child;
cout	<< "left is " << left << endl;				
			int right = tree_nlist[comm_v].right_child;
cout	<< "right is " << right << endl;			
			double leftValue = modularitySelection_debug(g, tree_nlist[comm_v].left_child, i, left, neighbor_comm_i, in, tot);
cout	<< "leftValue is " << leftValue << endl;			
			double rightValue = modularitySelection_debug(g, tree_nlist[comm_v].right_child, i, right, neighbor_comm_i, in, tot);
cout	<< "rightValue is " << rightValue << endl;			
			if(leftValue > rightValue)
				selected = left;
			else
				selected = right;
		}
cout<<"community "<<tree_nlist[comm_v].community<<endl;
cout<<"neighbor_comm_i "<<neighbor_comm_i[tree_nlist[comm_v].community]<<endl;		
		return GraphUtil::modularityGain(g, i, tree_nlist[comm_v].community, neighbor_comm_i[tree_nlist[comm_v].community], in, tot);
	}
	else{
		selected = -1000;
		return -1000;
	}
} */



double BinaryTree::addNode(Graph& g, int i, double max_threshold, EdgeWeight in, EdgeWeight tot){ //O(modeselect + mlgn)
	int root = Root;
//cout<<"root is "<<root<<endl;	
	int left_selected = tree_nlist[root].left_child;
	int right_selected = tree_nlist[root].right_child;
	pair<double, double> left = modularitySelection(g, tree_nlist[root].left_child, i, max_threshold, left_selected);
	pair<double, double> right = modularitySelection(g, tree_nlist[root].right_child, i, max_threshold, right_selected);
	double leftTotal = left.first;
	double leftMax = left.second;
	double rightTotal = right.first;
	double rightMax = right.second;
	if(left_selected == -1000 && right_selected == -1000)
		cout<<"add node "<<i<<" wrong! No node to add!"<<endl;
	if(leftMax < max_threshold && rightMax < max_threshold){
		if(leftTotal > rightTotal){
			insert(g, i, left_selected, in, tot); //cout<<"add node "<<i<<" to "<<left_selected<<endl;
			return leftTotal;
		}
		else{
			insert(g, i, right_selected, in, tot); //cout<<"add node "<<i<<" to "<<right_selected<<endl;
			return rightTotal;
		}
	}
	else if(leftMax >= max_threshold && rightMax >= max_threshold){
		if(leftTotal > rightTotal){
			insert(g, i, left_selected, in, tot); //cout<<"add node "<<i<<" to "<<left_selected<<endl;
			return leftTotal;
		}
		else{
			insert(g, i, right_selected, in, tot); //cout<<"add node "<<i<<" to "<<right_selected<<endl;
			return rightTotal;
		}
	}
	else{
		if(leftMax > rightMax){
			insert(g, i, left_selected, in, tot); //cout<<"add node "<<i<<" to "<<left_selected<<endl;
			return leftTotal;
		}
		else{
			insert(g, i, right_selected, in, tot); //cout<<"add node "<<i<<" to "<<right_selected<<endl;
			return rightTotal;
		}
	}
}

void BinaryTree::deleteNode(int vid){
	if(tree_nlist[vid].parent != -1){
		int pid = tree_nlist[vid].parent;
		int sid = getSibling(vid);		
		if(tree_nlist[pid].parent != -1){
			int ppid = tree_nlist[pid].parent;
			if(tree_nlist[ppid].left_child == pid)
				tree_nlist[ppid].left_child = sid;
			else
				tree_nlist[ppid].right_child = sid;
			tree_nlist[sid].parent = ppid;
		}
		else
			tree_nlist[sid].parent = -1;
		freeNode(pid);
	}
	freeNode(vid);
}

double BinaryTree::removeNode(Graph& g, int i){
//cout<<"remove node "<<i<<endl;
	double loss = 0;
	int cur = i;
	double m = g.getTotalWeight() * 2;
	while(cur != -1){
		cur = tree_nlist[cur].parent;
		loss += (-(2 * tree_nlist[i].tot * tree_nlist[cur].tot - tree_nlist[i].tot * tree_nlist[i].tot) / (m * m));
	}
//cout<<"before consider edges, current loss "<<loss<<endl;	
	for(EdgeList::iterator eit = g.getNeighbors(i).begin(); eit != g.getNeighbors(i).end(); eit++){
		int neighbor_label = tree_nlist[eit->first].lrange;
		cur = i;
		bool begin = false;
		int label = tree_nlist[i].lrange;
		while(cur != -1){
			cur = tree_nlist[cur].parent;
			if((label >= tree_nlist[cur].lrange) && (label <= tree_nlist[cur].rrange) && (neighbor_label >= tree_nlist[cur].lrange) && (neighbor_label <= tree_nlist[cur].rrange)){
				begin = true;
			}
			if(begin){
				tree_nlist[cur].in -= (2* (eit->second));
//if(isnan(tree_nlist[cur].in)){cout<<cur<<" in is not a number in remove "<<endl;}				
			}
			loss += (tree_nlist[i].in + 2 * (eit->second)) / m ;
//cout<<"current loss "<<loss<<endl;		
		}
	}
	cur = i;
	while(cur != -1){
		cur = tree_nlist[cur].parent;
		tree_nlist[cur].in -= tree_nlist[i].in;
		tree_nlist[cur].tot -= tree_nlist[i].tot;
//if(isnan(tree_nlist[cur].in)){cout<<cur<<" in is not a number in remove "<<endl;}
//if(isnan(tree_nlist[cur].tot)){cout<<cur<<" tot is not a number in remove "<<endl;}	
	}	
	deleteNode(i);
	return loss;
}

double BinaryTree::modularitySelection(Graph& g, int comm_v, int i, int& selected, map<int, EdgeWeight>& neighbor_comm_i, vector<EdgeWeight>& in, vector<EdgeWeight>& tot){ //O(lgn)
//cout << "select " << i << " and " << comm_v << endl;	
	//if(checkNeighborComm(g, i, comm_v)){
		if(tree_nlist[comm_v].left_child == -1 && tree_nlist[comm_v].right_child == -1){		
			for(EdgeList::iterator eit = g.getNeighbors(i).begin(); eit != g.getNeighbors(i).end(); eit++){
				if(eit->first == comm_v){		
					selected = comm_v;
					goto end;
				}		
			}
			selected = -1000;
//cout << "current select is " << selected << " " << endl; 			
			return -1000;
		}
		else{		
			int left = tree_nlist[comm_v].left_child;				
			int right = tree_nlist[comm_v].right_child;			
			double leftValue = modularitySelection(g, tree_nlist[comm_v].left_child, i, left, neighbor_comm_i, in, tot);			
			double rightValue = modularitySelection(g, tree_nlist[comm_v].right_child, i, right, neighbor_comm_i, in, tot);
/* cout << "select " << i << " and " << comm_v << endl;
cout << " leftValue is " << leftValue << endl;
cout << " rightValue is " << rightValue << endl;
cout << " left is " << left << endl;
cout << " right is " << right << endl;	 */		
			if(leftValue > rightValue)
				selected = left;
			else
				selected = right;
		}
//cout << "current select is " << selected << " " << "gain is -1000 " << endl;		
		if(selected == -1000) return -1000;
		end:
//cout << "current select is " << selected << " " << "gain is " << GraphUtil::modularityGain(g, i, tree_nlist[comm_v].community, neighbor_comm_i[tree_nlist[comm_v].community], in, tot) << " " << endl;
			return GraphUtil::modularityGain(g, i, tree_nlist[comm_v].community, neighbor_comm_i[tree_nlist[comm_v].community], in, tot);
	/*}
	 else{
		selected = -1000;
		return -1000;
	} */
}

double BinaryTree::countModularityGain(Graph& g, Node i, int cur){ //O(degree)
	double old_mod = GraphUtil::modularity(g, tree_nlist[cur].in, tree_nlist[cur].tot);
	double tot = tree_nlist[cur].tot + tree_nlist[i].tot;
	double in = tree_nlist[cur].in + tree_nlist[i].in;
	for(EdgeList::iterator eit = g.getNeighbors(i).begin(); eit != g.getNeighbors(i).end(); eit++){
		int neighbor_label = tree_nlist[eit->first].lrange;
		if((neighbor_label >= tree_nlist[cur].lrange) && (neighbor_label <= tree_nlist[cur].rrange))
			in += (2 * (eit->second));
	}
	double mod = GraphUtil::modularity(g, in, tot);
//cout<<cur<<":old_mod is "<<old_mod<<"; mod is "<<mod<<endl;	
	return mod - old_mod;
}

pair<double, double> BinaryTree::modularitySelection(Graph& g, int comm_v, int i, double max_threshold, int& selected){
	int lrange = tree_nlist[comm_v].lrange;
	int rrange = tree_nlist[comm_v].rrange;
//cout<<"node "<<i<<" select with node "<<comm_v<<" in is "<<tree_nlist[comm_v].tot<<" tot is "<<tree_nlist[comm_v].tot<<endl;
	bool begin = false;	
	for(EdgeList::iterator eit = g.getNeighbors(i).begin(); eit != g.getNeighbors(i).end(); eit++){
		int neighbor_label = tree_nlist[eit->first].lrange;
//cout<<"neighbor_label "<<neighbor_label<<" lrange "<<lrange<<" rrange "<<rrange<<endl;
		if(((neighbor_label - lrange) >= 0) && ((neighbor_label - rrange) <= 0)){
			begin = true;		
			break;
		}		
	}
	if(begin){
		double maxValue = 0, totalValue = 0;
		if(tree_nlist[comm_v].left_child == -1 && tree_nlist[comm_v].right_child == -1){
			selected = comm_v;
		}
		else{
			int left_selected = comm_v;
			int right_selected = comm_v;
			pair<double, double> left = modularitySelection(g, tree_nlist[comm_v].left_child, i, max_threshold, left_selected);
			pair<double, double> right = modularitySelection(g, tree_nlist[comm_v].right_child, i, max_threshold, right_selected);
			double leftTotal = left.first;
			double leftMax = left.second;
			double rightTotal = right.first;
			double rightMax = right.second;
			if(leftMax < max_threshold && rightMax < max_threshold){
				if(leftTotal > rightTotal){
					selected = left_selected;
					maxValue = leftMax;
					totalValue = leftTotal;
				}
				else{
					selected = right_selected;
					maxValue = rightMax;
					totalValue = rightTotal;
				}
			}
			else if(leftMax >= max_threshold && rightMax >= max_threshold){
				if(leftTotal > rightTotal){
					selected = left_selected;
					maxValue = leftMax;
					totalValue = leftTotal;
				}
				else{
					selected = right_selected;
					maxValue = rightMax;
					totalValue = rightTotal;
				}
			}
			else{
				if(leftMax > rightMax){
					selected = left_selected;
					maxValue = leftMax;
					totalValue = leftTotal;
				}
				else{
					selected = right_selected;
					maxValue = rightMax;
					totalValue = rightTotal;
				}
			}
		}
		double cur_gain = countModularityGain(g, i, comm_v);
		if(cur_gain > maxValue)
			maxValue = cur_gain;
//cout<<"return is "<<totalValue + cur_gain<<" and "<<maxValue<<endl;	
//cout<<"			node "<<i<<" select with node "<<comm_v<<" lrange "<<lrange<<" rrange "<<rrange<<endl;		
		return pair<double, double>(totalValue + cur_gain, maxValue);
	}
	else{
		selected = -1000;
//cout<<"return is "<<-1000<<" and "<<-1000<<endl;		
		return pair<double, double>(-1000, -1000);
	}
}

vector<int>& BinaryTree::getRoots(){ //O(n)
	int root_num = 0;
	for(vector<treenode>::iterator it = tree_nlist.begin(); it != tree_nlist.end(); it++){
		if((*it).parent == -1 && (*it).community != -1){
			root_num++;
		}
	}

	roots.clear();
	roots.resize(root_num);
//cout<<"root size:"<<root_num<<endl;	
	int i = 0;
//cout<<"roots are:";	
	for(vector<treenode>::iterator it = tree_nlist.begin(); it != tree_nlist.end(); it++, i++){
//cout<<i<<": parent: "<<	(*it).parent <<"; community: "<< (*it).community<<endl;
		if((*it).parent == -1 && (*it).community != -1){
//cout<<i<<" ";		
		roots[(*it).community] = i;
		}
	}
//cout<<endl;	
	return roots;
}


//add t2 to t1
void Engraft(BinaryTree& t1, BinaryTree& t2){ //O(n)
	vector<int> leaf_root = t1.getRoots();
	int t1_size = (int)t1.num_nodes();
	int root_size = (int)leaf_root.size();
	
	//debug
	/* int count = 0;
	for(int i = 0; i < t2.num_nodes(); i++){
		if(t2[i].left_child == -1 && t2[i].right_child == -1 && t2[i].community != -1)
			count++;
	}
	if(count != root_size){
		cout<<"in engraft: count != root size"<<endl;
		cout<<"count="<<count<<"; while root_size = "<<root_size<<endl;
	} */
	//end debug
	
	vector<int> ctoc;
	for(int i = 0; i < root_size; i++){
		ctoc.push_back(t2[i].community);
	}
	for(int i = 0; i < t1.num_nodes(); i++){
		if(t1[i].community == -1)
			continue;
		t1[i].community = ctoc[t1[i].community];
	}
/* cout<<"root_size:"<<root_size<<" t2 size:"<<t2.num_nodes()<<endl;
	for(int i = 0; i < root_size; i++){
		cout<<i<<" -> "<<leaf_root[i]<<endl;
	} */
	for(int j = root_size; j < t2.num_nodes(); j++){
		t1.add(t2[j]);		
	}
//cout<<"t1 size:"<<t1.num_nodes()<<endl;
	for(int j = t1_size; j < t1.num_nodes(); j++){
		if(t1[j].community == -1)
			continue;
//cout<<"parent"<<endl;			
		if(t1[j].parent != -1){
			t1[j].parent = t1[j].parent - root_size + t1_size;
		}
//cout<<"left child:"<<t1[j].left_child<<endl;		
		if(t1[j].left_child < root_size){
			t1[leaf_root[t1[j].left_child]].parent = j;
			t1[j].left_child = leaf_root[t1[j].left_child];
		}
		else
			t1[j].left_child = t1[j].left_child - root_size + t1_size;
//cout<<"right child"<<endl;			
		if(t1[j].right_child < root_size){
			t1[leaf_root[t1[j].right_child]].parent = j;
			t1[j].right_child = leaf_root[t1[j].right_child];
		}
		else
			t1[j].right_child = t1[j].right_child - root_size + t1_size;
	}
}

void BinaryTree::reorder(){ //O(nlgn)	
	int del_num = 0;
	map<int, int> reord;
	for(int i = 0; i < (int)tree_nlist.size(); i++){
		if(tree_nlist[i].community == -1 && tree_nlist[i].parent == -1 && tree_nlist[i].left_child == -1 && tree_nlist[i].right_child == -1){
			del_num++;
		}
		else reord.insert(make_pair(i, i - del_num));
	}
	vector<treenode> list;
	for(int i = 0; i < (int)tree_nlist.size(); i++){
		if(tree_nlist[i].community != -1 || tree_nlist[i].parent != -1 || tree_nlist[i].left_child != -1 || tree_nlist[i].right_child != -1){
			if(tree_nlist[i].parent != -1)
				tree_nlist[i].parent = reord[tree_nlist[i].parent];
			if(tree_nlist[i].left_child != -1)
				tree_nlist[i].left_child = reord[tree_nlist[i].left_child];
			if(tree_nlist[i].right_child != -1)
				tree_nlist[i].right_child = reord[tree_nlist[i].right_child];
			list.push_back(tree_nlist[i]);
		}
	}
	tree_nlist = list;
	for(int i = 0; i < (int)tree_nlist.size(); i++){
		if(tree_nlist[i].community == -1 && tree_nlist[i].parent == -1 && tree_nlist[i].left_child != -1 && tree_nlist[i].right_child != -1){
			Root = i;
//cout<<"set root to "<<i<<endl;			
			break;
		}
	}
	base_size = (int)tree_nlist.size();	
}

void BinaryTree::mergeRoot(int v1, int v2, vector<EdgeWeight>& in, vector<EdgeWeight>& tot){
	if(v1 == v2){
		cout<<"error: v1 == v2 when merge root"<<endl;
		return ;
	}
//cout<<"merge "<<v1<<" and "<<v2<<endl;	
	tree_nlist.push_back(treenode());
	int nid = tree_nlist.size() - 1;
	tree_nlist[nid].left_child = v1;
	tree_nlist[nid].right_child = v2;
	tree_nlist[v1].parent = nid;
	tree_nlist[v2].parent = nid;
	tree_nlist[nid].in = in[nid];
	tree_nlist[nid].tot = tot[nid];
	/* for(int i = 0; i < (int)tree_nlist.size(); i++){
		if(tree_nlist[i].community == comm1 || tree_nlist[i].community == comm2)
			tree_nlist[i].community = comm;
	} */
/* 	traverse(v1, comm);
	traverse(v2, comm); */
//cout<<"finish merge root"<<endl;	
}

void BinaryTree::traverseToLevel(int root, int level){
	if(root != -1){
		int left = tree_nlist[root].left_child;
		int right = tree_nlist[root].right_child;
		tree_nlist[root].level = level;
		traverseToLevel(left, level + 1);
		traverseToLevel(right, level + 1);
	}
}

void BinaryTree::traverseToLabel(int id, int& num){
	int left = tree_nlist[id].left_child;
	int right = tree_nlist[id].right_child;
	if(left == -1 && right == -1 && tree_nlist[id].parent != -1 && tree_nlist[id].community != -1){	
		tree_nlist[id].lrange = num;		
		tree_nlist[id].rrange = num;
		rangeToValue[num++] = id;
	}
	else{
		traverseToLabel(left, num);
		traverseToLabel(right, num);
		tree_nlist[id].lrange = tree_nlist[left].lrange;		
		tree_nlist[id].rrange = tree_nlist[right].rrange;			
	}
}

bool BinaryTree::checkNeighborComm(Graph& g, int i, int root){
	int left = tree_nlist[root].left_child;
	int right = tree_nlist[root].right_child;
	if(left != -1 && right != -1){
		return (checkNeighborComm(g, i, left) || checkNeighborComm(g, i, right));
	}
	else{
		for(EdgeList::iterator eit = g.getNeighbors(i).begin(); eit != g.getNeighbors(i).end(); eit++){
			if(eit->first == root) return true;
		}
		return false;
	}
}

void BinaryTree::getChildrenNode(int i, set<int>& children) { 
	int left = tree_nlist[i].left_child;
	int right = tree_nlist[i].right_child;
	if(left == -1 && right == -1) {
		children.insert(i);
	}
	else {
		getChildrenNode(left, children);
		getChildrenNode(right, children);
	}
}

void BinaryTree::labelCluster(int i, int cluster, Graph& g) { 
	int left = tree_nlist[i].left_child;
	int right = tree_nlist[i].right_child;
	if(left == -1 && right == -1) {
		g.setCluster(i, cluster);
	}
	else {
		labelCluster(left, cluster, g);
		labelCluster(right, cluster, g);
	}
}

void BinaryTree::readTree(string file_name) {
	ifstream in(file_name.c_str());
	string buf = "";
	int id, size = 0;
	EdgeWeight ew;
	string name;
	if(tree_nlist.size() > 0) {
		tree_nlist.clear();
	}
	getline(in, buf);
	buf = "";
	while(getline(in, buf)) {
		istringstream(buf.substr(0, buf.find(":"))) >> id;
		treenode tn;		
		if(tree_nlist.size() != id)
			cout << "read error!" << endl;
		buf.erase(0, buf.find(":")+1);
		buf.erase(0, buf.find(":")+1);
		istringstream(buf.substr(0, buf.find(";"))) >> id;
		tn.parent = id;
		buf.erase(0, buf.find(":")+1);
		istringstream(buf.substr(0, buf.find(";"))) >> id;
		tn.left_child = id;
		buf.erase(0, buf.find(":")+1);
		istringstream(buf.substr(0, buf.find(";"))) >> id;
		tn.right_child = id;
		buf.erase(0, buf.find(":")+1);
		istringstream(buf.substr(0, buf.find(";"))) >> id;
		tn.community = id;
		buf.erase(0, buf.find(":")+1);
		istringstream(buf.substr(0, buf.find(";"))) >> id;
		tn.level = id;
		buf.erase(0, buf.find(":")+1);
		istringstream(buf.substr(0, buf.find(";"))) >> ew;
		tn.in = ew;
		buf.erase(0, buf.find(":")+1);
		istringstream(buf.substr(0, buf.find(";"))) >> ew;
		tn.tot = ew;
		buf.erase(0, buf.find("[")+1);
		istringstream(buf.substr(0, buf.find(","))) >> id;
		tn.lrange = id;
		buf.erase(0, buf.find(",")+1);
		istringstream(buf.substr(0, buf.find("]"))) >> id;
		tn.rrange = id;
		buf.erase(0, buf.find(":")+1);
		name = buf.substr(0, buf.find("#") - 1);
		tn.label_name = name;
		tree_nlist.push_back(tn);
		buf.clear();
		if(tn.left_child == -1 && tn.right_child == -1) {
			size++;
		}		
	}
	rangeToValue.resize(size);
	for(int i = 0; i < size; i++) {
		rangeToValue[tree_nlist[i].lrange] = i;
	}
}

double BinaryTree::getDensity(Graph& g) {
	size_t denum = 0, enume = 0;
	for(size_t i = 0; i < tree_nlist.size(); i++) {
		if(tree_nlist[i].parent == -1 && tree_nlist[i].left_child != -1 && tree_nlist[i].right_child != -1) {
			set<int> nodes;
			getChildrenNode(i, nodes);
			size_t cur_denum = nodes.size();	
			denum += cur_denum * (cur_denum - 1);
			for(set<int>::iterator it = nodes.begin(); it != nodes.end(); it++) {
				//cout << "current node" << *it << " " <<endl;
				for(EdgeList::iterator eit = g.getNeighbors(*it).begin(); eit != g.getNeighbors(*it).end(); eit++) {
					//cout << "current edge " << eit->first << ";";
					if(eit->first == *it) continue;
					if(nodes.find(eit->first) != nodes.end()) {
						//cout << " hit!; ";
						enume++;
					}
				}
				//cout << " " << endl;
			}
			//cout << "cur denum is " << cur_denum << endl;
		}
	}
	//cout << "denum is " << denum << "; enum is " << enume << " " << endl;
	if(denum == 1 || denum == 0)
		return 0;
	return ((double)enume / (double)denum);
}

void BinaryTree::getValues(int cluster, set<int>& values) {
	int lrange = tree_nlist[cluster].lrange;
	int rrange = tree_nlist[cluster].rrange;
	for(int i = lrange;i <= rrange;i++){
		values.insert(rangeToValue[i]);
	}
}

int BinaryTree::countEdge(int cluster1, int cluster2, Graph& g) {
	int numEdge = 0;
	if(cluster1 > (int)tree_nlist.size() - 1)
		cout<<"the first id is illegal"<<endl;
	else if(cluster2 > (int)tree_nlist.size() - 1)
		cout<<"the second id is illegal"<<endl;
	else{
		int lrange1 = tree_nlist[cluster1].lrange;
		int rrange1 = tree_nlist[cluster1].rrange;
		int lrange2 = tree_nlist[cluster2].lrange;
		int rrange2 = tree_nlist[cluster2].rrange;
		if((rrange1 - lrange1) > (rrange2 - lrange2)) {
			int tmp = cluster1;
			cluster1 = cluster2;
			cluster2 = tmp;
			lrange1 = tree_nlist[cluster1].lrange;
			rrange1 = tree_nlist[cluster1].rrange;
			lrange2 = tree_nlist[cluster2].lrange;
			rrange2 = tree_nlist[cluster2].rrange;
		}
		
		set<int> values1; 
		getValues(cluster1, values1);
		for(set<int>::iterator it = values1.begin(); it != values1.end(); it++) {
			for(EdgeList::iterator eit = g.getNeighbors(*it).begin(); eit != g.getNeighbors(*it).end(); eit++) {				
				if((tree_nlist[eit->first].lrange >= lrange2) && (tree_nlist[eit->first].rrange <= rrange2)) {
//					values2.insert(*nit);
					++numEdge;
				}
			}
		}
		
		/*
		for(set<int>::iterator it1 = values1.begin();it1 != values1.end();it1++){
			cout<<*it1<<" ";
		}
		cout<<endl;
		for(set<int>::iterator it1 = values2.begin();it1 != values2.end();it1++){
			cout<<*it1<<" ";
		}
		cout<<endl;
		for(set<int>::iterator it1 = common.begin();it1 != common.end();it1++){
			cout<<*it1<<" ";
		}
		cout<<endl;
		*/
	}
	return numEdge;
}

int BinaryTree::countEdgeSlow(int cluster1, int cluster2, Graph& g) {
	int numEdge = 0;
	if(cluster1 > (int)tree_nlist.size() - 1)
		cout<<"the first id is illegal"<<endl;
	else if(cluster2 > (int)tree_nlist.size() - 1)
		cout<<"the second id is illegal"<<endl;
	else{
		set<int> values1, values2;
		getChildrenNode(cluster1, values1);
		getChildrenNode(cluster2, values2);	
		for(set<int>::iterator it1 = values1.begin();it1 != values1.end();it1++) {
			for(set<int>::iterator it2 = values2.begin();it2 != values2.end();it2++) {
				for(EdgeList::iterator eit = g.getNeighbors(*it2).begin(); eit != g.getNeighbors(*it2).end(); eit++) {
					if(*it1 == eit->first) {
						numEdge++;
					}
				}
			}
		}
	}
	return numEdge;
}

void BinaryTree::constructDPTable(Graph& g) {
	if(largest_table_content > g.getNumVertices()) {
		largest_table_content = g.getNumVertices();
	}
	//topological sort
	cout<<"topological sort "<<endl;
	stack<int> tree_stack;
	queue<int> tree_queue;
	for (int i = 0; i < tree_nlist.size(); i++){
		if(tree_nlist[i].parent == -1) {
			Root = i;
			break;
		}
	}
	tree_queue.push(Root);
	while(!tree_queue.empty()) {
		int root = tree_queue.front();
		tree_queue.pop();
		tree_stack.push(root);
		int left = tree_nlist[root].left_child;
		int right = tree_nlist[root].right_child; 
		if(left != -1 && right != -1) {	
			tree_queue.push(left);
			tree_queue.push(right);
		}
	}
	if(tree_stack.size() != tree_nlist.size()) {
		cout << "error: tree stack size not equal to tree size! " << endl;
		exit(0);
	}
	
	cout<<"initialize table "<<endl;
	//initialize table
	table.resize(tree_nlist.size());
	for(int i = 0; i < (int)tree_nlist.size(); i++) {
		table[i].resize(largest_table_content + 1);
	}
	
	 cout<<"count DP table "<<endl;
	//count DP table
	while(!tree_stack.empty()) {
		int cur = tree_stack.top();
		tree_stack.pop();
		int left = tree_nlist[cur].left_child; 
		int right = tree_nlist[cur].right_child; 
		int childnum = tree_nlist[cur].rrange - tree_nlist[cur].lrange + 1;
		int childnum_left = 0, childnum_right = 0;
		if(left != -1)
			childnum_left = tree_nlist[left].rrange - tree_nlist[left].lrange + 1;
		if(right != -1)
			childnum_right = tree_nlist[right].rrange - tree_nlist[right].lrange + 1;
		double cur_node_mod = GraphUtil::modularity(g, tree_nlist[cur].in, tree_nlist[cur].tot);
		for(int i = 1; i < largest_table_content + 1; i++) {
			double cur_max = 0;
			int cur_l = 0;
			int cur_r = 0;
			//set leaf node
			if(left == -1 && right == -1) {
				table[cur][i].max = cur_node_mod;
				table[cur][i].l = 0;
				table[cur][i].r = 0;
				continue;
			}
			if(i == 1)
				cur_max = cur_node_mod;
			else if (i == childnum) {
				cur_max = cur_node_mod;
				cur_l = childnum_left;
				cur_r = childnum_right;
				goto end;
			}
			else if (i > childnum) {
				cur_max = table[cur][childnum].max;
				cur_l = childnum_left;
				cur_r = childnum_right;
				goto end;
			}
			
			
			//choose j nodes in left, i-j nodes in right
			for(int j = 0; j < i + 1; j++) {
				if(i > 1 && j == 0) continue;
				if(j > childnum_left || (i - j) > childnum_right) continue;
				double max = table[left][j].max + table[right][i - j].max;
				if(max > cur_max) {
					cur_max = max;
					cur_l = j;
					cur_r = i - j;
				}
			}
			end:
			table[cur][i].max = cur_max;
			table[cur][i].l = cur_l;
			table[cur][i].r = cur_r;			
		} 
	}
}

int BinaryTree::getFixInput(int v, int users_input_low, int users_input_high) {
	
	int childnum = tree_nlist[v].rrange - tree_nlist[v].lrange + 1;
	
	if(childnum >= users_input_low && childnum <= users_input_high)
		return childnum;
		
	if(users_input_low == users_input_high)
		return users_input_low;
	
	if(users_input_high > largest_table_content) {
		users_input_high = largest_table_content;
	}
	
	int users_input_num = users_input_low;
	double max_mod = table[v][users_input_low].max;
	
	for (int i = users_input_low; i < users_input_high + 1; i++) {
		double cur_max = table[v][i].max;
		if(max_mod < cur_max) {
			users_input_num = i;
			max_mod = cur_max;
		}
	}
	
	if(users_input_num == 1)
		users_input_num = users_input_high;
	
	return users_input_num;
}

void BinaryTree::getVisualizationNodes(int root, int users_input_num, vector<int>& nodes) {	
		
	if(users_input_num > largest_table_content) {
		users_input_num = largest_table_content;
	}
	
	int left = tree_nlist[root].left_child;
	int right = tree_nlist[root].right_child;
	int l_input_num = table[root][users_input_num].l;
	int r_input_num = table[root][users_input_num].r;
	if(r_input_num == 0 && l_input_num == 0) {
		nodes.push_back(root);
	}
	else {
		if(l_input_num > 0)
			getVisualizationNodes(left, l_input_num, nodes);
		if(r_input_num > 0)
			getVisualizationNodes(right, r_input_num, nodes);
	}
}

void BinaryTree::setClusterLableName(Graph& g) {
	for(int i = 0; i < g.getNumVertices(); i++) {
		tree_nlist[i].label_name = g.getLabelName(i);
		tree_nlist[i].max_num = i;
	}
	
	//topological sort
	cout<<"topological sort "<<endl;
	stack<int> tree_stack;
	queue<int> tree_queue;
	for (int i = 0; i < tree_nlist.size(); i++){
		if(tree_nlist[i].parent == -1) {
			Root = i;
			break;
		}
	}
	tree_queue.push(Root);
	while(!tree_queue.empty()) {
		int root = tree_queue.front();
		tree_queue.pop();
		tree_stack.push(root);
		int left = tree_nlist[root].left_child;
		int right = tree_nlist[root].right_child;
		if(left != -1 && right != -1) {	
			tree_queue.push(left);
			tree_queue.push(right);
		}
	}
	if(tree_stack.size() != tree_nlist.size()) {
		cout << "error: tree stack size not equal to tree size! " << endl;
		exit(0);
	}
	
	while(!tree_stack.empty()) {
		int cur = tree_stack.top();
		tree_stack.pop();
		int left = tree_nlist[cur].left_child;
		int right = tree_nlist[cur].right_child;
		if(left == -1 && right == -1)
			continue;
		EdgeWeight max_left = tree_nlist[tree_nlist[left].max_num].tot;
		EdgeWeight max_right = tree_nlist[tree_nlist[right].max_num].tot;
		if(max_left > max_right) {
			tree_nlist[cur].max_num = tree_nlist[left].max_num;
			tree_nlist[cur].label_name = tree_nlist[left].label_name;
		}
		else {
			tree_nlist[cur].max_num = tree_nlist[right].max_num;
			tree_nlist[cur].label_name = tree_nlist[right].label_name;
		}
	}
	
}

