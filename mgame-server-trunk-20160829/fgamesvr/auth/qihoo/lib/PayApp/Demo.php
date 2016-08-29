<?php

require_once dirname(__FILE__) . "/../../config.php";
require_once dirname(__FILE__) . "/../../../utils/util.php";
require_once dirname(__FILE__) . "/../../../utils/redis.php";

/**
 * PayApp 演示如何编写使用360支付的app支付接口
 */
class PayApp_Demo implements PayApp_Interface
{

    //需要修改为应用自身的app_key
    private $_appKey ;
    //需要修改为应用自身的app_secret(服务器之间通讯使用)
    private $_appSecret ;
    //人民币-游戏货币的兑换比例
    private $_cashRate = 10;
    
    //redis实例
     private $redis = null;
    const REDIS_HASH = "recharge_order";

    //通知游戏服务器center
    // const CENTER_URL = "http://203.195.132.24:8602/qihoo/callback";

    public function __construct($appKey, $appSecret)
    {
        $this->_appKey = $appKey;
        $this->_appSecret = $appSecret;
        $this->getRedisInstance();
    }

    private function getRedisInstance()
    {
        $config = require(dirname(__FILE__) . "/../../../utils/config.php");
        $this->redis = new RedisHelper($config);
    }

    public function getAppKey()
    {
        return $this->_appKey;
    }

    public function getAppSecret()
    {
        return $this->_appSecret;
    }

    public function isValidOrder(array $orderParams)
    {
        if (!empty($orderParams['app_order_id'])) {
            //使用应用自身的订单号
            $orderId = $orderParams['app_order_id'];
        } else {
            //使用360支付的订单号
            $orderId = $orderParams['order_id'];
        }

        $order = $this->_getOrder($orderId);
        if (empty($order)) {
            return false;
        }

        //订单是否已经处理过
        //需要根据应用自身的数据表结构修改
        $orderProcessed = $order['Status'];
        if ($orderProcessed === "success") {
            return false;
        }

        return true;
    }

    private function _getOrder($orderId)
    {
        //应该根据360支付返回的订单号或者应用自身的订单号($order['app_order_id'])查询应用订单数据表
        $data = $this->redis->hGet(self::REDIS_HASH, $orderId);
        return json_decode($data, true);
    }

    public function processOrder(array $orderParams)
    {
        $order = $this->_getOrder($orderParams['app_order_id']);
        if (!$order){
            error_log(FormatLogStr('qihoo', 'callback', ERROR_NEXIST, $orderParams['app_order_id']));
            return false;
        }
        $config = include(dirname(__FILE__) . "/../../../utils/config.php");
        global $centerPath;
        $url = $config['center'] . $centerPath . '?uin=' . $order["Uin"] . '&orderId=' . $order['OrderId'] . "&shopId=" . $order['ShopId'];
        $res = $this->notifyCenter($url);
        if ($res === "success")//通知中心服务器
        {
            $log = GetLogger($config['logger']);
            $log->writeFile(FormatLogStr('qihoo', 'callback', ERROR_0, $url));
            //成功更新订单信息
            $order['Status'] = "success"; //更新订单状态为success
            $order['ExtOrderID'] = $orderParams['order_id'];//保存第三方的订单号
            $order['PayTime'] = time();
            $this->redis->hSet(self::REDIS_HASH, $orderParams['app_order_id'], $order);
            return true;
        }else{
            error_log(FormatLogStr('qihoo', 'callback', ERROR_CENTER, $url));
            return false;
        }
    }

    private function _updateOrder(array $orderParams)
    {
        //更新订单,标识为已经处理，避免重复处理
        //如果更新订单状态失败,记录异常，以便再次处理。再次处理的逻辑需应用自己处理
        $order = $this->_getOrder($orderParams['app_order_id']);
        $order['Status'] = "success"; //更新订单状态为success
        $order['ExtOrderID'] = $orderParams['order_id'];//保存第三方的订单号
        $order['PayTime'] = time();
        $this->redis->hSet(self::REDIS_HASH, $orderParams['app_order_id'], json_encode($order));
        return $order;
    }
    
    private function _getAmount(array $orderParams)
    {
        if (!empty($orderParams['is_sms'])) {
            //短信支付通知时，amount值不可靠，只能使用consumeCode
            $amount = 0;
            //TODO::根据$orderParams['pay_ext']['notify_data']['consumeCode'] 反推出正确金额。 注意amount的单位为分
        } else {
            $amount = $orderParams['amount'];
        }
        return $amount;
    }

    private function notifyCenter($url)
    {
        $ch = curl_init();
        curl_setopt($ch, CURLOPT_URL, $url);
        curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);    // 要求结果为字符串且输出到屏幕上
        curl_setopt($ch, CURLOPT_HEADER, 0); // 不要http header 加快效率
        curl_setopt($ch, CURLOPT_USERAGENT, 'Mozilla/5.0 (compatible; MSIE 5.01; Windows NT 5.0)');
        curl_setopt($ch, CURLOPT_TIMEOUT, 15);
        curl_setopt($ch, CURLOPT_SSL_VERIFYPEER, FALSE);    // https请求 不验证证书和hosts
        curl_setopt($ch, CURLOPT_SSL_VERIFYHOST, FALSE);
        $output = curl_exec($ch);
        curl_close($ch);
        return $output;
    }

    private function _addCash(array $orderParams)
    {
        //发货或者增加游戏中的货币
        //如果发货失败，记录异常，以便在再次处理。处理的逻辑需应用自己处理。
        //充值金额，以人民币分为单位。例如2000代表20元
        $amount = $this->_getAmount($orderParams);
        //兑换比例(人民币兑换游戏货币，$rate==10,表示1元人民币可兑换10游戏货币)
        $gameCashNum = $amount / 100 * $this->_cashRate;
        return true;
    }

}

?>
