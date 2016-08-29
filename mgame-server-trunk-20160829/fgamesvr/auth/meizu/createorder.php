<?php
$config = require_once(dirname(__FILE__) . "/../utils/config.php");
require_once(dirname(__FILE__) . "/../utils/util.php");
require_once(dirname(__FILE__) . "/../utils/redis.php");
require_once(dirname(__FILE__) . "/../utils/order.php");
require_once 'config.php';
// require_once 'log/logger.php';
// $log = Logger::getInstance($config["logger"]);

//有一个处理函数，来做这个的处理
// $AppId = 2839694; //应用开发者appid
// $Secretkey = "pJv4WMsIxI9rvRrbeae8jwnQJtpdrGnf"; //应用开发者apKey

$redis = new RedisHelper($config);

$Res = create_order($AppId,$Secretkey);


echo "" . print_r($Res, true);

/**
 * 此函数就是接收服务器那边传过来传后进行各种验证操作处理代码
 * @param int $AppId 应用Id
 * @param string $Secretkey 应用Secretkey
 * @return json 结果信息
 */
function create_order($AppId, $Secretkey){
    $product_id = trim($_POST['product_id']) === '' ? trim($_GET['product_id']) : trim($_POST['product_id']);
    $uid = trim($_POST['uid']) === '' ? trim($_GET['uid']) : trim($_POST['uid']);
    $buy_amount = trim($_POST['buy_amount']) === '' ? trim($_GET['buy_amount']) : trim($_POST['buy_amount']);
    $pay_type = trim($_POST['pay_type']) === '' ? trim($_GET['pay_type']) : trim($_POST['pay_type']);
    $uin = trim($_POST['ourUin']) === '' ? trim($_GET['ourUin']) : trim($_POST['ourUin']);

    date_default_timezone_set('Asia/Shanghai');  //这个有什么用
    $date = date('YmdHis');
    // $appid = trim($_POST['appid']) === '' ? trim($_GET['appid']) : trim($_POST['appid']);

    if (empty($product_id)) {
        // $log->writeFile("creatorder===== appid is null",true);
        //error_log("meizu creatorder=== product_id is null");
        error_log(FormatLogStr('meizu', 'creatorder', ERROR_0, " product_id is null"));
        echo '{"error":"3"}';
        return false;
    }

    if (empty($uid)) {
        // $log->writeFile("creatorder===== appid is null",true);
        //error_log("meizu creatorder=== uid is null");
        error_log(FormatLogStr('meizu', 'creatorder', ERROR_0, " uid is null"));
        echo '{"error":"3"}';
        return false;
     }

    $cp_order_id = intval($date.microtime() * 1000) ;
    $order_id = $cp_order_id . "_meizu";

    //返回成功结果
    $Result["app_id"] =  $AppId;
    $Result["cp_order_id"] = $cp_order_id;
    $Result["create_time"] =  intval($date.microtime());
    $Result["pay_type"] =  0;   // 0 表示固定金额， 1表示用户可修改金额
    $Result["product_id"] =  $product_id;        

    //$product_stat = $config["moneyMap"][$product_id];
    global $moneyMap;
    $product_stat = $moneyMap[$product_id];
    if (!$product_stat)
    {
        //error_log("meizu creatorder=== moneyMap product_id not exist");
        error_log(FormatLogStr('meizu', 'creatorder', ERROR_0, " moneyMap $product_id not exist"));
        echo '{"error":"4"}';
        return false;
    }

  
    $product_item = json_decode($product_stat, true);
    $diamond = $product_item["diamond"];
    $price = $product_item["price"];

    $Result["product_subject"] =  "购买" . $diamond ;
    $Result["product_body"] =  $price . "个钻石";
    $Result["buy_amount"] =  $buy_amount;
    $Result["product_per_price"] =  $price;
    $Result["product_unit"] =  1;
    $Result["total_price"] =  $price;
    $Result["uid"] =  $uid;
    $Result["user_info"] =  $uin;
    
    //遍历Result，然后做处理
    ksort($Result);
    $sb = "";
    while($key= key($Result)) {
        //echo $key."<br />";
        $value = $Result[$key];
        $sb = $sb . "&" . $key . "=" . $value;
        next($Result);
        //每个key()调用不会推进指针。为此要使用next()函数
    }

    $sb = substr($sb, 1);
    
    $sign_text = $sb . ":" . $Secretkey;
    $sign_code = md5($sign_text);

    $Result["sign_type"] =  "md5";   // sign_type, sing 不计入md5加密内容中
    $Result["sign"] =  $sign_code;

    //error_log("meizu sign " . $sb);

    $Ret["code"] = 200;
    $Ret["value"] = $Result;


    //save to redis
    //get a conn
    global $redis;
    //$redis = new  RedisHelper($config);
    $data = array('Time' => time(),
              'Status' => "fail",
              'ShopId' => intval($product_id),
              'OrderId' => $order_id,
              'Uid' => intval($uid),
              'Uin' => intval($uin),
    );

    if (!$redis->HSet("recharge_order", $order_id, $data)) {
        // $log->writeFile("creatorder=====  save to redis failed",true);
        //error_log("creatorder=====  save to redis failed");
        error_log(FormatLogStr('meizu', 'creatorder', ERROR_0, " save to redis failed"));
        echo '{"error":"4"}';
        return;
    }

    $Res = json_encode($Result);
    return urldecode($Res); 
}
?>
