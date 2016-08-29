----------------------------------------------------------------------------------
--[[
	FILE:			ActivityBasePage.lua
	ENCODING:		UTF-8, no-bomb
	DESCRIPTION:	累计登陆活动界面
	AUTHOR:			zhoutong
	CREATED:		2014-10-15
--]]
----------------------------------------------------------------------------------
local NodeHelper = require("NodeHelper")
local BasePage = require("BasePage")

----------------- local data -----------------
local ActivityBasePage = BasePage:new(nil,"ActivityBase",nil,nil)
-----------------------end Content------------

function ActivityBasePage:onExecute(container)
    self:onTimer(container);
end

function ActivityBasePage:onExit(container)
    TimeCalculator:getInstance():removeTimeCalcultor(self.timerName);
    self:removePacket(container)
	NodeHelper:deleteScrollView(container);
    self.mRebuildLock = true
    self.DataHelper:addOrSetVariableByKey("RemainTime",-1)
end
----------------------------------------------------------------
function ActivityBasePage:onTimer(container)
	if not TimeCalculator:getInstance():hasKey(self.timerName) then
	    if self.DataHelper:getVariableByKey("RemainTime") == 0 then
	        local endStr = common:getLanguageString("@ActivityEnd");
	        NodeHelper:setStringForLabel(container, {[self.timerLabel] = endStr});
	    elseif self.DataHelper:getVariableByKey("RemainTime") < 0 then
	        NodeHelper:setStringForLabel(container, {[self.timerLabel] = ""});
        end
        return; 
    end

	local remainTime = TimeCalculator:getInstance():getTimeLeft(self.timerName);
	if remainTime + 1 > self.DataHelper:getVariableByKey("RemainTime") then
		return;
	end

	self.DataHelper:addOrSetVariableByKey("RemainTime",remainTime)
	local timeStr = common:second2DateString(self.DataHelper:getVariableByKey("RemainTime"), false);
	
	if self.DataHelper:getVariableByKey("RemainTime") <= 0 then
	    timeStr = common:getLanguageString("@ActivityEnd");
    end
	NodeHelper:setStringForLabel(container, { [self.timerLabel] = timeStr});
end

function ActivityBasePage:clearPage(container)
    NodeHelper:setStringForLabel(container, {
		[self.timerLabel]	= ""
	});
end

function ActivityBasePage:refreshPage(container)
    local remainTime = self.DataHelper:getVariableByKey("RemainTime")
    if remainTime > 0 and not TimeCalculator:getInstance():hasKey(self.timerName) then
		TimeCalculator:getInstance():createTimeCalcultor(self.timerName, remainTime);
	end
end
----------------click event------------------------	
function ActivityBasePage:onBack()
    PageManager.changePage("ActivityPage");
end

function ActivityBasePage:onHelp()
    PageManager.showHelp(self.DataHelper:getConfigDataByKey("HELP"));
end
-------------------------------------------------------------------------
return ActivityBasePage
