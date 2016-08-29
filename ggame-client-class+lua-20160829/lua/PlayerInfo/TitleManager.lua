----------------------------------------------------------------------------------
--[[
	FILE:			TitleManager.lua
	ENCODING:		UTF-8, no-bomb
	DESCRIPTION:	manager for player title
	AUTHOR:			zhaolu
	CREATED:		2014-09-22
--]]
----------------------------------------------------------------------------------

----------------------------local data--------------------------------
local PlayerTitle_pb = require("PlayerTitle_pb")
local NodeHelper = require("NodeHelper")
local TitleManager = {
	myNowTitleId = 0,		-- the title I used now
	myOwnTitleIds = nil,		-- the titles I have
	myOwnTitlesChanged = 0, -- 1 as my own titles changed,0 as not
}
local playerTitleInfo = {}
local TitleCfg = ConfigManager.getPlayerTitleCfg() -- all titles include I have and not have
local TTFTag = 900
---------------------------local data end-----------------------------
-- title info get from server as a long listener
function TitleManager:setTitleInfo(msg)
	if msg.titleInfo:HasField("titleId") then
		TitleManager.myNowTitleId = msg.titleInfo.titleId
	end	
	TitleManager.myOwnTitleIds 	= msg.titleInfo.titleIds
	if msg:HasField("titleChanged") and msg.titleChanged==1 then
		NoticePointState.isChange = true
		NoticePointState.TITLE_CHANGE_MSG = true
	end
	PageManager.refreshPage("PlayerInfoPage")
	PageManager.refreshPage("EquipmentPage")
end

function TitleManager:getTitleById(id)
	if TitleCfg[id] ~= nil then
		return TitleCfg[id]
	end
end

function TitleManager:setMyNowTitle(id)
	if TitleCfg[id] ~= nil or id==0 then
		local msg = PlayerTitle_pb.ChooseTitleId();
		msg.titleId = tonumber(id);
		common:sendPacket(HP_pb.CHANGE_TITLE_C, msg, false)
	end
end

-- function TitleManager:setHtmlLabelTittle(node,key,id)
-- 	local htmlLabel = nil
-- 	if TitleCfg[id] ~= nil then
-- 		local titleType = TitleCfg[id].type
-- 		local size = CCSizeMake(GameConfig.LineWidth.PlayerTitle, 100);
-- 		local htmlLabelStr = common:fillHtmlStr(key.."_"..titleType, TitleCfg[id].name)
-- 		htmlLabel = NodeHelper:setCCHTMLLabelDefaultPos(node,size, htmlLabelStr)
-- 		return htmlLabel
-- 	end
-- end

function TitleManager:setBMFontLabelTittle(node,id,fontSize,withBracket)
	if TitleCfg[id] ~= nil or id==0 then
	    local size = fontSize~=nil and tonumber(fontSize) or 24
		local bmfontLable = CCLabelBMFont:create("","Lang/heiOutline24.fnt")
		local bWithBracket = withBracket==nil and false or withBracket
		bmfontLable:setScale(size/30)
		bmfontLable = self:setLabelTitle(bmfontLable, id, bWithBracket)
		node:getParent():removeChildByTag(TTFTag,true)
		node:getParent():addChild(bmfontLable,1,TTFTag)

		bmfontLable:setAnchorPoint(node:getAnchorPoint())
		local y = node:getPositionY()
		bmfontLable:setPosition(ccp(node:getPositionX()+node:getContentSize().width+10,node:getPositionY()))

		return bmfontLable
	end
end

function TitleManager:setLabelTitle(node,id,withBracket)
    id = tonumber(id)
	if TitleCfg[id] ~= nil then
		local titleType = tonumber(TitleCfg[id].type)
		local labelColor = nil
		if titleType == 3 then
			labelColor = NodeHelper:_getColorFromSetting(GameConfig.ColorMap.COLOR_ORANGE)
		elseif titleType == 2 then
			labelColor = NodeHelper:_getColorFromSetting(GameConfig.ColorMap.COLOR_PURPLE)
		elseif titleType == 1 then
			labelColor = NodeHelper:_getColorFromSetting(GameConfig.ColorMap.COLOR_BLUE)
		end
		if withBracket == true then
			node:setString("[" .. tostring(TitleCfg[id].name) .. "]")
		else
			node:setString(tostring(TitleCfg[id].name))
		end
		node:setColor(labelColor)
		return node
	elseif id==0 then
		node:setString("")
		return node
	end
end

function TitleManager:setLabelTitleWithBG(node, id)
	id = tonumber(id)
	if TitleCfg[id] ~= nil then
		local titleType = tonumber(TitleCfg[id].type)
		local labelColor = nil
		if titleType == 3 then
			labelColor = NodeHelper:_getColorFromSetting(GameConfig.ColorMap.COLOR_WHITE)
		elseif titleType == 2 then
			labelColor = NodeHelper:_getColorFromSetting(GameConfig.ColorMap.COLOR_TITLE_PURPLE)
		elseif titleType == 1 then
			labelColor = NodeHelper:_getColorFromSetting(GameConfig.ColorMap.COLOR_TITLE_BLUE)
		end
		node:setString(tostring(TitleCfg[id].name))
		node:setColor(labelColor)
		return node
	elseif id==0 then
		node:setString("")
	end
end

function TitleManager:setLabelTitleDescription(node,id)
	if TitleCfg[id] ~= nil then
		local titleType = tonumber(TitleCfg[id].type)
		local labelColor = nil
		if titleType == 3 then
			labelColor = NodeHelper:_getColorFromSetting(GameConfig.ColorMap.COLOR_DESCRIPTION_ORANGE)
		elseif titleType == 2 then
			labelColor = NodeHelper:_getColorFromSetting(GameConfig.ColorMap.COLOR_DESCRIPTION_PURPLE)
		elseif titleType == 1 then
			labelColor = NodeHelper:_getColorFromSetting(GameConfig.ColorMap.COLOR_DESCRIPTION_BLUE)
		end
		node:setString(tostring(TitleCfg[id].describe))
		node:setColor(labelColor)
	end
end

function TitleManager:orderTitle()
	playerTitleInfo = {}
    playerTitleInfo = common:deepCopy(TitleCfg)
	table.foreachi(playerTitleInfo, function(i,v)
		v.haveTitle = 0
	end)
	table.foreachi(TitleManager.myOwnTitleIds, function(i, v)
		playerTitleInfo[v].haveTitle = 1 --拥有此Id的称号
	end)
	nowTitleId = TitleManager.myNowTitleId
	--排序，已经达成的>未达成的，橙色 > 紫色 > 蓝色
	table.sort(playerTitleInfo, function (e1,e2)
		if e1.haveTitle ~= e2.haveTitle then
			return e1.haveTitle+0 > e2.haveTitle+0
		elseif e1.type ~= e2.type then
			return e1.type+0 > e2.type+0 
		else
			return e1.id+0 < e2.id+0
		end
	end)
	return playerTitleInfo
end

function TitleManager:getTittlePic(id)
	local picPath = ""
	if TitleCfg[id] ~= nil then
		picPath = TitleCfg[id].picPath
	end
	return picPath 
end

function TitleManager:getTittleBG(id)
	local BGPath = ""
	if TitleCfg[id] ~= nil then
		BGPath = TitleCfg[id].BGPath
	end
	return BGPath 
end

function TitleManager:setTittlePic(spriteNode, id)
	if spriteNode ~= nil and TitleCfg[id] ~= nil then
		spriteNode:setTexture(self:getTittlePic(id))
	end
end

function TitleManager:setTitleBG(spriteNode, id)
	if TitleCfg[id]~=nil and spriteNode~=nil then
		NodeHelper:setNodeVisible(spriteNode,true)
		spriteNode:setTexture(self:getTittleBG(id)) 
	else
		NodeHelper:setNodeVisible(spriteNode,false)
	end
end

return TitleManager