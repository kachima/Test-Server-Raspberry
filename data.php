<?php
header('Content-Type: application/json');

$conn = mysqli_connect("localhost","root","123456","temp"); //change database and password

$sqlQuery = "SELECT * FROM temp_log"; //select table

$result = mysqli_query($conn,$sqlQuery);

$data = array();
foreach ($result as $row) {
	$data[] = $row;
}

mysqli_close($conn);

echo json_encode($data);
?>