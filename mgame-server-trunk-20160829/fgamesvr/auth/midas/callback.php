<?php


require_once __DIR__ . '/lib/SnsSigCheck.php';
require(dirname(__FILE__) . "/config.php");
$config = require(dirname(__FILE__) . "/../utils/config.php");
require(dirname(__FILE__) . "/../utils/util.php");
require(dirname(__FILE__) . "/../utils/redis.php");

ini_set('default_socket_timeout', -1);  // socket 连接不超时


function getUrl($uri) {
    return strstr($uri, '?', true);
}

// 得到请求方式
$method = $_SERVER['REQUEST_METHOD'];
$url_path = getUrl($_SERVER["REQUEST_URI"]);
$logic = GetLogger($config['logiclog']);
$log = GetLogger($config['logger']);

$param = array();
if ($method == "GET") {
    $param = $_GET;
}elseif ($method == "POST") {
    $param = $_POST;
}else {
    $logic->writeFile(FormatLogStr('midas', 'callback', ERROR_PARAM,  'invalidate request method : ' . $method));
    return;
}

$log->writeFile(FormatLogStr('midas', 'callback', ERROR_BEGIN,  json_encode($param)));

$appkey = $appkey . '&';
if (SnsSigCheck::verifySig($method, $url_path, $param, $appkey, $param['sig'])) {
    //获取前台附加参数uin*shopid*qq  uin*shopid*weixin
    $shopData = explode('*', $param['appmeta']);
    if (count($shopData) < 3) {
        $logic->writeFile(FormatLogStr('midas', 'callback', ERROR_PARAM, json_encode($param)));
        die('result=FAIL&resultMsg=附加参数个数不正确');
    }

    $center = $config['center'];
    $platform = $shopData[2];
	
	$version = 0;
	$goal_version = $config['goal_version'];
	$release_flag = $config['release_flag'];
	if (count($shopData) > 3)
	{
		$version = intval($shopData[3]);
        $logic->writeFile(FormatLogStr('midas', 'callback --- version: ', $version));
	}
	
    if ($platform == "qq" || $platform == "QQ"){
		if ($release_flag == true && $version == $goal_version) {
			$center = $config['testcenter'];
			$platform = 'qq';
		}else{
			$center = $config['qqcenter'];
			$platform = 'qq';
		}
    }elseif ($platform == "weixin" || $platform == "WEIXIN") {
		if ($release_flag == true && $version == $goal_version) {
			$center = $config['testcenter'];
			$platform = 'weixin';
		}else{
			$center = $config['wxcenter'];
			$platform = 'weixin';
		}
    }elseif ($platform == "lenovo-qq"){
        $center = $config['lenovocenter'];
        $platform = 'lenovo-qq';
    }elseif ($platform == "lenovo-weixin"){
        $center = $config['lenovocenter'];
        $platform = 'lenovo-weixin';
    }else{
        $logic->writeFile(FormatLogStr('midas', 'callback', ERROR_PARAM, json_encode($param)));
        die('{"ret":4,"msg": "透传参数错误"}');
    }

    $redis = new RedisHelper($config);
    if ($redis->CheckOrder('recharge_order', $_REQUEST['billno']))
    {
        $logic->writeFile(FormatLogStr('midas', 'callback', ERROR_EXIST, json_encode($param)));
        die('{"ret":4,"msg": "订单已经存在"}');
    }

    $cache['Money'] = intval($param['amt']);
    $cache['Status'] = 'success';
    $cache['ExtOrderId'] = $param['billno'];
    $cache['PayTime'] = time();
    $cache['Time'] = time();
    $cache['Uin'] = intval($shopData[0]);
    $cache['ShopId'] = intval($shopData[1]);

    $order_ts = isset($param['ts']) ? $param['ts'] : 0;
    $order_openid = isset($param['openid']) ? $param['openid'] : "";
    $order_arr = array
    (
       "uin"      => $cache['Uin'],
       "time"     => $cache['Time'],
       "ts"       => $order_ts,
       "openid"   => $order_openid,
       "money"    => $cache['Money'],
       "order_id" => $cache['ExtOrderId'],
       "shop_id"  => $cache['ShopId'],
       "pf"       => $platform,
       "type"     => $isTest
    );

    $url = $center . $centerPath . '?uin=' . $shopData[0] . '&shopId=' . $shopData[1] . '&orderId=' . $cache['ExtOrderId'];
    $res = https_get($url);
    if ($res === "success") {
        $log->writeFile(FormatLogStr('midas', 'callback', ERROR_0, $url . json_encode($param)));
        $redis->HSet('recharge_order', $cache['ExtOrderId'], $cache);
        $order_arr['status'] = 1;
        insertOrder("t_order", $order_arr, $config['mysql']);
        die('{"ret":0,"msg":"OK"}');
    }else{
        $cache['Status'] = 'fail';
        $redis->HSet('recharge_order', $cache['ExtOrderId'], $cache);
        $logic->writeFile(FormatLogStr('midas', 'callback', ERROR_CENTER, $url . json_encode($param)));
        $order_arr['status'] = 0;
        insertOrder("t_order", $order_arr, $config['mysql']);
        die('{"ret":4,"msg":"下发商品失败"}');
    }
}else {
    $logic->writeFile(FormatLogStr('midas', 'callback', ERROR_SIGN, json_encode($param)));
    die('{"ret":4,"msg":"验证签名失败"}');
}



