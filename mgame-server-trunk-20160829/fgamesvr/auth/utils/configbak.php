<?php

return array(
    'center' => 'http://127.0.0.1:8602', //通知中心服务器的地址
	"logger" => '/data/fgame/61game/order/order.log', //记录所有充值成功的日志
	"redis"  => array(
					"host" => "10.10.128.49",
					"port" => "6379",
					"user" => "root",
					"pass" => "",
					"timeout" => 10),
    "shopMap" => array(	24923=>1, 
					24920=>2,
					24921=>3,
					24922=>4,
					24924=>5,
					24925=>6,
					24926=>7,
					24927=>8,
					24928=>9,
					24929=>10),
	"moneyMap" => array( 1 => '{"diamond":228, "price":20}',
		2 => '{"diamond":50, "price":5}',
		3 => '{"diamond":108,  "price":10}',
		4 => '{"diamond":168,  "price":15}',
		5 => '{"diamond":228,  "price":25}',
		6 => '{"diamond":368,  "price":29}',
		7 => '{"diamond":868,  "price":68}',
		8 => '{"diamond":1688,  "price":128}',
		9 => '{"diamond":4268,  "price":328}',
		10 => '{"diamond":8888,  "price":648}',
		),
	);