<?php
$MyUsername = "username";
$MyPassword = "password";
$MyHostname = "localhost";

$con=mysqli_connect($MyHostname,$MyUsername, $MyPassword);
// Check connection
if (mysqli_connect_errno())
  {
  echo "Failed to connect to MySQL: " . mysqli_connect_error();
  }
?>
