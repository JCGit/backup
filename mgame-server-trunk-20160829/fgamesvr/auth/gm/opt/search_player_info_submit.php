<?php

require_once(dirname(__FILE__)."/../title.php");
require_once(dirname(__FILE__)."/../util.php");

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
if(!$_GET["openid"])
{
	echo "没有输入玩家openid";
	return;
}


$server_ip = $_GET["server_ip"];
$server_port = $_GET["server_port"];
$openid = $_GET["openid"];
$platformid = $_GET["platformid"];

$sign_str = $openid.$platformid."61gamestks";
$sign = md5($sign_str);

$url = "http://".$server_ip.":".$server_port."/gm?";
$url = $url."gmtype=searchinfo&openid=".$openid."&platform=".$platformid."&sign=".$sign;

echo (https_get($url))
?>
