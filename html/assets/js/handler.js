
var vertices = [];
var edges = [];
var nodes_num = 1;
var edge_num = 0;
var modularity = 0;
var user_input_lower = 1;
var user_input_higher = 300;
var pre_query = new Array();
var name_list = new Array();
var max_weights = 1;
var max_edge_weights = 1;
var type = 0;
var edge_type = 0;
var percentage = 0;

function init() {
	vertices = [];
	edges = [];
	type = 0;
	vertices.push(
		{ "name" : "root", "node_weight" : 1, "modularity" : 0, "group" : 2, "label" : "root", "degree" : 0}
	);
	
	pre_query = new Array();
	
	name_list = new Array();
	
	/* vertices.push(
		{ "name" : 2, "group" : 2}
	);
	
	edges.push(
		{"source" : 0, "target" : 1, "value" : 8}
	); */
	
	max_weights = 1;
	max_edge_weights = 1;
	
	visualize(type, max_weights, edge_type, max_edge_weights, nodes_num);
}

function parseResponse(response) {
	
	var v_list = response.v_list;
	var e_list = response.e_list;
	nodes_num = response.nodes_num;
	edge_num = response.edge_num;
	modularity = response.modularity;
	
	vertices = [];
	edges = [];

	var v_lookup = {};
	var v_tokens = v_list.split(";");
	var weights = [];
	var names = [];
	var cluster_modularity = [];
	var cluster = [];
	var label = [];
	
	max_weights = 0;
	
	for (var i = 0; i < v_tokens.length; i++) {
		var tokens = v_tokens[i].split(">>");
		names.push(tokens[0]);
		var cur_weights = parseInt(tokens[1],10);
		weights.push(cur_weights);
		if(max_weights < cur_weights) {
			max_weights = cur_weights;
		}
		if(tokens.length == 5) {
			cluster_modularity.push(tokens[2]);
			cluster.push(tokens[3]);
			label.push(tokens[4]);
		}
		else {
			cluster_modularity.push(tokens[2] + ">>" + tokens[3]);
			cluster.push(tokens[4]);
			label.push(tokens[5]);
		}
	}
	document.getElementById("demo").innerHTML = "";
	for (var i = 0; i < label.length; i++) {
		document.getElementById("demo").innerHTML += label[i] + "<br/>";
	}
	type = 0;
	for (var i = 0; i < v_tokens.length; i++) {
		if(weights[i] != max_weights){
			type = 1;
			break;
		}
	}
	for (var i = 0; i < v_tokens.length; i++) {
		var cur_weight =  weights[i];
		vertices.push(
			{ "name" : names[i], "node_weight" : cur_weight, "modularity" : cluster_modularity[i], "group" : cluster[i], "label" : label[i] }
		);
		v_lookup[names[i]] = i;
	}	
	
	// if edges exist
	if (e_list != "") {
		var e_tokens = e_list.split(";");
		var edge_weights = [];
		var s_list = [];
		var t_list = [];
		max_edge_weights = 0;
		
		for (var i = 0; i < e_tokens.length; i++) {
			var buf = e_tokens[i].split(",");
			s_list.push(parseInt(buf[0],10));
			t_list.push(parseInt(buf[1],10));
			var cur_weights = parseFloat(buf[2],10);
			edge_weights.push(cur_weights);
			if(max_edge_weights < cur_weights) {
				max_edge_weights = cur_weights;
			}
		}
		
		edge_type = 0;
		for (var i = 0; i < e_tokens.length; i++) {
			if(edge_weights[i] != max_edge_weights){
				edge_type = 1;
				break;
			}
		}
		
		for (var i = 0; i < e_tokens.length; i++) {
			var s = v_lookup[s_list[i]];
			var t = v_lookup[t_list[i]];
			var w = edge_weights[i];
			edges.push(
				{ "source" : s, "target" : t, "value" : w, "status" : 0}
			);
		}
	}
	visualize(type, max_weights, edge_type, max_edge_weights, nodes_num);
}

function changeInfoTable(node_num, edge_num, modularity) {	
	document.getElementById('node_numbers').innerHTML = node_num;
	document.getElementById('edge_numbers').innerHTML = edge_num;
	if(modularity > 0)
		document.getElementById('modularity').innerHTML = modularity;
	else
		document.getElementById('modularity').innerHTML = 0;
}

function startQuery(query_type, query_v, user_input_lower, user_input_higher) {
	$("#status-message").html("");
	
	$.ajax({
		url: "process_query.php",
		beforeSend: function() {
			$('#status-message').Loadingdotdotdot({
				"speed": 400,
				"maxDots": 4,
				"message": 'Processing'
			});
		},
		complete: function() {
			//$("#status-message").Loadingdotdotdot("Stop");
		},
		type: 'POST',
		data: { type : query_type, query_v : query_v, vertices : vertices, nodes_num : nodes_num, edge_num : edge_num, modularity : modularity, user_input_lower : user_input_lower, user_input_higher : user_input_higher},
		dataType: 'json',
		success: function(response) {
			if (response.hasOwnProperty('error')) {
				$("#status-message").html("There was an error while processing the request.");
			}
			else {
				percentage = 0;
				document.getElementById("weights").innerHTML = percentage.toFixed(2);
				document.getElementById("label_control").value = "hide label";
				$("#status-message").html("");
				parseResponse(response);
				changeInfoTable(nodes_num, edge_num, modularity);
			}
		},
		error: function(xhr, textStatus, error){
			alert("Error processing the request.");
		}
	});
}

function onClick() {
	user_input_lower = document.getElementById("input_lower_num").value;
	user_input_higher = document.getElementById("input_higher_num").value;
	var query_v = pre_query.pop(); // dummy variable
	if(pre_query.length == 0)
		query_v = "root";
	pre_query.push(query_v);
	startQuery(1, query_v, user_input_lower, user_input_higher);
}

$(document).ready(function() {
	
	init();
	
	$('#query_root_btn').click(function(e){
		e.preventDefault();
		
		init();
		//document.getElementById("cluster").innerHTML = "root";
		//document.getElementById("demo").innerHTML = pre_query;
	});
	
	$('#query_up_all_btn').click(function(e){
		e.preventDefault();
		var query_type = 4;
		pre_query.pop();
		var query_v = pre_query.pop();
		pre_query.push(query_v);
		startQuery(query_type, query_v, user_input_lower, user_input_higher);
		//document.getElementById("cluster").innerHTML = query_v;
		//document.getElementById("demo").innerHTML = pre_query;
	});
	
	$('#edge_add').click(function(e){
		e.preventDefault();
		if(percentage < 0.99)
			percentage = percentage + 0.1;
		document.getElementById("weights").innerHTML = percentage.toFixed(2);		
		var svg = d3.select("#vis");
		var link = svg.selectAll(".link");
		link.style("visibility", function(d) {
			if(d.value < percentage * max_edge_weights) {
				if(d.status == 0) {
					d.source.degree = d.source.degree - 1;
					d.target.degree = d.target.degree - 1;
				}
				d.status = 1;
				return "hidden";
			}
			else {
				if(d.status == 1) {
					d.source.degree = d.source.degree + 1;
					d.target.degree = d.target.degree + 1;
				}
				d.status = 0;
				return "visible";
			}
		});
		var texts = svg.selectAll("text.label");
		texts.style("visibility", function(d){
			if(d.degree > 0)
				return "visible";
			else 
				return "hidden";
			
		});
	});
	
	$('#edge_minus').click(function(e){
		e.preventDefault();
		if(percentage >= 0.1)
			percentage = percentage - 0.1;
		document.getElementById("weights").innerHTML = percentage.toFixed(2);	
		var svg = d3.select("#vis");
		var link = svg.selectAll(".link");		
		link.style("visibility", function(d) {
			if(d.value < percentage * max_edge_weights) {
				if(d.status == 0) {
					d.source.degree = d.source.degree - 1;
					d.target.degree = d.target.degree - 1;
				}
				d.status = 1;
				return "hidden";
			}
			else {
				if(d.status == 1) {
					d.source.degree = d.source.degree + 1;
					d.target.degree = d.target.degree + 1;
				}
				d.status = 0;
				return "visible";
			}
		});
		var texts = svg.selectAll("text.label");
		texts.style("visibility", function(d){
			if(d.degree > 0)
				return "visible";
			else 
				return "hidden";
			
		});
	});
	
	$('#label_control').click(function(e){
		e.preventDefault();
		var cur_do = document.getElementById("label_control").value;		
		var svg = d3.select("#vis");
		var texts = svg.selectAll("text.label");
		texts.style("visibility", function(d){
			if(cur_do == "hide label") {
				document.getElementById("label_control").value = "show label";
				return "hidden";
			}
			else {
				document.getElementById("label_control").value = "hide label";
				if(d.degree > 0 || d.weight == 0)
					return "visible";
				else 
					return "hidden";
			}
		});
	});
	
	$(function(){
		$.contextMenu({
			selector: '.context-menu-query',
			callback: function(key, options) {
				var clickedNode = this[0].__data__;
				console.log(clickedNode);
				var query_type = 1;  // default down
				/* if (key == "up")
					query_type = 2; */
				
				startQuery(query_type, clickedNode.name, user_input_lower, user_input_higher);
				pre_query.push(clickedNode.name);
				//document.getElementById("cluster").innerHTML = clickedNode.name;
				//document.getElementById("demo").innerHTML = pre_query;
			},
			items: {
				/* "up": {name: "Query Up"},
				"sep1": "---------",
			    "down": {name: "Query Down"}, */
				"down": {name: "expand"},
			}
		});
		
		$('.context-menu-query').on('click', function(e){
			//console.log('clicked', this);

		})
	});
	
});              
