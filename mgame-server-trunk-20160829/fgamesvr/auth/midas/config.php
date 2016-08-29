<?php

// 应用基本信息
// 注意分水箱和现网的key
// 沙箱: QkEYDzOjsbqUh5TtAvTL1BkkSd3nrTMO  现网: TCmpOqG0XD6APD7LX1TydAw4Bf9tR5tG
$appid = 1450005915;
$appkey = 'QkEYDzOjsbqUh5TtAvTL1BkkSd3nrTMO';
//$appkey = 'TCmpOqG0XD6APD7LX1TydAw4Bf9tR5tG';

// OpenAPI的服务器IP 
// 最新的API服务器地址请参考wiki文档: http://wiki.open.qq.com/wiki/API3.0%E6%96%87%E6%A1%A3 
// 正式环境下使用域名：msdk.qq.com。测试环境下使用IP：msdktest.qq.com。
$server_name = 'msdktest.qq.com';

$isTest = 0;
if(strstr($server_name,"test"))
{
    $isTest = 1;
}

// 用户的OpenID/OpenKey
// $openid = 'E098C1E975A2459E534B48FB3224CFB6';
// $openkey = '05219DB6D7C04CA0B3F01A51D32635E3';

// 所要访问的平台, pf的其他取值参考wiki文档: http://wiki.open.qq.com/wiki/API3.0%E6%96%87%E6%A1%A3 
// $pf = 'qzone';

//中心服务器地址
$centerPath = '/midas/callback';

$plat_from =  array(
    'qq' => 'qq',
    'QQ' => 'qq',
    'weixin'   => 'weixin',
    'WEIXIN'   => 'weixin',
    'lenovo-qq' => 'lenovo-qq',
    'lenovo-weixin' => 'lenovo-weixin',
    );