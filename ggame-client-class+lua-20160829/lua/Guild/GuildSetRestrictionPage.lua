----------------------------------------------------------------------------------
--[[
	FILE:			GuildSetRestrictionPage.lua
	ENCODING:		UTF-8, no-bomb
	DESCRIPTION:	公会加入条件设置页面
	AUTHOR:			sunyj
	CREATED:		2014-8-4
--]]
----------------------------------------------------------------------------------

local NodeHelper = require("NodeHelper")
local thisPageName = 'GuildSetRestrictionPage'
local GuildSetRestrictionBase = {}

-- 最大输入战力限制
local GuildBattlePointLimit = 9999999

GuildSetRestrictionCallback = nil

local option = {
	ccbiFile = "GuildSetRestriction.ccbi",
	handlerMap = {
		onCancel 			= 'onClose',
		onConfirmation 	= 'onChange',
		onInput 		= 'onInputName',
		luaInputboxEnter 	= 'onInputboxEnter'
	}
}

local contentInput = ''

function GuildSetRestrictionBase:onLoad(container)
	container:loadCcbiFile(option.ccbiFile)
end

function GuildSetRestrictionBase:onEnter(container)
	container:registerLibOS()
	self:refreshPage(container)
end

function GuildSetRestrictionBase:onExit(container)
	container:removeLibOS()
	GuildSetRestrictionCallback = nil
end

function GuildSetRestrictionBase:refreshPage(container)
	NodeHelper:setStringForLabel(container, { mRestriction = common:getLanguageString('@InputRestriction')})
end

function GuildSetRestrictionBase:onClose(container)
	PageManager.popPage(thisPageName)
end

function GuildSetRestrictionBase:onChange(container)
	if common:trim(contentInput) == '' then
		MessageBoxPage:Msg_Box('@RestrictionEmpty')
		return
	end
	GuildSetRestrictionCallback(tonumber(contentInput))
	PageManager.popPage(thisPageName)
end

function GuildSetRestrictionBase:onInputName(container)
	libOS:getInstance():showInputbox(false)
end

function GuildSetRestrictionBase:onInputboxEnter(container)
	local content = container:getInputboxContent()
	local inputNumber = math.floor(tonumber(content))

	if common:trim(content) == '' then
		MessageBoxPage:Msg_Box('@RestrictionEmpty')
		return
	elseif (not inputNumber) or (inputNumber > GuildBattlePointLimit) then
		MessageBoxPage:Msg_Box(common:getLanguageString('@GuildRestrictTooLarge', GuildBattlePointLimit))
		return
	end

	contentInput = content
	NodeHelper:setStringForLabel(container, { mRestriction = contentInput })
end

local CommonPage = require('CommonPage')
local GuildSetRestrictionPage = CommonPage.newSub(GuildSetRestrictionBase, thisPageName, option)
