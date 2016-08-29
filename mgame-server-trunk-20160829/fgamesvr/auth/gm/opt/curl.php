<?php

require_once(dirname(__FILE__)."/../title.php");
require_once(dirname(__FILE__)."/../util.php");
require_once(dirname(__FILE__)."/../config.php");

if (!sessionCheck()){
    echo "你没有登录，请先登录";
    return;
}


$uin = $_GET['uin'];
echo https_get($uin);
?>
