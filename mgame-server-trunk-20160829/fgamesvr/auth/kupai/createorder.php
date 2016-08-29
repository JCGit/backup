<?php
error_reporting(E_ALL^E_NOTICE^E_WARNING);
$config = require_once(dirname(__FILE__) . "/../utils/config.php");
require(dirname(__FILE__) . "/../utils/util.php");
require(dirname(__FILE__) . "/lib/utils.php");
require(dirname(__FILE__) . "/../utils/redis.php");
include("config.php");

//example
//http://203.195.132.24/kupai/createorder.php?money=1&uin=1000001&shopid=1


$shopid = $_REQUEST['shopid'];
$uin = $_REQUEST['uin'];
$money = $_REQUEST['money'];

if (empty($shopid) ||
	empty($uin) ||
	empty($money)){
	error_log(FormatLogStr('kupai', 'createorder', ERROR_PARAM, json_decode($_REQUEST)));
	die('{"error" : "error param"}');
}

//商品号不存在则报错
if (!$goodsMap[$shopid]) {
	error_log(FormatLogStr('kupai', 'createorder', ERROR_GOODS, ' shopid : ' . $shopid));
	die('{"error" : "invalidate shopid"}');
}


//为了防止别人请求此地址,验证uin是否存在
$redis = new RedisHelper($config);
if (!$redis->Exist('account', $uin)){
	error_log(FormatLogStr('kupai', 'createorder', ERROR_UIN, 'uin :' . $uin));
	die('{"error" : "not found for uin"}');
}


$orderId = GenOrderId('_kupai');
//下单接口
$orderReq["appid"] = $appId;
$orderReq["waresid"] = intval($shopid);
$orderReq["cporderid"] = $orderId; 
$orderReq["price"] = (float)$money;   //单位：元
$orderReq["currency"] = "RMB";
$orderReq["appuserid"] = $uin; //uin
// $orderReq["cpprivateinfo"] = $uin . '#' . $shopid;

//组装请求报文
$reqData = composeReq($orderReq, $privateKey);

//发送到爱贝服务后台
$respData = request_by_curl($orderUrl, $reqData, "Mozilla/5.0 (compatible; MSIE 5.01; Windows NT 5.0)");

//返回报文解析
if(!parseResp($respData, $publicKey, $respJson)) {
	error_log(FormatLogStr('kupai', 'createorder', ERROR_THIRD, $respData));
    die('{"error" : "parse resp failed"}');
}

$cache['Money'] = intval($money);
$cache['Time'] = time();
$cache['Status'] = 'fail';
$cache['ShopId'] = $goodsMap[$shopid];
$cache['OrderId'] = $orderId;
$cache['Uin'] = $uin;
$redis->HSet('recharge_order', $orderId, $cache);
die(json_encode($respJson));

