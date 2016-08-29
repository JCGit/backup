<?php
require_once(dirname(__FILE__)."/../util.php");

$ret = array();

$status = true;
if (!$_GET["server_ip"]) {
    $status = false;
	$ret["msg"] = "没有输入服务器ip";
} else if(!$_GET["server_port"]){
    $status = false;
	$ret['msg'] = "没有输入服务器端口";
} else if(!$_GET["uin"]) {
    $status = false;
	$ret["msg"] = "没有输入玩家uin";
}

if (!$status) {
    echo json_encode($ret);
    return;
}

$server_ip = $_GET["server_ip"];
$server_port = $_GET["server_port"];
$uin = $_GET["uin"];

$url = "http://".$server_ip.":".$server_port."/gm?";
$url = $url."gmtype=change_player_name&uin=".$uin."&player_name=";

$msg = https_get($url);
$ret["msg"] = $msg;
echo json_encode($ret);

?>
