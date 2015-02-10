<?php

	if (isset($_POST['user_input_lower']) && isset($_POST['user_input_higher']) && isset($_POST['type']) && isset($_POST['query_v']) && isset($_POST['vertices']) && isset($_POST['nodes_num']) && isset($_POST['edge_num']) && isset($_POST['modularity'])) {
		
		$final_result = array();
		
		// load json functions
		if (!function_exists('json_decode')) {
			function json_decode($content, $assoc=false) {
				require_once 'JSON.php';
				if ($assoc) {
					$json = new Services_JSON(SERVICES_JSON_LOOSE_TYPE);
				}
				else {
					$json = new Services_JSON;
				}
				return $json->decode($content);
			}
		}

		if (!function_exists('json_encode')) {
			function json_encode($content) {
				require_once 'JSON.php';
				$json = new Services_JSON;
				return $json->encode($content);
			}
		}
	
		# retrieve data from POST
		$type = trim($_POST['type']);
		$query_v = trim($_POST['query_v']);
		$vertices = $_POST['vertices'];
		$nodes_num = $_POST['nodes_num'];
		$edge_num = $_POST['edge_num'];
		$modularity = $_POST['modularity'];
		$user_input_lower = $_POST['user_input_lower'];
		$user_input_higher = $_POST['user_input_higher'];

		# build vertices_str
		$vertices_str = '';
		foreach($vertices as $key => $value) {
			$vertices_str .= $value['name'] . ',';
			/* $vertices_str .= $value['weight'] . '-';
			$vertices_str .= $value['modularity'] . ','; */
		}
		$vertices_str = rtrim($vertices_str, ',');

		# insert query to db
		include 'connect.php';
		$sql = "INSERT INTO vis_jobs(status, type, query_v, current_v_list, start_time, user_input_lower, user_input_higher) VALUES ('pending', '$type', '$query_v', '$vertices_str', NOW(), '$user_input_lower', '$user_input_higher')";
		
		if (mysql_query($sql)) {
			$id = mysql_insert_id();
			$count = 0;
			$completed = false;
			while ($count < 8) {
				sleep(2);
				$sql = "SELECT new_v_list, new_e_list, nodes_num, edge_num, modularity FROM vis_jobs WHERE id = '$id' AND status = 'complete'";
				$result = mysql_query($sql);
				if (mysql_affected_rows() > 0) {
					$completed = true;
					$row = mysql_fetch_array($result);

					// get data
					$v_list = $row[0];
					$e_list = $row[1];
					$nodes_num = $row[2];
					$edge_num = $row[3];
					$modularity = $row[4];
					$v_list_arr = explode(";", $v_list);
					/* foreach($v_list_arr as $v) {
						$tokens = explode("-", $v);
						$tid = $tokens[0];
						$sql = "SELECT node_level FROM vis_levels WHERE node_id = '$tid'";
						$result = mysql_query($sql);
						if (mysql_affected_rows() > 0) {
							$row = mysql_fetch_array($result);
							array_push($levels, $row[0]);
						}
						else {
							// give default if not found
							array_push($levels, 1);
						}
					} */
					
					$final_result = array('v_list' => $v_list, 'e_list' => $e_list, 'nodes_num' => $nodes_num, 'edge_num' => $edge_num, 'modularity' => $modularity);
					
					break;
				}
				$count++;
			}
			if (!$completed) {
				$final_result = array('error' => 'error');
			}
		}
		else {
			$final_result = array('error' => 'error');
		}
		
		mysql_close($con);
		
		echo json_encode($final_result);
	}
?>
