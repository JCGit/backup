<?php

/**
 * 客户端调用本接口能通过access_token获取用户信息
 * 
 * 1.使用access_token获取用户信息
 * 请求地址格式为
 * http://{YOUR_SERVER_NAME}/{YOUR_PATH}/user.php?act=get_user&app_key={YOUR_APP_KEY}&token={ACCESS_TOKEN}
 * 类似:
 * http://your.server.com/user.php?act=get_user&app_key=8689e00460eabb1e66277eb4232fde6f&token=182717763ce4777a9839765fdebf194bf7c51e60b816bb8cf
 * 返回:
 * {
  "id": "182717763",
  "name": "adamsunyu",
  "avatar": "http://u1.qhimg.com/qhimg/quc/48_48/29/02/73/290273aq114f3.92d56f.jpg?f=8689e00460eabb1e66277eb4232fde6f"
  }
 */
require_once dirname(__FILE__) . '/common.inc.php';
/**
 * 
 */
$scope = empty($_REQUEST['scope']) ? '' : $_REQUEST['scope'];
$qihooOauth2 = new Qihoo_OAuth2(QIHOO_APP_KEY, QIHOO_APP_SECRET, $scope);
if (QIHOO_MSDK_DEBUG) {
    //打开调试日志
    $logger = Qihoo_Logger_File::getInstance();
    //设置日志路径
    $logger->setLogPath(QIHOO_MSDK_LOG);
    $qihooOauth2->setLogger($logger);
}

header('Content-Type: application/json; charset=utf-8');
try {
    $act = isset($_REQUEST['act']) ? $_REQUEST['act'] : 'get_user';
    $data = processRequest($qihooOauth2, $act);
    echo json_encode($data);
} catch (Qihoo_Exception $e) {
    echo json_encode(array(
        'error_code' => $e->getCode(),
        'error' => $e->getMessage(),
    ));
}

/**
 * 
 * @param Qihoo_OAuth2 $qihooOauth2
 * @return array
 */
function processRequest($qihooOauth2, $act)
{
    switch ($act) {

        //用token获取用户信息
        case 'get_user':
            $token = isset($_GET['token']) ? $_GET['token'] : '';
            if (empty($token)) {
                throw new Qihoo_Exception(Qihoo_Exception::CODE_NEED_TOKEN);
            }
            $userArr = $qihooOauth2->userMe($token);
            return $userArr;
        default:
            throw new Qihoo_Exception(Qihoo_Exception::CODE_BAD_PARAM);
    }
}
