--
-- Created by IntelliJ IDEA.
-- User: fish
-- Date: 13-9-11
-- Time: 下午5:40
-- To change this template use File | Settings | File Templates.
--
local AnalyticsManager = {}
AnalyticsManager.button = {}

local msgHandler = MessageScriptHandler:new(function(eventName, gameMsg)
	if gameMsg:getTypeId() == MSG_BUTTON_PRESSED then
		local ccb = MsgButtonPressed:getTrueType(gameMsg).ccb
		local func = MsgButtonPressed:getTrueType(gameMsg).func
		local tag = MsgButtonPressed:getTrueType(gameMsg).tag

		for i,v in ipairs(AnalyticsManager.button) do
			if v.ccb == ccb and v.func == func then
				if v.tag~="0" then
					libOS:getInstance():analyticsLogEvent(v.description.."_tag:"..tag)
				else
					libOS:getInstance():analyticsLogEvent(v.description)
				end
			end
		end

	end
end)

MessageManager:getInstance():regisiterMessageHandler(MSG_BUTTON_PRESSED, msgHandler)


function AnalyticsManager.init()
	local tabel = TableReaderManager:getInstance():getTableReader("AnalyticsButtonConfig.txt")
	count = tabel:getLineCount()-1;
	for i = 1,count do
		local index = tonumber(tabel:getData(i,0))
		if AnalyticsManager.button[index] == nil then
			AnalyticsManager.button[index] = {}
			AnalyticsManager.button[index].ccb = tabel:getData(i,1)
			AnalyticsManager.button[index].func = tabel:getData(i,2)
			AnalyticsManager.button[index].tag = tabel:getData(i,3)
			AnalyticsManager.button[index].description = tabel:getData(i,4)
		end
	end
end

AnalyticsManager.init()

-- added by sunyj
function AnalyticsByDescriptionAndTag(description, tag)
	libOS:getInstance():analyticsLogEvent(description..'_tag:'..tag)
end
-- end
