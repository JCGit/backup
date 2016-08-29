<?php

define("LOG_TABLE_NAME", "simple_log");

class SimpleLog
{
	private $redis = null;
	private $isInit = false;
	private static $instance = null; 
	private $host;
	private $port;

	public $on_off = true;

	public static function getInstance()
	{
		if (is_null(self::$instance))
		{
			self::$instance = new SimpleLog();
		}
		return self::$instance;
	}

	public function on_off_set($value)
	{
		$this->on_off = $value;
	}

	public function init($redis_conf)
	{
		if(!$this->on_off)
			return;

		$this->redis = new Redis();
		$con_ret = $this->redis->connect(
			$redis_conf["host"],
			$redis_conf["port"],
			$redis_conf["timeout"]);

		$this->host = $redis_conf["host"];
		$this->port = $redis_conf["port"];

		$this->isInit = $con_ret;
		if($con_ret)
			$this->redis->select($redis_conf["db_sel"]);
		return $con_ret;
	}

	public function writeFile($content)
	{
		if($this->isInit)
			$this->redis->hset(LOG_TABLE_NAME, time().rand(), $content);
	}

	public function close()
	{
		if($this->isInit)
			$this->redis->pconnect($this->host, $this->port);
	}

}

?>
