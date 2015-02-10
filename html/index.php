<!DOCTYPE html>

<html lang="en">
  <head>
    <meta charset="utf-8">
    <title>Vis Demo</title>
    <script type="text/javascript" src="./assets/js/jquery-1.7.2.min.js"></script>
    <script type="text/javascript" src = "./assets/js/jquery.loadingdotdotdot.js"></script>
	<script type="text/javascript" src = "./assets/js/visualizer.js"></script>
	<script type="text/javascript" src="./assets/js/d3.v3.min.js"></script>
	<script type="text/javascript" src = "./assets/js/handler.js"></script>
	<script type="text/javascript" src="./assets/js/jquery.ui.position.js"></script>
    <script type="text/javascript" src="./assets/js/jquery.contextMenu.js"></script>
	
	<meta name="viewport" content="width=device-width, initial-scale=1.0">
	<meta name="description" content="">
    <meta name="author" content="">
	
	<link href="./assets/css/bootstrap.css" rel="stylesheet">
	<link href="./assets/css/style.css" rel="stylesheet">
	<link href="./assets/css/jquery.contextMenu.css" rel="stylesheet"/>
  </head>

  <body>
	<div id="main">
		
		<div class="topban span12">
			<h1>Vis Demo</h1>
		</div>

		<div class="container-fluid">
			<div class="row-fluid">
				<div class="span2">
					<div class="bs-docs-sidebar">
						<ul class="nav nav-list bs-docs-sidenav">
							<li class='active'><a href='#'>Home</a></li>
						</ul>
					</div>				
					
					<div>
						<p id="cluster"></p>
						<p>please input lower bound: </p><input type="text" id="input_lower_num" value="1"><br>
						<p>please input higher bound: </p><input type="text" id="input_higher_num" value="300"><br>
						<input id="input" type="submit" onclick="onClick()">						
					</div>
					
					<div id="graph_info">
						<table style="width:100%" id="info">
							<tr>
								<td >node numbers</td> 
								<td id = "node_numbers"></td>
							</tr>
							<tr>
								<td >edge numbers</td> 
								<td id = "edge_numbers"></td>
							</tr>
							<tr>
								<td >modularity</td> 
								<td id = "modularity"></td>
							</tr>
						</table>
					</div>
					<div id="stack">
						<p id="demo"></p>
					</div>
				</div>
				<div class="span10">
					<div class="hero-unit" height="500px" align="justify">
						<div id="status-message"></div>
						<div id = "primary-wrapper">
							<div id = "query-btn-container">
								<input type="submit" value="previous" id="query_up_all_btn" class = "btn btn-primary btn-large btn-zoom"/><br />
								<input type="submit" value="back" id="query_root_btn" class = "btn btn-primary btn-large btn-zoom"/><br />
								<input type="submit" value="+" id="edge_add" class = "btn btn-primary btn-large btn-zoom"/><br />
								<br /><p id="weights" value="percentage: "></p><br />
								<input type="submit" value="-" id="edge_minus" class = "btn btn-primary btn-large btn-zoom"/><br />
								<input type="submit" value="hide label" id="label_control" class = "btn btn-primary btn-large btn-zoom"/>
							</div>
							<div id = "visual-container">
								<div id="vis"></div>
							</div>
						</div>
					</div>
					<p id="pre_weights"></p>
					<footer>Copyright &copy; The Network Laboratory of Biomedical Informatics, The Ohio State University</footer>
				</div>
				
			</div>
		</div>
	</div>
  </body>
</html>
