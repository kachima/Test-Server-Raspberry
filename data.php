<?php
header('Content-Type: application/json');

$conn = mysqli_connect("localhost","root","123456","temp");

$sqlQuery = "SELECT * FROM log";

$result = mysqli_query($conn,$sqlQuery);

$data = array();
foreach ($result as $row) {
	$data[] = $row;
}

mysqli_close($conn);

echo json_encode($data);
?>