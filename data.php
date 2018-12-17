<?php
header('Content-Type: application/json');

$conn = mysqli_connect("localhost","root","123456","homesystem"); //change database and password

$sqlQuery = "SELECT time(date_time),temperature FROM (SELECT * FROM docnhietdo ORDER BY STT DESC LIMIT 50) sub ORDER BY STT ASC"; //select table

$result = mysqli_query($conn,$sqlQuery);

$data = array();
foreach ($result as $row) {
	$data[] = $row;
}

mysqli_close($conn);

echo json_encode($data);
?>