<?php
/**
* 日志记录文件
* 注意 php-fpm 进程要以root用户启动 php-fpm.conf user : root  php-fpm -R
*/

date_default_timezone_set('Asia/Shanghai');
class Logger
{
	private $path = null;	//文件名称
	private $level = null;	//日志级别 1-trace 2-debug 3-info 4-error 
	public static  $MAX_SIZE = 0;

	function __construct( $name, $level = 1 )
	{
		$this->level = $level;
		$ps = strrpos($name, '.');
		if ( !$ps ) {
			$this->path = $name . date('Y-m-d') . '.log';
		}else{
			$this->path = substr($name, 0, $ps) . date('Y-m-d') . '.log';
		}
	}

	public static function getInstance($name, $level = 1)
	{
		return new Logger( $name,$level );
	}

    
	public function writeFile($str, $level = 1)
	{		
		if ($this->level <= $level) {
	        // 自动创建日志目录
	        $log_dir = dirname($this->path);
	        if (!is_dir($log_dir)) {
	            mkdir($log_dir, 0755, true);
	        }
	        //检测日志文件大小，超过配置大小则备份日志文件重新生成
	        if (is_file($this->path) && self::$MAX_SIZE <= filesize($this->path)) {
	            rename($this->path, dirname($this->path) . '/' . basename($this->path, '.log') . '-' . time() . '.log');
	        }
	        error_log( $str . "\n", 3, $this->path);
		} else {
			error_log("no need to recorde for level : " . $level);
		}
	}
}

Logger::$MAX_SIZE = 1024 * 1024 * 10; // 10M

?>