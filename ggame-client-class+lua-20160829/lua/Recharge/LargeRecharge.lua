require "Recharge_pb"

local thisPageName = "LargeRecharge"

local option = {
	ccbiFile = "LargeRechargePopUp.ccbi",
	handlerMap = {
		onMoney1 		= "onMoney1",
		onMoney2		= "onMoney2",
		onMoney3        = "onMoney3",
		onClose			= "onClose"
	}
};
local LargeRechargeBase = {}
local pageInfo = {
	largeRechargeList = {}
}
--------------------------------------------------------------

function LargeRechargeBase:onEnter( container )
	for i = 1,#pageInfo.largeRechargeList do
		container:getVarLabelBMFont("mMoney" .. i):setString(common:getLanguageString("@RMB") .. pageInfo.largeRechargeList[i].productPrice)
	end
	
end

function LargeRechargeBase:onExecute( container )
	
end

function LargeRechargeBase:onExit( container )
	
end

function LargeRechargeBase:onMoney1( container )
	self:onRechanrge(1)
end

function LargeRechargeBase:onMoney2( container )
	self:onRechanrge(2)
end

function LargeRechargeBase:onMoney3( container )
	self:onRechanrge(3)
end

function LargeRechargeBase:onRechanrge( index )
	
	local itemInfo = pageInfo.largeRechargeList[index]
		
	if itemInfo.productType == 1  then
		PacketManager:getInstance():sendPakcet(HP_pb.MONTHCARD_PREPARE_BUY, "", 0, false)
	end
		
	local buyInfo = BUYINFO:new()
	
	buyInfo.productCount = 1
	buyInfo.productName = itemInfo.productName
	buyInfo.productId = itemInfo.productId
	buyInfo.productPrice = itemInfo.productPrice
	buyInfo.productOrignalPrice = itemInfo.gold
	buyInfo.description = GamePrecedure:getInstance():getServerID()--ServerDateManager:getInstance().mLoginInfo.m_iPlayerID
		
	libPlatformManager:getPlatform():buyGoods(buyInfo)
end

function LargeRechargeBase:onClose( container )
	PageManager.popPage( thisPageName )
end

function Set_LargeRechargeList( largeRechargeList )
	pageInfo.largeRechargeList = largeRechargeList
end

---------------------------------------------------------------
local CommonPage = require("CommonPage");
local RechargePage = CommonPage.newSub(LargeRechargeBase, thisPageName, option)
LargeRechargeBase = nil