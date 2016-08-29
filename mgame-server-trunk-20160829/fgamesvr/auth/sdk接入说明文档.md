
<link rel="stylesheet" href="http://yandex.st/highlightjs/6.2/styles/googlecode.min.css">
  
<script src="http://code.jquery.com/jquery-1.7.2.min.js"></script>
<script src="http://yandex.st/highlightjs/6.2/highlight.min.js"></script>
  
<script>hljs.initHighlightingOnLoad();</script>
<script type="text/javascript">
 $(document).ready(function(){
      $("h2,h3,h4,h5,h6").each(function(i,item){
        var tag = $(item).get(0).localName;
        $(item).attr("id","wow"+i);
        $("#category").append('<a class="new'+tag+'" href="#wow'+i+'">'+$(this).text()+'</a></br>');
        $(".newh2").css("margin-left",0);
        $(".newh3").css("margin-left",20);
        $(".newh4").css("margin-left",40);
        $(".newh5").css("margin-left",60);
        $(".newh6").css("margin-left",80);
      });
 });
</script>
<div id="category"></div>



## SDK支付接入说明

### 1.接入步骤

#### 1.1 接入示意图 

![支付](http://203.195.132.24/pay.png)


> 说明 

- 步骤**1,2**登陆授权，走的是游戏服务器
- 之后的步骤后台全是是独立出的php服务器(创建订单，订单支付通知，订单查询)

#### 1.2 php服务器功能简述

- 创建自己内部的订单(主要是生成订单基础数据)
- 处理订单支付支付通知(收到第三方支付平台回调后的逻辑处理1.签名验证 2.更新订单 3.通知游戏服务器下发商品)
- 订单查询(查看订单的状态是否成功)


-----

### 2.目录说明


#### 2.1 目录结构


```file

├── lenovo 										目录名称代码渠道
│   ├── callback.php 							用于处理支付回调的接口
│   ├── checkorder.php 							订单查询的接口
│   ├── check_test.php 							添加测试代码
│   ├── config.php 								针对这个平台的配置 appkey 公钥 私钥
│   ├── createorder.php 						创建订单的接口
│   ├── create_test.php
│   └── lib
│       └── lenovo_cashier_rsa.inc.php          第三方提供的库
└── utils 										所有渠道公用的一些配置工具类、函数
    ├── config.php 								公有的配置redis,日志目录等
    ├── logger.php 								提供的日志服务
    ├── logger_test.php 						
    ├── order_factory.php 						
    ├── order.php 								抽像的订单meta信息的类
    ├── redis.php  								redis助手类
    ├── rsa.php 								抽像用于rsa加解密的类
    └── util.php 								一些共有的函数

```

#### 2.2 新增渠道说明

- 在同以上示例与lenovo同级目录下创建对应的渠道名称(见名知意)
- 渠道下面的文件(也可以说是接口)命名可以遵循**createorder.php(创建订单)、callback.php(支付回调接口)、checkorder.php(查询订单接口)**
- lib目录放第三方依赖的库
- 每个渠道下面应该有自己的一些配置**(config.php)**存放对应的私有配置

#### 2.3 config.php示例

> 注意: 引用config.php文件后，直接使用文件里面的变量即可

```php

<?php
$appKey = '7b8763D5870B7feefFC7c0fed6dbf575';
$publicKey ='MIGfMA0GCSqGSIb3DQEBAQUAA4GNADCBiQKBgQCmreYIkPwVovKR8rLHWlFVw7YDfm9uQOJKL89Smt6ypXGVdrAKKl0wNYc3/jecAoPi2ylChfa2iRu5gunJyNmpWZzlCNRIau55fxGW0XEu553IiprOZcaw5OuYGlf60ga8QT6qToP0/dpiL/ZbmNUO9kUhosIjEu22uFgR+5cYyQIDAQAB';
$centerPath = '/oppo/callback';


```

-----

### 3.公共目录说明

> 注意: 把可以抽像的东西放到此目录下~


#### 3.1 配置说明

```php

<?php
return array(
	"logger" => '/root/61game_sdk.log',
	"redis" => array(
					"host" => "127.0.0.1",
					"port" => "6380",
					"user" => "root",
					"pass" => "",
					"timeout" => 10)
	);

```

**
<font color="red">
日志文件配置后,php-fpm进程的拥有者一定要对日志文件所在的目录有读写权限
example:
修改php-fpm.conf里面的user : root
配置后不能service php-fpm start
记住，记住，记住 php-fpm -R
最要的事情说三遍~~
</font>
**


*redis配置是全局的，不要单独配置*

------


#### 3.2 日志格式说明

> 统一日志格式方便查阅日志

- 日志记录有两种<u>(1.充值成功记录**记录在logger配置下面** 2.接口逻辑日志处理会放在php-fpm默认日志记录地址php-fpm.conf=>error_log配置)</u>
- 日志格式使用util.php里面的`FormatLogStr($plateform = '61game', $type = 'callback', $error = 0, $str)`函数

**_输出日志的时候尽量记录详细些_**

#### 3.3 日志错误定义

```php

define("ERROR_0", 0);
define("ERROR_UIN", 1);//获取uin失败
define("ERROR_SIGN", 2);//签名错误
define("ERROR_CENTER", 3);//通知中心服务失败
define("ERROR_EXIST", 4);//订单已经存在且 Status:success
define("ERROR_PARAM", 5);//url参数错误
define("ERROR_THIRD", 6);//url参数错误
define("ERROR_PAY", 7); //收到支付不成功的订单
define("ERROR_NEXIST", 8);//订单不存在


```

> 日志记录示例

```php

error_log(FormatLogStr('oppo', 'callback', ERROR_SIGN, $toSign . "&sign=$sign"));
//逻辑错误日志调用error_log(...)
$log->writeFile(FormatLogStr('oppo', 'callback', ERROR_0, $url . ' ExtOrderId : ' . $cache['ExtOrderId']));
//通知到中心服务器日志用内部日志模块 $log = GetLogger($config['logger']);

```


**
<font color="red">
在支付回调处理逻辑后，用http请求通知中心服务(游戏服务器)的处理逻辑成功返回success以方便callback.php处理后续逻辑
</font>
**


### 4. 订单存储说明

**订单存在redis的hash结构里的recharge_order表中**
**hash的key的结构是YmdHis+毫秒(三位数)_平台 例如 : ```20151208114123785_jinli```**


#### 4.1 订单基本组成部分


```json

{
"Money" : 5,                            //订单价格(元)
"Time" : 1450081377,                    //订单创建时间
"Status" : "fail",                      //订单支付状态
"ShopId" : 2,                           //商品
"OrderId" : "20151208114123785_jinli",  //内部生成的订单号
"Uin" : 1000004,                        //uin
"PayTime" : 1450081377,                 //支付时间
"ExtOrderId" : "201512081141237"        //第三方订单号
}

```


### 5. 环境搭建

**以下安装环境是针对生产环境56来说明，其它测试环境安装即可，配置文件不需要关心**
**只需关注nginx和php-fpm的日志目录就行.默认日志文件/var/log/nginx|php-fpm**

#### 5.1 nginx安装


##### 5.1.1 添加软件安装源

- wget http://www.atomicorp.com/installers/atomic
- sh ./atomic

##### 5.1.2 安装开发包和库文件　

- `yum -y install ntp make openssl openssl-devel pcre pcre-devel libpng libpng-devel libjpeg-6b libjpeg-devel-6b freetype freetype-devel gd gd-devel zlib zlib-devel gcc gcc-c++ libXpm libXpm-devel ncurses ncurses-devel libmcrypt libmcrypt-devel libxml2 libxml2-devel imake autoconf automake screen sysstat compat-libstdc++-33 curl curl-devel`
- `yum install nginx`
- `service nginx start`
- `chkconfig --levels 235 nginx on` #设2、3、5级别开机启动


#### 5.2 php安装

##### 5.2.1 安装开发包和库文件　 

- `yum install php lighttpd-fastcgi php-cli php-mysql php-gd php-imap php-ldap php-pear php-xml php-xmlrpc php-mbstring php-mcrypt php-mssql php-snmp php-soap`
- `yum install  php-tidy php-common php-devel php-fpm php-mysql`
- `service php-fpm start`
- `chkconfig --levels 235 php-fpm on`

##### 5.2.2 安装redis扩展

- 下载安装包  [phpredis](https://github.com/phpredis/phpredis)
- 解压目录用phpize生成configure配置文件
- ./configure --with-php-config=php-config(指定些配置的目录)
- make & make install
- 在/etc/php.ini配置文件最后新增 extension="redis.so" 配置

#### 5.3 配置nginx支持php

```conf
# mv /etc/nginx/nginx.conf /etc/nginx/nginx.confbak
//将配置文件改为备份文件
 
# cp /etc/nginx/nginx.conf.default /etc/nginx/nginx.conf
//由于原配置文件要自己去写因此可以使用默认的配置文件作为配置文件
 
//修改nginx配置文件，添加fastcgi支持
# vi /etc/nginx/nginx.conf
index index.php index.html index.htm;
//加入index.php
 
location ~ \.php$ {
            #root           /usr/share/nginx/html; #这里改成自己的root文件
            fastcgi_pass   127.0.0.1:9100;
            fastcgi_index  index.php;
            fastcgi_param  SCRIPT_FILENAME  $document_root$fastcgi_script_name;
            include        fastcgi_params;
        }
//将以上代码注释去掉，并修改成nginx默认路径
```



#### 5.4 配置说明

#### 5.4.1 nginx配置修改

**nginx默认配置路径在/etc/nginx**

- 修改启动端口为8604
- 修改nginx默认启动用户为fgame在**/etc/nginx/nginx.conf**的user属性
- 修改nginx配置文件错误日志及访问日志文件目录改成**/data/fgame/61game/nginx**
- 在nginx配置目录下面**/etc/nginx/conf.d**新增proxy.conf用为正向代理https,**一定得修改resolver的地址**
- 修改nginx的root配置在**/etc/nginx/conf.d/default.conf**为**/data/fgame/auth**

#### 5.4.2 php配置修改

**php默认配置路径在/etc/php.ini | /etc/php-fpm.conf | /etc/php-fpm.d/下面**

- **/etc/php-fpm.conf**修改php的默认错误日志及访问日志文件目录改成**/data/fgame/61game/php-fpm**
- **/etc/php-fpm.d/www.conf**修改php工作进程的用户的user和group属性改成fgame用户
- **/etc/php-fpm.d/www.conf**及修改php进程的工作模式是动态还是静态的(默认动态)，可自己行择配置子进程的个数.
- **/etc/php-fpm.d/www.conf**修改php进程的监听端口的listen属性**nginx配置的时候要保持一致**

#### 5.4.3 配置好后重新启动示例

- `service nginx reload`
- `service php-fpm reload`

**启动用root用户启动**
**下面是一些日志及pid文件存放目录，为了方便查询放在/data/fgame/61game目录下**
**修改配置文件只需`service nginx | php-fpm reload`即可**


```file
├── nginx                               存放nginx日志的目录
│   ├── access.log                      访问nginx日志记录                
│   └── error.log                       访问nginx的错误日志
├── order                               订单的日志
│   └── 61game_order2015-12-23.log      充值成功的订单记录
└── php-fpm                             存放php进程的日志
    ├── access.log                      访问nginx日志记录
    └── error.log                       存放php进程的错误日志            
```


### 6. 请求url示例

- 创建订单

`
http://203.195.132.24/lenovo/createorder.php?appid=1510300252699.app.ln&lpsust=ZAgAAAAAAAGE9MTAwNjQzMzA1MTkmYj0yJmM9NCZkPTE1ODI4JmU9RDAxRjI4QjI2MTEyMTkyQTY4Mjg0RTQyRUIxQzMxQjQxJmg9MTQ1MDQzMjkwMjkxOCZpPTQzMjAwJmo9MCZvPTg2NjA0ODAyOTEzOTMxMCZwPWltZWkmcT0wJnVzZXJuYW1lPTEzNDI0MjA4NjE5Jmx0PTEmaWw9Y25YCqrBlZ12artHm58vhu5u&waresid=24923&money=10
`

- 支付回调

`
http://203.195.132.24/lenovo/callback.php?transdata={"transtype":0,"result":0,"transtime":"2015-12-23 13:33:53","count":1,"paytype":5,"money":600,"waresid":35921,"appid":"1510300252699.app.ln","exorderno":"151223133311175","feetype":0,"transid":"2151223133353379759137035","cpprivate":"10001808"}&sign=JmHXnEBCGsHsgjHp7PsDGUrgojoORVKfRTWhghwEEVj1jJwQnv2RWnO1BiazOLEFFePpNcLOs8vdQxCjW8PKnylLPnj7tnWWINX9cj1ZRTEjKHC0mp0nWDQZ48k0MtfRZEu0pVVKXveYFOQ1ry//LbGxOtdNgrXLjzhm/bMKzXg=
`

- 查询订单

`
http://203.195.132.24/lenovo/checkorder.php?exorderno=151223133311175
`


### 回调地址

- oppo的回调地址是客户端指定的 