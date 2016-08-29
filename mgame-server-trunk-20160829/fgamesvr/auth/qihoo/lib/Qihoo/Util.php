<?php

class Qihoo_Util
{

    const METHOD_GET = 'GET';
    const METHOD_POST = 'POST';
    
    const USERAGENT = 'MSDK_PHP_v0.0.5(20140218)';

    private static $err;

    public static function requestUrl($url)
    {
        return file_get_contents($url);
    }

    public static function getSign($params, $appSecret)
    {
        unset($params['sign']);
        unset($params['sign_return']);

        $processedParams = array();
        foreach ($params as $k => $v) {
            if (empty($v)) {
                continue;
            }

            $processedParams[$k] = $v;
        }
        ksort($processedParams);
        $signStr = join('#', $processedParams) . '#' . $appSecret;
        return md5($signStr);
    }

    private static $_followRedirect = false;

    public static function setFollowRedirect($val)
    {
        self::$_followRedirect = $val;
    }

    public static function request($url, $mode, $params = '', $needHeader = false, $timeout = 10)
    {
        self::$err = null;
        $ch = curl_init();
        curl_setopt($ch, CURLOPT_TIMEOUT, $timeout);
        curl_setopt($ch, CURLOPT_RETURNTRANSFER, true);
        curl_setopt($ch, CURLOPT_USERAGENT, self::USERAGENT);
        curl_setopt($ch, CURLOPT_CAINFO, dirname(__FILE__) . '/cacert.pem');

        if (self::$_followRedirect) {
            curl_setopt($ch, CURLOPT_FOLLOWLOCATION, true);
        }
        if ($needHeader) {
            curl_setopt($ch, CURLOPT_HEADER, true);
        }

        if ($mode == 'POST') {
            curl_setopt($ch, CURLOPT_HTTPHEADER, array('Expect:'));
            curl_setopt($ch, CURLOPT_POST, true);
            if (is_array($params)) {
                curl_setopt($ch, CURLOPT_POSTFIELDS, http_build_query($params));
            } else {
                curl_setopt($ch, CURLOPT_POSTFIELDS, $params);
            }
        } else {
            if (is_array($params)) {
                $url .= (strpos($url, '?') === false ? '?' : '&') . http_build_query($params);
            } else {
                $url .= (strpos($url, '?') === false ? '?' : '&') . $params;
            }
        }
        curl_setopt($ch, CURLOPT_URL, $url);

        $result = curl_exec($ch);

        if ($needHeader) {
            $tmp = $result;
            $result = array();
            $info = curl_getinfo($ch);
            $result['header'] = substr($tmp, 0, $info['header_size']);
            $result['body'] = trim(substr($tmp, $info['header_size']));  //直接从header之后开始截取，因为 1.body可能为空   2.下载可能不全   
            //$info['download_content_length'] > 0 ? substr($tmp, -$info['download_content_length']) : '';
        }

        $errno = curl_errno($ch);
        if ($errno) {
            self::$err = array(
                'errno' => $errno,
                'error' => curl_error($ch),
            );
        }

        curl_close($ch);
        return $result;
    }

    public static function getError()
    {
        return self::$err;
    }

}

