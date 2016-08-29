----------------------------------------------------------------------------------
--[[
	FILE:			ActivityPage.lua
	ENCODING:		UTF-8, no-bomb
	DESCRIPTION:	活动
	AUTHOR:			hgs
	CREATED:		2014-08-23
--]]
----------------------------------------------------------------------------------
local ActivityFunction  = require("ActivityFunction")
local thisPageName = "ActivityPage"
local NodeHelper = require("NodeHelper");
local option = {
	ccbiFile = "ActivitiesPage.ccbi",
	handlerMap = {
		onHelp	= "onHelp"
	},
	opcode = opcodes
};

----------------- global data -----------------

----------------- local data -----------------
local ActivityPageBase = {}


local ActivityItem = {
	ccbiFile = "ActivitiesContent.ccbi",
}

function ActivityItem.onFunction(eventName, container)
	if eventName == "luaRefreshItemView" then
		ActivityItem.onRefreshItemView(container)
	elseif eventName == "onActivities" then
		ActivityItem.goActivityPage(container)
	end
end

function ActivityItem.onRefreshItemView(container)
	local index = container:getItemDate().mID;
	local id = ActivityInfo.ids[index];
	local cfg = ActivityConfig[id];
	if cfg == nil then
		CCLuaLog("Error:: activity not found!!");
		return;
	end
	NodeHelper:setNormalImages(container, {mActivities = cfg.image});
	
	local showRedPoint = false;
	if id ~= ActivityInfo.closeId then
		local isNew = ActivityInfo.activities[id]["isNew"];
		local noticeCount = ActivityInfo.rewardCount[id] or 0;
		showRedPoint = isNew or noticeCount > 0;
	end
	NodeHelper:setNodesVisible(container, {mActivitiesPoint = showRedPoint});
end

function ActivityItem.goActivityPage(container)
	local index = container:getItemDate().mID;
	local id = ActivityInfo.ids[index];
	
	ActivityPage_goActivity(id);
end
-----------------------------------------------
--ActivityPageBase页面中的事件处理
----------------------------------------------
function ActivityPageBase:onEnter(container)
	container:registerMessage(MSG_MAINFRAME_REFRESH)
	local mScale9Sprite = container:getVarScale9Sprite("mScale9Sprite")
	if mScale9Sprite ~= nil then
		container:autoAdjustResizeScale9Sprite( mScale9Sprite )
	end
	NodeHelper:initScrollView(container, "mContent", 3)
	if container.mScrollView ~= nil then
		container:autoAdjustResizeScrollview(container.mScrollView);
	end
	self:refreshPage(container)
end

function ActivityPageBase:onExit(container)
	container:removeMessage(MSG_MAINFRAME_REFRESH)
	NodeHelper:deleteScrollView(container)
end
----------------------------------------------------------------

function ActivityPageBase:refreshPage(container)
	self:rebuildAllItem(container)
end
----------------scrollview-------------------------
function ActivityPageBase:rebuildAllItem(container)
	self:clearAllItem(container);
	self:buildItem(container);
end

function ActivityPageBase:clearAllItem(container)
	NodeHelper:clearScrollView(container)
end

function ActivityPageBase:buildItem(container)
	NodeHelper:buildScrollView(container, #ActivityInfo.ids, ActivityItem.ccbiFile, ActivityItem.onFunction)
end
	
----------------click event------------------------	
function ActivityPageBase:onHelp(container)
	PageManager.showHelp(GameConfig.HelpKey.HELP_ACTIVE)
end	

function ActivityPageBase:onReceiveMessage(container)
	local message = container:getMessage()
	local typeId = message:getTypeId()
	if typeId == MSG_MAINFRAME_REFRESH then
		local pageName = MsgMainFrameRefreshPage:getTrueType(message).pageName;
		if pageName == thisPageName then
			self:refreshPage(container);
		end
	end
end

-------------------------------------------------------------------------
local CommonPage = require("CommonPage");
ActivityPage = CommonPage.newSub(ActivityPageBase, thisPageName, option);

--活动跳转, openWay:打开方式，'change' or 'push'  or 'onClick'
function ActivityPage_goActivity(id)
	local cfg = ActivityConfig[id];
	if cfg.openWay == "none" then return; end
	
	if not ActivityInfo.activities[id] then
		MessageBoxPage:Msg_Box_Lan("@ActivityNotOpen");
		return;
	end

	GlobalData.nowActivityId = id
	if cfg.openWay == "push" then
		PageManager.pushPage(cfg.page);
    elseif cfg.openWay == "click" then
        if ActivityFunction[id]~=nil then
            ActivityFunction[id]()
        end
	else
		PageManager.changePage(cfg.page);
	end
		
	--检测红点是否取消
	ActivityInfo:saveVersion(id);
	ActivityInfo:checkReward();
end