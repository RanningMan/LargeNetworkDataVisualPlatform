
#include "Generator.h"

Generator::Generator() {
}

Generator::~Generator() {
}

EdgeWeight Generator::getEdgeWeightBetweenVertices(Graph& g, Node i, Node j){
	for(EdgeList::iterator eit = g.getNeighbors(i).begin(); eit != g.getNeighbors(i).end(); eit++){
		if((*eit).first == j)
			return (*eit).second;
	}
	return 0;
}

EdgeWeight Generator::getEdgeWeightBetweenCommunity(Graph& g, Node i, int comm){
	EdgeWeight ew = 0;
	for(EdgeList::iterator eit = g.getNeighbors(i).begin(); eit != g.getNeighbors(i).end(); eit++){
		if(i == (*eit).first)continue;
		if(g.getCommunity((*eit).first) == comm)
			ew += (*eit).second;
	}
	return ew;
}

void Generator::remove(Graph& g, Node i, vector<EdgeWeight>& in, vector<EdgeWeight>& tot){ 
	int comm = g.getCommunity(i);
	in[comm] -= (getEdgeWeightBetweenVertices(g,i,i) + 2 * getEdgeWeightBetweenCommunity(g,i,comm));
	tot[comm] -= g.getWeightedDegree(i);
	g.setCommunity(i, -1);
}

void Generator::add(Graph& g, Node i, int comm, vector<EdgeWeight>& in, vector<EdgeWeight>& tot){ 
	in[comm] += (getEdgeWeightBetweenVertices(g,i,i) + 2 * getEdgeWeightBetweenCommunity(g,i,comm));
	tot[comm] += g.getWeightedDegree(i);
	g.setCommunity(i, comm);
}

void Generator::partition(vector<double>& a, int& n, double& pivot, bool smaller){
	int pos = 0;
	for (int i = 0; i < n; i++)
		if (smaller && a[i] < pivot || !smaller && a[i] > pivot){
			double tmp = a[i];
			a[i] = a[pos];
			a[pos++] = tmp;
		}
	n = pos;
	pivot = a[n - 1];
}

double Generator::sortThres(vector<double>& a, double p){
	int len = (int) a.size();
	int pos = (int) (len * p);
	double pivot = a[len - 1];
	while(true){
		int smaller = 0;
		int larger = 0;
		for(int i = 0; i < len; i++){
			if(a[i] < pivot)
				smaller++;
			if(a[i] > pivot)
				larger++;
		}		
		if(smaller > pos){
			partition(a, len, pivot, true);
		}
		else if(pos < len - larger){
			return pivot;
		}
		else{
			pos -= (len - larger);
			partition(a, len, pivot, false);
		}
	}
}

double Generator::getModThres(Graph& g, vector<EdgeWeight>& in, vector<EdgeWeight>& tot, vector<bool>& active, double p){
	vector<double> mods_gain;
	vector<bool> visited(g.getNumVertices(), false);
	for(Node i = 0; i < g.getNumVertices(); i++){
		if(!active[i])continue;
		for(EdgeList::iterator eit = g.getNeighbors(i).begin(); eit != g.getNeighbors(i).end(); eit++){
			if(!(active[eit -> first]))continue;
			if(visited[eit -> first])continue;
			if(eit -> first == i)continue;
			int comm = eit->first;
			double w = eit->second;
			mods_gain.push_back(abs(GraphUtil::modularityGain(g, i, comm, w, in, tot)));
		}
		visited[i] = true;
	}
	double thres = sortThres(mods_gain, p);
	return thres;
}

//the first phase, stop when some condition reached
double Generator::detectCommunity(Graph& g, BinaryTree& t, vector<EdgeWeight>& in, vector<EdgeWeight>& tot, float EPSILON, int PASS_NUM){ //O(n + m + nlgn)
	int pass = 0;
	double gain = 0;
	double maxgain = MIN;
	double mod = 1;
	double pre_mod = 0;
	#if DEBUG == true
	double pre_density = 0;
	double density = 1;
	#endif
	int pre_comm = 0;
	bool changed = false;	
	map<int, int> roots;
	for(int i = 0; i < t.num_nodes(); i++){
		if(t[i].parent == -1 && t[i].community != -1)
			roots.insert(make_pair(t[i].community, i));
	}
	do {
		changed=false;
		pre_mod = GraphUtil::modularity(g, in, tot); 
		#if DEBUG == true
		pre_density = t.getDensity(g);
		#endif		
		pass++;
		for(Node i = 0; i < g.getNumVertices(); i++){
			map<int, EdgeWeight> neighborComm;
			pre_comm = g.getCommunity(i);			
			neighborComm.insert(make_pair(pre_comm, 0));
			remove(g,i,in,tot); 
			for(EdgeList::iterator eit = g.getNeighbors(i).begin(); eit != g.getNeighbors(i).end(); eit++){ 
				if(i == (*eit).first)continue;
				map<int, EdgeWeight>::iterator comit = neighborComm.find(g.getCommunity((*eit).first));
				if(comit != neighborComm.end())
					comit->second += (*eit).second;
				else
					neighborComm.insert(make_pair(g.getCommunity((*eit).first), (*eit).second));
			}			
			int max = pre_comm;
			for(map<int, EdgeWeight>::iterator cmit=neighborComm.begin(); cmit!=neighborComm.end(); cmit++){ 
				gain = GraphUtil::modularityGain(g, i, cmit->first, cmit->second, in, tot);
				if(gain > maxgain){
					maxgain = gain;
					max = cmit -> first;
				}
			}			
			add(g,i,max,in,tot); 
			int root_C = roots[max];
 			bool changed_root = false;			
			if(root_C == -1){		
				roots[max] = i;
				root_C = i;
				t[i].community = max;
				changed_root = true;
			} 
			if(max != pre_comm){
				if(t[i].parent == -1 && t[i].community != -1)
					roots[t[i].community] = -1;
				if(t[i].parent != -1 && t[t[i].parent].parent == -1)
					roots[t[i].community] = t.getSibling(i);
				t.deleteNode(i);
				t.addNode(g, i, root_C, neighborComm, in, tot);
				
				if(t[t.num_nodes() - 1].parent == -1 && t[t.num_nodes() - 1].community != -1){				
					roots[t[t.num_nodes() - 1].community] = t.num_nodes() - 1;
				}
				changed = true;
			}
			gain = 0;
			maxgain = MIN;		
		}		
		mod = GraphUtil::modularity(g, in, tot);
	} while(changed &&((mod - pre_mod) > EPSILON) && pass < PASS_NUM); 
	return mod;
}

double Generator::memberAdjustUp(Graph& original_g, BinaryTree& original_t, vector<EdgeWeight>& original_in, vector<EdgeWeight>& original_tot, float EPSILON, int PASS_NUM){
	return detectCommunity(original_g, original_t, original_in, original_tot, EPSILON, PASS_NUM);
}

//the second phase
int Generator::constructNewGraph(Graph& original_g, BinaryTree& original_t, Graph& g, vector<EdgeWeight>& in, vector<EdgeWeight>& tot){ //O(n + m + nlgn)
	int num_comm = 0;	
	int cur_comm, v_comm;
	vector<int> ctoc(g.getNumVertices(), 0);	//O(n)
	int num_vertices = g.getNumVertices();	
	for(Node i = 0; i < g.getNumVertices(); i++){  //O(n)
		if(tot[i] > 0.000001)
			ctoc[i] = num_comm++;
	}
	for(Node i = 0; i < original_g.getNumVertices(); i++){
		original_g.setCommunity(i, ctoc[original_g.getCommunity(i)]);
	}
	for(int i = 0; i < original_t.num_nodes(); i++){
		if(original_t[i].community == -1)
			continue;
		original_t[i].community = ctoc[original_t[i].community];
	}
	
	vector<hash_map<int, EdgeWeight> > newGraphEdgeSet (num_comm, hash_map<int, EdgeWeight>()); //O(n)
	for(Node i = 0; i < g.getNumVertices(); i++){	//O(n)
		v_comm = g.getCommunity(i);
		for(EdgeList::iterator eit = g.getNeighbors(i).begin(); eit != g.getNeighbors(i).end(); eit++){ //O(m)
			cur_comm = g.getCommunity((*eit).first);
			if(cur_comm != v_comm){
				if(newGraphEdgeSet[ctoc[cur_comm]].find(ctoc[v_comm])==newGraphEdgeSet[ctoc[cur_comm]].end()){ //O(lgn)
					newGraphEdgeSet[ctoc[cur_comm]][ctoc[v_comm]]=(*eit).second;
				}else{
					newGraphEdgeSet[ctoc[cur_comm]][ctoc[v_comm]]+=(*eit).second;
				}
			}
		}		
	}	
	g.resize(num_comm); //O(n)
	for(Node i = 0; i < num_vertices; i++){ //O(n)
		if(tot[i] > 0.000001){
			g.setWeightedDegree(ctoc[i], tot[i]);
			g.setCommunity(ctoc[i], ctoc[i]);
			g.addEdge(ctoc[i], ctoc[i], in[i]);
			
			//Here is an attempt to add no self edge
			//g.setWeightedDegree(ctoc[i], tot[i]-in[i]);
			//g.setCommunity(ctoc[i], ctoc[i]);
		}
	}	
	
	for(vector<hash_map<int, EdgeWeight> >::iterator it=newGraphEdgeSet.begin(); it!=newGraphEdgeSet.end(); it++){ //O(n)
		for(hash_map<int, EdgeWeight>::iterator jt=it->begin(); jt!=it->end(); jt++){ //O(m)
			g.addEdge(distance(newGraphEdgeSet.begin(), it), jt->first, jt->second);
		}
	}
	return num_comm;
}

int Generator::LouvainAlgorithm(Graph& g, Graph& original_g, BinaryTree& original_t, vector<vector<int> >& community, float EPSILON, int PASS_NUM, vector<EdgeWeight>& in, vector<EdgeWeight>& tot){ //O(n + m + nlgn)
	double mod = 0;
	double pre_mod = 0;
	int pass = 0;
	int com = 0;
	vector<int> leaf2root;
	do{
		//initialize
		int num_ver = g.getNumVertices();
		in.clear();
		tot.clear();
		BinaryTree t(num_ver);
		in.resize(num_ver);
		tot.resize(num_ver);
		pass++;
		pre_mod = mod;
		for (int i = 0; i < num_ver; i++){  //O(n + m)		
			in[i]  = getEdgeWeightBetweenVertices(g, i, i);
			tot[i] = g.getWeightedDegree(i);
		}
		mod = detectCommunity(g, t, in, tot, EPSILON, PASS_NUM);	
		for(Node n = 0; n < original_g.getNumVertices(); n++){
			int comm = g.getCommunity(original_g.getCommunity(n));
			original_g.setCommunity(n, comm);
		}		
		Engraft(original_t, t);		
		mod = memberAdjustUp(original_g, original_t, in, tot, EPSILON, PASS_NUM);		
		com = constructNewGraph(original_g, original_t, g, in, tot);		
	}while((mod - pre_mod) > EPSILON && pass <= PASS_NUM);
	original_t.reorder();
	return com;
}

void Generator::ClusterMerge(Graph& cur_g, Graph& original_g, BinaryTree& original_t){
	queue<int> q;	
	for(int i = 0; i < (int)(cur_g.getNumVertices()); i++){
		q.push(i);
	}
	int pre_q_size = 0, q_size = (int)q.size();
	bool changed = false;
	double ModularityThreshold = 0;
	vector<bool> active(q.size(), true);
	vector<int> roots = original_t.getRoots();
	vector<EdgeWeight> in, tot;
	int num_ver = cur_g.getNumVertices();
	in.resize(num_ver);
	tot.resize(num_ver);
	bool single = false;
	for(int i = 0; i < cur_g.getNumVertices(); i++){  //O(n' + m')		
		in[i]  = getEdgeWeightBetweenVertices(cur_g, i, i);
		tot[i] = cur_g.getWeightedDegree(i);
	}	
	if(q.size() == 1) {
		single = true;
		goto skip;
	}
	else {		 
		ModularityThreshold = getModThres(cur_g, in, tot, active, PER);
	}
	while(q_size > 1){
		if(changed)
			ModularityThreshold = getModThres(cur_g, in, tot, active, PER); 
		changed = false;
		do{
			pre_q_size = q_size;
			int v = q.front();
			q.pop();
			if(!active[v]) continue;
			int u = -1;
			double gain, mingain = 1;
			for(EdgeList::iterator eit = cur_g.getNeighbors(v).begin(); eit != cur_g.getNeighbors(v).end(); eit++){
				if(v == (*eit).first)continue;
				if(!active[(*eit).first])continue;						
				gain = abs(GraphUtil::modularityGain(cur_g, v, eit->first, eit->second, in, tot));			
				if(gain < mingain){
					mingain = gain;
					u = eit -> first;
				}
			}		
			if(mingain <= ModularityThreshold){		
				changed = true;
				cur_g.addVertex(Vertex());
				int nnode = cur_g.getNumVertices() - 1;			
				EdgeWeight voru = 0;
				hash_map<int, EdgeWeight> newGraphEdgeSet;
				for(EdgeList::iterator eit = cur_g.getNeighbors(v).begin(); eit != cur_g.getNeighbors(v).end(); eit++){
					if(!active[eit->first])continue;
					hash_map<int, EdgeWeight>::iterator hit = newGraphEdgeSet.find(eit->first);
					if(hit == newGraphEdgeSet.end())
						newGraphEdgeSet.insert(make_pair(eit->first, eit->second));
					else
						hit->second += eit->second;
				}
				for(EdgeList::iterator eit = cur_g.getNeighbors(u).begin(); eit != cur_g.getNeighbors(u).end(); eit++){				
					if(!active[eit->first])continue;
					if(eit->first == v) continue;
					hash_map<int, EdgeWeight>::iterator hit = newGraphEdgeSet.find(eit->first);
					if(hit == newGraphEdgeSet.end())
						newGraphEdgeSet.insert(make_pair(eit->first, eit->second));
					else
						hit->second += eit->second;
				}			
				for(hash_map<int, EdgeWeight>::iterator eit = newGraphEdgeSet.begin(); eit != newGraphEdgeSet.end(); eit++){
					if(eit->first == u || eit->first == v) voru += (eit->second);
					else {					
						cur_g.addEdge(eit->first, nnode, eit->second);
						cur_g.addEdge(nnode, eit->first, eit->second);
					}
				}
				cur_g.addEdge(nnode, nnode, voru);				
				active.push_back(true);
				q.push(nnode);
				active[u] = false;
				active[v] = false;
				in[u] = -1;
				in[v] = -1;
				tot[u] = -1;
				tot[v] = -1;
				in.push_back(voru);
				tot.push_back(cur_g.getWeightedDegree(nnode));
				original_t.mergeRoot(roots[v], roots[u], in, tot);
				roots[v] = -1;
				roots[u] = -1;
				roots.push_back(original_t.num_nodes() - 1);
				q_size--;
			}
			else{
				q.push(v);
			}		
		}while(pre_q_size > q_size);		
	}
	
	skip:
	int root = original_t.num_nodes() - 1;
	if(single) {
		for(int j = original_t.num_nodes() - 1; j >= 0; j--){
			if(original_t[j].parent == -1) {
				root = j;
			}
		}
	}
	
	original_t.setRoot(root);
	int label_begin_num = 0;
	original_t.traverseToLabel(root, label_begin_num);
	
	Node i = 0;
	for(; i < original_g.getNumVertices(); i++){
		original_t[i].in = getEdgeWeightBetweenVertices(original_g, i, i);
		original_t[i].tot = original_g.getWeightedDegree(i);
	}	
	for(int j = i; j < original_t.num_nodes(); j++){
		original_t[j].in = 0;
		original_t[j].tot = 0;
	}
	for(Node i = 0; i < original_g.getNumVertices(); i++){			
		int label = original_t[i].lrange;
		int cur = original_t[i].parent;
		while(cur != -1){			
			original_t[cur].in += original_t[i].in;
			original_t[cur].tot += original_t[i].tot;
			cur = original_t[cur].parent;			
		}		
		for(EdgeList::iterator eit = original_g.getNeighbors(i).begin(); eit != original_g.getNeighbors(i).end(); eit++){
			int neighbor_label = original_t[eit->first].lrange;
			int cur = original_t[i].parent;
			bool begin = false;
			while(cur != -1){
				if((label >= original_t[cur].lrange) && (label <= original_t[cur].rrange) && (neighbor_label >= original_t[cur].lrange) && (neighbor_label <= original_t[cur].rrange)){
					begin = true;
				}
				if(begin) original_t[cur].in += eit->second;
				cur = original_t[cur].parent;
			}			
		}
	}
}

//not construct vis graph as a graph object, only save nodes and edges in vector to meet the signature of generator.cpp
//return mod
double Generator::constructVisGraph(Graph& g, BinaryTree& tree, vector<Edge>& edges, vector<int>& nodes) {
	map<int, int> transfer;
	for(Node v = 0; v < g.getNumVertices(); v++) {
		g.setCluster(v, -1);
	}
	double mod = 0;
	for(vector<int>::iterator it = nodes.begin(); it != nodes.end(); it++) {
		int cluster = *it;		
		tree.labelCluster(*it, cluster, g);
		transfer.insert(make_pair(cluster, distance(nodes.begin(), it)));
		EdgeWeight in = tree[*it].in;
		EdgeWeight tot = tree[*it].tot;		
		mod += GraphUtil::modularity(g, in, tot);		
	}
	
	vector<vector<EdgeWeight> > cur_edges(nodes.size(), vector<EdgeWeight>(nodes.size(), 0));
	for(Node v = 0; v < g.getNumVertices(); v++) {
		if(g.getCluster(v) == -1)
			continue;
		int from = transfer[g.getCluster(v)];
		for(EdgeList::iterator eit = g.getNeighbors(v).begin(); eit != g.getNeighbors(v).end(); eit++){
			if(g.getCluster(eit->first) == -1)
				continue;
			cur_edges[from][transfer[g.getCluster(eit->first)]] += eit->second;
		}
	}
	
	for(vector<int>::iterator it = nodes.begin(); it != nodes.end(); it++) {
		for(vector<int>::iterator iit = it + 1; iit != nodes.end(); iit++) {
			EdgeWeight weight = cur_edges[transfer[*it]][transfer[*iit]];
			if(weight > 0)
				edges.push_back(Edge(*it, *iit, weight));
		}
	}
	
	return mod;
}

void Generator::init() {
	srand( time(NULL) );
	
	string input_graphfile = "./data/preprocessed_gene_coexp_small.txt";
	string input_treefile = "";
	//string input_treefile = "./data/preprocessed_com-youtube.tree";
	string input_nodefile = "./data/preprocessed_gene_coexp_small_name.txt";
	
	ifstream infile_graph(input_graphfile.c_str());
	ifstream infile_node(input_nodefile.c_str());
	
	double EPSILON = 0.000001;
	int	PASS_NUM = 100;
	vector<EdgeWeight> in, tot;	
	
	original_g.readGraph(infile_graph, infile_node, false);
	Graph g = original_g;
	int com_num = 300;
	
	if(input_treefile.compare("") != 0) {
		cout << "start read tree" << endl;
		tree.readTree(input_treefile);
		cout << "finish read tree" << endl;
	}
	else {
		tree.setSize(original_g.getNumVertices());	
		vector<vector<int> > community;
		LouvainAlgorithm(g, original_g, tree, community, EPSILON, PASS_NUM, in, tot);
		cout << "begin merge " << endl;
		ClusterMerge(g, original_g, tree);
		cout << "end merge " << endl;
	}
	tree.setLargestTableCount(com_num);
	cout << "begin construct table " << endl;
	tree.constructDPTable(original_g);
	cout << "end construct table " << endl;
	
	cout << "begin label cluster " << endl;
	tree.setClusterLableName(original_g);
	cout << "end label cluster " << endl;
}

// starting point of the program
void Generator::run(DBJob& job, string& new_vertices_str, string& new_edges_str, int& vertices_num, int& edges_num, double& mod, bool& flag) {
	vector<int> current_vertices;
	//parseVertexList(job.getCurrentVertices(), current_vertices);
	vector<Edge> new_edges;
	
	int query_v = tree.num_nodes() - 1;
	
	if(job.getQueryV().compare("root") != 0)
			query_v = Utils::strToInt(job.getQueryV());				
	
	flag = query(current_vertices, query_v, job.getInputLow(), job.getInputHigh());	
		
	//get the vis graph and set mod
	mod = constructVisGraph(original_g, tree, new_edges, current_vertices);
	
	cout << "mod " << mod << endl;
	
	//set new graph's vertices and edges' number
	vertices_num = (int) current_vertices.size();
	edges_num = (int) new_edges.size();	
	
	// set new_vertices_str and new_edges_str
	buildVertexStr(current_vertices, new_vertices_str);
	buildEdgeStr(new_edges, new_edges_str);
	
}

bool Generator::query(vector<int>& current_vertices, int v, int inputlow, int inputhigh) {
	
	/* // check if even contains query
	if (find(current_vertices.begin(), current_vertices.end(), v) == current_vertices.end()) {
		return false;
	} */
	
	//tree.writeDPTable();
	
	int vis_nodes_num = tree.getFixInput(v, inputlow, inputhigh);
	
	tree.getVisualizationNodes(v, vis_nodes_num, current_vertices);
	
	return true;
}

 void Generator::parseVertexList(const string& current_v_list, vector<int>& current_vertices) {
	vector<string> tokens;
	Utils::split(current_v_list, ',', tokens);
	for (vector<string>::iterator it = tokens.begin(); it != tokens.end(); it++) {
		vector<string> node_token;
		Utils::split(*it, '>>', node_token);
		int v = Utils::strToInt(node_token[0]);
		current_vertices.push_back(v);
	}
}

void Generator::buildVertexStr(const vector<int>& current_vertices, string& new_vertices_str) {
	for (vector<int>::const_iterator it = current_vertices.begin(); it != current_vertices.end(); it++) {
		int node = *it;
		EdgeWeight in = tree[node].in;
		EdgeWeight tot = tree[node].tot;		
		double mod = GraphUtil::modularity(original_g, in, tot);
		if(mod < 0) {
			mod = 0;
		}		
		int child_num = tree[node].rrange - tree[node].lrange + 1;
		int cluster = tree[node].community;
		if(cluster < 0) {
			cluster = node;
		}
		new_vertices_str += Utils::numToStr(node) + ">>" + Utils::numToStr(child_num) + ">>" + Utils::numToStr(mod) + ">>" + Utils::numToStr(cluster) + ">>" + tree[node].label_name;
		if (it != current_vertices.end()-1)
			new_vertices_str += ";";
	}
}

void Generator::buildEdgeStr(const vector<Edge>& new_edges, string& new_edges_str) {
	for (vector<Edge>::const_iterator it = new_edges.begin(); it != new_edges.end(); it++) {
		new_edges_str += Utils::numToStr((*it).v1) + "," + Utils::numToStr((*it).v2) + "," + Utils::numToStr((*it).weight);
		if (it != new_edges.end()-1)
			new_edges_str += ";";
	}
}

void Generator::buildLabelName(const vector<int>& current_vertices, string& label_name) {
	for (vector<int>::const_iterator it = current_vertices.begin(); it != current_vertices.end(); it++) {		
		label_name += tree[*it].label_name;
		if (it != current_vertices.end()-1)
			label_name += ";";
	}
}


