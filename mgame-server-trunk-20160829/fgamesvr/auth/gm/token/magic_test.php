<?php

//http://203.195.132.24/gm/magic.php

require "magic_cfg.php";
require(dirname(__FILE__) . "/../util.php");



$uri = array(
	'gmtype' => 'redis_opt',
	'opt' => 'hget_obj',
	'param1' => 'account',
    'param2' => '1000099',
    'param3' => 'test',
    'platform' => 'test',
	);

$str = getDataStr($uri);

$uri['sign'] = getSign($str, $priKey); //必需urlencode
$str = getDataStr($uri);

echo "request param is $str";
