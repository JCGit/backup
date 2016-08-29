<?php

define("OPT_STATE", "Status");
define("OPT_PAY_TIME", "PayTime");
define("OPT_SHOP_ID", "ShopId");

class OrderFactory
{

	private $redis;
	private $platform;
	private $platform_table;
	private $host;
	private $port;
	
	public function init($redis_conf, $platform)
	{
		$this->redis = new Redis();
		$con_ret = $this->redis->pconnect(
			$redis_conf["host"],
			$redis_conf["port"],
			$redis_conf["timeout"]);
		$this->platform = $platform;
		$this->host = $redis_conf["host"];
		$this->port = $redis_conf["port"];
		//$platform_table = $platform . "_orde";
		$this->platform_table = "recharge_order";

		if($con_ret == false)
		{
		}
		else
		{
			$this->redis->select($redis_conf["db_sel"]);

			$plat_exists = $this->redis->exists($this->platform_table);
			if(!$plat_exists)
			{
				$this->redis->hSet($this->platform_table, "table_exists_flag", "1");
			}
		}
		return $con_ret;
	}

	public function create_new_order()
	{
		do{

			$order = rand_order();
		}
		while($this->check_order_exist($order) != null);
		return $order;

	}

	protected function rand_order()
	{
		$cur_t = time();
		$cur_rand = rand();
		$order = $cur_t . $cur_rand;
		return $order;
	}

	public function insert_order($order)
	{
		$order = $this->get_platform_order($order);
		$objs = array(
			OPT_STATE => false);
		$objs_str = json_encode($objs);
		$this->redis->hSet($this->platform_table, $order, $objs_str);
	}

	public function check_order_exist($order)
	{
		return $this->redis->hExists($this->platform_table, $this->get_platform_order($order));
	}

	public function set_order_opt($order, $opt, $val)
	{
		if(!$this->check_order_exist($order))
			return;

		$order = $this->get_platform_order($order);

		$value = $this->redis->hGet($this->platform_table, $order);
		if($value == null)
			return;
		else
		{
			$objs = json_decode($value, true);
			$objs[$opt.""] = $val;
			$objs_str = json_encode($objs);
			$this->redis->hSet($this->platform_table, $order, $objs_str);
		}
	}

	public function get_order_opts($order)
	{
		$order = $this->get_platform_order($order);
		$val_str = $this->redis->hGet($this->platform_table, $order);
		return json_decode($val_str, true);
	}

	public function get_order_opt($order, $opt)
	{
		$objs = $this->get_order_opts($order);
		return $objs[$opt];
	}

	public function check_order_proc($order)
	{
		return (bool)($this->get_order_opt($order, OPT_STATE));
	}

	public function check_and_proc_order($order)
	{
		$is_proc = $this->check_order_proc($order);
		if($is_proc == false)
		{
			$this->set_order_opt($order, OPT_STATE, "1");
			$this->set_order_opt($order, OPT_PROC_T, (string)(time()));
		}
		return $is_proc;
	}

	public function get_platform_order($order)
	{
		return $order . "_" . $this->platform;
	}

	public function get_order_no_platform($plat_order)
	{
		$sp_arr = explode($plat_order);
		return $sp_arr[1];
	}

	public function close()
	{
		$this->redis->close();
	}
}

?>
