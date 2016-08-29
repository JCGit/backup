<?php

class Qihoo_Exception extends Exception
{

    const CODE_NET_ERROR = '999';
    const CODE_JSON_ERROR = '998';
    const CODE_NO_APPKEY = '997';
    const CODE_NO_SECRET = '996';
    const CODE_BAD_PARAM = '995';
    const CODE_NEED_TOKEN = '994';
    const CODE_NEED_CODE = '993';

    private static $_MESSAGE_MAP = array(
        '999' => '访问远程接口失败。',
        '998' => 'JSON解析失败，原始串：',
        '997' => '请填写app_key',
        '996' => '请填写app_secret',
        '995' => '请检查传入参数,需要传入act参数，并且值为get_token_info,get_user,get_info中的一种',
        '994' => '请传入token参数',
        '993' => '请传入code参数',
        '4000203'=>'app_key或者app_secret不正确,请检查',
    );

    public function __construct($code, $message='')
    {
       if($code=='4000203')
       {
           $message = '';
       }
       
       if (isset(self::$_MESSAGE_MAP[$code])) {
            $message = self::$_MESSAGE_MAP[$code].$message;
        }
        parent::__construct($message, $code);
    }

}
