<?php
require_once(dirname(__FILE__) . "/../utils/order_factory.php");
require_once(dirname(__FILE__) . "/../utils/util.php");
require_once(dirname(__FILE__) . "/config.php");


function send2center($host, $port, $platform_name, $post_arr)
{
	global $platform_name;
	$args_str = GetSortParam($post_arr);
	return https_get($host . ":" . $port . "/" . $platform_name . "/callback?" . $args_str);
}

function wError_log($error, $str)
{
	global $platform_name;
	error_log(FormatLogStr($platform_name, 'callback', $error ,$str));
}

function logger($error, $str)
{
	global $log_path;
	global $platform_name;
	GetLogger($log_path)->writeFile(FormatLogStr($platform_name, 'callback', $error, $str));
}

function check_order($orderId, $redis_config, $platform_name)
{
	$order_factory = new OrderFactory();
	
	if(!$order_factory->init($redis_config, $platform_name))
	{
		wError_log(ERROR_PARAM, "redis con is error");
		return false;
	}
	
	if(!$order_factory->check_order_exist($orderId))
	{
		$order_factory->insert_order($orderId);
	}
	return $order_factory->check_and_proc_order($orderId);
}

function respone_result($result)
{
	$res_objs = array("result" => $result);
	echo json_encode($res_objs);
}

function check_sign($valueMap, $pub_key)
{
	if(null !== $valueMap["sign"])
	{
	    $valueMap["sign"] = urldecode($valueMap["sign"]);
	}
	if(null !== $valueMap["extReserved"])
	{
	    $valueMap["extReserved"]= urldecode($valueMap["extReserved"]);
	}
	if(null !== $valueMap["sysReserved"])
	{
	    $valueMap["sysReserved"] = urldecode($valueMap["sysReserved"]);
	}
	
	ksort($valueMap);
	$sign = $valueMap["sign"];
	
	if(empty($sign))
	{
	    return false;
	}
	
	$content = "";
	$i = 0;
	foreach($valueMap as $key=>$value)
	{
	    if($key != "sign" )
	    {
	       $content .= ($i == 0 ? '' : '&').$key.'='.$value;
	    }
	    $i++;
	}	

	$openssl_public_key = @openssl_get_publickey($pub_key);
	$result = (bool)(@openssl_verify($content, base64_decode($sign_str), $openssl_public_key));
	@openssl_free_key($openssl_public_key);
	return $result;
}

function parse_and_check_args($args, $pub_key)
{
	if(sizeof($args) <= 0)
	{
		wError_log(ERROR_PARAM, "req param is null");
		return RES_ARGS_ERROR;
	}

	if(!array_key_exists("result", $args))
	{
		wError_log(ERROR_PARAM, "req param \"result\" is not exist");
		return RES_ARGS_ERROR;
	}

	if($args["result"] == "0")
	{
		if(!check_sign($args, $pub_key))
		{
			return RES_SUCCESS;
		}
		else
		{
			wError_log(ERROR_SIGN, "sign is failed");
			return RES_SIGN_FAILD;
		}
	}
	else
		return RES_SUCCESS;
}

$check_result = parse_and_check_args($_POST, $pub_key);
if($check_result != 0)
{
	respone_result($check_result);
	return;
}

if($_POST["result"] != 0)
{
	wError_log(ERROR_PAY, "request order is not sucess");
	return;
}

$orderId = $_POST["orderId"];

$order_factory = new OrderFactory();
if(!$order_factory->init($redis_config, $platform_name))
{
	wError_log(100, "redis is connect failed");
	respone_result(RES_SYS_ERROR);
	return;
}

if(!$order_factory->check_order_exist($orderId))
{
	$order_factory->insert_order($orderId);
	if(!$order_factory->check_order_exist($orderId))
	{
		wError_log("order : ". $orderId . " insert is failed");
		return;
	}
}

if($order_factory->get_order_opt($orderId, OPT_STATE) != 0)
{
	wError_log(ERROR_EXIST, "orderId : ". $orderId . " is exist and success");
	$order_factory->close();
	respone_result(RES_SUCCESS);
	return;
}

$order_factory->set_order_opt($orderId, OPT_STATE, 1);

if(!array_key_exists("extReserved", $_POST)) 
{                                         
    $order_factory->set_order_opt($orderId, OPT_STATE, 0);
    wError_log(ERROR_GOODS, "extReserved param is null");
    respone_result(RES_ARGS_ERROR);       
}                                         
else                                      
{                                         
    $extReserved = $_POST["extReserved"]; 
    $extR_arr = explode("_", $extReserved);
    $uin = $extR_arr[0];                  
    $shopId = $extR_arr[1];               
                                          
    $post_arr = array(                    
        "orderId" => $orderId,            
        "uin" => $uin,                    
        "shopId" => $shopId);             
    $result = send2center($cent_svr_config["host"], $cent_svr_config["port"], $platform_name, $post_arr);
    if($result == "success")              
    {                                     
        respone_result(RES_SUCCESS);      
        logger(ERROR_0, json_encode($post_arr));
        $order_factory->set_order_opt($orderId, OPT_STATE, 1);
        $order_factory->set_order_opt($orderId, OPT_PAY_TIME, time());
        $order_factory->set_order_opt($orderId, OPT_SHOP_ID, $shopId);
    }                                     
}
$order_factory->close();

?>
