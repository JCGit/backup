<?php
/**
 * 调用Sdk查询订单
 * 
 */
require_once 'Sdk.php';

$sdk = new Sdk();

//客户端SDK返回的登陆令牌
$accessToken = "03f26d6a04aa42b8873a69c788111d92-a3f7276f870cd726c38691919afcded5-20151204211052-44e00a04eca8ecff4eb59008d964f8c3-f6900774038f33a76719846a0673233a-df45e21cb299bddeba2e8badcdc19a73";//

$Res = $sdk->login_state_result($accessToken);

print_r($Res);

if($Res['ResultCode']=="1"&&$Res['Sign']==$sdk->SignMd5($Res['ResultCode'],urldecode($Res['Content']))){
	//Content参数需要urldecode后再进行base64解码
	$result=base64_decode(urldecode($Res['Content']));
	print($result);
	//json解析
	$Item=extract(json_decode($result,true));
	//根据获取的信息，执行业务处理


	//打印$Item信息
	print($UID);
}
?>