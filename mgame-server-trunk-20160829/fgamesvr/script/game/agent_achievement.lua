--  成就相关
local skynet = require "skynet"
local utils = require "utils"
local sharedata_utils = require "sharedata_utils"
local COMMON = require "common"
local redis_service
local agent_achievement = {}
local proto = require "proto"

local player
local account_info
local agent

local send_package, send_response, send_error_code
local busilog_service

agent_achievement.type = {
    receive_point = 1, -- 完成领取成就点数
    receive_level = 2, -- 领取成就等级奖励
}

-- 获取某个成就的完成进度
local function getCompleteCount(category, condition, player_rank_info)
    -- 评星类的直接返回
    if category == 2 then return end

    local condition_data = utils.split(condition,"#")
    local achieve_category = condition_data[1]
    local achieve_type = condition_data[2]
    local achieve_count = condition_data[3]
    if achieve_category == nil or achieve_type == nil or achieve_count == nil then return end
    achieve_category = tonumber(achieve_category)
    achieve_type = tonumber(achieve_type)
    local count = 0
    if (category == 1) and (achieve_category == 1) then -- 关卡——通关
        return
    elseif (category == 1) and (achieve_category == 2) then -- 关卡——pve道具使用次数
        return account_info.pve_use_items
    elseif category == 3 and achieve_category == 1 then -- 对站——赛季排名
        if player_rank_info == nil then return count end
        local silver = player_rank_info[2]
        local gold = player_rank_info[3]
        local diamond = player_rank_info[4]
        local king = player_rank_info[5]

        if silver ==nil then silver = 0 end
        if gold ==nil then gold = 0 end
        if diamond ==nil then diamond = 0 end
        if king ==nil then king = 0 end

        local diamond_count = diamond + king
        local gold_count = diamond_count + gold
        local silver_count = gold_count + silver

        if achieve_type == 1 then -- 青铜以上
            count = silver_count
        end
        if achieve_type == 2 then -- 白银以上
            count = gold_count
        end
        if achieve_type == 3 then -- 黄金以上
            count = diamond_count
        end
        if achieve_type == 4 then -- 争霸王者
            count = king
        end
        return count
    elseif (category == 3) and (achieve_category == 2) then -- 对站——对站
        if achieve_type == 5 then -- 累积随机对战
            count = account_info.rand_room_count
        elseif achieve_type == 6 then -- 累积开房对战
            count = account_info.fixed_room_count
        elseif achieve_type == 7 then -- 累计无道具场次数
            count = account_info.pvp_nopro_count
        elseif achieve_type == 8 then -- 段位积分(最大积分)
            count = account_info.pvp_max_point
        elseif achieve_type == 9 then -- 连胜成就(最大连胜记录)
            count = account_info.max_streak_win
        elseif achieve_type == 10 then -- PVP道具累计使用
            count = account_info.pvp_use_items
        elseif achieve_type == 11 then -- 累计对战胜利场次
            count = account_info.pvp_win_count
        elseif achieve_type == 12 then --累计荣誉
            count = account_info.pvp_addup_score
        elseif achieve_type == 13 then
            count = account_info.daily_task_count
        else
        end
        return count
    end
end


-- 领取成就点数
local function receive_achievement_point(account_info,achievementId)
    local ret = COMMON.ERRORCODE.OK
    local achievements = skynet.call(redis_service, "lua", "hget_obj", "achievement", account_info.uin)
    local achievement_info = skynet.call(redis_service, "lua", "hget_obj", "achievement_info", achievementId)
    if achievements then
        local update = false
        for k, v in pairs(achievements) do
            if v.Id == achievementId then
                update = true
                if v.Status == 1 then
                    v.Status = 0
                    achievements[k] = v
                    account_info.achievement_point = account_info.achievement_point + v.Award
                    if achievement_info then
                        achievement_info.count = achievement_info.count + 1
                    else
                        achievement_info = {
                            achievementId = achievementId,
                            count = 1,
                        }
                    end
                    skynet.call(redis_service, "lua", "hset_obj", "achievement_info", achievementId, achievement_info)
--                    skynet.call(redis_service, "lua", "hset_obj", "account", account_info.uin, account_info)
                    player.change_flag(account_info)
                    skynet.call(redis_service, "lua", "hset_obj", "achievement", account_info.uin, achievements)
                    skynet.send(busilog_service, "lua", "log_misc", COMMON.busilog_misc_type.misc_achievement, account_info.uin, achievementId, account_info.achievement_point, v.Award)
                else
                    ret = COMMON.ERRORCODE.RECEIVE_ERROR
                end
            end
        end
        if not update then
            ret = COMMON.ERRORCODE.ACHIEVEMENTS_NOT_COMPLEMENT
        end
    else
        ret = COMMON.ERRORCODE.ACHIEVEMENTS_NOT_COMPLEMENT
    end
    return ret
end


-- 获取角色成就信息
local function get_achievement_data(account_info, achievements, achievementDatas, achievement_infos)
    local achievementDatas = sharedata_utils.get_stat_all(COMMON.plan.AchievementAward) -- 所有成就
    local achievements = skynet.call(redis_service, "lua", "hget_obj", "achievement", account_info.uin)
    local achievement_infos = skynet.call(redis_service, "lua", "hgetall_obj", "achievement_info") -- 所有成就领取个数数据
    local player_rank_info = skynet.call(redis_service, "lua", "hget_obj", "player_rank_info", account_info.uin,true)
    local data = {}
    local achievementPointCount = 0
    for _,achievementData in pairs(achievementDatas) do
        local update = false
        local achievement_info = achievement_infos[achievementData.Id]
        if not achievement_info then
            achievement_info = {
                achievementId = achievementData.Id,
                count = 0,
            }
        end
        local count = getCompleteCount(achievementData.Category, achievementData.Condition, player_rank_info)
    
        if achievements then
            for _, v in pairs(achievements) do
                if v.Id == achievementData.Id then -- 玩家有该成就的
                    local achieve = {
                        Id = v.Id,
                        Status = v.Status,
                        CompleteCount = count,
                        ReceivePlayerCount = achievement_info.count
                    }
                    if v.Status == 0 then
                        achievementPointCount = achievementPointCount + v.Award
                    end
                    update = true
                    table.insert(data, achieve)
                end
            end

            if not update then
                local achieve = {
                    Id = achievementData.Id,
                    Status = 2,
                    CompleteCount = count,
                    ReceivePlayerCount = achievement_info.count
                }
                table.insert(data, achieve)
            end
        else
            local achieve = {
                Id = achievementData.Id,
                Status = 2, -- 未完成状态
                CompleteCount = count,
                ReceivePlayerCount = achievement_info.count
            }
            table.insert(data, achieve)
        end
    end
    return data, achievementPointCount
end


local function test_update_rank_achieve()
    local player_rank_info = skynet.call(redis_service, "lua", "hget_obj", "player_rank_info", account_info.uin)
    if player_rank_info == nil then
        player_rank_info = {}
    end
    for i=1,5,1 do
        local rank_count = player_rank_info[i]
        if rank_count == nil then
            rank_count = 1
            player_rank_info[i] = rank_count
        else
            rank_count = rank_count + 1
            player_rank_info[i] = rank_count
        end
    end
    skynet.call(redis_service, "lua", "hset_obj", "player_rank_info", account_info.uin, player_rank_info)
    player.check_pvp_rank_achievement(account_info, player_rank_info)
end

function agent_achievement.update_achievement()
    local update = false
    local list = {}
    local achievements = skynet.call(redis_service, "lua", "hget_obj", "achievement", account_info.uin, true) -- 角色已标记的成就
    if not achievements then
        achievements = {}
    end
    local achievementData = sharedata_utils.get_stat_all(COMMON.plan.AchievementAward) -- 所有成就
    local new_value = table.insert -- 加快频繁时的访问速度
    for _, a in pairs(achievementData) do
        -- 2#5#条件值
        -- 拆分条件字符串
        local cond_param
        local param1
        local param2
        local param3

        if a.Category == 1 or a.Category == 3 then
            cond_param = utils.split(a.Condition, "#")
            param1= tonumber(cond_param[1])
            param2= tonumber(cond_param[2])
            param3= tonumber(cond_param[3])
        end

        if not achievements[a.Id] or achievements[a.Id].Status ~= 0 then
            if a.Category == 1 then -- 关卡类别
                if param1 == 2 then -- pve道具使用次数
                    if account_info.pve_use_items >= param3 then
                        if not achievements[a.Id] then
                            achievements[a.Id] = {
                                Id = a.Id,
                                Status = 1, -- 1 : 代表成就可以领取 0 : 已经领取 2 : 未完成
                                Award = a.Award
                            }
                            update = true
                            new_value(list, a.Id)
                        end
                    end
                else
                end
            elseif a.Category == 3 then -- 对站类别          
                if param1 == 2 and param2 == 5 then -- 随机pk
                    if account_info.rand_room_count >= param3 then
                        if not achievements[a.Id] then
                            achievements[a.Id] = {
                                Id = a.Id,
                                Status = 1, -- 1 : 代表成就可以领取 0 : 已经领取 2 : 未完成
                                Award = a.Award
                            }
                            update = true
                            new_value(list, a.Id)
                        end
                    end  
                elseif param1 == 2 and param2 == 6 then -- 开房pk
                    if account_info.fixed_room_count >= param3 then
                        if not achievements[a.Id] then
                            achievements[a.Id] = {
                                Id = a.Id, 
                                Status = 1, -- 1 : 代表成就可以领取 0 : 已经领取 2 : 未完成
                                Award = a.Award
                            }
                            update = true
                            new_value(list, a.Id)
                        end
                    end
                elseif param1 == 2 and param2 == 7 then -- 累计无道具场次数
                    if account_info.pvp_nopro_count >= param3 then
                        if not achievements[a.Id] then
                            achievements[a.Id] = {
                                Id = a.Id,
                                Status = 1,   -- 1 : 代表成就可以领取 0 : 已经领取 2 : 未完成
                                Award = a.Award
                            }
                            update = true
                            new_value(list, a.Id)
                        end
                    end
                elseif param1 == 2 and param2 == 8 then -- 段位积分(最大积分)
                    if account_info.pvp_max_point >= param3 then
                        if not achievements[a.Id] then
                            achievements[a.Id] = {
                                Id = a.Id,
                                Status = 1,   -- 1 : 代表成就可以领取 0 : 已经领取 2 : 未完成
                                Award = a.Award
                            }
                            update = true
                            new_value(list, a.Id)
                        end
                    end
                elseif param1 == 2 and param2 == 9 then -- 连胜成就(最大连胜记录)
                    if account_info.max_streak_win >= param3 then
                        if not achievements[a.Id] then
                            achievements[a.Id] = {
                                Id = a.Id,
                                Status = 1,   -- 1 : 代表成就可以领取 0 : 已经领取 2 : 未完成
                                Award = a.Award
                            }
                            update = true
                            new_value(list, a.Id)
                        end
                    end  
                elseif param1 == 2 and param2 == 10 then -- PVP道具累计使用次数
                    if account_info.pvp_use_items >= param3 then
                        if not achievements[a.Id] then
                            achievements[a.Id] = {
                                Id = a.Id,
                                Status = 1,   -- 1 : 代表成就可以领取 0 : 已经领取 2 : 未完成
                                Award = a.Award
                            }
                            update = true
                            new_value(list, a.Id)
                        end
                    end
                elseif param1 == 2 and param2 == 11 then -- 累计对战胜利场次
                    if account_info.pvp_win_count >= param3 then
                        if not achievements[a.Id] then
                            achievements[a.Id] = {
                                Id = a.Id,
                                Status = 1,   -- 1 : 代表成就可以领取 0 : 已经领取 2 : 未完成
                                Award = a.Award
                            }
                            update = true
                            new_value(list, a.Id)
                        end
                    end
                elseif param1 == 2 and param2 == 12 then --累计荣誉
                    if account_info.pvp_addup_score >= param3 then
                        if not achievements[a.Id] then
                            achievements[a.Id] = {
                                Id = a.Id,
                                Status = 1,   -- 1 : 代表成就可以领取 0 : 已经领取 2 : 未完成
                                Award = a.Award
                            }
                            update = true
                            new_value(list, a.Id)
                        end
                    end
                elseif param1 ==2 and param2 == 13 then --累计每日任务完成次数
                    if account_info.daily_task_count >= param3 then
                        if not achievements[a.Id] then
                            achievements[a.Id] = {
                                Id = a.Id,
                                Status = 1,   -- 1 : 代表成就可以领取 0 : 已经领取 2 : 未完成
                                Award = a.Award
                            }
                            update = true
                            new_value(list, a.Id)
                        end
                    end
                else
                    -- 不需要处理
                end
            end
        else 
            -- 其它类别暂时不需要处理
        end
    end
    if update then
        skynet.call(redis_service, "lua", "hset_obj", "achievement", account_info.uin, achievements)
        local data = { -- 发送状态可领取的成就id值 前端把完成进度直接置成条件值
           ListIds = list
        }
        send_response(COMMON.MESSAGE.CS_CMD_UPDATE_ACHIEVEMENT,  data)
    end
end

-- 获取成就信息
function agent_achievement.handle_get_achievement(cmd, coontent, len)
--    test_update_rank_achieve()
    local data, achievementPointCount = get_achievement_data(account_info)
    if account_info.achievement_point ~= achievementPointCount then
        account_info.achievement_point = achievementPointCount
--        skynet.call(redis_service, "lua", "hset_obj", "account", account_info.uin, account_info)
        player.change_flag(account_info)
    end
    local resp = {
        Data = data,
        AchievementPointCount = achievementPointCount,
        AchievementLevel = account_info.achievement_level
    }
    return send_response(cmd, resp)
end

-- 领取成就点数奖励
function agent_achievement.handle_receive_achievement_point(cmd, content, len)
    local achievementId = proto.parse_proto_req(cmd, content, len).AchievementId
    local ret = receive_achievement_point(account_info, achievementId)
    local resp = {
        Ret = ret,
        AchievementId = achievementId
    }
    return send_response(cmd, resp)
end

-- 领取成就奖励
function agent_achievement.handle_receive_achievement_reward(cmd, content, len)
    local ret = COMMON.ERRORCODE.OK
    local achievementLevel = proto.parse_proto_req(cmd, content, len).AchievementLevel
    local achievementRewardData = sharedata_utils.get_stat(COMMON.plan.AchievePointAward, achievementLevel)
    if achievementRewardData then
        if account_info.achievement_level == achievementLevel and account_info.achievement_point >= achievementRewardData.AchievePoint then
            --领取成就点数奖励
            local gift_data = sharedata_utils.get_stat(COMMON.plan.Gift, achievementRewardData.GiftId)
            if gift_data then
                player.add_gift(account_info, achievementRewardData.GiftId, COMMON.change_src.achievement)
                account_info.achievement_level = achievementLevel + 1
--                skynet.call(redis_service, "lua", "hset_obj", "account", account_info.uin, account_info)
                player.change_flag(account_info)
                skynet.send(busilog_service, "lua", "log_misc", COMMON.busilog_misc_type.misc_achievement, account_info.uin, account_info.achievement_level, account_info.achievement_point, 0)
            else
                ret = COMMON.ERRORCODE.GIFT_NOT_EXIST
            end
        else
            ret = COMMON.ERRORCODE.LEVEL_OR_POINT_ERROR
        end
    else
        ret = COMMON.ERRORCODE.ACHIEVEMENT_REWARD_NOT_EXIST
    end
    local resp = {
        Ret = ret,
        AchievementLevel = achievementLevel
    }
    send_response(cmd, resp)
end

local function add_cmd(CMD)

end

local function add_cmd_handle_mapping(cmd_handle_mapping)
    cmd_handle_mapping[COMMON.MESSAGE.CS_CMD_GET_ACHIEVEMENT]               =   agent_achievement.handle_get_achievement
    cmd_handle_mapping[COMMON.MESSAGE.CS_CMD_RECEIVE_ACHIEVEMENT]           =   agent_achievement.handle_receive_achievement_point
    cmd_handle_mapping[COMMON.MESSAGE.CS_CMD_RECEIVE_ACHIEVEMENT_REWARD]    =   agent_achievement.handle_receive_achievement_reward
end

function agent_achievement.init(args)
    redis_service = skynet.queryservice("redis_service")
    account_info = args.info
    agent = args.agent
    send_package = args.send_package
    send_response = args.send_response
    send_error_code = args.send_error_code
    player = args.player
    -- 模块必须实现的接口
    add_cmd(args.cmd)
    add_cmd_handle_mapping(args.cmd_map)

    busilog_service = args.busilog_service
end

return agent_achievement

