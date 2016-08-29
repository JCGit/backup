----------------------------------------------------------------------------------
--[[
	FILE:			GuildCreatePage.lua
	ENCODING:		UTF-8, no-bomb
	DESCRIPTION:	创建公会弹窗页面
	AUTHOR:			sunyj
	CREATED:		2014-8-4
--]]
----------------------------------------------------------------------------------
local NodeHelper = require("NodeHelper")
local GuildDataManager = require("GuildDataManager")
local UserInfo = require("UserInfo");
local thisPageName = 'GuildCreatePage'

local GuildCreateBase = {}

-- 公会名称字数
local GuildNameLeast = 2
local GuildNameMost = 6

local option = {
	ccbiFile = "GuildEstablishPopUp.ccbi",
	handlerMap = {
		onCancel 			= 'onClose',
		onGuildEastablish 	= 'onCreateAlliance',
		onNameButton 		= 'onInputName',
		luaInputboxEnter 	= 'onInputboxEnter'
	}
}

local nameInput = ''

function GuildCreateBase:onLoad(container)
	container:loadCcbiFile(option.ccbiFile)
end

function GuildCreateBase:onEnter(container)
	container:registerLibOS()
	self:refreshPage(container)
end

function GuildCreateBase:onExit(container)
	container:removeLibOS()
end

function GuildCreateBase:refreshPage(container)
	local lb2Str = {
		mDecisionTex = common:getLanguageString('@GuildEastablishPromptTex', GameConfig.Cost.CreateAlliance)
	}
	NodeHelper:setStringForLabel(container, lb2Str)
	NodeHelper:setStringForTTFLabel(container, { mGuildName = common:getLanguageString('@GuildInputName')})
end

function GuildCreateBase:onClose(container)
	PageManager.popPage(thisPageName)
end

-- 创建按钮响应：使用输入的nameInput来调用回调函数，发送协议包
function GuildCreateBase:onCreateAlliance(container)
	if not UserInfo.isGoldEnough(GameConfig.Cost.CreateAlliance) then
		return;
	end
	
	if common:trim(nameInput) == '' then
		MessageBoxPage:Msg_Box('@GuildNameEmpty')
		return
	end
	GuildDataManager:createAlliance(nameInput)
	PageManager.popPage(thisPageName)
end

function GuildCreateBase:onInputName(container)
	libOS:getInstance():showInputbox(false)
end

function GuildCreateBase:onInputboxEnter(container)
	local content = container:getInputboxContent()
	local length = GameMaths:calculateStringCharacters(content)
	if  length < GuildNameLeast or length > GuildNameMost then
		MessageBoxPage:Msg_Box(common:getLanguageString('@GuildNamLengthError', GuildNameLeast, GuildNameMost))
		return
	elseif GameMaths:isStringHasUTF8mb4(content) then
        MessageBoxPage:Msg_Box("@NameHaveForbbidenChar")
		return
	elseif (not RestrictedWord:getInstance():isStringOK(content)) then
        MessageBoxPage:Msg_Box("@NameHaveForbbidenChar")
		return
	end
	nameInput = content
	NodeHelper:setStringForTTFLabel(container, { mGuildName = nameInput })
end

local CommonPage = require('CommonPage')
local GuildCreatePage = CommonPage.newSub(GuildCreateBase, thisPageName, option)
