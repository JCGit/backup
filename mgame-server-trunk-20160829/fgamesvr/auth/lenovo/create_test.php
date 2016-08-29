<?php

$data = array("appid" => "testid",
				"lpsust" => "hello",
				"waresid" => 24920,
				"money" => 20);

$curl = curl_init();
curl_setopt($curl, CURLOPT_URL, "http://127.0.0.1/lenovo/createorder.php"); // 要访问的地址
curl_setopt($curl, CURLOPT_FOLLOWLOCATION, 1); // 使用自动跳转
curl_setopt($curl, CURLOPT_POST, 1); // 发送一个常规的Post请求
curl_setopt($curl, CURLOPT_POSTFIELDS, $data); // Post提交的数据包
curl_setopt($curl, CURLOPT_TIMEOUT, 30); // 设置超时限制防止死循环
curl_setopt($curl, CURLOPT_HEADER, 0);
curl_setopt($curl, CURLOPT_RETURNTRANSFER, 1); // 获取的信息以文件流的形式返回
$tmpInfo = curl_exec($curl); // 执行操作
echo $tmpInfo;
?>
