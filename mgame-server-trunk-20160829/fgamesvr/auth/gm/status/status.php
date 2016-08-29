<?php

require '../response.php';
require '../config.php';
require '../util.php';

require '../token/magic_cfg.php';

/*
$uri = array(
    'gmtype' => 'adddiamond',
    'uin' => 10000014,
    'value' => 20,
    );

*/

/*
method 说明

get 请求用于读取数据
post 用于改变数据

停机维护参数:

get     type=1
post    type=1&status=1&content=停服维护中....


白名单参数:

get     type=2
post    type=2&action=1&openid=player1 //action = 1 代表新增 action = 2代表删除

*/


if (!sessionCheck()){
    Response::JsonRender(array('ret' => ERR_NO_LOGIN, 'msg' => '请优先登录!'));
}

$params = "";
$method = $_SERVER['REQUEST_METHOD'];
if (strtoupper($method) == "GET"){
    $params = $_GET;
    // if(!verifySign()){
    //     Response::JsonRender(array('ret' => ERR_SIGN, 'msg' => '验证签名失败!'));
    // }
    getData();
}elseif (strtoupper($method == "POST")) {
    $params = $_POST;
    // if(!verifySign()){
    //     Response::JsonRender(array('ret' => ERR_SIGN, 'msg' => '验证签名失败!'));
    // }
    changeData();
}else{
    Response::JsonRender(array('ret' => ERR_REQUEST , 'msg' => '不支持的请求'));
}

function verifySign(){
    global $params;
    if (empty($params['sign'])){
        error_log("verifySign no found sign data field:", json_encode($params));
        return false;
    }

    //得到请求中的sign
    $sign = $params['sign'];
    //去除sign字段
    unset($params['sign']);
    //得到请求参数
    $str = getDataStr($params);

    if (verify($str, $sign, $pubKey)) {
        return true;
    }else{
        error_log("verifySign failed with source[$str] dest[$sign]");
        return false;
    }
}

function getData(){
    global $params;
    $type = intval($params['type']);
    $platform = intval($params['platform']);
    $platformPath = getPlatfromPath($platform);
    $fileName = '';
    $serverDes = '';
    if ($type == 2) {
        $fileName = DATA_DIR . $platformPath . '/stop.json';
        $serverDes = file_get_contents(EXTRA_FILE);
        if (!$serverDes){
            Response::JsonRender(array('ret' => ERR_READ_FILE , 'msg' => '读取服务器状态文件数据失败'));
        }
    }elseif ($type == 3){
        $fileName = DATA_DIR . $platformPath . '/balance.json';
    }elseif($type == 4) {
        $fileName = DATA_DIR . $platformPath .'/gray.json';
    }elseif ($type == 6) {
        $cfg = require(dirname(__FILE__) . "/../../../www_9001/utils/config.php");
        //$cfg = require(dirname(__FILE__) . "/../../utils/config.php");
        $str = "{\"goal_version\" : " . $cfg['goal_version'] . ",\"release_flag\" :" . ($cfg['release_flag']?'true':'false') . "}";
        Response::JsonRender(array('ret' => ERR_0, 'data' => json_decode($str, 1)));
    }
    else{
        Response::JsonRender(array('ret' => ERR_TYPE , 'msg' => '没找到对应的请求类型'));
    }

    $data = file_get_contents($fileName);
    if (!$data){
        Response::JsonRender(array('ret' => ERR_READ_FILE , 'msg' => '读取数据失败'));
    }

    Response::JsonRender(array('ret' => ERR_0 , 'data' => json_decode($data,1), 'ext' => json_decode($serverDes, 1)));
}

function reloadConf($id){
    $url = $GLOBALS['gmToken'];
    $uri = '';
    if ($id != 0) {
        $platform = getPlatfrom($id);
        $uri =  'path=/management' . '&platform=' . $platform;
        $sign = getSign($uri, $GLOBALS['priKey']);
        $uri = $uri . '&sign=' . $sign;
        $url = $url . $uri;
    }else{
        $url = 'http://' . $GLOBALS['params']['ip'] . ':7602/management?';
    }
    $rst = https_get($url);
    if (empty($rst)){
        Response::JsonRender(array('ret' => ERR_GM , 'msg' => '能过token请求失败'));
    }else{
        Response::JsonRender(array('ret' => ERR_0 , 'msg' => "$rst"));
    }
}



function formatTab($level){
    $tab = '';
    for($i = 0; $i < $level; $i++){
        $tab = $tab . "\t";
    }
    return $tab;
}

function formatKey($key){
    $type = gettype($key);
    $r = '';
    if (is_string($key)){
        $r = "\"" . $key . "\"";
    }elseif (is_int($key)) {
        $r = $key;
    }elseif (is_bool($key)){
        $r = $key ? 'true' : 'false';
    }
    return $r;
}

function formatValue($v, $l){
    if (is_array($v)){
        return arrayFlatten($v, $l + 1);
    }else{
        return formatKey($v) . ",";
    }
}

function arrayFlatten($arr, $level = 1){
    if (is_array($arr)){
        $str = "array(\n";
        foreach ($arr as $key => $value) {
            $str = $str . formatTab($level) . formatKey($key) . " => " . formatValue($value, $level) . "\n";
        }
        if ($level !== 1){
            $str = $str . formatTab($level) . "),";
        }else{
            $str = $str . "\t);\n";
        }
        return $str;
    }
}

function release($v, $flag){
    $path = dirname(__FILE__) . "/../../../www_9001/utils/config.php";
    //$path = dirname(__FILE__) . "/../../utils/config.php";
    $cfg = require($path);
    $rawBegin = "<?php \n\nreturn ";
    $cfg['goal_version'] = intval($v);
    $cfg['release_flag'] = (intval($flag) == 1);
    $str = $rawBegin . arrayFlatten($cfg, 1);
    file_put_contents($path, $str);
    Response::JsonRender(array('ret' => ERR_0));
}


function changeData(){
    global $params;
    global $gmToken;
    global $priKey;
    $type = intval($params['type']);
    $platform = intval($params['platform']);
    $platformPath = getPlatfromPath($platform);
    $fileName = '';
    if ($type == 2) {
        $fileName = DATA_DIR . $platformPath . '/stop.json';
    }elseif ($type == 3) {
        $fileName = DATA_DIR . $platformPath . '/balance.json';
    }elseif ($type == 4) {
        $fileName = DATA_DIR . $platformPath . '/gray.json';
    }elseif ($type == 5) {
        reloadConf($platform);
    }elseif ($type == 6) {
        release($params['goal_version'], $params['release_flag']);
    } else{
        Response::JsonRender(array('ret' => ERR_TYPE , 'msg' => '没找到对应的类型'));
    }


    $data = file_get_contents($fileName);
    if (!$data){
        Response::JsonRender(array('ret' => ERR_READ_FILE , 'msg' => '读取数据失败'));
    }

    error_log(FormatLogStr( 'user : ' . $_SESSION['name'], json_encode($params)));

    $gmtype = '';

    if ($type == 2) {
        $gmtype = 'gmtype=whitelist&data=';
        if (is_null(json_decode($params['ext']))) {
            Response::JsonRender(array('ret' =>ERR_JSON, 'msg' => '服务器状态描述不是合法的json'));
        }
    }elseif ($type == 3){
        $gmtype = 'gmtype=balance&data=';
    }elseif ($type == 4){
        $gmtype = 'gmtype=gray&data=';
    }else{
        Response::JsonRender(array('ret' =>ERR_TYPE, 'msg' => "不知道的类型 $type!"));
    }

    if (is_null(json_decode($params['text']))) {
        Response::JsonRender(array('ret' =>ERR_JSON, 'msg' => '不是合法的json'));
    }
    $platform = intval($params['platform']);
    $url = '';
    $uri = $gmtype . str_replace(PHP_EOL,"", $params['text']);
    if ($platform != 0) {
        $url = $gmToken;
        $platform = getPlatfrom($platform);
        $uri =  $uri. '&path=/gm' . '&platform=' . $platform;
        $sign = getSign($uri, $priKey);
        $uri = $uri . '&sign=' . $sign;
    }else{
        $url = 'http://' . $params['ip'] . ':7602/gm?';
    }
    $url = $url . $uri;
    $rst = https_get($url);
    if ($rst == "true") {
        file_put_contents($fileName, $params['text']);
        if ($type == 2){
            file_put_contents(EXTRA_FILE, $params['ext']);
        }
        Response::JsonRender(array('ret' => ERR_0));
    }else{
        Response::JsonRender(array('ret' =>ERR_GM, 'msg' => "通知gm失败! $rst"));
    }

}


