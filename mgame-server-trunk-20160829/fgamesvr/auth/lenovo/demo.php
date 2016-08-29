<?php

require(dirname(__FILE__) . "/../utils/util.php");

$url = "http://203.195.132.24:8602/lenovo/callback" . '?uin=1000018' . '&orderId=5465465464' . "&shopId=8";
echo "url is $url\n";
$res = https_get($url);
if ($res === 'callback success'){
	echo "callback success $res";
}else{
	echo "fail $res";
}
?>
