<?php

/**
* redis助手类
*/
class RedisHelper
{
	private $redis = null; //redis instance

	function __construct($config = array())
	{
		$this->redis = new Redis();
		if($this->redis->connect($config["redis"]["host"],$config["redis"]["port"],$config["redis"]["timeout"]) === false){
			error_log("get redis conn failed");
			throw new Exception("get redis conn failed"); 
		}
		if (isset($config['redis']['auth'])){
			$this->redis->auth($config['redis']['auth']);
		}
	}

	//hash get
	public function HGet($table="recharge_order", $orderid)
	{
		return $this->redis->hGet($table, $orderid);
	}

	//hash set
	public function HSet($table="recharge_order", $orderid, $param = array())
	{
	    return $this->redis->hSet($table, $orderid, json_encode($param));
	}

	//check order status(success) true
	public function CheckOrder($table, $orderid)
	{
		$param = $this->redis->hGet($table, $orderid);
		if ($param)
		{
			$param = json_decode($param, true);
			if ($param['Status'] === "success")
			{
				return true;
			}
		}
		return false;		
	}

	//回调没传内部uin,通过key到hash(uin_mapping)查找对应的Uin,存在则返回value,不存在false
	public function CheckUin($key)
	{
		return $this->redis->hGet('uin_mapping', $key);
	}

	//判断hash-key是否存在
	public function Exist($table = '', $key = '')
	{
		return $this->redis->hExists($table, $key);
	}

}

?>