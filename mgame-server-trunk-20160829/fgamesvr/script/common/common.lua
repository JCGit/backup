-- common.lua
-- 想用常量或者枚举 都可以在这个文件定义table来实现

local COMMON = {}

-- 消息ID映射表
COMMON.MESSAGE = {
    CS_CMD_REG                          = 100,
    CS_CMD_GETSERVER                    = 200,
    CS_CMD_LOGIN                        = 500,
    CS_CMD_HEART_BEAT                   = 501,
    CS_CMD_ENTER_SERVER                 = 502,
    CS_CMD_LOGOUT                       = 503,
    CS_CMD_DIRECT_ENTER                 = 504,
    CS_CMD_CANENTER                     = 505,
    CS_CMD_QUEUE                        = 506,

    CS_CMD_CHANGE_NAME                  = 507,
    CS_CMD_GM_COMMAND                   = 508, -- GM命令
    CS_CMD_USE_GIFT_CODE                = 510, -- 使用礼包码
    CS_CMD_QUERY_PLAYERINFO             = 511, -- 查看玩家信息
    CS_CMD_CHANGE_ICON_ID               = 512, -- 修改玩家头像

    CS_CMD_SET_NEWBIE                   = 513, --设置新手标识(网络版使用)
    CS_CMD_GET_NEWBIE_LIST              = 514, --获取新手引导list
    CS_CMD_SHOP_GUIDE                   = 515, --修改玩家钻石 配合新手引导
    CS_CMD_SET_TITLE                    = 517, --设置称号
    CS_CMD_SHOP_BUY                     = 518, --商城购买
    CS_CMD_KICK_PLAYER                  = 519, --同一账号踢号协议
    CS_CMD_STOP_KICK_PLAYLER            = 520, --停服踢号协议

    CS_CMD_REQUEST_PKEY                 = 550, -- 请求本次握手使用的公钥
    CS_CMD_SEND_CLIENT_KEY              = 551, -- 客户端发送私钥给服务器

    -- 关卡
    CS_CMD_ENTER_POINT                  = 604, --进入PVE关卡

    --数据同步
    CS_CMD_RECHARGE_CALLBACK            = 704, -- 充值数据同步
    CS_CMD_SYC_PLAYERINFO               = 801, -- 玩家信息同步
    
    CS_CMD_MSG_TRANSACTION              = 811, --消息事务操作，一次性处理多条协议

    -- 精灵系统
    CS_CMD_AS_SYNC_INFO                 = 1000, -- 同步精灵数据
    CS_CMD_AS_NEW_ADD                   = 1001, -- 新增精灵
    CS_CMD_AS_OPERATE                   = 1002, -- 装备/卸下精灵; 进阶 升级
    CS_CMD_AS_USE_CARD                  = 1003, -- 使用精灵体验卡
    CS_CMD_AS_UPGRAD    				= 1004, -- 将军升级

    CS_CMD_PVP_MATCH                    = 2000, -- 请求pvp匹配
    CS_CMD_PVP_START                    = 2001, -- pvp开始
    CS_CMD_PVP_FIGHT                    = 2002, -- 出兵
    CS_CMD_PVP_UPGRADE                  = 2003, -- 建筑升级
    CS_CMD_PVP_END                      = 2004, -- pvp结算
    CS_CMD_PVP_MAP                      = 2005, -- 同步地图数据
    CS_CMD_PVP_PING                     = 2006, -- ping （判断主机）
    CS_CMD_PVP_LEAVE                    = 2007, -- 离开房间
    CS_CMD_PVP_USE_ITEM                 = 2008, -- 使用道具
    CS_CMD_PVP_STOP_MATCH               = 2009, -- 停止匹配
    CS_CMD_PVP_APPEAR_CREEP             = 2010, -- 怪物出现 （客户端上报）
    CS_CMD_PVP_OBSERVER                 = 2011, -- 观战
    CS_CMD_PVP_RECOVER_ITEM             = 2012, -- 恢复道具CD时间
    CS_CMD_PVP_USE_AS_SKILL             = 2013, -- 使用精灵技能
    CS_CMD_PVP_TRAP                     = 2015, -- pvp陷阱消息
    CS_CMD_PVP_HOST                     = 2016, -- pvp选择主机
    
    CS_CMD_PVP_HAPPY_ITEMS              = 2030, --欢乐道具场获取道具
    CS_CMD_PVP_HAPPY_USE_ITEM           = 2031, --欢乐道具场使用道具

    CS_CMD_PVP_RANKING_LIST             = 2100,
    CS_CMD_PVP_RANKING_INFO             = 2101,
    CS_CMD_PVP_SELF_RANKLIST            = 2103,
    CS_CMD_PVP_CHAT                     = 2200,
    CS_CMD_PVP_GET_AWARD                = 2201, -- PVP无道具场获得宝箱奖励
    CS_CMD_PVP_BATTLE_FAILE             = 2202, -- 多人战斗中途失败

    -- 商品
    CS_CMD_BUY_ITEM                     = 3000,
    CS_CMD_USE_ITEM                     = 3001,
    CS_CMD_SHOP_LIST                    = 3002, -- 商城列表
    CS_CMD_EXCHANGE_ITEM                = 3003, -- 兑换荣誉物品
    CS_CMD_UPDATE_EXCHANGE_LIST         = 3004, -- 刷新荣誉商店
    CS_CMD_BUY_VIP                      = 3005,
    CS_CMD_OPEN_CHEST                   = 3006, -- 开宝箱
    CS_CMD_VIP_TIME                     = 3007, -- 请求vip时间
    CS_CMD_BUY_NOPROP_COUNT             = 3008, -- 购买无道具场次数
    CS_CMD_SET_SIGNATURE                = 3010, --设置签名
    CS_CMD_SET_EXTRAINFO_VISIBLE        = 3011, --设置额外信息的可见性
    CS_CMD_GET_EXTRAINFO                = 3012, --获取额外信息
    CS_LIMIT_ITEM_LIST                  = 3013, --道具购买每日限量列表


    --美食研究院
    CS_CMD_INSTITUTE_INFO               = 3020,
    CS_CMD_INSTITUTE_BUY                = 3021,

    --前端支付成功通知服务器校验
    CS_CMD_PAY_SUCCESS                  = 3103,

    --绑定新的openid
    CS_CMD_BIND_OPENID                  = 3104,

    --属性
    CS_CMD_PROPERTY_UPDATE              = 3101,
    CS_CMD_SET_NEWBIE_OSS               = 3102,

    CS_CMD_GET_ACHIEVEMENT              = 4001, -- 获取成就信息
    CS_CMD_RECEIVE_ACHIEVEMENT          = 4002, -- 领取成就点数
    CS_CMD_RECEIVE_ACHIEVEMENT_REWARD   = 4003, -- 领取成就等级奖励
    CS_CMD_UPDATE_ACHIEVEMENT           = 4004, -- 成就更新

    --连胜
    CS_CMD_STREAK_WIN                   = 5001, --连胜场次
    CS_CMD_STREAK_WIN_GET               = 5002, --获取奖励
    CS_CMD_SHARE_REWARD_GET             = 5003,

    CS_CMD_PVP_LUCK_CHEST_INFO          = 5101, -- 竞技场幸运宝箱查询
    CS_CMD_PVP_LUCK_CHEST_OPEN          = 5102, -- 竞技场幸运宝箱开启
    CS_CMD_PVP_NORMAL_CHEST_INFO        = 5103, -- 竞技场宝箱查询
    CS_CMD_PVP_NORMAL_CHEST_OPEN        = 5104, -- 竞技场宝箱开启
    CS_CMD_PVP_BUY_CHEST_SLOT           = 5105, -- 购买竞技场宝箱插槽
    CS_CMD_PVP_CLEAR_CHEST_TIME         = 5106, -- 清除竞技场宝箱CD

    CS_CMD_PAY_GUIDE_BUY                = 5110, -- 付费引导购买
    CS_CMD_PAY_GUIDE_BUY_SHIELD         = 5111, -- 付费引导护盾购买

    -- 时装协议
    CS_CMD_SKIN_SYNC                    = 5200, --同步时装列表
    CS_CMD_SKIN_UP_DOWN                 = 5201, --时装装备卸下
    CS_CMD_SKIN_NEW                     = 5202, --获得新的时装
    CS_CMD_SKIN_BUY                     = 5203, --时装加过期时间
    CS_CMD_SKIN_GUIDE                   = 5204, --时装引导

    --公告, 邮件
    CS_CMD_NOTICE                       = 6001,  --公告
    CS_CMD_GET_MAIL                     = 6002,
    CS_CMD_DEL_MAIL                     = 6003,
    CS_CMD_NOTICE_MAIL                  = 6004,

    -- 运营活动相关
    CS_CMD_OP_ACT_ADD                   = 7000, -- 运营活动增加
    CS_CMD_OP_ACT_STATE_CHANGED         = 7001, -- 运营活动状态改变
    CS_CMD_ACT_SELF_DATA_CHANGED        = 7002, --个人活动数据改变
    CS_CMD_ACT_GLOBAL_DATA_CHANGED      = 7003, --全局活动数据改变
    CS_CMD_OP_ACT_CONFIG_LIST           = 7004, --运营活动配置列表
    CS_CMD_OP_ACT_SELF_DATA_LIST        = 7005, --请求个人活动数据列表
    CS_CMD_OP_ACT_GLOBAL_DATA_LIST      = 7006, --请求全局活动数据列表

    -- 运营活动——DEMO
    CS_CMD_OP_ACT_DEMO_TAKE             = 7100, -- 运营活动DEMO领取协议
    -- 运营活动——兑换活动
    CS_CMD_OP_ACT_EXCHANGE              = 7110, --运营活动兑换协议

    -- 7日签到
    CS_CMD_SEVEN_DAY_SIGN_LIST          = 8001,   --获取签到列表
    CS_CMD_SEVEN_DAY_SIGN               = 8002,   --签到

    --活动相关
    CS_CMD_GET_OPEN_ACTIVITY            = 8100,  -- 获取所有开启的活动
    CS_CMD_GET_ACTIVITY_DATA            = 8101,  -- 获取开启的活动信息
    CS_CMD_RECEIVE_ACTIVITY_REWARD      = 8102,  -- 领取充值活动奖励
    CS_CMD_GET_POWER_ENERGY_ACTIVITY_DATA = 8103,  -- 获取体力精力大放送活动数据
    CS_CMD_RECEIVE_POWER_ENERGY_REWARD  = 8104,  -- 领取体力精力大放送活动奖励
    CS_CMD_BUY_NEWBIE_GIFT              = 8105 , -- 购买新手礼包
    CS_CMD_ACTIVITY_CRAZY_USAGE         = 8107,  -- 领取狂欢活动礼包
    CS_CMD_SUM_RECHARGE                 = 8109,  -- 充值活动礼包可领取通知(首充&累充)

    -- 圣诞活动
    CS_CMD_CHRISTMAS_DECORATE_TREE      = 8110, -- 装扮圣诞树
    CS_CMD_CHRISTMAS_TAKE_BONUS         = 8111, -- 领取圣诞树奖励
    CS_CMD_CHRISTMAS_GET_DATA           = 8112, -- 获取圣诞活动数据
    CS_CMD_CHRISTMAS_GET_RANK           = 8113, -- 获取圣诞活动排行榜

    -- 情人节活动
    CS_CMD_VALENTINE_GET_DATA           = 8114, -- 获取情人节活动数据
    CS_CMD_VALENTINE_EXCHANGE           = 8115, -- 礼盒兑换

    -- 七日目标系统
    CS_CMD_GET_SEVENDAY_GOAL_LIST       = 8116;
    CS_CMD_NOTICE_SEVENDAY_GOAL         = 8117;
    CS_CMD_RETRIVE_SEVENDAY_GOAL        = 8118;

    -- 付费活动
    CS_CMD_LIMIT_EXCHANGE               = 8120;
    CS_CMD_SUM_CALL                     = 8121;

    --每日必做
    CS_CMD_MISSION_LIST                 = 8201, --任务列表
    CS_CMD_MISSION_UPDATE               = 8202, --任务更新
    CS_CMD_MISSION_COMPLETE             = 8203, --任务完成

    CS_CMD_ACTIVITY_LIST                = 8210, --活动列表
    CS_CMD_ACTIVITY_UPDATE              = 8211, --活动更新
    CS_CMD_ACTIVITY_SUBMIT              = 8212, --提交活动

    CS_CMD_BATTLE_GIFT                  = 8213, -- 勇者之心，战斗狂人领取任务礼包
    CS_CMD_BATTLE_SCORE                 = 8214, -- 王者之路荣誉领取

    --元旦活动
    CS_CMD_NEWYEAR_GET_INFO             = 8223,
    CS_CMD_NEWYEAR_REWARD               = 8224,
    CS_CMD_NEWYEAR_CMP                  = 8225,


    --抢红包活动
    CS_CMD_RED_PACKETS_GET_INFO         = 8231,
    CS_CMD_RED_PACKETS_GREB             = 8232,
    CS_CMD_RED_PACKETS_OPENED           = 8233,

    CS_CMD_RANK_WEEK_REWARD_INFO        = 8241,
    CS_CMD_RANK_WEEK_REWARD_REWARD      = 8242,

    -- 每日任务
    CS_CMD_DAILY_TASK_STATUS            = 8250, -- 本次登录是否显示每日任务
    CS_CMD_REQUEST_DAILY_TASK           = 8251, -- 获取当前的每日任务列表
    CS_CMD_CHANGE_DAILY_TASK            = 8252, -- 切换任务
    CS_CMD_OPEN_DAILY_TASK_AWARD        = 8253, -- 打开每日任务奖励
    CS_CMD_UPDATE_DAILY_TASK_PROGRESS   = 8254, -- 更新每日任务进度
    CS_CMD_NEW_DAILY_TASK               = 8255, -- 有新一天的每日任务通知
    CS_CMD_UPDATE_TASK_STATUS           = 8256, -- 单项任务的开启状态变化通知

    --好友
    CS_CMD_FRIEND_QUERY                 = 8301, --根据UNI查询信息
    CS_CMD_FRIEND_REQUEST_ADD           = 8302, --请求加好友
    CS_CMD_FRIEND_CONFIRM_ADD           = 8303, --加好友应答
    CS_CMD_FRIEND_GET_INFO              = 8304, --获取好友信息（包括好友列表、请求加好友信息）
    CS_CMD_FRIEND_REQUEST_REMOVE        = 8305, --删除好友
    CS_CMD_FRIEND_ADD_BLACK_LIST        = 8306, --加黑名单
    CS_CMD_FRIEND_REMOVE_BLACK_LIST     = 8307, --移除黑名单
    CS_CMD_FRIEND_SYC_LIST              = 8308, --同步平台好友

    --个人房间
    CS_CMD_PERSONAL_ROOM_CREATE         = 8401, --创建个人房间
    CS_CMD_PERSONAL_ROOM_ENTER          = 8402, --进入个人房间
    CS_CMD_PERSONAL_ROOM_LEAVE          = 8403, --离开个人房间
    CS_CMD_PERSONAL_ROOM_KICK           = 8404, --踢人
    CS_CMD_PERSONAL_ROOM_INVITE         = 8405, --邀请好友
    CS_CMD_PERSONAL_ROOM_BROADCAST      = 8406, --挑战全服
    CS_CMD_PERSONAL_ROOM_CHAT           = 8407, --聊天
    CS_CMD_PERSONAL_ROOM_MODE           = 8408, --改变房间模式
    CS_CMD_PERSONAL_ROOM_BEGIN_FIGHT    = 8409, --开战
    CS_CMD_PERSONAL_ROOM_APPLY_OR_REFUSE_INVITE = 8410, -- 接受或拒绝约战
    CS_CMD_PERSONAL_ROOM_COUNT_DOWN     = 8411, --倒计时

    -- PVE
    CS_CMD_PVE_CHECK_POINT_SAVE         = 8501, -- 设置关卡ID
    CS_CMD_PVE_CHECK_POINT_LIST         = 8502, -- 请求关卡list
    CS_CMD_PVE_CHAPTER_UNLOCK           = 8504, -- 关卡章节解锁
    CS_CMD_PVE_GET_CHEST                = 8505, -- 宝箱领取
    CS_CMD_PVE_NOTIFY                   = 8506, -- pve通知
    CS_CMD_PVE_CHECK_AWARD              = 8507, -- 检查新手训练营奖励
    CS_CMD_PVE_RANK                     = 8508, -- pve积分排名
    CS_CMD_TOP_RANK                     = 8509,
    -- 此协议是巅峰排行活动里面的排行 (pve星星排行，pvp胜场排名，pvp连胜排名)
    CS_CMD_TOP_RANK_TIME                = 8510, -- 巅峰排行时间

    -- 厨艺
    CS_CMD_GLYPHS_REQUEST_INFO          = 8700, -- 请求厨艺数据
    CS_CMD_GLYPHS_REQUEST_BASIC_INFO    = 8701, -- 请求厨艺材料数据
    CS_CMD_GLYPHS_ACTIVE                = 8702, -- 激活厨艺
    CS_CMD_GLYPHS_UPGRADE               = 8703, -- 升级厨艺
    CS_CMD_GLYPH_GUIDE_ADDSTUFF         = 8705, -- 请求新手引导添加材料
    CS_CMD_GLYPHS_UPDATE                = 8706, -- 厨艺状态改变后, 服务器推送通知

    --邀请码
    CS_CMD_INVITE_CODE                  = 9901,
    CS_CMD_FIVE_STAR                    = 9902,
    CS_CMD_WATCH_MOVIE                  = 9903,
}

do
    local data = {}
    for k, v in pairs(COMMON.MESSAGE) do
        data[v] = k
    end
    COMMON.MESSAGE_HELPER = data
end

-- 消息ID映射proto表
local MESSAGE = COMMON.MESSAGE
COMMON.protomap = {
    [MESSAGE.CS_CMD_REG]                    = {Req = "Reg_Req",                 Resp = "Reg_Resp"},
    [MESSAGE.CS_CMD_GETSERVER]              = {Req = "GetServer_Req",           Resp = "GetServer_Resp"},
    [MESSAGE.CS_CMD_LOGIN]                  = {Req = "Login_Req",               Resp = "Login_Resp"},
    [MESSAGE.CS_CMD_HEART_BEAT]             = {Req = "HeartBeat_Req",           Resp = "HeartBeat_Resp"},
    [MESSAGE.CS_CMD_ENTER_SERVER]           = {Req = "EnterServer_Req",         Resp = "EnterServer_Resp"},
    [MESSAGE.CS_CMD_LOGOUT]                 = {Req = "Logout_Req",              Resp = ""},
    [MESSAGE.CS_CMD_DIRECT_ENTER]           = {Req = "DirectEnter_Req",         Resp = "DirectEnter_Resp"},
    [MESSAGE.CS_CMD_CANENTER]               = {Req = "CanEnter_Req",            Resp = ""},
    [MESSAGE.CS_CMD_QUEUE]                  = {Req = "Queue_Req",               Resp = ""},

    [MESSAGE.CS_CMD_CHANGE_NAME]            = {Req = "ChangeName_Req",          Resp = "ChangeName_Resp"},
    [MESSAGE.CS_CMD_GM_COMMAND]             = {Req = "GMCmd_Req",               Resp = ""},
    [MESSAGE.CS_CMD_USE_GIFT_CODE]          = {Req = "UseGiftCode_Req",         Resp = "UseGiftCode_Resp"},
    [MESSAGE.CS_CMD_QUERY_PLAYERINFO]       = {Req = "QueryPlayerInfo_Req",     Resp = "QueryPlayerInfo_Resp"},
    [MESSAGE.CS_CMD_CHANGE_ICON_ID]         = {Req = "ChangePlayerIcon_Req",    Resp = "ChangePlayerIcon_Resp"},
    [MESSAGE.CS_CMD_SET_NEWBIE]             = {Req = "SetNewBie_Req",           Resp = "SetNewBie_Resp"},
    [MESSAGE.CS_CMD_GET_NEWBIE_LIST]        = {Req = "GetNewBieList_Req",       Resp = "GetNewBieList_Resp"},
    [MESSAGE.CS_CMD_SHOP_GUIDE]             = {Req = "ShopGuide_Req",           Resp = "ShopGuide_Resp"},
    [MESSAGE.CS_CMD_SET_TITLE]              = {Req = "SetTitle_Req",            Resp = "SetTitle_Resp"},
    [MESSAGE.CS_CMD_SHOP_BUY]               = {Req = "ShopBuy_Req",             Resp = "ShopBuy_Resp"},
    [MESSAGE.CS_CMD_KICK_PLAYER]            = {Req = "KickPlayer_Req",          Resp = ""},
    [MESSAGE.CS_CMD_STOP_KICK_PLAYLER]      = {Req = "StopKickPlayer_Req",      Resp = ""},
    [MESSAGE.CS_CMD_REQUEST_PKEY]           = {Req = "RequestPKey_Req",         Resp = "RequestPKey_Resp"},
    [MESSAGE.CS_CMD_SEND_CLIENT_KEY]        = {Req = "SendClientKey_Req",       Resp = "SendClientKey_Resp"},

    [MESSAGE.CS_CMD_ENTER_POINT]            = {Req = "EnterPoint_Req",          Resp = "EnterPoint_Resp"},
    [MESSAGE.CS_CMD_RECHARGE_CALLBACK]      = {Req = "RechargeCallback_Req",    Resp = "RechargeCallback_Resp"},
    [MESSAGE.CS_CMD_SYC_PLAYERINFO]         = {Req = "SycPlayerInfo_Req",       Resp = "SycPlayerInfo_Resp"},
    
    [MESSAGE.CS_CMD_MSG_TRANSACTION]        = {Req = "MsgTransaction_Req",       Resp = "MsgTransaction_Resp"},

    [MESSAGE.CS_CMD_AS_SYNC_INFO]           = {Req = "AsSyncInfo_Req",          Resp = "AsSyncInfo_Resp"},
    [MESSAGE.CS_CMD_AS_NEW_ADD]             = {Req = "AsNewAdd_Req",            Resp = "AsNewAdd_Resp"},
    [MESSAGE.CS_CMD_AS_OPERATE]             = {Req = "AsOperate_Req",           Resp = "AsOperate_Resp"},
    [MESSAGE.CS_CMD_AS_USE_CARD]            = {Req = "AsUseCard_Req",           Resp = "AsUseCard_Resp"},
    [MESSAGE.CS_CMD_AS_UPGRAD] 				= {Req = "AsUpGrad_Req",   			Resp = "AsUpGrad_Resp"},

    [MESSAGE.CS_CMD_PVP_MATCH]              = {Req = "Match_Req",               Resp = "Match_Resp"},
    [MESSAGE.CS_CMD_PVP_START]              = {Req = "Start_Req",               Resp = "Start_Resp"},
    [MESSAGE.CS_CMD_PVP_FIGHT]              = {Req = "Fight_Req",               Resp = "Fight_Resp"},
    [MESSAGE.CS_CMD_PVP_UPGRADE]            = {Req = "Upgrade_Req",             Resp = "Upgrade_Resp"},
    [MESSAGE.CS_CMD_PVP_END]                = {Req = "End_Req",                 Resp = "End_Resp"},
    [MESSAGE.CS_CMD_PVP_MAP]                = {Req = "Map_Req",                 Resp = "Map_Resp"},
    [MESSAGE.CS_CMD_PVP_PING]               = {Req = "Ping_Req",                Resp = "Ping_Resp"},
    [MESSAGE.CS_CMD_PVP_LEAVE]              = {Req = "Leave_Req",               Resp = "Leave_Resp"},
    [MESSAGE.CS_CMD_PVP_USE_ITEM]           = {Req = "PvpUseItem_Req",          Resp = "PvpUseItem_Resp"},
    [MESSAGE.CS_CMD_PVP_STOP_MATCH]         = {Req = "StopMatch_Req",           Resp = "StopMatch_Resp"},
    [MESSAGE.CS_CMD_PVP_APPEAR_CREEP]       = {Req = "AppearCreep_Req",         Resp = "AppearCreep_Resp"},
    [MESSAGE.CS_CMD_PVP_OBSERVER]           = {Req = "PvpObserver_Req",         Resp = "PvpObserver_Resp"},
    [MESSAGE.CS_CMD_PVP_RECOVER_ITEM]       = {Req = "RecoverItem_Req",         Resp = "RecoverItem_Resp"},
    [MESSAGE.CS_CMD_PVP_USE_AS_SKILL]       = {Req = "PvpUseSkill_Req",         Resp = "PvpUseSkill_Resp"},
    [MESSAGE.CS_CMD_PVP_TRAP]               = {Req = "PvpTrap_Req",             Resp = "PvpTrap_Resp"},
    [MESSAGE.CS_CMD_PVP_HOST]               = {Req = "PvpHost_Req",             Resp = "PvpHost_Resp"},

    [MESSAGE.CS_CMD_PVP_HAPPY_ITEMS]        = {Req = "", Resp = "PvpHappyItems_Resp"},
    [MESSAGE.CS_CMD_PVP_HAPPY_USE_ITEM]     = {Rep = "PvpHappyUseItem_Req", Resp = "PvpHappyUseItem_Resp"},

    [MESSAGE.CS_CMD_PVP_RANKING_LIST]       = {Req = "PvpRankingGetList_Req",   Resp = "PvpRankingGetList_Resp"},
    [MESSAGE.CS_CMD_PVP_RANKING_INFO]       = {Req = "PvpRankingPlayerInfo_Req",Resp = "PvpRankingPlayerInfo_Resp"},
    [MESSAGE.CS_CMD_PVP_SELF_RANKLIST]      = {Req = "PvpSelfRankList_Req",     Resp = "PvpSelfRankList_Resp"},
    [MESSAGE.CS_CMD_PVP_CHAT]               = {Req = "PvpChat_Req",             Resp = "PvpChat_Resp"},
    [MESSAGE.CS_CMD_PVP_GET_AWARD]          = {Req = "",                        Resp = "PvpGetAward_Resp"},
    [MESSAGE.CS_CMD_PVP_BATTLE_FAILE]       = {Req = "PvpBattleFail_Req",       Resp = "PvpBattleFail_Resp"},

    [MESSAGE.CS_CMD_BUY_ITEM]               = {Req = "BuyItem_Req",             Resp = "BuyItem_Resp"},
    [MESSAGE.CS_CMD_USE_ITEM]               = {Req = "UseItem_Req",             Resp = "UseItem_Resp"},
    [MESSAGE.CS_CMD_SHOP_LIST]              = {Req = "ShopList_Req",            Resp = "ShopList_Resp"},
    [MESSAGE.CS_CMD_EXCHANGE_ITEM]          = {Req = "ExchangeItem_Req",        Resp = "ExchangeItem_Resp"},
    [MESSAGE.CS_CMD_UPDATE_EXCHANGE_LIST]   = {Req = "UpdateExchangeList_Req",  Resp = "UpdateExchangeList_Resp"},
    [MESSAGE.CS_CMD_BUY_VIP]                = {Req = "BuyVip_Req",              Resp = "BuyVip_Resp"},
    [MESSAGE.CS_CMD_OPEN_CHEST]             = {Req = "OpenChest_Req",           Resp = "OpenChest_Resp"},
    [MESSAGE.CS_CMD_VIP_TIME]               = {Req = "VipTime_Req",             Resp = "BuyVip_Resp"},
    [MESSAGE.CS_CMD_BUY_NOPROP_COUNT]       = {Req = "BuyNopropCount_Req",      Resp = "BuyNopropCount_Resp"},
    [MESSAGE.CS_CMD_SET_SIGNATURE]          = {Req = "SetSignature_Req",        Resp = "SetSignature_Resp"},
    [MESSAGE.CS_CMD_SET_EXTRAINFO_VISIBLE]  = {Req = "SetExtraInfoVisible_Req", Resp = "SetExtraInfoVisible_Resp"},
    [MESSAGE.CS_CMD_GET_EXTRAINFO]          = {Req = "GetExtraInfo_Req",        Resp = "GetExtraInfo_Resp"},
    [MESSAGE.CS_CMD_INSTITUTE_INFO]         = {Req = "InstituteInfo_Req",       Resp = "InstituteInfo_Resp"},
    [MESSAGE.CS_CMD_INSTITUTE_BUY]          = {Req = "InstituteBuy_Req",        Resp = "InstituteBuy_Resp"},
    [MESSAGE.CS_CMD_PROPERTY_UPDATE]        = {Req = "",                        Resp = "PropertyUpdate_Resp"},
    [MESSAGE.CS_CMD_SET_NEWBIE_OSS]         = {Req = "SetNewBieOss_Req",        Resp = "SetNewBieOss_Resp"},
    [MESSAGE.CS_LIMIT_ITEM_LIST]            = {Req = "SynLimitItemList_Req",    Resp = "SynLimitItemList_Resp"},

    [MESSAGE.CS_CMD_GET_ACHIEVEMENT]        = {Req = "GetAchievement_Req",      Resp = "GetAchievement_Resp"},
    [MESSAGE.CS_CMD_RECEIVE_ACHIEVEMENT]    = {Req = "ReceiveAchievement_Req",  Resp = "ReceiveAchievement_Resp"},
    [MESSAGE.CS_CMD_RECEIVE_ACHIEVEMENT_REWARD] = {Req = "ReceiveAchievementReward_Req",
                                                                                Resp = "ReceiveAchievementReward_Resp"},
    [MESSAGE.CS_CMD_UPDATE_ACHIEVEMENT]     = {Req = "",                        Resp = "UpdateAchievement_Resp"},

    [MESSAGE.CS_CMD_STREAK_WIN]             = {Req = "StreakWinValue_Req",      Resp = "StreakWinValue_Resp"},
    [MESSAGE.CS_CMD_STREAK_WIN_GET]         = {Req = "StreakWinGet_Req",        Resp = "StreakWinGet_Resp"},
    [MESSAGE.CS_CMD_SHARE_REWARD_GET]       = {Req = "ShareReward_Req",         Resp = "ShareReward_Resp"},

    [MESSAGE.CS_CMD_PVP_LUCK_CHEST_INFO]    = {Req = "PVPLuckChestInfo_Req",    Resp = "PVPLuckChestInfo_Resp"},
    [MESSAGE.CS_CMD_PVP_LUCK_CHEST_OPEN]    = {Req = "PVPLuckChestOpen_Req",    Resp = "PVPLuckChestOpen_Resp"},
    [MESSAGE.CS_CMD_PVP_NORMAL_CHEST_INFO]  = {Req = "PVPNormalChestInfo_Req",  Resp = "PVPNormalChestInfo_Resp"},
    [MESSAGE.CS_CMD_PVP_NORMAL_CHEST_OPEN]  = {Req = "PVPNormalChestOpen_Req",  Resp = "PVPNormalChestOpen_Resp"},
    [MESSAGE.CS_CMD_PVP_BUY_CHEST_SLOT]     = {Req = "PVPNormalChestSlotBuy_Req", Resp ="PVPNormalChestSlotBuy_Resp"},
    [MESSAGE.CS_CMD_PVP_CLEAR_CHEST_TIME]   = {Req = "PVPNormalChestClearTime_Req",Resp="PVPNormalChestClearTime_Resp"},

    [MESSAGE.CS_CMD_PAY_GUIDE_BUY]          = {Req = "PayGuideBuy_Req",    Resp = "PayGuideBuy_Resp"},
    [MESSAGE.CS_CMD_PAY_GUIDE_BUY_SHIELD]   = {Req = "PayGuideBuyShield_Req",    Resp = "PayGuideBuyShield_Resp"},

    [MESSAGE.CS_CMD_NOTICE]                 = {Req = "Notice_Req",              Resp = ""},
    [MESSAGE.CS_CMD_GET_MAIL]               = {Req = "GetMail_Req",             Resp = "GetMail_Resp"},
    [MESSAGE.CS_CMD_DEL_MAIL]               = {Req = "DelMail_Req",             Resp = "DelMail_Resp"},
    [MESSAGE.CS_CMD_NOTICE_MAIL]            = {Req = "",                        Resp = "NoticeMail_Req"},

    [MESSAGE.CS_CMD_OP_ACT_ADD]             = {Req = "",                        Resp = "OpActAdd_Resp"},
    [MESSAGE.CS_CMD_OP_ACT_STATE_CHANGED]   = {Req = "",                        Resp = "OpActStateChanged_Resp"},
    [MESSAGE.CS_CMD_ACT_SELF_DATA_CHANGED]  = {Req = "",                        Resp = "OpActSelfDataChanged_Resp"},
    [MESSAGE.CS_CMD_ACT_GLOBAL_DATA_CHANGED]= {Req = "",                        Resp = "OpActGlobalDataChanged_Resp"},
    [MESSAGE.CS_CMD_OP_ACT_CONFIG_LIST]     = {Req = "OpActConfigList_Req",     Resp = "OpActConfigList_Resp"},
    [MESSAGE.CS_CMD_OP_ACT_SELF_DATA_LIST]  = {Req = "OpActSelfDataList_Req",   Resp = "OpActSelfDataList_Resp"},
    [MESSAGE.CS_CMD_OP_ACT_GLOBAL_DATA_LIST]= {Req = "OpActGlobalDataList_Req", Resp = "OpActGlobalDataList_Resp"},
    [MESSAGE.CS_CMD_OP_ACT_DEMO_TAKE]       = {Req = "OpActDemoTake_Req",       Resp = "OpActDemoTake_Resp"},
    [MESSAGE.CS_CMD_OP_ACT_EXCHANGE]        = {Req = "OpActExchange_Req",       Resp = "OpActExchange_Resp"},

    [MESSAGE.CS_CMD_SEVEN_DAY_SIGN_LIST]    = {Req = "SevenDaySignList_Req",    Resp = "SevenDaySignList_Resp"},
    [MESSAGE.CS_CMD_SEVEN_DAY_SIGN]         = {Req  = "SevenDaySign_Req",       Resp = "SevenDaySign_Resp"},

    -- 活动相关
    [MESSAGE.CS_CMD_GET_OPEN_ACTIVITY]      = {Req = "GetOpenActivity_Req",     Resp = "GetOpenActivity_Resp"},
    [MESSAGE.CS_CMD_GET_ACTIVITY_DATA]      = {Req = "GetActivityData_Req",     Resp = "GetActivityData_Resp"},
    [MESSAGE.CS_CMD_RECEIVE_ACTIVITY_REWARD]= {Req = "ReceiveActivityReward_Req",
                                                                                Resp = "ReceiveActivityReward_Resp"},
    [MESSAGE.CS_CMD_GET_POWER_ENERGY_ACTIVITY_DATA] = {Req = "GetPowerEnergyActivity_Req",
                                                                                Resp = "GetPowerEnergyActivity_Resp"},
    [MESSAGE.CS_CMD_RECEIVE_POWER_ENERGY_REWARD] = {Req = "ReceivePowerEnergy_Req",
                                                                                Resp = "ReceivePowerEnergy_Resp"},
    -- 购买新手礼包
    [MESSAGE.CS_CMD_BUY_NEWBIE_GIFT]        = {Req = "BuyNewbieGift_Req",       Resp = "BuyNewbieGift_Resp"},

    --狂欢周
    [MESSAGE.CS_CMD_ACTIVITY_CRAZY_USAGE]   = {Req = "ActivityCrazyUsage_Req",  Resp = "ActivityCrazyUsage_Resp"},
    [MESSAGE.CS_CMD_SUM_RECHARGE]           = {Req = "SumRecharge_Req",         Resp = "SumRecharge_Resp"},

    [MESSAGE.CS_CMD_CHRISTMAS_DECORATE_TREE]= {Req = "ChristmasDecorate_Req",   Resp = "ChristmasDecorate_Resp"},
    [MESSAGE.CS_CMD_CHRISTMAS_TAKE_BONUS]   = {Req = "ChristmasTakeBonus_Req",  Resp = "ChristmasTakeBonus_Resp"},
    [MESSAGE.CS_CMD_CHRISTMAS_GET_DATA]     = {Req = "ChristmasGetDate_Req",    Resp = "ChristmasGetDate_Resp"},
    [MESSAGE.CS_CMD_CHRISTMAS_GET_RANK]     = {Req = "ChristmasGetRank_Req",    Resp = "ChristmasGetRank_Resp"},

    [MESSAGE.CS_CMD_VALENTINE_GET_DATA]     = {Req = "ValentineData_Req",       Resp = "ValentineData_Resp"},
    [MESSAGE.CS_CMD_VALENTINE_EXCHANGE]     = {Req = "ValentineExchange_Req",   Resp = "ValentineExchange_Resp"},

    [MESSAGE.CS_CMD_GET_SEVENDAY_GOAL_LIST] = {Req = "GetSevenGoalList_Req",    Resp = "GetSevenGoalList_Resp"},
    [MESSAGE.CS_CMD_NOTICE_SEVENDAY_GOAL]   = {Req = "",                        Resp = "NoticeSevenGoal_Resp"},
    [MESSAGE.CS_CMD_RETRIVE_SEVENDAY_GOAL]  = {Req = "RetriveSevenGoal_Req",    Resp = "RetriveSevenGoal_Resp"},

    [MESSAGE.CS_CMD_MISSION_LIST]           = {Req = "MissionList_Req",         Resp = "MissionList_Resp"},
    [MESSAGE.CS_CMD_MISSION_UPDATE]         = {Req = "",                        Resp = "MissionUpdate_Resp"},
    [MESSAGE.CS_CMD_MISSION_COMPLETE]       = {Req = "MissionComplete_Req",     Resp = "MissionComplete_Resp"},

    [MESSAGE.CS_CMD_ACTIVITY_LIST]          = {Req = "ActivityList_Req",        Resp = "ActivityList_Resp"},
    [MESSAGE.CS_CMD_ACTIVITY_UPDATE]        = {Req = "",                        Resp = "ActivityUpdate_Resp"},
    [MESSAGE.CS_CMD_ACTIVITY_SUBMIT]        = {Req = "ActivitySubmit_Req",      Resp = "ActivitySubmit_Resp"},
    [MESSAGE.CS_CMD_BATTLE_GIFT]            = {Req = "BattleGift_Req",          Resp = "BattleGift_Resp"},
    [MESSAGE.CS_CMD_BATTLE_SCORE]           = {Req = "BattleScore_Req",         Resp = "BattleScore_Resp"},

    [MESSAGE.CS_CMD_RANK_WEEK_REWARD_INFO]  = {Req = "RankWeekRewardInfo_Req",  Resp = "RankWeekRewardInfo_Resp"},
    [MESSAGE.CS_CMD_RANK_WEEK_REWARD_REWARD]= {Req = "RankWeekRewardReward_Req",Resp = "RankWeekRewardReward_Resp"},

    -- 好友相关
    [MESSAGE.CS_CMD_FRIEND_QUERY]           = {Req = "FriendQuery_Req",         Resp = "FriendQuery_Resp"},
    [MESSAGE.CS_CMD_FRIEND_REQUEST_ADD]     = {Req = "FriendRequestAdd_Req",    Resp = "FriendRequestAdd_Resp"},
    [MESSAGE.CS_CMD_FRIEND_CONFIRM_ADD]     = {Req = "FriendConfirmAdd_Req",    Resp = "FriendConfirmAdd_Resp"},
    [MESSAGE.CS_CMD_FRIEND_GET_INFO]        = {Req = "FriendGetInfo_Req",       Resp = "FriendGetInfo_Resp"},
    [MESSAGE.CS_CMD_FRIEND_REQUEST_REMOVE]  = {Req = "FriendRequestRemove_Req", Resp = "FriendRequestRemove_Resp"},
    [MESSAGE.CS_CMD_FRIEND_ADD_BLACK_LIST]  = {Req = "FriendAddBlackList_Req",  Resp = "FriendAddBlackList_Resp"},
    [MESSAGE.CS_CMD_FRIEND_REMOVE_BLACK_LIST]={Req = "FriendRemoveBlackList_Req",Resp= "FriendRemoveBlackList_Resp"},
    [MESSAGE.CS_CMD_FRIEND_SYC_LIST]        = {Req = "SycFriendList_Req",       Resp = "SycFriendList_Resp"},

    -- 个人房间
    [MESSAGE.CS_CMD_PERSONAL_ROOM_CREATE]   = {Req = "PersonalRoomCreate_Req",  Resp = "PersonalRoomCreate_Resp"},
    [MESSAGE.CS_CMD_PERSONAL_ROOM_ENTER]    = {Req = "PersonalRoomEnter_Req",   Resp = "PersonalRoomEnter_Resp"},
    [MESSAGE.CS_CMD_PERSONAL_ROOM_LEAVE]    = {Req = "PersonalRoomLeave_Req",   Resp = "PersonalRoomLeave_Resp"},
    [MESSAGE.CS_CMD_PERSONAL_ROOM_KICK]     = {Req = "PersonalRoomKick_Req",    Resp = "PersonalRoomKick_Resp"},
    [MESSAGE.CS_CMD_PERSONAL_ROOM_INVITE]   = {Req = "PersonalRoomInvite_Req",  Resp = "PersonalRoomInvite_Resp"},
    [MESSAGE.CS_CMD_PERSONAL_ROOM_BROADCAST]= {Req = "PersonalRoomBroadcast_Req", Resp="PersonalRoomBroadcast_Resp"},
    [MESSAGE.CS_CMD_PERSONAL_ROOM_CHAT]     = {Req = "PersonalRoomChat_Req",    Resp = "PersonalRoomChat_Resp"},
    [MESSAGE.CS_CMD_PERSONAL_ROOM_MODE]     = {Req = "PersonalRoomMode_Req",    Resp = "PersonalRoomMode_Resp"},
    [MESSAGE.CS_CMD_PERSONAL_ROOM_BEGIN_FIGHT] = {Req = "PersonalRoomBeginFight_Req",
                                                                                Resp ="PersonalRoomBeginFight_Resp"},
    [MESSAGE.CS_CMD_PERSONAL_ROOM_APPLY_OR_REFUSE_INVITE] = {Req = "PersonalRoomApplyOrRefuseInvite_Req",
                                                                                Resp = "PersonalRoomApplyOrRefuseInvite_Resp"},
    [MESSAGE.CS_CMD_PERSONAL_ROOM_COUNT_DOWN]={Req = "PersonalRoomCountDown_Req",
                                                                                Resp = "PersonalRoomCountDown_Resp"},
    -- PVE
    [MESSAGE.CS_CMD_PVE_CHECK_POINT_SAVE]   = {Req = "SaveCheckPtData_Req",     Resp = "SaveCheckPtData_Resp"},
    [MESSAGE.CS_CMD_PVE_CHECK_POINT_LIST]   = {Req = "GetCheckPtData_Req",      Resp = "GetCheckPtData_Resp"},
    [MESSAGE.CS_CMD_PVE_CHAPTER_UNLOCK]     = {Req = "ChapterUnlock_Req",       Resp = "ChapterUnlock_Resp"},
    [MESSAGE.CS_CMD_PVE_GET_CHEST]          = {Req = "GetChest_Req",            Resp = "GetChest_Resp"},
    [MESSAGE.CS_CMD_PVE_NOTIFY]             = {Req = "PveNotify_Req",           Resp = "PveNotify_Resp"},
    [MESSAGE.CS_CMD_PVE_CHECK_AWARD]        = {Req = "PveCheckAward_Req",       Resp = "PveCheckAward_Resp"},
    [MESSAGE.CS_CMD_PVE_RANK]               = {Req = "PveScoreRank_Req",        Resp = "PveScoreRank_Resp"},
    [MESSAGE.CS_CMD_TOP_RANK]               = {Req = "TopRank_Req",             Resp = "TopRank_Resp"},
    [MESSAGE.CS_CMD_TOP_RANK_TIME]          = {Req = "TopRankTime_Req",         Resp = "TopRankTime_Resp"},

    -- 雕文系统 Glyphs
    [MESSAGE.CS_CMD_GLYPHS_REQUEST_INFO]    = {Req = "GlyphsRequestInfo_Req",   Resp = "GlyphsRequestInfo_Resp"},
    [MESSAGE.CS_CMD_GLYPHS_REQUEST_BASIC_INFO] = {Req = "GlyphsRequestBasicInfo_Req",
                                                                                Resp = "GlyphsRequestBasicInfo_Resp"},
    [MESSAGE.CS_CMD_GLYPHS_ACTIVE]          = {Req = "GlyphsActive_Req",        Resp = "GlyphsActive_Resp"},
    [MESSAGE.CS_CMD_GLYPHS_UPGRADE]         = {Req = "GlyphsUpgrade_Req",       Resp = "GlyphsUpgrade_Resp"},
    [MESSAGE.CS_CMD_GLYPH_GUIDE_ADDSTUFF]   = {Req = "GlyphGuide_Req",          Resp = "GlyphGuide_Resp"},
    [MESSAGE.CS_CMD_GLYPHS_UPDATE]          = {Req = "NotifyGlyphsUpdate_Req",  Resp = "NotifyGlyphsUpdate_Resp"},

    [MESSAGE.CS_CMD_NEWYEAR_GET_INFO]       = {Req = "",                        Resp = "NewYearGetInfo_Resp"},
    [MESSAGE.CS_CMD_NEWYEAR_REWARD]         = {Req = "",                        Resp = "NewYearReward_Resp"},
    [MESSAGE.CS_CMD_NEWYEAR_CMP]            = {Req = "",                        Resp = "NewYearCmp_Resp"},

    [MESSAGE.CS_CMD_RED_PACKETS_GET_INFO]   = {Req = "RedPacketsGetInfo_Req",   Resp = "RedPacketsGetInfo_Resp"},
    [MESSAGE.CS_CMD_RED_PACKETS_GREB]       = {Req = "RedPacketsGreb_Req",      Resp = "RedPacketsGreb_Resp"},
    [MESSAGE.CS_CMD_RED_PACKETS_OPENED]     = {Req = "RedPacketsOpened_Req",    Resp = "RedPacketsOpened_Resp"},

    -- 每日任务
    [MESSAGE.CS_CMD_DAILY_TASK_STATUS]      = {Req = "DailyTaskStatus_Req",     Resp = "DailyTaskStatus_Resp"},
    [MESSAGE.CS_CMD_REQUEST_DAILY_TASK]     = {Req = "RequestDailyTask_Req",    Resp = "RequestDailyTask_Resp"},
    [MESSAGE.CS_CMD_CHANGE_DAILY_TASK]      = {Req = "ChangeDailyTask_Req",     Resp = "ChangeDailyTask_Resp"},
    [MESSAGE.CS_CMD_OPEN_DAILY_TASK_AWARD]  = {Req = "OpenDailyTaskAward_Req",  Resp = "OpenDailyTaskAward_Resp"},
    [MESSAGE.CS_CMD_UPDATE_DAILY_TASK_PROGRESS] = {Req = "UpdateDailyTaskProgress_Req",
                                                                                Resp ="UpdateDailyTaskProgress_Resp"},
    [MESSAGE.CS_CMD_NEW_DAILY_TASK]         = {Req = "",                        Resp = "NewDailyTask_Resp"},
    [MESSAGE.CS_CMD_UPDATE_TASK_STATUS]     = {Req = "UpdateTaskStatus_Req",    Resp = "UpdateTaskStatus_Resp"},

    --邀请码
    [MESSAGE.CS_CMD_INVITE_CODE]            = {Req = "InviteCode_Req" ,         Resp = "InviteCode_Resp"},
    [MESSAGE.CS_CMD_FIVE_STAR]              = {Req = ""               ,         Resp = "FiveStar_Resp"},
    [MESSAGE.CS_CMD_WATCH_MOVIE]            = {Req = ""               ,         Resp = "WatchMovie_Resp"},
    [MESSAGE.CS_CMD_SKIN_SYNC]              = {Req = "SkinSync_Req",            Resp = "SkinSync_Resp"},
    [MESSAGE.CS_CMD_SKIN_UP_DOWN]           = {Req = "SkinUpDown_Req",          Resp = "SkinUpDown_Resp"},
    [MESSAGE.CS_CMD_SKIN_NEW]               = {Req = "SkinNew_Req",             Resp = "SkinNew_Resp"},
    [MESSAGE.CS_CMD_SKIN_BUY]               = {Req = "SkinBuy_Req",             Resp = "SkinBuy_Resp"},
    [MESSAGE.CS_CMD_SKIN_GUIDE]             = {Req = "SkinGuide_Req",           Resp = "SkinGuide_Resp"},

    -- 付费活动
    [MESSAGE.CS_CMD_LIMIT_EXCHANGE]         = {Req = "LimitExchange_Req",       Resp = "LimitExchange_Resp"},
    [MESSAGE.CS_CMD_SUM_CALL]               = {Req = "SumCall_Req",             Resp = "SumCall_Resp"},

    [MESSAGE.CS_CMD_PAY_SUCCESS]            = {Req = "PaySuccess_Req",          Resp = "PaySuccess_Resp"},
    [MESSAGE.CS_CMD_BIND_OPENID]            = {Req = "BindOpenid_Req",          Resp = "BindOpenid_Resp"},
}

-- 错误码映射表
COMMON.ERRORCODE = {
    OK                                  = 0,    -- 正常

    --登陆相关
    ACCOUNT_LOGIN_EXPECTION             = 1001, -- 登陆异常
    ACCOUNT_AUTH_FAIL                   = 1002, -- 第三方认证失败
    ACCOUNT_NO_LOGIN                    = 1003, -- 无登陆流程
    LOGIN_PLATFORM_NOT_EXIST            = 1004, -- 登录平台不存在
    CENTER_GETSERVER_EXCEPTION          = 1005, -- 中心服获取游戏服异常 表示没有游戏服或者游戏服注册不成功
    FORBID_LOGIN                        = 1006, -- 封号 禁止登录
    LOGIN_GAMEID_NOT_EXIST              = 1007, -- 无逻辑服信息
    VERSION_LOWER                       = 1008, -- 您当前版本过低，请到平台下载最新版本

    --Player相关
    POWER_NOT_ENOUGH                    = 2001, -- 体力不足
    NAME_EXIST_ERROR                    = 2003, -- 名称已存在
    NAME_NOT_CHANGE                     = 2004, -- 昵称没有变化
    POWER_IS_MAX                        = 2005, -- 体力最大值上限
    HAS_GET_WIN_REWARD                  = 2007, -- 当天已经领取过连胜奖励
    FORBID_SPEAK                        = 2008, -- 禁止聊天

    -- 激活码相关
    GIFT_CODE_NOE_EXIST                 = 2009, -- 激活码不存在
    GIFT_TYPE_NOE_EXIST                 = 2010, -- 激活码类型不存在
    GIFT_CODE_IS_USED                   = 2011, -- 激活码已经被使用了
    PARAM_ERROR                         = 2012, -- 参数错误
    GIFT_TYPE_IS_USED                   = 2013, -- 礼包类型已经使用过了
    NAME_ERROR                          = 2014, -- 昵称存在非法字符
    FREE_CHAT_NOT_ENOUGH_ERROR          = 2015, -- 免费聊天次数不足

    --PVP相关
    PVP_NOT_MATCH                       = 3001, -- pvp匹配失败
    NOT_PVP_MATCH_TIME                  = 3003, -- 不在pvp匹配时间内
    NOT_PVP_RANKLIST                    = 3004, -- 排行榜不存在
    PVP_ROOM_NOT_EXIST                  = 3005, -- pvp房间不存在
    NOT_MATCH_STATE                     = 3006, -- 不在匹配状态 无法取消匹配
    SCORE_NOT_ENOUGH                    = 3007, -- 荣誉不够

    PVP_EXCHANGE_ID_INVALID             = 3008, -- 错误的兑换类型编号
    PVP_EXCHANGE_NO_SCORE               = 3009, -- pvp兑换奖励荣誉值不够
    PVP_EXCHANGE_NO_MORE                = 3010, -- 今天已经不能再兑换了
    PVP_EXCHANGE_SELL_OUT               = 3011, -- 此次可兑换的所有人总量全部用完
    PVP_EXCHANGE_NO_SELL                = 3012, -- 商品不在兑换时间内
    PVP_EXCHANGE_OFF_SELL               = 3013, -- 商品已经下架

    NOT_NOPROP_COUNT                    = 3014, -- 没有无道具场次数
    NOT_BUY_NOPROP_COUNT                = 3015, -- 没有无道具场购买次数
    PVP_NOPROP_ROOM                     = 3016, -- 无道具场不能使用道具

    --礼品相关
    GIFT_NOT_EXIST                      = 4001, --  礼包码不存在
    GIFT_NOT_EXIST2                     = 4002, --  礼包码不存在
    ACHIEVEMENT_REWARD_NOT_EXIST        = 4003, -- 成就奖励不存在
    LEVEL_OR_POINT_ERROR                = 4004, -- 成就点数或者等久等级错误
    RECEIVE_ERROR                       = 4005, -- 成就奖励已经领取完了不能重复领取
    ACHIEVEMENTS_NOT_COMPLEMENT         = 4006,  -- 成就没有完成
    VIP_NOT_EXIST                       = 4007,  -- 不是vip用户

    --关卡相关
    STAGE_STAR_COUNT_ERROR              = 5001, -- 通关大关卡,前置关卡或者关卡星星总数不够
    CHAPTER_NO_EXIST                    = 5002, -- 不存在的章节
    CHAPTER_LOCKED                      = 5003, -- 章节还没解锁
    CHEST_REPEAT                        = 5004, -- 宝箱已经领取
    CHAPTER_RELEASE                     = 5005, -- 章节已经解锁
    CHAPTER_NO_REACH                    = 5006, -- 条件没达到
    CP_NO_EXIST                         = 5007, -- 关卡id不存在
    CP_MAX_SKIP                         = 5008, -- 超过跳关卡次数

    -- 道具相关
    ITEM_COUNT_ERROR                    = 6001, --道具数量错误
    DIAMAON_NOT_ENOUGH_ERROR            = 6002, --钻石不够
    ITEM_NOT_EXIST                      = 6003, --物品不存在
    ITEM_NOT_ENOUGH                     = 6004, --物品数量不足
    ITEM_LIMIT                          = 6005, --超过购买上限

    --活动相关
    ACTIVITY_RECEIVE_ERROR              = 6101, -- 活动奖励已领取
    ACTIVITY_NOT_RECEIVE_ROCODE_ERROR   = 6102, -- 没有可领取记录
    ACTIVITY_NOT_START_ERROR            = 6103, -- 活动还未开始
    ACTIVITY_END_ERROR                  = 6104, -- 活动已经结束了
    ACTIVITY_DATA_ERROR                 = 6105, -- 活动数据错误，找不到活动数据
    DISCOUNT_PERIOD_OVERDUE             = 6106, -- 优惠逾期
    SHOP_GIFT_COUNT_ERROR               = 6107, -- 限量的商店礼包数量不足
    TASK_MIS_MATCH                      = 6108, -- 条件没达到
    LIMIT_EXCEED                        = 6109, -- 兑换次数已经用完

    --任务
    MISSION_NOT_EXIST                   = 6201, --任务不存在
    MISSION_ALREADY_COMPLETED           = 6202, --已经完成
    MISSION_NOT_SATISFY_CONDITION       = 6203, --未满足条件

    -- 运营活动相关
    OP_ACT_STATE_NOT_RUN                = 6301, -- 运营活动还未开放
    OP_ACT_EXCHANGE_NO_COUNT            = 6302, -- 【兑换活动】兑换次数不够


    --pvp個人房間
    PERSONAL_ROOM_NOT_ENOUGH_DIAMOND    = 7001, -- 開房鉆石不够
    PERSONAL_ROOM_ALREADY_IN_ROOM       = 7002, -- 已经在房间中或是在自动匹配中
    PERSONAL_ROOM_NOT_EXIST             = 7003, -- pvp个人房间不存在
    PERSONAL_ROOM_ROOM_IS_FULL          = 7004, -- 房间已满
    PERSONAL_ROOM_ROOM_HAS_START        = 7005, -- 房间已经开始

    --邮件,公告相关
    MAIL_ID_NOT_EXIST                   = 8001, -- 邮件不存在

    -- 将军
    AS_TIME_EXPIRED                     = 8100, -- 将军过期
    AS_NOT_EXIST                        = 8101, -- 将军不存在
    AS_LEVEL_MIS                        = 8102, -- 将军当前级别不匹配
    AS_LEVEL_NO                         = 8103, -- 将军级别不存在
    AS_PIECE_LACK                       = 8104, -- 将军碎片不够
    AS_LEVEL_MAX                        = 8105, -- 将军已经升级到最高等级了
    AS_ITEM_LACK                        = 8106, -- 将军升级所需要的材料不够

    -- 时装
    SKIN_NO                             = 8200, -- 不存在此时装
    SKIN_DISABLE                        = 8201, -- 时装已过期
    SKIN_EXPIRE                         = 8202, -- 时装已过期
    SKIN_BUY                            = 8203, -- 无此购买项
}

-- 策划配置表
COMMON.plan = {
    MAP                     = 7,
    Names                   = 19,

    BUILD_INS               = 1,
    AI                      = 2,
    BUILD_ATTR              = 3,
    CHECK_POINT             = 4,
    INSISTUTE_ADD_ATTR      = 5,
    BUFF                    = 6,
    AchievementAward        = 8,
    AchievePointAward       = 9,
    PropAttr                = 10,
    PVP_CONFIG              = 11,
    PVP_RANK                = 12,
    Gift                    = 13,
    Winner                  = 14,
    ActLoginAward           = 15,
    Notice                  = 16,
    PvpMatch                = 17,
    Rank                    = 18,
    Push                    = 20,
    Recharge_Activity       = 21,
    Mission                 = 22,
    Power_Energy_Activity   = 23,
    Newbie_Gift             = 24,
    SHOP_DATA               = 26, -- 商店配置表
    PvpMap                  = 27,
    GradeData               = 30,
    ExchangeData            = 31, -- 荣誉值兑换数据
    ItemBuy                 = 32, -- 物品购买
    Glyphs                  = 35, -- 厨艺
    Vip                     = 36,
    LEVEL                   = 37, -- 经验等级
    NewYear                 = 38, -- 元旦活动
    Bonus                   = 39,
    ChristmasDecorate       = 40,
    ChristmasTree           = 41,
    ChristmasRank           = 42,
    Sys_Mail                = 43, -- 系统邮件列表
    Activity_Time           = 44,
    ActivityRedPackets      = 45,
    RedPacketRatio          = 46,
    PveChapter              = 47, -- pve关卡章节
    WechatShare             = 48, -- 微信分享
    RankWeekReward          = 49, -- 排行榜周奖励
    DAILY_TASK_CONFIG       = 50, -- 每日任务配置
    DAILY_TASK_AWARD        = 51, -- 每日任务宝箱奖励配置
    PVP_NOPROP_AWARD        = 52, -- pvp无道具场奖励
    Const                   = 53, -- 策划全局可配置的值
    Institute               = 54, -- 美味研究院
    Items                   = 55,
    SEVEN_DAY_GOAL          = 56, -- 七日目标
    Reward                  = 57, -- 综合奖励表
    AS_ACTIVE_SKILLS        = 58, -- 精灵主动技能表
    AS_ASSISTANT            = 59, -- 精灵表
    powerBuy                = 62, -- 购买递增数据表
    pvp_chest               = 63, -- 竞技场宝箱配置
    AS_LEVEL                = 64, -- 将军级别表(也就是精灵)
    SKIN                    = 65, -- 时装
    SHIELD_PRICE            = 69, -- 付费引导护盾价格
    ProduceName             = 66, -- 产出来源名字
    Badge                   = 67, -- 勋章表
    PVPAIDATA               = 68, -- AI数据
    PAY_GUIDE               = 70, -- 付费引导
    PvpHappyItems           = 71, -- 欢乐道具场随机道具
}

-- 策划表
local plan = COMMON.plan
COMMON.plan_map = {
    [plan.MAP]                  = "map.lua",
    [plan.Names]                = "names.lua",

    [plan.BUILD_INS]            = "BuildIns.csv",
    [plan.AI]                   = "AI.csv",
    [plan.BUILD_ATTR]           = "BuildAttr.csv",
    [plan.CHECK_POINT]          = "CheckPoint.csv",
    [plan.INSISTUTE_ADD_ATTR]   = "InsistuteAddAttr.csv",
    [plan.BUFF]                 = "Buff.csv",
    [plan.AchievementAward]     = "AchievementAward.csv",
    [plan.AchievePointAward]    = "AchievePointAward.csv",
    [plan.PropAttr]             = "PropAttr.csv",
    [plan.PVP_CONFIG]           = "PvpConfig.csv",
    [plan.PVP_RANK]             = "PvpRank.csv",
    [plan.Gift]                 = "Gift.csv",
    [plan.Winner]               = "Winner.csv",
    [plan.ActLoginAward]        = "ActLoginAward.csv",
    [plan.Notice]               = "Notice.csv",
    [plan.PvpMatch]             = "PvpMatch.csv",
    [plan.Rank]                 = "rank.csv",
    [plan.Push]                 = "push.csv",
    [plan.Recharge_Activity]    = "RechargeActivity.csv",
    [plan.Mission]              = "mission.csv",
    [plan.Power_Energy_Activity] = "Power_energy_activity.csv",
    [plan.Newbie_Gift]          = "NewPlayGift.csv",
    [plan.SHOP_DATA]            = "shop.csv",
    [plan.PvpMap]               = "PvpMap.csv",
    [plan.GradeData]            = "GradeData.csv",
    [plan.ExchangeData]         = "ExchangeItem.csv",
    [plan.ItemBuy]              = "itembuy.csv",
    [plan.Glyphs]               = "Glyph.csv",
    [plan.Vip]                  = "vip.csv",
    [plan.LEVEL]                = "levelConfig.csv",
    [plan.NewYear]              = "newYear.csv",
    [plan.Bonus]                = "Bonus.csv",
    [plan.ChristmasDecorate]    = "decorateData.csv",
    [plan.ChristmasTree]        = "treeData.csv",
    [plan.ChristmasRank]        = "rankBonus.csv",
    [plan.Sys_Mail]             = "sys_mail.csv",
    [plan.Activity_Time]        = "ActivityTime.csv",
    [plan.ActivityRedPackets]   = "ActivityRedPackets.csv",
    [plan.RedPacketRatio]       = "RedPacketRatio.csv",
    [plan.PveChapter]           = "Chapter.csv",
    [plan.WechatShare]          = "share.csv",
    [plan.RankWeekReward]       = "RankWeekReward.csv",
    [plan.DAILY_TASK_CONFIG]    = "dailyMissionNet.csv",
    [plan.DAILY_TASK_AWARD]     = "dailygift.csv",
    [plan.PVP_NOPROP_AWARD]     = "WuDaoJuJiangLi.csv",
    [plan.Const]                = "Const.csv",
    [plan.Institute]            = "Institute.csv",
    [plan.Items]                = "Items.csv",
    [plan.SEVEN_DAY_GOAL]       = "sevenDayMission.csv",
    [plan.Reward]               = "Reward.csv",
    [plan.AS_ACTIVE_SKILLS]     = "AsstActiveSkill.csv",
    [plan.AS_ASSISTANT]         = "AsstData.csv",
    [plan.powerBuy]             = "powerBuy.csv",
    [plan.pvp_chest]            = "PvpChest.csv",
    [plan.AS_LEVEL]             = "AsstLevel.csv",
    [plan.SKIN]                 = "Fashion.csv",
    [plan.PAY_GUIDE]            = "PayGuide.csv",
    [plan.SHIELD_PRICE]         = "ShieldPrice.csv",
    [plan.ProduceName]          = "ProduceName.csv",
    [plan.Badge]                = "Badge.csv",
    [plan.PVPAIDATA]            = "PvpAiData.csv",
    [plan.PvpHappyItems]        = "PvpHappyItems.csv",
}

-- 广播类型
COMMON.broadcast = {
    MARQUEE          = 1, --跑马灯
    ANNCOUNCE        = 2, --通告
}

-- Notice类型
COMMON.notice = {
    STREAK_WIN        = 1, --连胜通知
    STOP_STREAK       = 2, --终结连胜通知
    RANK              = 3, --排名
}

--第三方平台类型 不能修改 定义的字符串 即使是错的
COMMON.platform = {
    [0]    = "inner",
    [1]    = "momo",           -- 陌陌
    [2]    = "qiho360",        -- 奇虎360
    [3]    = "apple",          -- 苹果
    [4]    = "xiaomi",         -- 小米
    [5]    = "lenevo",         -- 联想
    [501]  = "leshop",         -- 联想-乐商店
    [502]  = "gamecenter",     -- 联想-游戏中心
    [6]    = "yidong",         -- 移动
    [601]  = "ydjd",           -- 移动基地
    [602]  = "ydmm",           -- 移动MM
    [7]    = "baidu",          -- 百度
    [701]  = "baidu91",        -- 百度91
    [702]  = "baidushouji",    -- 百度手机助手
    [703]  = "baiduduoku",     -- 百度多酷
    [704]  = "baidutieba",     -- 百度贴吧
    [8]    = "tengxun",        -- 腾讯
    [801]  = "yingyongbao",    -- 腾讯应用宝
    [802]  = "qqweb",          -- 腾讯qq浏览器
    [803]  = "qqgame",         -- 腾讯游戏中心
    [804]  = "qqshichang",     -- 腾讯应用市场
    [805]  = "qq",             -- 手机qq
    [806]  = "weixin",         -- 微信
    [9]    = "uc",             -- uc
    [10]   = "oppo",           -- OPPO
    [11]   = "vivo",           -- VIVO
    [12]   = "huawei",         -- 华为
    [13]   = "kupai",          -- 酷派
    [14]   = "jinli",          -- 金立
    [15]   = "anzhi",          -- 安智
    [16]   = "wandoujia",      -- 豌豆荚
    [17]   = "muzhiwan",       -- 拇指玩
    [18]   = "aiqiyi",         -- 爱奇艺PPS
    [19]   = "pptv",           -- PPTV
    [20]   = "sougou",         -- 搜狗
    [2001] = "sougouhelper",   -- 搜狗手机助手
    [2002] = "sougoushurufa",  -- 搜狗输入法
    [2003] = "sougouweb",      -- 搜狗浏览器
    [2004] = "sougousearch",   -- 搜狗搜索
    [2005] = "sougouyingyong", -- 搜狗应用市场
    [2006] = "sougougame",     -- 搜狗游戏大厅
    [21]   = "sina",           -- 新浪
    [22]   = "tudou",          -- 优酷土豆
    [23]   = "dianxin",        -- 电信爱游戏
    [26]   = "meitu",          -- 美图
    [27]   = "kugou",          -- 酷狗
    [28]   = "wifi",           -- wifi万能钥匙
    [29]   = "meizu",          -- 魅族
    [30]   = "woshop",         -- 沃商店
    [31]   = "zhangmeng",      -- 掌盟
    [32]   = "kaisi",          -- 凯斯冒泡
    [3201] = "kaisishichang",  -- 凯斯冒泡市场
    [3202] = "kaisigame",      -- 凯斯冒泡游戏
    [33]   = "dangle",         -- 当乐
    [34]   = "4399",           -- 4399
    [35]   = "yingyonghui",    -- 应用汇
    [36]   = "jinritoutiao",   -- 今日头条
    [37]   = "xunlei",         -- 迅雷
    [38]   = "zhuoyi",         -- 卓易市场
    [39]   = "oupeng",         -- 欧朋
    [40]   = "XY",             -- XY助手
    [41]   = "leshi",          -- 乐视
    [42]   = "yiwan",          -- 上海益玩
    [4201] = "yiwanqudao",     -- 上海益玩统接推广渠道
    [4202] = "yiwanapp",       -- 子渠道益玩app
    [100]  = "inner2",         -- 自主运营
    [101]  = "memoriki",       -- M+
    [102]  = "fb_ios",       -- facebook ios
    [103]  = "fb_android",   -- facebook android
    [999]  = "Windows",        -- Windows 内网单机版
}

-- oss 平台对应类型 必须和上面的第三方平台类型对应 最大值不能超过255
COMMON.oss_platform = {
    [0]  = "0", -- 内网登录
    [1] = "5", -- 联想
    [2] = "4", -- 小米
    [3] = "6", -- 移动基地
    [4] = "23", -- 电信
    -- 以下的暂定
    [100] = "100", -- 自主运营
    [101] = "101", -- M+
    [102] = "1", -- 陌陌
    [103] = "2", -- 奇虎360
    [104] = "3", -- 苹果
    [105] = "0", -- 自主运营
}

COMMON.GM_TYPE = {
    ADD_POWER           = 1,
    ADD_DIAMOND         = 3,
    SEND_NOTICE         = 4,
    QUERY_INFO          = 5,
    FORBID_SPEAK        = 6,
    FORBID_LOGIN        = 7,
    CHANGE_LEVEL        = 8,
    ADD_SCORE           = 9,
    ADD_PROPERTY        = 10,
    ADD_ITEM            = 11,
    CHANGE_PLAYER_NAME  = 12,
    ADD_VIP             = 13,
    ADD_ACTIVITY        = 14,
    SET_ACTIVITY_STATE  = 15,
    DEL_ACTIVITY        = 16,
}

-- 房间标识
COMMON.room_flag = {
    RAND    = 1, -- 随机匹配
    FIXED   = 2, -- 固定开房
    NOPROP  = 3, -- 无道具场
    MATCH   = 4, -- 比赛房
    HAPPY   = 5, -- 欢乐道具场
}

-- pvp结算类型
COMMON.pvp_end_type = {
    WIN     = 1, -- 胜利
    LOSE    = 2, -- 失败
    DRAW    = 3, -- 平局
}

--货币属性
COMMON.currency_attr = {
    diamond     = 1,
    score       = 2, -- 荣誉
    point       = 3, -- 积分
    chris       = 4, -- 圣诞积分
}

-- 货币、物品、礼包消费和新增来源
COMMON.change_src = {
    recharge     = 1,  -- 充值
    gm           = 2,  -- gm
    mail         = 3,  -- 邮件
    newer        = 4,  -- 新手
    newer_shop   = 5,  -- 新手商城购买引导
    rename       = 6,  -- 修改名称
    mission      = 7,  -- 日常
    pve          = 8,  -- pve
    pvp          = 9,  -- pvp
    pvp_room     = 10, -- pvp个人房间
    pvp_chat     = 11, -- pvp聊天
    pvp_noprop   = 12, -- pvp无道具场
    pvp_recover  = 13, -- pvp使用道具失败恢复
    exchange     = 14, -- 荣誉兑换
    shop         = 15, -- 商城
    buy_item     = 16, -- 商城购买道具
    five_star    = 17, -- 五星
    chest        = 18, -- 开宝箱
    achievement  = 19, -- 成就
    sign         = 20, -- 签到
    gift         = 21, -- 礼包激活码获得
    set_title    = 22, -- 勋章佩戴
    get_title    = 23, -- 勋章获得
    use_item     = 24, -- pve使用道具
    vip          = 25, -- vip
    wechat_share = 26, -- 分享奖励
    institute    = 27, --美食研究院
    buy_item_pve = 28, -- pve中购买道具
    buy_item_pvp = 29, -- pvp中购买道具
    buy_power    = 30, --购买体力
    newer_glyph  = 32, --新手符文引导
    glyph        = 33, --符文操作
    sevenday_goal = 34, --七日目标
    assistant    = 35, -- 将军
    institute_1  = 36,  -- 美食研究院 第一个抽奖
    institute_2  = 37,  -- 美食研究院 第二个抽奖
    institute_3  = 38,  -- 美食研究院 第三个抽奖
    pvp_chest    = 39,  -- 竞技场宝箱


    -- 活动 
    chris        = 40, -- 圣诞
    king         = 41, -- 王者之路
    red_pkg      = 42, -- 抢红包活动
    crazy        = 43, -- 狂欢周
    lover        = 44, -- 情人节
    new_year     = 45, -- 新年
    charge       = 46, -- 首充累充活动
    institute_1_1  = 47,  -- 美食研究院 第一个抽奖免费
    institute_2_1  = 48,  -- 美食研究院 第一个抽奖免费
    institute_3_1  = 49,  -- 美食研究院 第一个抽奖免费

    buy_noprop_count      = 50, -- 购买无道具场次数
    buy_pvp_chest_slot    = 51,  -- 购买竞技场宝箱插槽
    clear_pvp_chest_time  = 52,  -- 清除竞技场宝箱时间
    rank_week_reward      = 53, --周排行榜奖励
    op_act_exchange       = 54, -- 运营活动兑换
    open_pvp_gold_chest   = 55,  -- 立即开启黄金宝箱
    pay_guide_buy         = 56, -- 付费引导物品购买
    pay_guide_buy_shield  = 57, -- 付费引导保护盾购买

    skin               = 58,  -- 皮肤系统
    gm_mail            = 59,      --gm邮件
    opreate_mail       = 60,      --运营活动邮件
    gift_code_mail     = 61,    --激活码邮件
    month_card_mail    = 62,    --月卡邮件
    limit_exchange     = 63,     --限时兑换
    sum_call           = 64,     --累计召唤
    sum_charge         = 65,    -- 累积充值

    test               = 99, -- 测试用的
}

-- 业务日志混合分类
COMMON.busilog_misc_type = {
    misc_share                      = 1, -- 分享
    misc_title                      = 2, -- 称号
    misc_vip                        = 3, -- vip变化
    misc_recharge_reward            = 4, -- 充值活动领奖
    misc_sevenday_sign              = 5, -- 七天签到领奖
    misc_streak_win_reward          = 6, -- 王者之心领奖
    misc_newbie_gift                = 7, -- 新手礼包
    misc_add_gift                   = 8, -- 礼包发放
    misc_achievement                = 9, -- 成就
    misc_fiftyRank                  = 10, -- 前50名排行
    misc_cooker                     = 11, -- 厨艺学习升级
    misc_pve_chest                  = 12, -- pve 星级宝箱领取
    misc_pvp_noprop_award           = 13, -- pvp 无道具场宝箱奖励
    misc_assistant_add              = 14, -- 精灵增加
    misc_institute                  = 15, -- 魔法学院抽奖
    misc_daily_task                 = 16, -- 每日任务产生
    misc_daily_task_done            = 17, -- 每日任务完成
    misc_daily_task_reward          = 18, -- 每日任务钥匙奖励
    misc_first_play_time            = 19, -- 第一次玩的时间
    misc_kind_road                  = 20, -- 王者之路活动操作
    misc_crazy                      = 21, -- 狂欢周活动操作
    misc_sevenday_goal              = 22, -- 七日目标
    misc_top_star                   = 23, -- 巅峰排行榜星星榜
    misc_top_win                    = 24, -- 巅峰排行榜胜利榜
    misc_top_king                   = 25, -- 巅峰排行榜连胜榜
    misc_newbie                     = 26, -- 新手节点变化表
    misc_pvp_chest                  = 27, -- pvp宝箱开启
    misc_chat_barrage               = 28, -- 弹幕聊天
    misc_skin_ori                   = 29, -- 购买皮肤
}

COMMON.misc_action = {
    title_get = 1,         -- 获得称号
    title_set = 2,         -- 设置称号
    act_task_com  = 1,     -- 活动任务完成
    act_task_submit  = 2,  -- 活动任务提交
}


--玩家操作类型，包裹登录，登出等
COMMON.player_operation = {
    player_login      = 1,
    player_login_out  = 0,
    player_create     = 2,
}

-- 活动显示的icon代号
COMMON.activity_icon = {
    ACT_NULL         = -1,
    ACT_SIGN        = 0,  -- 签到
    ACT_FIRST_CHARGE = 1,  -- 首充
    ACT_DAILY_CHARGE = 2,  -- 每日充
    ACT_RECHARGE     = 3,  -- 累计充
    ACT_POWER_ENERGY = 4,  -- 体力精力大放送
    ACT_NEWBIE_GIGT  = 5,  -- 新手礼包
    ACT_CRAZY        = 6,  -- 狂欢周活动
    ACT_CHRISTMAS    = 7,  -- 圣诞活动
    ACT_NEWYEAR      = 8,  -- 元旦活动
    ACT_RED_PACKET   = 13, -- 红包
    ACT_LOVER        = 14, -- 情人节
    ACT_TOP_STAR     = 16,     -- 巅峰->星星排行
    ACT_TOP_WIN      = 17,     -- 巅峰->pvp当天胜场排行
    ACT_TOP_KING     = 18,     -- 巅峰->pvp当天连胜排行


    -- 特殊处理以下三个活动 其他活动一定要在上面加类型
    ACT_KING_ROAD   = 1001, --王者之路
    BRAVE_HEART     = 1002, --勇者之心
    FIGHT_MAD       = 1003, --战斗狂人
}

COMMON.activity_id = {
    ACT_CHRISTMAS = 11,     -- 圣诞活动
    ACT_NEWYEAR   = 12,     -- 新年活动
    ACT_MONEY     = 13,     -- 红包活动
    ACT_CRAZY     = 14,     -- 狂欢周活动
    ACT_LOVER     = 15,     -- 情人节活动

    --特殊活动
    ACT_RANK_WEEK_AWARD = 1001, --每周排行榜奖励活动
}


COMMON.mission_type = {
    MONTH_CARD = 1,
    PVE        = 2,
    AP         = 3,
    RAND_PVP   = 4,
    MULTI_PVP  = 5,
    WIN_PVP    = 6,
    STREAK_WIN = 7,
    VIP1       = 8,
    VIP2       = 9,
    VIP3       = 10,
    SHARE      = 11,
}

COMMON.mail_type = {
    SINGLE = 0,
    GLOBAL = 1
}

--邮件附件类型
COMMON.plugin_type = {
    DIAMOND = 20001,
    POWER   = 20002,
    SCORE   = 20004,
}

--聊天频道
COMMON.channel = {
    ALL_PLAYERS = 1, --全服聊天
    PVP_ROOM    = 2, --房间内聊天
}

--Gameserver服务器编号
COMMON.gamed_map = {
    ["gamed1"] = 1,
    ["gamed2"] = 2,
    ["gamed3"] = 3,
    ["gamed4"] = 4,
    ["gamed5"] = 5,
    ["gamed6"] = 6,
    ["gamed7"] = 7,

    [1] = "gamed1",
    [2] = "gamed2",
    [3] = "gamed3",
    [4] = "gamed4",
    [5] = "gamed5",
    [6] = "gamed6",
    [7] = "gamed7",
}

COMMON.gslb_algorithm = {
    NORMAL_POLL = 1,   --普通轮询
    WEIGHT_POLL = 2,   --加权轮询
    STATIC_RATE = 3,   --静态比例
}

COMMON.property_type = {
    POWER       = 1,
    DIAMOND     = 3,
    ITEM        = 4,
    POWER_SOLT  = 5,    --体力槽
    MONTH_CARD  = 7,    --月卡
    TITLE       = 8,
    ROOM_TICKET = 9,
    EXP         = 10,
    SCORE       = 11,   --荣誉
    STUFF       = 12,   --雕文材料
    POINT       = 13,   --积分
    CHAT        = 14,
    ASSISTANT   = 15,   --宠物助手
    GLYPH       = 16,   --符文
    PIECE       = 17,   --将军碎片
    AS_LEVEL    = 18,   -- 将军升级道具
    FASHION     = 19,   -- 道具对应的时装
}

COMMON.title_type = {
    WIN_GENERAL = 1, --常胜将军
    WIN_GENERAL_2 = 4, --常胜将军2级
    WIN_GENERAL_3 = 6, --常胜将军3级
    BRAVE_HEART = 2, --勇者之心
    FIGHT_MAD   = 3, --战斗狂人
    CHRISTMAS   = 7, --圣诞活动
    NEWER       = 5, -- 新手勋章
}


COMMON.global_war_status = {
    NONE        = -1, --无赛事(仅有后台用到该状态)
    UNSTART     = 0, --未开始
    AUDITION    = 1, --海选
    SEMIFINAL   = 2, --半决赛
    FINAL_EIGHT = 3, --决赛(食神赛)
    FINAL_FOUR  = 4,
    FINAL       = 5,
    AFTER       = 6,
}

-- 奖励类型
COMMON.BONUS_TYPE = {
    ITEM        = 1,
    POWER       = 2,
    DIAMOND     = 4,
    SCORE       = 5,
    STUFF       = 6, -- 雕文材料
    ROOMTICKET  = 7, -- 开房券
    TITLE       = 8, -- 勋章
    -- 其他类型
}

COMMON.sys_mail_id = {
    INVITED_CODE    = 1, --被邀请奖励邮件
    INVITE_CODE     = 2, --邀请人奖励邮件
    GIFT_CODE       = 3, --激活码奖励邮件
    MONTH_CARD      = 4, --月卡登陆奖励发送
}

COMMON.exp_src = {
    PVE = 1,
    PVP = 2,
}

COMMON.act_notify_update_type = {
    SPACE_TIME      = 1, --开启后间隔更新
    CLOCK_TIME      = 2, --开启后每天定点更新
    CUSTOM_CHECK    = 3, --自定义检测通知
}

-- 策划全局配置与Const.csv里面对应
COMMON.const = {
    PVE_SKIP                        = 1, -- pve跳关卡的次数限制
    WIN_RANK                        = 2, -- 巅峰活动pvp胜利场排名结算时间
    KING_RANK                       = 3, -- 巅峰活动pvp连胜场排名结算时间
    PVP_NORMAL_CHEST_MAX_TIME       = 4, -- 竞技场段位宝箱上限时间
    PVP_LUCK_CHEST_COUNT            = 5, -- 竞技场幸运宝箱连胜次数
    PVP_LUCK_CHEST_CD               = 6, -- 竞技场幸运宝箱cd时间
    PVP_NORMAL_CHEST_SOLT_COST1     = 7, -- 竞技场宝箱开启槽位消耗钻石
    PVP_NORMAL_CHEST_SOLT_COST2     = 8, -- 竞技场宝箱开启槽位消耗钻石
    PVP_NORMAL_CHEST_COST_RATE      = 9, -- 竞技场宝箱时间清除消耗比
    PVP_NORMAL_CHEST_MIN_TIME       = 10, -- 竞技场段位宝箱下限时间
    PVP_NORMAL_CHEST_GOLD_RATE      = 11 --竞技场宝箱黄金宝箱概率
}

------------ 运营活动配置 ------------------

-- 运营活动状态
COMMON.OP_ACT_STATE = {
    HIDE        = 1,    -- 添加未启用
    ENABLE      = 2,    -- 启用
    VISIBLE     = 3,    -- 可见
    RUN         = 4,    -- 运行
    EXPIRED     = 5,    -- 过期
}

-- 运营活动类型
COMMON.OP_ACT_TYPE = {
    DEMO           = 1000, -- 运营活动DEMO
    KINGROAD       = 1001, -- 王者之路
    MULTIPLEREWARD = 1002, -- 奖励设置倍数活动
    EXCHANGE       = 1003, -- 兑换活动【端午节活动】
    SCORE_EXCHANGE = 1004, -- 荣誉兑换活动折扣
    NOTICE         = 1005, -- 运营公告活动
    MAIL           = 1006, -- 活动邮件
    CRAZY          = 1007, -- 狂欢周活动
    TOP_STAR       = 1008, -- 巅峰排行榜星星榜
    TOP_WIN        = 1009, -- 巅峰排行榜pvp胜利榜
    TOP_KING       = 1010, -- 巅峰排行榜pvp连胜榜
    LIMIT_EXCHANGE       = 1011, -- 限时兑换活动
    SUM_CALL             = 1012, -- 累计召唤
    BRAVE          = 1013, -- 勇者之心
    BATTLE_CRAZY   = 1014, -- 战斗狂人
    KING_ROAD      = 1015, -- 王者之路
    SUM_RECHARGE   = 1016, -- 累积充值
}

-- 运营活动映射表
COMMON.OP_ACT_MAP = {
    -- 活动类型                                                     活动服务名[文件名]          是否下发客户端(0:不下发, 1:下发)
    [COMMON.OP_ACT_TYPE.DEMO]            = {"operate_activity_demo",             1},
    [COMMON.OP_ACT_TYPE.KINGROAD]        = {"operate_activity_kingroad",         1},
    [COMMON.OP_ACT_TYPE.EXCHANGE]        = {"operate_activity_exchange",         1},
    [COMMON.OP_ACT_TYPE.MULTIPLEREWARD]  = {"operate_activity_multipleReward",   1},
    [COMMON.OP_ACT_TYPE.SCORE_EXCHANGE]  = {"operate_activity_honor_exc_dis",    1},
    [COMMON.OP_ACT_TYPE.NOTICE]          = {"operate_activity_notice",           1},
    [COMMON.OP_ACT_TYPE.MAIL]            = {"operate_activity_mail",             0},
    [COMMON.OP_ACT_TYPE.CRAZY]           = {"operate_activity_crazy",            1},
    [COMMON.OP_ACT_TYPE.TOP_STAR]        = {"operate_activity_top_star",         1},
    [COMMON.OP_ACT_TYPE.TOP_WIN]         = {"operate_activity_top_win",          1},
    [COMMON.OP_ACT_TYPE.TOP_KING]        = {"operate_activity_top_king",         1},
    [COMMON.OP_ACT_TYPE.LIMIT_EXCHANGE]  = {"operate_activity_limit_exchange",   1},
    [COMMON.OP_ACT_TYPE.SUM_CALL]        = {"operate_activity_sum_call",         1},
    [COMMON.OP_ACT_TYPE.BRAVE]           = {"operate_activity_brave",            1},
    [COMMON.OP_ACT_TYPE.BATTLE_CRAZY]    = {"operate_activity_battle_crazy",     1},
    [COMMON.OP_ACT_TYPE.KING_ROAD]       = {"operate_activity_king_road",        1},
    [COMMON.OP_ACT_TYPE.SUM_RECHARGE]    = {"operate_activity_sum_recharge",     1},
}

COMMON.item_btype = {
    PVE         = 1,
    PVP         = 2, 
    GLYPH       = 3, --厨艺材料
    REWARD      = 4, --可开启奖励物品，use_extra 对应Reward表的组
    ASSISTANT   = 5, --宠物助手
    PROPERTY    = 6, --属性值
    PIECE       = 7, --将军碎片
    SKIN        = 8, --时装
}

COMMON.item_stype = {
    DIAMOND       = 1,
    POWER         = 2,
    ENERGY        = 3,
    SCORE         = 4,
    SKIN          = 5, -- 时装
    ROOM_TICKET   = 6, --开放卷
}

COMMON.achievement_type = {
    PVE_PERFECT_1       = 1011,
    PVE_PERFECT_2       = 1012,
    PVE_PERFECT_3       = 1013,
    PVE_USE_PROP_COUNT  = 1031,
    PVE_STAR_COUNT      = 1021,


    PVP_MAX_POINT       = 2011,
    PVP_STREAK_WIN      = 2021,
    PVP_RANDOM_COUNT    = 2031,
    PVP_ROOM_COUNT      = 2041,
    PVP_NON_PROPS_COUNT = 2051,
    PVP_WIN_COUNT       = 2022,
    PVP_USE_PROP_COUNT  = 2061,
    PVP_SCORE           = 2071,

    DAILY_TASK_COUNT    = 3011,
}

COMMON.offline_type = {
    HEARTBEAT_FAIL = 1,
    LOGOUT  = 2,
    CLIENT_CLOSE  = 3
}

-- 道具id
COMMON.ITEMID = {
    ITEM1 = 90001, -- 将军升级用到的道具
}

return COMMON
