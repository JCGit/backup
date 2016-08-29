
## agent模块拆分

` 参考：agent_pvp.lua `

### 思想

  `一个模块的【数据】和【方法】都尽量封装到这个模块里面去处理 以减少agent的体积`

### 模块必须实现接口

* 模块初始化

```lua
init(
reconnect,          -- 重连标识
CMD,                -- agent函数
cmd_handle_mapping, -- 消息处理函数映射表
account_info,       -- 玩家信息
user_fd,            -- 连接fd
)
{
    --在init中做赋值 注意不要用和参数相同的变量名

    -- 调用必须实现的接口
    add_cmd(CMD)
    add_cmd_handle_mapping(cmd_handle_mapping)
}
-- init调用的地方在agent的CMD.open()里面
```

* 将这个模块的处理函数放到agent的CMD中去

```
	add_cmd(CMD)
```

* 将这个模块的消息处理函数放到agent的映射表中去

```
	add_cmd_handle_mapping(cmd_handle_mapping)
```

* agent里面通过require对应的模块来访问这个模块的变量和方法

```lua
local agent_pvp = require "agent_pvp"               --require模块
agent_pvp.last_pvp_heartbeat_time = skynet.now()    --访问变量
agent_pvp.heartbeat_failed()                        --访问方法
```


