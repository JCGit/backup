<?php

abstract class Qihoo_Logger_Base
{

    public static function getInstance()
    {
        return new self;
    }

    public function log($location, $message)
    {
        $now = date('Y-m-d H:i:s');
        $msg = sprintf("%s %s %s", $now, $location, $message);
        return $this->writeMsg($msg);
    }

    protected function writeMsg($msg)
    {
        return false;
    }

}
