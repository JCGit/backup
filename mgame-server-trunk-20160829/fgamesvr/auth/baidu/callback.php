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
//$AppId = 7364523; //应用开发者appid
//$Secretkey = "vh265E1UsA1yZ395QGusda7ujbpaTl5H"; //应用开发者apKey

$redis = new RedisHelper($config);

$Res = notify_process($AppId, $Secretkey);

print_r($Res);

/**
 * 此函数就是接收服务器那边传过来传后进行各种验证操作处理代码
 * @param int $AppId 应用Id
 * @param string $Secretkey 应用Secretkey
 * @return json 结果信息
 */
function notify_process($AppId,$Secretkey){
	
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
	$connectorParam = "&";
	$spiltParam="=";
	if(!empty($inputParams)){
		if(strpos($inputParams,$connectorParam) && strpos($inputParams,$spiltParam)){
			$list=explode($connectorParam,$inputParams);
			//print(count($list));
			for($i=0;$i<count($list);$i++){
				$kv=explode($spiltParam,$list[$i]);
				if(count($kv)>1){
					if($kv[0]=="OrderSerial"){
						$OrderSerial=$kv[1];
					}else if($kv[0]=="CooperatorOrderSerial"){
						$CooperatorOrderSerial=$kv[1];
					}else if($kv[0]=="Sign"){
						$Sign=$kv[1];
					}else if($kv[0]=="Content"){
						$Content=urldecode($kv[1]);	//读取POST流的方式需要进行UrlDecode解码操作
						//print($Content);
					}
				}
			}
		}
	}
	//参数检测
	if(empty($OrderSerial)||empty($CooperatorOrderSerial)||empty($Sign)
		||empty($Content)){
		$Result["AppID"] =  $AppId;
		$Result["ResultCode"] =  1000;
		$Result["ResultMsg"] =  urlencode("接收参数失败");
		$Result["Sign"] =  md5($AppId.$Result["ResultCode"].$Secretkey);
		$Result["Content"] =  "";
		$Res = json_encode($Result);
		return urldecode($Res);
	}
	
	//检测请求数据签名是否合法
	if($Sign != md5($AppId.$OrderSerial.$CooperatorOrderSerial.$Content.$Secretkey)){
		$Result["AppID"] =  $AppId;
		$Result["ResultCode"] =  1001;
		$Result["ResultMsg"] =  urlencode("签名错误");
		$Result["Sign"] =  md5($AppId.$Result["ResultCode"].$Secretkey);
		$Result["Content"] =  "";
		$Res = json_encode($Result);
		return urldecode($Res);
	}

	//base64解码
	$Content=base64_decode($Content);
	//error_log("baidu  Content ---------------- " . $Content);

	//json解析
	$Item=extract(json_decode($Content,true));

	//$UID $MerchandiseName $OrderMoney $StartDateTime $BankDateTime $OrderStatus $StatusMsg $ExtInfo $VoucherMoney 
	//根据获取到的数据，执行业务处理
	// $CooperatorOrderSerial	//CP订单号

	//百度的发货请求里面只有商品名字，需要根据商品名字来找到商品Id, 这块放在cener中会比较好一点，但是center要重新加一个接口才能这样做
	//现在要做一个映射
	
	//到redis中去生成订单信息
	$orderId = $CooperatorOrderSerial . "_baidu";
	//error_log("baidu  === orderId " . $orderId);
	global $redis;
	$data = $redis->HGet("recharge_order", $orderId);
	if ($data)
	{
		//针对已经存在的这种情况，只需给平台方发送成功
		$Result["AppID"] =  $AppId;
		$Result["ResultCode"] =  1;
		$Result["ResultMsg"] =  urlencode("成功");
		$Result["Sign"] =  md5($AppId.$Result["ResultCode"].$Secretkey);
		$Result["Content"] = "";
		return;
	}

	 //ExtInfo 跟前端规定好， Extinfo 存  {"Uin":uin, "ShopId":shopid}
 
	$UserInfo = json_decode($ExtInfo, true);
	$Uin = $UserInfo["Uin"];
	$ShopId = $UserInfo["ShopId"];

	if ($OrderStatus === 1) {
		$Status = "success"; 
	}
	else
	{
		$Status = "fail";
	}

	$data = array(
		'Time' => $StartDateTime,
		'Status' => $Status,
		'ShopId' => $ShopId,    				//这个应该是内部shopid, 而不是  $Item["MerchandiseName"]对应的计费点
		'OrderId' => $orderId,  //应用订单号,   
		'ExtOrderId' => $OrderSerial,  //SDK系统内部订单号 <-> 也即第三方订单号
		'PayTime' => $BankDateTime,	
		'Uin' => $Uin,
		'PayId' => $OrderSerial,
		'Money' => $OrderMoney,
	);

	if ($data)
	{
		//$stream = json_encode($data);
		if ($redis->HSet("recharge_order",$orderId,$stream) === false){
			// $log->writeFile("callback===== save to redis failed",true);
			error_log(FormatLogStr('baidu', 'callback', ERROR_0, "save to redis failed"));
	    		echo 'FAILTURE';
	    		return;
    		}
	}
	
	//给center发送发货通知
		//给center发送发货通知
	global $centerPath;
	global $config;
	$url = $config['center'] .  $centerPath . '?uin=' . $Uin . '&orderId=' . $orderId . "&shopId=" . $ShopId;
	$res = https_get($url);

	if ($res === 'callback success'){
		$log = GetLogger($config['logger']);
	    	$log->writeFile(FormatLogStr('baidu', 'callback', ERROR_0, $url . ' ExtOrderId : ' . $orderId));
		error_log(FormatLogStr('baidu', 'callback', ERROR_0, " SUCCESS"));
	}else{
		$redis->HSet("recharge_order", $orderId, $cache);
		error_log(FormatLogStr('baidu', 'callback', ERROR_CENTER, $url));

	}

	//返回成功结果
	$Result["AppID"] =  $AppId;
	$Result["ResultCode"] =  1;
	$Result["ResultMsg"] =  urlencode("成功");
	$Result["Sign"] =  md5($AppId.$Result["ResultCode"].$Secretkey);
	$Result["Content"] = "";
	error_log(FormatLogStr('baidu', 'callback', ERROR_0, "finished!"));
	$Res = json_encode($Result);
	return urldecode($Res);	
}
?>