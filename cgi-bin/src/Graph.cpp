#include "Graph.h"

Graph::Graph(){
	vl = VertexList(0);
	vsize = 0;
	esize = 0;
	EdgeList el;
	graph = GRA(vsize, el);
	total_weight = 0;
}

Graph::Graph(istream& in, istream& label, bool preprocess) {
	readGraph(in, label, preprocess);
}

Graph::~Graph() {}

void Graph::readGraph(istream& in, istream& label, bool preprocess){
	string buf;
	vsize = 0;
	int id;
	int base = MAX_BASE;
	while(getline(in, buf)){
		if(buf.find("#") != string::npos)
			continue;
		istringstream(buf.substr(0, buf.find("	"))) >> id;
		base = base < id ? base : id;
		if(vsize < id)
			vsize = id;
		buf.erase(0, buf.find("	")+1);
		
		istringstream(buf.substr(0, buf.find("	"))) >> id;
		base = base < id ? base : id;
		if(vsize < id)
			vsize = id;
	}
	id = 0;
	EdgeList el;
	vsize -= base;
	vl = VertexList(++vsize);	
	graph = GRA(vsize, el);
	
	//build edges
	in.clear();
	in.seekg(0, ios::beg);
	Node sid, tid;
	EdgeWeight weight = 1;
	while(getline(in, buf)){
		if(buf.find("#") != string::npos)
			continue;
		istringstream(buf.substr(0, buf.find("	"))) >> sid;
		sid -= base;
		buf.erase(0, buf.find("	")+1);
		
		istringstream(buf.substr(0, buf.find("	"))) >> tid;
		tid -= base;
		buf.erase(0, buf.find("	")+1);

		if(buf.find("	") != string::npos)
			istringstream(buf.substr(0, buf.find("	"))) >> weight;
		else
			istringstream(buf.substr(0, string::npos)) >> weight;
		addEdge(sid, tid, weight);
		vl[sid].weighted_degree += weight;
		if(tid != sid){
			esize--;
			addEdge(tid, sid, weight);
			vl[tid].weighted_degree += weight;
		}		
		total_weight += weight;
	}
	if(preprocess) {
		while(id < vsize) {
			if(graph[id].size() == 0) {			
				id++;
				continue;
			}
			Compare com;
			sort(graph[id].begin(), graph[id].end(), com);
			int cut = 1;
			for(int i = 1; i < (int) graph[id].size(); i++) {
				if(graph[id][i] != graph[id][i - 1]) {
					graph[id][cut] = graph[id][i];
					cut++;
				}
			}
			graph[id].resize(cut);
			id++;
		}
	}
	else {
		buf.clear();
		while (getline(label, buf)) {
			int nid = 0;
			istringstream(buf.substr(0, buf.find("	"))) >> nid;
			buf.erase(0, buf.find("	")+1);
		
			string cur_label = buf.substr(1, buf.length() - 3);
			buf.clear();
		
			vl[nid].label_name = cur_label;
		}
	}
	while(id < vsize){
		setCommunity(id, id);
		id++;
	}
	
}

void Graph::readGraph(istream& in, bool preprocess){
	string buf;
	vsize = 0;
	int id;
	int base = MAX_BASE;
	while(getline(in, buf)){
		if(buf.find("#") != string::npos)
			continue;
		istringstream(buf.substr(0, buf.find("	"))) >> id;
		base = base < id ? base : id;
		if(vsize < id)
			vsize = id;
		buf.erase(0, buf.find("	")+1);
		
		istringstream(buf.substr(0, buf.find("	"))) >> id;
		base = base < id ? base : id;
		if(vsize < id)
			vsize = id;
	}
	id = 0;
	EdgeList el;
	vsize -= base;
	vl = VertexList(++vsize);	
	graph = GRA(vsize, el);
	
	//build edges
	in.clear();
	in.seekg(0, ios::beg);
	Node sid, tid;
	EdgeWeight weight = 1;
	while(getline(in, buf)){
		if(buf.find("#") != string::npos)
			continue;
		istringstream(buf.substr(0, buf.find("	"))) >> sid;
		sid -= base;
		buf.erase(0, buf.find("	")+1);
		
		istringstream(buf.substr(0, buf.find("	"))) >> tid;
		tid -= base;
		buf.erase(0, buf.find("	")+1);

		if(buf.find("	") != string::npos)
			istringstream(buf.substr(0, buf.find("	"))) >> weight;
		else
			istringstream(buf.substr(0, string::npos)) >> weight;
		addEdge(sid, tid, weight);
		vl[sid].weighted_degree += weight;
		if(tid != sid){
			esize--;
			addEdge(tid, sid, weight);
			vl[tid].weighted_degree += weight;
		}		
		total_weight += weight;
	}
	if(preprocess) {
		while(id < vsize) {
			if(graph[id].size() == 0) {			
				id++;
				continue;
			}
			Compare com;
			sort(graph[id].begin(), graph[id].end(), com);
			int cut = 1;
			for(int i = 1; i < (int) graph[id].size(); i++) {
				if(graph[id][i] != graph[id][i - 1]) {
					graph[id][cut] = graph[id][i];
					cut++;
				}
			}
			graph[id].resize(cut);
			id++;
		}
	}
	while(id < vsize){
		setCommunity(id, id);
		id++;
	}
	
}

void Graph::writeGraph(){
	cout<<"number of vertices: "<<getNumVertices()<<"  number of edges: "<<getNumEdges()<<endl;
	for(Node i = 0; i < getNumVertices(); i++){
		cout<<"node "<<i<<"; community "<<getCommunity(i)<<endl;
	}
	for(Node i = 0; i < getNumVertices(); i++){
		for(EdgeList::iterator eit = getNeighbors(i).begin(); eit != getNeighbors(i).end(); eit++){
			cout<<i<<" "<<(*eit).first<<" "<<(*eit).second<<endl;
		}
	}
}

void Graph::resize(int num){
	graph.clear();
	vl.resize(num);
	EdgeList el;
	graph = GRA(vsize, el);
	vsize = num;
	esize = 0;
}

Graph& Graph::operator=(const Graph& g){
	if (this != &g) {
		graph = g.graph;
		vl = g.vl;
		vsize = g.vsize;
		esize = g.esize;
		total_weight = g. total_weight;
	}
	return *this;
}

