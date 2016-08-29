return {
    -- 符文材料变化
    GLYPH_STUFF_CHANGE  = 'GLYPH_STUFF_CHANGE',

    -- vip变化
    PLAYER_VIP_CHANGE   = 'PLAYER_VIP_CHANGE',

    -- 等级变化
    -- new_level
    PLAYER_LEVEL_CHANGE = 'PLAYER_LEVEL_CHANGE',

    -- pvp结束
    --[[
        room_flag    -- 房间类型: RAND 随机匹配, FIXED 固定开房, NOPROP 无道具场
        room_type    -- 房间人数类型类型: 2人房 3人房 4人房
        pvp_end_type -- pvp战斗结果: 胜利 失败 平局
        addscore     -- 增加的荣誉值
        addpoint     -- 增加的积分
    --]]
    PVP_END = 'PVP_END',

    -- pve 结束
    --[[
        new_flag --新关卡
        star     --星星数
    --]]
    PVE_END = 'PVE_END',

    --open只会触发一次，玩家重连不会派发此事件
    AGENT_OPEN = 'AGENT_OPEN',
    
    
    -- agent open， 玩家登录或重连成功
    -- bool reconnect: 是否是重连进入的
    AGENT_REL_OPEN = "AGENT_REL_OPEN",

    -- notice_flag: 是否通知客户端
    DAY_CHANGE = 'DAY_CHANGE', -- 在线玩家的日期变更事件

    CHECKPOINT_UPDATE = 'CHECKPOINT_UPDATE', -- PVE关卡更新

    --符文升级
    --[[
        level --等级
    --]]
    GLYPH_UPGRADE = 'GLYPH_UPGRADE', --符文升级

    --荣誉增加
    --[[
	  honor --等级
    --]]
    HONOR_ADD = 'HONOR_ADD',  --荣誉增加

    AGENT_EXIT = 'AGENT_EXIT', -- agent销毁事件

    -- 时间到 pvp无道具场开放
    PVP_NOPROP_INFO = 'PVP_NOPROP_INFO',

    --[[
        数量    当前物品信息    物品配置信息    额外信息    来源    是否通知client
        count,      item,       item_cfg,       extra,       src,    notice_flag
    --]]
    ITEM_ON_ADD             = "ITEM_ON_ADD", --增加物品
    ITEM_ON_REDUCE          = "ITEM_ON_REDUCE", --减少物品时
    ITEM_ON_USE             = "ITEM_ON_USE", --物品使用时
    ITEM_REPLASE_EXTRA      = "ITEM_REPLASE_EXTRA", --物品替换额外数据时
    --[[物品事件的格式一致]]

    DAILY_TASK_COMPLETE = "DAILY_TASK_COMPLETE", --每日任务完成

    CHECK_SAVE_DB = "CHECK_SAVE_DB", --数据备份到数据库的检查
}
