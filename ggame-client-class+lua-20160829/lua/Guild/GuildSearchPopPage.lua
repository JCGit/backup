----------------------------------------------------------------------------------
--[[
	FILE:			GuildSearchPopPage.lua
	ENCODING:		UTF-8, no-bomb
	DESCRIPTION:	搜索公会弹窗
	AUTHOR:			sunyj
	CREATED:		2014-8-4
--]]
----------------------------------------------------------------------------------
local NodeHelper = require("NodeHelper")
local GuildDataManager = require('GuildDataManager')
local thisPageName = 'GuildSearchPopPage'
local GuildSearchBase = {}

-- 搜索id最大值，与后端一致
local GuildSearchIdMax = 100000000

local option = {
	ccbiFile = "GuildSearchPopUp.ccbi",
	handlerMap = {
		onCancel 		= 'onClose',
		onGuildSearch 	= 'onGuildSearch',
		onIDButton 		= 'onInput',
		luaInputboxEnter = 'onInputboxEnter'
	}
}

local idInput = ''

function GuildSearchBase:onLoad(container)
	container:loadCcbiFile(option.ccbiFile)
end

function GuildSearchBase:onEnter(container)
	container:registerLibOS()
	self:refreshPage(container)
end

function GuildSearchBase:onExit(container)
	container:removeLibOS()
end

function GuildSearchBase:refreshPage(container)
	local lb2Str = {
		mTitle = common:getLanguageString('@GuildSearch'),
		mSearchTex = common:getLanguageString('@GuildSearchExplain')
	}
	NodeHelper:setStringForLabel(container, lb2Str)
	NodeHelper:setStringForTTFLabel(container, { mGuildID = common:getLanguageString('@GuildInputSearchContent')})
end

function GuildSearchBase:onClose(container)
	PageManager.popPage(thisPageName)
end

function GuildSearchBase:onGuildSearch(container)
	if common:trim(idInput) == '' then
		MessageBoxPage:Msg_Box('@GuildSearchNoContent')
		return
	end
	GuildDataManager:sendSearchGuildPacket(tonumber(idInput))
	PageManager.popPage(thisPageName)
end

function GuildSearchBase:onInput(container)
	libOS:getInstance():showInputbox(false)
end

function GuildSearchBase:onInputboxEnter(container)
	local content = container:getInputboxContent()
	local inputNumber = math.floor(tonumber(content))

	-- 检查id合法性
	if common:trim(content) == '' then
		MessageBoxPage:Msg_Box('@GuildSearchNoContent')
		return
	elseif (not inputNumber) or (inputNumber >= GuildSearchIdMax) then
		MessageBoxPage:Msg_Box('@GuildInputSearchNumber')
		return 
	end

	idInput = content
	NodeHelper:setStringForTTFLabel(container, { mGuildID = idInput })
end


local CommonPage = require('CommonPage')
local GuildSearchPopPage= CommonPage.newSub(GuildSearchBase, thisPageName, option)
