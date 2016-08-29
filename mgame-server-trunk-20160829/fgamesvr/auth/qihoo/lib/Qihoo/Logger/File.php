<?php

class Qihoo_Logger_File extends Qihoo_Logger_Base
{

    private $_logPath = QIHOO_MSDK_LOG;

    public static function getInstance()
    {
        return new self;
    }

    public function setLogPath($logPath)
    {
        $this->_logPath = $logPath;
    }

    protected function writeMsg($msg)
    {
        $logFile = $this->_logPath;
        $fp = fopen($logFile, 'a');
        $isNewFile = !file_exists($logFile);
        if (flock($fp, LOCK_EX)) {
            if ($isNewFile) {
                chmod($logFile, 0666);
            }
            fwrite($fp, $msg . "\n");
            flock($fp, LOCK_UN);
        }
        fclose($fp);
    }

}