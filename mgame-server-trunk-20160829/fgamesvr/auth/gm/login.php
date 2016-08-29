<?php

require 'response.php';
require 'config.php';
require 'util.php';

$user = $_REQUEST['user_name'];
$pass = $_REQUEST['user_pass'];


$sql = "SELECT u_name FROM user WHERE u_name = '$user' AND u_pass = '$pass'";

$mysqli = new mysqli($mysql['host'], $mysql['user'], $mysql['pass'], $mysql['db']);
if ($mysqli->connect_error){
    error_log("mysql conn error " . 'Connect Error (' . $mysqli->connect_errno . ') '
            . $mysqli->connect_error);
    Response::JsonRender(array('ret' => ERR_MYSQL, 'msg' => 'sorry,连接数据库失败!'));
}

if ($mysqli->query($sql)->num_rows > 0) {
    $_SESSION['name'] = $user;
    Response::JsonRender(array('ret' => ERR_0, 'msg' => '成功登录'));
}else{
    Response::JsonRender(array('ret' => ERR_NO_USER, 'msg' => '用户名或密码不正确'));
}
