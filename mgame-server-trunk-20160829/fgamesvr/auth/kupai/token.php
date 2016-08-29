<?php
require(dirname(__FILE__) . "/../utils/util.php");
include('config.php');

//获取授权码
$code = $_REQUEST['code'];
$url = $tokenUrl . "?grant_type=authorization_code&client_id=5000002657&redirect_uri=$appKey&client_secret=$appKey&code=$code";
$res = https_get($url);
echo $res;



/*
//成功返回
{
	"openid": "103400",
	"expires_in": "2592000",
	"refresh_token": "0.b6037940ec15224e3f711fae3b9fbb74",
	"access_token": "0.e10adc3949ba59abbe56e057f20f883e.cf0243b77a2ba505a1ab2c8c82574736.1410342326163"
}
//失败
{
    "error": "2009",
    "error_description": ""
}
*/

