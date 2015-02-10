#include "GraphUtil.h"

double GraphUtil::modularity(Graph& g, vector<EdgeWeight>& in, vector<EdgeWeight>& tot){ //O(n)
	double sum = 0;
	double m = g.getTotalWeight();
	for (vector<EdgeWeight>::size_type i = 0; i < in.size(); i++){
		if(tot[i] > 0){
			sum += in[i] / (2 * m) - tot[i] * tot[i] / (4 * m * m);
		}
	}
	return sum;
}

double GraphUtil::modularity(Graph& g, EdgeWeight in, EdgeWeight tot){ //O(1)
	double m = g.getTotalWeight();
//cout<<"in is "<<in<<" tot is "<<tot<<" m is "<<m<<endl;	
	double sum = in / (2 * m) - tot * tot / (4 * m * m);
	return sum;
}

double GraphUtil::modularityGain(Graph& g, Node i, int comm, EdgeWeight v_to_comm_weight, vector<EdgeWeight>& in, vector<EdgeWeight>& tot){ //O(1)
	double m = g.getTotalWeight();
	double n2c = v_to_comm_weight;
	double wd = g.getWeightedDegree(i);
//if(i == 468102) {
//cout << "tot range: " << (int) tot.size() <<"; i is "<< i << " comm is " << comm <<" m is "<<m<<" n2c is "<<n2c<<" wd is "<<wd<<" "<<endl;	}
	return (n2c - tot[comm] * wd / (2 * m)) / m;
	//return (2 * n2c - ( 2 * tot[comm] * wd + wd * wd) / (2 * m)) / 2 * m;
}

