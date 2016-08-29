-- sharedata_utils.lua
-- sharedata帮助函数库
-- sharedata.query()中可能含有元表 用skynet.call()传输可能失败 共享内存数据最好用这个库的方法来取
local sharedata = require "sharedata"
local logger = require "logger"
local COMMON = require "common"
local utils = require "utils"

local sharedata_utils = {}

local plan_set
local function get_plan_set()
    if not plan_set then
        plan_set = sharedata.query("plan_set")
    end
    return plan_set
end

function sharedata_utils.get_stat(set_type, id)
    assert(set_type and tonumber(id), set_type, id)
    id = tonumber(id)
    assert(id ~= nil)  --id为空判断
    local temp = get_plan_set()
    if not temp then
        logger.warning("plan_set error", set_type, id)
        return nil
    end

    if not temp[set_type] then
        logger.warning("plan_set error", set_type, id)
        return nil
    end

    if not temp[set_type][id] then
        logger.warning("plan_set error", set_type, id)
        return nil
    end

    return temp[set_type][id]
end

function sharedata_utils.get_stat_all(set_type)
    local temp = get_plan_set()
    if not temp then
        logger.warning("plan_set error", set_type)
        return nil
    end


    if not temp[set_type] then
        logger.warning("plan_set error", set_type)
        return nil
    else
        return temp[set_type]
    end
end

function sharedata_utils.get_rand_name()
    local names = sharedata_utils.get_stat_all(COMMON.plan.Names)
    local SEX = {
        [1] = "man",
        [2] = "woman",
    }
    local index = math.random(1, 2)
    local first_index = math.random(1, #names[SEX[index]]["first"])
    local first_name = names[SEX[index]]["first"][first_index]
    local last_index = math.random(1, #names[SEX[index]]["last"]) or ""
    local last_name = names[SEX[index]]["last"][last_index] or ""

    return first_name .. last_name
end

local map_cache
function sharedata_utils.get_rand_room_type()
    if map_cache == nil then
        map_cache = {}
        for k, _ in pairs(get_plan_set()[COMMON.plan.MAP]) do
            table.insert(map_cache, k)
        end
    end
    return map_cache[math.random(1, #map_cache)]
end

-- 地图筛选规则
function sharedata_utils.get_rand_map(map_type, room_playerinfo, flag)
    local temp = get_plan_set()
    if not temp then
        logger.warning("plan_set error")
        return nil
    end

    if not temp[COMMON.plan.PvpMap] then
        logger.warning("plan_set error")
        return nil
    end

    if not temp[COMMON.plan.PvpMap][flag] then
        logger.warning("plan_set error")
        return nil
    end

    --[[ 测试地图阵营问题【代码保留】
    for flag, v in pairs(temp[COMMON.plan.PvpMap]) do
        if flag == 1 then
            print("sharedata_utils.lua<sharedata_utils.get_rand_map> flag: " .. flag)

            for type, maps in pairs(temp[COMMON.plan.PvpMap][flag]) do
                print("sharedata_utils.lua<sharedata_utils.get_rand_map> type: " .. type)
                for _, data  in pairs(maps) do
                    local camps = sharedata_utils.get_map_camp(type, flag, data.mapid)
                    utils.var_dump(camps)
                    if #camps ~= type then
                        print("=================map_id ================ " .. data.mapid)
                    end
                end
            end
        end
    end
    --]]

    local maps = temp[COMMON.plan.PvpMap][flag][map_type]
    if not maps then
        logger.warning("plan_set error")
        return nil
    end

    -- key: map_id , value: weight
    local data = {}
    for k, v in pairs(maps) do
        data[v.mapid] = v.weight
    end

    for key, value in pairs(maps) do
        local pvp_min_count = tonumber(value.minCount)
        local pvp_max_count = tonumber(value.maxCount)
        local mapid = tonumber(value.mapid)
        for k, v in pairs(room_playerinfo) do
            if k > 0 then
                local pvp_count = utils.getPvpTotalCount(v)
                if pvp_count < pvp_min_count then
                    data[mapid] = 0
                end

                if pvp_max_count ~= -1 then -- 如果为-1 表示无限制
                    if pvp_count > pvp_max_count then
                        data[mapid] = 0
                    end
                end
            end
        end
    end

    local total_weight = 0
    for k, v in pairs(data) do
        total_weight = total_weight + v
    end
    local rand_weight = 0
    if total_weight ~= 0 then
        rand_weight = math.random(1, total_weight)
    else
        rand_weight = 1
    end

    for k, v in pairs(data) do
        if rand_weight <= v then
           return k
        else
            rand_weight = rand_weight - v
        end
    end
    logger.trace("get_rand_map error")
end

function sharedata_utils.get_map_camp(map_type, flag, map_id)
    local temp = get_plan_set()
    if not temp then
        logger.warning("plan_set error")
        return nil
    end

    if not temp[COMMON.plan.MAP] then
        logger.warning("plan_set error")
        return nil
    end

    local pvpMapData = temp[COMMON.plan.PvpMap]
    if not pvpMapData then
        logger.warning("plan_set error")
        return nil
    end
    local filename = ""
    local arr = pvpMapData[flag][map_type]
    for k, v in pairs(arr) do
        if v.mapid == map_id then
            filename = v.filename
            break
        end
    end
    local index = string.match(filename, "^pvp_[%d]+_([%d]+)$")
    local map_data = temp[COMMON.plan.MAP][map_type][tonumber(index)]
    local buildins = temp[COMMON.plan.BUILD_INS]
    if not buildins then
        logger.warning("plan_set error")
        return nil
    end

    local ENUM_CAMP = {
        ECAMP_TYPE_MIN      = 5,    -- PVP类型最小值
        ECAMP_TYPE_Player1  = 5,    --PVP	包子
        ECAMP_TYPE_Player2  = 6,    --PVP	粽子
        ECAMP_TYPE_Player3  = 7,	--PVP	汉堡
        ECAMP_TYPE_Player4  = 8,    --PVP	蛋糕
        ECAMP_TYPE_MAX      = 8,    -- PVP类型最小值
    }
    local camps = {}
    for k, v in pairs(map_data) do
        local init_camp = buildins[k]["initCamp"]
        if init_camp >= ENUM_CAMP.ECAMP_TYPE_MIN and init_camp <= ENUM_CAMP.ECAMP_TYPE_MAX then
            if init_camp ~= 0 then
                local is_exist = false
                for i, v in ipairs(camps) do
                    if v == init_camp then
                        is_exist = true
                    end
                end
                if not is_exist then
                    table.insert(camps, init_camp)
                end
            end
        end
    end
    return camps
end


function sharedata_utils.get_pvp_exp(point, pvp_result, members)
    local exp_str
    local gradeData = sharedata_utils.get_stat_all(COMMON.plan.GradeData)
    for k, v in pairs(gradeData) do
        if point >= v.minPoint and point <= v.maxPoint then
            if pvp_result == COMMON.pvp_end_type.WIN then
                exp_str = v.WinExp
            else
                exp_str = v.FailExp
            end
            break
        end
    end

    if not exp_str then return 0 end

    local index = (members == 2 and 1) or (members == 3 and 2) or (members == 4 and 3) or 1
    local i = 1
    local exp
    for v in string.gmatch(exp_str, '(%d+)') do
        if index == i then
            exp = tonumber(v)
            break;
        end
        i = i + 1
    end

    if not exp then
        logger.debugf("error: not found exp in cfg %s members %d", exp_str, members)
        exp = 0
    end

    return exp
end

-- 根据积分获得段位
function sharedata_utils.get_group_by_point(point)
    local grade = 0
    local id
    local gradeData = sharedata_utils.get_stat_all(COMMON.plan.GradeData)
    for k, v in pairs(gradeData) do
        if point >= v.minPoint and point <= v.maxPoint then
            grade = v.group
            id = v.id
        end
    end

    id = id or 1
    return grade, id
end

-- 计算荣誉
function sharedata_utils.calc_score(pvp_end_type, room_flag, point, is_room_opener)
    local grade, id = sharedata_utils.get_group_by_point(point)
    local gradeData = sharedata_utils.get_stat(COMMON.plan.GradeData, id)

    local score = 0
    if pvp_end_type == COMMON.pvp_end_type.WIN then
        if room_flag == COMMON.room_flag.RAND then
            score = gradeData.RandWinScore
        elseif room_flag == COMMON.room_flag.FIXED then
            if is_room_opener then
                score = gradeData.FixedHostWinScore or 0
            else
                score = gradeData.FixedWinScore
            end
        elseif room_flag == COMMON.room_flag.NOPROP then
            score = gradeData.NopropWinScore
        end
    else
        if room_flag == COMMON.room_flag.RAND then
            score = gradeData.RandLoseScore
        elseif room_flag == COMMON.room_flag.FIXED then
            if is_room_opener then
                score = gradeData.FixedHostLoseScore or 0
            else
                score = gradeData.FixedLoseScore
            end
        elseif room_flag == COMMON.room_flag.NOPROP then
            score = gradeData.NopropLoseScore
        end
    end
    return score
end

-- 积分计算公式常数
local pvp_point_const = {
    [COMMON.room_flag.RAND]     = 40,
    [COMMON.room_flag.NOPROP]   = 40,
}

--[[
随机匹配常数k	   40
无道具匹配常数k    40
胜利加分	P=k*（1-（1/(1+10^(-(玩家积分-对手平均积分)/400))))
失败扣分	P=k*（1/(1+10^(-(玩家积分-对手平均积分)/400)))
]]

local function get_int_score(score)
    local flag = 1 -- 正负数标识
    if score < 0 then
        score = -score
        flag = -1
    end

    if score < 1 then -- 最少保证加1分的变化
        score = 1
    end

    local int_score = math.floor(score)
    local point_score = score - int_score
    if point_score >= 0.5 then -- 小数 四舍五入
        score = int_score + 1
    else
        score = int_score
    end

    return score * flag
end

-- 根据玩家信息获得天梯分
function sharedata_utils.calc_point(pvp_end_type, room_flag, room_playerinfo, account_info, win_uin)
    if room_flag == COMMON.room_flag.FIXED or room_flag == COMMON.room_flag.NOPROP then -- 开房不加积分
        return 0
    end

    if pvp_end_type == COMMON.pvp_end_type.DRAW then -- 平局不加分
        return 0
    end

    -- 有少于5场的人参加比赛 则所有人,包括电脑都不加分
    local map_type = 0 -- 地图类型
    local player_count = 0 -- 真实玩家人数
    local is_all_addscore = true

    for k, v in pairs(room_playerinfo) do
        map_type = map_type + 1
        if k > 0 then
            player_count = player_count + 1
            if v.pvp_point <= 0 then
                is_all_addscore = false
            end
        end
    end

    if not is_all_addscore then
        return 0
    end

    -- 玩家积分
    local my_score = account_info.pvp_point
    if my_score <= 1400 then -- 玩家积分少于1400 输了不再扣分
        if pvp_end_type == COMMON.pvp_end_type.LOSE then
            return 0
        end
    end

    local win_playerinfo
    local lose_playerinfos = {}
    local score_tab = {}
    local win_point = 0
    local lose_max_point = 0
    for k, v in pairs(room_playerinfo) do
        if v.uin == win_uin then
            win_playerinfo = v
            win_point = (v.uin <= 0) and v.real_point or v.pvp_point
        else
            lose_playerinfos[v.uin] = v
            local lose_point = (v.uin <= 0) and v.real_point or v.pvp_point
            lose_max_point = (lose_max_point < lose_point) and lose_point or lose_max_point
        end
    end

    local k = pvp_point_const[room_flag] -- 常数
    local win_addscore = 0 -- 胜利者得到的分数
    local dif = win_point - lose_max_point
    if player_count > 1 then
        if map_type == 2 then
            dif = dif > 200 and 200 or dif
        else -- 3, 4
            dif = dif > 300 and 300 or dif
        end
    end

    local win_addscore = k * (1 - (1/(1 + 10^(-dif/400))))
    score_tab[win_uin] = win_addscore
    if pvp_end_type == COMMON.pvp_end_type.WIN then
        return get_int_score(win_addscore)
    else
        for uin, v in pairs(lose_playerinfos) do
            local self_point = (v.uin <= 0) and v.real_point or v.pvp_point
            local power = (-(self_point - win_point)/400)
            local powerValue = 10^power
            local addscore = -(k * (1/(1 + powerValue)))
            score_tab[uin] = addscore
        end

        local real_score = 0
        local totol_score = 0
        for k, v in pairs(score_tab) do
            if k ~= win_uin then
                totol_score = totol_score + (-score_tab[k])
            end

            if k == account_info.uin then
                real_score = score_tab[k]
            end
        end
        return get_int_score(win_addscore * real_score/totol_score)
    end
end

function sharedata_utils.getBonusById(bonusId)
    local temp = get_plan_set()
    local bonusData = temp[COMMON.plan.Bonus]
    if not bonusData[bonusId] then
        logger.warning("not exist bonus id : " .. bonusId)
        return nil
    else
        return bonusData[bonusId]
    end
end

function sharedata_utils.getItemById(itemId)
    local tmp = get_plan_set()
    local itemData = tmp[COMMON.plan.PropAttr]
    if not itemData[tonumber(itemId)] then
        logger.warning("not exist item id : " .. itemId)
        return nil
    else
        return itemData[tonumber(itemId)]
    end
end

function sharedata_utils.getBonusByPvpType(type, activityId)
    local tmp = get_plan_set()
    if not activityId then
        activityId = COMMON.activity_id.ACT_CHRISTMAS
    end
    local activityData = tmp[COMMON.plan.Activity_Time][activityId]
    local bonusId = 0
    if type == COMMON.pvp_end_type.WIN then
        bonusId = activityData.winBonusId
    else
        bonusId = activityData.loseBonusId
    end
    return bonusId
end

function sharedata_utils.getActivityConfigById(activityId)
    local tmp = get_plan_set()
    return tmp[COMMON.plan.Activity_Time][activityId] or nil
end

function sharedata_utils.getDecorateDataByType(type)
    local tmp = get_plan_set()
    local decorateData = tmp[COMMON.plan.ChristmasDecorate]
    local bonusId = 0
    for k, v in pairs(decorateData) do
        if v.type == type then
            return v
        end
    end
    return nil
end

function sharedata_utils.getChristmasBonusByLevel(level)
    local tmp = get_plan_set()
    local treeData = tmp[COMMON.plan.ChristmasTree]
    local bonusId = treeData[level].BonusId or nil
    return bonusId
end

function sharedata_utils.getChristmasRankBonus()
    local tmp = get_plan_set()
    local data = tmp[COMMON.plan.ChristmasRank]
    local rankBonus = {}
    local indexs = {}
    for k, v in pairs(data) do
        table.insert(indexs, k)
    end
    table.sort(indexs, function(a, b) return a < b end)
    local last_index = 0
    for _, v in ipairs(indexs) do
        for i = last_index + 1, data[v].rank do
            rankBonus[i] = data[v]
        end
        last_index = data[v].rank
    end

    return rankBonus
end

function sharedata_utils.Bonus2Plugin(bonusId)
    local bonusData = sharedata_utils.getBonusById(bonusId)
    local plugins = {}
    for _, v in ipairs(bonusData) do -- 不允许有随机的在奖励里面
        if v.type == COMMON.BONUS_TYPE.ITEM then -- 物品
            table.insert(plugins, v.itemId .. ":" .. v.count)
        elseif v.type == COMMON.BONUS_TYPE.POWER then
            table.insert(plugins, COMMON.plugin_type.POWER .. ":" .. v.value)
        elseif v.type == COMMON.BONUS_TYPE.DIAMOND then
            table.insert(plugins, COMMON.plugin_type.DIAMOND .. ":" .. v.value)
        elseif v.type == COMMON.BONUS_TYPE.SCORE then
            table.insert(plugins, COMMON.plugin_type.SCORE .. ":" .. v.value)
        end
    end
    local pluginsStr = utils.join(plugins, ",")
    return "{" .. pluginsStr .. "}"
end

function sharedata_utils.getChristmasLevelByScore(score)
    local tmp = get_plan_set()
    local treeData = tmp[COMMON.plan.ChristmasTree]
    local levels = {}
    for k, v in pairs(treeData) do
        table.insert(levels, k)
    end
    table.sort(levels, function(a, b) return a < b end)
    local level = 0
    for i, v in ipairs(levels) do
        if score >= treeData[v].score then
            level = treeData[v].level
        end
    end
    return level
end

local share_pvpId = {
    ["win"] = {},
    ["lost"] = {},
}

function sharedata_utils.isNeedShareByPvp(winCount, lostCount)
    local temp = get_plan_set()
    if not temp then
        logger.warning("plan_set error")
        return false
    end

    local confData = temp[COMMON.plan.WechatShare]
    if not confData then
        logger.warning("plan_set error! confData Nil")
        return false
    end

    if #share_pvpId.win == 0 then
        for k,v in pairs(confData) do
            if v["type"] then
                local list =  utils.split(v["type"], "#")
                if #list == 2  then
                    if tonumber(list[1]) == 1 then
                        share_pvpId.win[tonumber(list[2])] = tonumber(v["ID"])
                    elseif  tonumber(list[1]) == 2 then
                        share_pvpId.win[1] = tonumber(v["ID"])
                    elseif  tonumber(list[1]) == 3 then
                        share_pvpId.lost[tonumber(list[2])] = tonumber(v["ID"])
                    end
                end
            end
        end
    end

    if winCount > 0 then
        if share_pvpId.win[winCount] then
            return true,share_pvpId.win[winCount]
        end
    else
        if share_pvpId.lost[lostCount] then
            return true,share_pvpId.lost[lostCount]
        end
    end
    return false
end

local share_chapId = {}
function sharedata_utils.isNeedShareByChapId(chapId)
    local temp = get_plan_set()
    if not temp then
        logger.warning("plan_set error")
        return false
    end

    local confData = temp[COMMON.plan.WechatShare]
    if not confData then
        logger.warning("plan_set error! confData Nil")
        return false
    end

    if #share_chapId == 0 then
        for k,v in pairs(confData) do
            if v["type"] then
                local list =  utils.split(v["type"], "#")
                if #list == 2 and tonumber(list[1]) == 4 then
                    share_chapId[tonumber(list[2])] = tonumber(v["ID"])
                  --  logger.warning("share Data",tonumber(list[2]), tonumber(v["ID"]))
                end
            end
        end
    end

    if share_chapId[chapId] then
        return true,share_chapId[chapId]
    end
    return false
end

function sharedata_utils.check_rand_mode(room_flag, room_type, player_info)
    local flag = false -- 能否进入这个mode的房间
    local temp = get_plan_set()
    local mapData = temp[COMMON.plan.PvpMap][room_flag][room_type]
    local totalCount = utils.getPvpTotalCount(player_info)

    for mapId, v in pairs(mapData) do
        if (totalCount >= tonumber(v.minCount))  then
            if ((tonumber(v.maxCount) == -1) or (tonumber(v.maxCount) ~= -1 and totalCount <= tonumber(v.maxCount))) then
                if v.weight > 0 then
                    flag = true
                end
            end
        end
    end
    return flag
end

function sharedata_utils.get_rand_mode(room_flag, player_info)
    local temp = get_plan_set()
    local pvpMapData = temp[COMMON.plan.PvpMap][room_flag]
    local totalCount = utils.getPvpTotalCount(player_info)

    local tmp_data = {} -- key:mapId value: v
    local totalWeight = 0
    for type, data in pairs(pvpMapData) do
        for mapId, v in pairs(data) do
            if (totalCount >= tonumber(v.minCount))  then
                if ((tonumber(v.maxCount) == -1) or (tonumber(v.maxCount) ~= -1 and totalCount <= tonumber(v.maxCount))) then
                    totalWeight = totalWeight + v.weight
                    tmp_data[mapId] = v
                end
            end
        end
    end

    local rand_weight = math.random(1, totalWeight)
    for mapId, v in pairs(tmp_data) do
        if rand_weight <= v.weight then
            return v.type
        else
            rand_weight = rand_weight - v.weight
        end
    end
end

function sharedata_utils.getAiDataByPlayerInfos(room_playerinfo)
    local scores = {}
    for k, v in pairs(room_playerinfo) do
        if k > 0 then
            table.insert(scores, v.pvp_point)
        end
    end
    table.sort(scores, function(a, b) return a > b end)
    --   1个人 则以该玩家积分作为标准
    -- >=2个人 则
        -- 第一名和第二分差>400  则选第一个人的分数作为标准
        -- 第一名和第二分差<=400 则选两个人的平均分作为标准
    local playerScore = 0
    if #scores <= 0 then
        return nil
    elseif #scores == 1 then
        playerScore = scores[1]
    else
        local difScore = scores[1] - scores[2]
        if difScore > 400 then
            playerScore = scores[1]
        else
            playerScore = math.floor((scores[1] + scores[2])/2)
        end
    end
    local aiScore = (playerScore > 0) and playerScore - math.random(100, 200) or 0

    local temp = get_plan_set()
    local pvpAiData = temp[COMMON.plan.PVPAIDATA]
    local index = 1
    for i, v in pairs(pvpAiData) do
        index = i
        if v.score >= aiScore then
            break
        end
    end
    local randIndex = math.random(1, #(pvpAiData[index].data))
    local data = pvpAiData[index].data[randIndex]
    while true do
        local is_exist = false
        for k, v in pairs(room_playerinfo) do
            if v.name == data.name then
                is_exist = true
            end
        end

        if not is_exist then
            break
        else
            data.name = sharedata_utils.get_rand_name()
        end
    end
    return data
end

function sharedata_utils.getAsstSkillByLevel(asstid, level)
    local data = sharedata_utils.get_stat(COMMON.plan.AS_LEVEL, tonumber(string.format("%d%02d", asstid, (level == 0 and 1 or level))))
    assert(data, " data error assit " .. asstid)
    return data.SkillId
end

return sharedata_utils

