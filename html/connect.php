<?php

$server = "netlab.bmi.osumc.edu";
$user = "appAdmin";
$password = "GWCtMcS^@M";
$database = "vis_demo";
$con = mysql_connect($server, $user, $password);

//$server = "127.0.0.1";
//$user = "root";
//$password = "rkysue";
//$database = "query_jobs";
//$con = mysql_connect($server, $user, $password);
if (!$con) {
  die('Could not connect.');
}

mysql_select_db($database) or die('Could not find database.');

?>
