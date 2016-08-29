local Hp_pb = require "HP_pb"
local Player_pb = require ("Player_pb")
local Reward_pb = require "Reward_pb"
local Notice_pb = require "Notice_pb"
local MessageFlowPopPage = require "MessageFlowPopPage"
local Chat_pb = require "Chat_pb"
local SysProtocol_pb = require "SysProtocol_pb"
local Login_pb = require "Login_pb"
local Battle_pb = require "Battle_pb"
local Friend_pb = require "Friend_pb"
local Consume_pb = require "Consume_pb";
local Const_pb = require("Const_pb");
local mission = require("Mission_pb")
local alliance = require("Alliance_pb")
local Shop_pb = require("Shop_pb");
local Item_pb =require("Item_pb");
local GameConfig = require("GameConfig");
local EquipOprHelper = require("EquipOprHelper");
local ChatManager = require("ChatManager");
local SkillManager = require("SkillManager")
local PackageLogicForLua = {}
local UserInfo = require("UserInfo");
--mailInvalidateList = {}

local noticeCache = {};

g_firstGotMapStatic = false
g_mapStaticMsg = {}

--鍏呭€煎垪琛紝鍙瓨鍌ㄤ簡閮ㄥ垎鏁版嵁
g_RechargeItemList = {};

local chatCloseFlag = -1

--宸ヤ細鑱婂ぉlist
memberChatList = {}
--涓栫晫鑱婂ぉlist
worldChatList = {}
--涓栫晫骞挎挱list
worldBroadCastList = {}

-- 鍏ㄥ眬鍙橀噺
GlobalData = {}

function ChatList_Reset()
	memberChatList = {}

	worldChatList = {}
	
	worldBroadCastList = {}
end

function PackageLogicForLua.Update()
	--绌簍ick锛岀敤浜庝繚鐣橮ackageLogicForLua 瀵硅薄锛屼笉琚獹C
end

-----------------Player state handler-------------------
function PackageLogicForLua.onReceivePlayerStates(eventName,handler)
	if eventName == "luaReceivePacket" then
		local msg = Player_pb.HPPlayerStateSync();
		local msgbuff = handler:getRecPacketBuffer();
		msg:ParseFromString(msgbuff)
		CCLuaLog("@onReceivePlayerStates -- ");
		if msg~=nil then
			UserInfo.stateInfo = msg;
			
			if chatCloseFlag ~= msg.chatClose then			
				PageManager.refreshPage("BattlePage","ChatCloseSwitch");
				chatCloseFlag = msg.chatClose
			end
			-- 闊充箰锛岄煶鏁堣繘鍏ユ椂鍊欑殑鎾斁
			if UserInfo.stateInfo.musicOn ~= nil and UserInfo.stateInfo.soundOn ~= nil then
				SoundManager:getInstance():setMusicOn(tonumber(UserInfo.stateInfo.musicOn)==1)
				SoundManager:getInstance():setEffectOn(tonumber(UserInfo.stateInfo.soundOn)==1)
			end
			if msg:HasField("curBattleMap") or msg:HasField("fastFightBuyTimes") then
				PageManager.refreshPage("BattlePage");
			end				
			
			if msg:HasField("passMapId") or msg:HasField("bossFightTimes") then
				PageManager.refreshPage("MapPage");
			end
			
			if msg:HasField("currentEquipBagSize") then
				UserEquipManager:checkPackage();
				PageManager.refreshPage("PackagePage","refreshBagSize");
			end
			if msg:HasField("equipSmeltRefesh") then
				UserInfo.smeltInfo.freeRefreshTimes = msg.equipSmeltRefesh;
			end
			if msg:HasField("eliteFightTimes") then
				PageManager.refreshPage("EliteMapRewardPopup");
			end
			
		else
			CCLuaLog("@onReceivePlayerStates -- error in data");
		end
	end
end
PackageLogicForLua.HPStateInfoSyncHandler = PacketScriptHandler:new(HP_pb.STATE_INFO_SYNC_S, PackageLogicForLua.onReceivePlayerStates);


-----------------Assembly finish handler---
function PackageLogicForLua.onReceiveAsemblyFinish(eventName,handler)
	if eventName == "luaReceivePacket" then
		local msg = Player_pb.HPPlayerStateSync();
		local msgbuff = handler:getRecPacketBuffer();
		msg:ParseFromString(msgbuff)
		CCLuaLog("@onReceiveAsemblyFinish -- ");
		if msg~=nil then
			--BattlePage_Reset()
			return BattlePage_ResetForAssembly()
		else
			CCLuaLog("@onReceiveAsemblyFinish -- error in data");
		end
	end
end
PackageLogicForLua.HPAssemblyFinishHandler = PacketScriptHandler:new(HP_pb.ASSEMBLE_FINISH_S, PackageLogicForLua.onReceiveAsemblyFinish);

-----------player kick out handler------------
function PackageLogicForLua.onPlayerKickOut(eventName,handler)
	if eventName == "luaReceivePacket" then
		local msg = Player_pb.HPPlayerKickout();
		local msgbuff = handler:getRecPacketBuffer();
		msg:ParseFromString(msgbuff)
		CCLuaLog("@onPlayerKickOut -- ");
		if msg~=nil then
			local reason = msg.reason;
			local title = Language:getInstance():getString("@LogOffTitle")
			local message  = ""
			if reason == Const_pb.DUPLICATE_LOGIN then
				message = Language:getInstance():getString("@DuplicateLogin")
			elseif reason == Const_pb.SERVER_SHUTDOWN then
				message = Language:getInstance():getString("@ServerShutDown")
			elseif reason == Const_pb.LOGIN_FORBIDEN then
				message = Language:getInstance():getString("@LoginForbiden")
			elseif reason == Const_pb.KICKED_OUT then
				message = Language:getInstance():getString("@HasKickedOut")
			else
				--鍏朵粬鍘熷洜鍚﹀垯鐩存帴 return
				return
			end
			local sureToLogOut = function(flag)
				if flag then
					--zhengxin 2014-08-10
					GamePrecedure:getInstance():reEnterLoading();
					--
				end
			end;
			PageManager.showConfirm(title,message,sureToLogOut)
		else
			CCLuaLog("@onPlayerKickOut -- error in data");
		end
	end
end
PackageLogicForLua.HPPlayerKickOutHandler  = PacketScriptHandler:new(HP_pb.PLAYER_KICKOUT_S, PackageLogicForLua.onPlayerKickOut);

-----------chat msg handler-----------
function PackageLogicForLua.onReceiveChatMsg(eventName,handler)
	if eventName == "luaReceivePacket" then
		local msg = Chat_pb.HPPushChat();
		local msgbuff = handler:getRecPacketBuffer();
		msg:ParseFromString(msgbuff)
		CCLuaLog("@onReceiveChatMsg -- ");
		if msg~=nil then		
			local chatSize = #msg.chatMsg
			local maxSize = GameConfig.ChatMsgMaxSize;
			for i=1,chatSize do
				local oneChatMsg = msg.chatMsg[i]
				local chatType = oneChatMsg.type
				local inShield = ChatManager.isInShield(oneChatMsg.playerId)
				if inShield == false then
					if chatType == Const_pb.CHAT_WORLD then
						--濡傛灉澶т簬maxsize 锛屽垹闄ょ涓€涓?
						if #worldChatList > maxSize then
							table.remove(worldChatList,1)
						end
						table.insert(worldChatList,oneChatMsg);
						hasNewChatComing = true
						hasNewWorldChatComing = true
					elseif chatType == Const_pb.CHAT_ALLIANCE then
						if #memberChatList > maxSize then
							table.remove(memberChatList,1)
						end
						table.insert(memberChatList,oneChatMsg);
						hasNewChatComing = true
						hasNewMemberChatComing = true
					elseif chatType == Const_pb.CHAT_BROADCAST then
						if #worldChatList > maxSize then
							table.remove(worldChatList,1)
						end
						table.insert(worldChatList,oneChatMsg);
						if #memberChatList > maxSize then
							table.remove(memberChatList,1)
						end
						table.insert(memberChatList,oneChatMsg);
						hasNewChatComing = true
						hasNewWorldChatComing = true
						--鍙湁宸ヤ細寮€鍚殑鎯呭喌涓嬫墠璁╁伐浼氳亰澶╃殑绾㈢偣鏄剧ず锛屾殏鏃朵笉鍋?
						--[[					if AllianceOpen then
						hasNewMemberChatComing = true
						end--]]
					
				elseif chatType == Const_pb.WORLD_BROADCAST then
					if #worldBroadCastList > maxSize then
						table.remove(worldBroadCastList,1)
					end
					table.insert(worldBroadCastList,oneChatMsg);
				end
			end				
		end
		PageManager.refreshPage("BattlePage")
		
	else
		CCLuaLog("@onReceiveChatMsg -- error in data");
	end
end
end
PackageLogicForLua.HPPushChatHandler = PacketScriptHandler:new(HP_pb.PUSH_CHAT_S, PackageLogicForLua.onReceiveChatMsg);


-----------------player award handler-------------------
function PackageLogicForLua.onReceivePlayerAward(msgbuff)
	local msg = Reward_pb.HPPlayerReward();
	msg:ParseFromString(msgbuff)
	CCLuaLog("@onReceivePlayerAward -- ");
	if msg~=nil then
		
		
		if msg:HasField("rewards") then
			UserInfo.syncPlayerInfo();
			local rewards = msg.rewards
			if rewards:HasField("bossFightTimes") then
				UserInfo.stateInfo.bossFightTimes = rewards.bossFightTimes
				PageManager.refreshPage("MapPage")
			end
			
			if rewards:HasField("contribution") then
				local GuildData = require("GuildData")
				if GuildData.MyAllianceInfo and GuildData.MyAllianceInfo.myInfo
					and GuildData.MyAllianceInfo.myInfo.contribution then
					GuildData.MyAllianceInfo.myInfo.contribution = rewards.contribution
				end
			end
			--[[移动到商店界面里头监听
			if msg.rewards:HasField("gold")
				or msg.rewards:HasField("coin")
				or msg.rewards:HasField("level")
				or msg.rewards:HasField("exp")
				or msg.rewards:HasField("vipLevel")
				then				
				PageManager.refreshPage("MarketPage","TopMsgUpdate");
			end
			--]]
		end
		
		local flag = msg.flag
		
		if flag == 1 then
			local wordList = {}
			local colorList = {}
			local rewards = msg.rewards.showItems
			for i =1,#rewards do
				local oneReward = rewards[i]
				if oneReward.itemCount > 0 then
					local ResManager = require "ResManagerForLua"
					local resInfo = ResManager:getResInfoByTypeAndId(oneReward.itemType, oneReward.itemId, oneReward.itemCount);
					local getReward = Language:getInstance():getString("@GetRewardMSG");
					local godlyEquip = Language:getInstance():getString("@GodlyEquip");
					--GodlyEquip
					local rewardName = resInfo.name;
					if resInfo.mainType == Const_pb.EQUIP then
						rewardName = string.format("LV%d %s", EquipManager:getLevelById(oneReward.itemId), rewardName);
					end
					local rewardStr = rewardName .."*"..oneReward.itemCount.." ";
					local itemColor = ""
					if resInfo.quality == 1 then
						itemColor = GameConfig.ColorMap.COLOR_GREEN
					elseif resInfo.quality == 2 then
						itemColor = GameConfig.ColorMap.COLOR_GREEN
					elseif resInfo.quality == 3 then
						itemColor = GameConfig.ColorMap.COLOR_BLUE
					elseif resInfo.quality == 4 then
						itemColor = GameConfig.ColorMap.COLOR_PURPLE
					elseif resInfo.quality == 5 then
						itemColor = GameConfig.ColorMap.COLOR_ORANGE
					end
					--local newEquipStr = common:fill(equipStr,rewardStr)
					--table.insert(wordList,rewardStr)
					local finalStr = getReward
					if oneReward:HasField("itemStatus") then
						if oneReward.itemStatus == 1 then
							finalStr = finalStr..godlyEquip
						end
					end
					finalStr= finalStr..rewardStr
					table.insert(wordList,finalStr)
					table.insert(colorList,itemColor)
				end
			end				
			return insertMessageFlow(wordList,colorList)			
		end
	else
		CCLuaLog("@onReceivePlayerAward -- error in data");
	end
end


------------------------------ market/coins info -------------------------------------
function PackageLogicForLua.onReceiveMarketDropsInfo(eventName,handler)
	if eventName == "luaReceivePacket" then
		local msg = Shop_pb.OPShopInfoRet();
		local msgbuff = handler:getRecPacketBuffer();
		msg:ParseFromString(msgbuff)
		CCLuaLog("@onReceiveMarketDropsInfo -- ");
		if msg~=nil then
			marketAdventureInfo.dropsItems = msg.shopItems;
			marketAdventureInfo.refreshCount = msg.refreshCount;
		end
	end
	--handler:removePacket(HP_pb.SHOP_S);
end

function PackageLogicForLua.onReceiveMarketCoinsInfo(eventName,handler)
	if eventName == "luaReceivePacket" then
		local msg = Shop_pb.OPBuyCoinRet();
		local msgbuff = handler:getRecPacketBuffer();
		msg:ParseFromString(msgbuff)
		CCLuaLog("@onReceiveMarketCoinsInfo -- ");
		if msg~=nil then
			marketAdventureInfo.coinReward = msg.coin;
			marketAdventureInfo.coinCost = msg.coinPrice;
			marketAdventureInfo.coinCount = msg.canBuyNums;
		end
		
	end
	
	--handler:removePacket(HP_pb.SHOP_COIN_S);
end

PackageLogicForLua.HPMarketDropsHandler = PacketScriptHandler:new(HP_pb.SHOP_S, PackageLogicForLua.onReceiveMarketDropsInfo);
PackageLogicForLua.HPMarketCoinHandler = PacketScriptHandler:new(HP_pb.SHOP_COIN_S, PackageLogicForLua.onReceiveMarketCoinsInfo);
------------------------------ market/coins info -------------------------------------



function PackageLogicForLua.onReceiveNoticePush(eventName,handler)
	if eventName == "luaReceivePacket" then
		local msg = Notice_pb.HPNotice();
		local msgbuff = handler:getRecPacketBuffer();
		msg:ParseFromString(msgbuff)
		CCLuaLog("@onReceiveNoticePush -- ");
		local hasSendMailMsg = false;
		if msg~=nil then
			local itemsSize = table.maxn(msg.notices);
			
			for i=1,itemsSize,1 do
				local item = msg.notices[i];
				
				noticeCache[i] = {};
				noticeCache[i].noticeType = item.noticeType;
				noticeCache[i].count = item.count;
				
				local size = table.maxn(item.params);
				
				for j=1,size do
					noticeCache[i].params = {};
					noticeCache[i].params[j] = item.params[j];
				end
				
				local message = MsgMainFrameGetNewInfo:new()
				message.type = item.noticeType;
				MessageManager:getInstance():sendMessageForScript(message)
				
				--[[
				if message.type == Const_pb.NEW_MAIL  and (not hasSendMailMsg) then
					local mailMsg = Mail_pb.OPMailInfo();
					
					local index = table.maxn(MailInfo.mails);
					if index > 0 then
						local mail = MailInfo.mails[index];
						if mail ~= nil then
							mailMsg.version = mail.id;
						else
							mailMsg.version = 0;
						end
					else
						mailMsg.version = 0;
					end
					
					local pb_data = mailMsg:SerializeToString();
					PacketManager:getInstance():sendPakcet(HP_pb.MAIL_INFO_C, pb_data, #pb_data, true);
					hasSendMailMsg = true;
				end
				--]]
			end
			
			
		end
		
	end
	
	
end
PackageLogicForLua.HPNoticePushHandler = PacketScriptHandler:new(HP_pb.NOTICE_PUSH, PackageLogicForLua.onReceiveNoticePush);
------------------------------new red point-------------------------------------

--[[
------------------------------new mail point-------------------------------------
function PackageLogicForLua.onReceiveMailInfo(eventName,handler)
	if eventName == "luaReceivePacket" then
		local msg = Mail_pb.OPMailInfoRet()
		local msgbuff = handler:getRecPacketBuffer();
		msg:ParseFromString(msgbuff)
		CCLuaLog("@onReceiveMailInfo -- ");
		if msg~=nil then
			local maxSize = table.maxn(msg.mails);
			if maxSize > 0 then
			    MailInfo.lastMail = msg.mails[maxSize]
			end
			for i=1,maxSize,1 do
				local mail = msg.mails[i];
				local mailId = mail.id;
				if mailInvalidateList[mailId] ~=nil then
					CCLuaLog("already in the list");
				else					
					mailInvalidateList[mailId] = true
					if mail.type == Mail_pb.ARENA_ALL then
						table.insert(MailInfo.mailAreanAll , mail)
					else
						table.insert(MailInfo.mails , mail)
					end
					
				end
			end
			table.sort(MailInfo.mails,
			function ( e1, e2 )			
				if not e2 then return true end
				if not e1 then return false end
				if e1.type == Mail_pb.Reward and e2.type ~= Mail_pb.Reward then return true end
				if e1.type ~= Mail_pb.Reward and e2.type == Mail_pb.Reward then return false end
				if e1.type == Mail_pb.Reward and e2.type == Mail_pb.Reward then 
					--mailId = 7 涓烘湀鍗?
					if e1.mailId == 7 and e2.mailId ~= 7 then return true end 
					if e1.mailId ~= 7 and e2.mailId == 7 then return false end
					return e1.id + 0 > e2.id + 0
				end
				return e1.id + 0 > e2.id + 0
			end
			);
			
			
			-- new mail notice
			if maxSize > 0 then
				local msg = MsgMainFrameGetNewInfo:new()
				msg.type = Const_pb.NEW_MAIL;
				MessageManager:getInstance():sendMessageForScript(msg)
				
				local msg = MsgMainFrameRefreshPage:new()
				msg.pageName = "MailPage";
				MessageManager:getInstance():sendMessageForScript(msg)
				
				local msg1 = MsgMainFrameRefreshPage:new()
				msg1.pageName = "ArenaRecordPage";
				MessageManager:getInstance():sendMessageForScript(msg1)
			end
		end						
	end
	
end
PackageLogicForLua.HPMailInfoHandler = PacketScriptHandler:new(HP_pb.MAIL_INFO_S, PackageLogicForLua.onReceiveMailInfo);
--]]

------------------------------new mail point-------------------------------------

function PackageLogicForLua.onReceiveNewLeaveMessage(eventName,handler)
	if eventName == "luaReceivePacket" then
		local msg = Arena_pb.HPArenaDefenderListSyncS()
		if BlackBoard:getInstance():hasVarible( GameConfig.Default.NewMsgKey ) then
			BlackBoard:getInstance():setVarible( GameConfig.Default.NewMsgKey , true)
		else
			BlackBoard:getInstance():addVarible( GameConfig.Default.NewMsgKey , true )
		end
	end
end
PackageLogicForLua.HPNewMsgHandler = PacketScriptHandler:new(HP_pb.NEW_MSG_SYNC_S, PackageLogicForLua.onReceiveNewLeaveMessage)


--澶勭悊error code 鐨勯瀛楁儏鍐碉紝鐜板湪error code 鍏跺疄鍏呭綋浜哠tatus code鐨勮鑹?by zhenhui 2014/8/23
function PackageLogicForLua.onReceiveErrorCode(eventName,handler)
	if eventName == "luaReceivePacket" then
		local msgBuff = handler:getRecPacketBuffer();
		local msg = SysProtocol_pb.HPErrorCode()
		msg:ParseFromString(msgBuff)
		if msg:HasField("errFlag") then
			local flag = msg.errFlag
			--if flag == 2 椋樺瓧
			if flag == 2 then
				local errorCodeCfg = ConfigManager.getErrorCodeCfg()
				local content = errorCodeCfg[msg.errCode].content
				local wordList = {}
				local colorList = {}
				table.insert(wordList,content)
				itemColor = GameConfig.ColorMap.COLOR_GREEN
				table.insert(colorList,itemColor)
				return insertMessageFlow(wordList,colorList)
			end
		end
	end
end
PackageLogicForLua.HPErrorCodeHandler = PacketScriptHandler:new(HP_pb.ERROR_CODE, PackageLogicForLua.onReceiveErrorCode)

-- added by sunyj for Gift Package Popup on game startup
function PackageLogicForLua.onReceiveGiftPackage(eventName,handler)
	if eventName == "luaReceivePacket" then
		local msgBuff = handler:getRecPacketBuffer();
		local msg = mission.HPMissionListS()
		msg:ParseFromString(msgBuff)
		GiftList = msg.infos
		GiftListReceived = true
		table.sort(GiftList, sortGiftList)
		
		-- notice GiftPage to refreshPage
		local message = MsgMainFrameRefreshPage:new()
		message.pageName = 'GiftPage'
		MessageManager:getInstance():sendMessageForScript(message)
		
		-- 娌℃湁鍙鍙栫殑濂栧姳浜嗗氨娓呯┖绾㈢偣
		if not hasUngetReward() then
			local newInfoMsg = MsgMainFrameGetNewInfo:new()
			newInfoMsg.type = GameConfig.NewPointType.TYPE_GIFT_NEW_CLOSE
			MessageManager:getInstance():sendMessageForScript(newInfoMsg)
		end
	end
end
PackageLogicForLua.HPMissionListHandler = PacketScriptHandler:new(HP_pb.MISSION_LIST_S, PackageLogicForLua.onReceiveGiftPackage)
-- end sunyj

-- added by sunyj for alliance info
function PackageLogicForLua.onReceiveAlliancePersonalInfo(eventName,handler)
	if eventName == "luaReceivePacket" then
		local msgBuff = handler:getRecPacketBuffer();
		local msg = alliance.HPAllianceEnterS()
		msg:ParseFromString(msgBuff)
		MyAllianceInfo = msg
	end
end
PackageLogicForLua.HPAllianceEnterHandler = PacketScriptHandler:new(HP_pb.ALLIANCE_ENTER_S, PackageLogicForLua.onReceiveAlliancePersonalInfo)

function PackageLogicForLua.onReceiveAllianceInfo(eventName,handler)
	local GuildData = require('GuildData')
	if eventName == "luaReceivePacket" then
		local msgBuff = handler:getRecPacketBuffer();
		local msg = alliance.HPAllianceInfoS()
		msg:ParseFromString(msgBuff)
		GuildData.allianceInfo.commonInfo = msg
	end
end
PackageLogicForLua.HPAllianceCreateHandler = PacketScriptHandler:new(HP_pb.ALLIANCE_CREATE_S, PackageLogicForLua.onReceiveAllianceInfo)
-- end sunyj


function sortGiftList(item1, item2)
	-- 椹磋劇鑴劖鑴曠煕鑴犻殕纰岃剾璺劀鑴劮鑴冲嵂鑴¤劎鑴欏繖
	if item1.tag and (not item2.tag) then
		return true
	elseif (not item1.tag) and item2.tag then
		return false
	end
	
	-- 椹磋劇鑴曠煕鑴犻殕鑴抽簱鑴ゅ崲鑴ц効鑴ュ崲纰岃剾鎺抽簱鑴岃効鑴ㄨ劌鑴滆劀鑴ㄨ矊
	local giftCfg = ConfigManager.getGiftConfig()
	local id1 = item1.id
	local id2 = item2.id
	local type1 = giftCfg[id1].type
	local type2 = giftCfg[id2].type
	return type1 < type2
end

function hasUngetReward()
	for i=1, #GiftList do
		if GiftList[i] and GiftList[i].tag then
			return true
		end
	end
	return false
end

function PackageLogicForLua.onEquipSyncFinish(eventName, handler)
	if eventName == "luaReceivePacket" then
		UserEquipManager:check();
	end
end
PackageLogicForLua.HPEquipSyncFinish = PacketScriptHandler:new(HP_pb.EQUIP_SYNC_FINISH_S, PackageLogicForLua.onEquipSyncFinish);

---------------------褰曡効鑴ゅ獟鑴虫幊鍗よ祩鑴ュ崲铏忛檰---------------
function PackageLogicForLua.onRecieveSyncEquip(eventName, handler)
	if eventName == "luaReceivePacket" then
		return EquipOprHelper:syncEquipInfoFromMsg(handler:getRecPacketBuffer());
	end
end
PackageLogicForLua.HPSyncEquipInfoHandler = PacketScriptHandler:new(HP_pb.EQUIP_INFO_SYNC_S, PackageLogicForLua.onRecieveSyncEquip);

---------------------褰曡効鑴ゅ獟鑴虫幊鍗よ祩鑴ュ崲铏忛檰---------------
function PackageLogicForLua.onRecieveSyncItem(eventName, handler)
	if eventName == "luaReceivePacket" then
		local msg = Item_pb.HPItemInfoSync();
		local msgBuff = handler:getRecPacketBuffer();
		msg:ParseFromString(msgBuff);
		for _, itemInfo in ipairs(msg.itemInfos) do
			ActivityInfo:checkAfterItemSync(itemInfo.itemId, itemInfo.count);
		end
	end
end
PackageLogicForLua.HPSyncItemInfoHandler = PacketScriptHandler:new(HP_pb.ITEM_INFO_SYNC_S, PackageLogicForLua.onRecieveSyncItem);
---------------------褰曡効鑴ゅ獟鑴ф病娼炶剾鎷㈤鎷㈤---------------
function PackageLogicForLua.onRecievePlayerConsume(eventName, handler)
	if eventName == "luaReceivePacket" then
		local msg = Consume_pb.HPConsumeInfo();
		local msgBuff = handler:getRecPacketBuffer();
		msg:ParseFromString(msgBuff);
		
		if msg:HasField("attrInfo") then
			if msg.attrInfo:HasField("gold")
				or msg.attrInfo:HasField("coin")
				or msg.attrInfo:HasField("level")
				or msg.attrInfo:HasField("exp")
				or msg.attrInfo:HasField("vipLevel")
				then
				PageManager.refreshPage("MainScenePage");
				--PageManager.refreshPage("MarketPage","TopMsgUpdate");
			end
			if msg.attrInfo:HasField("honorValue") then
				UserInfo.playerInfo.honorValue = msg.attrInfo.honorValue
			end
			if msg.attrInfo:HasField("reputationValue") then
				UserInfo.playerInfo.reputationValue = msg.attrInfo.reputationValue
			end
		end
		
		local hasConsumeEquip = false;
		local hasConsumeItem = false;
		for _, consumeItem in ipairs(msg.consumeItem) do
			if consumeItem.type == Const_pb.CHANGE_EQUIP then
				EquipOprHelper:deleteEquip(consumeItem.id);
				hasConsumeEquip = true;
			elseif consumeItem.type == Const_pb.CHANGE_TOOLS then
				hasConsumeItem = true;
				ActivityInfo:checkAfterItemSync(consumeItem.itemId);
			end
		end
		if hasConsumeEquip or hasConsumeItem then
			PageManager.refreshPage("PackagePage");
		end
	end
end
PackageLogicForLua.HPPlayerConsumeHandler = PacketScriptHandler:new(HP_pb.PLAYER_CONSUME_S, PackageLogicForLua.onRecievePlayerConsume);


function PackageLogicForLua.onRecieveSeeOtherPlayerInfo(eventName, handler)
	if eventName == "luaReceivePacket" then
		local msgBuff = handler:getRecPacketBuffer();
		ViewPlayerInfo:setInfo(msgBuff);
		PageManager.pushPage("ViewPlayerInfoPage");
	end
end
PackageLogicForLua.HPSeeOtherPlayerInfo = PacketScriptHandler:new(HP_pb.SEE_OTHER_PLAYER_INFO_S, PackageLogicForLua.onRecieveSeeOtherPlayerInfo);

function PackageLogicForLua.onRecieveEquipDress(eventName, handler)
	if eventName == "luaReceivePacket" then
		local msg = EquipOpr_pb.HPEquipDressRet();
		local pbMsg = handler:getRecPacketBuffer();
		msg:ParseFromString(pbMsg);
		
		if msg:HasField("onEquipId") then
			UserEquipManager:takeOn(msg.onEquipId);
		end
		if msg:HasField("offEquipId") then
			UserEquipManager:takeOff(msg.offEquipId);
		end
	end
end
PackageLogicForLua.HPListenEquipDress = PacketScriptHandler:new(HP_pb.EQUIP_DRESS_S, PackageLogicForLua.onRecieveEquipDress);


function PackageLogicForLua.onRecieveAttrChange(eventName, handler)
	if eventName == "luaReceivePacket" then
		local msg = Attribute_pb.AttrInfo();
		local pbMsg = handler:getRecPacketBuffer();
		msg:ParseFromString(pbMsg);
		
		local wordList = {};
		local colorList = {};
		for _, attr in ipairs(msg.attribute) do
			if attr.attrValue ~= 0 then
				local valStr = "";
				if EquipManager:getAttrGrade(attr.attrId) == Const_pb.GODLY_ATTR 
					and not EquipManager:isGodlyAttrPureNum(attr.attrId)
				then
					valStr = string.format(" %+.1f%%", attr.attrValue / 100);
				else
					valStr = string.format(" %+d", attr.attrValue);
				end
				table.insert(wordList, common:getLanguageString("@AttrName_" .. attr.attrId) .. valStr);
				local colorKey = attr.attrValue > 0 and "COLOR_GREEN" or "COLOR_RED";
				table.insert(colorList, GameConfig.ColorMap[colorKey]);
			end
		end
		return insertMessageFlow(wordList, colorList);
	end
end
PackageLogicForLua.HPListenAttrChange = PacketScriptHandler:new(HP_pb.ATTRIBUTE_CHANGE_NOTICE, PackageLogicForLua.onRecieveAttrChange);

--[[
function PackageLogicForLua.onRecieveLogin(eventName, handler)
	if eventName == "luaReceivePacket" then
		UserEquipManager:setUninited();
		local msg = Login_pb.HPLoginRet();
		local pbMsg = handler:getRecPacketBuffer();
		msg:ParseFromString(pbMsg);
		if msg == nil or msg.playerId == nil then
			return
		end
	end
end
PackageLogicForLua.HPListenLogin = PacketScriptHandler:new(HP_pb.LOGIN_S, PackageLogicForLua.onRecieveLogin);
--]]

--鍦板浘缁熻淇℃伅鐩稿叧
function PackageLogicForLua.onReceiveMapStaticSync(eventName,handler)
	if eventName == "luaReceivePacket" then
		local msg = Battle_pb.HPMapStatisticsSync();
		local msgbuff = handler:getRecPacketBuffer();
		msg:ParseFromString(msgbuff)
		--鐢变簬BattlePage鏃舵晥鎬э紝绗竴娆＄敱鍏ㄥ眬鑾峰彇锛屼箣鍚庣敱BattlePage鍐冲畾
		if msg~=nil and g_firstGotMapStatic ==false then
			g_firstGotMapStatic = true
			g_mapStaticMsg = msg
		else
			--CCLuaLog("@onReceiveMapStaticSync -- error in data");
		end
	end
end
PackageLogicForLua.HPMapStaticsSync = PacketScriptHandler:new(HP_pb.MAP_STATISTICS_SYNC_S, PackageLogicForLua.onReceiveMapStaticSync);

-----------------message box handler-------------------
function PackageLogicForLua.onReceiveOfflineMessageBox(eventName,handler)
	if eventName == "luaReceivePacket" then
		local msg = Friend_pb.HPMsgBoxInfo();
		local msgbuff = handler:getRecPacketBuffer();
		msg:ParseFromString(msgbuff)
		if msg~=nil then
			hasNewChatComing = true -- 绂荤嚎鏂版秷鎭笘鐣岃亰澶╂樉绀虹孩鐐?
			for i=1,#msg.msgBoxUnits do
				local oneData = msg.msgBoxUnits[i]
				--offline msg push
				if oneData~=nil then
					ChatManager.insertPrivateMsg(oneData.playerId,oneData,nil,true)
				end
			end
		else
			CCLuaLog("@onReceiveOfflineMessageBox -- error in data");
		end
	end
end
PackageLogicForLua.HPOfflineMessageBox = PacketScriptHandler:new(HP_pb.MESSAGE_BOX_INFO_S, PackageLogicForLua.onReceiveOfflineMessageBox);

-----------------message box handler-------------------
function PackageLogicForLua.onReceivePrivateChatMsg(eventName,handler)
	if eventName == "luaReceivePacket" then
		local msg = Friend_pb.HPMsgPush();
		local msgbuff = handler:getRecPacketBuffer();
		msg:ParseFromString(msgbuff)
		if msg~=nil then
			--online msg push
			ChatManager.insertPrivateMsg(msg.msgBoxUnit.playerId,msg.msgBoxUnit,msg.friendMsg,false)
			hasNewChatComing = true
			if MainFrame:getInstance():getCurShowPageName() =="" then
				PageManager.refreshPage("BattlePage")
			end				
		else
			CCLuaLog("@onReceivePrivateChatMsg -- error in data");
		end
	end
end
PackageLogicForLua.HPPrivateChatMsg = PacketScriptHandler:new(HP_pb.MESSAGE_PUSH_S, PackageLogicForLua.onReceivePrivateChatMsg);

-----------------shield player handler-------------------
function PackageLogicForLua.onReceiveShieldPlayerList(eventName,handler)
	if eventName == "luaReceivePacket" then
		local msg = Friend_pb.HPShieldList();
		local msgbuff = handler:getRecPacketBuffer();
		msg:ParseFromString(msgbuff)
		if msg~=nil then		
			return ChatManager.recieveShieldList(msg.shieldPlayerId)
		else
			CCLuaLog("@onReceiveShieldPlayerList -- error in data");
		end
	end
end
PackageLogicForLua.HPShieldPlayerList = PacketScriptHandler:new(HP_pb.FRIEND_SHIELD_LIST_S, PackageLogicForLua.onReceiveShieldPlayerList);

function PackageLogicForLua.onReceiveSkillBag( eventName ,handler )
	if eventName == "luaReceivePacket" then
		local msg = Skill_pb.HPSkillInfo()
		local msgBuff = handler:getRecPacketBuffer()
		msg:ParseFromString(msgBuff)
		if msg ~= nil then
			UserInfo.sync()
			if msg.roleId == UserInfo.roleInfo.roleId then
				SkillManager:setSkillListInfo(msg)				
				local msg = MsgMainFrameRefreshPage:new()
				msg.pageName = "SkillPage"
				MessageManager:getInstance():sendMessageForScript(msg)
				PageManager.refreshPage("CampMainPage")
			else
				SkillManager:onRecieveMerSkillList(msg)	
				local msg = MsgMainFrameRefreshPage:new()
				msg.pageName = "MercenarySkillPage"
				MessageManager:getInstance():sendMessageForScript(msg) 
			end		
		end
	end
end
PackageLogicForLua.HPSysSkillList = PacketScriptHandler:new(HP_pb.SKILL_BAG_S , PackageLogicForLua.onReceiveSkillBag)

function PackageLogicForLua.onReceiveNotify(eventName,handler)
	if eventName == "luaReceivePacket" then
		local msg = Player_pb.HPDataNotify();
		local msgbuff = handler:getRecPacketBuffer();
		msg:ParseFromString(msgbuff)
		if msg.type == Const_pb.NOTIFY_RECHARGE then		
			PageManager.pushPage("RechargeSucceedPopUpPage")
			PageManager.refreshPage("MainScenePage")
			
			local msg1 = MsgMainFrameRefreshPage:new()
			msg1.pageName = "RechargePage"
			MessageManager:getInstance():sendMessageForScript(msg1)
			
			local msg2 = MsgMainFrameRefreshPage:new()
			msg2.pageName = "VipWelfarePage"
			MessageManager:getInstance():sendMessageForScript(msg2)
		end
	end
end
PackageLogicForLua.HPRechargeNotify = PacketScriptHandler:new(HP_pb.DATA_NOTIFY_S, PackageLogicForLua.onReceiveNotify);



function PackageLogicForLua.onReceiveCampWarState(eventName,handler)
	if eventName == "luaReceivePacket" then
		local CampWar_pb = require("CampWar_pb");
		local msg = CampWar_pb.HPCampWarStateSyncS();
		local msgbuff = handler:getRecPacketBuffer();
		msg:ParseFromString(msgbuff)
		local CampWarManager = require("CampWarManager")
		return CampWarManager.RecieveCampWarMainState(msg)
	end
end
PackageLogicForLua.HPCampWarStateSync = PacketScriptHandler:new(HP_pb.CAMPWAR_STATE_SYNC_S, PackageLogicForLua.onReceiveCampWarState);

function PackageLogicForLua.onReceiveCampWarInFightState(eventName,handler)
	if eventName == "luaReceivePacket" then
		local CampWar_pb = require("CampWar_pb");
		local msg = CampWar_pb.HPCampWarInfoSyncS();
		local msgbuff = handler:getRecPacketBuffer();
		msg:ParseFromString(msgbuff)
		local CampWarManager = require("CampWarManager")
		return CampWarManager.RecieveCampWarInFightState(msg)
	end
end
PackageLogicForLua.HPCampWarInFightStateSync = PacketScriptHandler:new(HP_pb.CAMPWAR_INFO_SYNC_S, PackageLogicForLua.onReceiveCampWarInFightState);

function PackageLogicForLua.onReceiveCampRankInfo(eventName,handler)
	if eventName == "luaReceivePacket" then
		local CampWar_pb = require("CampWar_pb");
		local msg = CampWar_pb.HPLastCampWarRankInfoSyncS();
		local msgbuff = handler:getRecPacketBuffer();
		msg:ParseFromString(msgbuff)
		local CampWarManager = require("CampWarManager")
		return CampWarManager.RecieveCampLastRankInfo(msg)
	end
end
PackageLogicForLua.HPCampWarLastRankSync = PacketScriptHandler:new(HP_pb.LAST_CAMPWAR_RANK_SYNC_S, PackageLogicForLua.onReceiveCampRankInfo);

function PackageLogicForLua.onReceivePlayerTitleInfo(eventName,handler)
	if eventName == "luaReceivePacket" then
		local PlayerTitle_pb = require("PlayerTitle_pb");
		local msg = PlayerTitle_pb.HPTitleInfoSyncS();
		local msgbuff = handler:getRecPacketBuffer();
		msg:ParseFromString(msgbuff)
		local TitleManager = require("TitleManager")
		TitleManager:setTitleInfo(msg)
	end
end
PackageLogicForLua.HPTitleInfoSyncS = PacketScriptHandler:new(HP_pb.TITLE_SYNC_S, PackageLogicForLua.onReceivePlayerTitleInfo);

function PackageLogicForLua.onReceiveRechargeList(eventName, handler)
	if eventName == "luaReceivePacket" then
		local msgbuff = handler:getRecPacketBuffer();
		local Recharge_pb = require("Recharge_pb");
		local msg = Recharge_pb.HPShopListSync();
		msg:ParseFromString(msgbuff);
		g_RechargeItemList = {};
		for _, item in ipairs(msg.shopItems) do
			--local goodsId = tonumber(item.productId:sub(0, -11));
			local goodsId = tonumber(item.productId);
			g_RechargeItemList[goodsId] = {
				productId		= item.productId,
				productType		= item.productType,
				productName		= item.productName,
				productPrice 	= item.productPrice,
				gold			= item.gold
			};
		end
	end
end
PackageLogicForLua.HPShopListSync = PacketScriptHandler:new(HP_pb.FETCH_SHOP_LIST_S, PackageLogicForLua.onReceiveRechargeList);

function PackageLogicForLua.onReceiveLoginDay(eventName, handler)
	if eventName == "luaReceivePacket" then
		local msgbuff = handler:getRecPacketBuffer();
		local Activity_pb = require("Activity_pb");
		local msg = Activity_pb.HPRegisterCycleRet();
		msg:ParseFromString(msgbuff);
		local mainScenePage = require("MainScenePage")
		mainScenePage:setLoginDay(msg.registerSpaceDays) 

		if GlobalData.diamondRatio == nil then
			if msg:HasField("ratio") then
				GlobalData.diamondRatio = 0.5
			else
				GlobalData.diamondRatio = 0
			end
		end
		PageManager.refreshPage("MainScenePage")
	end
end
PackageLogicForLua.HPLoginDaySync = PacketScriptHandler:new(HP_pb.REGISTER_CYCLE_INFO_S, PackageLogicForLua.onReceiveLoginDay);

-- 鍚屾鍏細鍏冩皵
function PackageLogicForLua.onReceiveGuildBossVitality(eventName, handler)
	local GuildData = require('GuildData')
	if eventName == "luaReceivePacket" then
		local msgbuff = handler:getRecPacketBuffer();
		local Alliance_pb = require("Alliance_pb");
		local msg = Alliance_pb.HPBossVitalitySyncS();
		msg:ParseFromString(msgbuff);
		GuildData.allianceInfo.commonInfo.curBossVitality = msg.curBossVitality
		GuildData.allianceInfo.commonInfo.openBossVitality = msg.openBossVitality
		PageManager.refreshPage("GuildPage_Refresh_BossPage")
	end
end
PackageLogicForLua.HPGuildBossVitalitySync = PacketScriptHandler:new(HP_pb.BOSS_VITALITY_SYNC_S, PackageLogicForLua.onReceiveGuildBossVitality);


-- 
function PackageLogicForLua.onReceiveRoleRingInfoSync(eventName, handler)
	if eventName == "luaReceivePacket" then
		local msgbuff = handler:getRecPacketBuffer();				
		local msg = Player_pb.HPRoleRingInfoSync();
		msg:ParseFromString(msgbuff);
		local MercenaryHaloManager = require("MercenaryHaloManager")
		MercenaryHaloManager:onRecieveSyncMsg(msg);
		PageManager.refreshPage("MercenaryHaloPage")
	end
end
PackageLogicForLua.HPRoleRingSync= PacketScriptHandler:new(HP_pb.ROLE_RING_INFO_S, PackageLogicForLua.onReceiveRoleRingInfoSync);

function PackageLogicForLua.onReceiveSkillSync(eventName,handler)
	if eventName == "luaReceivePacket" then	
		local SkillManager = require("SkillManager")
		SkillManager:classifyOpenSkillDataFromC()
	end
end
PackageLogicForLua.HPSkillSyncHandler = PacketScriptHandler:new(HP_pb.SKILL_INFO_SYNC_S, PackageLogicForLua.onReceiveSkillSync);


function PackageLogicForLua.onReceiveEliteMapInfoSync(eventName,handler)
	if eventName == "luaReceivePacket" then	
		local msgbuff = handler:getRecPacketBuffer();				
		local msg = Player_pb.HPEliteMapInfoSync();
		msg:ParseFromString(msgbuff);
		local EliteMapManager = require("EliteMapManager")
		EliteMapManager:onRecieveInfoSync(msg)
		PageManager.refreshPage("EliteMapInfoPage")
	end
end
PackageLogicForLua.HPEliteMapSyncHandler = PacketScriptHandler:new(HP_pb.ELITE_MAP_INFO_SYNC_S, PackageLogicForLua.onReceiveEliteMapInfoSync);


function PackageLogicForLua.onReceiveClientSetting(eventName,handler)
	if eventName == "luaReceivePacket" then	
		local msgbuff = handler:getRecPacketBuffer();				
		local msg = Player_pb.HPClientSetting();
		msg:ParseFromString(msgbuff);
		local ClientSettingManager = require("ClientSettingManager")
		ClientSettingManager:onReceivePacket(msg)
	end
end
PackageLogicForLua.HPClientSettingPush = PacketScriptHandler:new(HP_pb.CLIENT_SETTING_PUSH, PackageLogicForLua.onReceiveClientSetting);




--娉ㄦ剰--
--姣忔鍔犱竴涓叏灞€鐨勯暱鐩戝惉鍑芥暟锛岄渶瑕佸湪杩欓噷闈㈠姞鍏ヤ竴涓猺egisterFunctionHandler function
--鐢ㄤ簬function澶辨晥鍚庯紝鏍￠獙骞堕噸娉ㄥ唽浣跨敤銆? by zhenhui 2014/9/3
function validateAndRegisterAllHandler()
	
	PackageLogicForLua.HPStateInfoSyncHandler:registerFunctionHandler(PackageLogicForLua.onReceivePlayerStates)
	PackageLogicForLua.HPAssemblyFinishHandler:registerFunctionHandler(PackageLogicForLua.onReceiveAsemblyFinish)
	PackageLogicForLua.HPPlayerKickOutHandler:registerFunctionHandler(PackageLogicForLua.onPlayerKickOut)
	PackageLogicForLua.HPPushChatHandler:registerFunctionHandler(PackageLogicForLua.onReceiveChatMsg);
	PackageLogicForLua.HPMarketDropsHandler:registerFunctionHandler( PackageLogicForLua.onReceiveMarketDropsInfo);
	PackageLogicForLua.HPMarketCoinHandler:registerFunctionHandler(PackageLogicForLua.onReceiveMarketCoinsInfo);
	PackageLogicForLua.HPNoticePushHandler:registerFunctionHandler(PackageLogicForLua.onReceiveNoticePush);
--	PackageLogicForLua.HPMailInfoHandler:registerFunctionHandler(PackageLogicForLua.onReceiveMailInfo);
	PackageLogicForLua.HPNewMsgHandler:registerFunctionHandler(PackageLogicForLua.onReceiveNewLeaveMessage)
	PackageLogicForLua.HPErrorCodeHandler:registerFunctionHandler(PackageLogicForLua.onReceiveErrorCode)
	PackageLogicForLua.HPMissionListHandler:registerFunctionHandler(PackageLogicForLua.onReceiveGiftPackage)
	PackageLogicForLua.HPAllianceEnterHandler:registerFunctionHandler(PackageLogicForLua.onReceiveAlliancePersonalInfo)
	PackageLogicForLua.HPAllianceCreateHandler:registerFunctionHandler(PackageLogicForLua.onReceiveAllianceInfo)
	PackageLogicForLua.HPEquipSyncFinish:registerFunctionHandler(PackageLogicForLua.onEquipSyncFinish);
	PackageLogicForLua.HPSyncEquipInfoHandler:registerFunctionHandler(PackageLogicForLua.onRecieveSyncEquip)
	PackageLogicForLua.HPSyncItemInfoHandler:registerFunctionHandler(PackageLogicForLua.onRecieveSyncItem)
	PackageLogicForLua.HPPlayerConsumeHandler:registerFunctionHandler(PackageLogicForLua.onRecievePlayerConsume)
	PackageLogicForLua.HPSeeOtherPlayerInfo:registerFunctionHandler(PackageLogicForLua.onRecieveSeeOtherPlayerInfo)
	PackageLogicForLua.HPListenEquipDress:registerFunctionHandler( PackageLogicForLua.onRecieveEquipDress)
	PackageLogicForLua.HPListenAttrChange:registerFunctionHandler(PackageLogicForLua.onRecieveAttrChange)
	--PackageLogicForLua.HPListenLogin:registerFunctionHandler(PackageLogicForLua.onRecieveLogin)
	PackageLogicForLua.HPMapStaticsSync:registerFunctionHandler(PackageLogicForLua.onReceiveMapStaticSync);
	PackageLogicForLua.HPOfflineMessageBox:registerFunctionHandler(PackageLogicForLua.onReceiveOfflineMessageBox);
	PackageLogicForLua.HPPrivateChatMsg:registerFunctionHandler(PackageLogicForLua.onReceivePrivateChatMsg);
	PackageLogicForLua.HPShieldPlayerList:registerFunctionHandler(PackageLogicForLua.onReceiveShieldPlayerList);
	PackageLogicForLua.HPRechargeNotify:registerFunctionHandler(PackageLogicForLua.onReceiveNotify);
	PackageLogicForLua.HPCampWarStateSync:registerFunctionHandler(PackageLogicForLua.onReceiveCampWarState);
	PackageLogicForLua.HPCampWarInFightStateSync:registerFunctionHandler(PackageLogicForLua.onReceiveCampWarInFightState);
	PackageLogicForLua.HPCampWarLastRankSync:registerFunctionHandler(PackageLogicForLua.onReceiveCampRankInfo);
	PackageLogicForLua.HPTitleInfoSyncS:registerFunctionHandler(PackageLogicForLua.onReceivePlayerTitleInfo);
	PackageLogicForLua.HPShopListSync:registerFunctionHandler(PackageLogicForLua.onReceiveRechargeList);
	PackageLogicForLua.HPLoginDaySync:registerFunctionHandler(PackageLogicForLua.onReceiveLoginDay);
	PackageLogicForLua.HPGuildBossVitalitySync:registerFunctionHandler(PackageLogicForLua.onReceiveGuildBossVitality);
	PackageLogicForLua.HPRoleRingSync:registerFunctionHandler(PackageLogicForLua.onReceiveRoleRingInfoSync);
	PackageLogicForLua.HPSkillSyncHandler:registerFunctionHandler(PackageLogicForLua.onReceiveSkillSync);
	PackageLogicForLua.HPEliteMapSyncHandler:registerFunctionHandler(PackageLogicForLua.onReceiveEliteMapInfoSync);
	PackageLogicForLua.HPClientSettingPush:registerFunctionHandler(PackageLogicForLua.onReceiveClientSetting)
	local ArenaData = require("ArenaData")
	ArenaData.validateAndRegister()
	require("ActivityInfo")
	ActivityInfo:validateAndRegister()
	local BattleDataHelper = require("BattleDataHelper")
	BattleDataHelper:validateAndRegister()
end



return PackageLogicForLua;

