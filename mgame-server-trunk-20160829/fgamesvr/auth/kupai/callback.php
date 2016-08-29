<?php
error_reporting(E_ALL^E_NOTICE^E_WARNING);
$config = require(dirname(__FILE__) . "/../utils/config.php");
require(dirname(__FILE__) . "/../utils/util.php");
require(dirname(__FILE__) . "/lib/utils.php");
require(dirname(__FILE__) . "/../utils/redis.php");
include("config.php");

$param = $_REQUEST;

//得到第三名的签名，等待验证
$sign = $param['sign'];

//获取等待签名验证的数据
$signData = $param['transdata'];

if ($signData === NULL ||
	$sign === NULL)
{
	error_log(FormatLogStr('kupai', 'callback', ERROR_PARAM, 'sign or transdata param is NULL '));
	die('FAILURE');	
}

// $toSign = 'transdata=' . urlencode($signData) . '&sign=' . urlencode($sign) . '&signtype=RSA';

if(verify($signData, $sign, formatPubKey($publicKey)))
{
	$data = json_decode($signData, 1);
	
	//1.检查订单是否处理成功
	//2.新订单通知center服务发商品
	//3.成功则入库
	$redis = new RedisHelper($config);
	$cache = $redis->HGet('recharge_order', $data['cporderid']);

	if (!$cache) {
		error_log(FormatLogStr('kupai', 'callback', ERROR_NEXIST, $signData));
		die("FAILURE");
	}

	$cache = json_decode($cache, true);
	if ($cache['Status'] === 'success')
	{
		error_log(FormatLogStr('kupai', 'callback', ERROR_EXIST, $signData));
		die("SUCCESS");
	}

	$cache['Status'] = $data['result'] ? "fail" : "success";
	$cache['ExtOrderId'] = $data['transid'];
	$cache['PayTime'] = strtotime($data['transtime']);


	if ($cache['Status'] === 'success')//支付成功的订单
	{
		$url = $config['center'] . $centerPath . '?uin=' . $cache['Uin'] . '&shopId=' . $cache['ShopId'] . '&orderId=' . $cache['OrderId'];
		$res = https_get($url);
		if ($res === "success") {
			$log = GetLogger($config['logger']);
			$log->writeFile(FormatLogStr('kupai', 'callback', ERROR_0, $url . ' ExtOrderId : ' . $cache['ExtOrderId']));
			$redis->HSet('recharge_order', $cache['OrderId'], $cache);
			die("SUCCESS");
		}else{
			$cache['Status'] = 'fail';
			$redis->HSet('recharge_order', $cache['OrderId'], $cache);
			error_log(FormatLogStr('kupai', 'callback', ERROR_CENTER, $url));
	        die("FAILURE");
		}
	}else{
		$redis->HSet('recharge_order', $cache['OrderId'], $cache);
		error_log(FormatLogStr('kupai', 'callback', ERROR_PAY, $signData));
		die("SUCCESS");
	}
}
else
{
	error_log(FormatLogStr('kupai', 'callback', ERROR_SIGN, $signData . "&sign=$sign"));
	die('FAILURE');	
}



