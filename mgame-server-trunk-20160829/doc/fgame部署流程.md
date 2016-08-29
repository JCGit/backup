
##fgame部署流程

###CentOS环境配置

#### 安装gcc/g++
* yum install gcc
* yum install gcc-c++

#### 安装mysql
* yum install mysql-server
* yum install mysql-devel
* service mysqld restart
* 在云环境下仅安装客户端: yum install mysql

#### 安装redis[安装redis3.0]
* yum install redis
* service redis restart

#### 从源码安装redis
* wget http://download.redis.io/releases/redis-3.0.7.tar.gz
* tar -zx -f redis-3.0.7.tar.gz
* cd redis-3.0.7
* make && make test && make install

#### 安装protobuf 2.6
* tar zxvf protobuf.tar.gz
* ./configure
* make -j8 //八核编译
* make check
* make install

### 服务器部署流程

#### 编译程序
* make linux [skynet]
* make #第三方库编译成so文件 [*.so]

#### 打包程序
* bash release/pack.sh

#### 传文件到服务器
* scp fgame.tar.gz root@ip:/root/fgame

#### 解压文件
* tar -zxvf fgame.tar.gz

#### 停服
* sh fgame/stop_game.sh
  注意:一定要等脚本执行完

#### 启动
* sh fgame/start/start_game.sh

```
注意：服务器的server_config.lua需要在开服的时候备份一份 以后线上修改也一定要在bak中修改
```

### QQ和微信部署流程的区别
* 部署的时候 server_config中的config.platform 赋值为 "qq" 或者 "weixin"
* initdb.sh 支持带一个平台参数 "qq" 或者 "weixin"


