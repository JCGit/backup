<?php
date_default_timezone_set('Asia/Shanghai');


//支持跨域请求
header('Access-Control-Allow-Origin: *');

session_start();

define("ERR_0", 0);
define("ERR_NO_LOGIN", 101);
define("ERR_NO_USER", 102);
define("ERR_REQUEST", 103);
define("ERR_READ_FILE", 104);
define("ERR_SIGN", 105);
define("ERR_TYPE", 106);
define("ERR_SSH", 107);
define("ERR_SSH_DATA", 108);
define('ERR_USER_EMPTY', 109);
define('ERR_PASS_EMPTY', 110);
define('ERR_REGISTER', 111);
define('ERR_MYSQL', 112);
define('ERR_JSON', 113);
define('ERR_GM', 114);
define('ERR_PARAM', 115);


function https_post($url, $post_arr = array(), $timeout = 10)
{
	$curl = curl_init($url);
	curl_setopt($curl, CURLOPT_RETURNTRANSFER, 1);
	curl_setopt($curl, CURLOPT_POST, 1);
	curl_setopt($curl, CURLOPT_POSTFIELDS, $post_arr);
	curl_setopt($curl, CURLOPT_TIMEOUT, $timeout);
	curl_setopt($curl, CURLOPT_SSL_VERIFYPEER, FALSE);
	curl_setopt($curl, CURLOPT_SSL_VERIFYHOST, FALSE);
	$content = curl_exec($curl);
	curl_close($curl);

	return $content;
}

function https_get($url, $timeout = 10)
{
    $ch = curl_init($url);
    curl_setopt($ch, CURLOPT_URL, $url);
    curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);    // 要求结果为字符串且输出到屏幕上
    curl_setopt($ch, CURLOPT_HEADER, 0); // 不要http header 加快效率
    curl_setopt($ch, CURLOPT_USERAGENT, 'Mozilla/5.0 (compatible; MSIE 5.01; Windows NT 5.0)');
    curl_setopt($ch, CURLOPT_TIMEOUT, $timeout);
    curl_setopt($ch, CURLOPT_SSL_VERIFYPEER, FALSE);    // https请求 不验证证书和hosts
    curl_setopt($ch, CURLOPT_SSL_VERIFYHOST, FALSE);
    $output = curl_exec($ch);
    curl_close($ch);
    return $output;
}

function http_get($url, $timeout = 10)
{
    $ch = curl_init($url);
    curl_setopt($ch, CURLOPT_URL, $url);
    curl_setopt($ch, CURLOPT_HEADER, 0); // 不要http header 加快效率
    curl_setopt($ch, CURLOPT_USERAGENT, 'Mozilla/5.0 (compatible; MSIE 5.01; Windows NT 5.0)');
    curl_setopt($ch, CURLOPT_TIMEOUT, $timeout);
    $output = curl_exec($ch);
    curl_close($ch);
    return $output;
}

//false表示16进制
function HMacSha1($data, $key, $raw = FALSE) 
{
	return hash_hmac("sha1", $data, $key, $raw);
}


//获取按字母排序后的key1=value1&key2=value2的字符串
function GetSortParam($param, $join = '&')
{
	ksort($param);
	$signStr = '';
	foreach ($param as $key => $value) {
		if (!empty($signStr)){
			$signStr = $signStr . $join;
		}
		$signStr = $signStr . $key . '=' . $value;
	}
	return $signStr;
}

function FormatLogStr($user = '61game', $url)
{
    if (empty($user)){
        $user = '61game';
    }
	return '[' . date('Y-m-d H:i:s') . ']' . ' [' . $user . '] ' . "url : [". $url . "]";
}

function GetLogger($path, $level = 1)
{
    return Logger::getInstance($path, $level);
}

function sessionCheck(){
    if (empty($_SESSION['name'])){
        return false;
    }else{
        return true;
    }
}

function getPlatfrom($id){
    if ($id == 1){
        return "tecent";
    }elseif ($id == 2) {
        return "lenovo";
    }elseif ($id == 3) {
        return "mplus";
    }

    return "test";
}

function getPlatfromPath($id){
    if ($id == 1){
        return "/tecent";
    }elseif ($id == 2) {
        return "/lenovo";
    }elseif ($id == 3) {
        return "/mplus";
    }
    return '';
}

// 用私钥生成签名，用公钥验证
function getSign($data, $key) {
    $pem = chunk_split($key,64,"\n");
    $pem = "-----BEGIN PRIVATE KEY-----\n".$pem."-----END PRIVATE KEY-----";
    $key = openssl_pkey_get_private ( $pem );

    $res = openssl_sign ( $data, $encrypted, $key );
    if ($res) {
        return urlencode(base64_encode($encrypted));
    }else {
        return null;
    }
}


function getDataStr($data){
    $query_string = array();
    foreach ($data as $key => $val ) 
    { 
        array_push($query_string, $key . '=' . $val);
    }   
    $query_string = join('&', $query_string);
    return $query_string;
}

function verify($data, $srcSign, $key) {
    $pem = chunk_split($key,64,"\n");
    $pem = "-----BEGIN PUBLIC KEY-----\n".$pem."-----END PUBLIC KEY-----";
    $key = openssl_pkey_get_public ( $pem );
    if (openssl_verify($data, base64_decode($srcSign), $key) == 1 )
        return true;
    return false;
}