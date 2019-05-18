<?php

require_once 'config.php';

session_start();

if (isset ($_POST['login'])) {
    $myusername = $mysqli->real_escape_string($_POST["username"]);
    $userpass = $mysqli->real_escape_string($_POST["password"]);
    $userpass=md5($userpass);
    $sql = "SELECT username FROM users WHERE username = '$myusername' AND userpass = '$userpass'";
    $result = $mysqli->query($sql);
    $row = $result->fetch_array(MYSQLI_ASSOC);

    $count = mysqli_num_rows($result);

    if ($count == 1) {

        $sql = "SELECT username FROM users WHERE username = '$myusername' 
                AND userpass = '$userpass' AND usergroup = 'admin'";
        $result = $mysqli->query($sql);
        $row = $result->fetch_array(MYSQLI_ASSOC);

        $count = mysqli_num_rows($result);
        
        if ($count == 0) {
            $_SESSION['login_user'] = $myusername;
            header("location: home.php");
        }
        else  {
            $_SESSION['login_user'] = $myusername;
            header("location: admin.php");
        }

    } else {
        $error = "Не правильный логин или пароль!";
    }
}
elseif (isset ($_POST['reg'])) {
    header("location: registry.php");
}

$mysqli ->close();
?>

<html>

<head>
    <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
    <title>Войти</title>
    <link rel="stylesheet" type="text/css" href="css/style.css">
</head>

<body>

<div align = "center">
    <div class = login>

        <form action = "" method = "post">
            <label>Пользователь  :</label><input type = "text" name = "username" class = "box"/><br /><br />
            <label>Пароль  :</label><input type = "password" name = "password" class = "box" /><br/><br />
            <input type = "submit" name ="login" value = " Войти "/><br />
            <input type = "submit" name="reg" value=" Регистрация"/><br/>
        </form>

        <div class="errorlogin"><?php echo $error; ?></div>
    </div>
</div>

</body>