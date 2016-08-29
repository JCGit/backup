----------------------------------------------------------------------------------
--[[
	FILE:			FullAttributePage.lua
	ENCODING:		UTF-8, no-bomb
	DESCRIPTION:	
	AUTHOR:			hgs
	CREATED:		2014-01-21
--]]
----------------------------------------------------------------------------------
local Const_pb = require("Const_pb");
local UserInfo = require("UserInfo");
local thisPageName = "FullAttributePage"

local HelpConfg = {}
local option = {
	ccbiFile = "GeneralHelpPopUp1.ccbi",
	handlerMap = {
		onClose 		= "onClose"
	},
	opcode = opcodes
};

local FullAttributePageBase = {}

local NodeHelper = require("NodeHelper");
--------------------------------------------------------------
local function getBasicInfo()
	local level = UserInfo.roleInfo.level;
	local prof = common
	local tb = {
		common:fillHtmlStr("LvNameProf", level, UserInfo.roleInfo.name, UserInfo.getProfessionName()),
		common:fill(FreeTypeConfig[23].content, UserInfo.roleInfo.fight, level)
	};
	return table.concat(tb, "<br/>");
end

local function getBasicAttr()
	local basicAttrs = {
		Const_pb.STRENGHT,
		Const_pb.AGILITY,
		Const_pb.INTELLECT,
		Const_pb.STAMINA,
		Const_pb.HP,
		Const_pb.MP
	};
	local prof2MainAttr = {
		[Const_pb.WARRIOR] 	= Const_pb.STRENGHT,
		[Const_pb.HUNTER]	= Const_pb.AGILITY,
		[Const_pb.MAGIC]	= Const_pb.INTELLECT
	};
	local tb = {
		FreeTypeConfig[24].content,
	};
	local mainAttrId = prof2MainAttr[UserInfo.roleInfo.prof];
	
	local contentIdBase = 26;
	for i, attrId in ipairs(basicAttrs) do
		local contentId = i + contentIdBase;
		local val = UserInfo.getRoleAttrById(attrId);
		local str = FreeTypeConfig[contentId].content;
		local dmgStr = "";
		if attrId == mainAttrId then
			local minRatio, maxRatio = 0.5, 1.0;
			if attrId == Const_pb.INTELLECT then
				minRatio, maxRatio = 0.6, 1.2;
			end
			dmgStr = common:fill(FreeTypeConfig[42].content, math.floor(val * minRatio), math.floor(val * maxRatio));
		end
		if attrId == Const_pb.STRENGHT then
			str = common:fill(str, val, val, math.floor(0.6 * val), dmgStr);
		elseif attrId == Const_pb.AGILITY then
			str = common:fill(str, val, val, math.floor(0.2 * val), dmgStr);
		elseif attrId == Const_pb.INTELLECT then
			str = common:fill(str, val, val, math.floor(math.sqrt(val)), dmgStr);
		elseif attrId == Const_pb.STAMINA then
			str = common:fill(str, val, val * 10, val);
		else
			str = common:fill(str, val);
		end
		table.insert(tb, str);
	end
	return table.concat(tb, "<br/>");	
end

local function getFightAttr()
	local fightAttrs = {
		Const_pb.MINDMG,
		Const_pb.MAXDMG,
		Const_pb.ARMOR,
		Const_pb.PHYDEF,
		Const_pb.MAGDEF,
		Const_pb.CRITICAL,
		Const_pb.HIT,
		Const_pb.DODGE,
		Const_pb.RESILIENCE
	};

	
	local tb = {
		FreeTypeConfig[25].content
	};
	local contentIdBase = 32;
	for i, attrId in ipairs(fightAttrs) do
		local contentId = i + contentIdBase;
		local val = UserInfo.getRoleAttrById(attrId);
		local str = FreeTypeConfig[contentId].content;
		if attrId == Const_pb.ARMOR 
			or attrId == Const_pb.PHYDEF 
			or attrId == Const_pb.MAGDEF
		then
			str = common:fill(str, val, EquipManager:getBattleAttrEffect(attrId, val));
		elseif attrId == Const_pb.CRITICAL then
			local dmg = UserInfo.getRoleAttrById(Const_pb.BUFF_CRITICAL_DAMAGE);
			local dmgStr = string.format("%.1f", (180.0 + dmg * 0.01));
			str = common:fill(str, val, EquipManager:getBattleAttrEffect(attrId, val), dmgStr);
		else
			str = common:fill(str, val);
		end
		table.insert(tb, str);
	end
	return table.concat(tb, "<br/>");	
end

local function getGodlyAttr()
	local tb = {
		FreeTypeConfig[26].content
	};
	for i = 1, 2 do
		for _, attrId in ipairs(GameConfig['Part2GodlyAttr_' .. i]) do
			local name = common:getLanguageString("@AttrName_" .. attrId);
			local val = UserInfo.getRoleAttrById(attrId);
			if val > 0 then
				local valStr = EquipManager:getGodlyAttrString(attrId, val, "%.1f%%%%");
				local str = string.format("%s : %s", name, valStr);
				if EquipManager:isGodlyAttrPureNum(attrId) then
					local battleAttr = EquipManager:getBattleAttrEffect(attrId, val);
					local avoidStr = common:getLanguageString("@AttrAvoid_" .. attrId);
					table.insert(tb, common:fillHtmlStr("FullGodlyAttr_1", str, battleAttr, avoidStr));
				else
					table.insert(tb, common:fillHtmlStr("FullGodlyAttr", str));
				end
			end
		end
	end
	return table.concat(tb, "<br/>");
end
-----------------------------------------------
--FullAttributePageBase页面中的事件处理
----------------------------------------------
function FullAttributePageBase:onEnter(container)
	self:refreshPage(container);
end

function FullAttributePageBase:onExit(container)
	local mContent = container:getVarScrollView("mContent");
	if mContent then
		mContent:removeAllChildren();
	end
end
----------------------------------------------------------------

function FullAttributePageBase:refreshPage(container)
	NodeHelper:setStringForLabel(container, {mTitle = common:getLanguageString("@MoreAttrTitle")});
	
	local mContent = container:getVarScrollView("mContent");
	local width = mContent:getContentSize().width;
	
	local attrTb = {
		getBasicInfo(),
		getBasicAttr(),
		getFightAttr(),
		getGodlyAttr()
	};
	local content = table.concat(attrTb, "<br/><br/>");
	
	local lineWidth = GameConfig.LineWidth.MoreAttribute;
	local label = CCHTMLLabel:createWithString(content, CCSizeMake(lineWidth, 700));
	label:setPosition(ccp(0, 0));
	mContent:addChild(label);
	
	local height = label:getContentSize().height;
	mContent:setContentSize(CCSizeMake(width, height));
	mContent:setContentOffset(ccp(0, mContent:getViewSize().height - height * mContent:getScaleY()));
end

----------------click event------------------------
function FullAttributePageBase:onClose(container)
	PageManager.popPage(thisPageName);
end

-------------------------------------------------------------------------
local CommonPage = require("CommonPage");
FullAttributePage = CommonPage.newSub(FullAttributePageBase, thisPageName, option);