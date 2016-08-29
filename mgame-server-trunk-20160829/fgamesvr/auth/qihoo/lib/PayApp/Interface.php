<?php

interface PayApp_Interface
{

    //返回app_key
    public function getAppKey();

    //返回app_secret
    public function getAppSecret();

    /**
     * 是否是合法的订单(订单是否存在，订单是否被处理过)
     * @param array $order
     * @return Boolean 订单是否需要处理 
     */
    public function isValidOrder(array $order);

    //处理订单，发货或者增加游戏中的游戏币
    public function processOrder(array $order);
}

?>
