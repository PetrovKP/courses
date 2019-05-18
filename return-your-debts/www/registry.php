<?php
require_once 'config.php';

//session_start();
$error = '';

if (isset ($_POST['reg'])) {
    $Login = $_POST['Login'];
    $Name = $_POST['Name'];
    $Password = $_POST['password'];
    $RPassword = $_POST['rpassword'];

    $sql = "SELECT username FROM users WHERE username = '$Login'";
    $result = $mysqli->query($sql);
    $row = $result->fetch_array(MYSQLI_ASSOC);

    $count = mysqli_num_rows($result);

    $sql = "SELECT username FROM registration WHERE username = '$Login'";
    $result_reg = $mysqli->query($sql);
    $row_reg = $result_reg->fetch_array(MYSQLI_ASSOC);

    $count_reg = mysqli_num_rows($result_reg);

    try {
        if (!($count == 0 && $count_reg == 0))
            throw new Exception("Пользователь с таким логином уже существует!");
        elseif ($Password != $RPassword)
            throw new Exception("Пароли не совпадают!");
        elseif (strlen($Login) < 4)
            throw new Exception("Слишком короткий логин!");
        elseif (strlen($Name) < 3)
            throw new Exception("Слишком короткое имя!");
        elseif (strlen($Password) < 4)
            throw new Exception("Слишком короткий пароль!");
        $SPassword = md5($Password);
        $sql_insert = "INSERT INTO debts.registration (username, name, password)" .
            " VALUES ('$Login', '$Name', '$SPassword')";
        $result = $mysqli->query($sql_insert);
        header("location: index.php");
    }
    catch (Exception $e) {
        $error = $e->getMessage();
    }

}

?>

<html>

<head>
    <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
    <title>Регистрация</title>
    <link rel="stylesheet" type="text/css" href="css/style.css">
</head>

<body>

<div align = "center">
    <div class = login>

        <form action = "" method = "post">
            <label>Логин  :</label><input type = "text" name = "Login" class = "box"/><br /><br />
            <label>Имя пользователя  :</label><input type = "text" name = "Name" class = "box"/><br /><br />
            <label>Пароль  :</label><input type = "password" name = "password" class = "box" /><br/><br />
            <label>Повторный ввод  :</label><input type = "password" name = "rpassword" class = "box" /><br/><br />
            <input type = "submit" name = "reg" value = " Регистрация "/><br />
        </form>

        <div class="errorlogin"><?php echo $error; ?></div>
    </div>
</div>

<div class="logoutReg"> <a href = "logout.php"> Выйти </a> </div>

</body>