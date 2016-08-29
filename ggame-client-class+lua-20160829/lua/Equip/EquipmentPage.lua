----------------------------------------------------------------------------------
--[[
	FILE:			EquipmentPage.lua
	ENCODING:		UTF-8, no-bomb
	DESCRIPTION:		装备页面
	AUTHOR:			hgs
	CREATED:		2014-01-21
--]]
----------------------------------------------------------------------------------
local Const_pb = require("Const_pb");
local HP_pb = require("HP_pb");
local UserInfo = require("UserInfo");
local NodeHelper = require("NodeHelper");
------------local variable for system api--------------------------------------
local tostring = tostring;
local tonumber = tonumber;
local string = string;
local pairs = pairs;
local titleManager = require("TitleManager")
--------------------------------------------------------------------------------
local fullAttributePage = "FullAttributePage";

local thisPageName = "EquipmentPage";

local opcodes = {
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
	ccbiFile = "EquipmentPage.ccbi",
	handlerMap = {
		onMoreAttribute		= "showMoreAttribute",
		onPlayerTitleBtn  	= "showPlayerTitle",
	},
	opcode = opcodes
};
for equipName, _ in pairs(EquipPartNames) do
	option.handlerMap["on" .. equipName] = "showEquipDetail";
end

local EquipPageBase = {};

local PBHelper = require("PBHelper");

local RoleManager = require("RoleManager");
local ItemManager = require("ItemManager");
	
-----------------------------------------------
--EquipPageBase
----------------------------------------------
function EquipPageBase:onEnter(container)

      local roleId = UserInfo.roleInfo.itemId; 
      local cfg =GameConfig.SpineAvatar[roleId];
      CCArmatureDataManager:sharedArmatureDataManager():addArmatureFileInfo(cfg[1].."/"..cfg[2]..".png", cfg[1].."/"..cfg[2]..".plist", cfg[1].."/"..cfg[2]..".xml")
	container:registerMessage(MSG_SEVERINFO_UPDATE);
	container:registerMessage(MSG_MAINFRAME_REFRESH);
	self:refreshPage(container);
end

function EquipPageBase:onExecute(container)
end

function EquipPageBase:onExit(container)
	container:removeMessage(MSG_SEVERINFO_UPDATE);
	container:removeMessage(MSG_MAINFRAME_REFRESH);
	
	local roleId = UserInfo.roleInfo.itemId; 
      local cfg =GameConfig.SpineAvatar[roleId];
	--CCArmatureDataManager:sharedArmatureDataManager():removeArmatureFileInfo(cfg[1].."/"..cfg[2]..".xml")
end
----------------------------------------------------------------

function EquipPageBase:refreshPage(container)
	UserInfo.sync();

	self:showPlayerInfo(container);
	self:showFightAttrInfo(container);
	self:showEquipInfo(container);
end

function EquipPageBase:showPlayerInfo(container)
	local lb2Str = {
		mHpNum 					= UserInfo.getRoleAttrById(Const_pb.HP),
		mMpNum 					= UserInfo.getRoleAttrById(Const_pb.MP),
		mLV						= "LV." .. UserInfo.roleInfo.level,
		mName				 	= UserInfo.roleInfo.name,
		mFightingCapacityNum 	= UserInfo.roleInfo.fight,
		mOccupationName			= UserInfo.getProfessionName()
	};
	NodeHelper:setStringForLabel(container, lb2Str);

	-- 称号
	NodeHelper:setNodeVisible(container:getVarNode("mPlayerTitleNode"),titleManager.myOwnTitleIds~=nil)
	local titleLabelNode = container:getVarLabelBMFont("mPlayerTitle")
	local titleSprite = container:getVarSprite("mPlayerTitleSprite")
	titleManager:setLabelTitleWithBG(titleLabelNode,titleManager.myNowTitleId)
	titleManager:setTitleBG(titleSprite,titleManager.myNowTitleId)

	local roleId = UserInfo.roleInfo.itemId;   
	NodeHelper:setSpriteImage(container, {	
		mOccupation = RoleManager:getOccupationIconById(UserInfo.roleInfo.itemId)
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
			if heroNode:getChildByTag(10010) == nil then
				local spineCCBI = ScriptContentBase:create(GameConfig.SpineCCBI[roleId])
				spineCCBI:setTag(10010)
				
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
			else
--				local spineNode  =  heroNode:getChildByTag(10010)
				
			end				
		end	
	end		

end

function EquipPageBase:showFightAttrInfo(container)
	local lb2Str = {
		mStrengthNum 			= UserInfo.getRoleAttrById(Const_pb.STRENGHT),
		mDamageNum 				= UserInfo.getDamageString(),
		mDexterityNum			= UserInfo.getRoleAttrById(Const_pb.AGILITY),
		mArmorNum				= UserInfo.getRoleAttrById(Const_pb.ARMOR),
		mCritRatingNum			= UserInfo.getRoleAttrById(Const_pb.CRITICAL),
		mIntelligenceNum	 	= UserInfo.getRoleAttrById(Const_pb.INTELLECT),
		mCreateRoleNum			= UserInfo.getRoleAttrById(Const_pb.MAGDEF),
		mDodgeNum				= UserInfo.getRoleAttrById(Const_pb.DODGE),
		mStaminaNum				= UserInfo.getRoleAttrById(Const_pb.STAMINA),
		mHitRatingNum			= UserInfo.getRoleAttrById(Const_pb.HIT),
		mTenacityNum 			= UserInfo.getRoleAttrById(Const_pb.RESILIENCE)
	};
	NodeHelper:setStringForLabel(container, lb2Str);
end

function EquipPageBase:showEquipInfo(container)
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
		local showNotice = UserEquipManager:isPartNeedNotice(part);
		
		local name 	= "m" .. equipName;
		local roleEquip = UserInfo.getEquipByPart(part);
		if roleEquip then
			local equipId = roleEquip.equipItemId;
			levelStr = "Lv" .. EquipManager:getLevelById(equipId);
			enhanceLvStr = "+" .. roleEquip.strength;
			icon = EquipManager:getIconById(equipId);
			quality = EquipManager:getQualityById(equipId);
			aniVisible = UserEquipManager:isGodly(roleEquip.equipId);
			local userEquip = UserEquipManager:getUserEquipById(roleEquip.equipId);		
			local gemInfo = PBHelper:getGemInfo(userEquip.gemInfos);
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
		nodesVisible[name .. "Point"] 	= showNotice;
		
		if name == "mMainHand" then
			
			if UserInfo.roleInfo.level == 2 or showNotice == false then
				nodesVisible["mMainHandHintNode"] = showNotice;
			end
			
		end
		
		
		NodeHelper:addEquipAni(container, name .. "Ani", aniVisible, roleEquip and roleEquip.equipId);
	--[[	
		if aniVisible then
			local ps = container:getParticleSystem(name .. "AniFX");
			if ps then ps:resetSystem(); end
		end
	--]]
	end
	
	NodeHelper:setStringForLabel(container, lb2Str);
	NodeHelper:setSpriteImage(container, sprite2Img, scaleMap);
	NodeHelper:setQualityFrames(container, itemImg2Qulity);
	NodeHelper:setNodesVisible(container, nodesVisible);
end
	
----------------click event------------------------
function EquipPageBase:showMoreAttribute(container)
	RegisterLuaPage(fullAttributePage);
	PageManager.pushPage(fullAttributePage);
end

function EquipPageBase:showPlayerTitle(container)
	PageManager.pushPage("PlayerTitlePage")
end

function EquipPageBase:showEquipDetail(container, eventName)
	local partName = string.sub(eventName, 3);
	local part = EquipPartNames[partName];
	UserEquipManager:cancelNotice(part);
	NodeHelper:setNodesVisible(container, {["m" .. partName .. "Point"] = false});
	local roleEquip = UserInfo.getEquipByPart(part);
	if roleEquip then
		PageManager.showEquipInfo(roleEquip.equipId);
	else
		EquipSelectPage_setPart(part);
		PageManager.pushPage("EquipSelectPage");
	end
end	

function EquipPageBase:onReceiveMessage(container)
	local message = container:getMessage()
	local typeId = message:getTypeId();
	if typeId == MSG_SEVERINFO_UPDATE then
		local opcode = MsgSeverInfoUpdate:getTrueType(message).opcode;
		if opcode == HP_pb.ROLE_INFO_SYNC_S or opcode == HP_pb.EQUIP_INFO_SYNC_S then
			if UserEquipManager:hasInited() then
				self:refreshPage(container);
			end
		end
	elseif typeId == MSG_MAINFRAME_REFRESH then
		local pageName = MsgMainFrameRefreshPage:getTrueType(message).pageName;
		if pageName == thisPageName and UserEquipManager:hasInited() then
			self:refreshPage(container);
		end
	end
end

-------------------------------------------------------------------------
local CommonPage = require("CommonPage");
EquipmentPage = CommonPage.newSub(EquipPageBase, thisPageName, option);

function MainFrame_onEquipmentPageBtn()
	PageManager.changePage(thisPageName);
	--UserEquipManager:cancelAllNotice();
	return 0;
end
