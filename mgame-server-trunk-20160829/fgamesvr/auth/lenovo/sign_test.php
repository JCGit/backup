<?php

require("lib/lenovo_cashier_rsa.inc.php");
require_once 'config.php';
require(dirname(__FILE__) . "/../utils/util.php");


/*
sign=G/tSDpqZeNDZg7enTRd6W9I6gKukcG6Gc61dTDuiJpPJxSBiw1FSpniwb2my0Tbk9bftsenFJPTsM5xfE4eQ+8SWlztWnQgGD7/YmpOx0wwvtGEGKZUlNf4U0LKr0SRFlYBGPhdhA2F1n2DDHewWOCRTC+DqIFVzTyVEkomnpN0=&transdata={"transtype":0,"result":0,"transtime":"2015-12-23 14:08:03","count":1,"paytype":5,"money":600,"waresid":35921,"appid":"1510300252699.app.ln","exorderno":"151223140717773","feetype":0,"transid":"2151223140803713759198526","cpprivate":"10001808"}
*/

$transdata = '{"transtype":0,"result":0,"transtime":"2015-12-23 14:08:03","count":1,"paytype":5,"money":600,"waresid":35921,"appid":"1510300252699.app.ln","exorderno":"151223140717773","feetype":0,"transid":"2151223140803713759198526","cpprivate":"10001808"}';
$sign = 'G/tSDpqZeNDZg7enTRd6W9I6gKukcG6Gc61dTDuiJpPJxSBiw1FSpniwb2my0Tbk9bftsenFJPTsM5xfE4eQ+8SWlztWnQgGD7/YmpOx0wwvtGEGKZUlNf4U0LKr0SRFlYBGPhdhA2F1n2DDHewWOCRTC+DqIFVzTyVEkomnpN0=';

$transdata = urlencode($transdata);
$sign = urlencode($sign);

echo "Urlencode : " . $sign . "\n";

$url = "http://203.195.132.24/lenovo/callback.php?transdata=$transdata&sign=$sign";
echo "url is : $url \n";
$res = https_get($url);
echo "result is $res";
