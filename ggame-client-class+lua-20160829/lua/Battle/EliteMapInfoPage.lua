----------------------------------------------------------------------------------
--[[
FILE:			EliteMapInfoPage.lua
ENCODING:		UTF-8, no-bomb
DESCRIPTION:	精英副本二级页面，可滑动页面
AUTHOR:			zhenhui
CREATED:		2014-10-16
--]]
----------------------------------------------------------------------------------


local thisPageName = "EliteMapInfoPage"

local EliteMapManager = require("EliteMapManager")
local UserInfo = require("UserInfo")
local EliteMapCfg = ConfigManager.getEliteMapCfg();
local option = {
ccbiFile = "EliteMapInfoPage.ccbi",
handlerMap = {
onReturn = "onReturn",
onHelp = "onHelp",
onArrowLeft = "onArrowLeft",
onArrowRight = "onArrowRight",
}
}

local EliteMapInfoPageBase = {}


local m_BegainX = 0
local m_EndX = 0
local totalSize = 0
local NodeHelper = require("NodeHelper");
local TodoStr = "99";
local TodoImage = "UI/MainScene/UI/u_ico000.png";
local TodoPoster = "person/poster_char_Boss.png";

local EliteMapItem = {
ccbiFile 	= "EliteMapInfoContentItem.ccbi"
}

local fOneItemWidth = 0
local fScrollViewWidth = 0
local m_currentIndex = 0

local svMaxHeight = 600
local svMinHeight = 300

local mScrollViewRef = {}
local mContainerRef = {}

function EliteMapItem.onRefreshItemView(container)
	local level = container.id
	local index = container:getTag();
	
	local mapId = EliteMapManager:getMapIdByLevelAndIndex(level,index)
	CCLuaLog("EliteMapItem.onRefreshItemView"..level..index..mapId)	
	
	
	NodeHelper:setStringForLabel(container,{
	mLvNUm = "Lv"..EliteMapManager:getMonsterLevelById(mapId),
	mName = EliteMapManager:getNameById(mapId)
	})
	CCLuaLog("EliteMapItem.onRefreshItemView"..level..index.."mapid"..mapId)
	NodeHelper:setSpriteImage(container,{
	mPic = EliteMapManager:getMonsterPic(mapId)
	})	
	
	NodeHelper:setQualityFrames(container, {mHand = EliteMapManager:getQualityById(mapId)});
		
	local dependMap = EliteMapManager:getDependMapId(mapId)	
	
	local menuHand = container:getVarMenuItemImage("mHand")
	local lockNode = container:getVarNode("mLock")
	
	local curLevelLimit = EliteMapManager:getLevelById(mapId)
	local curPassedMap = EliteMapManager:getPassedMapIdByLevel(curLevelLimit)
	if menuHand~=nil then
		if tonumber(dependMap) > curPassedMap   then
			menuHand:setEnabled(false)
		else
			menuHand:setEnabled(true)
		end
	end
	CCLuaLog("dependMap = "..dependMap .. "curPassedMap"..curPassedMap)
	if lockNode~=nil then
		if tonumber(dependMap) > curPassedMap   then
			lockNode:setVisible(true)
		else
			lockNode:setVisible(false)
		end
	end
end

function EliteMapItem.onHand(container)
	local level = container.id
	local index = container:getTag();
	local mapId = EliteMapManager:getMapIdByLevelAndIndex(level,index)
	RegisterLuaPage("EliteMapRewardPopup")
	EliteMapRewardPopup_setMapId(mapId)
	PageManager.pushPage("EliteMapRewardPopup")
	--MessageBoxPage:Msg_Box("clicked the hand"..level..index)
end

function EliteMapItem.onFunction(eventName, container)
	if eventName == "luaInitItemView" then
		EliteMapItem.onRefreshItemView(container)
	elseif eventName == "onHand" then
		EliteMapItem.onHand(container)
	end
end
----------------------------------------------------------------------------------

local EliteMapInfoContent = {
ccbiFile 	= "EliteMapInfoContent.ccbi"
}

function EliteMapInfoContent.onRefreshItemView(container)
	local levelId = tonumber(container:getItemDate().mID)
	for i=1,9 do
		local nodeStr = "mCareerNode0"..i
		local monsterItem = CCBManager:getInstance():createAndLoad2("EliteMapInfoContentItem.ccbi")
		monsterItem.id = levelId
		monsterItem:setTag(i);
		monsterItem:registerFunctionHandler(EliteMapItem.onFunction)
		monsterItem.__CCReViSvItemNodeFacade__:initItemView();
		local oneNode = container:getVarNode(nodeStr)
		if oneNode == nil then
			assert(false,"error in node "..nodeStr)
		end
		oneNode:removeAllChildren()
		--monsterItem:release();
		oneNode:addChild(monsterItem)
	end
	
	
end

function EliteMapInfoContent.onFunction(eventName, container)
	if eventName == "luaRefreshItemView" then
		EliteMapInfoContent.onRefreshItemView(container)
	elseif eventName == "onChanllage" then
		EliteMapInfoContent.onBoss(container)
	elseif eventName == "onMap" then
		EliteMapInfoContent.onMap(container)
	end
end
----------


-------------------------------------
--EliteMapInfoPageBase页面中的事件处理
----------------------------------------------
function EliteMapInfoPageBase:onEnter(container)
	
	container:registerMessage(MSG_MAINFRAME_REFRESH)
	
	NodeHelper:initScrollView(container, "mContent", 15);
	container.scrollview=container:getVarScrollView("mContent");
	mScrollViewRef = container.scrollview
	mContainerRef = container
	local chatSVSize = container.scrollview:getViewSize();
	local chatSVWorldPos = container.scrollview:getParent():convertToWorldSpaceAR(ccp(container.scrollview:getPositionX(),container.scrollview:getPositionY()))
	svMinHeight = chatSVWorldPos.y - chatSVSize.height
	svMaxHeight = chatSVWorldPos.y
	
	EliteMapManager:syncPaseedMapId()
	container.scrollview:setTouchEnabled(true)
	container.scrollview:setBounceable(false)
	fScrollViewWidth = container.scrollview:getViewSize().width
	local touchLayer = container.scrollview:getParent():getChildByTag(100302);
	if not touchLayer then
		touchLayer = CCLayer:create();
		touchLayer:setTag(100302);
		container.scrollview:getParent():addChild(touchLayer);
		touchLayer:setTouchEnabled(true)
		touchLayer:setContentSize(CCSize(container.scrollview:getViewSize().width,container.scrollview:getViewSize().height))
		touchLayer:setPosition(container.scrollview:getPosition());
		touchLayer:registerScriptTouchHandler(EliteMapInfoPageBase.onTouchHandler,false,0,false);
	end		
	self:rebuildAllItem(container);
	self:refreshPage(container);
end

function EliteMapInfoPageBase.onTouchHandler(eventName,pTouch)
	
	if eventName == "began" then
		local point = pTouch:getLocationInView();
		point = CCDirector:sharedDirector():convertToGL(point);
		m_BegainX = point.x;
	elseif eventName == "moved" then
		local point = pTouch:getLocationInView();
		point = CCDirector:sharedDirector():convertToGL(point);
		m_EndX = point.x
		if m_EndX>640 or m_EndX<0 then
			EliteMapInfoPageBase:ResetToCur()
		end
	elseif eventName == "ended" then
		local point = pTouch:getLocationInView();
		point = CCDirector:sharedDirector():convertToGL(point);
		m_EndX = point.x
		if m_EndX>640 or m_EndX<0 or point.y<svMinHeight or point.y>svMaxHeight  then
			return
		end
		local dis = m_EndX-m_BegainX
		CCLuaLog("m_EndX-m_BegainX"..m_EndX.."m_BegainX"..m_BegainX.."dis.."..dis)
		if dis>200 then
			return EliteMapInfoPageBase:MoveToLeft()
		elseif dis<-200 then
			return EliteMapInfoPageBase:MoveToRight()
		else
			return EliteMapInfoPageBase:ResetToCur()
		end
	elseif eventName == "cancelled" then
		EliteMapInfoPageBase:ResetToCur()
	end
	
end

function EliteMapInfoPageBase:MoveToIndex(index)
	m_currentIndex = index
	--local index = EliteMapInfoPageBase:calNewIndexBycurOffset(curOffset);
	local newOffset = EliteMapInfoPageBase:calOffsetByIndex(index)
	CCLuaLog("newOffset"..newOffset.."index = "..index)
	local array = CCArray:create();	
	array:addObject(CCDelayTime:create(0.1));
	local functionAction = CCCallFunc:create(function ()
	    mScrollViewRef:getContainer():stopAllActions()
		mScrollViewRef:setContentOffsetInDuration(ccp(newOffset, mScrollViewRef:getContentOffset().y),0.2);
		self:refreshPage(mContainerRef)
	end)
	array:addObject(functionAction);
	local seq = CCSequence:create(array);	
	mScrollViewRef:runAction(seq)	
end

function EliteMapInfoPageBase:MoveToLeft()
	newIndex = m_currentIndex - 1
	newIndex = math.max(newIndex,1)
	newIndex = math.min(newIndex,totalSize)
	EliteMapInfoPageBase:MoveToIndex(newIndex)
end

function EliteMapInfoPageBase:MoveToRight()
	newIndex = m_currentIndex + 1
	newIndex = math.max(newIndex,1)
	newIndex = math.min(newIndex,totalSize)
	EliteMapInfoPageBase:MoveToIndex(newIndex)
end

function EliteMapInfoPageBase:ResetToCur()
	EliteMapInfoPageBase:MoveToIndex(m_currentIndex)
end

function EliteMapInfoPageBase:calNewIndexBycurOffset(curOffset)
	local index = math.floor(2 - curOffset / fOneItemWidth)
	return index
end

function EliteMapInfoPageBase:calOffsetByIndex(index)
	local offset = fScrollViewWidth - (index ) * fOneItemWidth
	return offset
end

function EliteMapInfoPageBase:onExecute(container)
	
end

function EliteMapInfoPageBase:onExit(container)
	
end


----------------------------------------------------------------

function EliteMapInfoPageBase:refreshPage(container)
--[[	CCLuaLog(tostring(UserInfo.roleInfo.prof ))
	CCLuaLog(tostring(EliteMapManager.stageName))
	CCLuaLog(tostring(EliteMapManager.stageLevel))--]]
	--[[local temp = EliteMapManager.stageName
	temp = EliteMapManager.stageLevel	
	assert((  EliteMapManager.stageName[UserInfo.roleInfo.prof] == nil 
		),"Error in refresh page in m_currentIndex" )
	assert((EliteMapManager.stageName[UserInfo.roleInfo.prof] == nil ),"Error in refresh page in m_currentIndex" )--]]
			
	local stageName = EliteMapManager.stageName[UserInfo.roleInfo.prof][(m_currentIndex)]			
	local stageLevel = EliteMapManager.stageLevel[UserInfo.roleInfo.prof][(m_currentIndex)]	
	CCLuaLog("EliteMapInfoPageBase:refreshPage---stageName"..stageName .. "stageLevel"..stageLevel)
	NodeHelper:setStringForLabel(container,{
		mCareerName = (stageName)
	})			
	
	
	if UserInfo.roleInfo.prof == 1 then 
		NodeHelper:setNodesVisible(container,{
			mEliteMapWarriorNode = true,
			mEliteMapHunterNode = false,
			mEliteMapMasterNode = false,
		})
	elseif UserInfo.roleInfo.prof == 2 then 
		NodeHelper:setNodesVisible(container,{
			mEliteMapWarriorNode = false,
			mEliteMapHunterNode = true,
			mEliteMapMasterNode = false,
		})
	elseif UserInfo.roleInfo.prof == 3 then 
		NodeHelper:setNodesVisible(container,{
			mEliteMapWarriorNode = false,
			mEliteMapHunterNode = false,
			mEliteMapMasterNode = true,
		})
	end
	
	if m_currentIndex == 1 then
		NodeHelper:setNodesVisible(container,{mArrowLeft = false})
	else
		NodeHelper:setNodesVisible(container,{mArrowLeft = true})
	end
	
	if m_currentIndex == totalSize then
		NodeHelper:setNodesVisible(container,{mArrowRight = false})
	else
		NodeHelper:setNodesVisible(container,{mArrowRight = true})
	end
end

----------------scrollview-------------------------
function EliteMapInfoPageBase:rebuildAllItem(container)
	self:clearAllItem(container);
	self:buildItem(container);
end

function EliteMapInfoPageBase:clearAllItem(container)
	container.m_pScrollViewFacade:clearAllItems();
	container.mScrollViewRootNode:removeAllChildren();
end

function EliteMapInfoPageBase:buildItem(container)
	local size = EliteMapManager.HGroupSize;
	
	local ccbiFile = EliteMapInfoContent.ccbiFile
	CCLuaLog(tostring(EliteMapManager.HGroup))
	if size == 0 or ccbiFile == nil or ccbiFile == ''then return end
	local iMaxNode = container.m_pScrollViewFacade:getMaxDynamicControledItemViewsNum()
	local iCount = 0
	local fOneItemHeight = 0
	local currentPos = 0
	
	local nextAvailableLevel = EliteMapManager:getNextFightLevelIndex()
	totalSize = math.min(size,nextAvailableLevel)
	--totolSize = 6
	for i= 1, totalSize do
		local pItemData = CCReViSvItemData:new_local()		
		pItemData.mID = i
		pItemData.m_iIdx = i
		pItemData.m_ptPosition = ccp(currentPos, 0)
		
		if iCount < iMaxNode then
			ccbiFile = EliteMapInfoContent.ccbiFile
			local pItem = CCBManager:getInstance():createAndLoad2(ccbiFile)
			--pItem:release();
			pItem.id = iCount
			pItem:registerFunctionHandler(EliteMapInfoContent.onFunction)
			fOneItemHeight = pItem:getContentSize().height
			
			if fOneItemWidth < pItem:getContentSize().width then
				fOneItemWidth = pItem:getContentSize().width
			end
			currentPos = currentPos + fOneItemWidth
			container.m_pScrollViewFacade:addItem(pItemData, pItem.__CCReViSvItemNodeFacade__)
		else
			container.m_pScrollViewFacade:addItem(pItemData)
		end
		iCount = iCount + 1
	end
	
	m_currentIndex = EliteMapManager:getCurFightLevelIndex()
	local size = CCSizeMake(currentPos, fOneItemHeight)
	container.mScrollView:setContentSize(size)
	local newOffset = EliteMapInfoPageBase:calOffsetByIndex(m_currentIndex)
	container.mScrollView:setContentOffset(ccp( newOffset, 0))	
	
	container.m_pScrollViewFacade:setDynamicItemsStartPosition(iCount - 1);
	container.mScrollView:forceRecaculateChildren();
end



----------------click event------------------------
function EliteMapInfoPageBase:onHelp(container)
	PageManager.showHelp(GameConfig.HelpKey.HELP_ELITEMAP)
end

function EliteMapInfoPageBase:onReturn(container)
	--PageManager.changePage("EliteMapPage");
	PageManager.showFightPage()
end

function EliteMapInfoPageBase:onArrowRight(container)
	return EliteMapInfoPageBase:MoveToRight()
end

function EliteMapInfoPageBase:onArrowLeft(container)
	return EliteMapInfoPageBase:MoveToLeft()
end


-------------------------------------------------------------------------
local CommonPage = require("CommonPage");
local EliteMapInfoPage = CommonPage.newSub(EliteMapInfoPageBase, thisPageName, option);


