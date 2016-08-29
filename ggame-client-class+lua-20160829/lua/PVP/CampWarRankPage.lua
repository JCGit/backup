----------------------------------------------------------------------------------
--[[
FILE:			CampWarRankPage.lua
ENCODING:		UTF-8, no-bomb
DESCRIPTION:	阵营战排行
AUTHOR:			zhenhui
CREATED:		2014-09-19
--]]
----------------------------------------------------------------------------------
local CampWar_pb = require "CampWar_pb"
local Const_pb = require "Const_pb"
local HP_pb = require "HP_pb"
local GameConfig = require "GameConfig"
local thisPageName = "CampWarRankPage"



local option = {
	ccbiFile = "CampWarRankingPopUp.ccbi",
	handlerMap = {	
		onClose = "onClose"
	},
	opcodes = {
		CampWarRank_LIST_S = HP_pb.CampWarRank_LIST_S
	}
}

local CampWarRankPageBase = {}

local NodeHelper = require("NodeHelper");
local CampWarManager = require("CampWarManager")

--------------------------------------------------------------
local CampWarRankItem = {
ccbiFile 	= "CampWarRankingContent.ccbi"
}


function CampWarRankItem.onFunction(eventName, container)
	if eventName == "luaRefreshItemView" then
		CampWarRankItem.onRefreshItemView(container)
	elseif eventName == "onViewDetail" then
		CampWarRankItem.onViewDetail(container)	
	end
end

function CampWarRankItem.isInTable(tabel,id)
	for _,v in ipairs(tabel) do
		if v == id then
			return true;
		end
	end
	return false;
end


function CampWarRankItem.removeItemFromTable(tabel,id)
	for k,v in ipairs(tabel) do
		if v == id then
			table.remove(tabel,k);
			break
		end
	end
end


function CampWarRankItem.onRefreshItemView(container)
	local CampWarRankId = tonumber(container:getItemDate().mID)
	local CampWarRankInfo = CampWarManager.m_lastCampWarInfo.rankInfo[CampWarRankId]
	NodeHelper:setStringForLabel(container,{
		mRankNum = tostring(CampWarRankInfo.rank),
		mName = tostring(CampWarRankInfo.playerName),
		mKillNum = tostring(CampWarRankInfo.multiKillQty),
		mPrestigeNum = tostring(CampWarRankInfo.reputation)
	})
	
	local color = GameConfig.ColorMap.COLOR_DESCRIPTION_BLUE
	if CampWarRankInfo.campId == 1 then
		color = GameConfig.ColorMap.COLOR_DESCRIPTION_BLUE
	else
		color = GameConfig.ColorMap.COLOR_RED
	end
	NodeHelper:setColorForLabel( container, {
		mName = color
	} )
		
end		


function CampWarRankItem.onViewDetail(container)
	local CampWarRankId = tonumber(container:getItemDate().mID)
	local CampWarRankInfo = CampWarManager.m_lastCampWarInfo.rankInfo[CampWarRankId]
	if CampWarRankInfo== nil or CampWarRankInfo.playerName == nil then
		CCLuaLog("ERROR in CampWarRank id, not found in CampWarManager.m_lastCampWarInfo.rankInfo"..CampWarRankId);
		return;
	end
	PageManager.viewPlayerInfo(CampWarRankInfo.playerId, true);
end

----------------------------------------------------------------------------------

-----------------------------------------------
--CampWarRankPageBase页面中的事件处理
----------------------------------------------
function CampWarRankPageBase:onLoad(container)
	container:loadCcbiFile(option.ccbiFile)
	container.scrollview=container:getVarScrollView("mContent");				
end

function CampWarRankPageBase:onEnter(container)
	
	NodeHelper:initScrollView(container, "mContent", 10);	
	self:refreshPage(container);
	self:rebuildAllItem(container)		
		
end

function CampWarRankPageBase:onClose( container )
	PageManager.popPage(thisPageName)
end	

function CampWarRankPageBase:onExit(container)
	--self:removePacket(container)
	--container:removeMessage(MSG_MAINFRAME_REFRESH)
	NodeHelper:deleteScrollView(container);	
end
----------------------------------------------------------------

function CampWarRankPageBase:refreshPage(container)
	--哪方胜利了
	if CampWarManager.m_lastCampWarInfo.winCampId == 1 then
		NodeHelper:setNodesVisible(container,{mBlueWinNode = true,mRedWinNode = false})
	else
		NodeHelper:setNodesVisible(container,{mBlueWinNode = false,mRedWinNode = true})
	end
	
	--挂机获得的声望，金钱
	local reputationStr = common:getLanguageString("@CampWarRankLastRep",tostring(CampWarManager.m_lastCampWarInfo.hangReputation))
	local coinStr = common:getLanguageString("@CampWarRankLastCoin",tostring(CampWarManager.m_lastCampWarInfo.hangGold))
	NodeHelper:setStringForLabel(container,{mReputationLabel = reputationStr,
	mCoinLabel = coinStr})
end


----------------scrollview-------------------------
function CampWarRankPageBase:rebuildAllItem(container)
	self:clearAllItem(container);
	self:buildItem(container);
end

function CampWarRankPageBase:clearAllItem(container)
	container.m_pScrollViewFacade:clearAllItems();
	container.mScrollViewRootNode:removeAllChildren();
end

function CampWarRankPageBase:buildItem(container)
	
	--All the CampWarRank's list
	local CampWarRankSize = #CampWarManager.m_lastCampWarInfo.rankInfo;
	
	
	local ccbiFile = CampWarRankItem.ccbiFile	
	if CampWarRankSize <= 0 or ccbiFile == nil or ccbiFile == ''then return end
	local iMaxNode = container.m_pScrollViewFacade:getMaxDynamicControledItemViewsNum()
	local iCount = 0
	local fOneItemHeight = 0
	local fOneItemWidth = 0	
	--totolSize = 6
	for i=CampWarRankSize, 1,-1 do
		local pItemData = CCReViSvItemData:new_local()		
		pItemData.mID = i
		pItemData.m_iIdx = i
		pItemData.m_ptPosition = ccp(0, fOneItemHeight * iCount)
		
		if iCount < iMaxNode then
			local pItem = ScriptContentBase:create(ccbiFile)			
			pItem.id = iCount
			pItem:registerFunctionHandler(CampWarRankItem.onFunction)
			if fOneItemHeight < pItem:getContentSize().height then
				fOneItemHeight = pItem:getContentSize().height
			end
			
			if fOneItemWidth < pItem:getContentSize().width then
				fOneItemWidth = pItem:getContentSize().width
			end
			container.m_pScrollViewFacade:addItem(pItemData, pItem.__CCReViSvItemNodeFacade__)
		else
			container.m_pScrollViewFacade:addItem(pItemData)
		end
		iCount = iCount + 1
	end
	
	local size = CCSizeMake(fOneItemWidth, fOneItemHeight * iCount)
	container.mScrollView:setContentSize(size)	
	container.mScrollView:setContentOffset(ccp(0, container.mScrollView:getViewSize().height - container.mScrollView:getContentSize().height * container.mScrollView:getScaleY()))	
	
	container.m_pScrollViewFacade:setDynamicItemsStartPosition(iCount - 1);
	container.mScrollView:forceRecaculateChildren();
end

----------------click event------------------------
function CampWarRankPageBase:onClose(container)
	PageManager.popPage(thisPageName);
end


-------------------------------------------------------------------------
local CommonPage = require("CommonPage");
local CampWarRankPage = CommonPage.newSub(CampWarRankPageBase, thisPageName, option);