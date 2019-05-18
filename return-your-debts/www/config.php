<?php
// Подключение к базе данных

define('DB_HOST', '****');
define('DB_DATABASE','debts');
define('DB_USER', '****');
define('DB_PASSWORD', '****');

$mysqli = new mysqli(DB_HOST, DB_USER, DB_PASSWORD, DB_DATABASE);
$mysqli->query( "SET CHARSET utf8" );
?>