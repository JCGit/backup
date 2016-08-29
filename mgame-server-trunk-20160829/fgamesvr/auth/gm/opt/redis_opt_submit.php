<?php

require_once(dirname(__FILE__)."/../title.php");
require_once(dirname(__FILE__)."/../util.php");
require_once(dirname(__FILE__)."/../config.php");

if(!$_POST["server_ip"])
{
    echo "没有输入服务器ip";
    return;
}
if(!$_POST["server_port"])
{
    echo "没有输入服务器端口";
    return;
}
if(!$_POST["opt"])
{
    echo "没有输入活动id";
    return;
}

// 0 测试 1 qq 2 微信
if(!isset($_POST["platform"]))
{
    echo "请选择平台!";
    return;
}


$platform = intval($_GET['platform']);
if (($platform != 0) and !sessionCheck()){
    echo "你没有登录，请先登录";
    return;
}

$server_ip = $_POST["server_ip"];
$server_port = $_POST["server_port"];
$content = '';


$url = '';

//区分测试和线上平台，测试不需要验证签名,且可以指定ip 和端口
if ($platform != 0) {
    $url = $gmToken;
    $platform = getPlatfrom($platform);
    $content = 'gmtype=redis_opt&opt=' . $_POST['opt'] . '&param1=' . $_POST['param1'] . '&param2=' . $_POST['param2'] . '&param3=' . $_POST['param3']."&platform=".$platform."&path=/gm";
    $sign = getSign($content, $priKey);
    $fields = array(
        'gmtype' => 'redis_opt',
        'opt'    => $_POST['opt'],
        'param1' => $_POST['param1'],
        'param2' => $_POST['param2'],
        'param3' => $_POST['param3'],
        'platform' => $platform,
        'path'   => '/gm',
        'sign'   => $sign,
    );
    error_log(FormatLogStr($_SESSION['name'], $url . ' post param' . json_encode($fields)));
    echo (https_post($url, $fields));
    return;
}else{
    $url = "http://".$server_ip.":".$server_port."/gm";
    $content = 'gmtype=redis_opt&opt=' . $_POST['opt'] . '&param1=' . $_POST['param1'] . '&param2=' . $_POST['param2'] . '&param3=' . $_POST['param3'];
}

$param = array('content' => $content);
error_log(FormatLogStr($_SESSION['name'], $url));
echo (https_post($url, $param))
?>
