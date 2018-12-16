<?php
header('Content-Type: application/json');

$conn = mysqli_connect("localhost","root","123456","temp"); //change database and password

$sqlQuery = "SELECT * FROM (SELECT * FROM temp_log ORDER BY STT DESC LIMIT 50) sub ORDER BY STT ASC"; //select table

$result = mysqli_query($conn,$sqlQuery);

$data = array();
foreach ($result as $row) {
	$data[] = $row;
}

mysqli_close($conn);

echo json_encode($data);
?>