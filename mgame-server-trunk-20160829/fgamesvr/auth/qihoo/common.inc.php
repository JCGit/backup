<?php

//是否打开调试模式，打开后，可以查看oauth日志
//上线时，将调试设置成false. 线上记录日志，会导致产生大量日志，占用磁盘空间。
//设置为true将打开日志
define('QIHOO_MSDK_DEBUG', true);

if (stripos(PHP_OS, 'win') === 0) {
    //windows 日志路径为当前目录 qihoo_msdk.log
    define('QIHOO_MSDK_LOG', dirname(__FILE__) . '/qihoo_msdk.log');
} else {
    //*nix 日志路径为/tmp/qihoo_msdk.log
    define('QIHOO_MSDK_LOG', '/tmp/qihoo_msdk.log');
}

include "config.php";

//TODO::在此处添加应用 app_key=>app_secret 配置
$_keyStore = array(
    $qihookey => $qihoosecrety,
);

$appKey = filter_input(INPUT_GET, 'app_key');
$appSecret = isset($_keyStore[$appKey]) ? $_keyStore[$appKey] : '';
if (empty($appSecret)) {
    throw new Exception("no appSecret for appKey:$appKey");
}

define('QIHOO_APP_KEY', $appKey);
define('QIHOO_APP_SECRET', $appSecret);

define('QIHOO_MSDK_ROOT', realpath(dirname(__FILE__) . '/lib/'));

function qihooLoad($className)
{
    static $loadedClassList = array();
    if (!empty($loadedClassList[$className])) {
        return;
    }

    $path = str_replace('_', DIRECTORY_SEPARATOR, $className) . '.php';
    require_once QIHOO_MSDK_ROOT . DIRECTORY_SEPARATOR . $path;
    $loadedClassList[$className] = 1;
}

spl_autoload_register('qihooLoad');

