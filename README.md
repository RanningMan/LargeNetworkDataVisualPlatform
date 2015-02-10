In this project, we developed an efficient visual analysis platform for large scale networks to help researchers discover interesting relationships between nodes and communities in large graphs easily. The visualization is based on the construction of a clustering tree. To build this clustering tree, we proposed an algorithm that based on the maximization of the network modularity. After successfully constructing the clustering tree, we developed an efficient algorithm which forms the summarized intermediate cluster graphs with maximum modularity to help users visualize the whole graph in an iterative way. With the help of the clustering tree, the algorithm for constructing intermediate cluster graphs is time efficient. Therefore the platform supports online analysis for graphs with huge number of nodes and links.
We also developed several user-friendly functions in the interface of the platform. According to the user specified range of node number, our system is able to visualize a network component by generating an intermediate graph which summarizes the component. In the intermediate graph, bigger nodes represent more vertices in the corresponding clusters, and thicker edges represent larger weights of links connecting two clusters.


/********* Setup *********/

Please make sure that you have copied all the folders, source code and files on the server before moving on.

/********* database *********/

please construct the database as follows.

Table vis_jobs: 
id
status -> gets updated to "pending", "processing", "complete"
type -> set to 1 for queryDown, 2 for queryUp, 3 for queryDownAll, 4 for queryUpAll (currently only support type 1 and type 4, can be improved in the future if necessary)
query_v -> the vertex being queried
current_v_list -> list of current vertices (comma-separated)
new_v_list -> new vertex list returned by the C++ program (semicolon-separated), for each vertices, use ">>" to separate all their attributes (id>>children number>>modularity>>module id>>label)
new_e_list -> new edge list returned by the C++ program (v1 and v2 are comma-separated, edges are semicolon-separated)
start_time
end_time
node_num -> the number of nodes that will be shown in the current graph
edge_num -> the number of edges that will be shown in the current graph
modularity -> the modularity of the current graph
user_input_lower -> the value of input lower bound, from user's input
user_input_higher -> the value of input higher bound, from user's input


/********* cgi-bin *********/

The starting point of the program is main.cpp. It sits in a while(true) loop, checking the database for new tasks every 2 seconds. If a task is found, a thread will be dispatched to process the task.

An instance of Generator is held in memory, which contains a full binary clustering tree. The tree is generated from the original graph.

The work flow of the program is as follows:

1) In init() function of generator class, it first reads TWO files, one edge file and one node file (in which should contain label for each node). Then by applying readGraph function of graph class, it read the graph and save it in the memory. After reading graph, the program uses LouvainAlgorithm and ClusterMerge function to do the clustering and build the clustering tree. The final step of constructing the tree is to build a table for each node of the tree to help find the intermediate cluster graph with maximum modularity, according to the minimum and maximum numbers of clusters given by users.

NOTE: because constructing the clustering tree may take 1 or 2 hours when graph has more than 1m nodes, you can also construct the tree first and save it in a separate. When start the visualization program, just chaneg the path of the constructed tree file in line 428 of generator.cpp

2) After the construction of clustering tree, the visualization program is ready to interact with users. When a task from a user is initially put into the database from php program, it has the status "pending". The C++ program finds these tasks, and updates the status field as "processing". 

Interaction with the database is contained in the DBManager class.  

3) When the program get the information of pending job, run() function of generator class will realize the intermediate graph construction and generate result data to save in database waiting for php program to retrieve. 

4) After the thread computes the result, it will finally set the status to "complete". At this point, the PHP code knows the task is done to retrieve the result (that is, new_v_list and new_e_list).

For details, please check the source code.


/********* html *********/

The interface starts with one node, the root of the clustering tree, by default. This is displayed using the force-directed graph layout from d3.js. 

On the left side of the interface window, users can input the lower bound and higher bound of the node numbers of intermediate graph that will be drawn. The corresponding information of the graph will also be shown.

The "previous" button is used to redraw the previous one graph of current graph; the "back" button is used to get back to the root and clear all the history; the "+" button shown less edges while the "-" button shows more by filtering the number of edges between two clusters shown in the graph; clicking "hide label"/"show label" will hide/show the label of nodes; right-clicking on a node gives the option to extend on that node. These calls are performed using ajax in handler.js under the function startQuery(query_type, query_v, user_input_lower, user_input_higher).

process_query.php is called from the client which takes the (possibly) queried vertex as well as all vertices, inserts them as a task to the database, waits for the "complete" status on that row, finally retrieves the result and sends it back to the client as json.

At this point, the result updates the global vertices and edges variables, and visualize() is called in visualize.js. This removes the current force-directed graph and starts a new one with the updated data using d3.js.

