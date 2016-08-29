
## pvp文档

### 数据结构
* agent_pvp.lua
    - agent_pvp.roomid = nil              -- 房间ID，可能是个人房间，或者是自动匹配的
    - agent_pvp.room_is_personal = false  -- 房间是否是个人房间，不是的话就是自动匹配的
    - agent_pvp.has_handle_match = false  -- 自动匹配中
    - agent_pvp.personal_room_id = 0      -- 个人房间
    - agent_pvp.is_personal_host = false  -- 是否是个人房间的房主
    - agent_pvp.room_server_node = nil    -- 房间所属服务节点 在本服为nil跨服为房间所在服的gamenode名

* pvpserver.lua
    - pvp_match_pool { [随机匹配池] = {}, [无道具场匹配池] = {}}
    - 匹配池中是Team的数组 pools = [team]
        Team                队伍
            flag            房间类型
            mode            类型
            players         队伍中的人[agent => account_info]
            createTime      创建时间
    - pvp_match_info = { agent = account_info }

* room.lua
    - room_start_time       -- 房间开始时间
    - room_host             -- 主机
    - room_flag             -- 房间类型： 随机 开放 无道具场
    - room_type             -- 房间人数
    - room_opener           -- 开房的人的uin
    - map_id                -- 地图ID
    - real_count = 0        -- 真实玩家数量

    - player_set = {}       -- [ agent ] 数组
    - camp_set = {}         -- { agent = campno }
    - room_playerinfo = {}  -- { source = account_info}
    - average_ping = {}     -- { uin = [ts] }
    - room_msg = {}         -- { uin = msgNo } 如果小于最大值 则过滤掉
    - leave_mode = {}       -- { uin = mode } 如果不是离开则为nil

### pvp随机|无道具场 匹配流程
1.  发送匹配消息 CS_CMD_PVP_MATCH -> 返回匹配成功消息
    * 随机匹配 如果匹配不到人或者不足 都用AI补充 AI的uin 0， -1， -2
    * 无道具场 都是真人 只有等待到符合条件的人才开 否则一直等待
    - [agent_pvp.lua]
        - agent_pvp.handle_match_msg
        - skynet.call(pvpserver, "lua", "match", agent, mode, account_info)
    - [pvpserver.lua]
        - CMD.match(agent, flag, info)
            - checkCanJoinTeam(team, agent) -- 判断是否能加入队伍
            - joinTeam(team, agent) -- 加入队伍
            - createTeam(flag, agent)   -- 创建队伍
        - enterRoom(team)
            - removeTeam(team)  -- 移除队伍
        - skynet.call(room, "lua", "open", team.flag, 0, team.mode, agent_infos)
    - [room.lua]
        - CMD.open(t1, t2, t3, t4, agent_infos)
            - sharedata_utils.get_rand_map(room_type, room_playerinfo, room_flag) -- 随机地图
            - sharedata_utils.get_map_camp(room_type, room_flag, map_id)    -- 获取阵营
            - get_camp_no(camps)    -- 分配阵营
        - room_cluster_call(v, "lua", "match_ret", skynet.self(), room_type, player_infos, room_flag, room_opener, battleId, map_id)
    - [agent_pvp.lua]
        - match_ret(room, type, player_infos, flag, opener, _battleId, _mapId)
            local data = {
                Type = type,
                Flag = flag,
                RoomId = room,
                Opener = opener,
                MapId = mapId,
                PlayInfo = player_infos,
                            Uin     = info.uin,
                            Camp    = camp,
                            Name    = info.name,
                            Icon    = info.icon_id,
                            Point   = info.pvp_point,
                            PropData  = k > 0 and room_cluster_call(k, "lua", "getPvpProp") or {},
                            GlyphsInfo = getPvpGlyphData(k),
                            AsInfo = getAssistantData(k),
                            SkinId = getSkinData(k),
            }
        - send_response(MESSAGE.CS_CMD_PVP_MATCH, data)
    - 停止匹配 CS_CMD_PVP_STOP_MATCH

2. 发送准备好消息 CS_CMD_PVP_START -> 当所有玩家准备好 服务器就返回
    - [agent_pvp.lua]
        - agent_pvp.handle_start_fight_msg
        - skynet_room_call(agent_pvp.roomid, "start_fight", type, msgNo)
    - [room.lua]
        - CMD.start_fight(source, type, msgNo)
        - send_ready_resp(source) -- 所有人都准备好 则返回; 如果超时 也返回
        - broadcast(source, MESSAGE.CS_CMD_PVP_START, data, true)
3 发送ping消息 CS_CMD_PVP_PING 返回 CS_CMD_PVP_HOST
    - 发送三次后 服务器选定主机 发CS_CMD_PVP_HOST消息
    - [改] 现在客户端一直定时发送ping 作为客户端的心跳
4. 房间内消息
    CS_CMD_PVP_FIGHT                    = 2002, -- 出兵
    CS_CMD_PVP_UPGRADE                  = 2003, -- 建筑升级
    CS_CMD_PVP_MAP                      = 2005, -- 同步地图数据
    CS_CMD_PVP_USE_ITEM                 = 2008, -- 使用道具
    CS_CMD_PVP_APPEAR_CREEP             = 2010, -- 怪物出现 （客户端上报）
    CS_CMD_PVP_OBSERVER                 = 2011, -- 观战
    CS_CMD_PVP_RECOVER_ITEM             = 2012, -- 恢复道具CD时间
    CS_CMD_PVP_USE_AS_SKILL             = 2013, -- 使用精灵技能
    CS_CMD_PVP_TRAP                     = 2015, -- pvp陷阱消息
* 服务器主要做转发
* 每次发送消息 消息号都要增加 消息号小的过滤掉
* broadcast发送给所有房间内的玩家
* 提前被打败 进入观察模式 或者 离开房间
* 道具使用无效 要用CS_CMD_PVP_RECOVER_ITEM 恢复cd时间
* 主机要定时发送CS_CMD_PVP_MAP

5. 结算
* 结算有三种情况：
    * 1 平局 超时或者房间内的玩家掉线 都算这种情况
    * 2 胜利
    * 3 失败

    - [agent.lua]
        - agent_pvp.handle_pvp_end_msg
        - skynet_room_call(agent_pvp.roomid, "end_fight", win, ts, msgNo)
    - [room.lua]
        - CMD.end_fight(source, win_uin, ts, msgNo)
        - room_cluster_call(v, "lua", "process_pvp_end", room_flag, room_type, room_opener, pvp_end_type, room_playerinfo, win_uin, leave_mode[room_playerinfo[v].uin], math.floor(battleTime))
    - [agent.lua]
        - process_pvp_end
        - sharedata_utils.calc_score -- 计算荣誉
        - sharedata_utils.calc_point -- 计算积分
        - streak_win_notice -- 胜利广播
        - process_pvp_lose_notice -- 失败广播
    - [room.lua]
        - broadcast(source, MESSAGE.CS_CMD_PVP_END, data, true)
        - room_exit()

