----------------------------------------------------------------------------------
--[[
FILE:			ReplaceSkillPage.lua
ENCODING:		UTF-8, no-bomb
DESCRIPTION:	����Ӷ������
AUTHOR:			zhenhui
CREATED:		2014-08-6
--]]
----------------------------------------------------------------------------------
local thisPageName = "ReplaceMercenarySkillPage"
local thisCurIndex = 1
local hp = require('HP_pb')
local UserInfo = require("UserInfo");
local skillPb = require("Skill_pb")
local SkillManager = require("SkillManager")
local option = {
	ccbiFile = "ReplaceSkillPopUp.ccbi",
	handlerMap = {
		onCancel				= "onCancel",
		onClose				= "onCancel",
		onPreservation		= "onSave"
	}
}

local thisMercenaryInfo = {}

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
	-- ��ӳ��������Ҳ�����itemId, ����˵�������û�п���
	if skillItemIdMap[skill.id] == nil then
		-- user level too low, skill closed
		NodeHelper:setNodeVisible(container:getVarNode('ConsumptionMpNode'), false)
		NodeHelper:setNodeVisible(container:getVarNode('mChoiceBtnNode'), false)
		NodeHelper:setNodeVisible(container:getVarLabelBMFont('mLevelLimit'), true)
		local limitStr = common:getLanguageString('@MercenarySkillOpenAtLevel', skillOpenLevel)
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
			--����������������������а�ťΪfalse
			local count = #selectedTable			
			if count == maxSlotSize then
				--��ѡ������������������ҳ���content��Ϊ����ѡ
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

	--����Ѿ��ڶ������ˣ���ɾ��������������ȥ
	if SkillItem.isInTable(selectedTable,skillId) then
		SkillItem.removeItemFromTable(selectedTable,skillId);		
	else
		--������ڶ����������У�ͬʱ�жϿ�ѡtable�Ƿ�������
		table.insert(selectedTable,skillId)		
	end
	
	--ReplaceSkillPageBase:rebuildAllItem(containerRef)
	--ˢ��ҳ��
	PageManager.refreshPage(thisPageName)	
end

----------------------------------------------------------------------------------

-----------------------------------------------
--ReplaceSkillPageBaseҳ���е��¼�����
----------------------------------------------
function ReplaceSkillPageBase:onEnter(container)
	containerRef = container

	container:registerMessage(MSG_MAINFRAME_REFRESH)
	
	NodeHelper:initScrollView(container, "mContent", 4)	
	
	UserInfo.sync()
	--����Ӷ������
	SkillManager:classifyMerSkill( skillCfg )
	
	assignedSkills = SkillManager:getEquipSkillListByRoleIdAndBagId(thisRoleId)
			
	if assignedSkills == nil then
		CCLuaLog("ERROR  in assignedSkills == null");
		return
	end
	--��࿪�ŵĿ����������Ǽ���Я����
	maxSlotSize = #assignedSkills

	-- ��������selectedTable
	self:setSelectedTable()
	--��ȡӶ����Ϣ
	self:getMercenaryInfo( container )
	
	-- ��skillCfg��ȡ����ְҵ��cfg
	--self:getMyProfessionSkillCfg(thisMercenaryInfo.itemId)
	myProfessionSkillCfg = SkillManager:getMerAllSkillByRoleId( thisMercenaryInfo.itemId )

	-- ��cfg���itemId��SkillInfo�����id��������
	self:fillSkillItemIdMap()

	self:refreshPage(container)
	self:rebuildAllItem(container)
end

----��ȡӶ����Ϣ

function ReplaceSkillPageBase:getMercenaryInfo( container )
	local mercenaryStr = ServerDateManager:getInstance():getRoleInfoByIndexForLua(thisCurIndex)
	local data = Player_pb.RoleInfo()
	data:ParseFromString(mercenaryStr)
	thisMercenaryInfo =  data
end

-- ��SkillInfo����ļ���id��cfg�����skillItemId��������
-- û�й�������itemId������ʾû�п�����
function ReplaceSkillPageBase:fillSkillItemIdMap()
	skillItemIdMap = {}
	
	local openSkillList = SkillManager:getOpenSkillListByRoleId( thisRoleId )
	
	for k,v in pairs(openSkillList) do
		skillItemIdMap[v.itemId] = v.id		
	end
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
	local myLevel = thisMercenaryInfo.starLevel
	local canCarriedCount = 0
	local skills = SkillManager:getEquipSkillListByRoleIdAndBagId(thisRoleId)
	if skills then
		canCarriedCount = #skills
	end

	local lb2Str = {
		mTitle = common:getLanguageString('@ReplaceMercenarySkillTitle'),
		mReplaceSkillTex = common:getLanguageString('@ReplaceMercenarySkillTex', myLevel, canCarriedCount)
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

-- ѡ�ü���֮�󣬵�����棬������SkillPage.lua �����ذ�������ˢ���Լ�
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
	msg.skillBagId = 1
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

function ReplaceMercenarySkillPage_setBagId( roleId ,curIndex)
	thisRoleId = roleId
	thisCurIndex = curIndex
end

-------------------------------------------------------------------------
local CommonPage = require("CommonPage");
local ReplaceSkillPage = CommonPage.newSub(ReplaceSkillPageBase, thisPageName, option);
