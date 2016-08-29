<?php
require 'response.php';
require 'config.php';
require 'util.php';


if (empty($_SESSION['name'])){
    Response::JsonRender(array('ret' => ERR_NO_LOGIN, 'msg' => '请先登录'));
}else{
    Response::JsonRender(array('ret' => ERR_0, 'msg' => '已登录'));
}