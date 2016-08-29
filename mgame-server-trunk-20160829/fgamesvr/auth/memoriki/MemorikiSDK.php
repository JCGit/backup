<?php
/*
 * MemorikiSDK class to handle decode and encode of signed request & Send data back to our payment servers
 * @author Lawrence Tsang <lawrence@memoriki.com>
 */
class MemorikiSDK {

	private static $sandBox = false;													//@var boelean default false, only turn on when testing
	private static $payment_url = "http://payment-m.memoriki.com/payment/verify";		//@var string url for production payment
	private static $test_url = "http://payment-test.memoriki.com/payment/verify";		//@var string url for sandbox mode test payment
	private static $curlInfo;															//@var String contains info from curl request
	
	/*
	 * Used to encode a new signedRequest before sending back to our payment server
	 */
	public static function encodeSignedRequest($params,$secret) {
        if(!isset($secret))     return false;
        if(!isset($params['algorithm'])) $params['algorithm'] = "HMAC-SHA256";
        $payload = self::base64_url_encode(json_encode($params));
        $sig = self::base64_url_encode(hash_hmac('sha256', $payload, $secret, $raw = true));
        return $sig.".".$payload;
    }

	/*
	 * Decodes the signed Request we sent
	 */
    public static function decodeSignedRequest($signed_request, $secret) {
        if(!isset($secret))		return false;
        list($encoded_sig, $payload) = explode('.', $signed_request, 2);

        // decode the data
        $sig = self::base64_url_decode($encoded_sig);
        $data = json_decode(self::base64_url_decode($payload), true);
        if (strtoupper($data['algorithm']) !== 'HMAC-SHA256') {
            echo('Unknown algorithm. Expected HMAC-SHA256');
            return null;
        }
        if(isset($data['algorithm']))   unset($data['algorithm']);
        // check sig
        $expected_sig = hash_hmac('sha256', $payload, $secret, $raw = true);
        if ($sig !== $expected_sig) {
            echo('Bad Signed JSON signature!');
            return null;
        }
        return $data;
    }

    private static function base64_url_decode($input) {
        return base64_decode(strtr($input, '-_', '+/'));
    }

	private static function base64_url_encode($input) {
        return strtr(base64_encode($input), '+/', '-_');
    }

	public static function setSandBox($status) {
		if($status) {
			self::$sandBox = true;
		}
	}
	
	/*
	 * Notify our payment server once the coins/items have been transfered to user
	 */	
	public static function notifyPaymentServer($appId, $transactionId, $snid, $secret) {
		if(!isset($appId))      		return false;
		if(!isset($transactionId))		return false;
		if(!isset($snid))				return false;
		if(!isset($secret))      		return false;

		$payload = array(
						'error' 		=> false,
						'transactionId' => $transactionId,
		);
		$signedRequest = self::encodeSignedRequest($payload, $secret);
		$transfered = 1;
		$params = array(
					'appId'			=> $appId,
					'snid'			=> $snid,
					'transfer'		=> $transfered,
					'secret'		=> $secret,
					'signedRequest' => $signedRequest,
		);
		
		// During testing get the correct URL and call our API
		if(self::$sandBox) {
			$response = self::CurlReq(self::$test_url, $params);
		} else {
			$response = self::CurlReq(self::$payment_url, $params);
		}
	}
	
	/*
	 * Used to call our API
	 */
	private static function CurlReq($url, $params=array(), $method='GET') {
        $ch = curl_init();
        $method = strtoupper($method);
        if ($method == 'GET') {
            $sep = strpos($url, '?') === false ? '?' : '&';
            $url .= $sep . http_build_query($params);
        } else {
            curl_setopt($ch, CURLOPT_POST, true);
            curl_setopt($ch, CURLOPT_POSTFIELDS, $params);
        }

        curl_setopt($ch, CURLOPT_URL, $url);
        curl_setopt($ch, CURLOPT_RETURNTRANSFER, true);
        curl_setopt($ch, CURLOPT_FOLLOWLOCATION, true);
        curl_setopt($ch, CURLOPT_TIMEOUT, 10);
        curl_setopt($ch, CURLOPT_HTTPHEADER, array('Expect:'));
        $retry = 0;

        while ($retry < 3) {
            $response = curl_exec($ch);
            $info = curl_getinfo($ch);
            self::$curlInfo = $info;
            if ($info['http_code'] == 200) break;
            $retry++;
        }

        curl_close($ch);
        return $response;
    }
}

?>
