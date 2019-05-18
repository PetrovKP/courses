<?php
include('config.php');

session_start();

if(!isset($_SESSION['login_user'])){
    header("location: index.php");
}

$user_check = $_SESSION['login_user'];
?>