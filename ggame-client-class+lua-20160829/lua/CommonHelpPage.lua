----------------------------------------------------------------------------------
--[[
	FILE:			CommonHelpPage.lua
	ENCODING:		UTF-8, no-bomb
	DESCRIPTION:	通用帮助页面，其中帮助内容scrollview中的item高度可变
					一般通过common:goHelpPage(helpString)来弹出此页面
	AUTHOR:			hgs
	CREATED:		2013-09-11
	
	MODIFY 1:		hgs at 2013-12-09
					add notes
--]]
----------------------------------------------------------------------------------

require "CommonPage"

local option = {
	handlerMap = {
		onClose = "onClose"
	}
}
local CommonHelpPage = CommonPage.new("CommonHelpPage", option)

--内部变量:帮助标题，帮助内容
local HelpTitle = "@Explantation"
local HelpInfo = nil

----------------------------------------------------------------------------------
--唯一外部接口，用于设置帮助内容及标题(标题可默认)
--------------------------------------------------
CommonHelpPageVar = {
	set = function(helpInfo, title)
			HelpInfo = helpInfo
			HelpTitle = Language:getInstance():getString(title or "@Explanation")
		end
}

----------------------------------------------------------------------------------
--帮助内容scrollview单子item,高度可变
---------------------------------------
local HelpContent = {}

function HelpContent:new(mID)
	local pItem = ScriptContentBase:create("CommonHelpPopUpC.ccbi")
	local helpInfo = HelpInfo[mID]

	--local lbTitle = pItem:getVarLabelBMFont("mMAPUCTitle")
	local lbContent = pItem:getVarLabelBMFont("mRefineHelpTex")
	--lbTitle:setString(tostring(helpInfo["title"]))
	lbContent:setString(common:stringAutoReturn(helpInfo["content"], 23))

	local msgHeight = lbContent:getContentSize().height
	local space = 50 --50 : height for space	
	
	if msgHeight > pItem:getContentSize().height*pItem:getScaleY() then
		local size = CCSizeMake(pItem:getContentSize().width,msgHeight)
		--reset the position of the content by zhenhui
		lbContent:setPositionY(size.height);
		pItem:setContentSize(size)
	end
	return pItem
end

----------------------------------------------------------------------------------
--帮助页面基本lua事件处理
---------------------------------------
function CommonHelpPage.onLoad(container)
	container:loadCcbiFile("CommonHelpPopUp.ccbi")
	container.mScrollView = container:getVarScrollView("mContent")
	container.mScrollViewRootNode = container.mScrollView:getContainer()
end

function CommonHelpPage.onEnter(container)
	CommonHelpPage.rebuildAllItem(container)
	common:setStringForLabel(container, {mExplanation = HelpTitle})
end

function CommonHelpPage.onClose(container)
	MainFrame:getInstance():popPage("CommonHelpPage")
end

function CommonHelpPage.onExit(container)
	CommonHelpPage.clearAllItem(container)
end

function CommonHelpPage.rebuildAllItem(container)
	CommonHelpPage.clearAllItem(container);
	CommonHelpPage.buildItem(container);
end

function CommonHelpPage.buildItem(container)
	local fOneItemWidth = 0;
	local positionY = 0;

	for i = #HelpInfo, 1, -1 do
		local pItem = HelpContent:new(i)
		container.mScrollViewRootNode:addChild(pItem)
		pItem:setPosition(ccp(0, positionY))
		if fOneItemWidth < pItem:getContentSize().width then
			fOneItemWidth = pItem:getContentSize().width
		end
		positionY = positionY + pItem:getContentSize().height
	end
	local size = CCSizeMake(fOneItemWidth, positionY)
	container.mScrollView:setContentSize(size);
	container.mScrollView:setContentOffset(ccp(0, container.mScrollView:getViewSize().height - container.mScrollView:getContentSize().height * container.mScrollView:getScaleY()));
end

function CommonHelpPage.clearAllItem(container)
	container.mScrollViewRootNode:removeAllChildren()
end
