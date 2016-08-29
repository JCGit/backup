--
-- Created by IntelliJ IDEA.
-- User: HuangLijun
-- Date: 2015/9/21
-- Time: 16:15
-- Desc: agent的pve模块
--

local agent_pve = {}

local skynet          = require "skynet"

local sharedata_utils = require "sharedata_utils"
local utils           = require "utils"
local setting         = require "setting"
local logger          = require "logger"
local proto           = require "proto"
local COMMON          = require "common"
local MESSAGE         = COMMON.MESSAGE
local cluster         = require "cluster"
local agent_pay_guide = require "agent_pay_guide"

local send_response, send_error_code
local account_info, uin, player
local redis_service, agent
local busilog_service
local traceback = debug.traceback

local CMD

local cp_data = nil -- 关卡数据

-- 关卡章节配置数据
local chapter_cfg = nil
-- 记录最大的章节id 一定要从1递增
local max_chapter = 1

-- 是否刷新到redis
local flush = false


--[[

redis对应:
check_points uin cp_data

old:

cp_data 结构
{
    [1] = {                      -- 章节id
        [1010] = {               -- 关卡id   
            id          = 1010,  -- 关卡id
            stars       = 0,     -- 战斗获得的星星数
            reward      = false, -- 是否通关奖励
            full_reward = false, -- 是否满星奖励
            begin_at  = os.time(),
            win         = 0,    -- 胜利次数
            fail        = 0,    -- 失败次数
            num         = 0,    -- 进入关卡的次数
            reset       = 0,    -- 重置关卡的次数
            exit        = 0,    -- 退出关卡的次数
            new_flag    = true, -- 是否新关卡,创建的时候默认是新的，pve结算后就为false
            score       = 0,    -- 此关卡的得分
        },
        [1020] = {
            id          = 1020,  -- 关卡id
            stars       = 0,     -- 战斗获得的星星数
            reward      = false, -- 是否通关奖励
            full_reward = false, -- 是否满星奖励
            begin_at  = os.time(),
            win         = 0,    -- 胜利次数
            fail        = 0,    -- 失败次数
            num         = 0,    -- 进入关卡的次数
            reset       = 0,    -- 重置关卡的次数
            exit        = 0,    -- 退出关卡的次数
            new_flag    = true, -- 是否新关卡,创建的时候默认是新的，pve结算后就为false
            score       = 0,    -- 此关卡的得分
        }
        max_cp = 1020,   -- 最大关卡号
        lock = false,    -- 是否解锁
        stars = 0,       -- 章节所得到的星星数
        chest = {},      -- 所获得的宝箱
    },
    [2] = {
        [2010] = {
            id          = 2010,  -- 关卡id
            stars       = 0,     -- 战斗获得的星星数
            reward      = false, -- 是否通关奖励
            full_reward = false, -- 是否满星奖励
            begin_at  = os.time(),
            win         = 0,    -- 胜利次数
            fail        = 0,    -- 失败次数
            num         = 0,    -- 进入关卡的次数
            reset       = 0,    -- 重置关卡的次数
            exit        = 0,    -- 退出关卡的次数
            new_flag    = true, -- 是否新关卡,创建的时候默认是新的，pve结算后就为false
            score       = 0,    -- 此关卡的得分
        },
        [2010] = {
            id          = 2010,  -- 关卡id
            stars       = 0,     -- 战斗获得的星星数
            reward      = false, -- 是否通关奖励
            full_reward = false, -- 是否满星奖励
            begin_at  = os.time(),
            win         = 0,    -- 胜利次数
            fail        = 0,    -- 失败次数
            num         = 0,    -- 进入关卡的次数
            reset       = 0,    -- 重置关卡的次数
            exit        = 0,    -- 退出关卡的次数
            new_flag    = true, -- 是否新关卡,创建的时候默认是新的，pve结算后就为false
            score       = 0,    -- 此关卡的得分
        }
        max_cp = 2020,   -- 最大关卡号
        lock = false,    -- 是否解锁
        stars = 0,       -- 章节所得到的星星数
        chest = {},      -- 所获得的宝箱
    }
}

** 关卡详细字段改成数组下标读取
please refer to array_index


节省空间及存储简单改造成新的下面的结构

new:

{
    cp_list = {
        [1010] = {  -- 关卡id
            0,     -- 开始时间
            0,     -- 战斗获得的星星数
            0,    -- bit位集合
            0,    -- 此关卡的得分 
            0,    -- 关卡所属章节
        },
        [2010] = { -- 关卡id
            0,     -- 开始时间
            0,     -- 战斗获得的星星数
            0,    -- bit位集合
            0,    -- 此关卡的得分 
            0,    -- 关卡所属章节
        },
        [3010] = { -- 关卡id
            0,     -- 开始时间
            0,     -- 战斗获得的星星数
            0,    -- bit位集合
            0,    -- 此关卡的得分 
            0,    -- 关卡所属章节
        },
    },
    cp_category = {
        [1] = {
            max_cp = 2020,   -- 最大关卡号
            lock = false,    -- 是否解锁
            stars = 0,       -- 章节所得到的星星数
            chest = {},      -- 所获得的宝箱
        },
        [2] = {
            max_cp = 2020,   -- 最大关卡号
            lock = false,    -- 是否解锁
            stars = 0,       -- 章节所得到的星星数
            chest = {},      -- 所获得的宝箱
        },
        [3] = {
            max_cp = 2020,   -- 最大关卡号
            lock = false,    -- 是否解锁
            stars = 0,       -- 章节所得到的星星数
            chest = {},      -- 所获得的宝箱
        }
    },
    skip_num = 0,
    update = os.time(),
    share = {},
    rank_stars = 0,
    rs_time = os.time(),
    chapter_stars = {[1] = 100}
}
]]

--[[
    begin_at  = os.time(),
    stars       = 0,     -- 战斗获得的星星数
    bits        = 0,    
    score       = 0,    -- 此关卡的得分
    category    = 0,    -- 关卡的类别
]]

--[[

0000      1      1       1         1
        首胜    关卡   完美通关  通关奖励

]]

local pass_bit = 0x01 -- 通关奖励
local full_bit = 0x02 -- 完美通关奖励
local new_bit  = 0x04 -- 是否第一次玩 
local win_bit  = 0x08 -- 关卡首次胜利

-- 数组下标，务必要递增！！
local array_index = {
    begin_at  = 1,       -- 开始时间
    stars     = 2,       -- 战斗获得的星星数
    bits      = 3,       -- 为true fals的bit位集合
    score     = 4,       -- 此关卡的得分
    category  = 5,       -- 关卡所属章节
}

local function load_chapter_cfg()
    chapter_cfg = sharedata_utils.get_stat_all(COMMON.plan.PveChapter)
    for k,v in pairs(chapter_cfg or {}) do
        if k ~= 99 and k > max_chapter  then
            max_chapter = k -- 记录最大的章节id
        end
    end
    if not chapter_cfg then
        logger.debug("load chapter configure failed")
    end
end

-- 初始化所有章节关卡的数据
local function init_cache()
    local cp = {}
    -- 动态读取所有的pve章节数
    -- 99 章节是新手引导章节
    for k,v in pairs(chapter_cfg or {}) do
        if not cp.cp_category then
            cp.cp_category = {}
        end
        if k == 1 or k == 99 then
            cp.cp_category[k] = {max_cp = 0, lock = false, chest = {}, stars = 0} -- max_cp : 最大关卡id  chest : 获得的宝箱  stars : 章节获得的星星数
        else
            cp.cp_category[k] = {max_cp = 0, lock = true, chest = {}, stars = 0} -- max_cp : 最大关卡id  chest : 获得的宝箱 stars : 章节获得的星星数
        end
    end
    cp.cp_list = {}
    cp.chapter_stars = {}     --大关卡行星
    cp.share = {}           -- pve关卡是否可分享的Id
    cp.update = os.time()   -- 缓存更新时间，用来处理每天只能用钻石跳过的次数
    cp.skip_num = 0         -- 已经跳过的次数
    return cp
end

local function chapter_check()
    for k,v in pairs(chapter_cfg or {}) do
        if not cp_data.cp_category[k] then
            if k == 1 or k == 99 then
                cp_data.cp_category[k] = {max_cp = 0, lock = false, chest = {}, stars = 0} -- max_cp : 最大关卡id  chest : 获得的宝箱
            else
                cp_data.cp_category[k] = {max_cp = 0, lock = true, chest = {}, stars = 0} -- max_cp : 最大关卡id  chest : 获得的宝箱
            end

            if k ~= 99 and k > max_chapter  then
                max_chapter = k -- 记录最大的章节id
            end
        end
    end
end

-- 初始当前关卡Id的数据
local function init_cp(category)
    -- 关卡太多，为了节省空间用数组来表示
    return {
        os.time(),  -- 开始时间
        0,          -- 战斗获得的星星数
        4,          -- 为true bit位集合，4代表第3位为1即是新的关卡
        0,          -- 此关卡的得分
        category,   -- 所属章节
    }
end

local function get_cpinfo(uin)
    if uin then
        return skynet.call(redis_service, "lua", "hget_obj", "check_points", uin)
    end
end

-- 读取此用户之前打过的pve战斗数据
local function load_data()
    local cp = skynet.call(redis_service, "lua", "hget_obj", "check_points", account_info.uin)
    if not cp then
        cp = init_cache()
    end

    -- 防止旧的数据没有此字段
    if not cp.chapter_stars then
        cp.chapter_stars = {}
    end

    -- 下面代码是在开发中关卡结构变动为了兼容老数据而加上的
    if not cp.share then
        cp.share = {}
    end

    if not cp.update then
        cp.update = os.time()
    end

    if not cp.skip_num then
        cp.skip_num = 0
    end

    if not cp.rank_stars then
        cp.rank_stars = 0
    end

    if not utils.is_same_day(cp.update, os.time()) then
        cp.skip_num = 0
    end

    return cp
end

-- 获取最大章节的最大关卡数
-- 若最大章节的关卡id为0则显示上一章节的
local function get_max_cp(data)
    -- 从大到小遍历每个章节，遇到解锁的就返回
    for v = max_chapter,1,-1 do
        local tmp = data and data.cp_category and data.cp_category[v] or nil
        if tmp and not tmp.lock then
            if tmp.max_cp == 0 and v ~= 1 then
                local last_chapter = v - 1
                return data.cp_category[last_chapter].max_cp
            end
            return tmp.max_cp
        end
    end
    return 0
end

-- 获取指定章节的最大关卡数
local function get_max_cp_by_chpater(chapter)
    local tmp = cp_data.cp_category[chapter]
    if tmp and not tmp.lock then
        return tmp.max_cp
    else
        return 0
    end
end

-- 得到对应章节的关卡所获得的星星
local function count_chapter_star(chapter, star)
    if cp_data.cp_category and cp_data.cp_category[chapter] and (star ~= 0) then
        cp_data.cp_category[chapter].stars = cp_data.cp_category[chapter].stars + star
    end
end

-- 得到所有章节星星 format : 章节id|星星数#章节id|星星数
local function get_all_star_str()
    -- 动态读取所有的pve章节数
    local star = ''
    for k,v in pairs(chapter_cfg or {}) do
        if #star > 0 then
            star = star .. '#'
        end
        if cp_data.cp_category[k] then
            star = star .. k .. '|' .. cp_data.cp_category[k].stars
        end
    end
    return star
end

-- 得到所有章节的星星总数
local function get_all_stars()
    -- 动态读取所有的pve章节数
    local stars = 0
    -- 从大到小遍历每个章节，遇到解锁的就返回
    for v = max_chapter,1,-1 do
        local tmp = cp_data.cp_category[v]
        if tmp and not tmp.lock then
            stars = stars + tmp.stars
        end
    end
    return stars
end

-- 得到对应章节得到的宝箱数
local function get_chest_num(chest_info)
    local num = 0
    for k,v in pairs(chest_info or {}) do
        if v == true then
            num = num + 1
        end
    end
    return num
end

local function chapter_exist(id)
    if cp_data.cp_category[id] then
        return true
    end
    return false
end

--获取当前关卡的满星关卡数
local function get_max_star(point_config)
   local plan_max_star = 3    --当前最大星星数
   local full_star = 0
   for i=1, plan_max_star do
      local star_id = "star" .. tostring(i)
      if point_config[star_id] == 0 then break end
      full_star = full_star + 1
   end

   return full_star
end

local function update_pve_score(id, uin, name, avator, score)
    cluster.call("commonrank", ".score_rank_service", "update_pve_rank", id, uin, name, avator, score)
end


-- pve战斗结算
local function handle_checkpoint_save(cmd, content, len)
    local cp_info = proto.parse_proto_req(cmd, content, len)

    local to_save_cp = cp_info.CPData
    local chapter = to_save_cp.ChapterId -- 关卡所在章节号(1,2,3,99)

    if not chapter_exist(chapter) then
        logger.debugf("handle_checkpoint_save not exist chapter[%d]", chapter)
        return send_error_code(cmd, COMMON.ERRORCODE.CHAPTER_NO_EXIST)
    end

    local cp_cache = cp_data.cp_list
    local cp_id = to_save_cp.id
    local stars_id = 1     --存储所有获得的星星数的标识
    local win = 0
    local src = COMMON.change_src.pve
    local inc_stars = 0  -- 重复玩关卡增量的星星数
    local new_stars = 0  -- 新关卡得到的星星数

    -- 得到此关卡id的配置
    local point_config = sharedata_utils.get_stat(COMMON.plan.CHECK_POINT, cp_id)
    if not point_config then
        logger.debugf("get check_point configure failed for id[%d]", cp_id)
        return send_error_code(cmd, COMMON.ERRORCODE.CP_NO_EXIST)
    end

    -- 是否跳过
    if to_save_cp.Skip then
        cp_data.skip_num = cp_data.skip_num + 1
        local const_cfg = sharedata_utils.get_stat(COMMON.plan.Const, COMMON.const.PVE_SKIP)
        if const_cfg and cp_data.skip_num > const_cfg.value then
            return send_error_code(cmd, COMMON.ERRORCODE.CP_MAX_SKIP)
        end
        local skip_set = utils.split(point_config.skipValue or '', '#')
        local skip_price = skip_set[cp_data.skip_num]
        if skip_price and tonumber(skip_price) > 0 then
            player.change_diamond(account_info, COMMON.change_src.pve, -tonumber(skip_price), true)
        end
    end

    if not cp_cache[cp_id] then
        cp_cache[cp_id] = init_cp(chapter)
    end


    -- 之前此关卡Id的战斗数据
    local cur_cp_info = cp_cache[cp_id]

    -- 数据兼容报错
    if not cur_cp_info[array_index.score] then
        cur_cp_info[array_index.score] = 0
    end

    -- 是否第一次玩此关卡
    local new_flag = false
    local bits = cur_cp_info[array_index.bits]
    if utils.bit_set(bits, new_bit) then
        cur_cp_info[array_index.bits] = utils.zero_bit(bits, new_bit)
        new_stars = to_save_cp.stars
        new_flag = true
    end

    if not cp_data.chapter_stars[stars_id] then
        cp_data.chapter_stars[stars_id] = 0
    end

    -- 记录此关卡得到的最大星星数
    if cur_cp_info[array_index.stars] < to_save_cp.stars then
        if not new_flag then
            inc_stars = to_save_cp.stars - cur_cp_info[array_index.stars]
        end
        cp_data.chapter_stars[stars_id] = cp_data.chapter_stars[stars_id] + (to_save_cp.stars - cur_cp_info[array_index.stars])
        cur_cp_info[array_index.stars] = to_save_cp.stars
    end

    account_info.pve_count = (account_info.pve_count or 0) + 1

    -- 失败or胜利
    if to_save_cp.Win then
        win = 1
    end

    --添加通过奖励
    --未领取过的有效
    local add_exp = 0

    -- 记录当前章节最大关卡数
    if cp_data.cp_category[chapter].max_cp < cp_id then
        cp_data.cp_category[chapter].max_cp = cp_id
    end

    if chapter == 99 then -- 99代表新手训练营
        src = COMMON.change_src.newer
        new_flag = false  --新手训练营不算新关卡
    end

    if utils.bit_set(cur_cp_info[array_index.bits], pass_bit) and to_save_cp.reward then -- 重复通关
        add_exp = add_exp + point_config.repeatEX
    end

    bits = cur_cp_info[array_index.bits]
    if not utils.bit_set(bits, pass_bit) and to_save_cp.Win then -- 首次通关奖励
        if point_config.baseBonus ~= 0 then
            player.add_gift(account_info, point_config.baseBonus, src)
        end
        cur_cp_info[array_index.bits] = utils.set_bit(bits, pass_bit)
        add_exp = add_exp + point_config.firstEX
    end

    bits = cur_cp_info[array_index.bits]
    if not utils.bit_set(bits, full_bit) then   -- 首次满星通关
        local full_star = get_max_star(point_config)
        if full_star > 0 and full_star ==  to_save_cp.stars then
            if point_config.perfectBonus ~= 0 then
                player.add_gift(account_info, point_config.perfectBonus, src)
            end
            cur_cp_info[array_index.bits] = utils.set_bit(bits, full_bit)
            add_exp = add_exp + point_config.maxStarEX
        end
    end

    --agent新增添加经验接口
    if win ~= 0 then -- 失败不在加经验
        player.add_exp(account_info, add_exp, COMMON.exp_src.PVE)
    end

    -- 此次得到的星星数，新关卡就是new_stars的值，重复玩就是inc_stars的值
    local id_star = new_stars + inc_stars

    -- 每次pve结算记录此章节的星星数
    count_chapter_star(chapter, id_star)

    local pay_guide_item = 0
    local pay_guide_count = 0
    if win == 0 then
        pay_guide_item,pay_guide_count = agent_pay_guide.on_pve_lost()
    end

    bits = cur_cp_info[array_index.bits]
    local data = {
        id = cp_id,
        reward = utils.bit_set(bits, pass_bit),
        FullReward = utils.bit_set(bits, full_bit),
        Stars = get_all_star_str(),
        Win = to_save_cp.Win,
        ShareId = 0,
        PayItemId = pay_guide_item,
        PayItemCount = pay_guide_count,
    }

    if to_save_cp.Skip then
        data.SkipNum = cp_data.skip_num
    end

    --是否需要分享
    if to_save_cp.Win and not utils.bit_set(bits, win_bit) then
        cur_cp_info[array_index.bits] = utils.set_bit(bits, win_bit)
        local ret,res = sharedata_utils.isNeedShareByChapId(cp_id)
        if ret then
            data.ShareId = res
            cp_data.share[res] = 1;
        end
    end

    send_response(cmd, data)

    -- 为了防止跨服调用失败报错影响后续逻辑，用skynet.fork  ?性能如何呢？
    if to_save_cp.Score and chapter ~= 99 and cur_cp_info[array_index.score] < to_save_cp.Score then
    -- if to_save_cp.Score and chapter ~= 99  then
        cur_cp_info[array_index.score] = to_save_cp.Score       
        skynet.call(".proxy", "lua", "pve_update", cp_id, uin, account_info.name, account_info.icon_id, to_save_cp.Score)     
    end

 
    -- 更新完美通关和星星数成就
    if utils.bit_set(cur_cp_info[array_index.bits], full_bit) or id_star ~= 0 then
        --成就处理
        player.check_point_achievement(account_info, cp_id, cur_cp_info, stars_id,
            cp_data.chapter_stars[stars_id])
    end

    flush = true

    EventCenter.dispatchEvent(Events.PVE_END, new_flag, to_save_cp.stars, id_star)

    skynet.send(busilog_service, "lua", "log_battle", account_info.uin, "pve", "end", cp_id, win,  to_save_cp.stars, cur_cp_info[array_index.begin_at], to_save_cp.BattleTime, to_save_cp.Score, get_all_stars())

end

function agent_pve.isCanRewardByShare(chapId)
    return cp_data.share[chapId] == 1
end

function agent_pve.setShareRewardFlag(chapId)
    cp_data.share[chapId] = 0
    flush = true
end


-- 获取所有章节已经玩过的pve战斗数据
local function handle_checkpoint_list(cmd, content, len)

    local rsp = {
    }

    -- 得到已经完过的关卡数据
    local chapter_data = {}
    for k,v in pairs(chapter_cfg or {}) do
        local chapter = {}
        if cp_data.cp_category[k] and not cp_data.cp_category[k].lock then
            local cp = {} -- 章节关卡id的战斗数据
            for k1, v1 in pairs(cp_data.cp_list) do
                -- 关卡在table表里面是[id] = {}
                if v1[array_index.category] == k then
                    table.insert(cp, {id = k1, stars = v1[array_index.stars], reward = utils.bit_set(v1[array_index.bits], pass_bit), FullReward = utils.bit_set(v1[array_index.bits], full_bit)})
                end
            end
            chapter.ChapterId = k
            chapter.CpData = cp
            chapter.Stars = cp_data.cp_category[k].stars
            chapter.Chest = get_chest_num(cp_data.cp_category[k].chest)
            chapter.MaxCpId = cp_data.cp_category[k].max_cp
            table.insert(chapter_data, chapter)
        end
    end
    rsp.Data = chapter_data
    rsp.SkipNum = cp_data.skip_num
    return send_response(cmd, rsp)
end


-- 解锁章节
local function handle_chapter_unlock(cmd, content, len)

    local data = proto.parse_proto_req(cmd, content, len)
    local cur_chapter = data.ChapterId
    local to_lock = cp_data.cp_category[cur_chapter]
    local pre_chapter = cp_data.cp_category[cur_chapter - 1]

    -- 判断需要解锁的章节是否存在
    if not to_lock or not pre_chapter then
        logger.errorf("handle_chapter_unlock(...) to lock chapter[%d] for uin[%d]", data.ChapterId, uin)
        return send_error_code(cmd, COMMON.ERRORCODE.CHAPTER_NO_EXIST)
    end

    -- 是否已经解锁
    if not to_lock.lock then
        return send_error_code(cmd, COMMON.ERRORCODE.CHAPTER_RELEASE)
    end

    -- 条件解锁
    if data.UnlockBy == 1 then
        if get_all_stars() >= chapter_cfg[cur_chapter].needScore and 
            pre_chapter.max_cp >= chapter_cfg[cur_chapter].needMissionID then
        else
            return send_error_code(cmd, COMMON.ERRORCODE.CHAPTER_NO_REACH)
        end
    -- 钻石解锁
    elseif data.UnlockBy == 2 then
        local price = chapter_cfg[cur_chapter].needCost
        if pre_chapter.max_cp >= chapter_cfg[cur_chapter].needMissionID then
            if account_info.diamond < price then
                return send_error_code(cmd, COMMON.ERRORCODE.DIAMAON_NOT_ENOUGH_ERROR)  
            end
            player.change_diamond(account_info, COMMON.change_src.pve, -price)
        else
            return send_error_code(cmd, COMMON.ERRORCODE.CHAPTER_NO_REACH)
        end
    else
        logger.errorf("handle_chapter_unlock(...) not found data.UnlockBy[%d] for uin[%d]", data.UnlockBy, uin)
        return
    end

    to_lock.lock = false
    flush = true

    local rsp = {
        ChapterId = data.ChapterId,
        UnlockBy = data.UnlockBy,
    }

    return send_response(cmd, rsp)
end

-- 宝箱获取
local function handle_get_chest(cmd, content, len)
    local data = proto.parse_proto_req(cmd, content, len)
    local chapter_data = cp_data.cp_category[data.ChapterId]
    local src = COMMON.change_src.pve
    -- 是否已经领取
    if chapter_data.chest[data.ChestId] then
        return send_error_code(cmd, COMMON.ERRORCODE.CHEST_REPEAT)
    end

    if data.ChapterId == 99 then -- 新手训练营章节
        src = COMMON.change_src.newer
    end
    -- 发放礼包
    player.add_gift(account_info, data.ChestGift, src)

    -- 记录发放的宝箱
    chapter_data.chest[data.ChestId] = true
    flush = true

    local rsp = {
        ChapterId = data.ChapterId,
        Chest = get_chest_num(chapter_data.chest)
    }

    skynet.send(busilog_service, "lua", "log_misc", COMMON.busilog_misc_type.misc_pve_chest, account_info.uin, data.ChapterId, rsp.Chest)
    return send_response(cmd, rsp)
end

-- pve战斗通知
local function handle_pve_notify(cmd, content, len)

    local data = proto.parse_proto_req(cmd, content, len)
    local chapter_data = cp_data.cp_category[data.ChapterId]

    if not chapter_data then
        return send_error_code(cmd, COMMON.ERRORCODE.CHAPTER_NO_EXIST)
    end

    if not cp_data.cp_list[data.CheckPoint] then
        cp_data.cp_list[data.CheckPoint] = init_cp(data.ChapterId)
    end


    local power_value = 0
    -- 扣体力
    if data.NeedPower then
        local point_config = sharedata_utils.get_stat(COMMON.plan.CHECK_POINT, data.CheckPoint)
        if point_config then
            if point_config.needPower > account_info.power then
                return send_error_code(cmd, COMMON.ERRORCODE.POWER_NOT_ENOUGH)
            end
            power_value = point_config.needPower
            player.add_power(account_info, -power_value)
        end
    end 

    local action = data.Action
    local cp_info = cp_data.cp_list[data.CheckPoint]
    if action == 1 then -- pve战斗开始
        cp_info[array_index.begin_at] = os.time()
    elseif action == 2 then --pve战斗重置
        cp_info[array_index.begin_at] = os.time() -- 重置得重新记录一条开始的数据
    elseif action == 3 then --pve战斗退出

    else
        logger.debugf("the action[%d] not found ", action)
        return
    end
    flush = true
    skynet.send(busilog_service, "lua", "log_battle", account_info.uin, "pve", "begin", data.CheckPoint, power_value, data.Action)
    return send_response(cmd, { NeedPower = data.NeedPower})
end

-- 检查新手训练营奖励
local function handle_pve_check_award(cmdid, content, len)
    -- 获取刚刚通过的新手训练营关卡，检查是否有奖励，检查是否领过
    -- 如果有，返回giftid
    local rsp = { HaveAward = false, GiftID = 0 }
    local cp_id = get_max_cp_by_chpater(99)
    if cp_id ~= 0 then
        local data = cp_data.cp_list[cp_id]
        if data and not data.showed and utils.bit_set(data[array_index.bits], pass_bit) then
            -- 得到此关卡id的配置
            cp_data.cp_list[cp_id].showed = true -- 在新手训练营是否已经展示过
            flush = true
            local point_config = sharedata_utils.get_stat(COMMON.plan.CHECK_POINT, cp_id)
            if not point_config then
                logger.debugf("handle_pve_check_award(...) get configure failed for cp_id[%d]", cp_id)
                return send_error_code(cmd, COMMON.ERRORCODE.CP_NO_EXIST)
            end
            rsp.GiftID = point_config.baseBonus
            rsp.HaveAward = true
        end
    end

    return send_response(cmdid, rsp)
end

local function append_pve_rank(des, src)
    for id,list in pairs(src) do
        -- 此关卡的排行榜 CpId:关卡id Data:此关卡对应的排名
        local rank_data = { CpId = id, Data = {}}
        for index, value in ipairs(list) do
            table.insert(rank_data.Data, { Rank = index, Uin = value.uin, 
                Name = value.name, Avator = value.avator, Score = value.score })
        end
        table.insert(des, rank_data)
    end
end

-- 获取所有关卡的排行榜
local function handle_pve_rank(cmd, content, len)

    -- 通过proxy服务读取缓存
    local list = skynet.call(".proxy", "lua", "get_score_rank")

    local rsp = {}
    local rank_list = {}
    append_pve_rank(rank_list, list)
    rsp.Rank = rank_list
    send_response(cmd, rsp)
end


-- 参数可以忽略
-- 只要是跨天登录或者在线跨天都会调用
local function daily_check(...)
    cp_data.skip_num = 0
    cp_data.update = os.time()
    flush = true
end

local function register_events()
    EventCenter.addEventListener(Events.DAY_CHANGE, daily_check)
end

function agent_pve.chapter_unlock(shopid)
    for k,v in pairs(chapter_cfg or {}) do
        if to_number(v.shopId) == shopid then
            return send_response(cmd, { UnlockBy = 2, ChapterId = k })
        end
    end
end

-- 获取最大章节的最大关卡数
-- 若最大章节的关卡id为0则显示上一章节的
-- 参数说明： query_uin 玩家uin
function agent_pve.get_max_checkpoint_id(query_uin)
    if query_uin and (query_uin ~= uin) then
        local cache = get_cpinfo(query_uin)
        if cache then
            return get_max_cp(cache)
        else
            return 0
        end
    else
        return get_max_cp(cp_data)
    end
end

-- 判断关卡id是否胜利
function agent_pve.played_id( id )
    id = assert(tonumber(id), id)
    if cp_data.cp_list[id] and utils.bit_set(cp_data.cp_list[id][array_index.bits], pass_bit) then
        return true
    end
    return false
end

-- 添加agent消息处理函数
local function add_cmd_handle_mapping(handle_mapping)
    handle_mapping[MESSAGE.CS_CMD_PVE_CHECK_POINT_SAVE] = handle_checkpoint_save
    handle_mapping[MESSAGE.CS_CMD_PVE_CHECK_POINT_LIST] = handle_checkpoint_list
    handle_mapping[MESSAGE.CS_CMD_PVE_CHAPTER_UNLOCK]   = handle_chapter_unlock
    handle_mapping[MESSAGE.CS_CMD_PVE_GET_CHEST]        = handle_get_chest
    handle_mapping[MESSAGE.CS_CMD_PVE_NOTIFY]           = handle_pve_notify
    handle_mapping[MESSAGE.CS_CMD_PVE_CHECK_AWARD]      = handle_pve_check_award
    handle_mapping[MESSAGE.CS_CMD_PVE_RANK]             = handle_pve_rank
end

local function add_cmd(CMD)
    CMD.get_pve_info        = function() return cp_data end
end

function agent_pve.init(args)
    send_response   = args.send_response
    send_error_code = args.send_error_code

    account_info  = args.info
    uin           = args.uin
    player        = args.player
    redis_service = args.redis
    CMD           = args.cmd
    busilog_service = args.busilog_service

    add_cmd(CMD)
    add_cmd_handle_mapping(args.cmd_map)

    load_chapter_cfg()
    cp_data = load_data()
    chapter_check()

    register_events()
end

function agent_pve.check_save_db()
    if flush and cp_data then
        cp_data.update = os.time()
        skynet.call(redis_service, "lua","hset_obj", "check_points", account_info.uin, cp_data)
        flush = false
    end
end

return agent_pve
