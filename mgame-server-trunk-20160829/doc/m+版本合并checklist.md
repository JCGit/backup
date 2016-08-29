# M+版本合并checklist

	auth: diandian
	date: 2016.5.10
	
	A : add
	M : modify
	D : delete

## script/

* `center_main.lua`: 
  ```lua
  #A -> in line 47
  --邀请码生成服务
  local invite_code_service = skynet.uniqueservice("invite_code_service")
  skynet.name(".invite_code_service", invite_code_service)
  ```
* `activity_power_energy.lua`:
  ```lua
  #M -> in function "receive_reward" and in line 135
  player.add_power(account_info, 20)
  ```
* `activity_recharge.lua`:
  ```lua
  #M -> in function "activity_recharge.get_open_activity" and in line 45
  if recharge_data.recharge_type == recharge_enum.first_recharge then
                local activity_info = skynet.call(redis_service, "lua", "hget_obj", "activity_info", account_info.uin, true) --玩家领取活动记录数据 key为uin
                if activity_info == nil then
                    data[COMMON.activity_icon.ACT_FIRST_CHARGE] = 1
                else
                    local key = account_info.uin .. "_" .. recharge_data.id .. "_" .. recharge_data.stage
                    if activity_info[key] and activity_info[key].status == 1 then
                        data[COMMON.activity_icon.ACT_FIRST_CHARGE] = 1
                    end
                end
            end
            if recharge_data.recharge_type == recharge_enum.everyday_recharge then
                data[COMMON.activity_icon.ACT_DAILY_CHARGE] = 1-- 目前是取消此类了，以后需要再打开
            end
  ```
* `agent_player.lua`:
  ```lua
  #A -> in function "handle_query_playerinfo" and in line 204
  if not player_info.area or player_info.area == 0 then
      player_info.area = 7
  end

   #A -> in function "handle_sys_playerinfo" and in line 265
  if not player_info.area or player_info.area == 0 then
      player_info.area = 7
  end

  #A -> in function "handle_sys_playerinfo" and in line 294
          InviteCode = account_info.invite_code,
    }
    if account_info.is_invite_code == nil or account_info.is_invite_code == 0 then
        data.CanInvite = true
    else
        data.CanInvite = false
    end
    
  #A -> in line 519
  --新加三个function
  local function handle_invite_code(cmd, content, len)
  local function handle_five_star(cmd, content, len)
  local function handle_watch_movie(cmd, content, len)

  #A -> in function "add_cmd_handle_mapping" and in line 607
  cmd_handle_mapping[COMMON.MESSAGE.CS_CMD_INVITE_CODE]   = 	  handle_invite_code
  cmd_handle_mapping[COMMON.MESSAGE.CS_CMD_FIVE_STAR]     =   handle_five_star
  cmd_handle_mapping[COMMON.MESSAGE.CS_CMD_WATCH_MOVIE]   = handle_watch_movie
  ```
* `agent_shop.lua`:
  ```lua
  #M -> in function "handle_buy_gift" and in line 281
  local r = skynet.call(redis_service, 'lua', 'eval', lua_script, 1, 'shop_info', field, left_count)
  if r ~= 'ok' then return send_error_code(cmd, ERRORCODE.SHOP_GIFT_COUNT_ERROR) end
  ```
* `gameserver.lua`:
  ```lua
  #A -> in function "CMD.login" and in line 478
  local code = cluster.call("centernode", ".invite_code_service", "get_invite_code")
  local code_uin = skynet.call(redis_service, "lua", "hget_obj", "invite_code_mapping", code)
  if code_uin ~= nil then
      logger.error("invite code create a exist code : ", code)
  end
  logger.tracef("player invite_code %d  %s", uin, code)
  account_info.invite_code = code
  skynet.call(redis_service, "lua", "hset_obj", "invite_code_mapping", account_info.invite_code, uin);
  ```
* `gslb_main_od.lua and gslbserver_od.lua`:
  ```lua
  #A -> 由于可能有老客户端用老登陆方式更新游戏，所以保留老版本gslb登陆。
  ```



## setting/

* `setting.lua`:

  ```lua
  setting.contant.player_personal_room_create_price = 8, -- 玩家创建个人房间需要的钻石数
  ```

  ​