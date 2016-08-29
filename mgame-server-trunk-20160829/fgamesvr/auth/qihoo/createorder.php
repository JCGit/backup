<?php

$config = require_once(dirname(__FILE__) . "/../utils/config.php");
require(dirname(__FILE__) . "/../utils/order.php");
require(dirname(__FILE__) . "/../utils/util.php");


//创建订单基本信息
$order = new OrderFactory('_qihoo', $config);
if (!$order->ProcessParam($_REQUEST)){
	error_log(FormatLogStr('qihoo', 'createorder', ERROR_PARAM, json_decode($_REQUEST)));
	die('{"error":"参数错误^_^"}');
}

if (!$order->CheckUin($_REQUEST['uin'])){
	error_log(FormatLogStr('qihoo', 'createorder', ERROR_UIN, 'uin :' . $uin));
	die('{"error" : "not found for uin"}');
}

$order->SaveOrder();

die('{"orderId": "' . $order->GetOrderId() . '"}');
