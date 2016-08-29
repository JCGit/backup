<?php

//用于生成签名的公钥和私钥
$priKey = 'MIICdQIBADANBgkqhkiG9w0BAQEFAASCAl8wggJbAgEAAoGBAKCKeO3hCro2Yfj5BciyFPe/7ETyt6Saj2oYta3XbpjDUDZO/H/WAViICUMT0jDqxEPkxEgkFuVLTvqozmMr+cwaLblxifvLNCYMU/Tab5vNRXSp6WvIA9rOCgjO+5iLwdSQcOXh+qsjUi5l5apZoGkE8+Y33lAoFSO5nCtCD7BtAgMBAAECgYBr4GCeSCgzNLPk30DZuYCJcjfLpOVuAEX6XxxF8otor2XI+I6HQECrQs4mer01DaxQivqyFz4jWdV6bdAxp600PrT+XMtY+7cMUKChEou9weMppfuB64CtTHxu4vmO0zCXgKdCkvRgYjrq7I7+wSEJIwMHVL1DJm0x1gk5dVCkAQJBAMxh6yw3q6DPQtrdHpCN5iubRCN3aUnFNjYCuN96ohaq1v/i3aB9yVtOkURc3OePvaOZWPZZlo3AYa7JEIStvIECQQDJFgiZ7Y+Lz5UCEeA17Rn9+PLvE1ZQJJJzNaWUGFO1MGnJcB9JS5EjOgenhzmvl8QKGOJinXpkcOpX9YhuVa3tAkB59iSkKkRcndHDURggIs0rUGgE0gkeYHTNHiq8ES4QYLoT0Il4cBdsSSIerVuVQw1jRurzdtqElDy2VH1q71IBAkB2KmyDiAaCskluHfMTvXE4vcKEm2htUBB/g1b54BHQt9JyfWDlQXLYsJEu8VgEx7p79IOUT9ZMj84mQjMaI19BAkALlhe9IjpJ+0L7rThXmCE/jX3Fz7ROdEZGORBYilVfJRUgMbr2i22fHdlvo97dB4xnG/7c+ZLyPYw+IbGmslvj';

$gmToken = 'http://127.0.0.1/token/magic.php?';
define('DEV', true);

$mysql = array(
    'host' => '127.0.0.1:3306',
    'user' => 'root',
    'pass' => '',
    'db'   => 'dev_user',
);

// 服务器部署列表
$server_list = array(
    'qq' => array("login" => '10.105.112.32',
                  "game" => array('10.105.118.6',
                                  '10.105.53.193')),
    'wx' => array("login" => '10.105.86.119',
                  "game"  => array('10.105.104.244',
                                   '10.105.62.246')),
    'test' => array("login" => '127.0.0.1',
                    "game"  => array('127.0.0.1')),
);

// $user = 'fgame';
// $pass = 'fgame@tx@61';
// $game_path = '/data/fgame/fgame';
$user = 'root';
$pass = 'root';
// $game_path = '/data/fgame/fgame/fgamesvr';
$game_path = '/home/yibin/trunk/fgamesvr';
$cmd_str = 'ps -eo pid,stat,cmd | grep -v grep |grep sky';

$sh_arr = array('config' => 'gameserver.sh', 
                'config.login' => 'loginserver.sh');


define("DATA_DIR", '../data');
define("EXTRA_FILE", '../../../www_9001/server.json');
// define("EXTRA_FILE", '../data/server.json');