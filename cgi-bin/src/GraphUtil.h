#ifndef _GRAPHUTIL_H
#define _GRAPHUTIL_H

#include "Graph.h"

using namespace std;

class GraphUtil{
	public:
		static double modularity(Graph& g, vector<EdgeWeight>& in, vector<EdgeWeight>& tot);
		static double modularityGain(Graph& g, Node i, int comm, EdgeWeight v_to_comm_weight, vector<EdgeWeight>& in, vector<EdgeWeight>& tot);
		static double modularity(Graph& g, EdgeWeight in, EdgeWeight tot);
		static double modularityGain(Graph& g, Node i, double neighbor, EdgeWeight tot);
};

#endif
