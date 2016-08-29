----------------------------------------------------------------------------------
--[[
FILE:			ReplaceSkillPage.lua
ENCODING:		UTF-8, no-bomb
DESCRIPTION:	换技能
AUTHOR:			zhenhui
CREATED:		2014-08-6
--]]
----------------------------------------------------------------------------------
local thisPageName = "ReplaceSkillPage"
local UserInfo = require("UserInfo");
local hp = require('HP_pb')
local skillPb = require("Skill_pb")
local SkillManager = require("SkillManager")
local option = {
	ccbiFile = "ReplaceSkillPopUp.ccbi",
	handlerMap = {
		onCancel				= "onCancel",
		onClose             = "onCancel",
		onPreservation		= "onSave"
	}
}

local ReplaceSkillPageBase = {}

local NodeHelper = require("NodeHelper");
--------------------------------------------------------------
local SkillItem = {
	ccbiFile 	= "SkillContent.ccbi",
}

local skillCfg = ConfigManager.getSkillCfg()
local roleCfg = ConfigManager.getRoleCfg();
local skillOpenCfg = ConfigManager.getSkillOpenCfg()
local myProfessionSkillCfg = {}
-- { (skillItemId, skillId) }
local skillItemIdMap = {}

local selectedTable = {}

local maxSlotSize = 0

local containerRef = {}

local thisBagId = 1

local thisRoleId = 1

local assignedSkills

function SkillItem.onFunction(eventName, container)
	if eventName == "luaRefreshItemView" then
		SkillItem.onRefreshItemView(container)
	elseif eventName == "onChoiceBtn" then
		SkillItem.onChooseItem(container)
	end
end

function SkillItem.isInTable(tabel,id)
	for _,v in ipairs(tabel) do
		if v == id then
			return true	
		end
	end
	return false
end

function SkillItem.removeItemFromTable(tabel,id)
	for k,v in ipairs(tabel) do
		if v == id then
			table.remove(tabel,k)			
			break
		end
	end		
end

function SkillItem.onRefreshItemView(container)
	local index = tonumber(container:getItemDate().mID)
	
	local skill = myProfessionSkillCfg[index]
	if not skill then
		CCLuaLog("ERROR in refresh ReplaceSkill page, skill cfg not found in skill.txt")
		return;
	end	

	-- icon
	local sprite2Img = {
		mSkillPic = skill.icon
	}
	NodeHelper:setSpriteImage(container, sprite2Img)
	
	local newStr =GameMaths:stringAutoReturnForLua(skill.describe, 18, 0)
	
	local lb2Str = {
		mSkillName 		= skill.name,
		mSkillTex 		= newStr
	}
	
	local skillOpenLevel = skill.openLevel	
	-- 在映射表里面找不到此itemId, 就是说这个技能没有开放
	if skillItemIdMap[skill.id] == nil then
		-- user level too low, skill closed
		NodeHelper:setNodeVisible(container:getVarNode('ConsumptionMpNode'), false)
		NodeHelper:setNodeVisible(container:getVarNode('mChoiceBtnNode'), false)
		NodeHelper:setNodeVisible(container:getVarLabelBMFont('mLevelLimit'), true)
		local limitStr = common:getLanguageString('@SkillOpenAtLevel', skillOpenLevel)
		lb2Str.mLevelLimit = limitStr
	else
		NodeHelper:setNodeVisible(container:getVarNode('ConsumptionMpNode'), true)
		NodeHelper:setNodeVisible(container:getVarNode('mChoiceBtnNode'), true)
		NodeHelper:setNodeVisible(container:getVarLabelBMFont('mLevelLimit'), false)

		local selectPic = container:getVarSprite("mSelected")
		local menu = container:getVarMenuItemImage("mMenu")	
	
		lb2Str.mConsumptionMp 	= skill.costMP

		local skillId = skillItemIdMap[skill.id] -- itemId to id
		if SkillItem.isInTable(selectedTable,skillId) then
			--NodeHelper:setNodeVisible(container:getVarMenuItemImage('mMenu'), false)
			menu:setEnabled(true)
			selectPic:setVisible(true)
		else
			--如果已满，则设置其他所有按钮为false
			local count = #selectedTable			
			if count == maxSlotSize then
				--已选择满，设置其他所有页面的content条为不可选
					selectPic:setVisible(false)
				menu:setEnabled(false)
			else
				selectPic:setVisible(false)
				menu:setEnabled(true)
			end
		end
	end
	NodeHelper:setStringForLabel(container, lb2Str);
end

function SkillItem.onChooseItem(container)
	local index = tonumber(container:getItemDate().mID)

	local skill = myProfessionSkillCfg[index]

	if not skill then
		CCLuaLog("ERROR in choose item in ReplaceSkill page, skill cfg not found in skill.txt")
		return;
	end

	local skillId = skillItemIdMap[skill.id]
	if not skillId then return end

	--如果已经在队列里了，则删除掉，否则加入进去
	if SkillItem.isInTable(selectedTable,skillId) then
		SkillItem.removeItemFromTable(selectedTable,skillId);		
	else
		--如果不在队列里，插入队列，同时判断可选table是否已满，
		table.insert(selectedTable,skillId)		
	end
	
	--ReplaceSkillPageBase:rebuildAllItem(containerRef)
	--刷新页面
	PageManager.refreshPage("ReplaceSkillPage")	
end

----------------------------------------------------------------------------------

-----------------------------------------------
--ReplaceSkillPageBase页面中的事件处理
----------------------------------------------
function ReplaceSkillPageBase:onEnter(container)
	containerRef = container

	container:registerMessage(MSG_MAINFRAME_REFRESH)
	
	NodeHelper:initScrollView(container, "mContent", 4)	
	
	UserInfo.sync()
	 --= UserInfo.roleInfo.skills
	
	if thisBagId == 1 then --
		assignedSkills = SkillManager:getFightSkillList()
	elseif thisBagId == 2 then
		assignedSkills = SkillManager:getArenaSkillList()
	else 
		assignedSkills = SkillManager:getDefenseSkillList()
	end
	
	if assignedSkills == nil then
		CCLuaLog("ERROR  in assignedSkills == null");
		return
	end
	--最多开放的孔数，或者是技能携带数
	maxSlotSize = #assignedSkills

	-- 重新设置selectedTable
	self:setSelectedTable()

	-- 从skillCfg中取出本职业的cfg
	self:getMyProfessionSkillCfg(UserInfo.roleInfo.itemId)

	-- 把cfg里的itemId和SkillInfo里面的id关联起来
	self:fillSkillItemIdMap()

	self:refreshPage(container)
	self:rebuildAllItem(container)
end

-- 把SkillInfo里面的技能id跟cfg里面的skillItemId关联起来
-- 没有关联到的itemId，即表示没有开启的
function ReplaceSkillPageBase:fillSkillItemIdMap()
	skillItemIdMap = {}
	local count = ServerDateManager:getInstance():getSkillInfoTotalSize()
	for i=0, count-1 do
		local skillStr = ServerDateManager:getInstance():getSkillInfoByIndexForLua(i)
		local skillInfo = skillPb.SkillInfo()
		skillInfo:ParseFromString(skillStr)
		if skillInfo then
			for k, v in ipairs(myProfessionSkillCfg) do
				if skillInfo.itemId == v.id then 
					skillItemIdMap[v.id] = skillInfo.id
					break
				end
			end
		end
	end
end

-- 从skill.txt里面筛选出本职业能装备的skill
-- 按照开放等级升序排列
function ReplaceSkillPageBase:getMyProfessionSkillCfg(professionItemId)
	myProfessionSkillCfg = {}
	if roleCfg[professionItemId] then
		local myProfession = roleCfg[professionItemId]["profession"]
		if myProfession then
			local count = 0
			for k, v in ipairs(skillCfg) do
				if v.profession == myProfession then
					count = count + 1
					myProfessionSkillCfg[count] = {}
					myProfessionSkillCfg[count] = v
				end
			end
		end
	end
	-- 按开放等级升序排列
	table.sort(myProfessionSkillCfg, function (c1, c2)
		return c1.openLevel < c2.openLevel
	end)
end

function ReplaceSkillPageBase:setSelectedTable()
	selectedTable = {}
	local openedSkillCount = #assignedSkills
	local count = 0
	for i=1, openedSkillCount do
		if assignedSkills[i] ~= 0 then
			count = count + 1
			selectedTable[count] = assignedSkills[i]
		end
	end
end

function ReplaceSkillPageBase:onExecute(container)
end

function ReplaceSkillPageBase:onExit(container)
	--self:removePacket(container)
	container:removeMessage(MSG_MAINFRAME_REFRESH)
	NodeHelper:deleteScrollView(container);
end
----------------------------------------------------------------
function ReplaceSkillPageBase:refreshPage(container)
	UserInfo.sync()
	local myLevel = UserInfo.roleInfo.level
	local canCarriedCount = 0
	local skills = UserInfo.roleInfo.skills
	if skills then
		canCarriedCount = #skills
	end

	local lb2Str = {
		mTitle = common:getLanguageString('@ReplaceSkillTitle'),
		mReplaceSkillTex = common:getLanguageString('@ReplaceSkillTex', myLevel, canCarriedCount)
	}
	NodeHelper:setStringForLabel(container, lb2Str) 
end

----------------scrollview-------------------------
function ReplaceSkillPageBase:rebuildAllItem(container)
	self:clearAllItem(container);
	self:buildItem(container);
end

function ReplaceSkillPageBase:clearAllItem(container)
	NodeHelper:clearScrollView(container);
end

function ReplaceSkillPageBase:buildItem(container)
	NodeHelper:buildScrollView(container, #myProfessionSkillCfg, SkillItem.ccbiFile, SkillItem.onFunction)
end

----------------click event------------------------
function ReplaceSkillPageBase:onCancel(container)
	PageManager.popPage(thisPageName);
end

-- 选好技能之后，点击保存，发包。SkillPage.lua 监听回包，并且刷新自己
function ReplaceSkillPageBase:onSave(container)

	UserInfo.sync()
	local roleInfo = UserInfo.roleInfo
	if not roleInfo then return end

	local msg = skillPb.HPSkillCarry()
	msg.roleId = thisRoleId

	local selectedSize = #selectedTable
	local skillOpenCount = #roleInfo.skills
	if not skillOpenCount then return end

	for i=1, skillOpenCount do
		if i <= selectedSize then
			msg.skillId:append(selectedTable[i])
		else
			msg.skillId:append(0)
		end
	end
	msg.skillBagId = thisBagId
	local pb = msg:SerializeToString()
	container:sendPakcet(hp.ROLE_CARRY_SKILL_C, pb, #pb, true)
	PageManager.popPage(thisPageName)
end

function ReplaceSkillPageBase:onReceiveMessage(container)
	local message = container:getMessage()
	local typeId = message:getTypeId()
	if typeId == MSG_MAINFRAME_REFRESH then
		local pageName = MsgMainFrameRefreshPage:getTrueType(message).pageName
		if pageName == thisPageName then
			local posx = container.mScrollView:getContainer():getPositionX();
			local posy = container.mScrollView:getContainer():getPositionY();
			self:rebuildAllItem(container)
			container.mScrollView:getContainer():setPosition(ccp(posx,posy));
		end
	end
end

function ReplaceSkillPage_setBagId( bagId ,roleId )
	thisBagId = bagId	
	if roleId == nil then
		UserInfo.sync()
		thisRoleId = UserInfo.roleInfo.roleId 
	else	
		thisRoleId = roleId
	end
	
end

-------------------------------------------------------------------------
local CommonPage = require("CommonPage");
local ReplaceSkillPage = CommonPage.newSub(ReplaceSkillPageBase, thisPageName, option);
