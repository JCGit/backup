----------------------------------------------------------------------------------
--[[
	FILE:			EquipSelectPage.lua
	ENCODING:		UTF-8, no-bomb
	DESCRIPTION:	
	AUTHOR:			hgs
	CREATED:		2014-01-21
--]]
----------------------------------------------------------------------------------
EquipFilterType = {
	Dress 		= 1,	--上装
	Melt 		= 2,	--熔炼
	Swallow		= 3,	--吞噬
	Extend		= 4,	--传承
	CompoundDST = 5,	--融合
	CompoundSRC	= 5		--SCR, DST 的逻辑改为一样了，所以值一样为5
};
------------local variable for system api--------------------------------------
local ceil = math.ceil;
--------------------------------------------------------------------------------
local HP_pb = require("HP_pb");
local UserInfo = require("UserInfo");
local thisPageName = "EquipSelectPage"
local NodeHelper = require("NodeHelper")

local opcodes = {
	EQUIP_DRESS_S = HP_pb.EQUIP_DRESS_S
}

local option = {
	ccbiFile = "EquipmentSelectPopUp.ccbi",
	handlerMap = {
		onClose				= "onClose",
		onConfirmation		= "onConfirm"
	},
	opcode = opcodes
};

local EquipSelectPageBase = {}

local RoleManager = require("RoleManager");
local ItemManager = require("ItemManager");
local EquipOprHelper = require("EquipOprHelper");
local PBHelper = require("PBHelper");
local UserMercenaryManager = require("UserMercenaryManager");

local SelectType = {
	Single 	= 1,		--单选
	Multi	= 2			--多选
};	
local SortType = {
	Score_ASC 	= 1,
	Score_DESC 	= 2
};
local PageInfo = {
	roleId = UserInfo.roleInfo.roleId,
	targetPart = 1,
	currentEquipId = nil,
	selectedEquipId = nil,
	optionIds = {},
	dressType = 1,
	selectedIds = {},
	selectType = SelectType.Single,
	filterType = EquipFilterType.Dress,
	sortType = SortType.Score_DESC,
	limit = 1,
	isFull = false,
	callback = nil
};
local thisScrollView = nil;
local thisScrollViewOffset = nil;
local thisContainer

function sortEquipByScore(id_1, id_2)
	local isAsc = PageInfo.sortType == SortType.Score_ASC;
	
	if id_2 == nil then
		return isAsc;
	end		
	if id_1 == nil then
		return not isAsc;
	end
	
	local userEquip_1 = UserEquipManager:getUserEquipById(id_1);
	local userEquip_2 = UserEquipManager:getUserEquipById(id_2);
	
	if not EquipManager:isDressableWithLevel(userEquip_1.equipId) and EquipManager:isDressableWithLevel(userEquip_2.equipId) then
	    return not isAsc;
	elseif EquipManager:isDressableWithLevel(userEquip_1.equipId) and not EquipManager:isDressableWithLevel(userEquip_2.equipId) then
	    return isAsc;
	end
	
	
	if userEquip_1.score ~= userEquip_2.score then
		if userEquip_1.score > userEquip_2.score then
			return isAsc;
		end
		return not isAsc;
	end
	
	if id_1 > id_2 then
		return isAsc;
	end
	
	return not isAsc;
end

function sortEquipByQualityScore(id_1, id_2)
	local isAsc = PageInfo.sortType == SortType.Score_ASC;
	
	if id_2 == nil then
		return isAsc;
	end		
	if id_1 == nil then
		return not isAsc;
	end
	
	local userEquip_1 = UserEquipManager:getUserEquipById(id_1);
	local userEquip_2 = UserEquipManager:getUserEquipById(id_2);
	
	local quality_1 = EquipManager:getQualityById(userEquip_1.equipId);
	local quality_2 = EquipManager:getQualityById(userEquip_2.equipId);
	
	if quality_1 ~= quality_2 then
		if quality_1 < quality_2 then
			return isAsc;
		else
			return not isAsc;
		end
	end
	
	if userEquip_1.score ~= userEquip_2.score then
		if userEquip_1.score > userEquip_2.score then
			return isAsc;
		end
		return not isAsc;
	end
	
	if id_1 > id_2 then
		return isAsc;
	end
	
	return not isAsc;
end
--------------------------------------------------------------
local EquipItem = {
	ccbiFile = "EquipmentSelectContent.ccbi",
	initTexHeight = nil,
	initSize = {
		container = nil
	},
	top = nil
};

function EquipItem.onFunction(eventName, container)
	if eventName == "luaInitItemView" then
		EquipItem.onRefreshItemView(container);
	elseif eventName == "onEquipment" then
		EquipItem.dressEquip(container);
		PageManager.popPage(thisPageName);
	elseif eventName == "onChocice" then
		EquipItem.onSelect(container);
	end
end

function EquipItem.dressEquip(container)
	local contentId = container:getTag();
	local userEquipId = PageInfo.optionIds[contentId];
	EquipOprHelper:dressEquip(userEquipId, PageInfo.roleId, PageInfo.dressType);
end	

function EquipItem.onSelect(container)
	local contentId = container:getTag();
	local userEquipId = PageInfo.optionIds[contentId];
	local isSelected = common:table_hasValue(PageInfo.selectedIds, userEquipId);
	local needRebuildAll = false;
	if isSelected then
		PageInfo.selectedIds = common:table_removeFromArray(PageInfo.selectedIds, userEquipId);
		if PageInfo.isFull then
			needRebuildAll = true;
			PageInfo.isFull = false;
		end
	else
		table.insert(PageInfo.selectedIds, userEquipId);
		if #PageInfo.selectedIds >= PageInfo.limit then
			needRebuildAll = true;
			PageInfo.isFull = true;
		end
	end
	thisScrollViewOffset = thisScrollView:getContentOffset();
	if needRebuildAll then
		--PageManager.refreshPage(thisPageName);
		EquipSelectPageBase:refreshSelectedBox(thisContainer)
	else
		NodeHelper:setMenuItemSelected(container, {mChocice = not isSelected});
	end
end	

function EquipItem.refreshSelectBox(container)
    local btnVisible = {}
    local contentId = container:getTag();
	local userEquipId = PageInfo.optionIds[contentId];
	local userEquip = UserEquipManager:getUserEquipById(userEquipId);
    
    local isSelected = common:table_hasValue(PageInfo.selectedIds, userEquipId);
    NodeHelper:setMenuItemSelected(container, {mChocice = isSelected});
    btnVisible["mChociceNode"] = isSelected or not PageInfo.isFull;
    
    NodeHelper:setNodesVisible(container, btnVisible);
end


function EquipItem.onRefreshItemView(container)
	local contentId = container:getTag();
	local userEquipId = PageInfo.optionIds[contentId];
	local userEquip = UserEquipManager:getUserEquipById(userEquipId);
	local equipId = userEquip.equipId;
	
	if equipId == nil then return end
	
	local texLable = container:getVarLabelBMFont("mEquipmentTex");
	if EquipItem.initTexHeight == nil then
		EquipItem.initTexHeight = texLable:getContentSize().height;
	end
	if EquipItem.initSize.container == nil then
		EquipItem.initSize.container = container:getContentSize();
	end
	
	local level = EquipManager:getLevelById(equipId);
	local name	= EquipManager:getNameById(equipId);
	local quality = EquipManager:getQualityById(equipId);
	local roleName = "";
	if PageInfo.filterType == EquipFilterType.Extend then
		roleName = UserEquipManager:getDressedRoleName(userEquipId);
	end
	local lb2Str = {
		mLv 				= "Lv" .. level,
		mName				= roleName,
		mLvNum				= "+" .. userEquip.strength,
		mEquipmentName		= "", --common:getLanguageString("@LevelName", level, name),
		mEquipmentTex		= "", --UserEquipManager:getEquipInfoString(userEquipId),
		mEquipmentPosition	= EquipManager:getPartNameById(equipId);
	};
	local sprite2Img = {
		mPic = EquipManager:getIconById(equipId)
	};
	local itemImg2Qulity = {
		mHand = quality
	};
	local scaleMap = {mPic = 1.0};
	
	local nodesVisible = {};
	local gemVisible = false;
	local aniVisible = UserEquipManager:isEquipGodly(userEquip);			
	local gemInfo = PBHelper:getGemInfo(userEquip.gemInfos);
	if table.maxn(gemInfo) > 0 then
		gemVisible = true;
		for i = 1, 4 do
			local gemId = gemInfo[i];
			nodesVisible["mGemBG" .. i] = gemId ~= nil;
			local gemSprite = "mGem0" .. i;
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
	nodesVisible["mAni"]	= false;--aniVisible;
	nodesVisible["mGemNode"]	= gemVisible;
	NodeHelper:setNodesVisible(container, nodesVisible);
	
	NodeHelper:setStringForLabel(container, lb2Str);
	NodeHelper:setSpriteImage(container, sprite2Img, scaleMap);
	NodeHelper:setQualityFrames(container, itemImg2Qulity);	
	
	local tag = GameConfig.Tag.HtmlLable;
	local nameStr = common:getLanguageString("@LevelName", level, name);
	nameStr = common:fillHtmlStr("Quality_" .. quality, nameStr);
	local nameNode = container:getVarNode("mEquipmentName");

	local _label = NodeHelper:addHtmlLable(nameNode, nameStr, tag);
	local stepLevel = EquipManager:getEquipStepById(equipId)	
	local starSprite = container:getVarSprite("mStar")
	local posX = _label:getContentSize().width + _label:getPositionX()-20
	local posY = _label:getPositionY() - ( _label:getContentSize().height - starSprite:getContentSize().height )/2
	EquipManager:setStarPosition(starSprite, stepLevel == GameConfig.ShowStepStar, posX, posY)

	local str = UserEquipManager:getEquipInfoString(userEquipId);
	local lbNode = container:getVarNode("mEquipmentTex");
	local lable = NodeHelper:addHtmlLable(lbNode, str, tag + 1);
	
	local isMultiSelect = PageInfo.selectType == SelectType.Multi;
	local isSmelt = PageInfo.filterType == EquipFilterType.Melt;
	local promptStr = "";
	local btnVisible = {
		mEquipmentNode		= not isMultiSelect,
		mChociceNode		= isMultiSelect,
		mRefningPromptNode	= false
	};

	if isMultiSelect then
		local isSelected = common:table_hasValue(PageInfo.selectedIds, userEquipId);
		NodeHelper:setMenuItemSelected(container, {mChocice = isSelected});
		btnVisible["mChociceNode"] = isSelected or not PageInfo.isFull;
	else
		local btnEnabled = true;
		if PageInfo.filterType == EquipFilterType.Dress then
			if not EquipManager:isDressableWithLevel(userEquip.equipId) then
				btnEnabled = false;
				promptStr = common:getLanguageString("@HighLevelToEquip");
			end
		end
		NodeHelper:setMenuItemEnabled(container, "mEquipmentBtn", btnEnabled);
	end
	
	if isSmelt then
		local gain = EquipManager:getSmeltGainById(equipId) or 0;
		promptStr = common:getLanguageString("@SmeltGain", gain);
	end
	

	if promptStr ~= "" then
		NodeHelper:setStringForLabel(container, {RefningPromptTex = promptStr});
		btnVisible["mRefningPromptNode"] = true;
	end
	NodeHelper:setNodesVisible(container, btnVisible);
	
	local texHeight = lable:getContentSize().height;
	local offset = texHeight - EquipItem.initTexHeight;
	if offset > 0 then
		local margin = GameConfig.Margin.EquipSelectBottom;
		texHeight = texHeight + margin;
		lable:setContentSize(CCSizeMake(lable:getContentSize().width, texHeight));
		lable:setPositionY(lable:getPositionY() + margin * lable:getScaleY());
		offset = offset + margin;
	end
	offset = offset * lable:getScaleY();
	for i = 1, 3 do
		local nodeName = "mScale9Sprite" .. i;
		local node = container:getVarNode(nodeName);
		if EquipItem.initSize[nodeName] == nil then
			EquipItem.initSize[nodeName] = node:getContentSize();
		end
		local size = EquipItem.initSize[nodeName];
		if offset > 0 then
			node:setContentSize(CCSizeMake(size.width, size.height + offset));
		else
			node:setContentSize(size);
		end
	end
	local size = EquipItem.initSize.container;
	if offset > 0 then
		container:setContentSize(CCSizeMake(size.width, size.height + offset));
	else
		container:setContentSize(size);
	end
	local node2 = container:getVarNode("mContentNode");
	if EquipItem.top == nil then
		EquipItem.top = node2:getPositionY();
	end
	local posY = EquipItem.top;
	if offset > 0 then
		posY = posY + offset;
	end
	node2:setPositionY(posY);
	
	NodeHelper:addEquipAni(container, "mEquipmentAni", aniVisible, userEquipId);
end	
----------------------------------------------------------------------------------
	
-----------------------------------------------
--EquipSelectPageBase页面中的事件处理
----------------------------------------------
function EquipSelectPageBase:onEnter(container)
	self:registerPacket(container)
	--container:registerMessage(MSG_SEVERINFO_UPDATE);
	--container:registerMessage(MSG_MAINFRAME_POPPAGE);
	container:registerMessage(MSG_MAINFRAME_REFRESH);
	self:setCurrentEquipId();
	self:setOptionIds();
	
	self:initScrollview(container);
	
	self:refreshPage(container);
	self:rebuildAllItem(container);
	thisScrollView = container.mScrollView;
	thisContainer = container
end

function EquipSelectPageBase:onExit(container)
	self:removePacket(container)
	--container:removeMessage(MSG_SEVERINFO_UPDATE);
	--container:removeMessage(MSG_MAINFRAME_POPPAGE);
	container:removeMessage(MSG_MAINFRAME_REFRESH);
	NodeHelper:deleteScrollView(container);
	thisScrollView = nil;
	thisScrollViewOffset = nil;
end
----------------------------------------------------------------
function EquipSelectPageBase:setCurrentEquipId()
	if PageInfo.selectType == SelectType.Multi then
		return;
	end
	
	local roleEquip = nil;
	if PageInfo.roleId == UserInfo.roleInfo.roleId then
		roleEquip = UserInfo.getEquipByPart(PageInfo.targetPart);
	else
		roleEquip = UserMercenaryManager:getEquipByPart(PageInfo.roleId, PageInfo.targetPart);
	end
	
	if roleEquip then
		PageInfo.currentEquipId = roleEquip.equipId;
		PageInfo.dressType = GameConfig.DressEquipType.Change;
	else 
		PageInfo.currentEquipId = nil;
		PageInfo.dressType = GameConfig.DressEquipType.On;
	end
end

function EquipSelectPageBase:setOptionIds()
	--reset
	PageInfo.optionIds = {};
	
	local filterType = PageInfo.filterType;
	if filterType == EquipFilterType.Swallow then
		PageInfo.optionIds = UserEquipManager:getEquipIdsForSwallow(PageInfo.currentEquipId);
		return;
	elseif filterType == EquipFilterType.Melt then
		PageInfo.optionIds = UserEquipManager:getEquipIdsForSmelt();
		return;
	end
	
	local ids = {};
	
	--classify
	if filterType == EquipFilterType.Dress 
		or filterType == EquipFilterType.Extend 
		or (filterType == EquipFilterType.CompoundDST and PageInfo.targetPart)
	then
		ids = UserEquipManager:getEquipIdsByClass("Part", PageInfo.targetPart);
		local compoundPos = 0;
		local isCompounding = filterType == EquipFilterType.CompoundDST;
		if isCompounding then
			local _, pos = UserEquipManager:canCompound(PageInfo.currentEquipId);
			compoundPos = pos;
		end
		if filterType == EquipFilterType.Extend or filterType == EquipFilterType.CompoundDST then
			for _, id in ipairs(ids) do
				if not UserEquipManager:isGodly(id) then
					if not isCompounding and id ~= PageInfo.currentEquipId then
						table.insert(PageInfo.optionIds, id);
					end
				elseif isCompounding and id ~= PageInfo.currentEquipId then
					local canCompound, pos = UserEquipManager:canCompound(id);
					if canCompound and pos ~= compoundPos then
						table.insert(PageInfo.optionIds, id);
					end
				end
			end
			for _, id in ipairs(UserEquipManager:getEquipIdsByClass("Dress")) do
				local equipInfo = UserEquipManager:getUserEquipById(id);
				if id ~= PageInfo.currentEquipId and EquipManager:getPartById(equipInfo.equipId) == PageInfo.targetPart then
					if not UserEquipManager:isGodly(id) then
						if not isCompounding then
							table.insert(PageInfo.optionIds, id);
						end
					elseif isCompounding then
						local canCompound, pos = UserEquipManager:canCompound(id);
						if canCompound and pos ~= compoundPos then
							table.insert(PageInfo.optionIds, id);
						end
					end
				end
			end
			table.sort(PageInfo.optionIds, sortEquipByScore);
			return;
		end
	elseif filterType == EquipFilterType.CompoundSRC then
		for _, id in ipairs(UserEquipManager:getEquipIdsByClass("Godly")) do
			if (UserEquipManager:canCompound(id)) then
				table.insert(PageInfo.optionIds, id);
			end
		end
		for _, id in ipairs(UserEquipManager:getEquipIdsByClass("Dress")) do
			if (UserEquipManager:canCompound(id)) then
				table.insert(PageInfo.optionIds, id);
			end
		end
		table.sort(PageInfo.optionIds, sortEquipByScore);
		return;
	else
		ids = UserEquipManager:getEquipIdsByClass("All");
	end
	
	--filter
	local roleProf = nil;
	if filterType == EquipFilterType.Dress then
		if PageInfo.roleId == UserInfo.roleInfo.roleId then
			roleProf = UserInfo.roleInfo.prof;
		else
			roleProf = UserMercenaryManager:getProfessioinIdByPart(PageInfo.roleId, PageInfo.targetPart);
		end
	end
	for _, id in ipairs(ids) do
		local isOk = false;
		local isSame = PageInfo.currentEquipId ~= nil and PageInfo.currentEquipId == id;
		if not isSame and not UserEquipManager:isEquipDressed(id) then
			if filterType == EquipFilterType.Dress then
				local userEquip = UserEquipManager:getUserEquipById(id);
				if EquipManager:isDressableWithProfession(userEquip.equipId, roleProf) then
					isOk = true;
				end
			else
				isOk = true;
			end
		end
		--[[
		if filterType == EquipFilterType.Melt then
			if UserEquipManager:isGodly(id) or UserEquipManager:hasGem(id) then
				isOk = false;
			end
		end
		--]]
		
		if isOk then
			table.insert(PageInfo.optionIds, id);
		end
	end
	
	--sort
	if filterType == EquipFilterType.Melt then
		table.sort(PageInfo.optionIds, sortEquipByQualityScore);
	else
		table.sort(PageInfo.optionIds, sortEquipByScore);
	end
end

function EquipSelectPageBase:showCurrentEquip(container)
	local userEquipId = PageInfo.currentEquipId;
	local userEquip = UserEquipManager:getUserEquipById(userEquipId);
	local equipId = userEquip.equipId;
	
	local texLable = container:getVarLabelBMFont("mEquipmentTex");
	local containerNode = container:getVarScrollView("mEquipmentContent");
	if EquipItem.initTexHeight == nil then
		EquipItem.initTexHeight = texLable:getContentSize().height;
	end
	if EquipItem.initSize.containerNow == nil then
		EquipItem.initSize.containerNow = containerNode:getViewSize();
	end
	
	local level = EquipManager:getLevelById(equipId);
	local name	= EquipManager:getNameById(equipId);
	local quality = EquipManager:getQualityById(equipId);
	local roleName = "";
	if PageInfo.filterType == EquipFilterType.Extend then
		roleName = UserEquipManager:getDressedRoleName(userEquipId);
	end
	local lb2Str = {
		mLv 				= "Lv" .. level,
		mName				= roleName,
		mLvNum				= "+" .. userEquip.strength,
		mEquipmentName		= "", --common:getLanguageString("@LevelName", level, name),
		mEquipmentTex		= "", --UserEquipManager:getEquipInfoString(userEquipId),
		mEquipmentPosition	= EquipManager:getPartNameById(equipId)
	};
	local sprite2Img = {
		mPic = EquipManager:getIconById(equipId)
	};
	local itemImg2Qulity = {
		mHand = quality
	};
	local scaleMap = {mPic = 1.0};
	
	local nodesVisible = {};
	local gemVisible = false;
	local aniVisible = UserEquipManager:isEquipGodly(userEquip);			
	local gemInfo = PBHelper:getGemInfo(userEquip.gemInfos);
	if table.maxn(gemInfo) > 0 then
		gemVisible = true;
		for i = 1, 4 do
			local gemId = gemInfo[i];
			nodesVisible["mGemBG" .. i] = gemId ~= nil;
			local gemSprite = "mGem0" .. i;
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
	nodesVisible["mAni"]	= aniVisible;
	nodesVisible["mGemNode"]	= gemVisible;
	NodeHelper:setNodesVisible(container, nodesVisible);
	
	NodeHelper:setStringForLabel(container, lb2Str);
	NodeHelper:setSpriteImage(container, sprite2Img, scaleMap);
	NodeHelper:setQualityFrames(container, itemImg2Qulity);
	
	local nameStr = common:getLanguageString("@LevelName", level, name);
	nameStr = common:fillHtmlStr("Quality_" .. quality, nameStr);
	local nameNode = container:getVarNode("mEquipmentName");
	
	local _label = NodeHelper:addHtmlLable(nameNode, nameStr);
	local stepLevel = EquipManager:getEquipStepById(equipId)	
	local starSprite = container:getVarSprite("mStar")
	local posX = _label:getContentSize().width + _label:getPositionX()-20
	local posY = _label:getPositionY() - ( _label:getContentSize().height - starSprite:getContentSize().height )/2
	EquipManager:setStarPosition(starSprite, stepLevel == GameConfig.ShowStepStar, posX, posY)

	
	local str = UserEquipManager:getEquipInfoString(userEquipId);
	local lbNode = container:getVarNode("mEquipmentTex");
	local lable = NodeHelper:addHtmlLable(lbNode, str);
	
	local texHeight = lable:getContentSize().height;
	local offset = texHeight - EquipItem.initTexHeight;
	if offset > 0 then
		local margin = GameConfig.Margin.EquipSelectBottom;
		texHeight = texHeight + margin;
		lable:setContentSize(CCSizeMake(lable:getContentSize().width, texHeight));
		lable:setPositionY(lable:getPositionY() + margin * lable:getScaleY());
		offset = offset + margin;
	end
	offset = offset * lable:getScaleY();
	for i = 1, 3 do
		local nodeName = "mScale9Sprite" .. i;
		local node = container:getVarNode(nodeName);
		if EquipItem.initSize[nodeName] == nil then
			EquipItem.initSize[nodeName] = node:getContentSize();
		end
		local size = EquipItem.initSize[nodeName];
		if offset > 0 then
			node:setContentSize(CCSizeMake(size.width, size.height + offset));
		else
			node:setContentSize(size);
		end
	end
	local size = EquipItem.initSize.containerNow;
	if offset > 0 then
		containerNode:setViewSize(CCSizeMake(size.width, size.height + offset));
	else
		containerNode:setViewSize(size);
	end
	containerNode:setBounceable(false);
	containerNode:setTouchEnabled(false);
	local node2 = container:getVarNode("mContentNode");
	if EquipItem.top == nil then
		EquipItem.top = node2:getPositionY();
	end
	local posY = EquipItem.top;
	if offset > 0 then
		posY = posY + offset;
	end
	node2:setPositionY(posY);
	
	if offset > 0 then
		local topNode = container:getVarNode("mBG01");
		local size = topNode:getContentSize();
		topNode:setContentSize(CCSizeMake(size.width, size.height + offset));
	
		local margin = 20.0;
		local svNode = container:getVarScrollView("mBackpackContent2");
		local viewSize = svNode:getViewSize();
		svNode:setViewSize(CCSizeMake(viewSize.width, viewSize.height - offset));
	
		local buttomNode = container:getVarNode("mBG02");
		local size = buttomNode:getContentSize();
		buttomNode:setContentSize(CCSizeMake(size.width, size.height - offset));
	end
	
	NodeHelper:addEquipAni(container, "mAni", aniVisible, userEquipId);
end

function EquipSelectPageBase:refreshPage(container)
	if PageInfo.currentEquipId then
		self:showCurrentEquip(container);
	end
	local isMultiSelect = PageInfo.selectType == SelectType.Multi;
	NodeHelper:setNodesVisible(container, {
		mEquipmentSelectPrompt 	= #PageInfo.optionIds == 0,
		mUnloadNode				= not isMultiSelect,
		mConfirmationNode		= isMultiSelect
	});
end	

----------------scrollview-------------------------
function EquipSelectPageBase:initScrollview(container)
	local svName = PageInfo.currentEquipId and "mBackpackContent2" or "mBackpackContent1";
	NodeHelper:initRawScrollView(container, svName);
	
	local hasEquiped = PageInfo.currentEquipId ~= nil;
	NodeHelper:setNodesVisible(container, {
		mBackpackContentNode 	= not hasEquiped,
		mEquipmentContentNode 	= hasEquiped
	});
end

function EquipSelectPageBase:rebuildAllItem(container)
	self:clearAllItem(container);
	if #PageInfo.optionIds > 0 then
		self:buildItem(container);
	end
end

function EquipSelectPageBase:clearAllItem(container)
	NodeHelper:clearScrollView(container);
end

function EquipSelectPageBase:buildItem(container)
	local size = #PageInfo.optionIds;
	NodeHelper:buildRawScrollView(container, size, EquipItem.ccbiFile, EquipItem.onFunction);
	if thisScrollView and thisScrollViewOffset then
		thisScrollView:setContentOffset(thisScrollViewOffset);
	end
end
	
	
function EquipSelectPageBase:refreshSelectedBox(container)
    if container.mScrollViewRootNode then
       local children = container.mScrollViewRootNode:getChildren()
       if children then
            for i=1,children:count(),1 do
                if children:objectAtIndex(i-1) then
                    local node =  tolua.cast(children:objectAtIndex(i-1),"CCNode")
                    EquipItem.refreshSelectBox(node)
                end
            end
       end
    end
end
----------------click event------------------------
function EquipSelectPageBase:onClose(container)
	PageManager.popPage(thisPageName);
end

function EquipSelectPageBase:onConfirm(container)
	if PageInfo.callback then
		PageInfo.callback(PageInfo.selectedIds);
	end
	PageManager.popPage(thisPageName);
end

--回包处理
function EquipSelectPageBase:onReceivePacket(container)
	local opcode = container:getRecPacketOpcode()
	local msgBuff = container:getRecPacketBuffer()

	if opcode == opcodes.EQUIP_DRESS_S then
		--UserEquipManager:takeOn();
		return
	end
end
--]]

--继承此类的活动如果同时开，消息监听不能同时存在,通过tag来区分
function EquipSelectPageBase:onReceiveMessage(container)
	local message = container:getMessage();
	local typeId = message:getTypeId();
	--[[
	if typeId == MSG_SEVERINFO_UPDATE then
		local opcode = MsgSeverInfoUpdate:getTrueType(message).opcode;
		if opcode == HP_pb.EQUIP_INFO_SYNC_S then
			self:refreshPage(container);
		end
	elseif typeId == MSG_MAINFRAME_POPPAGE then
		local pageName = MsgMainFramePopPage:getTrueType(message).pageName;
		if pageName == filterPage then
			self:refreshPage(container);
			self:rebuildAllItem(container);
		end
	else--]]
	if typeId == MSG_MAINFRAME_REFRESH then
		local pageName = MsgMainFrameRefreshPage:getTrueType(message).pageName
		if pageName == thisPageName then
			self:rebuildAllItem(container);
		end
	end
end

function EquipSelectPageBase:registerPacket(container)
	for key, opcode in pairs(opcodes) do
		if string.sub(key, -1) == "S" then
			container:registerPacket(opcode)
		end
	end
end

function EquipSelectPageBase:removePacket(container)
	for key, opcode in pairs(opcodes) do
		if string.sub(key, -1) == "S" then
			container:removePacket(opcode)
		end
	end
end
-------------------------------------------------------------------------
local CommonPage = require("CommonPage");
EquipSelectPage = CommonPage.newSub(EquipSelectPageBase, thisPageName, option);


function EquipSelectPage_setPart(part, roleId)
	PageInfo.targetPart = part;
	PageInfo.roleId = roleId or UserInfo.roleInfo.roleId;
	PageInfo.selectType = SelectType.Single;
	PageInfo.filterType = EquipFilterType.Dress;
	PageInfo.sortType = SortType.Score_ASC;
end	

function EquipSelectPage_multiSelect(selectedIds, limit, callback, currentId, filterType)
	PageInfo.selectedIds = common:deepCopy(selectedIds or {});
	PageInfo.currentEquipId = nil;
	if currentId and currentId > 0 then
		PageInfo.currentEquipId = currentId;
	end
	PageInfo.selectType = SelectType.Multi;
	PageInfo.limit = limit;
	PageInfo.callback = callback;
	PageInfo.isFull = #PageInfo.selectedIds >= PageInfo.limit;
	PageInfo.filterType = filterType;
	
	if filterType == EquipFilterType.Extend or filterType == EquipFilterType.CompoundDST then
		PageInfo.sortType = SortType.Score_ASC;
		PageInfo.targetPart = nil;
		if PageInfo.currentEquipId then
			local userEquip = UserEquipManager:getUserEquipById(PageInfo.currentEquipId);
			PageInfo.targetPart = EquipManager:getPartById(userEquip.equipId);
		end
	else
		PageInfo.sortType = SortType.Score_DESC;
	end
end		
