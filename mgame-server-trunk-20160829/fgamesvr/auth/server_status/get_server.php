<?php 
	$filename = "./server.json";
	$json_string = file_get_contents($filename);
	$obj=json_decode($json_string,true);
	if (!is_array($obj)) die('no successful');
	
	//php获取参数
	$openid = trim($_POST['openid']) === '' ? trim($_GET['openid']) : trim($_POST['openid']);
	$ret = 2;
	//0 停服 ， 1 维护中， 2 开服
	if ($obj["status"] === 0){
		$ret = 0;  //停服维护中
	} else if ($obj["status"] === 1) {
		//维护中
			$ret = 1;
	}else if ($obj["status"] === 2){
		//正常开服状态
		$ret = 2;
	}
	
	// 远程返回的不是 json 格式, 说明返回包有问题
	$ResultArray = array(
		'ret' => $ret,
		'content' => $obj["content"]
	);
	
	$Res = json_encode($ResultArray);
	
	print_r($Res);
?>