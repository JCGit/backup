<?php
require 'redis.php';
$config = require("config.php");
require 'order.php';

$con = new RedisHelper($config);

var_dump($con);

$order = new OrderFactory('afafaf', $config);
var_dump($order);

// // $redis = new Redis();
// // $redis->connect("127.0.0.1","6379");
// $redis = getConn();
// if ($redis) {
// 	$data = $redis->hGet("recharge_order","21130619144434998192_yibin");
// 	var_dump($data);
// }else {
// 	echo "get conn failed";
// }



?>
