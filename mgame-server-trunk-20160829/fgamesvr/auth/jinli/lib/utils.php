<?php

function rsa_verify($post_arr){
	ksort($post_arr);
	foreach($post_arr as $key => $value){
		if($key == 'sign') continue;
		$signature_str .= $key.'='.$value.'&';
	}
	include(dirname(__FILE__) . "/../config.php");
	$signature_str = substr($signature_str,0,-1);
	$publickey= $publicKey;
	$pem = chunk_split($publickey,64,"\n");
	$pem = "-----BEGIN PUBLIC KEY-----\n".$pem."-----END PUBLIC KEY-----\n";
	$public_key_id = openssl_pkey_get_public($pem);
	$signature =base64_decode($post_arr['sign']);
	return openssl_verify($signature_str, $signature, $public_key_id );
}

function rsa_sign($post_arr){
	ksort($post_arr);
	foreach($post_arr as $key => $value){
		$signature_str .= $value;
	}
	include(dirname(__FILE__) . "/../config.php");
	$private_key= $privateKey;
	$pem = chunk_split($private_key,64,"\n");
	$pem = "-----BEGIN PRIVATE KEY-----\n".$pem."-----END PRIVATE KEY-----\n";
	$private_key_id = openssl_pkey_get_private($pem);
	$signature = false;
	openssl_sign($signature_str, $signature, $private_key_id);
	$sign =  base64_encode($signature);
	return $sign;
}

