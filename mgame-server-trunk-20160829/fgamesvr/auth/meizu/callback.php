<?php
/**
 * PHP for  BaiDuSDK
 *
 * @version 1.0
 * @author 91
 */

$config = require_once(dirname(__FILE__) . "/../utils/config.php");
require_once(dirname(__FILE__) . "/../utils/util.php");
require_once(dirname(__FILE__) . "/../utils/redis.php");
require_once(dirname(__FILE__) . "/../utils/order.php");
require_once 'config.php';

header("Content-type: text/html; charset=utf-8");

if (!function_exists('json_decode')){
	exit('您的PHP不支持JSON，请升级您的PHP版本。');
}

/**
 * 应用服务器接收服务器端发过来发货通知的接口DEMO
 * 当然这个DEMO只是个参考，具体的操作和业务逻辑处理开发者可以自由操作
 */
/*
 * 这里的AppId和Secretkey是我们自己做测试的
 * 开发者可以自己根据自己在平台上创建的具体应用信息进行修改
 */
// $AppId = 2839694; //应用开发者appid
// $Secretkey = "pJv4WMsIxI9rvRrbeae8jwnQJtpdrGnf";//应用开发者apKey

$redis = new RedisHelper($config);

$Res = notify_process($AppId,$Secretkey);
print_r($Res);

/**
 * 此函数就是接收服务器那边传过来传后进行各种验证操作处理代码
 * @param int $AppId 应用Id
 * @param string $Secretkey 应用Secretkey
 * @return json 结果信息
 */
function notify_process($AppId, $Secretkey){	
	$Result = array();//存放结果数组
	$OrderSerial='';
	$CooperatorOrderSerial='';
	$Sign='';
	$Content='';

	//获取参数  提供两种获取参数方式
	//1.Request方式获取请求参数
	//if(isset($_REQUEST['OrderSerial']))
		//$OrderSerial= $_REQUEST['OrderSerial'];
	//if(isset($_REQUEST['CooperatorOrderSerial']))
		//$CooperatorOrderSerial= $_REQUEST['CooperatorOrderSerial'];
	//if(isset($_REQUEST['Sign']))
		//$Sign= $_REQUEST['Sign'];
	//if(isset($_REQUEST['Content']))
		//$Content= $_REQUEST['Content'];//Content通过Request读取的数据已经自动解码

	//2.读取POST流方式获取请求参数
	$inputParams = file_get_contents('php://input');
	$inputParams = urldecode($inputParams);
	$connectorParam = "&";
	$spiltParam="=";
	if(!empty($inputParams)){
		if(strpos($inputParams,$connectorParam) && strpos($inputParams,$spiltParam)){
			$list=explode($connectorParam,$inputParams);
			//print(count($list));
			for($i=0; $i<count($list); $i++){
				$kv=explode($spiltParam,$list[$i]);
				if(count($kv)>1){
					$Params[$kv[0]] = $kv[1];	
				}
			}
		}
	}

	ksort($Params);
	$sb = "";
	while($key= key($Params)) {
                // sign , sign_type 不用放到sign验证中
	    if($key != 'sign'  &&  $key != 'sign_type') 
	    {

		    $value = $Params[$key];
		    $sb = $sb . "&" . $key . "=" . $value;
                }
	    next($Params);
	    //每个key()调用不会推进指针。为此要使用next()函数
	}
	if ($sb !=  "")
	{
	    $sb = substr($sb, 1);
	}
	$sign_text = $sb . ":" . $Secretkey;
	error_log(" orign content " . $sign_text);
	$server_sign = md5($sign_text);

	error_log("calc sign: " . $server_sign . " param sign: " .  $Params["sign"]);

	$trade_status = $Params["trade_status"];
	
	//if ("3" != $trade_status)
	if ($server_sign != $Params["sign"]  || "3" != $trade_status)
	{
		error_log(FormatLogStr('meizu', 'callback', ERROR_SIGN, " sign check fail"));
		$Result["code"] =  900000;  //未知异常
		$Result["message"] =  "sign签名失败";
		$Res = json_encode($Result);
		return $Res;
	}

	$orderId = $Params["cp_order_id"] . "_meizu";
	global $redis;
	$data = $redis->HGet("recharge_order", $orderId);
	if ($data === false)
	{
		$Result["code"] =  900000;  //未知异常
		$Result["message"] =  "订单号不存在";

		error_log(FormatLogStr('meizu', 'callback', ERROR_NEXIST, " $orderId order not found"));
		
		$Res = json_encode($Result);
		return $Res;
	}

	$detl = json_decode($data, true);
	if ($detl && ($detl["Status"] === "success")) 
	{
		$Result["code"] =  900000;  //未知异常
		$Result["message"] =  "订单号已领取";

		error_log(FormatLogStr('meizu', 'callback', ERROR_NEXIST, " $orderId order have recharged"));
	
		$Res = json_encode($Result);
		return $Res;
	}

	$detl["Status"] = "success";
	$detl["notify_time"] = $Params["notify_time"];
	$detl["notify_id"] = $Params["notify_id"];
	$detl["uid"] = $Params["uid"];
	$detl["total_price"] = $Params["total_price"];
	$detl["pay_time"] = $Params["pay_time"];
	$detl["pay_type"] = $Params["pay_type"];
	$detl["ExtOrderId"] = $Params["order_id"];
	$our_uin =$Params["user_info"];
	$uin = $detl["Uin"];

	if ($our_uin != $uin)
	{
		$Result["code"] =  900000;  //未知异常
		$Result["message"] =  "用户Uin不对应";

		error_log(FormatLogStr('meizu', 'callback', ERROR_UIN, " uin not mapping  uin: $uin   our_uin: $our_uin  order: $orderId"));
	
		$Res = json_encode($Result);
		return $Res;
	}
	
	//$uin = $redis->HGet("uin_mapping", $uin);
	if ($uin === false)
	{
		$Result["code"] =  900000;  //未知异常
		$Result["message"] =  "用户Id不存在";

		error_log(FormatLogStr('meizu', 'callback', ERROR_UIN, " uin: $uin not exsit order: $orderId"));
	
		$Res = json_encode($Result);
		return $Res;
	}

	$shopid = $detl["ShopId"];

	$stream = json_encode($detl);
	if ($redis->HSet("uin_mapping", $orderId, $stream) === false)
	{
		//记错误日志
		error_log(FormatLogStr('meizu', 'callback', ERROR_0, " set redis  fail orderId: $orderId"));
		$Result["code"] =  900000;  //未知异常
		$Result["message"] =  "Redis写错误";
	
		$Res = json_encode($Result);
		return $Res;
	}
	
	//给center发送发货通知
	global $centerPath;
	global $config;
	//$url = $config['center'] . $centerPath . '?uin=' . $param["cpprivate"] . '&orderId=' . $orderId . "&shopId=" . $shopid;
	$url = $config['center'] .  $centerPath . '?uin=' . $uin . '&orderId=' . $orderId . "&shopId=" . $shopid;
	$res = https_get($url);

    	//echo 'SUCCESS';

	//返回成功结果
	$Result["AppID"] =  $AppId;
	$Result["ResultCode"] =  1;
	$Result["ResultMsg"] =  urlencode("成功");
	$Result["Sign"] =  md5($AppId.$Result["ResultCode"].$Secretkey);
	$Result["Content"] = "";
	$Res = json_encode($Result);
	error_log(FormatLogStr('meizu', 'callback', ERROR_0, " callback finished"));
	return urldecode($Res);	
}
?>