----------------------------------------------------------------------------------
--[[
	FILE:			GodlyEquipBuildPage.lua
	ENCODING:		UTF-8, no-bomb
	DESCRIPTION:	神器打造页面
	AUTHOR:			hgs
	CREATED:		2014-09-05
--]]
----------------------------------------------------------------------------------
------------local variable for system api--------------------------------------
local ceil = math.ceil;
--------------------------------------------------------------------------------
local HP_pb = require("HP_pb");
local UserInfo = require("UserInfo");
local thisPageName = "GodlyEquipBuildPage"
local NodeHelper = require("NodeHelper");
local opcodes = {
	EQUIP_SPECIAL_CREATE_S = HP_pb.EQUIP_SPECIAL_CREATE_S
}

local option = {
	ccbiFile = "RefiningCreateGodPopUp.ccbi",
	handlerMap = {
		onGodEquipmentFusion	= "onCompound",
		onClose					= "onClose",
		onHelp					= "onHelp"
	},
	opcode = opcodes
};

local GodlyEquipBuildPageBase = {}

local EquipOprHelper = require("EquipOprHelper");

local PageInfo = {
	optionEquips = {},
	userLv = 0
};
local thisScrollView = nil;
local thisScrollViewOffset = nil;

--------------------------------------------------------------
local EquipItem = {
	ccbiFile = "RefiningCreateGodContent.ccbi",
	initTexHeight = nil,
	initSize = {
		container = nil
	},
	top = nil
};

function EquipItem.onFunction(eventName, container)
	if eventName == "luaRefreshItemView" then
		EquipItem.onRefreshItemView(container);
	elseif eventName == "onCreate" then
		EquipItem.onBuild(container);
	end
end

function EquipItem.dressEquip(container)
	local contentId = container:getTag();
	local userEquipId = PageInfo.optionEquips[contentId];
	EquipOprHelper:dressEquip(userEquipId, PageInfo.roleId, PageInfo.dressType);
end	

function EquipItem.onBuild(container)
	local contentId = container:getItemDate().mID;
	local equipCfg = PageInfo.optionEquips[contentId];
	
	local needReputation, needSmeltValue = tonumber(equipCfg["reputation"]), equipCfg["smeltValue"];
	local title = common:getLanguageString("@SpecialBuildEquip_Title");
	local msg = common:getLanguageString("@SpecialBuildEquip_Msg", needReputation, needSmeltValue);
	PageManager.showConfirm(title, msg, function(isSure)
		if isSure then
			if needReputation > UserInfo.playerInfo.reputationValue then
				MessageBoxPage:Msg_Box_Lan("@ReputationNotEnough");
				return;
			elseif needSmeltValue > UserInfo.playerInfo.smeltValue then
				MessageBoxPage:Msg_Box_Lan("@SmeltValueNotEnough");
				return;
			end
			EquipOprHelper:buildGodlyEquip(equipCfg["id"]);
		end
	end);
end	

function EquipItem.onRefreshItemView(container)
	local contentId = container:getItemDate().mID;
	local equipCfg = PageInfo.optionEquips[contentId];
	local equipId = equipCfg["equipId"];
	
	local level = EquipManager:getLevelById(equipId);
	local name	= EquipManager:getNameById(equipId);
	local quality = EquipManager:getQualityById(equipId);

	local lb2Str = {
		mEquipmentName		= common:getLanguageString("@LevelName", level, name),
		mAttribute			= EquipManager:getInitAttr(equipId),
		mGodAttribute		= EquipManager:getGodlyAttr(equipId, equipCfg["attrCount"], "\n"),
		--mPrestigeUnlockNum	= common:getLanguageString("@OpenWithReputation", equipCfg["reputation"]),
		mEquipmentPosition	= EquipManager:getPartNameById(equipId)
	};
	local sprite2Img = {
		mPic = EquipManager:getIconById(equipId)
	};
	local itemImg2Qulity = {
		mHand = quality
	};
	local scaleMap = {mPic = 1.0};
	
	NodeHelper:setStringForLabel(container, lb2Str);
	NodeHelper:setSpriteImage(container, sprite2Img, scaleMap);
	NodeHelper:setQualityFrames(container, itemImg2Qulity);	
	
	local aniNode = container:getVarNode("mEquipmentAni");
	if aniNode then
		aniNode:removeAllChildren();
		local ccbiFile = EquipManager:getGodlyAni(equipCfg["attrCount"]);
		local ani = CCBManager:getInstance():createAndLoad2(ccbiFile);
		ani:unregisterFunctionHandler();
		aniNode:addChild(ani);
	end
end	
----------------------------------------------------------------------------------
	
-----------------------------------------------
--GodlyEquipBuildPageBase页面中的事件处理
----------------------------------------------
function GodlyEquipBuildPageBase:onEnter(container)
	NodeHelper:setStringForLabel(container, {
		mMeltingNum 	= common:getLanguageString("@BuildGodlyCost"),
		mSmeltingNum	= common:getLanguageString("@SmeltValueTip")
	});
	self:registerPacket(container)
	self:setOptionEquips();
	
	self:initScrollview(container);
	
	self:refreshPage(container);
	self:rebuildAllItem(container);
	thisScrollView = container.mScrollView;
end

function GodlyEquipBuildPageBase:onExit(container)
	self:removePacket(container)
	NodeHelper:deleteScrollView(container);
	thisScrollView = nil;
	thisScrollViewOffset = nil;
end
----------------------------------------------------------------

function GodlyEquipBuildPageBase:setOptionEquips()
	local userLevel = UserInfo.roleInfo.level;
	if PageInfo.userLv ~= userLevel or common:table_isEmpty(PageInfo.optionEquips) then
		PageInfo.optionEquips = {};
		for _, cfg in ipairs(ConfigManager.getGodlyEquipCanBuild() or {}) do
			if cfg["minLv"] <= userLevel and cfg["maxLv"] >= userLevel then
				table.insert(PageInfo.optionEquips, cfg);
			end
		end
		PageInfo.userLv = userLevel;
	end
end

function GodlyEquipBuildPageBase:refreshPage(container)
	self:showMyRP(container);
end	

function GodlyEquipBuildPageBase:showMyRP(container)
	local rpStr = common:getLanguageString("@MyReputation", UserInfo.playerInfo.reputationValue);
	local smeltStr = common:getLanguageString('@MySmelting', UserInfo.playerInfo.smeltValue);
	NodeHelper:setStringForLabel(container, {
		mMyReputationNum 	= rpStr,
		mMySmeltingNum		= smeltStr
	});
end
----------------scrollview-------------------------
function GodlyEquipBuildPageBase:initScrollview(container)
	NodeHelper:initScrollView(container, "mContent", 4);
end

function GodlyEquipBuildPageBase:rebuildAllItem(container)
	self:clearAllItem(container);
	if #PageInfo.optionEquips > 0 then
		self:buildItem(container);
	end
end

function GodlyEquipBuildPageBase:clearAllItem(container)
	NodeHelper:clearScrollView(container);
end

function GodlyEquipBuildPageBase:buildItem(container)
	local size = #PageInfo.optionEquips;
	NodeHelper:buildScrollView(container, size, EquipItem.ccbiFile, EquipItem.onFunction);
	if thisScrollView and thisScrollViewOffset then
		thisScrollView:setContentOffset(thisScrollViewOffset);
	end
end
	
----------------click event------------------------
--神器融合
function GodlyEquipBuildPageBase:onCompound(container)
	local compoundPage = "EquipCompoundPage";
	PageManager.pushPage(compoundPage);
end

function GodlyEquipBuildPageBase:onClose(container)
	PageManager.popPage(thisPageName);
end

function GodlyEquipBuildPageBase:onHelp(container)
	PageManager.showHelp(GameConfig.HelpKey.HELP_FORGEGODLY);
end

--回包处理
function GodlyEquipBuildPageBase:onReceivePacket(container)
	local opcode = container:getRecPacketOpcode()

	if opcode == opcodes.EQUIP_SPECIAL_CREATE_S then
		self:refreshPage(container);
		PageManager.refreshPage("ArenaPage");
		PageManager.refreshPage("MeltPage");
		return
	end
end

function GodlyEquipBuildPageBase:registerPacket(container)
	for key, opcode in pairs(opcodes) do
		if string.sub(key, -1) == "S" then
			container:registerPacket(opcode)
		end
	end
end

function GodlyEquipBuildPageBase:removePacket(container)
	for key, opcode in pairs(opcodes) do
		if string.sub(key, -1) == "S" then
			container:removePacket(opcode)
		end
	end
end
-------------------------------------------------------------------------
local CommonPage = require("CommonPage");
GodlyEquipBuildPage = CommonPage.newSub(GodlyEquipBuildPageBase, thisPageName, option);	