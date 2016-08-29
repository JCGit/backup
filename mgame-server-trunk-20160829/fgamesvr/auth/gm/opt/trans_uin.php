<?php

function trans_uin($uin)
{
    $map = array(
        "5" => "0",
        "4" => "1",
        "6" => "2",
        "7" => "3",
        "1" => "4",
        "0" => "5",
        "2" => "6",
        "3" => "7",
        "9" => "8",
        "8" => "9",
    );

    $len = strlen($uin);
    $trans = "";
    for($i = 0; $i < $len; $i ++){
        $trans = $trans . $map[$uin[$i]];
    }
    return (int)($trans) - 987654;
}

$uin = trans_uin($_GET['uin']);
$arr = array("uin" => $uin);
echo json_encode($arr);
?>
