<?php
/* *
 * 联想支付平台RSA函数
 * 详细：RSA签名、验签
 * 版本：1.0
 * 日期：2014-10-30
 * 说明：
 * 以下代码只是为了方便商户测试而提供的样例代码，商户可以根据自己网站的需要，按照技术文档编写,并非一定要使用该代码。
 * 该代码仅供学习和研究联想支付平台接口使用，只是提供一个参考。
 */

/**
 * RSA签名
 * @param $data   待签名数据
 * @param $priKey 密钥
 * return 签名结果
 */
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

/**
 * RSA验签
 * @param $data   待签名数据
 * @param $priKey 密钥
 * @param $sign   要校对的的签名结果
 * return 验证结果
 */
function verify($data, $priKey, $sign)  {
	$selfSign = sign($data, $priKey);
    if($selfSign === $sign){
		return true;
	}else{
		return false;
	}
}

?>