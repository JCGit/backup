----------------------------------------------------------------------------------
--[[
FILE:			MapPage.lua
ENCODING:		UTF-8, no-bomb
DESCRIPTION:	������
AUTHOR:			zhenhui
CREATED:		2014-08-6
--]]
----------------------------------------------------------------------------------
local Battle_pb = require "Battle_pb"
local Const_pb = require "Const_pb"
local UserInfo = require("UserInfo");
local thisPageName = "MapPage"



local option = {
	ccbiFile = "MapPage.ccbi",
	handlerMap = {
		onBuyLimit = "onBuyLimit",
		onWipeBoss = "onWipeBoss",
	}
}

local MapPageBase = {}

local NodeHelper = require("NodeHelper");
local TodoStr = "99";
local TodoImage = "UI/MainScene/UI/u_ico000.png";
local TodoPoster = "person/poster_char_Boss.png";

--------------------------------------------------------------
local MapItem = {
ccbiFile 	= "MapContent.ccbi",
ccbiFileWithBar 	= "MapContentWithBar.ccbi"
}

local exitFlag = false;
NewBeeFightBossDone = false
local isBoss = false
local MapCfg = ConfigManager.getMapCfg();
local monsterCfg = ConfigManager.getMonsterCfg();
local selectedTable = {};

local passedMapId = 0;
local currentBattleMap = 0;
BossWipeMsg = {}


function MapItem.onFunction(eventName, container)
	if eventName == "luaRefreshItemView" then
		MapItem.onRefreshItemView(container)
	elseif eventName == "onChanllage" then
		MapItem.onBoss(container)
	elseif eventName == "onMap" then
		MapItem.onMap(container)
	end
end

function MapItem.isInTable(tabel,id)
	for _,v in ipairs(tabel) do
		if v == id then
			return true;
		end
	end
	return false;
end


function MapItem.removeItemFromTable(tabel,id)
	for k,v in ipairs(tabel) do
		if v == id then
			table.remove(tabel,k);
			break
		end
	end
end


function MapItem.onRefreshItemView(container)
	local MapId = tonumber(container:getItemDate().mID)
	
	if MapCfg[MapId]== nil then
		CCLuaLog("ERROR in Map id, not found in Map.txt"..MapId);
		return;
	end
	local newStr =common:getLanguageString("@MapMonsterLevel", MapCfg[MapId]["monsterLevel"])
	
	local lb2Str = {
	mMapName 		= MapCfg[MapId]["name"],
	mMapLv 	= newStr
	};
	NodeHelper:setStringForLabel(container, lb2Str);
	local sprite2Img = {
	mMapPic = MapCfg[MapId]["path"]
	};
	NodeHelper:setSpriteImage(container, sprite2Img);
	
	
	
	--����ǵ�ǰ�Ĺؿ�
	local currentMapNode = container:getVarSprite("mSelectedTex")
	local currentBattleNode = container:getVarScale9Sprite("mSelectedPic")
	if MapId == currentBattleMap then
		currentMapNode :setVisible(true)
		currentBattleNode:setVisible(true)
	else
		currentMapNode :setVisible(false)
		currentBattleNode:setVisible(false)
	end
	
	
	
	--������µ�ͼ
	local newMapNode = container:getVarSprite("mNewMapTex")
	if MapId ~= currentBattleMap and MapId == (passedMapId+1) then
		newMapNode:setVisible(true)
	else
		newMapNode:setVisible(false)
	end
	
	--[[if MapId == (passedMapId+1) then
		NodeHelper:setNodeVisible(container:getVarNode("mMapPromptNode"),true)
	else
		NodeHelper:setNodeVisible(container:getVarNode("mMapPromptNode"),false)
	end--]]
	
	--�����ɨ��boss,��ʾɨ��boss���ӣ���������
	local bossWipeNode = container:getVarNode("mBossWipeNode")
	local bossFightNode = container:getVarNode("mBossFightNode")
	if bossWipeNode~=nil and bossFightNode~=nil then
		if UserInfo.stateInfo.bossWipe == 1 then
			if MapId<= passedMapId then
				bossWipeNode:setVisible(true)
				bossFightNode:setVisible(false)
			else
				bossWipeNode:setVisible(false)
				bossFightNode:setVisible(true)
			end								
		else
			bossWipeNode:setVisible(false)
			bossFightNode:setVisible(true)
		end
	end

	
		
end

--չʾ����������Ϣ
function MapItem.onBoss(container)
	local MapId = tonumber(container:getItemDate().mID)
	isBoss = true
	if UserInfo.stateInfo.bossWipe ==1 and MapId < (UserInfo.stateInfo.passMapId+1) then
		--bossɨ��
		local leftTime = UserInfo.stateInfo.bossFightTimes;
		if leftTime <= 0 then
			--��Ҫ����BOSS��ս�� ʹ�ô���
			local UserItemManager = require("UserItemManager");
			if UserItemManager:getCountByItemId(GameConfig.ItemId.ChallengeTicket) > 0 then
				local title = common:getLanguageString("@UseChallengeTicket_Title");
				local msg = common:getLanguageString("@UseChallengeTicket_Msg");
				PageManager.confirmUseItem(title, msg, GameConfig.ItemId.ChallengeTicket);				
			else
				MessageBoxPage:Msg_Box_Lan("@MapFightTimeNotEnough");
			end
			return 
		end
		if UserInfo.playerInfo.vipLevel <1 then
			MessageBoxPage:Msg_Box_Lan("@MapFightWipeBossVIPLimit");
			return 
		end
		
		local message = Battle_pb.HPBossWipe()
		if message~=nil then		
			message.mapId = MapId;
			local pb_data = message:SerializeToString();
			PacketManager:getInstance():sendPakcet(HP_pb.BOSS_WIPE_C,pb_data,#pb_data,false);
		end
		NewBeeFightBossDone = true
	else
		--��սBOSS
		local leftTime = UserInfo.stateInfo.bossFightTimes;
		if leftTime == 0 then
			--��Ҫ����BOSS��ս�� ʹ�ô���
			local UserItemManager = require("UserItemManager");
			if UserItemManager:getCountByItemId(GameConfig.ItemId.ChallengeTicket) > 0 then
				local title = common:getLanguageString("@UseChallengeTicket_Title");
				local msg = common:getLanguageString("@UseChallengeTicket_Msg");
				PageManager.confirmUseItem(title, msg, GameConfig.ItemId.ChallengeTicket);
			else
				MessageBoxPage:Msg_Box_Lan("@MapFightTimeNotEnough");
			end
			return 
		end
		local message = Battle_pb.HPBattleRequest()
		if message~=nil then
			message.battleType = Battle_pb.BATTLE_PVE_BOSS;
			message.battleArgs = 	MapId;
			

			--�ɷ���������������ʱ���ÿͻ����Լ���
	--[[		local bossId = MapCfg[MapId]["bossId"]	
			nextChanllengeName = monsterCfg[bossId]["name"]
			nextFightIsBoss = true
			PageManager.refreshPage("BattlePage");	--]]		
			
			
			local pb_data = message:SerializeToString();
			PacketManager:getInstance():sendPakcet(HP_pb.BATTLE_REQUEST_C,pb_data,#pb_data,false);
		end
		
		NewBeeFightBossDone = true
		exitFlag = true
	end
	
		
	
end

--չʾ����������Ϣ
function MapItem.onMap(container)
	local MapId = tonumber(container:getItemDate().mID)
	
	local message = Battle_pb.HPBattleRequest()
	if message~=nil then
		message.battleType = Battle_pb.BATTLE_PVE_MONSTER;
		message.battleArgs = 	MapId;
		
		local pb_data = message:SerializeToString();
		PacketManager:getInstance():sendPakcet(HP_pb.BATTLE_REQUEST_C,pb_data,#pb_data,false);
	end
	
	exitFlag = true
end



----------------------------------------------------------------------------------

-----------------------------------------------
--MapPageBaseҳ���е��¼�����
----------------------------------------------
function MapPageBase:onEnter(container)
	--self:registerPacket(container)
	container:registerPacket(HP_pb.BUY_BOSS_FIGHT_TIMES_S)
	container:registerPacket(HP_pb.BOSS_WIPE_S)
	
	RegisterLuaPage("BossWipeResultPage")
	
	container:registerMessage(MSG_MAINFRAME_REFRESH)
	
	NodeHelper:initScrollView(container, "mContent", 15);
	container.scrollview=container:getVarScrollView("mContent");
	if container.scrollview~=nil then
		container:autoAdjustResizeScrollview(container.scrollview);
	end		
	
	local mScale9Sprite = container:getVarScale9Sprite("mScale9Sprite")
	if mScale9Sprite ~= nil then
		container:autoAdjustResizeScale9Sprite( mScale9Sprite )
	end
	UserInfo.sync()
	passedMapId = UserInfo.stateInfo.passMapId
	currentBattleMap = UserInfo.stateInfo.curBattleMap	
	
	
	self:refreshPage(container);
	self:rebuildAllItem(container);
		
	self:showNewBeeHint(container)
end


function MapPageBase:onWipeBoss(container)
	local message = Player_pb.HPSysSetting();
	
	if message~=nil then	
		local newFlag
		if UserInfo.stateInfo.bossWipe == 1 then
			newFlag = 0
		else
			newFlag = 1
		end
		message.wipeBoss = newFlag;		
		UserInfo.stateInfo.bossWipe = newFlag;		
		local pb_data = message:SerializeToString();
		PacketManager:getInstance():sendPakcet(HP_pb.SYS_SETTING_C,pb_data,#pb_data,false);		
	end
	PageManager.refreshPage("MapPage");
end

function MapPageBase:showNewBeeHint(container)
	local isNewbeeFight = UserInfo.isNewbeeInFightBoss()	
	local hintNode = container:getVarNode("mNewbeeHintNode")
	if hintNode == nil then	
		return 
	end
	
	if NewBeeFightBossDone == true then
		hintNode:setVisible(false)
		return 
	end
	
	if isNewbeeFight then
		hintNode:setVisible(true)
	else
		hintNode:setVisible(false)
	end
end


function MapPageBase:onExecute(container)
	if exitFlag then
		PageManager.showFightPage();
		if isBoss then
			PageManager.refreshPage("BattlePage","Boss")	
			isBoss = false
		end
		exitFlag = false
	end
end


function MapPageBase:toPurchaseTimes(flag)
	if flag then
		local message = Battle_pb.HPBuyBossFightTimes()
		if message~=nil then
			message.times = 1;
			local pb_data = message:SerializeToString();
			PacketManager:getInstance():sendPakcet(HP_pb.BUY_BOSS_FIGHT_TIMES_C,pb_data,#pb_data,false);
		end
	end
end

function MapPageBase:onShowBuyLimit(container)
	local title = Language:getInstance():getString("@MapBuyTimeTitle")
	local message = Language:getInstance():getString("@MapBuyTimeMsg")
	UserInfo.syncPlayerInfo()
	local vipLevel = UserInfo.playerInfo.vipLevel;
	local vipStr = tostring(vipLevel)
	UserInfo.syncStateInfo()
	
	--���������ʯ���ã�--buyBossFightPrice = 50,100,100��200
	local buyBossFightPrice = {}
	buyBossFightPrice[0]= 50
	buyBossFightPrice[1]= 100
	buyBossFightPrice[2]= 100
	
	
	local buyedTime = UserInfo.stateInfo.bossFightBuyTimes
	local price = 200
	if buyedTime<3 then
		price = buyBossFightPrice[buyedTime]
	end
	local vipCfg = ConfigManager.getVipCfg();
	local buyTimeStr = tostring(buyedTime+1)
	local leftTime = vipCfg[vipLevel]["buyBossFightTime"] - buyedTime
	local infoTab = {buyTimeStr,tostring(price), vipStr,tostring(leftTime) }
	
	PageManager.showConfirm(title,common:getGsubStr(infoTab,message), function(isSure)
		if isSure and UserInfo.isGoldEnough(price) then
			self:toPurchaseTimes(true);
		end
	end);

end

function MapPageBase:onExit(container)
	--self:removePacket(container)
	container:removePacket(HP_pb.BUY_BOSS_FIGHT_TIMES_S)
	container:removePacket(HP_pb.BOSS_WIPE_S)
	container:removeMessage(MSG_MAINFRAME_REFRESH)
	NodeHelper:deleteScrollView(container);
	--MapPageBase:clearAllItem(container)
end
----------------------------------------------------------------

function MapPageBase:refreshPage(container)
	UserInfo.syncStateInfo();
	local leftTime = UserInfo.stateInfo.bossFightTimes;
	local bossFightLeftTimes = container:getVarLabelBMFont("mLimit")
	bossFightLeftTimes:setString(tostring(leftTime))
	
	local wipeBossNode = container:getVarNode("mWipeBossNode")
	if wipeBossNode~=nil then
		--vip1 �����ܿ���
		if UserInfo.playerInfo.vipLevel >=1 then
			wipeBossNode:setVisible(true)
		else
			wipeBossNode:setVisible(false)
		end
	end
	
	local checkSprite = container:getVarSprite("mChoice02")
	if checkSprite ~=nil then
		if UserInfo.stateInfo.bossWipe ==1 then
			checkSprite:setVisible(true)
		else
			checkSprite:setVisible(false)
		end
	end
end


----------------scrollview-------------------------
function MapPageBase:rebuildAllItem(container)
	self:clearAllItem(container);
	self:buildItem(container);
end

function MapPageBase:clearAllItem(container)
	container.m_pScrollViewFacade:clearAllItems();
	container.mScrollViewRootNode:removeAllChildren();
end

function MapPageBase:buildItem(container)
	
	--�ܹ���ͼ��С
	local size = GameConfig.CurrentOpenMaxMap;
	
	
	local ccbiFile = MapItem.ccbiFile
	
	if size == 0 or ccbiFile == nil or ccbiFile == ''then return end
	local iMaxNode = container.m_pScrollViewFacade:getMaxDynamicControledItemViewsNum()
	local iCount = 0
	local fOneItemHeight = 0
	local fOneItemWidth = 0
	local currentPos = 0
	local totalSize = math.min(size,passedMapId+1)
	--totolSize = 6
	for i=totalSize, 1,-1 do
		local pItemData = CCReViSvItemData:new_local()
		--��ʱд��������ְҵ������1-10 ��Ӧսʿ��11-20��Ӧ���ˣ�21-30 ��Ӧ��ʦ
		pItemData.mID = i
		pItemData.m_iIdx = i
		pItemData.m_ptPosition = ccp(0, currentPos)
		
		if iCount < iMaxNode then
			if i == totalSize then
				ccbiFile = MapItem.ccbiFileWithBar
			else
				ccbiFile = MapItem.ccbiFile
			end
			local pItem = ScriptContentBase:create(ccbiFile)
			--pItem:release();
			pItem.id = iCount
			pItem:registerFunctionHandler(MapItem.onFunction)			
			fOneItemHeight = pItem:getContentSize().height			
			currentPos = currentPos + fOneItemHeight
			if fOneItemWidth < pItem:getContentSize().width then
				fOneItemWidth = pItem:getContentSize().width
			end
			container.m_pScrollViewFacade:addItem(pItemData, pItem.__CCReViSvItemNodeFacade__)
		else
			container.m_pScrollViewFacade:addItem(pItemData)
		end
		iCount = iCount + 1
	end
	
	local size = CCSizeMake(fOneItemWidth, currentPos)
	container.mScrollView:setContentSize(size)
	if totalSize >3 then
		container.mScrollView:setContentOffset(ccp(0, 0))
	else
		container.mScrollView:setContentOffset(ccp(0, container.mScrollView:getViewSize().height - container.mScrollView:getContentSize().height * container.mScrollView:getScaleY()))
	end
	
	container.m_pScrollViewFacade:setDynamicItemsStartPosition(iCount - 1);
	container.mScrollView:forceRecaculateChildren();
end

----------------click event------------------------
function MapPageBase:onCancel(container)
	--PageManager.popPage(thisPageName);
end

function MapPageBase:onBuyLimit(container)
	--Todo
	self:onShowBuyLimit(container)
end

--�ذ�����

function MapPageBase:onReceivePacket(container)
	local opcode = container:getRecPacketOpcode()
	local msgBuff = container:getRecPacketBuffer()
	
	if opcode == HP_pb.BUY_BOSS_FIGHT_TIMES_S then
		local msg = Battle_pb.HPBuyBossFightTimesRet()
		msg:ParseFromString(msgBuff)
		--����յ��ذ���ˢ��mapҳ��
		if msg.bossFightTimes~=nil then
			UserInfo.stateInfo.bossFightTimes = msg.bossFightTimes
		end
		
		if msg.bossFightBuyTimes~=nil then
			UserInfo.stateInfo.bossFightBuyTimes = msg.bossFightBuyTimes
		end
		self:refreshPage(container)
		return
	end
	
	
	if opcode == HP_pb.BOSS_WIPE_S then
		--�յ�boss ɨ���ذ�����������ҳ��
		local msg = Battle_pb.HPBossWipeRet()
		msg:ParseFromString(msgBuff)
		BossWipeMsg = msg;
		PageManager.pushPage("BossWipeResultPage");
	end
end


--�̳д���Ļ���ͬʱ������Ϣ��������ͬʱ����,ͨ��tag������

function MapPageBase:onReceiveMessage(container)
	local message = container:getMessage()
	local typeId = message:getTypeId()
	if typeId == MSG_MAINFRAME_REFRESH then
		local pageName = MsgMainFrameRefreshPage:getTrueType(message).pageName
		if pageName == thisPageName then
			local posx = container.mScrollView:getContainer():getPositionX();
			local posy = container.mScrollView:getContainer():getPositionY();
			MapPageBase:rebuildAllItem(container)
			self:refreshPage(container)
			container.mScrollView:getContainer():setPosition(ccp(posx,posy));
		end
	end
end


--[[
function MapPageBase:registerPacket(container)
	for key, opcode in pairs(opcodes) do
		if string.sub(key, -1) == "S" then
			container:registerPacket(opcode)
		end
	end
end

function MapPageBase:removePacket(container)
	for key, opcode in pairs(opcodes) do
		if string.sub(key, -1) == "S" then
			container:removePacket(opcode)
		end
	end
end
--]]
-------------------------------------------------------------------------
local CommonPage = require("CommonPage");
local MapPage = CommonPage.newSub(MapPageBase, thisPageName, option);