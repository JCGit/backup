<?php

date_default_timezone_set('Asia/Shanghai');
require_once("redis.php");

class OrderFactory
{
	private $cach = null;
	private $suffix = "_61game";
	private $_OrderData = array();

	function __construct($suffix = '_61game', $config)
	{
		$this->cach = new RedisHelper($config);
		$this->suffix = $suffix;
	}

	//generate a orderid
	private function generateOrderId()
	{
		return intval(date('YmdHis') . microtime() * 1000) . $this->suffix;
	}

	//process param, then produce a order with based meta
	public function ProcessParam($param)
	{
		$money = $param['money'];
		$uin = $param['uin'];
		$shopid = $param['shopid'];

		if (empty($money) || empty($uin) || empty($shopid))
		{
			return false;
		}

		$this->_OrderData = array('Money' => intval($money),
			  					  'Time' => time(),
			  					  'Status' => "fail",
			  					  'ShopId' => intval($shopid),
			  					  'OrderId' => $this->generateOrderId(),
			  					  'Uin' => intval($uin),
		);
		return true;
	}

	//add extra order information
	public function AddExtraData($data = array())
	{
		foreach ($data as $key => $value) {
			# code...
			if (!isset($this->_OrderData[$key]))
				$this->_OrderData[$key] = $value;
		}
	}

	//save the order to the redis
	public function SaveOrder($table = "recharge_order")
	{
		$this->cach->HSet($table, $this->_OrderData['OrderId'], $this->_OrderData);
	}

	//获取订单数据到内存中
	public function LoadOrder($table = "recharge_order", $orderid)
	{
		$data = $this->cach->HGet($table, $orderid);
		if ($data) {//没获取到数据返回的是false
			$this->_OrderData = json_decode($data, true);
		}else {
			error_log("LoadOrder get empty " . $data . 'for orderid : ' . $orderid);
		}
	}

	//改变array的元素
	public function ChangeOrder($data = array())
	{
		foreach ($data as $key => $value) {
			# code...
			$this->_OrderData[$key] = $value;
		}
	}

	//检查订单状态是否为success
	public function CheckOrder($table = "recharge_order", $orderid)
	{
		return $this->cach->CheckOrder($table, $orderid);
	}

	//获取订单号
	public function GetOrderId()
	{
		if (!empty($this->_OrderData))
		{
			return $this->_OrderData['OrderId'];
		}
		return '';
	}

	//获取订单数据
	public function GetOrderData()
	{
		if (!empty($this->_OrderData))
		{
			return $this->_OrderData;
		}
		return null;
	}

	//判断是否合法的uin
	public function CheckUin($uin)
	{
		return $this->cach->Exist('account', $uin);
	}


}

