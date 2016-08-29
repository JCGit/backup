<?php
error_reporting(E_ALL^E_NOTICE);
$config = require_once(dirname(__FILE__) . "/../utils/config.php");
require_once(dirname(__FILE__) . "/../utils/util.php");
require_once(dirname(__FILE__) . "/../utils/order.php");
require_once(dirname(__FILE__) . "/config.php");
require_once(dirname(__FILE__) . "/lib/utils.php");
//1.verity sign
//2.response to jinli
//3.update order
//4.notify center do shopping

//post get all in
$contents = $_REQUEST;

//error_log("param is " . print_r($contents, 1));

//签名验证不成功则返回错误给金立
if(!rsa_verify($contents)){
	error_log(FormatLogStr('jinli', 'callback', ERROR_SIGN, $content));
	die('error sign');
}

//获取自己创建的订单
$orderId = filter_input(INPUT_POST, 'out_order_no');

//error_log("orderId is " . $orderId);

//获取操作订单的实例
$order = new OrderFactory('_jinli', $config);
$order->LoadOrder('recharge_order', $orderId);
//检查订单是否已经成功，防止多次发送
if ($order->CheckOrder('recharge_order', $orderId)){
	error_log(FormatLogStr('jinli', 'callback', ERROR_EXIST, $content));
	die('success');
}

//通知游戏服务器center服务器进行商品充值
$data = $order->GetOrderData();
$url = $config['center'] . $centerPath . '?uin=' . $data['Uin'] . '&shopId=' . $data['ShopId'] . '&orderId=' . $data['OrderId'];
$res = https_get($url);
if ($res === "success")
{
	$log = GetLogger($config['logger']);
	$log->writeFile(FormatLogStr('jinli', 'callback', ERROR_0, $url . ' ExtOrderId : ' . $data['ExtOrderId']));

	//改变订单状态 
	$order->ChangeOrder(array('Status' => 'success', 'PayTime' => time()));
	$order->SaveOrder();
	die('success');
}else
{
	error_log(FormatLogStr('jinli', 'callback', ERROR_CENTER, $url));
	die('notify center failed');
}
