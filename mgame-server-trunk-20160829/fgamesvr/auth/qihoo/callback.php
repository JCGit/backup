<?php

/**
 * 用户支付成功后，360通知游戏，游戏接受通知的接口。
 * TODO:: 参考 PayApp_Demo 里的逻辑实现真正的游戏内购买逻辑。
 */
require_once dirname(__FILE__) . '/common.inc.php';

//TODO::实现自己的App逻辑后，替换此处代码
$myApp = new PayApp_Demo(QIHOO_APP_KEY, QIHOO_APP_SECRET);


$qihooPay = new Qihoo_Pay($myApp);
$qihooPay->processRequest();

