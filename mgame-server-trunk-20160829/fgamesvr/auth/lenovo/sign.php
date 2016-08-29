<?php
// require("lib/lenovo_cashier_rsa.inc.php");
require_once 'config.php';
require(dirname(__FILE__) . "/../utils/util.php");

function sign($data, $priKey) {
	if(strpos($priKey, "BEGIN RSA PRIVATE KEY") === false)
	{
		$priKey = wordwrap($priKey, 64, "\n", true);
		$priKey = "-----BEGIN PRIVATE KEY-----\n".$priKey."\n-----END PRIVATE KEY-----";
	}
    $res = openssl_get_privatekey($priKey);
    openssl_sign($data, $sign, $res);
    openssl_free_key($res);
    $sign = base64_encode($sign);
    return $sign;
}


$transdata = '{"transtype":0,"result":0,"transtime":"2015-11-27 10:33:01","count":1,"paytype":5,"money":100,"waresid":24923,"appid":"1510300252699.app.ln","exorderno":"lenovosdk-20151127-1100000000000267265","feetype":1,"transid":"2151127103301062812814910","cpprivate":"123456"}';

$sign = sign($transdata, $appKey);

echo "sign : $sign";


