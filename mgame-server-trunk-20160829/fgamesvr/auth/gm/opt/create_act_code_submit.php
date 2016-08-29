<?php

require_once(dirname(__FILE__)."/../title.php");
require_once(dirname(__FILE__)."/../util.php");

if(!$_GET["server_ip"])
{
	echo "没有输入服务器ip";
	return;
}
if(!$_GET["server_port"])
{
	echo "没有输入服务器端口";
	return;
}
if(!$_GET["name"])
{
	echo "没有输入名字";
	return;
}
if(!$_GET["type"])
{
	echo "没有输入类型";
	return;
}

if(!$_GET["count"])
{
	echo "没有输入数量";
	return;
}

if(!$_GET["length"])
{
	echo "没有输入长度";
	return;
}

if(!$_GET["reward"])
{
	echo "没有输入奖励";
	return;
}

$server_ip = $_GET["server_ip"];
$server_port = $_GET["server_port"];
$name = $_GET["name"];
$type = $_GET["type"];
$count = $_GET["count"];
$length = $_GET["length"];
$reward = $_GET["reward"];
$reward_type = $_GET["reward_type"];

$url = "http://".$server_ip.":".$server_port."/cdk/genKey?";
$url = $url."count=".$count."&length=".$length."&type=".$type."&reward=".$reward."&reward_type=".$reward_type;
$url = $url."&name=".$name."&platform=0";

$ret = https_get($url, 40);
if(!$ret)
{
	echo "连接失败";
}
$ret_obg = json_decode($ret, true);
if($ret_obg["status"] == 0)
{
	echo "<h1>生成成功</h1>";
	$cdks = $ret_obg["cdks"];
	$cdks_arr = explode(",", $cdks);

	foreach($cdks_arr as $value)
	{
		echo "<div>".$value."</div>";
	}

}
?>
