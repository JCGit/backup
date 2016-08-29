<?php

require '../response.php';
require '../config.php';
require '../util.php';


if (!sessionCheck()){
    Response::JsonRender(array('ret' => ERR_NO_LOGIN, 'msg' => '请优先登录!'));
}

$params = '';
$method = $_SERVER['REQUEST_METHOD'];
if (strtoupper($method) == "GET"){
    $params = $_GET;
}elseif (strtoupper($method == "POST")) {
    $params = $_POST;
}else{
    Response::JsonRender(array('ret' => ERR_REQUEST , 'msg' => '不支持的请求'));
}

$result = array();

$platform = $params['platform'];
// $platform = 'test';

foreach ($server_list[$platform] as $key => $value) {
    if ($key == "login"){
        $conn = ssh2_connect($value);
        if (!$conn) {
            Response::JsonRender(array('ret' => ERR_SSH, 'msg' => 'ssh建立连接失败!'));
        }
        if (!ssh2_auth_password($conn, $user, $pass)){
            Response::JsonRender(array('ret' => ERR_SSH, 'msg' => 'ssh授权失败！'));
        }
        $stream = ssh2_exec($conn, $cmd_str);
        sleep(1);
        $errorStream = ssh2_fetch_stream($stream, SSH2_STREAM_STDERR);

        $exist = false;
        while($line = fgets($stream)) {
            flush();
            $arr = explode(' ', preg_replace ("/\s(?=\s)/","\\1", trim($line)));
            array_splice($arr,2,1);
            $arr[2] = trim(basename($arr[2]));
            $result[$value][] = $arr;
            $exist = true;
        }
        
        if (!$exist) {
            $result[$value][] = array('0', '不存在', 'config.login');
        }

        // Close the streams        
        fclose($errorStream);
        fclose($stream);
    }else{
        foreach ($value as $gKey => $gValue) {
            $conn = ssh2_connect($gValue);
            if (!$conn) {
                Response::JsonRender(array('ret' => ERR_SSH, 'msg' => 'ssh建立连接失败!'));
            }
            if (!ssh2_auth_password($conn, $user, $pass)){
                Response::JsonRender(array('ret' => ERR_SSH, 'msg' => 'ssh授权失败！'));
            }
            $stream = ssh2_exec($conn, $cmd_str);
            sleep(1);
            $errorStream = ssh2_fetch_stream($stream, SSH2_STREAM_STDERR);

            $exist = false;
            while($line = fgets($stream)) {
                flush();
                $arr = explode(' ', preg_replace ("/\s(?=\s)/","\\1", trim($line)));
                array_splice($arr,2,1);
                $arr[2] = trim(basename($arr[2]));
                $result[$gValue][] = $arr;
            }

            if (!$exist) {
                $result[$gValue][] = array('0', '不存在', 'config');
            }
            
            // Close the streams        
            fclose($errorStream);
            fclose($stream);
        }

    }
}


Response::JsonRender(array('ret' => ERR_0, 'data' => json_encode($result)));