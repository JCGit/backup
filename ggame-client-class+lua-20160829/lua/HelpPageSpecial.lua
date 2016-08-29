----------------------------------------------------------------------------------
--[[
	FILE:			HelpPage.lua
	ENCODING:		UTF-8, no-bomb
	DESCRIPTION:	能量之核: 探索、拼合、列表页面
	AUTHOR:			hgs
	CREATED:		2014-01-21
--]]
----------------------------------------------------------------------------------
--require "ExploreEnergyCore_pb"

local thisPageName = "HelpPageSpecial"

local HelpConfg = {}
local option = {
	ccbiFile = "GeneralHelpPopUp2.ccbi",
	handlerMap = {
		onClose 		= "onClose"
	},
	opcode = opcodes
};

local HelpPageBase = {}

local NodeHelper = require("NodeHelper");
local TodoStr = "99";
local TodoImage = "UI/MainScene/UI/u_ico000.png";
local TodoPoster = "person/poster_char_Boss.png";
local helpTitle = ""

--------------------------------------------------------------
local HelpItem = {
	
}

function HelpItem.onFunction(eventName, container)
	if eventName == "luaRefreshItemView" then
		HelpItem.onRefreshItemView(container)		
	end
end

function HelpItem.onRefreshItemView(container)
    --[[
	local contentId = container:getItemDate().mID
	local itemInfo = HelpConfg[contentId]	
	
	local nameNode = container:getVarLabelBMFont("mLabel")
	NodeHelper:setCCHTMLLabel( nameNode,CCSize(500,200),ccp(0,0),itemInfo.content )
	]]
end

--展示奖励描述信息
function HelpItem.showEnergyCoreInfo(container, resIndex)
	
end

----------------------------------------------------------------------------------
	
-----------------------------------------------
--HelpPageBase页面中的事件处理
----------------------------------------------
function HelpPageBase:onEnter(container)
	--self:registerPacket(container)
	--container:registerMessage(MSG_SEVERINFO_UPDATE)
	if  HelpConfg == nil or #HelpConfg == 0 then
		HelpConfg = ConfigManager.getHelpCfg( GameConfig.HelpKey.HELP_DEFAULT )
	end
	if helpTitle == nil or helpTitle == "" then
	    helpTitle = Language:getInstance():getString("@HelpTitle")
	end
	NodeHelper:initScrollView(container, "mContent", 1);
	--HelpConfg = ConfigManager.getHelpCfg()
	self:refreshPage(container);
	self:rebuildAllItem(container);
	
	container:getVarLabelBMFont("mTitle"):setString(helpTitle)
end

function HelpPageBase:onExecute(container)
end

function HelpPageBase:onExit(container)
	--self:removePacket(container)
	--container:removeMessage(MSG_SEVERINFO_UPDATE)	
	NodeHelper:deleteScrollView(container)
	HelpConfg = {}
end
----------------------------------------------------------------

function HelpPageBase:refreshPage(container)
	local noticeStr = common:getLanguageString("@MailNotice", "");
	NodeHelper:setStringForLabel(container, {mMailPromptTex = noticeStr});
end
----------------scrollview-------------------------
function HelpPageBase:rebuildAllItem(container)
	self:clearAllItem(container);
	self:buildItem(container);
end

function HelpPageBase:clearAllItem(container)
	NodeHelper:clearScrollView(container);
end

function HelpPageBase:buildItem(container)
	--NodeHelper:buildScrollView(container, #HelpConfg, "HelpContent.ccbi", HelpItem.onFunction);
	local iMaxNode = container.m_pScrollViewFacade:getMaxDynamicControledItemViewsNum()
	local iCount = 0
	local fOneItemHeight = 0
	local fOneItemWidth = 0

	for i=#HelpConfg, 1, -1 do
		local pItemData = CCReViSvItemData:new_local()
		pItemData.mID = i
		pItemData.m_iIdx = i
		pItemData.m_ptPosition = ccp(0, fOneItemHeight * iCount)

		if iCount < iMaxNode then
			local pItem = ScriptContentBase:create("GeneralHelpContent.ccbi")
            
            
			
            pItem.id = iCount
			pItem:registerFunctionHandler(HelpItem.onFunction)
			
			local itemHeight = 0
			
			local nameNode = pItem:getVarLabelBMFont("mLabel")
			--NodeHelper:setCCHTMLLabel( nameNode,CCSize(500,200),ccp(20,40),HelpConfg[i].content )
			CCLuaLog("html -------star")
			local cSize = NodeHelper:setCCHTMLLabelDefaultPos( nameNode , CCSize(600,200) , HelpConfg[i].content  ):getContentSize()
			--[[
			local label = CCHTMLLabel:createWithString(HelpConfg[i].content,CCSize(500,200))
			
			local posX = nameNode:getPositionX()
			local posY = nameNode:getPositionY()
			
			--label:setPosition(ccp(posX,posY))
			label:setPosition(ccp(0,0))
			if nameNode ~= nil then
				nameNode:getParent():addChild(label)
			end
			
			CCLuaLog("html -------end")
			
			]]
			if fOneItemHeight < cSize.height then
				fOneItemHeight = cSize.height 
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

	local size = CCSizeMake(fOneItemWidth, fOneItemHeight * iCount )
	container.mScrollView:setContentSize(size)
	container.mScrollView:setContentOffset(ccp(0, container.mScrollView:getViewSize().height - container.mScrollView:getContentSize().height * container.mScrollView:getScaleY()))
	container.m_pScrollViewFacade:setDynamicItemsStartPosition(iCount - 1);
	container.mScrollView:forceRecaculateChildren();
	ScriptMathToLua:setSwallowsTouches(container.mScrollView)
end
function Help_SetHelpConfigSpecial( key ,title )
	
	HelpConfg = ConfigManager.getHelpCfg( key )
	helpTitle = title
	
end	
----------------click event------------------------
function HelpPageBase:onClose(container)
	PageManager.popPage(thisPageName);
end

-------------------------------------------------------------------------
local CommonPage = require("CommonPage");
HelpPageSpecial = CommonPage.newSub(HelpPageBase, thisPageName, option);