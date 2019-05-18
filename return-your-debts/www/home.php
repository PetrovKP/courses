<?php
include('session.php');


// Узнать как зовут пользователя
$name_user = $mysqli->query("SELECT name FROM debts.users where username = '$user_check' ");
$row = mysqli_fetch_array($name_user, MYSQLI_ASSOC);
$name_user = $row['name'];
if (!isset($name_user)) {
    $name_user = 'Неизвестный';
}

// Добавление в таблицу нового долга
if($_SERVER["REQUEST_METHOD"] == "POST") {

    $debtor = $_POST["debtor"];
    $money = $_POST["money"];
    if ($_POST["select"] == "I")
        $money = - $money;
    $description = $_POST["comment"];
    $data = date("Y.n.j");

    $sql_insert = "INSERT INTO debts.debt (username, debtor, debt, description, data)".
                  " VALUES ('$user_check', '$debtor', $money, '$description', '$data')";
    $result = $mysqli->query ($sql_insert);

    $sql_insert = "INSERT INTO debts.debt (username, debtor, debt, description, data)".
        " VALUES ('$debtor', '$user_check' , -$money, '$description', '$data')";
    $result = $mysqli->query ($sql_insert);
}

$query = "SELECT * FROM debts.debt WHERE username = '$user_check' ";
$result = $mysqli->query($query);

$users=[];      // Массив: пользователь - сколько должен
$history = '';  // История долгов
while ($row = $result->fetch_array(MYSQLI_ASSOC)) {
    $history =$history . $row['debtor'] . ' | ' . (string)$row['debt'] .
        ' руб. | ' . $row['data'] . ' | '. $row['description'] . '<br/>';
    $users [$row['debtor']] += $row['debt'];
}

// Вывод всех должников
function printDebt($users){
    foreach ($users as $user => $sum) {
        if ($sum > 0) {
            printf("<b>%s</b> должен вам <b>%d</b> рублей<br/>", $user, $sum);
        } else
            printf("Вы должны <b>%s</b> <b>%d</b> рублей<br/>", $user, abs($sum));
    }
}
// Список возможных пользователей, кроме текущего
$query = "SELECT * FROM debts.users WHERE usergroup = 'user' 
          AND username <> '$user_check'";
$result = $mysqli->query($query);
$list_login = [];
while ($row = $result->fetch_array(MYSQLI_ASSOC)) {
    array_push($list_login, $row['username']);
}

?>

<html>

<head>
    <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
    <title>Стартовая страница </title>
    <link rel="stylesheet" type="text/css" href="css/style.css">
</head>

<body>

<h1>Добро пожаловать <?php echo $name_user; ?></h1>

<div id = leftcol>
    <p><?php printDebt($users)?></p>
    <br/>
    <p> История долгов </p>
    <div id="prokrutka"> <?php echo $history; ?> </div>
</div>

<script>
    var block = document.getElementById("prokrutka");
    block.scrollTop = 9999;
</script>

<div id="rightcol">
<form action="home.php" method="post">

    <br/><br/>
    <nobr> Выберите пользователя </nobr>

    <div class="styled-select">
        <select name="debtor" required>
            <?php foreach ($list_login as $login) { ?>
                <option value="<?php echo $login;?>"><?php echo $login;?></option>
            <?php } ?>

        </select>
    </div>

    <div class="styled-select slate">
    <select name="select" required>
            <option selected value="me"> мне должны </option>
            <option value="I"> я должен </option>
    </select>
    </div>

    <nobr> Сумма </nobr>
    <input name="money" pattern="\d+" required autocomplete="off">

    <nobr> Описание </nobr>
    <input name="comment" type="text" maxlength="15" autocomplete="off">

    <input type="submit" value="Отправить">
</form>
</div>

<div class="logout"> <a href = "logout.php"> Выйти </a> </div>

</body>

</html>