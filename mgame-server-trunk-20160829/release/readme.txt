 * release.sh
       发布代码的主文件, 提供参数 copy 或者 deploy, copy 将仅仅只拷贝文件到对应
   的服务器上的 /data/fgame/release/archive 目录中; deploy 将执行远程服务器中的
   /data/fgame/release/deploy.sh 文件执行部部署; 在执行过程中会要求输入对应的服务索引,
   索引从 0 开始, 如果提供 a 则表示部署所有外网服务器;

 * conf.sh
       其它文件会使用到的配置信息, 主要是服务器列表, 他们的 ip, 用户, 以及项目路径,
   如果有多个路径则用 ; 分割, deploy.sh 文件会分别部署到对应的多个目录中去;
   OUTER_MASTER 节点, 描述从本地开发机先上传到此节点, 再由此节点分发到其它的节点, 分发
   过程是按照配置, 依次往下分发的, 如果有特定的启动顺序则需要相应调整 SERVER_LIST 的顺序;

 * deploy.sh
        在服务器上调用, 目录必须是在 /data/fgame/release 中, 调用时仅需要传递项目
   路径, 脚本会从 /data/fgame/release/archive 中查找最新的 tar 文件, 并且会备份项目到
   /data/fgame/release/bak 目录中. 并且关闭服务进程, 然后部署完之后, 再开启服务进程;

 * pack.sh
        将会编译所有的二进制文件, 并且将它们打包到 build 目录中, 部署完之后不会被
   删除, 等到下一次部署时才会删除上一次的 build 文件;
 
 * remote_deploy.sh
        用于将生成好的 build 中的 tar 文件包, 分发到远程服务器. 一般情况下,
   不需要理会此文件. 次文件接收 3 个参数, $1={copy|deploy} , $2={a|0|1|2..}, $3=[file_name]
   当为 copy 时表示复制文件到远程服务器, 远程服务器由 $2 指定, $3 指定上传的文件.
   当为 deploy 时表示部署服务器, 远程服务器由 $2 指定, 不必提供 $3 .
