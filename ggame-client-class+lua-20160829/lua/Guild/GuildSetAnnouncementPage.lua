----------------------------------------------------------------------------------
--[[
	FILE:			GuildSetAnnouncementPage.lua
	ENCODING:		UTF-8, no-bomb
	DESCRIPTION:	设置公会公告页面
	AUTHOR:			sunyj
	CREATED:		2014-8-4
--]]
----------------------------------------------------------------------------------

local NodeHelper = require("NodeHelper")
local thisPageName = 'GuildSetAnnouncementPage'
local GuildSetAnnouncementBase = {}

-- 公告字数限制
local GuildAnnouncementLengthLimit = 200

GuildSetAnnouncementCallback = nil

local option = {
	ccbiFile = "GuildSetAnnouncement.ccbi",
	handlerMap = {
		onCancel 			= 'onClose',
		onConfirmation 	= 'onChange',
		onInput 		= 'onInputName',
		luaInputboxEnter 	= 'onInputboxEnter'
	}
}

local contentInput = ''

function GuildSetAnnouncementBase:onLoad(container)
	container:loadCcbiFile(option.ccbiFile)
end

function GuildSetAnnouncementBase:onEnter(container)
	container:registerLibOS()
	self:refreshPage(container)
end

function GuildSetAnnouncementBase:onExit(container)
	container:removeLibOS()
	GuildSetAnnouncementCallback = nil
end

function GuildSetAnnouncementBase:refreshPage(container)
	NodeHelper:setStringForTTFLabel(container, { mAnnouncement = common:getLanguageString('@GuildNewAnnouncement')})
end

function GuildSetAnnouncementBase:onClose(container)
	PageManager.popPage(thisPageName)
end

function GuildSetAnnouncementBase:onChange(container)
	GuildSetAnnouncementCallback(contentInput)
	PageManager.popPage(thisPageName)
end

function GuildSetAnnouncementBase:onInputName(container)
	libOS:getInstance():showInputbox(false)
end

function GuildSetAnnouncementBase:onInputboxEnter(container)
	local content = container:getInputboxContent()
	local length = GameMaths:calculateStringCharacters(content)
	if length > GuildAnnouncementLengthLimit then
        MessageBoxPage:Msg_Box("@GuildAnnouncementTooLong")
		return
	elseif GameMaths:isStringHasUTF8mb4(content) then
        MessageBoxPage:Msg_Box("@NameHaveForbbidenChar")
		return
    end
	--[[
	if not RestrictedWord:getInstance():isStringOK(content) then
        MessageBoxPage:Msg_Box("@NameHaveForbbidenChar")
		return
	end
	--]]
	contentInput = RestrictedWord:getInstance():filterWordSentence(content);

	local str =GameMaths:stringAutoReturnForLua(contentInput, 20, 0)
	NodeHelper:setStringForTTFLabel(container, { mAnnouncement = str })
end

local CommonPage = require('CommonPage')
local GuildSetAnnouncementPage = CommonPage.newSub(GuildSetAnnouncementBase, thisPageName, option)
