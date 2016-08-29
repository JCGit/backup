--[[
	FILE:			MercenaryUpSetPage.lua
	ENCODING:		UTF-8, no-bomb
	DESCRIPTION:	佣兵升星
	AUTHOR:			dmq
	CREATED:		2014-10-18
--]]
------------------------------------------------------------------------
local SkillManager = require "SkillManager" 
local UserInfo = require("UserInfo");
local NodeHelper = require("NodeHelper");
local PageInfo = {
	curMercenaryInfo = {},
	SoulStoneInfo = {},
	SoulStoneIds = {},
	SoulSelectedStone = {},
	curExp = 0,
	curLevel = 0,
	curIndex = 1,
	starStoneTimes = 0,
	starStoneMaxTimes = 0,
	isNeedRefreshAttr = true
}

local ItemManager = require ("ItemManager")

local UserMercenaryManager = require ("UserMercenaryManager")

local ConfigManager = require ("ConfigManager")

local UserItemManager = require ("UserItemManager")

local MercenaryUpStepPageBase = {}

function luaCreat_MercenaryUpStepPage(container)
	container:registerFunctionHandler(MercenaryUpStepPageBase.onFunction);
end


function MercenaryUpStepPageBase.onFunction( eventName,container )
	if eventName == "luaLoad" then
		MercenaryUpStepPageBase.onLoad(container)
	elseif eventName == "luaEnter" then
		MercenaryUpStepPageBase.onEnter(container)
	elseif eventName == "luaExecute" then
		MercenaryUpStepPageBase.onExecute(container)
	elseif eventName == "luaExit" then
		MercenaryUpStepPageBase.onExit(container)
	elseif eventName == "luaGameMessage" then
		MercenaryUpStepPageBase.onReceiveMessage(container)
	elseif eventName == "onHelp" then
		MercenaryUpStepPageBase.onHelp( container )
	elseif eventName == "onClose" then
		MercenaryUpStepPageBase.onClose( container )
	elseif eventName == "onAKeyInto" then
		MercenaryUpStepPageBase.onOneKeyPutIn( container )
	elseif eventName == "onSynthesisBook" then
		PageManager.pushPage("SoulStoneUpgradePage")
	elseif eventName == "onUpgradeImmediately" then
		MercenaryUpStepPageBase.onUpStep( container )
	elseif eventName:sub(1, 6) == "onFeet" then
		MercenaryUpStepPageBase.onCancleGoods( eventName:sub(8) , container )
	elseif eventName:sub(1, 11) == "onStoneFeet" then
		MercenaryUpStepPageBase.onSelectGoods( eventName:sub(13) , container )
	end
end

function MercenaryUpStepPageBase.onLoad( container )
	container:loadCcbiFile("MercenaryUpgradeStarPage.ccbi")
end

function MercenaryUpStepPageBase.onEnter( container )
	MercenaryUpStepPageBase.registerPacket( container )
	container:registerMessage(MSG_MAINFRAME_REFRESH)
	MercenaryUpStepPageBase.initSoulStoneInfo( container )	
	MercenaryUpStepPageBase.refreshPage( container )
end

function MercenaryUpStepPageBase.refreshPage( container )
	UserInfo.sync() 	
	
	PageInfo.starStoneMaxTimes = ConfigManager.getVipCfg()[UserInfo.playerInfo.vipLevel].soulStoneNum
	
	local step =  PageInfo.curMercenaryInfo.starLevel
	
	for i = 1,GameConfig.MercenaryMaxStar,1 do
	    if i == GameConfig.MercenaryMaxStar then
	        container:getVarSprite("mYellowStar" .. i ):setVisible( i <= step )
	    else
	        container:getVarSprite("mYellowStar0" .. i ):setVisible( i <= step )
	    end
	end
	
	--local curExp = PageInfo.curMercenaryInfo.starExp
	
	local previewExp = PageInfo.curMercenaryInfo.starExp
	
	local totExp
		
    if PageInfo.curMercenaryInfo.starLevel >= GameConfig.MercenaryMaxStar then 
        totExp = UserMercenaryManager:getUpStepTableByRoleId( PageInfo.curMercenaryInfo.itemId )[9].exp
        previewExp = tonumber( totExp )	
		container:getVarLabelBMFont("mExperienceValue"):setString( common:getLanguageString( "@MaxStar" ) )
	else
		totExp = UserMercenaryManager:getUpStepTableByRoleId( PageInfo.curMercenaryInfo.itemId )[PageInfo.curMercenaryInfo.starLevel].exp
		container:getVarLabelBMFont("mExperienceValue"):setString( previewExp .. "/" .. totExp )
	end
	
	if PageInfo.curLevel == PageInfo.curMercenaryInfo.starLevel then
		container:getVarScale9Sprite("mBar"):setScaleX( PageInfo.curExp/totExp )
	else
		container:getVarScale9Sprite("mBar"):setScaleX( 0 )
	end
	
	
	container:getVarScale9Sprite("mGreenBar"):setScaleX( previewExp/totExp )
	
	
	
	local _str = common:getLanguageString("@TodaySurplusNum", PageInfo.starStoneMaxTimes - PageInfo.starStoneTimes, PageInfo.starStoneMaxTimes)
	container:getVarLabelBMFont("mTodaySurplusNum"):setString(_str)
	
	MercenaryUpStepPageBase.showOwnStone( container )
	MercenaryUpStepPageBase.showSelectedStone( container )
	
	if PageInfo.isNeedRefreshAttr then
		MercenaryUpStepPageBase.showMercenaryAttribute( container )
		PageInfo.isNeedRefreshAttr = false
	end
    
end

function MercenaryUpStepPageBase.initSoulStoneInfo( container )
	PageInfo.SoulStoneIds = UserItemManager:getItemIdsByType(Const_pb.SOUL_STONE)
	table.sort( PageInfo.SoulStoneIds,function (e1, e2)
		if not e2 then return true end
		if not e1 then return false end
		if e1 > e2 then
			return true
		end
		return false
	end )
	PageInfo.SoulStoneInfo = {}
	for k,v in ipairs(PageInfo.SoulStoneIds) do
		if v ~= 90002 then
			
			userItemInfo = UserItemManager:getUserItemByItemId( v )
			
			PageInfo.SoulStoneInfo[v] = {} -- userItemInfo
			PageInfo.SoulStoneInfo[v].id = userItemInfo.id
			PageInfo.SoulStoneInfo[v].itemId = userItemInfo.itemId
			PageInfo.SoulStoneInfo[v].count = userItemInfo.count
			PageInfo.SoulStoneInfo[v].status = userItemInfo.status
		else
            table.remove(PageInfo.SoulStoneIds , k)	
		end
	end
	PageInfo.curExp = PageInfo.curMercenaryInfo.starExp
	PageInfo.curLevel = PageInfo.curMercenaryInfo.starLevel
	--[[
	table.sort(PageInfo.SoulStoneInfo , function (e1 , e2)
		if not e2 then return true end
		if not e1 then return false end
		if e1.id > e2.id then
			return true
		end
		return false
	end)
	]]--
	UserInfo.sync()
	PageInfo.starStoneTimes = UserInfo.stateInfo.starStoneTimes
	PageInfo.starStoneMaxTimes = ConfigManager.getVipCfg()[soulStoneNum]
end

function MercenaryUpStepPageBase.showMercenaryAttribute( container )
	
	local step = PageInfo.curLevel
	local upStepInfo = UserMercenaryManager:getUpStepTableByRoleId( PageInfo.curMercenaryInfo.itemId )
	
	local previeStep = PageInfo.curMercenaryInfo.starLevel
	if  previeStep <= step and previeStep < GameConfig.MercenaryMaxStar then
		previeStep = step + 1
	end
	local previewUpStepInfo = UserMercenaryManager:getUpStepTableByRoleId( PageInfo.curMercenaryInfo.itemId )
	
	local mHpNumCur = (upStepInfo[step]["hpCoe"])/100 
	local mCritRatingNumCur = (upStepInfo[step]["critCoe"])/100
	local mMpNumCur = (upStepInfo[step]["magicCoe"])/100
	local mDodgeNumCur = (upStepInfo[step]["dodgeCoe"])/100
	local mPhysicsResistanceNumCur = (upStepInfo[step]["phyResistanceCoe"])/100
	local mTenacityNumCur = (upStepInfo[step]["tenacityCoe"])/100
	local mCreateRoleNumCur = (upStepInfo[step]["magResistanceCoe"])/100
	local mMinimumDamageNumCur = (upStepInfo[step]["harmCoe"])/100
	local mHitRatingNumCur = (upStepInfo[step]["hitCoe"])/100 
	local mMaximumDamageNumCur = (upStepInfo[step]["harmCoe"])/100 
	
	local mHpNumNext = (previewUpStepInfo[previeStep]["hpCoe"])/100
	local mCritRatingNumNext = (previewUpStepInfo[previeStep]["critCoe"])/100 
	local mMpNumNext = (previewUpStepInfo[previeStep]["magicCoe"])/100 
	local mDodgeNumNext = (previewUpStepInfo[previeStep]["dodgeCoe"])/100
	local mPhysicsResistanceNumNext = (previewUpStepInfo[previeStep]["phyResistanceCoe"])/100 
	local mTenacityNumNext = (previewUpStepInfo[previeStep]["tenacityCoe"])/100 
	local mCreateRoleNumNext = (previewUpStepInfo[previeStep]["magResistanceCoe"])/100
	local mMinimumDamageNumNext = (previewUpStepInfo[previeStep]["harmCoe"])/100
	local mHitRatingNumNext = (previewUpStepInfo[previeStep]["hitCoe"])/100 
	local mMaximumDamageNumNext = (previewUpStepInfo[previeStep]["harmCoe"])/100 

	local strMap = {
		mHpNum = common:fillHtmlStr("MecenaryUpStepFormat" ,mHpNumCur,mHpNumNext ),
		mCritRatingNum = common:fillHtmlStr("MecenaryUpStepFormat" ,mCritRatingNumCur,mCritRatingNumNext ),
		mMpNum = common:fillHtmlStr("MecenaryUpStepFormat" ,mMpNumCur,mMpNumNext ),
		mDodgeNum = common:fillHtmlStr("MecenaryUpStepFormat" ,mDodgeNumCur,mDodgeNumNext ),
		mPhysicsResistanceNum = common:fillHtmlStr("MecenaryUpStepFormat" ,mPhysicsResistanceNumCur,mPhysicsResistanceNumNext ),
		mTenacityNum = common:fillHtmlStr("MecenaryUpStepFormat" ,mTenacityNumCur,mTenacityNumNext ),
		mCreateRoleNum = common:fillHtmlStr("MecenaryUpStepFormat" ,mCreateRoleNumCur,mCreateRoleNumNext ),
		mMinimumDamage_Node = common:fillHtmlStr("MecenaryUpStepFormat" ,mMinimumDamageNumCur,mMinimumDamageNumNext ),
		mHitRatingNum = common:fillHtmlStr("MecenaryUpStepFormat" ,mHitRatingNumCur,mHitRatingNumNext ),
		mMaximumDamageNum = common:fillHtmlStr("MecenaryUpStepFormat" ,mMaximumDamageNumCur,mMaximumDamageNumNext )
	}
	
	if PageInfo.curMercenaryInfo.starLevel >= GameConfig.MercenaryMaxStar then
		strMap = {}
		strMap = {
			mHpNum = common:fillHtmlStr("MecenaryUpStepFormat1" ,mHpNumNext ) ,
			mCritRatingNum = common:fillHtmlStr("MecenaryUpStepFormat1" ,mCritRatingNumNext),
			mMpNum = common:fillHtmlStr("MecenaryUpStepFormat1" ,mMpNumNext),
			mDodgeNum = common:fillHtmlStr("MecenaryUpStepFormat1" ,mDodgeNumNext),
			mPhysicsResistanceNum = common:fillHtmlStr("MecenaryUpStepFormat1" ,mPhysicsResistanceNumNext),
			mTenacityNum = common:fillHtmlStr("MecenaryUpStepFormat1" ,mTenacityNumNext),
			mCreateRoleNum = common:fillHtmlStr("MecenaryUpStepFormat1" ,mCreateRoleNumNext),
			mMinimumDamage_Node = common:fillHtmlStr("MecenaryUpStepFormat1" ,mMinimumDamageNumNext),
			mHitRatingNum = common:fillHtmlStr("MecenaryUpStepFormat1" ,mHitRatingNumNext),
			mMaximumDamageNum = common:fillHtmlStr("MecenaryUpStepFormat1" ,mMaximumDamageNumNext)
		}
	end
	
	MercenaryUpStepPageBase.addHtmls( container ,strMap )
end

function MercenaryUpStepPageBase.addHtmls( container , strMap )
	
	if strMap == nil then return end
	--[[
	if PageInfo.curMercenaryInfo.starLevel >= 10 then
		NodeHelper:setStringForLabel(  container , strMap )
		return
	end
	]]--
	for nodestr,str in pairs( strMap ) do
		node = container:getVarNode( nodestr )
		if node ~= nil then
			NodeHelper:addHtmlLable( node , str , GameConfig.Tag.HtmlLable ,CCSize(300,100))
		end	
	end
	
end

function MercenaryUpStepPageBase.showOwnStone( container )
	local i = 1		
	for _,itemId in pairs( PageInfo.SoulStoneIds ) do
		local userItemInfo = PageInfo.SoulStoneInfo[itemId]
		local itemInfo = ItemManager:getItemCfgById( itemId )	
		if userItemInfo and itemInfo then
			container:getVarSprite("mStonePic0" .. i):setTexture(itemInfo.icon)
			container:getVarLabelBMFont("mStoneName0" .. i):setVisible(true)
			container:getVarLabelBMFont("mStoneName0" .. i):setString( itemInfo.name )
			container:getVarLabelBMFont("mStoneNum0" .. i):setVisible(true)
			container:getVarLabelBMFont("mStoneNum0" .. i):setString( userItemInfo.count )
			local itemName = "mStoneFeet0" .. i
			NodeHelper:setMenuItemQuality( container , itemName ,itemInfo.quality  )
			i = i + 1
		end
	end		
	
	for j = i,4,1 do
	    container:getVarLabelBMFont("mStoneName0" .. j):setVisible(false)
	    container:getVarSprite( "mStonePic0" .. j ):setTexture(GameConfig.Image.DefaultSoulStone)
		container:getVarLabelBMFont("mStoneNum0" .. j):setVisible(false)
		local itemName = "mStoneFeet0" .. j
		NodeHelper:setMenuItemQuality( container , itemName ,GameConfig.Default.Quality  )
	end
			
end

function MercenaryUpStepPageBase.showSelectedStone( container )
	local i = 1
	for _,itemInfo in pairs( PageInfo.SoulSelectedStone ) do
		container:getVarSprite( "mTextPic0" .. i ):setTexture(itemInfo.icon)
		
		local itemName = "mFeet0" .. i
		NodeHelper:setMenuItemQuality( container , itemName ,itemInfo.quality  )
		
		i = i + 1
	end
	
	for j = i,5,1 do
	    
	    container:getVarSprite( "mTextPic0" .. j ):setTexture(GameConfig.Image.EmptySoulStone)
		local itemName = "mFeet0" .. j
		NodeHelper:setMenuItemQuality( container , itemName ,GameConfig.Default.Quality  )
	end
	
end

function MercenaryUpStepPageBase.onExecute( container )

end

function MercenaryUpStepPageBase.onExit( container )
	MercenaryUpStepPageBase.removePacket( container )
	container:removeMessage(MSG_MAINFRAME_REFRESH)
	MercenaryUpStepPageBase.clearData( container )
end

function MercenaryUpStepPageBase.onHelp( container )
	PageManager.showHelp(GameConfig.HelpKey.HELP_MERCENARYUPSTAR)
end

function MercenaryUpStepPageBase.clearData( container )
	
	PageInfo.curMercenaryInfo = {}
	PageInfo.SoulStoneInfo = {}
	PageInfo.SoulStoneIds = {}
	PageInfo.SoulSelectedStone = {}
	PageInfo.curExp = 0
	PageInfo.curLevel = 0
	PageInfo.starStoneTimes = 0
	PageInfo.starStoneMaxTimes = 0
	PageInfo.isNeedRefreshAttr = true
end

function MercenaryUpStepPageBase.onClose( container )
	PageManager.popPage("MercenaryUpStepPage")
end

function MercenaryUpStepPageBase.onOneKeyPutIn( container )
    if PageInfo.curMercenaryInfo.starLevel >= GameConfig.MercenaryMaxStar then 
		MessageBoxPage:Msg_Box_Lan("@MaxStar") 
		return 
	end
	if #PageInfo.SoulSelectedStone >=5 then return end
	if PageInfo.starStoneTimes >= PageInfo.starStoneMaxTimes then
		MessageBoxPage:Msg_Box_Lan("@PromptTimesOut")
		return
	end

	local max = 0
	if PageInfo.starStoneMaxTimes - PageInfo.starStoneTimes > 5 then
		max = 5
	else
		max = PageInfo.starStoneMaxTimes - PageInfo.starStoneTimes
	end
	local _num = #PageInfo.SoulSelectedStone
	for i = 1, _num , 1 do
		MercenaryUpStepPageBase.cancelOneStone( index )
	end
	local removeIds = {}
	for k,v in pairs( PageInfo.SoulStoneIds ) do
		local num = PageInfo.SoulStoneInfo[v].count
		for i = 1, num ,1 do
			local index = #PageInfo.SoulSelectedStone + 1
			--table.insert(PageInfo.SoulSelectedStone , ItemManager:getItemCfgById( v ) )
			PageInfo.SoulStoneInfo[v].count = PageInfo.SoulStoneInfo[v].count - 1
			if PageInfo.SoulStoneInfo[v].count == 0 then
			    table.insert( removeIds , k - #removeIds ) --在remove时 每remove掉一个 其后元素会前移
				table.remove( PageInfo.SoulStoneInfo , v )
			end
			table.insert( PageInfo.SoulSelectedStone , ItemManager:getItemCfgById( v ) )
			local stoneItemInfo = UserMercenaryManager:getUpStepTableByRoleId( PageInfo.curMercenaryInfo.itemId )	 
			local totExp = stoneItemInfo[PageInfo.curMercenaryInfo.starLevel].exp
			PageInfo.curMercenaryInfo.starExp = PageInfo.curMercenaryInfo.starExp + ItemManager:getItemCfgById( v ).soulStoneExp
			while PageInfo.curMercenaryInfo.starExp >= tonumber(totExp) do		
				PageInfo.curMercenaryInfo.starLevel = PageInfo.curMercenaryInfo.starLevel +1
				PageInfo.isNeedRefreshAttr = true
				PageInfo.curMercenaryInfo.starExp = PageInfo.curMercenaryInfo.starExp -totExp
				if PageInfo.curMercenaryInfo.starLevel >= GameConfig.MercenaryMaxStar then 
                    --totExp = stoneItemInfo[9].exp
                    --PageInfo.curMercenaryInfo.starExp = tonumber( totExp )
                    break
                end	
                totExp = stoneItemInfo[PageInfo.curMercenaryInfo.starLevel].exp
			end	
			PageInfo.SoulSelectedStone[index] = ItemManager:getItemCfgById( v )
			PageInfo.starStoneTimes = PageInfo.starStoneTimes + 1
			if PageInfo.curMercenaryInfo.starLevel >= GameConfig.MercenaryMaxStar then break	end
			if #PageInfo.SoulSelectedStone >= max then break end
		end
		if PageInfo.curMercenaryInfo.starLevel >= GameConfig.MercenaryMaxStar then break	end
		if #PageInfo.SoulSelectedStone >= max then break end
	end
	for k,v in ipairs( removeIds ) do
	    table.remove( PageInfo.SoulStoneIds ,v)
	end
	MercenaryUpStepPageBase.refreshPage( container )
	
end

function MercenaryUpStepPageBase.onUpStep( container )
	local msg = Player_pb.HPRoleIncStarExp()
	msg.roleId = PageInfo.curMercenaryInfo.roleId	
	for _,v in pairs( PageInfo.SoulSelectedStone ) do
		msg.itemId:append( v.id )
	end
	common:sendPacket(HP_pb.ROLE_INC_STAR_EXP_C, msg ,false)
end

function MercenaryUpStepPageBase.onSelectGoods( eventIndex , container )
    --PageInfo.starStoneMaxTimes = 1000
	if PageInfo.starStoneTimes >= PageInfo.starStoneMaxTimes then
		MessageBoxPage:Msg_Box_Lan("@PromptTimesOut")
		return
	end
	if table.maxn( PageInfo.SoulSelectedStone ) >= 5 then return end
	MercenaryUpStepPageBase.addOneStone( eventIndex )
	MercenaryUpStepPageBase.refreshPage( container )
end

function MercenaryUpStepPageBase.addOneStone( eventIndex )
    if PageInfo.curMercenaryInfo.starLevel >= GameConfig.MercenaryMaxStar then 
		MessageBoxPage:Msg_Box_Lan("@MaxStar") 
		return 
	end
	if #PageInfo.SoulSelectedStone >=5 then return end
	local id = PageInfo.SoulStoneIds[tonumber(eventIndex)]	
    if PageInfo.SoulStoneInfo[id] == nil then
        return
    end	
	PageInfo.SoulStoneInfo[id].count = PageInfo.SoulStoneInfo[id].count - 1
	if PageInfo.SoulStoneInfo[id].count == 0 then
		table.remove(PageInfo.SoulStoneIds , tonumber(eventIndex) )
		table.remove( PageInfo.SoulStoneInfo , id )
	end
	table.insert( PageInfo.SoulSelectedStone , ItemManager:getItemCfgById( id ) )
	local stoneItemInfo = UserMercenaryManager:getUpStepTableByRoleId( PageInfo.curMercenaryInfo.itemId )	 
	local totExp = stoneItemInfo[PageInfo.curMercenaryInfo.starLevel].exp
	PageInfo.curMercenaryInfo.starExp = PageInfo.curMercenaryInfo.starExp + ItemManager:getItemCfgById( id ).soulStoneExp
	while PageInfo.curMercenaryInfo.starExp >= tonumber(totExp) do
		PageInfo.curMercenaryInfo.starLevel = PageInfo.curMercenaryInfo.starLevel +1
		PageInfo.isNeedRefreshAttr = true
		PageInfo.curMercenaryInfo.starExp = PageInfo.curMercenaryInfo.starExp -totExp
		totExp = stoneItemInfo[PageInfo.curMercenaryInfo.starLevel].exp
		if PageInfo.curMercenaryInfo.starLevel >= GameConfig.MercenaryMaxStar then 
            --totExp = stoneItemInfo[9].exp
            --PageInfo.curMercenaryInfo.starExp = tonumber( totExp )
            break
        end	
	end	
	PageInfo.starStoneTimes = PageInfo.starStoneTimes + 1
end

function MercenaryUpStepPageBase.onCancleGoods( eventIndex , container )
	MercenaryUpStepPageBase.cancelOneStone( eventIndex )
	MercenaryUpStepPageBase.refreshPage( container )
end

function MercenaryUpStepPageBase.cancelOneStone( eventIndex )
	local itemInfo = PageInfo.SoulSelectedStone[tonumber(eventIndex)]
	if itemInfo == nil then return end
	local hasStone = false
	table.foreachi(PageInfo.SoulStoneIds, function(i, v)
			if v == itemInfo.id then
				hasStone = true
			end
		end)
	
	if not hasStone then
		table.insert( PageInfo.SoulStoneIds , itemInfo.id )
		table.sort( PageInfo.SoulStoneIds ,  function (e1, e2)
			if not e2 then return true end
			if not e1 then return false end
			if e1 > e2 then return true	end
			return false
		end)
	end	
	
	PageInfo.SoulStoneInfo[itemInfo.id].count = PageInfo.SoulStoneInfo[itemInfo.id].count + 1
		 
	local stoneItemInfo = UserMercenaryManager:getUpStepTableByRoleId( PageInfo.curMercenaryInfo.itemId )	 
	local totExp = stoneItemInfo[PageInfo.curMercenaryInfo.starLevel].exp
	PageInfo.curMercenaryInfo.starExp = PageInfo.curMercenaryInfo.starExp - ItemManager:getItemCfgById( itemInfo.id ).soulStoneExp
	while PageInfo.curMercenaryInfo.starExp < 0  do
		PageInfo.curMercenaryInfo.starLevel = PageInfo.curMercenaryInfo.starLevel -1
		PageInfo.isNeedRefreshAttr = true
		totExp = stoneItemInfo[PageInfo.curMercenaryInfo.starLevel].exp
		PageInfo.curMercenaryInfo.starExp = PageInfo.curMercenaryInfo.starExp + totExp
	end
	
	table.remove( PageInfo.SoulSelectedStone , tonumber(eventIndex) )
	PageInfo.starStoneTimes = PageInfo.starStoneTimes - 1
end

function MercenaryUpStepPageBase:onReceivePacket( container )
	local opcode = container:getRecPacketOpcode()
	local msgBuff = container:getRecPacketBuffer()
	
	if opcode == HP_pb.ROLE_INC_STAR_EXP_S then
		local msg = Recharge_pb.HPShopListSync()
		msg:ParseFromString(msgBuff)
		MercenaryUpStepPageBase.handleUpStep( container ,msg )
	end
end

function MercenaryUpStepPageBase.handleUpStep( container ,msg )
	local roleId = msg.roleId
	local starLevel = msg.starLevel
	local starExp = msg.starExp
end	

function MercenaryUpStepPageBase.onReceiveMessage(container)
	local message = container:getMessage()
	local typeId = message:getTypeId()
	if typeId == MSG_MAINFRAME_REFRESH then
		local pageName = MsgMainFrameRefreshPage:getTrueType(message).pageName;
		if pageName == "MercenaryUpStepPage" then
			MercenaryUpStepPageBase.clearData(container)
			
			MercenaryUpStepPageBase.getSyncMercenaryInfoByIndex( PageInfo.curIndex ) 
			
			PageInfo.curLevel = PageInfo.curMercenaryInfo.starLevel
			PageInfo.isNeedRefreshAttr = true
			
			
			MercenaryUpStepPageBase.initSoulStoneInfo( container )
			MercenaryUpStepPageBase.refreshPage( container )
			
			SkillManager:refreshMerSkillListBy(  PageInfo.curMercenaryInfo.roleId ,PageInfo.curMercenaryInfo.starLevel )
			
			PageManager.refreshPage("MercenaryPage_RefreshSkill")
		end
	end
end

function MercenaryUpStepPageBase.getSyncMercenaryInfoByIndex( index ) 
	local mercenaryStr = ServerDateManager:getInstance():getRoleInfoByIndexForLua(index)
	local data = Player_pb.RoleInfo()
	data:ParseFromString(mercenaryStr)
	PageInfo.curMercenaryInfo = data
end

function MercenaryUpStepPageBase.registerPacket(container)
	--container:registerPacket(HP_pb.ROLE_INC_STAR_EXP_S)	
end

function MercenaryUpStepPageBase.removePacket(container)
	--container:removePacket(HP_pb.ROLE_INC_STAR_EXP_S)
end

function MercenaryUpStepPage_SetMercenary(curIndex)
	MercenaryUpStepPageBase.getSyncMercenaryInfoByIndex( curIndex ) 
	PageInfo.curIndex = curIndex
end