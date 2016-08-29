## start目录, 里边包含了 start_game.sh, stop_game.sh, restart_game.sh 文件用于启动，关闭和重启服务；
 同时，里边有 server_dependency.sh 文件, server_dependency.sh 在发布外网时时不会拷贝此文件;

 * 在这个文件中可以配置  `FILES` 指定 fgamesvr 目录下需要备份的文件或者文件夹, 每个配置必须以 / 开头;
 * GSLBSERVER_ON, CENTERSERVER_ON 等多个 *_ON 变量如果设置为 true, 将使得对应的服务启动, 配置为 false 将不启动;
 * GSLBSERVER_CONFIG, CENTERSERVER_CONFIG 等 *_CONFIG 变量用于配置具体使用哪个 config 文件
   作为服务启动时 skynet 的启动配置，如果不提供将使用默认的 config 配置;
 * RANK_EXIT_PORT, GAME_EXIT_PORT 用于配置服务关闭时由 Python 脚本, 这个端口需要与 server_config.lua
   中的相应的 monitor 端口相同, 如果不配置, 将使用 Python 脚本中默认的端口;
