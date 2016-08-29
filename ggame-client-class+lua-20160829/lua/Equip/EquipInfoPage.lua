----------------------------------------------------------------------------------
--[[
	FILE:			EquipInfoPage.lua
	ENCODING:		UTF-8, no-bomb
	DESCRIPTION:	
	AUTHOR:			hgs
	CREATED:		2014-01-21
--]]
----------------------------------------------------------------------------------
local Const_pb 		= require("Const_pb");
local EquipOpr_pb 	= require("EquipOpr_pb");
local HP_pb			= require("HP_pb");
local UserInfo = require("UserInfo");
------------local variable for system api--------------------------------------
local tostring = tostring;
local tonumber = tonumber;
local string = string;
local pairs = pairs;
local table = table;
--------------------------------------------------------------------------------
local enhancePage = "EquipEnhancePage";
--registerScriptPage(enhancePage);
local baptizePage = "EquipBaptizePage";
--registerScriptPage(baptizePage);
local embedPage = "EquipEmbedPage";
--registerScriptPage(embedPage);
local swallowPage = "EquipSwallowPage";
--registerScriptPage(swallowPage);
local extendPage = "EquipExtendPage";
--registerScriptPage(extendPage);

local thisPageName 	= "EquipInfoPage";
local thisEquipId	= 0;
local thisRoleId = 0;
local isEquipDressed = false;
local equipPart = 0;
local isViewingOther = false;

local opcodes = {
	EQUIP_ENHANCE_C	= HP_pb.EQUIP_ENHANCE_C,
	EQUIP_ENHANCE_S	= HP_pb.EQUIP_ENHANCE_S,
	EQUIP_BAPTIZE_C	= HP_pb.EQUIP_BAPTIZE_C,
	EQUIP_BAPTIZE_S	= HP_pb.EQUIP_BAPTIZE_S,
	EQUIP_SWALLOW_C = HP_pb.EQUIP_SWALLOW_C,
	EQUIP_SWALLOW_S = HP_pb.EQUIP_SWALLOW_S,
	EQUIP_EXTEND_C	= HP_pb.EQUIP_EXTEND_C,
	EQUIP_EXTEND_S	= HP_pb.EQUIP_EXTEND_S,
	EQUIP_PUNCH_C	= HP_pb.EQUIP_PUNCH_C,
	EQUIP_PUNCH_S	= HP_pb.EQUIP_PUNCH_S,
	EQUIP_DRESS_S	= HP_pb.EQUIP_DRESS_S
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
	ccbiFile = "EquipmentInfoPopUp.ccbi",
	handlerMap = {
		onEnhance				= "onEnhance",
		onRefinement			= "onBaptize",	--洗炼
		onTakeOff				= "onTakeOff",
		onChange				= "onChange",
		onCameoIncrustation		= "onEmbed",
		onGobbleUp				= "onSwallow",
		onInherit				= "onExtend",
		onGodEquipmentFusion	= "onCompound",
		onClose					= "onClose"
	},
	opcode = opcodes
};

local EquipInfoPageBase = {};

local NodeHelper = require("NodeHelper");
local EquipOprHelper = require("EquipOprHelper");
local PBHelper = require("PBHelper");
local ItemManager = require("ItemManager");
-----------------------------------------------
--EquipInfoPageBase页面中的事件处理
----------------------------------------------
function EquipInfoPageBase:onEnter(container)
	if not isViewingOther then
		self:registerPacket(container);
		container:registerMessage(MSG_MAINFRAME_POPPAGE);
		isEquipDressed = UserEquipManager:isEquipDressed(thisEquipId);
	end
	self:refreshPage(container);
end

function EquipInfoPageBase:onExit(container)
	if not isViewingOther then
		self:removePacket(container);
		container:removeMessage(MSG_MAINFRAME_POPPAGE);
	end
end
----------------------------------------------------------------

function EquipInfoPageBase:refreshPage(container)
	self:showEquipInfo(container);
	self:showButtons(container);
end

--显示装备详情
function EquipInfoPageBase:showEquipInfo(container)
	local userEquip = nil;
	
	if not isViewingOther then
		userEquip = UserEquipManager:getUserEquipById(thisEquipId);
	else
		--查看别人阵容
		userEquip = ViewPlayerInfo:getEquipById(thisEquipId);
	end
		
	if userEquip == nil or userEquip.id == nil then
		return;
	end
	local equipId = userEquip.equipId;
	local level = EquipManager:getLevelById(equipId);
	local name	= EquipManager:getNameById(equipId);
	local quality = EquipManager:getQualityById(equipId);
	local lb2Str = {
		mLv 				= "Lv" .. level .. "\n" .. EquipManager:getPartNameById(equipId),	--等级、部位
		mLvNum				= "+" .. userEquip.strength,										--强化等级
		mEquipmentName		= "",																--等级名字，用HtmlLabel
		mEquipmentInfoTex	= ""																--装备信息，用HtmlLabel
	};
	local sprite2Img = {
		mPic = EquipManager:getIconById(equipId)
	};
	local itemImg2Qulity = {
		mHand = quality
	};
	local scaleMap = {mPic = 1.0};	
	
	--装备图标上的宝石孔显示（最多４个）
	local nodesVisible = {};
	local gemVisible = false;
	local aniVisible = UserEquipManager:isEquipGodly(userEquip);			
	local gemInfo = PBHelper:getGemInfo(userEquip.gemInfos);
	if table.maxn(gemInfo) > 0 then	--是否有孔
		gemVisible = true;
		for i = 1, 4 do
			local gemId = gemInfo[i];
			nodesVisible["mGemBG" .. i] = gemId ~= nil;
			local gemSprite = "mGem0" .. i;
			nodesVisible[gemSprite] = false;
			if gemId ~= nil and gemId > 0 then --是否有宝石
			local icon = ItemManager:getGemSmallIcon(gemId);
			if icon then
				nodesVisible[gemSprite] = true;
				sprite2Img[gemSprite] = icon;
				end
			end
		end
	end
	nodesVisible["mAni"]	= aniVisible;
	nodesVisible["mGemNode"]	= gemVisible;
	NodeHelper:setNodesVisible(container, nodesVisible);
	
	NodeHelper:setStringForLabel(container, lb2Str);
	NodeHelper:setSpriteImage(container, sprite2Img, scaleMap);
	NodeHelper:setQualityFrames(container, itemImg2Qulity);
	
	--装备名字显示，HtmlLabel
	local tag = GameConfig.Tag.HtmlLable;
	local nameStr = common:getLanguageString("@LevelName", level, name);
	nameStr = common:fillHtmlStr("Quality_" .. quality, nameStr);
	local nameNode = container:getVarNode("mEquipmentName");

	local _label = NodeHelper:addHtmlLable(nameNode, nameStr, tag);
	local stepLevel = EquipManager:getEquipStepById(equipId)	
	local starSprite = container:getVarSprite("mStar")
	local posX = _label:getContentSize().width + _label:getPositionX()
	if posX > 100 then 
		posX = posX + 12 
	end
	local posY = _label:getPositionY() - ( _label:getContentSize().height - starSprite:getContentSize().height )/2
	EquipManager:setStarPosition(starSprite, stepLevel == GameConfig.ShowStepStar, posX, posY)
		
	--装备属性信息显示（装备限制、评分、主属性、副属性、神器属性、神器强化效果、宝石加成），HtmlLabel
	local str = UserEquipManager:getDesciptionWithEquipInfo(userEquip, true, isViewingOther);
	local lbNode = container:getVarNode("mEquipmentInfoTex");
	local widthKey = isViewingOther and "EquipInfoFull" or "EquipInfo";
	local width = GameConfig.LineWidth[widthKey];
	local size = CCSizeMake(width, 200);
	local mScrollView = container:getVarScrollView("mContent");
	mScrollView:getContainer():removeAllChildren();
	mScrollView:setViewSize(CCSizeMake(width, mScrollView:getViewSize().height));
	local htmlNode = NodeHelper:addHtmlLable(lbNode, str, tag + 1, size, mScrollView);

	local height = 0;
	
	--宝石列表（有孔才显示）
	local listNode = self:showGemList(userEquip, htmlNode:getParent());
	if listNode then	
		listNode:setPosition(ccp(-5.0, 0));
		height = height + listNode:getContentSize().height;
	end
	htmlNode:setPosition(ccp(0, height));
		
	--属性信息及宝石列表在同一个ScrollView中，设置大小及偏移
	local size = htmlNode:getContentSize();
	height = height + size.height;
	mScrollView:setContentSize(CCSizeMake(size.width, height));
	mScrollView:setContentOffset(ccp(0, mScrollView:getViewSize().height - height * mScrollView:getScaleY()));
	
	--神器特效显示（如果是神器，添加特效）
	NodeHelper:addEquipAni(container, "mAni", aniVisible, nil, userEquip);
	
	equipPart = EquipManager:getPartById(equipId);
end

function EquipInfoPageBase:showGemList(userEquip, parentNode)
	local tag = GameConfig.Tag.GemList;
	parentNode:removeChildByTag(tag, true);
	local gemInfo = PBHelper:getGemInfo(userEquip.gemInfos);
	if table.maxn(gemInfo) > 0 then
		local listNode = ScriptContentBase:create("GemEquipmentItem.ccbi", tag);
		local nodesVisible = {};
		local sprite2Img = {};
		for i = 1, 4 do
			nodesVisible["mGemNode" .. i] = false;
		end
		local j = 1;
		for _, gemId in pairs(gemInfo) do
			nodesVisible["mGemNode" .. j] = true;
			sprite2Img["mGemPic" .. j] = GameConfig.Image.Empty;
			if gemId > 0 then
				local icon = ItemManager:getIconById(gemId);
				if icon ~= "" then
					sprite2Img["mGemPic" .. j] = icon;
				end
			end
			j = j + 1;
		end
		NodeHelper:setNodesVisible(listNode, nodesVisible);
		NodeHelper:setSpriteImage(listNode, sprite2Img);
		listNode:setAnchorPoint(ccp(0, 0));
		parentNode:addChild(listNode, tag);
		listNode:release();
		return listNode;
	end
	return nil;
end

function EquipInfoPageBase:showButtonContainer(container, isVisible)
	local node2Visible = {
		mLeftBtnNode	= isVisible,
		mRightBtnNode	= isVisible
	};
	NodeHelper:setNodesVisible(container, node2Visible);
end

function EquipInfoPageBase:showButtons(container)
	self:showButtonContainer(container, not isViewingOther);
	if isViewingOther then return; end
	
	local userEquip = UserEquipManager:getUserEquipById(thisEquipId);
	local isGodly = UserEquipManager:isGodly(thisEquipId);
	local canBeEmbed = EquipManager:canBeEmbed(userEquip.equipId);
	local canBeBaptized = EquipManager:canBeBaptized(userEquip.equipId);
	local canCompound = UserEquipManager:canCompound(thisEquipId);
	local btnVisible = {
		mChangeNode					= isEquipDressed,
		mCameoIncrustationNode 		= canBeEmbed,
		mGodEquipmentGobbleUpNode 	= isGodly,
		mGodEquipmentInheritNode 	= isGodly,
		mRefinementNode				= canBeBaptized,
		mGodEquipmentFusionNode		= canCompound
	};
	local lb2Str = {
		mUnloadTex	= common:getLanguageString(isEquipDressed and "@TakeOff" or "@SellOut")
	};
	
	NodeHelper:setNodesVisible(container, btnVisible);
	NodeHelper:setStringForLabel(container, lb2Str);
end
	
----------------click event------------------------
function EquipInfoPageBase:onEnhance(container)
	RegisterLuaPage(enhancePage);
	EquipEnhancePage_setEquipId(thisEquipId);
	--self:onClose();
	PageManager.pushPage(enhancePage);
end

--洗炼
function EquipInfoPageBase:onBaptize(container)
	RegisterLuaPage(baptizePage);
	EquipBaptizePage_setEquipId(thisEquipId);
	--self:onClose();
	PageManager.pushPage(baptizePage);
end	

--卸下/出售
function EquipInfoPageBase:onTakeOff(container)
	if isEquipDressed then
		local dressType = GameConfig.DressEquipType.Off;
		EquipOprHelper:dressEquip(thisEquipId, thisRoleId, dressType);
		--self:onClose();
	else
		if UserEquipManager:isGodly(thisEquipId) then
			MessageBoxPage:Msg_Box_Lan("@CanNotSellGodly");
		elseif UserEquipManager:hasGem(thisEquipId) then
			MessageBoxPage:Msg_Box_Lan("@SelectedEquipHasGem");
		else
			EquipOprHelper:sellEquip(thisEquipId);
			self:onClose();
		end
	end
end

--更换
function EquipInfoPageBase:onChange(container)
	if isEquipDressed then
		EquipSelectPage_setPart(equipPart, thisRoleId);
		PageManager.pushPage("EquipSelectPage");
	end
	--self:onClose();
end

--镶嵌
function EquipInfoPageBase:onEmbed(container)
	RegisterLuaPage(embedPage);
	EquipEmbedPage_setEquipId(thisEquipId);
	--self:onClose();
	PageManager.pushPage(embedPage);
end

--吞噬
function EquipInfoPageBase:onSwallow(container)
	if not (UserEquipManager:canSwallow(thisEquipId)) then
		MessageBoxPage:Msg_Box_Lan("@EquipmentStarLevelHighest");
		return;
	end
	RegisterLuaPage(swallowPage);
	EquipSwallowPage_setEquipId(thisEquipId);
	--self:onClose();
	PageManager.pushPage(swallowPage);
end

--传承
function EquipInfoPageBase:onExtend(container)
	RegisterLuaPage(extendPage);
	EquipExtendPage_setEquipId(thisEquipId);
	--self:onClose();
	PageManager.pushPage(extendPage);
end

--神器融合
function EquipInfoPageBase:onCompound(container)
	local compoundPage = "EquipCompoundPage";
	RegisterLuaPage(compoundPage);
	EquipCompoundPage_setEquipId(thisEquipId);
	PageManager.pushPage(compoundPage);
end

function EquipInfoPageBase:onClose(container)
	PageManager.popPage(thisPageName);
end

--回包处理
function EquipInfoPageBase:onReceivePacket(container)
	local opcode = container:getRecPacketOpcode()
	local msgBuff = container:getRecPacketBuffer()

	if opcode == opcodes.EQUIP_ENHANCE_S then
		self:refreshPage(container);
		return;
	end
	
	if opcode == opcodes.EQUIP_DRESS_S then
		--self:refreshPage(container);
		self:onClose();
		return;
	end
end

function EquipInfoPageBase:onReceiveMessage(container)
	local message = container:getMessage();
	local typeId = message:getTypeId();
	if typeId == MSG_MAINFRAME_POPPAGE then
		local pageName = MsgMainFramePopPage:getTrueType(message).pageName;
		if pageName ~= thisPageName then
			self:refreshPage(container);
		end	
	end
end

function EquipInfoPageBase:registerPacket(container)
	for key, opcode in pairs(opcodes) do
		if string.sub(key, -1) == "S" then
			container:registerPacket(opcode)
		end
	end
end

function EquipInfoPageBase:removePacket(container)
	for key, opcode in pairs(opcodes) do
		if string.sub(key, -1) == "S" then
			container:removePacket(opcode)
		end
	end
end
-------------------------------------------------------------------------
local CommonPage = require("CommonPage");
EquipInfoPage = CommonPage.newSub(EquipInfoPageBase, thisPageName, option);

function EquipInfoPage_setEquipId(equipId, roleId)
	thisEquipId = equipId;
	thisRoleId = roleId or UserInfo.roleInfo.roleId;
	isViewingOther = false;
end

function EquipInfoPage_viewEquipId(equipId)
	thisEquipId = equipId;
	isViewingOther = true;
end
