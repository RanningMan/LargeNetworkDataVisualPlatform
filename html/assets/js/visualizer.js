
function visualize(type, max_weights, edge_type, max_edge_weights, node_num) {

	$('#vis').html('');

	var w = 1000, h = 800;
	var color = d3.scale.category20().domain(d3.range(0,10));  // set domain explicitly

	var force = d3.layout.force()
		.charge(-100)
		.size([w, h]);

	var svg = d3.select("#vis").append("svg:svg")
		.attr("width", w)
		.attr("height", h);

	force
		.nodes(vertices)
		.links(edges)
		.linkDistance(function(d){
			var norm = d.value / max_edge_weights;
			if(norm < 0.5)
				norm = 0.5;
			if(type == 0)
				norm = 0.5;	
			if(node_num < 100)
				return 200 / norm;
			else if(node_num < 200)
				return 130 / norm;
			else
				return 100 / norm; 
		})
		.linkStrength(function(d) {
			var norm = d.value / max_edge_weights;
			if(norm < 0.1)
				norm = 0.1;
			if(type == 0)
				norm = 0.1;	
			return norm;
		})
		.start();

	var link = svg.selectAll(".link")
		.data(edges)
		.enter().append("line")
		.attr("class", "link")
		.style("stroke-width", function(d) { 
			 var norm = d.value / max_edge_weights;
			if(norm < 0.1)
				norm = 0.1;
			if(max_edge_weights < 5)
				norm = 0.1;
			return norm * 10;
		});
	
	link.append("title")
		.html(function(d) { 
			return d.value;
		});

	var node = svg.selectAll(".node")
		.data(vertices)
		.enter().append("circle")
		.attr("class", "node context-menu-query")		
		.style("fill", function(d) { return color(d.group + 2); })
		.call(force.drag)
		.attr("r", function(d) {
			d.degree = d.weight;
			var norm = d.node_weight / max_weights;
			if(max_weights > 20) {
				if(norm < 0.1)
					norm = 0.1;
			}
			if(type == 0)
				norm = norm / 5;	
			return norm * 50; 
		} );

	node.append("title")
		.html(function(d) { 
			var title_text = d.label;
			title_text += "</br>nodes number: " + d.node_weight;
			title_text += "</br>modularity: " + d.modularity;
			title_text += "</br>community number: " + d.group;
			return title_text;
		});
	
	var texts = svg.selectAll("text.label")
                .data(vertices)
                .enter().append("text")
                .attr("class", "label context-menu-query")
                .attr("fill", "black")
				.style("font-size","24px")
				.style("visibility","visible")
				.attr({
					"dy": -3,
					"transform": "translate(" + 20 + "," + 15 + ")"
				 })
                .text(function(d) {  return d.label;  });

	force.on("tick", function() {
		link.attr("x1", function(d) { return d.source.x; })
			.attr("y1", function(d) { return d.source.y; })
			.attr("x2", function(d) { return d.target.x; })
			.attr("y2", function(d) { return d.target.y; });

		node.attr("cx", function(d) { return d.x; })
			.attr("cy", function(d) { return d.y; });
			
		texts.attr("transform", function(d) {
			return "translate(" + d.x + "," + d.y + ")";
		});
	});
	
	
	
}
