<?php

/*
此php脚本用于创建开发人员操作简版的gm网页的用户
用php cli实现添加
usage:
	dir/bin/php username pass

** 说明
    dir 是安装php的目录
	username 创建的用户名
	pass  新用户的密码
*/


require 'response.php';
require 'config.php';
require 'util.php';

if (sizeof($argv) < 3) {
   die("usage : php $argv[0] [user] [pass] \n"); 
}

$user = $argv[1];
$pass = $argv[2];


if (empty($user)){
    Response::JsonRender(array('ret' => ERR_USER_EMPTY, 'msg' => 'empty user!'));
}

if (empty($pass)){
    Response::JsonRender(array('ret' => ERR_PASS_EMPTY, 'msg' => 'empty pass!'));
}


$sql = "select u_name from user where u_name = '$user'";
$pass = md5($pass);


$mysqli = new mysqli($mysql['host'], $mysql['user'], $mysql['pass'], $mysql['db']);
if ($mysqli->connect_error){
    Response::JsonRender(array('ret' => ERR_MYSQL, 'msg' => 'sorry,connect to mysql failed!'));
}

if ($mysqli->query($sql)->num_rows > 0) {
    Response::JsonRender(array('ret' => ERR_REGISTER, 'msg' => 'sorry,the user is exist!'));
}

$sql = "insert into user(u_name, u_pass) values('$user','$pass')";

if(!$mysqli->query($sql)){
    Response::JsonRender(array('ret' => ERR_MYSQL, 'msg' => 'sorry,insert mysql failed!'));    
}
Response::JsonRender(array('ret' => ERR_0, 'msg' => 'congratulations !!'));





