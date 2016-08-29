<?php


require "magic_cfg.php";
require(dirname(__FILE__) . "/../util.php");


$str = 'gmtype=redis_opt&opt=hget_obj&param1=account&param2=1000099&param3=test&platform=test';

$sign = 'ndDY243mIkWk8FOafujI%2F4iMygg24ut7DB62ZhL4652m%2Fsgmui6OXj1QRAzFbW9%2BEH%2BkEMDEF8wmNWrdiYoNwaayV9KbsbFm8z4ay0uWGt%2FfllqaLLg%2Bzx9zkTjLGhZwiWq%2BAkYQr6Jmq3OTIvgsdo57dreQ5Wq%2B2ZSIXM4%2BHvA%3D';


if (verify($str, $sign, $pubKey)){
    echo 'true';
}else{
    echo 'false';
}