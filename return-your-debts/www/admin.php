<?php

include('session.php');

if (isset ($_POST['runClear']))
{
    $name=$_POST["User"];
    if ($_POST["select"]=="Delete")
    {
        $sql = "DELETE FROM users WHERE username = '$name'";
        $result = $mysqli->query($sql);
        $sql = "DELETE FROM debt WHERE username = '$name' OR debtor = '$name'";
        $result = $mysqli->query($sql);

    }
    
    if ($_POST["select"]=="ClearHistory")
    {
        $sql = "DELETE FROM debt WHERE username = '$name' OR debtor = '$name'";
        $result = $mysqli->query($sql);
    }
}

if (isset ($_POST['registration']))
{
    $un_reg_name=$_POST["UnRegUser"];

    if ($_POST["select"]=="Add")
    {
        $sql = "SELECT * FROM registration WHERE username = '$un_reg_name' ";
        $result = $mysqli->query($sql);
        $row = $result->fetch_array(MYSQLI_ASSOC);

        $username = $row['username'];
        $name = $row['name'];
        $password = $row['password'];

        $sql_insert = "INSERT INTO debts.users (username, name, userpass, usergroup)".
            " VALUES ('$username','$name', '$password', 'user')";

        $result = $mysqli->query ($sql_insert);

        $sql = "DELETE FROM registration WHERE username = '$un_reg_name'";
        $result = $mysqli->query($sql);
    }
    
    if ($_POST["select"]=="DeleteUnReg")
    {
        $sql = "DELETE FROM registration WHERE username = '$un_reg_name'";
        $result = $mysqli->query($sql);
    }
}
$query = 'SELECT * FROM users';
$result = $mysqli->query($query);
$list_name = [];
while ($row = $result->fetch_array(MYSQLI_ASSOC))
{
    array_push($list_name, $row['username']);
}

$query = 'SELECT * FROM registration';
$result_reg = $mysqli->query($query);
$list_name_reg = [];
while ($row_reg = $result_reg->fetch_array(MYSQLI_ASSOC))
{
    array_push($list_name_reg, $row_reg['username']);
}
?>

<head>
    <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
    <title>Админ панель</title>
    <link rel="stylesheet" type="text/css" href="css/style.css">
</head>

<body>

<div id="rightcol">
<form action="admin.php" method="post">

    <br/><br/>
    <nobr> Выберите пользователя </nobr>

    <div class="styled-select">
        <select name="User" required>
            <?php foreach ($list_name as $name) { if ($name != $user_check) { ?>
                <option value="<?php echo $name;?>"><?php echo $name;?></option>
            <?php } } ?>

        </select>
    </div>

    <div class="styled-select slate">
    <select name="select" required>
            <option selected value="Delete"> Удалить </option>
            <option value="ClearHistory"> Очистить историю </option>
    </select>
    </div>

    <input type="submit" name = "runClear" value="Выполнить">
</form>
</div>
    
<div id="leftcol">
<form action="admin.php" method="post">

    <br/><br/>
    <nobr> Выберите пользователя </nobr>

    <div class="styled-select">
        <select name="UnRegUser" required>
            <?php foreach ($list_name_reg as $name) { if ($name != $user_check) { ?>
                <option value="<?php echo $name;?>"><?php echo $name;?></option>
            <?php } } ?>

        </select>
    </div>

    <div class="styled-select slate">
    <select name="select" required>
            <option selected value="Add"> Добавить </option>
            <option value="DeleteUnReg"> Удалить </option>
    </select>
    </div>

    <input type="submit" name = "registration" value="Выполнить">
</form>
</div>
    <div class="logout"> <a href = "logout.php"> Выйти </a> </div>
</body>