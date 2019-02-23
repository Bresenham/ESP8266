<?php
    $servername = "localhost";
    $username = "scalar_sql";
    $password = "";
    $database = "scalar_temperature";

    // Create connection
    $conn = new mysqli($servername, $username, $password, $database);

    // Check connection
    if ($conn->connect_error) {
        die("Connection failed: " . $conn->connect_error);
    } 

    $sql = "";

    if($_POST["VALUE"]) {
        $sql = "INSERT INTO MEASUREMENTS (TIME, TEMP_VALUE) VALUES ('" . date("Y-m-d H:i:s") . "', " . $_POST["VALUE"] . ")";

        if($conn->query($sql) === TRUE) {
            echo "New record created successfully";
        } else {
            echo "Error: " . $sql . "<br>" . $conn->error;
        }

    } else if(isset($_GET)) {
        $sql = "SELECT TIME, TEMP_VALUE FROM MEASUREMENTS ORDER BY TIME ASC";

        /* Select queries return a resultset */
        if ($result = $conn->query($sql)) {

            $rows = array();
            while($r = mysqli_fetch_assoc($result)) {
                $rows[] = $r;
            }
            print json_encode($rows);

            /* free result set */
            $result->close();
        } else {
            echo "Error: " . $sql . "<br>" . $conn->error;
        }

    } else {
        die("NEITHER POST NOR GET REQUEST IS PRESENT");
    }

    $conn->close();
?>