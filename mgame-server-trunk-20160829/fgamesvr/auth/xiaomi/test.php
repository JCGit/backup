<?php

require_once(dirname(__FILE__) . "/../utils/util.php");




$param = array('appId' => '2882303761517412415',
               'cpOrderId' => '3a2d726e-4ece-4a9f-a7e5-f2d8b6c0fe28',
               'cpUserInfo' => '1000005#2',
               'orderId' => '21144913365962954104',
               'orderStatus' => 'TRADE_SUCCESS',
               'payFee' => 100,
               'payTime' => '2015-12-03 17:08:03',
               'productCode' => '01',
               'productCount' => 1,
               'productName' => '钻石',
               'uid' => 78996857,
               'signature' => '6f9a861511337e91e4f2b46e4ac180dfdbb56f16');

$sign = $param['signature'];
// unset($param['signature']);

$signStr = '';

foreach ($param as $key => $value) {
	if (!empty($signStr)){
		$signStr = $signStr . '&';
	}
	$signStr = $signStr . $key . '=' . $value;
}

$url = 'http://203.195.132.24/xiaomi/callback.php?' . $signStr;
echo "url is " . $url;
$res = https_get($url);

echo "res is " . $res;
