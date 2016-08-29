----------------------------------------------------------------------------------
--[[
	FILE:			RechargePage.lua
	ENCODING:		UTF-8, no-bomb
	DESCRIPTION:	充值页面
	AUTHOR:			hgs
	CREATED:		2014-01-21
--]]
----------------------------------------------------------------------------------
require "Recharge_pb"
local UserInfo = require("UserInfo");
local thisPageName = "RechargePage"

local option = {
	ccbiFile = "RechargePopUp.ccbi",
	handlerMap = {
		onClose 		= "onClose",
		onVipPrivilege	= "showVipPrivilege",
		onNext          = "onVipNext",
        onPrevious      = "onVipPrevious",
		onLargeRecharge = "onLargeRecharge"  
	},
	opcode = opcodes
};
local ResManagerForlua = require("ResManagerForLua")

local RechargePageBase = {}

local NodeHelper = require("NodeHelper");
local TodoStr = "99";
local TodoImage = "UI/MainScene/UI/u_ico000.png";
local TodoPoster = "person/poster_char_Boss.png";

local PageInfo = {
	rechargeListInfo = {},
	largeRechargeListInfo = {},
	ONE_LINE_COUNT = 3,
	currentPageType = nil,
	vipTag = 0
}

local PageType = {
	recharge = 1,
	vip = 2
}

local vipTable = {}

local platformManager = {}
--RechargePlatformNames = {}
--------------------------------------------------------------
local RechargeItem = {
	ccbiFile 	= "RechargeContent.ccbi"
}

function RechargeItem.onFunction(eventName, container)
	if eventName == "luaRefreshItemView" then
		RechargeItem.onRefreshItemView(container)
	end
end

function RechargeItem.onRefreshItemView(container)
	local contentId = container:getItemDate().mID
	local baseIndex = (contentId - 1) * PageInfo.ONE_LINE_COUNT
	--local baseIndex = contentId * PageInfo.ONE_LINE_COUNT
	for i = 1, PageInfo.ONE_LINE_COUNT do
		local nodeContainer = container:getVarNode("mPositionNode" .. i)
		NodeHelper:setNodeVisible(nodeContainer, false)
		nodeContainer:removeAllChildren()
		
		local itemNode = nil
		local index = baseIndex + i
		--#PageInfo.personList
		if index <= #PageInfo.rechargeListInfo then
			itemNode = RechargeItem.newLineItem(index)
		end	
		
		if itemNode then
			nodeContainer:addChild(itemNode)
			NodeHelper:setNodeVisible(nodeContainer, true)
		end
	end
end

function RechargeItem.newLineItem( index )
    local itemInfo = PageInfo.rechargeListInfo[index]
	local itemNode = ScriptContentBase:create("RechargeContent.ccbi", index)
	itemNode:registerFunctionHandler(RechargeItem.onRecharge)	
	itemNode:getVarLabelBMFont("mmItemRMB"):setString( common:getLanguageString("@RMB") .. itemInfo.productPrice )
	itemNode:getVarLabelBMFont("mItemGold"):setString( itemInfo.gold )
	
	
	
	if itemInfo.productType == 0 then
	    NodeHelper:setSpriteImage(itemNode, {mItemPic = GameConfig.Image.Diamond})
		local framePath = GameConfig.RechargeItemFrame[index]
	    if framePath == nil then
	        framePath = GameConfig.RechargeItemFrame[6]
	    end
		
		itemNode:getVarMenuItemImage("mItemFrame"):setNormalImage(CCSprite:create(framePath))
        --NodeHelper:setSpriteImage( itemNode , {mItemFrame = GameConfig.RechargeItemFrame[index]} )
	    if itemInfo:HasField("ratio") then
	    	local itemLabelSprite = itemNode:getVarSprite("mItemLabel")
	    	if itemLabelSprite ~= nil then
				itemLabelSprite:setVisible(true)
		    	--首冲3倍
		    	if itemInfo.ratio == 3 then
		    		local threeRatioRecharge = VaribleManager:getInstance():getSetting("threeRatioRecharge")
		    		itemLabelSprite:setTexture(tostring(threeRatioRecharge))
	    		elseif itemInfo.ratio == 2 then
		    		local twoRatioRecharge = VaribleManager:getInstance():getSetting("twoRatioRecharge")
		    		itemLabelSprite:setTexture(tostring(twoRatioRecharge))
	    		else
	    			itemLabelSprite:setVisible(false)
		    	end
		    end
	    else
		    itemNode:getVarSprite("mItemLabel"):setVisible(false)
	    end
	    itemNode:getVarLabelBMFont("mItemGold"):setString( itemInfo.gold )
	elseif itemInfo.productType == 1 then
	    NodeHelper:setSpriteImage(itemNode, {mItemPic = GameConfig.Image.MonthCard})
	    itemNode:getVarSprite("mItemLabel"):setVisible(false)
	    itemNode:getVarLabelBMFont("mItemGold"):setString( common:getLanguageString("@MonthCard") )
	end
	
	
	itemNode:release()
	
	return itemNode
end

--展示奖励描述信息
function RechargeItem.onRecharge(eventName , container)
	if eventName == "onItemFrame" then
		local index = container:getTag()
		local itemInfo = PageInfo.rechargeListInfo[index]
		
		if itemInfo.productType == 1  then
		    PacketManager:getInstance():sendPakcet(HP_pb.MONTHCARD_PREPARE_BUY, "", 0, false)
		end
		
		local buyInfo = BUYINFO:new()
	    
	    local productName = ""
		
		if itemInfo.productType == 0 then
			productName = itemInfo.productName
		else
			productName = common:getLanguageString("@MonthCard")
		end
	    
		buyInfo.productCount = 1
		buyInfo.productName = productName
		buyInfo.productId = itemInfo.productId
		buyInfo.productPrice = itemInfo.productPrice
		buyInfo.productOrignalPrice = itemInfo.gold
		buyInfo.description = GamePrecedure:getInstance():getServerID()--ServerDateManager:getInstance().mLoginInfo.m_iPlayerID
		
		libPlatformManager:getPlatform():buyGoods(buyInfo)
	end
end

----------------------------------------------------------------------------------
	
-----------------------------------------------
--RechargePageBase页面中的事件处理
----------------------------------------------
function RechargePageBase:onEnter(container)
	container:registerMessage(MSG_MAINFRAME_REFRESH)
	self:registerPacket(container)
	NodeHelper:initScrollView(container, "mContent", 3)
	
	UserInfo.sync()
	self:readTable()
	vipTable = ConfigManager.getVipCfg()
	
	NodeHelper:setNodesVisible(container, {
			mPrivilegeNode 		= false,
			mContentNode		= true
	})
	
	self:refreshPage( container )
	
	container:getVarNode("mLargeRechargeNode"):setVisible( UserInfo.playerInfo.vipLevel >= 0 )
	
	if PageInfo.currentPageType == nil then
		PageInfo.currentPageType = PageType.recharge;
	end
	if PageInfo.currentPageType == PageType.vip then
		PageInfo.currentPageType = PageType.recharge;
		self:showVipPrivilege(container);
	end
	
	local msg = Recharge_pb.HPFetchShopList()

	if BlackBoard:getInstance().PLATFORM_TYPE_FOR_LUA == 1 then
		local  pName = RechargePlatformNames[libPlatformManager:getPlatform():getPlatformId()]
		if pName == nil then
			msg.platform = "Android_Default"
		else
			msg.platform = pName --	""	--读取配置表
		end			
	elseif BlackBoard:getInstance().PLATFORM_TYPE_FOR_LUA == 2 then
		msg.platform = "ios_appstore"
	else
		msg.platform = ""--RechargePlatformNames[libPlatformManager:getPlatform():getPlatformId()]
	end 
	
	--msg.platform = ""
	CCLuaLog( "PlatformName2:" .. msg.platform )
	pb_data = msg:SerializeToString()
	PacketManager:getInstance():sendPakcet(HP_pb.FETCH_SHOP_LIST_C, pb_data , #pb_data, true)	
	--self:rebuildAllItem(container)
end

function RechargePageBase:refreshPage( container )
	container:getVarSprite("mNowVip"):setTexture(GameConfig.VipLevelPic[UserInfo.playerInfo.vipLevel])
	
	
	local vipLv = UserInfo.playerInfo.vipLevel + 1
	
	if vipLv <= 15 then
		local needDiamon = vipTable[vipLv]["buyDiamon"] - UserInfo.playerInfo.recharge
		nameStr = common:getGsubStr( {needDiamon,vipLv} , FreeTypeConfig[83].content)
		
		NodeHelper:addHtmlLable(container:getVarLabelBMFont("mRechargePrompt"), nameStr , GameConfig.Tag.HtmlLable)
		
		container:getVarSprite("mTargetVip"):setVisible(true) 
		container:getVarSprite("mTargetVip"):setTexture(GameConfig.VipLevelPic[vipLv])
		
		local scal = (UserInfo.playerInfo.recharge - vipTable[UserInfo.playerInfo.vipLevel]["buyDiamon"])/(vipTable[vipLv]["buyDiamon"] - vipTable[UserInfo.playerInfo.vipLevel]["buyDiamon"])
		if scal < 0 then
		    scal = 0
		end
		container:getVarScale9Sprite("mVipExp"):setScaleX(scal)		
	else
		container:getVarSprite("mTargetVip"):setVisible(false)
		container:getVarScale9Sprite("mVipExp"):setScaleX(1.0)
		container:getVarLabelBMFont("mRechargePrompt"):setString( common:getLanguageString("@MostVip") )	 
	end
	
	container:getVarNode("mLargeRechargeNode"):setVisible( UserInfo.playerInfo.vipLevel >= 7 )
		
end

function RechargePageBase:onExecute(container)
	
end

function RechargePageBase:onExit(container)
	self:removePacket(container)
	--container:removeMessage(MSG_SEVERINFO_UPDATE)	
	container:removeMessage(MSG_MAINFRAME_REFRESH)	
	NodeHelper:deleteScrollView(container);
	PageInfo.rechargeListInfo = {}
	PageInfo.largeRechargeListInfo = {}
	PageInfo.currentPageType = nil;
end

function RechargePageBase:onReceiveMessage(container)
	local message = container:getMessage()
	local typeId = message:getTypeId()
	if typeId == MSG_MAINFRAME_REFRESH then
		local pageName = MsgMainFrameRefreshPage:getTrueType(message).pageName
		if pageName == thisPageName then
			self:refreshCurPage(container)
			self:refreshPage(container)
		end
	end
end

function RechargePageBase:refreshCurPage( container )
	if PageInfo.currentPageType == PageType.recharge then

		NodeHelper:setNodesVisible(container, {
			mPrivilegeNode 		= false,
			mContentNode		= true
		})
		container:getVarLabelBMFont("mVipPrivilege"):setString(common:getLanguageString("@VipPrivilege"))
		container:getVarLabelBMFont("mTitle"):setString(common:getLanguageString("@RechargeTitle"))
				
	else
	
		NodeHelper:setNodesVisible(container, {
			mPrivilegeNode 		= true,
			mContentNode		= false
		})
		PageInfo.vipTag = UserInfo.playerInfo.vipLevel
		self:refreshVipPage( container )		
		container:getVarLabelBMFont("mVipPrivilege"):setString(common:getLanguageString("@RechargeTitle"))
		container:getVarLabelBMFont("mTitle"):setString(common:getLanguageString("@VipTitle"))
	
	end
end

----------------------------------------------------------------

----------------scrollview-------------------------
function RechargePageBase:rebuildAllItem(container)
	self:clearAllItem(container);
	self:buildItem(container);
end

function RechargePageBase:clearAllItem(container)
	NodeHelper:clearScrollView(container);
end

function RechargePageBase:buildItem(container)
    --#PageInfo.rechargeListInfo
    local size = math.ceil(#PageInfo.rechargeListInfo / PageInfo.ONE_LINE_COUNT) -- RechargeContentItem
	NodeHelper:buildScrollView(container,size, "RechargeContentItem.ccbi", RechargeItem.onFunction);
end
	
----------------click event------------------------
function RechargePageBase:onClose(container)
	PageManager.popPage(thisPageName);
end

function RechargePageBase:showVipPrivilege(container)
	if PageInfo.currentPageType == PageType.recharge then
		PageInfo.currentPageType = PageType.vip
		NodeHelper:setNodesVisible(container, {
			mPrivilegeNode 		= true,
			mContentNode		= false
		})
		PageInfo.vipTag = UserInfo.playerInfo.vipLevel
		self:refreshVipPage( container )		
		container:getVarLabelBMFont("mVipPrivilege"):setString(common:getLanguageString("@RechargeTitle"))
		container:getVarLabelBMFont("mTitle"):setString(common:getLanguageString("@VipTitle"))
	else
		PageInfo.currentPageType = PageType.recharge
		NodeHelper:setNodesVisible(container, {
			mPrivilegeNode 		= false,
			mContentNode		= true
		})
		container:getVarLabelBMFont("mVipPrivilege"):setString(common:getLanguageString("@VipPrivilege"))
		container:getVarLabelBMFont("mTitle"):setString(common:getLanguageString("@RechargeTitle"))
	end
	
	
	
end

function RechargePageBase:refreshVipPage( container )
	local bossExtraAward = common:getLanguageString("@Null")
	if vipTable[PageInfo.vipTag]["bossExtraAward"] ~= "0" then
		local reward = ResManagerForlua:getResInfoByTypeAndId( tonumber(Split(vipTable[PageInfo.vipTag]["bossExtraAward"],"_")[1]),tonumber(Split(vipTable[PageInfo.vipTag]["bossExtraAward"],"_")[2]),tonumber(Split(vipTable[PageInfo.vipTag]["bossExtraAward"],"_")[3]) )
		bossExtraAward = reward.name	
	end
	local vipTrain = common:getLanguageString("@VipTrain" .. vipTable[PageInfo.vipTag]["maxMercenaryTime"])
    local hasBossMopUp = vipTable[PageInfo.vipTag]["hasBossMopUp"]
    local strBossMopUp = ""
    local hasUnionBoss = vipTable[PageInfo.vipTag]["hasUnionBoss"]
	local expBuffer = vipTable[PageInfo.vipTag]["expBuffer"]
    local strUnionBoss = ""
    if hasBossMopUp == 0 then strBossMopUp = nil end
    if hasUnionBoss == 0 then strUnionBoss = nil end
    local vipData = {		
		[1] = vipTable[PageInfo.vipTag]["buyDiamon"],
		[2] = vipTable[PageInfo.vipTag]["fastFightTime"],
		[3] = vipTable[PageInfo.vipTag]["buyBossFightTime"],
		[4] = vipTable[PageInfo.vipTag]["buyCoinTime"],
		[5] = tonumber(vipTable[PageInfo.vipTag]["shopItemCount"]) - GameConfig.Default.ShopItemNum,
		[6] = bossExtraAward,
		[7] = vipTrain,
		[8] = expBuffer,--strBossMopUp,
		[9] = strBossMopUp --strUnionBoss
	}
	
	for i = 1,9 do
		
		local labText = container:getVarLabelBMFont("mPrivilegeTex" .. i)
		
		if labText ~= nil then
			
			local tag = GameConfig.Tag.HtmlLable + i
			local index = 199 + i			
			nameStr = common:getGsubStr( {vipData[i]} , FreeTypeConfig[index].content)
			--local nameNode = container:getVarNode("mEquipmentName")
			NodeHelper:addHtmlLable(labText, nameStr, tag);
		end
		
		
	end
		
	container:getVarLabelBMFont("mPrivilegeTitle"):setString(common:getLanguageString("@PrivilegeTitle",vipTable[PageInfo.vipTag]["vipLevel"]))
	
	
end

function RechargePageBase:onVipNext( container )
	if PageInfo.vipTag < 15 then
		PageInfo.vipTag = PageInfo.vipTag + 1
		self:refreshVipPage( container )
	end
end

function RechargePageBase:onVipPrevious( container )
	if PageInfo.vipTag > 0 then
		PageInfo.vipTag = PageInfo.vipTag - 1
		self:refreshVipPage( container )
	end
end

function RechargePageBase:onReceivePacket(container)
	local opcode = container:getRecPacketOpcode()
	local msgBuff = container:getRecPacketBuffer()
	CCLuaLog( "Recharge opcode :" .. opcode )
	if opcode == HP_pb.FETCH_SHOP_LIST_S then
		local msg = Recharge_pb.HPShopListSync()
		msg:ParseFromString(msgBuff)
		CCLuaLog( "Recharge ShopItemNum :" .. #msg.shopItems )
		self:onGetShopList( container , msg )
	end
end

function RechargePageBase:onGetShopList( container , msg )
	for k,v in ipairs(msg.shopItems) do
		if v.productPrice < 960 then
			table.insert(PageInfo.rechargeListInfo , v)
		else
			table.insert(PageInfo.largeRechargeListInfo , v)
		end
	end	
	self:rebuildAllItem(container);
end

function RechargePageBase:registerPacket(container)
	container:registerPacket(HP_pb.FETCH_SHOP_LIST_S)	
end

function RechargePageBase:removePacket(container)
    
	container:removePacket(HP_pb.FETCH_SHOP_LIST_S)
end

function RechargePageBase:readTable()
	
	if RechargePlatformNames==nil or table.maxn(RechargePlatformNames)<=0 then
        local tabel = TableReaderManager:getInstance():getTableReader("PlatformName.txt")
        local count = tabel:getLineCount()-1;
        for i = 1,count do
            if RechargePlatformNames[index] == nil then
			local id = tonumber(tabel:getData(i,0))
                RechargePlatformNames[id] = tabel:getData(i,1)
            end        
        end
    end
end

function RechargePageBase:onLargeRecharge( container )
	RegisterLuaPage("LargeRecharge");
	Set_LargeRechargeList( PageInfo.largeRechargeListInfo )
	PageManager.pushPage("LargeRecharge")
	--PageInfo.rechargeListInfo = {}
	--PageInfo.largeRechargeListInfo = {}
end

-------------------------------------------------------------------------
local CommonPage = require("CommonPage");
local RechargePage = CommonPage.newSub(RechargePageBase, thisPageName, option)
RechargePageBase = nil

function RechargePage_showPage(isRecharge)	
	PageInfo.currentPageType = isRecharge == false and PageType.vip or PageType.recharge;
	PageManager.pushPage(thisPageName);
end