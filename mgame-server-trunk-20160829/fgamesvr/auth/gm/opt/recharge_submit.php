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
if(!$_GET["platform_name"])
{
	echo "没有输入平台名字";
	return;
}
if(!$_GET["open_id"])
{
	echo "没有输入玩家open_id";
	return;
}

if(!$_GET["order_id"])
{
	echo "没有输入模拟订单号";
	return;
}

if(!$_GET["shop_id"])
{
	echo "没有输入商品id";
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
}

$server_ip = $_GET["server_ip"];
$server_port = $_GET["server_port"];
$platform_name = $_GET["platform_name"];
$open_id = $_GET["open_id"];
$order_id = $_GET["order_id"];
$shop_id = $_GET["shop_id"];

$url = '';
$param = '';

//区分测试和线上平台，测试不需要验证签名,且可以指定ip 和端口
if ($platform != 0) {
    $url = $gmToken;
    $platform = getPlatfrom($platform);
    $param = "&orderId=".$order_id."&uin=".$open_id."&shopId=".$shop_id."&platform=".$platform.'&path=/gm';    
    $sign = getSign($param, $priKey);
    $param = $param . '&sign=' . $sign;
}else{
    $url = "http://".$server_ip.":".$server_port."/".$platform_name."/callback?";
    $param = "orderId=".$order_id."&uin=".$open_id."&shopId=".$shop_id;
}

$url = $url . $param;
error_log(FormatLogStr($_SESSION['name'],$url));
echo (https_get($url))
?>
