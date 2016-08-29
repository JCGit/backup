<?php

$config = require(dirname(__FILE__) . "/../utils/config.php");
require(dirname(__FILE__) . "/../utils/util.php");
require(dirname(__FILE__) . "/../utils/rsa.php");
require(dirname(__FILE__) . "/../utils/redis.php");
require(dirname(__FILE__) . "/config.php");


$param = $_REQUEST;
//得到第三名的签名，等待验证
$sign = $param['sign'];
//sign参数不需要参与签名验证
unset($param['sign']);

//顺序一定要如此，要不然签名不成功
function geturlparam($contents)
{
	return "notifyId={$contents['notifyId']}&partnerOrder={$contents['partnerOrder']}&productName={$contents['productName']}&productDesc={$contents['productDesc']}&price={$contents['price']}&count={$contents['count']}&attach={$contents['attach']}";
}

//获取url参数
$toSign = geturlparam($param);

$rsa = new Rsa;
$rsa->setupPubKey($publicKey);
if($rsa->verify($toSign, $sign))
{
	//获取前台附加参数uin#shopid
	$shopData = explode('#', $param['attach']);
	if (count($shopData) !== 2) {
		error_log(FormatLogStr('oppo', 'callback', ERROR_PARAM, 'param count error'));
		die('result=FAIL&resultMsg=附加参数个数不正确');
	}

	//1.检查订单是否已经处理成功
	//2.新订单通知center服务发商品
	//3.成功则入库
	$redis = new RedisHelper($config);
	if ($redis->CheckOrder('recharge_order', $param['partnerOrder']))
	{
		error_log(FormatLogStr('oppo', 'callback', ERROR_EXIST, $toSign));
		die('result=OK&resultMsg=订单已经存在');
	}


	$cache['Money'] = intval($param['price']) / 100;
	$cache['Status'] = 'success';
	$cache['ExtOrderId'] = $param['notifyId'];
	$cache['OrderId'] = $param['partnerOrder'];
	$cache['PayTime'] = time();
	$cache['Time'] = time();
	$cache['Uin'] = intval($shopData[0]);
	$cache['ShopId'] = intval($shopData[1]);

	$url = $config['center'] . $centerPath . '?uin=' . $shopData[0] . '&shopId=' . $shopData[1] . '&orderId=' . $cache['OrderId'];
	$res = https_get($url);
	if ($res === "success") {
		$log = GetLogger($config['logger']);
	    $log->writeFile(FormatLogStr('oppo', 'callback', ERROR_0, $url . ' ExtOrderId : ' . $cache['ExtOrderId']));
		$redis->HSet('recharge_order', $cache['OrderId'], $cache);
		die('result=OK&resultMsg=订单处理成功');
	}else{
		$cache['Status'] = 'fail';
		$redis->HSet('recharge_order', $cache['OrderId'], $cache);
		error_log(FormatLogStr('oppo', 'callback', ERROR_CENTER, $url));
        die('result=FAIL&resultMsg=下发商品失败');
	}

}else
{
	error_log(FormatLogStr('oppo', 'callback', ERROR_SIGN, $toSign . "&sign=$sign"));
	die('result=FAIL&resultMsg=验证签名失败');
}

