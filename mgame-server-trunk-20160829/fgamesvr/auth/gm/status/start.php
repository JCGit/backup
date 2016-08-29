<?php

require '../response.php';
require '../config.php';
require '../util.php';

if (!sessionCheck()){
    Response::JsonRender(array('ret' => ERR_NO_LOGIN, 'msg' => '请优先登录!'));
}

$params = '';
$method = $_SERVER['REQUEST_METHOD'];
if (strtoupper($method) == "GET"){
    $params = $_GET;
}elseif (strtoupper($method == "POST")) {
    $params = $_POST;
}else{
    Response::JsonRender(array('ret' => ERR_REQUEST , 'msg' => '不支持的请求'));
}

$config = $params['config'];
$opt = $params['opt'];
$host = $params['host'];

if (!isset($sh_arr[$config])){
    Response::JsonRender(array('ret' => ERR_PARAM , 'msg' => 'config参数错误^_^'));
}

if (($opt != 'start') and ($opt != 'stop')) {
    Response::JsonRender(array('ret' => ERR_PARAM, 'msg' => 'opt参数错误^_^' ));
}

if (!$host){
    Response::JsonRender(array('ret' => ERR_PARAM, 'msg' => 'host参数错误'));
}

$cmd = "cd $game_path;./" . $sh_arr[$config] . " $opt";


$conn = ssh2_connect($host);
if (!$conn) {
    Response::JsonRender(array('ret' => ERR_SSH, 'msg' => 'ssh建立连接失败!'));
}
if (!ssh2_auth_password($conn, $user, $pass)){
    Response::JsonRender(array('ret' => ERR_SSH, 'msg' => 'ssh授权失败！'));
}

$stream = ssh2_exec($conn, $cmd);
$errorStream = ssh2_fetch_stream($stream, SSH2_STREAM_STDERR);

// Enable blocking for both streams
stream_set_blocking($errorStream, true);
stream_set_blocking($stream, true);

// Whichever of the two below commands is listed first will receive its appropriate output.  The second command receives nothing
$stdout = stream_get_contents($stream);
$stderr = stream_get_contents($errorStream);

if ($stderr) {
    error_log(FormatLogStr('status/start.php', 'ssh2_exec result is ' . $stderr . " stdout is $stdout"));
    Response::JsonRender(array('ret' => ERR_SSH, 'msg' => '执行命令失败'));
}

// Close the streams        
fclose($errorStream);
fclose($stream);

Response::JsonRender(array('ret' => ERR_0, 'msg' => '成功!'));


