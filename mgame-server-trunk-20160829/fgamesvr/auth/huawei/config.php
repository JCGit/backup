<?php

$pub_key = "MIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEAghHLUYYk54R+8FVrMVURVmvfNcG/8rjdhm0meDLw9TAcifgVe3xMaq2PVxXitGQ3C+UXxkfKTfCaWp0DdsaalqKqy6Wh3r52K2QgC0hMH+VrSn241PekHIlQ7FMLYePmz08LRNO8RrOy/W2Ad3rrCt0jSOqoHT/sWYqFPKauyhgCI6KkkDnPdjtyosRa3nFuH702H5mzgsV49phUYEpFpR5Ok62My71XhXLif2qnQrJnYw8vbftg47MlJn6HLzFbGC+GCkoeyCrpihbaDD3ewaRo8PHMoAUVQ+w1pFv+Hc8hbfR5TiACEeqfYbCGjjzllWB6JdX7wtsA2VRCEFTb7QIDAQAB";  //签名验证公钥
$config = require(dirname(__FILE__) . "/../utils/config.php");

//center 服务器信息
$cent_svr_config = array(
	"host" => "127.0.0.1",
	"port" => 8602);

$platform_name = "huawei";
$log_path = "/root/61sdklog/61game_sdk.log";

//redis 配置
$redis_config = array(
	"host" => "127.0.0.1",
	"port" => 6379,
	"timeout"=> 10,
	"db_sel" => 0);


//返回码定义
define("RES_SUCCESS", 0);   //支付成功
define("RES_SIGN_FAILD", 1);  //签名验证失败
define("RES_URL_ERROR", 96);	//错误的url
define("RES_ARGS_ERROR", 98);   //参数错误
define("RES_OTHER_ERROR", 99);  //其他错误
define("RES_SYS_ERROR", 94);  //系统错误

?>
