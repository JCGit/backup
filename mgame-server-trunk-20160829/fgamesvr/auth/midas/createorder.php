<?php

require_once __DIR__ . '/lib/OpenApiV3.php';
require_once 'config.php';
require(dirname(__FILE__) . "/../utils/util.php");
$config = require(dirname(__FILE__) . "/../utils/config.php");

//http://wiki.open.qq.com/wiki/v3/pay/buy_goods
// v3/pay/buy_goods 

// HTTP GET /mpay/buy_goods_m?openid=67E87013295359D1E1C300EE6BC1230F&openkey=7022343841EB434C713E16A008799AC8&pay_token=E1EB7DD2CC7459DD058B83642CECC2C8&appid=1450005915&ts=1453445462&sig=ar66I%2BxFBJ6MMi1gEr9y%2BOvm%2BVg%3D&pf=desktop_m_qq-73213123-android-73213123-qq-1105115982-67E87013295359D1E1C300EE6BC1230F&pfkey=pfkey&zoneid=1&payitem=1*1*1&goodsmeta=1*1&goodsurl=1app_metadata=10000#23


$sdk = new OpenApiV3($appid, $appkey);
$sdk->setServerName($server_name);

$params = array();
$cookie = array();
// 得到请求方式
$method = $_SERVER['REQUEST_METHOD'];

$logic = GetLogger($config['logiclog']);

if ($method == "GET") {
    $params = $_GET;
}elseif ($method == "POST") {
    $params = $_POST;
}else {
	$logic->writeFile(FormatLogStr('midas', 'createorder', ERROR_PARAM,  'invalidate request method : ' . $method));
    return;
}


if (isset($params['pay_token'])) {
	$params = array(
		'pfkey' 		=> 'pfkey',
		'pay_token' 	=> $params['pay_token'],
		// 'amt'   => $param['amt'],
		'payitem' 		=> $params['payitem'],
		'pf'  			=> $params['pf'],
		'openid'    	=> $params['openid'],
		'openkey'    	=> $params['openkey'],
		'ts'      		=> time(),
		'goodsmeta' 	=> $params['goodsmeta'],
		'goodsurl'  	=> $params['goodsurl'],
		'zoneid' 		=> 1,
		'app_metadata'	=> $params['app_metadata'],
		);

	$cookie = array('session_id' 	=> 'openid',
					'session_type' 	=> 'kp_actoken',
					'org_loc' 		=> urlencode('/mpay/buy_goods_m'));
}else{
	$params = array(
		'pfkey' 		=> 'pfkey',
		// 'amt'   => $_REQUEST['amt'],
		'pay_token' 	=> '',
		'payitem' 		=> $params['payitem'],
		'pf'  			=> $params['pf'],
		'openid'    	=> $params['openid'],
		'openkey'    	=> $params['openkey'],
		'ts'      		=> time(),
		'goodsmeta'	 	=> $params['goodsmeta'],
		'goodsurl'  	=> $params['goodsurl'],
		'zoneid' 		=> 1,
		'app_metadata'	=> $params['app_metadata'],
		);

	$cookie = array('session_id'	=> 'hy_gameid',
					'session_type' 	=> 'wc_actoken',
					'org_loc' 		=> urlencode('/mpay/buy_goods_m'));
}

$params['appmode'] = 1; //订单价格不能修改

$ret = $sdk->api('/mpay/buy_goods_m', $params, $cookie, 'get', 'http');

$logic->writeFile(FormatLogStr('midas', 'createorder', ERROR_0, 'result is : ' . $ret . ' with param ' . json_encode($params)));

echo $ret;

