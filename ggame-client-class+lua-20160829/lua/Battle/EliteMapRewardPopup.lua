local Battle_pb =  require "Battle_pb"
local Const_pb = require "Const_pb"
local UserInfo = require("UserInfo");

local thisPageName = "EliteMapRewardPopup"
local NodeHelper = require("NodeHelper");
local EliteMapManager = require("EliteMapManager")
local EliteMapCfg = ConfigManager.getEliteMapCfg();

local option = {
	ccbiFile = "EliteMapRewardPopUp.ccbi",
	handlerMap = {
		onClose = "onClose",
		onHelp = "onHelp",
		onAdd = "onAdd",
		onImmediatelyDekaron= "onImmediatelyDekaron",
		onFastSweep= "onFastSweep"
	}
}

local curMapId = 0

local EliteMapRewardPopup = {}

----------------------------------------------------------------------------------

-----------------------------------------------
--EliteMapRewardPopup页面中的事件处理
----------------------------------------------
function EliteMapRewardPopup:onEnter(container)
	container:registerPacket(HP_pb.BOSS_WIPE_S)
	container:registerPacket(HP_pb.STATE_INFO_SYNC_S)
	container:registerMessage(MSG_MAINFRAME_REFRESH)
	self:refreshPage(container);		
end

function EliteMapRewardPopup.onFunctionExt(eventName, container)
	if eventName:sub(1, 12) == "onRewardFeet" then
		
		local firstDigit = eventName:sub(13) 
		local index = 0
		if firstDigit == "0" then
			index = tonumber(eventName:sub(14))
		else
			index = tonumber(eventName:sub(13,14))
		end
		CCLuaLog("EliteMapRewardPopup:onFunctionExt -- index is "..index)
		EliteMapRewardPopup:onFeetById(container,index)
		
	end	
end

function EliteMapRewardPopup:onExecute(container)
	
end

function EliteMapRewardPopup:onFeetById(container,index)
	local thisResList = EliteMapManager:getDropItemCfg(curMapId)
	local resSize = #thisResList
	if index>resSize then
		return 
	else
		local resCfg = thisResList[index];	
		if resCfg then
			GameUtil:showTip(container:getVarNode(string.format("mRewardFeet%02d", index)), resCfg);
		end
	end
end

function EliteMapRewardPopup:onExit(container)
	curMapId = 0
	container:removePacket(HP_pb.BOSS_WIPE_S)
	container:removeMessage(MSG_MAINFRAME_REFRESH)
end
----------------------------------------------------------------

function EliteMapRewardPopup:refreshPage(container)
	local thisResList = EliteMapManager:getDropItemCfg(curMapId)
	local resSize = #thisResList
	
	--Boss阵容
	local bossInfo = EliteMapManager:getMonsterInfo(curMapId)
	NodeHelper:setNodesVisible(container,{
        mEnemyNode1 = #bossInfo>=1,
        mEnemyNode2 = #bossInfo>=2,
        mEnemyNode3 = #bossInfo>=3
    })
	for i=1,#bossInfo do
	    local str = {
	        ["mEnemyNum0"..i] = "Lv."..tostring(bossInfo[i].level),
	        ["mEnemyName0"..i] = tostring(bossInfo[i].name)
	    }
	    local pic = {
	        ["mEnemyPic0"..i] = bossInfo[i].pic
	    }
	    local quality = {
	        ["mEnemyFeet0"..i] = EliteMapManager:getQualityById(curMapId)
	    }
	    
	    for j=1,#bossInfo[i].skillName do
	        str["mSkillName"..(j+(i-1)*4)] = bossInfo[i].skillName[j]
	    end
	    NodeHelper:setStringForLabel(container, str);
	    NodeHelper:setSpriteImage(container, pic);
	    NodeHelper:setQualityFrames(container, quality);	
	end
	--
	
	for i=1,12 do
		local itemNode =string.format("mRewardNode%02d", i);		
		local flag = (i<=resSize)
		local ccbNode = container:getVarNode(itemNode)
		if ccbNode then
			ccbNode:setVisible(flag)
		end
	end
	 
	local index = 1 
	local lb2Str,numMap, quaMap, picMap = {}, {}, {},{};
	for _,value in pairs(thisResList) do
		
		local resCfg = value;		
		local resInfo = ResManagerForLua:getResInfoByTypeAndId(resCfg.type, resCfg.itemId, resCfg.count);				
		lb2Str[string.format("mRewardName%02d", index)]	= resInfo.name
		numMap[string.format("mRewardNum%02d", index)] = "x"..resInfo.count;	
		quaMap[string.format("mRewardFeet%02d", index)] = resInfo.quality;	
		picMap[string.format("mRewardPic%02d", index)] = resInfo.icon
		
		index = index + 1 
	end		
	NodeHelper:setStringForLabel(container, lb2Str);
	--NodeHelper:setStringForLabel(container, numMap);
	NodeHelper:setSpriteImage(container, picMap);
	NodeHelper:setQualityFrames(container, quaMap);		
	NodeHelper:setStringForLabel(container,{
		mTodaySurplusNum = common:getLanguageString("@TodaySurpluseNum")..tostring(UserInfo.stateInfo.eliteFightTimes)
	})
	
	local stageName = EliteMapManager:getStageName(curMapId)	
	local stageLevel = EliteMapManager:getLevelById(curMapId)
	CCLuaLog("EliteMapRewardPopup:refreshPage---stageName"..stageName .. "stageLevel"..stageLevel)
	NodeHelper:setStringForLabel(container,{
		mCareerName = (stageName.."("..stageLevel..")")
	})	
	
	--通过了该地图  可以显示扫荡	
	local curPassedMapId = EliteMapManager:getPassedMapIdByLevel(stageLevel)
	
	if curPassedMapId >= curMapId   then
		NodeHelper:setNodesVisible(container,{
			mFastSweepNode = true,
			mImmediatelyDekaron = false,
			mImmediatelyBtn = true
		})
	else
		NodeHelper:setNodesVisible(container,{
			mFastSweepNode = false,
			mImmediatelyDekaron = true,
			mImmediatelyBtn = false
		})
	end
	
	
	if UserInfo.roleInfo.prof == 1 then 
		NodeHelper:setNodesVisible(container,{
			mWarriorCareerBG01 = true,
			mHunterCareerBG01 = false,
			mMasterCareerBG01 = false,
		})
	elseif UserInfo.roleInfo.prof == 2 then 
		NodeHelper:setNodesVisible(container,{
			mWarriorCareerBG01 = false,
			mHunterCareerBG01 = true,
			mMasterCareerBG01 = false,
		})
	elseif UserInfo.roleInfo.prof == 3 then 
		NodeHelper:setNodesVisible(container,{
			mWarriorCareerBG01 = false,
			mHunterCareerBG01 = false,
			mMasterCareerBG01 = true,
		})
	end
	
end

----------------click event------------------------
function EliteMapRewardPopup:onClose(container)
	PageManager.popPage(thisPageName)
end

function EliteMapRewardPopup:onHelp(container)
	
end

function EliteMapRewardPopup:onAdd(container)
	EliteMapRewardPopup:onShowBuyLimit()
end

function EliteMapRewardPopup:onFastSweep(container)
	if UserInfo.playerInfo.vipLevel <1 then
		MessageBoxPage:Msg_Box_Lan("@MapFightWipeBossVIPLimit");
		return 
	end
	local leftTime = UserInfo.stateInfo.eliteFightTimes;
	if leftTime <= 0 then
		return EliteMapRewardPopup:onShowBuyLimit();		
	end
	local message = Battle_pb.HPBossWipe()
	if message~=nil then		
		message.mapId = curMapId;
		local pb_data = message:SerializeToString();
		PacketManager:getInstance():sendPakcet(HP_pb.BOSS_WIPE_C,pb_data,#pb_data,true);
	end
end


function EliteMapRewardPopup:onShowBuyLimit()
	local title = Language:getInstance():getString("@MapBuyTimeTitle")
	local message = Language:getInstance():getString("@EliteMapBuyTimeMsg")
	UserInfo.syncPlayerInfo()
	local vipLevel = UserInfo.playerInfo.vipLevel;
	local vipStr = tostring(vipLevel)
	UserInfo.syncStateInfo()
	
	--购买次数钻石配置，--buyBossFightPrice = 50,100,100，200
	local buyBossFightPrice = {}
	buyBossFightPrice[0]= 50
	buyBossFightPrice[1]= 100
	buyBossFightPrice[2]= 100
	
	
	local buyedTime = UserInfo.stateInfo.eliteFightBuyTimes
	local price = 200
	if buyedTime<3 then
		price = buyBossFightPrice[buyedTime]
	end
	local vipCfg = ConfigManager.getVipCfg();
	local buyTimeStr = tostring(buyedTime+1)
	local leftTime = vipCfg[vipLevel]["buyEliteFightTime"] - buyedTime
	local infoTab = {buyTimeStr,tostring(price), vipStr,tostring(leftTime) }
	
	PageManager.showConfirm(title,common:getGsubStr(infoTab,message), function(isSure)
		if isSure and UserInfo.isGoldEnough(price) then
			self:toPurchaseTimes(true);
		end
	end);

end

function EliteMapRewardPopup:toPurchaseTimes(flag)
	if flag then
		local message = Battle_pb.HPBuyEliteFightTimes()
		if message~=nil then
			message.times = 1;
			local pb_data = message:SerializeToString();
			PacketManager:getInstance():sendPakcet(HP_pb.BUY_ELITE_FIGHT_TIMES_C,pb_data,#pb_data,true);
		end
	end
end

function EliteMapRewardPopup:onImmediatelyDekaron(container)
	local leftTime = UserInfo.stateInfo.eliteFightTimes;
	if leftTime <= 0 then
		return EliteMapRewardPopup:onShowBuyLimit();		
	end
	local message = Battle_pb.HPBattleRequest()
	if message~=nil then
		message.battleType = Battle_pb.BATTLE_PVE_ELITE_BOSS;
		message.battleArgs = 	curMapId;			
		EliteMapManager.curFightMap = curMapId						
		local pb_data = message:SerializeToString();
		PacketManager:getInstance():sendPakcet(HP_pb.BATTLE_REQUEST_C,pb_data,#pb_data,false);
	end
	--直接显示战斗页面
	--[[PageManager.popPage(thisPageName)
	PageManager.showFightPage();--]]
end	

function EliteMapRewardPopup:onReceivePacket(container)
	local opcode = container:getRecPacketOpcode()
	local msgBuff = container:getRecPacketBuffer()
	
	--[[if opcode == HP_pb.BUY_BOSS_FIGHT_TIMES_S then
		local msg = Battle_pb.HPBuyBossFightTimesRet()
		msg:ParseFromString(msgBuff)
		--如果收到回包，刷新map页面
		if msg.bossFightTimes~=nil then
			UserInfo.stateInfo.bossFightTimes = msg.bossFightTimes
		end
		
		if msg.bossFightBuyTimes~=nil then
			UserInfo.stateInfo.bossFightBuyTimes = msg.bossFightBuyTimes
		end
		self:refreshPage(container)
		return
	end		--]]
	
	
	if opcode == HP_pb.BOSS_WIPE_S then
		--收到boss 扫荡回包，弹出结算页面
		local msg = Battle_pb.HPBossWipeRet()
		msg:ParseFromString(msgBuff)
		RegisterLuaPage("EliteMapWinPopup")
		EliteMapWinPopup_setAward(msg.award,msg.mapId)
		PageManager.pushPage("EliteMapWinPopup")
	end
	
	if opcode == HP_pb.STATE_INFO_SYNC_S then
		self:refreshPage(container);
	end
end

function EliteMapRewardPopup:onReceiveMessage(container)
	local message = container:getMessage()
	local typeId = message:getTypeId()
	if typeId == MSG_MAINFRAME_REFRESH then
		local pageName = MsgMainFrameRefreshPage:getTrueType(message).pageName
		if pageName == thisPageName then		
			self:refreshPage(container)			
		end
	end
end

function EliteMapRewardPopup_setMapId(mapId)
	curMapId = mapId
end

-------------------------------------------------------------------------
local CommonPage = require("CommonPage");
local EliteMapPage = CommonPage.newSub(EliteMapRewardPopup, thisPageName, option,EliteMapRewardPopup.onFunctionExt);