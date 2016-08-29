<?php

require_once __DIR__ . '/lib/SnsSigCheck.php';
require_once __DIR__ . '/lib/SnsSigCheck.php';

require "config.php";

$data = '{"amt":"600","appid":"1450005915","appmeta":"1000040*14*qdqb*qq","billno":"-APPDJSX38525-20160126-1739135208","channel_id":"73213123-android-73213123-qq-1105115982-67E87013295359D1E1C300EE6BC1230F-qq","clientver":"android","ebazinga":"1","openid":"67E87013295359D1E1C300EE6BC1230F","payamt_coins":"0","payitem":"14*60*1","providetype":"5","pubacct_payamt_coins":"","token":"0CCC5F261838D6EFF846A3EA60187C6213669","ts":"1453801153","version":"v3","zoneid":"1","sig":"T5YTSP1DZ6yfs\/XWBaI+64jWqLo="}';

$arr = json_decode($data, 1);

$sig = $arr['sig'];


SnsSigCheck::verifySig("GET", "/midas/callback.php", $arr, $appkey.'&', $sig);


