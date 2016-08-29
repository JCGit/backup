----------------------------------------------------------------------------------
--[[
FILE:			ViewPlayerInfoPage.lua
ENCODING:		UTF-8, no-bomb
DESCRIPTION:
AUTHOR:			hgs
CREATED:		2014-01-21
--]]
----------------------------------------------------------------------------------
local Const_pb = require("Const_pb");
local HP_pb = require("HP_pb");
local Snapshot_pb = require("Snapshot_pb");
local Friend_pb = require("Friend_pb");
local UserInfo = require("UserInfo");
local NodeHelper = require("NodeHelper");
------------local variable for system api--------------------------------------
local tostring = tostring;
local tonumber = tonumber;
local string = string;
local pairs = pairs;
local titleManager = require("TitleManager")
--------------------------------------------------------------------------------

local thisPageName = "ViewPlayerInfoPage";

local opcodes = {
FRIEND_ADD_S = HP_pb.FRIEND_ADD_S,
FRIEND_DELETE_S = HP_pb.FRIEND_DELETE_S,
MESSAGE_CANCEL_SHIELD_S = HP_pb.MESSAGE_CANCEL_SHIELD_S,
MESSAGE_SHIELD_S = HP_pb.MESSAGE_SHIELD_S
};

local EquipPartNames = {
["Helmet"] 		= Const_pb.HELMET,
["Neck"]		= Const_pb.NECKLACE,
["Finger"]		= Const_pb.RING,
["Wrist"]		= Const_pb.GLOVE,
["Waist"]		= Const_pb.BELT,
["Feet"]		= Const_pb.SHOES,
["Chest"]		= Const_pb.CUIRASS,
["Legs"]		= Const_pb.LEGGUARD,
["MainHand"]	= Const_pb.WEAPON1,
["OffHand"]		= Const_pb.WEAPON2
};

local option = {
ccbiFile = "ArenaViewEquipmentPopUp.ccbi",
ccbiPadFile = "ArenaViewEquipmentPopUp_ipad.ccbi",
handlerMap = {
onClose			= "onClose",
onShieldMessage = "onShieldMessage",
onSendMessage = "onSendMessage",
onAddFriend = "onAddFriend"
},
opcode = opcodes
};
for equipName, _ in pairs(EquipPartNames) do
	option.handlerMap["on" .. equipName] = "showEquipDetail";
end

local ViewPlayerInfoPageBase = {};

local PBHelper	= require("PBHelper");

local ItemManager = require("ItemManager");

local thisPlayerId = 0;
local playerInfo = {};
local thisFlagShowButton = false
-----------------------------------------------
--EquipPageBase页面中的事件处理
----------------------------------------------

function ViewPlayerInfoPageBase:onLoad(container)
	local height = CCDirector:sharedDirector():getWinSize().height
	if height<900 then
		container:loadCcbiFile(option.ccbiPadFile);
	else
		container:loadCcbiFile(option.ccbiFile);
	end	
end

function ViewPlayerInfoPageBase:onEnter(container)
	self:setSelectTxtDisappear(container)
	
	thisFlagShowButton = not (UserInfo.playerInfo.playerId == thisPlayerId)
	NodeHelper:setNodesVisible(container,
	{
	mAddFriendNode = thisFlagShowButton,
	mSendMessageNode = thisFlagShowButton,
	mShieldMessageNode = thisFlagShowButton
	})
	UserInfo.sync()
	self:refreshPage(container);
	self:registerPacket(container);
	
	local roleId = ViewPlayerInfo:getRoleInfo().itemId;
      local cfg =GameConfig.SpineAvatar[roleId];
      CCArmatureDataManager:sharedArmatureDataManager():addArmatureFileInfo(cfg[1].."/"..cfg[2]..".png", cfg[1].."/"..cfg[2]..".plist", cfg[1].."/"..cfg[2]..".xml")
	--self:getPlayerInfo();
end

function ViewPlayerInfoPageBase:setSelectTxtDisappear( container )
	container:getVarSprite("mHelmetPic"):setVisible(false)
	
	container:getVarSprite("mFingerPic"):setVisible(false)
	container:getVarSprite("mWaistPic"):setVisible(false)
	container:getVarSprite("mChestPic"):setVisible(false)
	container:getVarSprite("mMainHandPic"):setVisible(false)
	container:getVarSprite("mOffHandPic"):setVisible(false)
	container:getVarSprite("mLegsPic"):setVisible(false)
	container:getVarSprite("mFeetPic"):setVisible(false)
	container:getVarSprite("mWristPic"):setVisible(false)
	container:getVarSprite("mNeckPic"):setVisible(false)
end

function ViewPlayerInfoPageBase:onExit(container)

      local roleId = ViewPlayerInfo:getRoleInfo().itemId;
      local cfg =GameConfig.SpineAvatar[roleId];
	--CCArmatureDataManager:sharedArmatureDataManager():removeArmatureFileInfo(cfg[1].."/"..cfg[2]..".xml")


	self:removePacket(container);
	ViewPlayerInfo:clearInfo();
	--清除纹理缓存
	GameUtil:purgeCachedData();
end
----------------------------------------------------------------
function ViewPlayerInfoPageBase:getPlayerInfo()
	ViewPlayerInfo:getInfo(thisPlayerId);
end

function ViewPlayerInfoPageBase:refreshPage(container)
	self:showPlayerInfo(container);
	self:showFightAttrInfo(container);
	self:showEquipInfo(container);
	self:showFunctionButtonInfo(container)
end

function ViewPlayerInfoPageBase:showFunctionButtonInfo(container)
	local lb2Str = {
	mShieldMessage 			= ViewPlayerInfo:isShieldLabelStr(),
	mSendMessage 				= ViewPlayerInfo:isSendAllowLabelStr(),
	mAddFriend			= ViewPlayerInfo:isFriendLabelStr(),	
	};
	NodeHelper:setStringForLabel(container, lb2Str);
end


function ViewPlayerInfoPageBase:showPlayerInfo(container)
	local level = ViewPlayerInfo:getRoleInfo().level;
	local lb2Str = {
	mHpNum 					= ViewPlayerInfo:getRoleAttrById(Const_pb.HP),
	mMpNum 					= ViewPlayerInfo:getRoleAttrById(Const_pb.MP),
	mLV						= "LV." .. ViewPlayerInfo:getRoleInfo().level,
	mName				 	= "LV." ..level .. " " ..  ViewPlayerInfo:getRoleInfo().name,
	mFightingCapacityNum 	= ViewPlayerInfo:getRoleInfo().fight,
	mPlayerId 				= "ID :  " .. ViewPlayerInfo:getPlayerInfo().playerId,
	mOccupationName			= ViewPlayerInfo:getProfessionName()
	};
	NodeHelper:setStringForLabel(container, lb2Str);

	-- 称号
	local titleNode = container:getVarLabelBMFont("mPlayerTitle")
	local titleSprite = container:getVarSprite("mPlayerTitleSprite")
	titleManager:setLabelTitleWithBG(titleNode,ViewPlayerInfo:getTittleInfo().titleId)
	titleManager:setTitleBG(titleSprite,ViewPlayerInfo:getTittleInfo().titleId)

	local roleId = ViewPlayerInfo:getRoleInfo().itemId;
	local RoleManager = require("RoleManager");
	NodeHelper:setSpriteImage(container, {
		mOccupation = RoleManager:getOccupationIconById(roleId)
	});
			
	if GameConfig.ShowSpineAvatar == false then 
		NodeHelper:setSpriteImage(container, {
		mHeroPic 	= RoleManager:getPosterById(roleId)
	});
	else
		local heroNode = container:getVarNode("mHero");
		if heroNode then
			local heroSprite = container:getVarSprite("mHeroPic")
			if heroSprite then
				heroSprite:setVisible(false)
			end	
			heroNode:removeAllChildren()
			local spineCCBI = ScriptContentBase:create(GameConfig.SpineCCBI[roleId])
			
			 
                  local cfg =GameConfig.SpineAvatar[roleId];
                  CCArmatureDataManager:sharedArmatureDataManager():addArmatureFileInfo(cfg[1].."/"..cfg[2]..".png", cfg[1].."/"..cfg[2]..".plist", cfg[1].."/"..cfg[2]..".xml")
			local armature = CCArmature:create(GameConfig.SpineAvatar[roleId][2])
			armature :setAnchorPoint(CCPointMake(0.5,1))
			local offsetY = -20;
			local offsetX = 0;
			      
			      if roleId == 2 then
			            offsetY = -40
			      elseif roleId ==6 then
			            offsetX = 45
			            offsetY = -40
			      end
			      
			      armature :setPosition(offsetX, offsetY)
			      armature:setScale(spineCCBI:getScale());
			      armature :getAnimation(): playWithIndex(0)
			      spineCCBI :addChild(armature)
			
			heroNode:addChild(spineCCBI)
			
			
			
			local spineContainer = spineCCBI:getVarNode("mSpineNode");
			if spineContainer then
				spineContainer:removeAllChildren();
				local spine = SpineContainer:create(unpack(GameConfig.SpineAvatar[roleId]));
				local spineNode = tolua.cast(spine, "CCNode");
				spineContainer:addChild(spineNode);
				spine:runAnimation(1, "Stand", -1);
			end
			spineCCBI:release()
		end	
	end
end

function ViewPlayerInfoPageBase:showFightAttrInfo(container)
	local lb2Str = {
	mStrengthNum 			= ViewPlayerInfo:getRoleAttrById(Const_pb.STRENGHT),
	mDamageNum 				= ViewPlayerInfo:getDamageString(),
	mDexterityNum			= ViewPlayerInfo:getRoleAttrById(Const_pb.AGILITY),
	mArmorNum				= ViewPlayerInfo:getRoleAttrById(Const_pb.ARMOR),
	mCritRatingNum			= ViewPlayerInfo:getRoleAttrById(Const_pb.CRITICAL),
	mIntelligenceNum	 	= ViewPlayerInfo:getRoleAttrById(Const_pb.INTELLECT),
	mCreateRoleNum			= ViewPlayerInfo:getRoleAttrById(Const_pb.MAGDEF),
	mDodgeNum				= ViewPlayerInfo:getRoleAttrById(Const_pb.DODGE),
	mStaminaNum				= ViewPlayerInfo:getRoleAttrById(Const_pb.STAMINA),
	mHitRatingNum			= ViewPlayerInfo:getRoleAttrById(Const_pb.HIT),
	mTenacityNum 			= ViewPlayerInfo:getRoleAttrById(Const_pb.RESILIENCE)
	};
	NodeHelper:setStringForLabel(container, lb2Str);
end

function ViewPlayerInfoPageBase:showEquipInfo(container)
	local lb2Str = {};
	local sprite2Img = {};
	local itemImg2Qulity = {};
	local scaleMap = {};
	local nodesVisible = {};
	
	for equipName, part in pairs(EquipPartNames) do
		local levelStr = "";
		local enhanceLvStr = "";
		local icon = GameConfig.Image.ClickToSelect;
		local quality = GameConfig.Default.Quality;
		local aniVisible = false;
		local gemVisible = false;
		
		local name 	= "m" .. equipName;
		local roleEquip = ViewPlayerInfo:getRoleEquipByPart(part);
		local userEquip = nil;
		if roleEquip then
			local equipId = roleEquip.equipItemId;
			levelStr = "Lv" .. EquipManager:getLevelById(equipId);
			enhanceLvStr = "+" .. roleEquip.strength;
			icon = EquipManager:getIconById(equipId);
			quality = EquipManager:getQualityById(equipId);
			container:getVarSprite("m" .. equipName .. "Pic"):setVisible(true)
			
			userEquip = ViewPlayerInfo:getEquipById(roleEquip.equipId);
			aniVisible = UserEquipManager:isEquipGodly(userEquip);
			local gemInfo = PBHelper:getGemInfo(roleEquip.gemInfo);
			if table.maxn(gemInfo) > 0 then
				gemVisible = true;
				for i = 1, 4 do
					local gemId = gemInfo[i];
					nodesVisible[name .. "GemBG" .. i] = gemId ~= nil;
					local gemSprite = name .. "Gem0" .. i;
					nodesVisible[gemSprite] = false;
					if gemId ~= nil and gemId > 0 then
						local icon = ItemManager:getGemSmallIcon(gemId);
						if icon then
							nodesVisible[gemSprite] = true;
							sprite2Img[gemSprite] = icon;
						end
					end
				end
			end
		end
		
		lb2Str[name .. "Lv"] 			= levelStr;
		lb2Str[name .. "LvNum"]			= enhanceLvStr;
		sprite2Img[name .. "Pic"] 		= icon;
		itemImg2Qulity[name] 			= quality;
		scaleMap[name .. "Pic"] 		= 1.0;
		nodesVisible[name .. "Ani"]		= aniVisible;
		nodesVisible[name .. "GemNode"]	= gemVisible;
		nodesVisible[name .. "Point"] 	= false;
		
		NodeHelper:addEquipAni(container, name .. "Ani", aniVisible, nil, userEquip);
	end
	
	NodeHelper:setStringForLabel(container, lb2Str);
	NodeHelper:setSpriteImage(container, sprite2Img, scaleMap);
	NodeHelper:setQualityFrames(container, itemImg2Qulity);
	NodeHelper:setNodesVisible(container, nodesVisible);
end

----------------click event------------------------
function ViewPlayerInfoPageBase:showEquipDetail(container, eventName)
	local part = EquipPartNames[string.sub(eventName, 3)];
	local roleEquip = ViewPlayerInfo:getRoleEquipByPart(part);
	if roleEquip then
		PageManager.viewEquipInfo(roleEquip.equipId);
	end
end

function ViewPlayerInfoPageBase:onClose()
	PageManager.popPage(thisPageName);
end

--留言
function ViewPlayerInfoPageBase:onLeaveMessage(container)
	if thisPlayerId == UserInfo.playerInfo.playerId then
		MessageBoxPage:Msg_Box("@MessageToSelfErro")
		return
	end
	
	LeaveMsgDetail_setPlayId( thisPlayerId , ViewPlayerInfo:getRoleInfo().name)
	PageManager.pushPage("LeaveMessageDetailPage")
end

--屏蔽消息
function ViewPlayerInfoPageBase:onShieldMessage(container)
	if ViewPlayerInfo.isShield then
		--解除屏蔽 1 屏蔽 2 取消屏蔽
		local msg = Friend_pb.HPMsgShield();
		msg.playerId = thisPlayerId;
		msg.type = 2
		common:sendPacket(HP_pb.MESSAGE_CANCEL_SHIELD_C, msg);
	else
		--屏蔽
		local msg = Friend_pb.HPMsgShield();
		msg.playerId = thisPlayerId;
		msg.type = 1
		common:sendPacket(HP_pb.MESSAGE_SHIELD_C, msg);
	end
end

--发送消息
function ViewPlayerInfoPageBase:onSendMessage(container)
	if ViewPlayerInfo.isSendAllow then
		--跳转到个人聊天页面
		local ChatManager = require("ChatManager")
		local Friend_pb = require("Friend_pb")
		--add playerinfo into msgbox
		local chatUnit = Friend_pb.MsgBoxUnit()
		chatUnit.playerId = thisPlayerId
		chatUnit.name = ViewPlayerInfo:getRoleInfo().name
		chatUnit.level = ViewPlayerInfo:getRoleInfo().level
		chatUnit.roleItemId = ViewPlayerInfo:getRoleInfo().itemId
		ChatManager.insertPrivateMsg(thisPlayerId,chatUnit,nil, false,false)
		
		ChatManager.setCurrentChatPerson(thisPlayerId)
		--PageManager.popPage(thisPageName)
		PageManager.popAllPage()		
		PageManager.showFightPage();
		PageManager.refreshPage("BattlePage","PrivateChat")
		
	else
		MessageBoxPage:Msg_Box("@PrivateChatLimitInvoke")
	end
end

--添加或删除消息
function ViewPlayerInfoPageBase:onAddFriend(container)
	if ViewPlayerInfo.isFriend then
		--删除好友
		local msg = Friend_pb.HPFriendDel();
		msg.targetId = thisPlayerId;
		common:sendPacket(HP_pb.FRIEND_DELETE_C, msg);
	else
		--添加好友
		local msg = Friend_pb.HPFriendAdd();
		msg.targetId = thisPlayerId;
		common:sendPacket(HP_pb.FRIEND_ADD_C, msg);
	end
end

--------------------------------------------------------

--回包处理

function ViewPlayerInfoPageBase:onReceivePacket(container)
	local opcode = container:getRecPacketOpcode()
	local msgBuff = container:getRecPacketBuffer()
	
	if opcode == opcodes.FRIEND_ADD_S then
		ViewPlayerInfo:setIsFriend(true)
		MessageBoxPage:Msg_Box("@AddFriendSuccess")
		self:showFunctionButtonInfo(container)
		PageManager.refreshPage("FriendPage");
	end
	
	if opcode == opcodes.FRIEND_DELETE_S then
		ViewPlayerInfo:setIsFriend(false)
		MessageBoxPage:Msg_Box("@DelFriendSuccess")
		self:showFunctionButtonInfo(container)
		PageManager.refreshPage("FriendPage");
	end
	
	if opcode == opcodes.MESSAGE_SHIELD_S then
		ViewPlayerInfo:setIsShield(true)
		MessageBoxPage:Msg_Box("@SheMsgSuccess")
		self:showFunctionButtonInfo(container)
		local ChatManager = require("ChatManager")
		ChatManager.addShieldList(thisPlayerId)
	end
	
	if opcode == opcodes.MESSAGE_CANCEL_SHIELD_S then
		ViewPlayerInfo:setIsShield(false)
		MessageBoxPage:Msg_Box("@UnSheMsgSuccess")
		self:showFunctionButtonInfo(container)
		local ChatManager = require("ChatManager")
		ChatManager.removeShieldList(thisPlayerId)
	end

end


function ViewPlayerInfoPageBase:registerPacket(container)
	for key, opcode in pairs(opcodes) do
		if string.sub(key, -1) == "S" then
			container:registerPacket(opcode)
		end
	end
end

function ViewPlayerInfoPageBase:removePacket(container)
	for key, opcode in pairs(opcodes) do
		if string.sub(key, -1) == "S" then
			container:removePacket(opcode)
		end
	end
end
-------------------------------------------------------------------------
local CommonPage = require("CommonPage");
local ViewPlayerInfoPage = CommonPage.newSub(ViewPlayerInfoPageBase, thisPageName, option);

function ViewPlayerInfoPage_setPlayerId(playerId ,flagShowButton)
	thisPlayerId = playerId
	if flagShowButton == nil then
		thisFlagShowButton = true
	else
		thisFlagShowButton = flagShowButton
	end
	
end
