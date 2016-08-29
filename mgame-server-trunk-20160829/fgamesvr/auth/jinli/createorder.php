<?php

error_reporting(E_ALL^E_NOTICE);
$config = require_once(dirname(__FILE__) . "/../utils/config.php");
require_once(dirname(__FILE__) . "/../utils/order.php");
require_once(dirname(__FILE__) . "/../utils/util.php");
require_once(dirname(__FILE__) . "/lib/utils.php");
require_once("config.php");

//example : http://203.195.132.24/jinli/createorder.php?money=10&uin=1000001&shopid=10

//1.本地创建订单
//2.请求金立创建订单接口
//3.返回客户端

//生成内部订单，创建金立订单

//创建订单基本信息
$order = new OrderFactory('_jinli', $config);
if (!$order->ProcessParam($_REQUEST)){
	error_log(FormatLogStr('jinli', 'createorder', ERROR_PARAM, json_decode($_REQUEST)));
	die('{"error":"参数错误^_^"}');
}

if (!$order->CheckUin($_REQUEST['uin'])){
	error_log(FormatLogStr('jinli', 'createorder', ERROR_UIN, 'uin :' . $uin));
	die('{"error" : "not found for uin"}');
}


$return['paynum'] = $order->GetOrderId();

$dst_url = "https://pay.gionee.com/order/create";
$post_arr['api_key'] = $appKey;//app key
$post_arr['subject'] = empty($_REQUEST['name'])?'订单':$_REQUEST['name'];
$post_arr['out_order_no'] = $return['paynum'];
$post_arr['deliver_type'] = '1';
$post_arr['deal_price'] = $_REQUEST['money'];
$post_arr['total_fee'] = $_REQUEST['money'];
$post_arr['submit_time'] = date('YmdHis');
// $post_arr['notify_url'] = "http://{$_SERVER['HTTP_HOST']}/jinli/callback.php"; //此地址在第三方后台配置，测试时可以自己指定
$post_arr['sign'] = rsa_sign($post_arr);
$post_arr['player_id'] = $_REQUEST['uin'];

$json = json_encode($post_arr);

$return_json = https_post($dst_url,$json);
$return_arr = json_decode($return_json,1);

//订单创建成功的状态码判断
if ($return_arr['status'] !== '200010000') {
	//创建失败处理
	error_log(FormatLogStr('jinli', 'createorder', ERROR_THIRD, ' return ' . $return_arr['status']));
	die('{"error": "' . $return_arr['status'] . '"}');
}

//记录第三方订单号
$order->AddExtraData(array('ExtOrderId'=>$return_arr['order_no']));

$order->SaveOrder();

$return['submit_time'] = $return_arr['submit_time'];
die(json_encode($return));
