----------------------------------------------------------------------------------
--[[
	FILE:			GuildShopPage.lua
	ENCODING:		UTF-8, no-bomb
	DESCRIPTION:	公会商店页面
	AUTHOR:			sunyj
	CREATED:		2014-8-4
--]]
----------------------------------------------------------------------------------

local NodeHelper = require("NodeHelper")
local alliance = require('Alliance_pb')
local hp = require('HP_pb')
local GuildData = require("GuildData")
local GuildDataManager = require("GuildDataManager")
local thisPageName = 'GuildShopPage'
local GuildShopBase = {}

local option = {
	ccbiFile = "GuildShopPopUp.ccbi",
	handlerMap = {
		onCancel 		= 'onClose',
		onClose 		= 'onClose'
	}
}

function GuildShopBase:onEnter(container)
	self:registerPackets(container)
	NodeHelper:initScrollView(container, 'mContent', 10)
	self:refreshPage(container)
	GuildDataManager:getShopList(container)
end

function GuildShopBase:onExit(container)
	self:removePackets(container)
	NodeHelper:deleteScrollView(container)
end

function GuildShopBase:refreshPage(container)
	-- title 
	local lb2Str = {
		mYourContribution = 0
	}
	if GuildData.MyAllianceInfo.myInfo.contribution then
		lb2Str.mYourContribution = GuildData.MyAllianceInfo.myInfo.contribution
	end
	NodeHelper:setStringForLabel(container, lb2Str)
	-- scrollview
	if #GuildData.shopList ~= 0 then
		self:rebuildAllItem(container)
	end
end

function GuildShopBase:onClose(container)
	PageManager.popPage(thisPageName)
end

----------------scrollview item-------------------------
local ShopItem = {
	ccbiFile = 'GuildShopContentItem.ccbi',
}

local ShopItemSub = {
	ccbiFile = 'GuildShopContent.ccbi',
}

function ShopItem.onFunction(eventName, container)
	if eventName == "luaRefreshItemView" then
		ShopItem.onRefreshItemView(container)
	end
end

function ShopItemSub.onFunction(eventName, container)
	if eventName == "luaRefreshItemView" then
		ShopItemSub.onRefreshItemView(container)
	elseif eventName == 'onbuy' then
		ShopItemSub.buy(container)
	elseif eventName == 'onHand' then
		ShopItemSub.showTip(container)
	end
end

function ShopItemSub.onRefreshItemView(container)
	MessageBoxPage:Msg_Box('@refresh sub shop item')
end

function ShopItemSub.buy(container)
	local itemIndex = container:getTag()
	local info = GuildData.shopList[itemIndex]
	if not info then
		MessageBoxPage:Msg_Box('@GuildBuyItemEmpty')
		return 
	end
	local msg = alliance.HPAllianceShopBuyC()
	msg.type = info.type
	msg.id = info.id
	local pb = msg:SerializeToString()
	PacketManager:getInstance():sendPakcet(hp.ALLIANCE_SHOP_BUY_C, pb, #pb, false)
end

function ShopItemSub.showTip(container)
	local index = container:getTag();
	local item = GuildData.shopList[index];
	if item == nil then return; end
	
	local stepLevel = EquipManager:getEquipStepById(item.itemId)

	GameUtil:showTip(container:getVarNode('mHand'), {
		type 		= item.type, 
		itemId 		= item.id,
		buyTip		= true,
		starEquip	= stepLevel == GameConfig.ShowStepStar
	});
end

function ShopItem.onRefreshItemView(container)
	local shopItemId = container:getItemDate().mID
	local subContent
	local contentContainer
	for i=1, 3 do

		local subItemIndex = (shopItemId - 1) * 3 + i

		if subItemIndex > #GuildData.shopList then return end

		local shopInfo = GuildData.shopList[subItemIndex]
		if not shopInfo then return end

		local resInfo = ResManagerForLua:getResInfoByTypeAndId(shopInfo.type, shopInfo.id)
		if not resInfo then return end

		subContent = container:getVarNode('mPosition' .. tostring(i))

		if subContent then
			--clear old subItems
			subContent:removeAllChildren();
			
			-- create subItem
			contentContainer = ScriptContentBase:create(ShopItemSub.ccbiFile, subItemIndex)	
			contentContainer:registerFunctionHandler(ShopItemSub.onFunction)

			-- set sub item's view
			local lb2Str = {
				mNumber 			= shopInfo.remTime,
				mCommodityName 		= resInfo.name,
				mContributionNum 	= shopInfo.contribution
			}
			NodeHelper:setStringForLabel(contentContainer, lb2Str)

			--image
			NodeHelper:setSpriteImage(contentContainer,{mPic = resInfo.icon})

			-- add subItem into item
			subContent:addChild(contentContainer)
			contentContainer:release()
		end
	end
end	

----------------scrollview-------------------------
function GuildShopBase:rebuildAllItem(container)
	self:clearAllItem(container)
	self:buildItem(container)
end

function GuildShopBase:clearAllItem(container)
	NodeHelper:clearScrollView(container)
end

function GuildShopBase:buildItem(container)
	local needItemCount = math.ceil(#GuildData.shopList / 3)
	NodeHelper:buildScrollView(container, needItemCount, ShopItem.ccbiFile, ShopItem.onFunction);
end

---------------------------- packet function ------------------------------------
function GuildShopBase:registerPackets(container)
	container:registerPacket(hp.ALLIANCE_SHOP_S)
	container:registerPacket(hp.ALLIANCE_ENTER_S)
end

function GuildShopBase:removePackets(container)
	container:removePacket(hp.ALLIANCE_SHOP_S)
	container:removePacket(hp.ALLIANCE_ENTER_S)
end

function GuildShopBase:onReceiveShopList(container, msg)
	GuildData.shopList = msg.items
end

function GuildShopBase:onReceiveAllianceEnterInfo(container, msg)
	GuildData.MyAllianceInfo = msg
end

function GuildShopBase:onReceivePacket(container)
	local opcode = container:getRecPacketOpcode()
	local msgBuff = container:getRecPacketBuffer()

	if opcode == hp.ALLIANCE_SHOP_S then
		local msg = alliance.HPAllianceShopS()
		msg:ParseFromString(msgBuff)
		self:onReceiveShopList(container, msg)
		self:refreshPage(container)
		return
	end

	if opcode == hp.ALLIANCE_ENTER_S then
		-- alliance enter
		local msg = alliance.HPAllianceEnterS()
		msg:ParseFromString(msgBuff)
		self:onReceiveAllianceEnterInfo(container, msg)
		self:refreshPage(container)
		return
	end
end

local CommonPage = require('CommonPage')
local GuildShopPage = CommonPage.newSub(GuildShopBase, thisPageName, option)
