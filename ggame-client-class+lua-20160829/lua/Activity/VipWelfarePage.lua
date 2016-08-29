--[[
	FILE:			VipWelfarePage.lua
	ENCODING:		UTF-8, no-bomb
	DESCRIPTION:	Vip福利
	AUTHOR:			dmq
	CREATED:		2014-10-16
--]]
require "Activity_pb"
local UserInfo = require("UserInfo");
local CommonPage = require("CommonPage")
local option = {
	ccbiFile = "Act_VipWelfarePage.ccbi",
	handlerMap = {
		onReturnButton	= "onBack",
		onRechargeBtn	= "onRecharge",
		onReceiveBtn	= "onReceiveReward",
		onHelp			= "onHelp"
	}
}
local NodeHelper = require("NodeHelper");
local VipWelfarePageBase = {}

local VipWelfareItem = {
	ccbiFile = "Act_VipWelfareContent.ccbi",
}

local PageInfo = {
	vipWelfareInfo = {},
	rewardState = false
}

local thisPageName = "VipWelfarePage"
---------------------------------------------------------------

function VipWelfareItem.onFunction( eventName, container )
	if eventName == "luaRefreshItemView" then
		VipWelfareItem.onRefreshItemView(container)
	elseif eventName:sub(1, 7) == "onFrame" then
		VipWelfareItem.onShowItemInfo( container , eventName )
	end
end

function VipWelfareItem.onRefreshItemView( container )
	local index = container:getItemDate().mID
	local itemInfo = PageInfo.vipWelfareInfo[index]	
	container:getVarSprite("mVipNum"):setTexture(GameConfig.VipLevelPic[itemInfo.id])
	container:getVarScale9Sprite("mSelecteBG"):setVisible(itemInfo.id == UserInfo.playerInfo.vipLevel)
		
	local rewardItems = {}
	if itemInfo.reward ~= nil then
		for _, item in ipairs(common:split(itemInfo.reward, ",")) do
			local _type, _id, _count = unpack(common:split(item, "_"));
			table.insert(rewardItems, {
				type 	= tonumber(_type),
				itemId	= tonumber(_id),
				count 	= tonumber(_count)
			});
		end
	end
	
	NodeHelper:fillRewardItem(container, rewardItems)
	
end

function VipWelfareItem.onShowItemInfo( container , eventName )
	local index = container:getItemDate().mID
	local itemInfo = PageInfo.vipWelfareInfo[index]	
	
	local rewardItems = {}
	if itemInfo.reward ~= nil then
		for _, item in ipairs(common:split(itemInfo.reward, ",")) do
			local _type, _id, _count = unpack(common:split(item, "_"));
			table.insert(rewardItems, {
				type 	= tonumber(_type),
				itemId	= tonumber(_id),
				count 	= tonumber(_count)
			});
		end
	end
	
	local rewardIndex = tonumber(eventName:sub(8))
	GameUtil:showTip(container:getVarNode('mFrame' .. rewardIndex), rewardItems[rewardIndex])
	
end

function VipWelfarePageBase:onEnter( container )
	self:registerPacket( container )	
	container:registerMessage(MSG_MAINFRAME_REFRESH)
	PageInfo.vipWelfareInfo = ConfigManager.getVipWelfareCfg()
	NodeHelper:initScrollView(container, "mContent", 5);
	if container.mScrollView ~=nil then
		container:autoAdjustResizeScrollview(container.mScrollView);
	end
	self:rebuildAllItems( container )
	self:getRewardStatue( container )
end	

function VipWelfarePageBase:getRewardStatue( container )
	common:sendEmptyPacket( HP_pb.VIP_WELFARE_INFO_C , true )
end

function VipWelfarePageBase:refreshPage( container )
	if PageInfo.rewardState then
		container:getVarMenuItemImage("mReceiveBtn"):selected()
	else
		container:getVarMenuItemImage("mReceiveBtn"):unselected()
	end
end

function VipWelfarePageBase:rebuildAllItems( container )
	UserInfo.sync()
	self:clearAllItems( container )
	self:buildAllItems( container )
	self:setCurVipReward( container )
end

function VipWelfarePageBase:setCurVipReward( container )
	local totalOffset = container.mScrollView:getContentOffset()
	if UserInfo.playerInfo.vipLevel == 0 or UserInfo.playerInfo.vipLevel == 1 then
	    return
	end
	
	local curY = totalOffset.y + container.mScrollView:getContentSize().height * ( UserInfo.playerInfo.vipLevel - 1) / 15
	
	if curY > 0  then
		curY = 0
	end
	
	local curOffset = CCPointMake( totalOffset.x , curY ) 
	container.mScrollView:setContentOffset( curOffset )
end

function VipWelfarePageBase:clearAllItems( container )
	NodeHelper:clearScrollView(container)
end

function VipWelfarePageBase:buildAllItems( container )
	NodeHelper:buildScrollView(container, #PageInfo.vipWelfareInfo, VipWelfareItem.ccbiFile, VipWelfareItem.onFunction)
end

function VipWelfarePageBase:onExecute( container )

end

function VipWelfarePageBase:onExit( container )
	self:removePacket( container )
end

function VipWelfarePageBase:onBack( container )
	PageManager.changePage("ActivityPage")
end

function VipWelfarePageBase:onRecharge( container )
	PageManager.pushPage("RechargePage")
end

function VipWelfarePageBase:onHelp( container )
	PageManager.showHelp(GameConfig.HelpKey.HELP_VIPWELFARE)
end

function VipWelfarePageBase:onReceiveReward( container )
	if PageInfo.rewardState then
		MessageBoxPage:Msg_Box_Lan("@VipWelfareAlreadyReceive")
		self:refreshPage( container )
	else
		UserInfo.sync()
		if UserInfo.playerInfo.vipLevel > 0 then
			common:sendEmptyPacket( HP_pb.VIP_WELFARE_AWARD_C , true )
		else
			MessageBoxPage:Msg_Box_Lan("@VipLevelNotEnough")
		end
	end
end	

function VipWelfarePageBase:onReceivePacket(container)
	local opcode = container:getRecPacketOpcode();
	local msgBuff = container:getRecPacketBuffer();
    if opcode == HP_pb.VIP_WELFARE_INFO_S then
		local msg = Activity_pb.HPVipWelfareInfoRet()
		msg:ParseFromString(msgBuff)
		PageInfo.rewardState = msg.awardStatus == 1
		self:refreshPage( container )
		return;
	end
	if opcode == HP_pb.VIP_WELFARE_AWARD_S then
		local msg = Activity_pb.HPVipWelfareRewardRet()
		msg:ParseFromString(msgBuff)
		PageInfo.rewardState = msg.awardStatus == 1
		self:refreshPage( container )
		return;
	end		
end

function VipWelfarePageBase:onReceiveMessage(container)
	local message = container:getMessage()
	local typeId = message:getTypeId()
	if typeId == MSG_MAINFRAME_REFRESH then
		local pageName = MsgMainFrameRefreshPage:getTrueType(message).pageName
		if pageName == thisPageName then
			self:rebuildAllItems( container )
			self:getRewardStatue( container )
		end
	end
end

function VipWelfarePageBase:registerPacket(container)
	container:registerPacket(HP_pb.VIP_WELFARE_INFO_S)
	container:registerPacket(HP_pb.VIP_WELFARE_AWARD_S)	
end

function VipWelfarePageBase:removePacket(container)
	container:removePacket(HP_pb.VIP_WELFARE_INFO_S)
	container:removePacket(HP_pb.VIP_WELFARE_AWARD_S)	
end
---------------------------------------------------------------
VipWelfarePage = CommonPage.newSub(VipWelfarePageBase, thisPageName, option);

