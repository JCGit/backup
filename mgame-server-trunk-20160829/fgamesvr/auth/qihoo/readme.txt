1.user.php 提供用户相关接口给客户端。客户端传入code可获取用户信息和access_token
pay_callback.php为订单付款后，游戏方提供给360的服务端回调接口。

2.需要游戏方修改的地方:
 1) common.inc.php 需要修改$_keyStore数组，将游戏自身的app_key, app_secret加入配置
 2) 需要参考PayApp/Demo.php 实现服务端收到订单通知后，游戏内部处理逻辑。编写完成后需要替换pay_callback.php中PayApp_Demo类
 3) user.php中，使用code获取token后，可以将refresh_token保存起来，access_token过期后，使用refresh_token刷新access_token。

3.如何调试
  1) common.inc.php中修改
  define('QIHOO_MSDK_DEBUG', false);
  改成
  define('QIHOO_MSDK_DEBUG', true);
  便可以打开调试
  
  2)上线后切记将
  define('QIHOO_MSDK_DEBUG', true);
  改回
  define('QIHOO_MSDK_DEBUG', false);
  否则线上环境将会产生大量日志记录，占用磁盘空间
 
  3)日志路径
  windows 下日志路径为 当前路径下qihoo_msdk.log
  *nix等其它系统下日志路径为 /tmp/qihoo_msdk.log
  也可以在common.inc.php中修改 QIHOO_MSDK_LOG 的定义来修改日志文件路径。
  请确认web服务器程序有权限对日志文件有写入权限。
  