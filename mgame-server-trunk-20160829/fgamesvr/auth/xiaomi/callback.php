<?php

error_reporting(E_ALL^E_NOTICE);
$config = require_once(dirname(__FILE__) . "/../utils/config.php");
require_once(dirname(__FILE__) . "/../utils/redis.php");
require_once(dirname(__FILE__) . "/../utils/util.php");
require_once("config.php");


$param = $_REQUEST;

$orderId = $param['orderId'] . '_xiaomi';
$sign = $param['signature'];

if ($param['orderStatus'] !== 'TRADE_SUCCESS') {
	error_log(FormatLogStr('xiaomi', 'callback', ERROR_THIRD, ' return ' . $param['orderStatus']));
	die('{"errcode": 1506}');
}

//些参数不参与签名验证
unset($param['signature']);

//获取按字母排序后的key1=value1&key2=value2的字符串
$signStr = GetSortParam($param);
$mySign = HMacSha1($signStr, $appSecret);
if ($sign !== $mySign)
{
	error_log(FormatLogStr('xiaomi', 'callback', ERROR_SIGN, $signStr));
	die('{"errcode": 1525}');
}

$shopData = explode('#', $param['cpUserInfo']);

if (count($shopData) !== 2) {
	error_log(FormatLogStr('xiaomi', 'callback', ERROR_PARAM, 'private param count error'));
	die('{"errcode": 1000}');
}

$uin = intval($shopData[0]);
$shopid = intval($shopData[1]);

$data = array('Time' => time(),
			  'Uin' => intval($uin),
			  'Money' => intval($param['payFee']),
			  'ShopId' => intval($shopid),
			  'OrderId' => $orderId,
			  'Status' => 'fail');

$redis = new RedisHelper($config);
$oldOrder = $redis->HGet("recharge_order", $orderId);
if ($oldOrder && json_decode($oldOrder, 1)['Status'] === "success") {
	error_log(FormatLogStr('xiaomi', 'callback', ERROR_EXIST, $orderId));
	die('{"errcode": 200}');
}

$url = $config['center'] . $centerPath . '?uin=' . $uin . '&shopId=' . $shopid . '&orderId=' . $orderId;
$res = https_get($url);
if ($res === "success")
{
	//充值日志记录
	$log = GetLogger($config['logger']);
	$log->writeFile(FormatLogStr('xiaomi', 'callback', ERROR_0, $url));
	//改变订单状态
	$data['Status'] = 'success';
	$data['PayTime'] = strtotime($param['payTime']);
	$data['ExtOrderId'] = $param['cpOrderId'];
	$redis->HSet("recharge_order", $orderId, $data);
	die('{"errcode": 200}');
}else
{
	$log->writeFile(FormatLogStr('xiaomi', 'callback', ERROR_CENTER, $url));
	$redis->HSet("recharge_order", $orderId, $url);
	die('{"errcode": 1000}');
}



