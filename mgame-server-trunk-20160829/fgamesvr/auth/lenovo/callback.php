<?php

/**
 * 功能：用于联想支付平台支付回调接口
 * 说明：当有订单通过第三方支付平台支付成功后，第三方平台支付成功后回调此接口，告诉cp的服务器此订单接收成功
 * @date		2015/11/16
 */
error_reporting(E_ALL^E_NOTICE);
require("lib/lenovo_cashier_rsa.inc.php");
require(dirname(__FILE__) . "/../utils/util.php");
require(dirname(__FILE__) . "/../utils/redis.php");
$config = require(dirname(__FILE__) . "/../utils/config.php");
require_once 'config.php';

/*
example:
http://203.195.132.24/lenovo/callback.php?transdata={"transtype":0,"result":0,"transtime":"2015-12-23 13:33:53","count":1,"paytype":5,"money":600,"waresid":35921,"appid":"1510300252699.app.ln","exorderno":"151223133311175","feetype":0,"transid":"2151223133353379759137035","cpprivate":"10001808"}&sign=JmHXnEBCGsHsgjHp7PsDGUrgojoORVKfRTWhghwEEVj1jJwQnv2RWnO1BiazOLEFFePpNcLOs8vdQxCjW8PKnylLPnj7tnWWINX9cj1ZRTEjKHC0mp0nWDQZ48k0MtfRZEu0pVVKXveYFOQ1ry//LbGxOtdNgrXLjzhm/bMKzXg=
*/


$transdata = trim($_REQUEST['transdata']);
$sign =  trim($_REQUEST['sign']);

if (empty($transdata) ||
	empty($sign)) {
	error_log(FormatLogStr('lenovo', 'callback', ERROR_PARAM, 'sign,transdata param is NULL '));
	die('FAILTURE');
}

$result = verify($transdata, $appKey, $sign);
if ($result) {
	$param = json_decode($transdata,true);
	$shopid = @$shopMap[$param['waresid']];
	if (!$shopid) {
        error_log(FormatLogStr('lenovo', 'callback', ERROR_PARAM, 'get shopid faile for waresid ' . $param['waresid']));
        die('FAILTURE');
	}

	$orderId = $param["exorderno"];
	//updat redis
	$redis = new  RedisHelper($config);
	$data = $redis->HGet("recharge_order", $orderId);
	if ($data === false) {
		error_log(FormatLogStr('lenovo', 'callback', ERROR_NEXIST, "order : $orderId is not exist"));
		die('FAILTURE');
	}

	$cache = json_decode($data, true);
	$cache["ExtOrderId"] = $param["transid"];
	$cache['PayTime'] = strtotime($param["transtime"]);
	$cache['Uin'] = intval($param["cpprivate"]);
	

	$url = $config['center'] . $centerPath . '?uin=' . $param["cpprivate"] . '&orderId=' . $orderId . "&shopId=" . $shopid;
	$res = https_get($url);
	if ($res === 'callback success'){
		error_log(FormatLogStr('lenovo', 'callback', ERROR_0, $transdata . "&sign=$sign"));
		$log = GetLogger($config['logger']);
	    $log->writeFile(FormatLogStr('lenovo', 'callback', ERROR_0, $url . ' ExtOrderId : ' . $cache['ExtOrderId']));
		$cache["Status"] = "success";
		$redis->HSet("recharge_order", $orderId, $cache);
		die('SUCCESS');
	}else{
		$redis->HSet("recharge_order", $orderId, $cache);
		error_log(FormatLogStr('lenovo', 'callback', ERROR_CENTER, $url));
		die('FAILTURE');
	}
} else {
    error_log(FormatLogStr('lenovo', 'callback', ERROR_SIGN, $transdata . "&sign=$sign"));
    die('FAILTURE');
}


?>