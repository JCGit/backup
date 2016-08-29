----------------------------------------------------------------------------------
--[[
	FILE:			ExpeditionContributePage.lua
	ENCODING:		UTF-8, no-bomb
	DESCRIPTION:	活动
	AUTHOR:			zhoutong
	CREATED:		2014-10-15
--]]
----------------------------------------------------------------------------------
local UserItemManager = require("UserItemManager");
local Activity_pb = require("Activity_pb");
local HP_pb = require("HP_pb");
local NodeHelper = require("NodeHelper");
local Const_pb = require("Const_pb");
local ExpeditionDataHelper = require("ExpeditionDataHelper")

local thisPageName = "ExpeditionContributePage"
local opcodes = {
	EXPEDITION_ARMORY_USE_ITEM_C    = HP_pb.EXPEDITION_ARMORY_USE_ITEM_C,
	EXPEDITION_ARMORY_INFO_C 		= HP_pb.EXPEDITION_ARMORY_INFO_C,
	PLAYER_CONSUME_S                = HP_pb.PLAYER_CONSUME_S
};
local option = {
	ccbiFile = "Act_ExpeditionMaterialsDonationPopUp.ccbi",
	handlerMap = {
		onClose	        = "onClose",
	},
	opcode = opcodes
};
----------------- local data -----------------
local ExpeditionContributePageBase = {}

local ExpeditionDonationItem = {
	ccbiFile = "Act_ExpeditionMaterialsDonationContent.ccbi"
}

function ExpeditionDonationItem.onFunction(eventName, container)
    if eventName == "luaRefreshItemView" then
		ExpeditionDonationItem.onRefreshItemView(container);
	elseif eventName == "onDonation" then
		ExpeditionDonationItem.onDonationOne(container);
	elseif eventName == "onDonationTen" then
		ExpeditionDonationItem.onDonationTen(container);
    elseif eventName == "onFeet" then
		ExpeditionDonationItem.showItemInfo(container, eventName);
	end	
end

function ExpeditionDonationItem.onRefreshItemView(container)
	local index = container:getItemDate().mID;
	local labelString = {}
	local nodeVisible = {}
	local donationInfo = ExpeditionDataHelper.getDonationItemInfoByIndex(index)

	if donationInfo~=nil then
	    local resInfo = ResManagerForLua:getResInfoByTypeAndId(donationInfo.itemType, donationInfo.item, 1);

        local htmlNode = container:getVarLabelBMFont("mGetReward")
        if htmlNode~=nil then
            local str = FreeTypeConfig[501].content
            str = common:fill(str,tostring(donationInfo.reward))
            NodeHelper:setCCHTMLLabelAutoFixPosition( htmlNode, CCSize(400,64),str )
            htmlNode:setVisible(false)
        end
        local itemInfo = UserItemManager:getUserItemByItemId(donationInfo.item)
        if itemInfo~=nil  then
            labelString.mItemCount = tostring(itemInfo.count)
        end
        
        NodeHelper:setNodesVisible(container, nodeVisible);
        NodeHelper:setStringForLabel(container, labelString);
        NodeHelper:setSpriteImage(container, {mTextPic = resInfo.icon});
        NodeHelper:setQualityFrames(container, {mFeet = resInfo.quality});
	end
end

function ExpeditionDonationItem.showItemInfo(container,eventName)
    local index = container:getItemDate().mID;
	local donationInfo = ExpeditionDataHelper.getDonationItemInfoByIndex(index)
	if donationInfo~=nil then 
	    local cfg = {type=donationInfo.itemType,itemId=donationInfo.item,count=1}
	    GameUtil:showTip(container:getVarNode('mFeet'), common:table_merge(cfg,{buyTip=true,hideBuyNum=true}));
	end
end


function ExpeditionDonationItem.onDonationOne(container)
    local index = container:getItemDate().mID;
    local donationItem = ExpeditionDataHelper.getDonationItemInfoByIndex(index)
    
	if donationItem~=nil then
        local itemInfo = UserItemManager:getUserItemByItemId(donationItem.item)
        if itemInfo==nil or itemInfo.count<1 then
            MessageBoxPage:Msg_Box_Lan("@ExpeditionContributeNotEnough");
            return
        end
    end
        
    if donationItem~=nil then
        local msg = Activity_pb.HPExpeditionArmoryItemUse();
        msg.id =  index
        msg.count = 1
        common:sendPacket(opcodes.EXPEDITION_ARMORY_USE_ITEM_C, msg,false);
        --PageManager.popPage(thisPageName);
        
        local msg = Activity_pb.HPExpeditionArmoryInfo();
        msg.version = 1
        common:sendPacket(opcodes.EXPEDITION_ARMORY_INFO_C, msg);
    end
end

function ExpeditionDonationItem.onDonationTen(container)
    local index = container:getItemDate().mID;
    local donationItem = ExpeditionDataHelper.getDonationItemInfoByIndex(index)
    
    if donationItem~=nil then
        local itemInfo = UserItemManager:getUserItemByItemId(donationItem.item)
        if itemInfo==nil or itemInfo.count<10 then
            MessageBoxPage:Msg_Box_Lan("@ExpeditionContributeNotEnough");
            return
        end
    end
    
    if donationItem~=nil then
        local msg = Activity_pb.HPExpeditionArmoryItemUse();
        msg.id =  index
        msg.count = 10
        common:sendPacket(opcodes.EXPEDITION_ARMORY_USE_ITEM_C, msg,false);
        --PageManager.popPage(thisPageName);
        
        local msg = Activity_pb.HPExpeditionArmoryInfo();
        msg.version = 1
        common:sendPacket(opcodes.EXPEDITION_ARMORY_INFO_C, msg);
    end
end

-----------------------------------------------
--ExpeditionContributePageBase页面中的事件处理
----------------------------------------------
function ExpeditionContributePageBase:onEnter(container)
    NodeHelper:initScrollView(container, "mContent", 3);
    
    self:registerPacket(container)
    ExpeditionContributePageBase:refreshPage(container)
end

function ExpeditionContributePageBase:onExecute(container)

end

function ExpeditionContributePageBase:onExit(container)
    self:removePacket(container)
    NodeHelper:deleteScrollView(container);
end


function ExpeditionContributePageBase:refreshPage(container)
    self:rebuildAllItem(container);
end



----------------scrollview-------------------------
function ExpeditionContributePageBase:rebuildAllItem(container)
    self:clearAllItem(container);
	self:buildItem(container);
end

function ExpeditionContributePageBase:clearAllItem(container)
    NodeHelper:clearScrollView(container);
end

function ExpeditionContributePageBase:buildItem(container)
    local size = ExpeditionDataHelper.getDonationItemSize()
    NodeHelper:buildScrollView(container,size, ExpeditionDonationItem.ccbiFile, ExpeditionDonationItem.onFunction);
end
----------------click event------------------------	
function ExpeditionContributePageBase:onClose(container)
	PageManager.popPage(thisPageName);
end	
-------------------------------------------------------------------------
function ExpeditionContributePageBase:onReceivePacket(container)
	local opcode = container:getRecPacketOpcode();
	local msgBuff = container:getRecPacketBuffer();
    if opcode == opcodes.PLAYER_CONSUME_S then
		self:refreshPage(container);
	end
end

function ExpeditionContributePageBase:registerPacket(container)
	for key, opcode in pairs(opcodes) do
		if string.sub(key, -1) == "S" then
			container:registerPacket(opcode)
		end
	end
end

function ExpeditionContributePageBase:removePacket(container)
	for key, opcode in pairs(opcodes) do
		if string.sub(key, -1) == "S" then
			container:removePacket(opcode)
		end
	end
end
----------------------------------------------------------------------------




local CommonPage = require("CommonPage");
ExpeditionContributePage = CommonPage.newSub(ExpeditionContributePageBase, thisPageName, option);
