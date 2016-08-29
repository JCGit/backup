<?php

require_once(dirname(__FILE__) . "/MemorikiSDK.php");
require_once(dirname(__FILE__) . "/config.php");
require_once(dirname(__FILE__) . "/../utils/order_factory.php");
require_once(dirname(__FILE__) . "/../utils/util.php");

function send2center($cent_svr_config, $post_arr, $platform_name)
{
	$args_str = GetSortParam($post_arr);
	return https_get($cent_svr_config["host"] . ":" . $cent_svr_config["port"] . "/" . $platform_name . "/callback?" . $args_str);
}

function wError_log($error, $str)
{
	global $platform_name;
	error_log(FormatLogStr($platform_name, 'callback', $error ,$str));
}

function logger($error, $str)
{
	global $log_path;
	global $platform_name;
	GetLogger($log_path)->writeFile(FormatLogStr($platform_name, 'callback', $error, $str));
}

function pase_data($args, $secret)
{
	if(sizeof($args) <= 0)
	{
		return null;
	}
	if(!array_key_exists("payment_sig", $args))
		return null;
	$pay_res = MemorikiSDK::decodeSignedRequest($args["payment_sig"], $secret);

	if(!$pay_res["error"])
	{
		return $pay_res["payload"];
	}
	return null;
}

$req_data = pase_data($_GET, $secret);
if(!$req_data)
{
	wError_log(ERROR_PARAM, "req_data is null");
	return;
}
else
{
	//订单号
	$transactionId = $req_data["transactionId"];
	//opendid
	$snid = $req_data["snid"];
	//商品id
	$productId = $req_data["productId"];
    
    $pay_amount = $req_data["paymentAmount"];
    $pay_currency = $req_data["currency"];
}

$order_factory = new OrderFactory();
if(!$order_factory->init($redis_config, $platform_name))
{
	wError_log(100, "redis-server is connect faild");
	return;
}

if(!$order_factory->check_order_exist($transactionId))
{
	$order_factory->insert_order($transactionId);
	if(!$order_factory->check_order_exist($transactionId))
	{
		wError_log(101, "insert transactionId:" . $transactionId . " to redis failed");
		$order_factory->close();
		return;
	}
}

if($order_factory->get_order_opt($transactionId, OPT_STATE) != 0)
{
	wError_log(ERROR_EXIST, "transactionId : ". $transactionId . " is exist and success");
	//sdk 需要回执请求告诉处理成功
	MemorikiSDK::setSandBox($sendBox); // 请于游戏上线前把沙箱关掉！
	MemorikiSDK::notifyPaymentServer($app_id, $transactionId, $snid, $secret);
	$order_factory->close();
	return;
}

$order_factory->set_order_opt($transactionId, OPT_STATE, 1);

if(array_key_exists($productId, $shop_mapping))
{
	$shopId = $shop_mapping[$productId];
	$post_arr = array(
		"orderId" => $transactionId,
		"openId" => $platform_name."_".$snid,
		"shopId" => $shopId,
        "pay_amount" => $pay_amount,
        "pay_currency" => $pay_currency);
	$result = send2center($cent_svr_config, $post_arr, $platform_name);
	if($result == "success")
	{
		logger(ERROR_0, json_encode($post_arr));
		//sdk 需要回执请求告诉处理成功
		MemorikiSDK::setSandBox($sendBox); // 请于游戏上线前把沙箱关掉！
		MemorikiSDK::notifyPaymentServer($app_id, $transactionId, $snid, $secret);
		$order_factory->set_order_opt($transactionId, OPT_STATE, 1);
		$order_factory->set_order_opt($transactionId, OPT_PAY_TIME, time());
		$order_factory->set_order_opt($transactionId, OPT_SHOP_ID, $shopId);
        $order_factory->set_order_opt($transactionId, "pay_amount", $pay_amount);
        $order_factory->set_order_opt($transactionId, "pay_currency", $pay_currency);
	}
	else
	{
		$order_factory->set_order_opt($transactionId, OPT_STATE, 0);
		wError_log(ERROR_CENTER, "transactionId : ".$transactionId . " center resp is faild");
	}
}
else
{
	$order_factory->set_order_opt($transactionId, OPT_STATE, 0);
	wError_log(ERROR_GOODS, "productId : " . $productId . " is not exist");
}

$order_factory->close();

?>
