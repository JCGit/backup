----------------------------------------------------------------------------------
--[[
	FILE:			FriendSearchPopPage.lua
	ENCODING:		UTF-8, no-bomb
	DESCRIPTION:	搜索好友弹窗
	AUTHOR:			zhenhui
	CREATED:		2014-9-4
--]]
----------------------------------------------------------------------------------
local NodeHelper = require("NodeHelper")
local thisPageName = 'FriendSearchPopPage'
local FriendSearchBase = {}

-- 搜索id最大值，与后端一致
local FriendSearchIdMax = 100000000

FriendSearchPageCallback = nil

local option = {
	ccbiFile = "FriendSearchPopUp.ccbi",
	handlerMap = {
		onCancel 		= 'onClose',
		onFriendSearch 	= 'onFriendSearch',
		onIDButton 		= 'onInput',
		luaInputboxEnter = 'onInputboxEnter'
	}
}

local idInput = ''

function FriendSearchBase:onLoad(container)
	container:loadCcbiFile(option.ccbiFile)
end

function FriendSearchBase:onEnter(container)
	container:registerLibOS()
	self:refreshPage(container)
end

function FriendSearchBase:onExit(container)
	idInput = ''
	container:removeLibOS()
end

function FriendSearchBase:refreshPage(container)
	local lb2Str = {
		mTitle = common:getLanguageString('@FriendSearch'),
		mSearchTex = common:getLanguageString('@FriendSearchExplain')
	}
	NodeHelper:setStringForLabel(container, lb2Str)
	NodeHelper:setStringForTTFLabel(container, { mFriendID = common:getLanguageString('@FriendInputSearchContent')})
end

function FriendSearchBase:onClose(container)
	PageManager.popPage(thisPageName)
end

function FriendSearchBase:onFriendSearch(container)
	if common:trim(idInput) == '' then
		MessageBoxPage:Msg_Box('@FriendSearchNoContent')
		return
	end		
	PageManager.viewPlayerInfo(tonumber(idInput));	
	--PageManager.popPage(thisPageName)
end

function FriendSearchBase:onInput(container)
	Flag_Chat = 1
	libOS:getInstance():showInputbox(false)
end

function FriendSearchBase:onInputboxEnter(container)
	local content = container:getInputboxContent()
	if (not tonumber(content)) then
		MessageBoxPage:Msg_Box('@FriendSearchNoContent')
		return 
	end
	local inputNumber = math.floor(tonumber(content))

	-- 检查id合法性
	if common:trim(content) == '' then
		MessageBoxPage:Msg_Box('@FriendSearchNoContent')
		return
	elseif (not inputNumber) or (inputNumber >= FriendSearchIdMax) then
		MessageBoxPage:Msg_Box('@GuildInputSearchNumber')
		return 
	end

	idInput = content
	NodeHelper:setStringForTTFLabel(container, { mFriendID = idInput })
end


local CommonPage = require('CommonPage')
local FriendSearchPopPage= CommonPage.newSub(FriendSearchBase, thisPageName, option)
