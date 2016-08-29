<?php

require(dirname(__FILE__) . "/lib/utils.php");

$transdata = '{"appid":"5000002657","appuserid":"1000015","cporderid":"20151216160422346_kupai","cpprivate":"1000015#1","currency":"RMB","feetype":2,"money":0.06,"paytype":401,"result":0,"transid":"32041512161604227086","transtime":"2015-12-16 16:05:28","transtype":0,"waresid":1}';
$sign = 'IZ3k3P20nk9TNzQmUBXXAPE4p6LjR9dElpaFbYu6EvUFK5svqGp3gUm9Y1UVJ8FdngqL1hAZVGV+a/1nK87CNj6kZWN3AZ+IQSDeuLJiOPDT/Sz3hZe/QItnP4khfRbqmaD/6TdiD6Cr4aweZCIM/grl1fiRSfOwjJhBnTQzIPY=';

$publicKey = 'MIGfMA0GCSqGSIb3DQEBAQUAA4GNADCBiQKBgQCHwGMN9MIRCleIa5KeIYiHx+RS4ZoA/fazprdKXASP6/rmvF4DrUSaFVPYTHb6UCWxwh5Zife2Sht52+6p7kZuhJa5yztkWhBPVL0qBlp4Yx8wWg43Jnb4mV0/QCnBFsKDDXpPYveGNSKkYPYWNm8eT5wALCN5ln8x3BuXqHk6GwIDAQAB';

$privateKey = 'MIICXgIBAAKBgQDTm2iP4cHqnBa6xdAgg+X6LLLG2FjL6R+ZU7uJ1zDMHtLf90JZ55h4Qhj/rkDLscdjnufHlXs2qBoBYE+nRBMfaCSj1lwpV2xUfsZFZGl9r5jLeBpBiSOfPXpodiMpjOZtjDpMxunYE8H7OcGG0HX8WewDHzLHmcOSOBnt3PD6QwIDAQABAoGBAMk6ydXCF7m20/qrsL+rDuG47g4goVjQiecydG6hwFrsmRa5OVTr49fA66sBl8bdekKnU709zt1sQMa8QyNKnm8MjN9HN8nOLftET5U0X22Zf/m8j7LJp90iUsab2zsWmdfwDqYsBxeT138bvsdUQ6NCWwXdKlLC/K9C2zWeTtIxAkEA+1KtqfrBhaoUxTF37lc9QdhKsWlxbZH5GF2A67m3QzOhToNsJiDJA4cEnjYcoYNuXXf0rye24VV8HpTYrGjr6QJBANeLhUEQ9ilLFrm2nEKaqpDudVBKXO6sh7xDCCLhMkzEYWCXAWEqUBIxhz2O7S8QCHBWM4Mr9JcP2AkFjAqz1UsCQFcpaHNgNQevYf611ltBxEv2wIjhAea3ToWlwBWA1k8nM9Vv/O0IR87CX9ai+7EETR17gc/RXTqvFBnjE3ZP+SkCQQCE6vbLfwgtwq5K679Em1gfc81IK/NPA3nYsIVt7O01xcw+4h/pqUwjTAOYBHFjGkH59E5jz1FPbmfMeG05lFgtAkEAvimsJFeu83mB46qy1FF43Z5GYCPpqwjZk7XupzeM2CY82rjSSSaCvDkfaZIgxQrOu9l3YHBLhyWtMg9IrUNE/w==';

$toSign = 'transdata=' . urlencode($transdata) . '&sign=' . urlencode($sign) . '&signtype=RSA';

if(verify($transdata, $sign, formatPubKey($publicKey)))
{
	echo "success";
}else
{
	echo "fail";
}

