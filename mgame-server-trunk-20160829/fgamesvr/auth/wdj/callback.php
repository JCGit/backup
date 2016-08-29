<?php
error_reporting(E_ALL^E_NOTICE);
$config = require(dirname(__FILE__) . "/../utils/config.php"); //尽量不要用requre_once
require(dirname(__FILE__) . "/../utils/util.php");
require(dirname(__FILE__) . "/../utils/redis.php");
require(dirname(__FILE__) . "/../utils/rsa.php");
include('config.php');

$content=$_REQUEST['content'];
$sign=$_REQUEST['sign'];

$log = GetLogger($config['logger']);

//创建用于Rsa加解密实例
$rsa = new Rsa;
//设置公钥，然后用公钥解密用私钥加密的数字签名，然后再验证签名是否相同
$rsa->setupPubKey($publicKey);
$result=$rsa->verify($content,$sign);
if($result) {
	//拿到回调参数
	$data = json_decode($content, 1);
	$cache = array();

	//要检测uin存在
	$redis = new RedisHelper($config);
	// $uin = $redis->CheckUin(GetUinKey('wandoujia', $data['buyerId']));
	// if (!$uin) //不存在则直接返回
	// {
	// 	$log->writeFile(FormatLogStr('wandoujia', 'callback', ERROR_UIN, $content));
	// 	die("fail");
	// }

	//1.检查订单是否处理成功
	//2.新订单通知center服务发商品
	//3.成功则入库
	

	$myOrder = explode('#', $data['out_trade_no']);
	if (count($myOrder) !== 3) {
		error_log(FormatLogStr('wandoujia', 'callback', ERROR_PARAM, 'my order param count error should be [orderid#uin#shopid]'));
		die('result=FAIL&resultMsg=附加参数个数不正确');
	}

	$orderId = trim($myOrder[0]);
	if ($redis->CheckOrder('recharge_order', $orderId))
	{
		error_log(FormatLogStr('wandoujia', 'callback', ERROR_EXIST, $content));
		die("success");
	}

	$cache['Money'] = intval($data['money']) / 100;
	$cache['Status'] = 'success';
	$cache['ExtOrderId'] = $data['orderId'];
	$cache['OrderId'] = $orderId;
	$cache['PayTime'] = intval($data['timeStamp']);
	$cache['Time'] = time();
	$cache['Uin'] = intval(trim($myOrder[1]));
	$cache['ShopId'] = intval(trim($myOrder[2]));
	
	$url = $config['center'] . $centerPath . '?uin=' . $cache['Uin'] . '&shopId=' . $cache['ShopId'] . '&orderId=' . $orderId;
	$res = https_get($url);
	if ($res === "success") {
        $log->writeFile(FormatLogStr('wandoujia', 'callback', ERROR_0, $url . ' ExtOrderId : ' . $cache['ExtOrderId']));
		$redis->HSet('recharge_order', $orderId, $cache);
		die("success");
	}else{
		$cache['Status'] = 'fail';
		$redis->HSet('recharge_order', $orderId, $cache);
		error_log(FormatLogStr('wandoujia', 'callback', ERROR_CENTER, $content));
        die("fail");
	}
} else {
	error_log(FormatLogStr('wandoujia', 'callback', ERROR_SIGN, $url));
    die("fail");
}
?>

