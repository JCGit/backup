<?php

require __DIR__ . "/magic_cfg.php";
require(dirname(__FILE__) . "/../util.php");


$method = $_SERVER['REQUEST_METHOD'];
$data = array();
if ($method == "GET") {
	$data = $_GET;
}elseif ($method == "POST") {
	$data = $_POST;
}else{
	error_log(FormatLogStr('magic.php', 'invalidate request method : ' . $method));
	die('false');
}

if (empty($data['sign']) || empty($data['platform'])) {
	//没有发现签名数据
	error_log(FormatLogStr('magic.php', 'not found sign | platform param with param ' . json_encode($data)));
	die('false');
}

//得到请求中的sign
$sign = $data['sign'];

$path = $data['path'];

// ** post的sign必须得解码,get会自动解码
if ($method == "POST"){
	$sign = urldecode($sign);
}

//去除sign字段
unset($data['sign']);
//得到请求参数
$str = getDataStr($data);

function do_path($src, $path){
	if ($path != '/gm') {
		return str_replace('/gm', $path, $src);
	}else{
		return $src;
	}
}

//平台为ip地址说明是内网测试用的
function getUrlByPlatform($platform){
	if (filter_var($platform, FILTER_VALIDATE_IP, FILTER_FLAG_NO_RES_RANGE)){
		return "http://" . $platform . ":7602/gm?";
	}elseif ($platform == "tecent"){
		return $GLOBALS['gmQQAddr'];
	}elseif ($platform == "lenovo"){
		return $GLOBALS['gmLenovoAddr'];
	}elseif ($platform == "mplus"){
		return $GLOBALS['gmMplus'];
	}elseif ($platform == "pokemon"){
		return $GLOBALS['gmpokemon'];
	}else{
		return false;
	}
}

if (verify($str, $sign, $pubKey)) {
	$platform = $data['platform'];
	unset($data['platform']);
	unset($data['path']);
	$str = getDataStr($data);
	$url = getUrlByPlatform($platform);
	if (!$str){
		error_log(FormatLogStr('magic.php', "platform error with param " . $str));
		echo 'false';
		return;
	}
	$url = do_path($url, $path);
	error_log(FormatLogStr('magic.php', "$url" . $str));
	echo https_get($url . $str);
	return;
}else{
	error_log(FormatLogStr('magic.php', 'verify sign failed with param ' . $str . " with sign : $sign"));
	echo 'false';
}
