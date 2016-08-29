----------------------------------------------------------------------------------
--[[
	FILE:			PlayerTitlePage.lua
	ENCODING:		UTF-8, no-bomb
	DESCRIPTION:	玩家称号
	AUTHOR:			zhaolu
	CREATED:		2014-9-18
--]]
----------------------------------------------------------------------------------

----------------local data--------------------------------
local NodeHelper = require("NodeHelper")
local titleManager = require("TitleManager")
local thisPageName = 'PlayerTitlePage'
local playerTitleInfo = {} 		--title info for order
local PlayerTitleBase = {}
local nowTitleId = 0 	  	 	--my own choose title for show
local PlayerTitleContainer = nil
local PlayerNowTitleItemContainer = nil

local option = {
	ccbiFile = "ChangeTitlePopUp.ccbi",
	handlerMap = {
		onPreservation 	= "onPreservation",
		onCancel 		= "onClose",
		onClose 		= "onClose",
		onHelp  		= "onHelp",
	}
}
----------------local data end-----------------------------

function PlayerTitleBase:onEnter( container )
	nowTitleId = titleManager.myNowTitleId
	-- 显示排序的玩家称号
	playerTitleInfo = titleManager:orderTitle()

	-- 已达成多少个，未达成多少个
	local headlineStr = common:getLanguageString("@ChangeTitleHeadline",#titleManager.myOwnTitleIds,(#playerTitleInfo-#titleManager.myOwnTitleIds))
	NodeHelper:setStringForLabel(container, {mTitleCompleteNum = headlineStr})

	PlayerTitleContainer = container
	NodeHelper:initScrollView(container, "mContent", 6);

	self:rebuildAllItem(container)
end

function PlayerTitleBase:onExit(container)
	nowTitleId = 0
	playerTitleInfo = {}
	PlayerTitleContainer = nil
	PlayerNowTitleItemContainer = nil
end

function PlayerTitleBase:onPreservation( container )
	titleManager:setMyNowTitle(nowTitleId)
	PageManager.popPage(thisPageName)
end

function PlayerTitleBase:onClose( container )
	PageManager.popPage(thisPageName)
end

function PlayerTitleBase:onHelp(container)
	PageManager.showHelp(GameConfig.HelpKey.HELP_TITLE)
end
----------------scrollview-------------------------
local playerTitleItem = {
	ccbiFile = "ChangeTitleContent.ccbi"
}

function playerTitleItem.onFunction(eventName, container)
	if eventName == "luaRefreshItemView" then
		playerTitleItem.onRefreshItemView(container)
	elseif eventName == "onChoice" then
		playerTitleItem.onChoiceBtn(container)
	end
end

function playerTitleItem.onRefreshItemView( container )
	local contentId = container:getItemDate().mID
	local htmlTitle = nil -- 称号
	local htmlDescribe = nil -- 称号描述
	-- 设置称号颜色和描述颜色
	local titleLabelNode = container:getVarLabelBMFont("mChangeTitleName")
	local describeLabelNode = container:getVarLabelBMFont("mChangeTitleTex")
	if titleLabelNode ~= nil and describeLabelNode ~= nil then
		titleManager:setLabelTitleWithBG(titleLabelNode, playerTitleInfo[contentId].id)
		titleManager:setLabelTitleDescription(describeLabelNode, playerTitleInfo[contentId].id)
	end
	local titleSpriteNode = container:getVarSprite("mChangeTittlePic")
	if titleSpriteNode ~= nil then
		titleManager:setTittlePic(titleSpriteNode,playerTitleInfo[contentId].id)
	end
	-- 称号背景图
	local titleSprite = container:getVarSprite("mPlayerTitleSprite")
	titleManager:setTitleBG(titleSprite,playerTitleInfo[contentId].id)
	-- 没有的,显示未完成，拥有的显示可点击，已选中的显示已选中
	if playerTitleInfo[contentId].haveTitle == 0 then
		container:getVarNode("mNotReached"):setVisible(true)
		container:getVarNode("mChoiceNode"):setVisible(false)
	else
		container:getVarNode("mNotReached"):setVisible(false)
		container:getVarNode("mChoiceNode"):setVisible(true)
		-- 是所选的称号
		if nowTitleId == playerTitleInfo[contentId].id then
			PlayerNowTitleItemContainer = container
			container:getVarSprite("mChoiceSprite"):setVisible(true)
		else
			container:getVarSprite("mChoiceSprite"):setVisible(false)
		end
	end
end

function playerTitleItem.onChoiceBtn(container)
	local contentId = container:getItemDate().mID
	if nowTitleId == playerTitleInfo[contentId].id then
		nowTitleId = 0
	else
		nowTitleId = playerTitleInfo[contentId].id
	end
	if PlayerNowTitleItemContainer ~= nil then
	    PlayerNowTitleItemContainer:getVarSprite("mChoiceSprite"):setVisible(false)
	end
	container:getVarSprite("mChoiceSprite"):setVisible(nowTitleId~=0)
	PlayerNowTitleItemContainer = container
end

function PlayerTitleBase:rebuildAllItem(container)
	self:clearAllItem(container);
	self:buildItem(container);
end

function PlayerTitleBase:clearAllItem(container)
	NodeHelper:clearScrollView(container)
end

function PlayerTitleBase:buildItem(container)
	NodeHelper:buildScrollView(container, #playerTitleInfo, playerTitleItem.ccbiFile, playerTitleItem.onFunction)
end
----------------scrollview end----------------------

local CommonPage = require('CommonPage')
local PlayerTitlePage= CommonPage.newSub(PlayerTitleBase, thisPageName, option)
