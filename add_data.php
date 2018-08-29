<?php
    // Connect to MySQL
    include("dbconnect.php");

    // Prepare the SQL statement
    $SQL = "INSERT INTO arduinotest.temperature (temperature,temperature_2,humidity,arduinoID) VALUES ('".$_GET["temperature"]."', '".$_GET["therperature"]."','".$_GET["humidity"]."', '".$_GET["ID"]."')";    

    // Execute SQL statement
    mysqli_query($con,$SQL);
?>
