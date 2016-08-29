<?php

//center 服务器信息
$cent_svr_config = array(
    "host" => "127.0.0.1",
    "port" => 8602);

$platform_name = "memoriki";

//redis 配置
$redis_config = array(
    "host" => "127.0.0.1",
    "port" => 6379,
    "timeout"=> 10,
    "db_sel" => 0);

$shop_mapping = array(
    "foodwars_gold_1" => 3,
    "foodwars_gold_2" => 4,
    "foodwars_gold_3" => 5,
    "foodwars_gold_4" => 6,
    "foodwars_gold_5" => 7,
    "foodwars_gold_6" => 8,
    "foodwars_gold_7" => 9,
    "foodwars_gold_14" => 1,
    "foodwars_gold_8" => 9,
    "foodwars_gold_9" => 10,
    "foodwars_gold_10" => 11,
    "foodwars_gold_11" => 12,
    "foodwars_gold_12" => 13,
    "foodwars_gold_13" => 14,
    "foodwars_gold_15" => 1,
    "foodwars_gold_16" => 101,
    "foodwars_gold_17" => 102,
    "foodwars_gold_18" => 103,
    "foodwars_gold_19" => 101,
    "foodwars_gold_20" => 102,
    "foodwars_gold_21" => 103,
    "foodwars_gold_22" => 2,
    "foodwars_gold_23" => 201,
    "foodwars_gold_24" => 202,
    "foodwars_gold_25" => 203,
    "foodwars_gold_26" => 204,
    "foodwars_gold_27" => 205,
    "foodwars_gold_28" => 206,
    "foodwars_gold_29" => 207,
    "foodwars_gold_30" => 208,
);

$app_id = "10094";
$secret = "e950a75f4775f1dcb0598168eb8ae656";  //密钥

//测试沙箱：正式上线一定要改成false
$sendBox = false;
//输出log开关
$log_offon = false;
$log_path = "/mount/61sdklog/61game_sdk.log"

?>
