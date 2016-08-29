<?php

error_reporting(E_ALL^E_NOTICE);
$config = require(dirname(__FILE__) . "/../utils/config.php");
require(dirname(__FILE__) . "/../utils/redis.php");
require(dirname(__FILE__) . "/../utils/util.php");

//有错误状态码一定要非200!!!

$orderId = trim($_REQUEST['exorderno']);
if (empty($orderId)) {
	error_log(FormatLogStr('lenovo', 'query', ERROR_PARAM, "exorderno : $orderId param is null"));
	http_response_code(400);
    die('{"error":3}');
} else {
    $redis = new  RedisHelper($config);
    $data = $redis->HGet("recharge_order",$orderId);
	if ($data) {
		$dtls = json_decode($data, true);
		if ($dtls["Status"] === "success") {
			return;
		}else{
			error_log(FormatLogStr('lenovo', 'query', ERROR_STATUS, "exorderno : $orderId status is failed"));
			http_response_code(400);
			die('{"error":"2"}');
		}
	}else{
		error_log(FormatLogStr('lenovo', 'query', ERROR_NEXIST, "order : $orderId is not exist"));
		http_response_code(400);
		die('{"error":1}');
	}
}
