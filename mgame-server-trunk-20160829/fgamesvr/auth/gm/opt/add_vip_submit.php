<?php

require_once(dirname(__FILE__)."/../title.php");
require_once(dirname(__FILE__)."/../util.php");
require_once(dirname(__FILE__)."/../config.php");

if(!$_GET["server_ip"])
{
    echo "没有输入服务器ip";
    return;
}
if(!$_GET["server_port"])
{
    echo "没有输入服务器端口";
    return;
}
if(!$_GET["uin"])
{
    echo "没有输入玩家uin";
    return;
}
if(!$_GET["level"])
{
    echo "没有输入vip等级";
    return;
}
if(!$_GET["day"])
{
    echo "没有输入天数";
    return;
}

// 0 测试 1 qq 2 微信
if(!isset($_GET["platform"]))
{
    echo "请选择平台!";
    return;
}

$platform = intval($_GET['platform']);
if (($platform != 0) and !sessionCheck()){
    echo "你没有登录，请先登录";
    return;
}return;
}

$server_ip = $_GET["server_ip"];
$server_port = $_GET["server_port"];
$uin = $_GET["uin"];
$level = $_GET["level"];
$day = $_GET["day"];

$url = '';
$param = '';

//区分测试和线上平台，测试不需要验证签名,且可以指定ip 和端口
if ($platform == 0) {
    $url = $gmToken;
    $platform = getPlatfrom($platform);
    $param = "gmtype=vip&uin=".$uin."&level=".$level."&day=".$day."&platform=".$platform;    
    $sign = getSign($param, $priKey);
    $param = $param . '&sign=' . $sign;
}else {
    $url = "http://".$server_ip.":".$server_port."/gm?";
    $param = "gmtype=vip&uin=".$uin."&level=".$level."&day=".$day;
}

$url = $url . $param;

error_log(FormatLogStr($_SESSION['name'],$url));

echo (https_get($url))
?>
