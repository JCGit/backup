<?php

error_reporting(E_ALL^E_NOTICE);
require_once(dirname(__FILE__) . "/../utils/redis.php");
require_once(dirname(__FILE__) . "/../utils/util.php");
$config = require_once(dirname(__FILE__) . "/../utils/config.php");
require_once("config.php");


//兼容get,post 牺牲速度
$appid = $_REQUEST['appid'];
$lpsust = $_REQUEST['lpsust'];
$waresid = $_REQUEST['waresid'];
$money = $_REQUEST['money'];

//判断参数的合法性
if (empty($appid) ||
	empty($lpsust) ||
	empty($waresid) ||
	empty($money)) 
{
    error_log(FormatLogStr('lenovo', 'createorder', ERROR_PARAM, 'get request param is NULL '));
    die('{"error":"3"}');
}

/*

http://203.195.132.24/lenovo/createorder.php?appid=1510300252699.app.ln&lpsust=ZAgAAAAAAAGE9MTAwNjQzMzA1MTkmYj0yJmM9NCZkPTE1ODI4JmU9RDAxRjI4QjI2MTEyMTkyQTY4Mjg0RTQyRUIxQzMxQjQxJmg9MTQ1MDQzMjkwMjkxOCZpPTQzMjAwJmo9MCZvPTg2NjA0ODAyOTEzOTMxMCZwPWltZWkmcT0wJnVzZXJuYW1lPTEzNDI0MjA4NjE5Jmx0PTEmaWw9Y25YCqrBlZ12artHm58vhu5u&waresid=24923&money=10

fail:
<Error>
<Code>USS-0540</Code>
<Timestamp>2015-12-18T18:07:23+08:00</Timestamp>
</Error>

success:
<IdentityInfo>
<AccountID>10064330519</AccountID>
<Username>13424208619</Username>
<DeviceID>866048029139310</DeviceID>
<verified>1</verified>
</IdentityInfo>

*/

$url = $authUrl . 'lpsust=' . $lpsust . '&realm=' . $appid;
$res = https_get($url);
if (strchr($res,'<Error>')) {
	error_log(FormatLogStr('lenovo', 'createorder', ERROR_THIRD, 'auth failed ' . str_replace("\n","", $res)));
	die('{"error":"3"}');
}

//todo 验证是否有权限
$shopid = @$shopMap[$waresid];
if (!$shopid) {
	error_log(FormatLogStr('lenovo', 'createorder', ERROR_PARAM, 'get shopid faile for waresid ' . $waresid));
	die('{"error":"3"}');
}

$orderId = GenOrderId('_lenovo');
$data = array('waresid' => $waresid, 
			  'Money' => $money,
			  'Time' => time(),
			  'Status' => "fail",
			  'ShopId' => $shopid,
			  'OrderId' => $orderId,
		);

$redis = new  RedisHelper($config);
$redis->HSet("recharge_order", $orderId, $data);//保证redis是连接的
error_log(FormatLogStr('lenovo', 'createorder', ERROR_0,  $orderId));
die('{"orderId": "' . $orderId . '"}');
?>