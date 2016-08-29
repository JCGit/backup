----------------------------------------------------------------------------------
--[[
FILE:			SkillPage.lua
ENCODING:		UTF-8, no-bomb
DESCRIPTION:	能量之核: 探索、拼合、列表页面
AUTHOR:			hgs
CREATED:		2014-01-21
--]]
----------------------------------------------------------------------------------
registerScriptPage("ReplaceSkillPage");
local hp = require('HP_pb')
local skillPb = require("Skill_pb")
local SkillManager = require("SkillManager")
local NewbieGuideManager = require("NewbieGuideManager")
local UserInfo = require("UserInfo");
local thisPageName = "SkillPage";
local NodeHelper = require("NodeHelper");

local SkillExPage = require("SkillExPage")

local option = {
	ccbiFile = "SkillPage.ccbi",
	handlerMap = {
		onHelp				= "showHelp",
		onSkillspecialty	= "enhanceSkill",
		onReplaceSkill		= "replaceSkill",
		onFightSkill		= "onFightSkill",
		onArenaSkill		= "onArenaSkill",
		onDefenseSkill	 	= "onDefenseSkill"
		
	},
	opcode = opcodes
}

local skillCfg = ConfigManager.getSkillCfg();
local skillOpenCfg = ConfigManager.getSkillOpenCfg()

local SkillPageBase = {}
local SkillPageNormalContent = {}
local SkillPageEmptyContent = {}

--------------------------------------------------------------
local SkillItem = {
	ccbiFile_empty 	= "SkillEmptyContent.ccbi",
	ccbiFile_close	 	= "SkillNotOpenContent.ccbi",
	ccbiFile_open	 	= "SkillOpenContent.ccbi",
}

local PageType = {
    FIGHT_SKILL = 1,
    ARENA_SKILL = 2,
	DEFENSE_SKILL = 3
}

local currPageType = PageType.ARENA_SKILL

----------------------------------------------------------------------------------

-----------------------------------------------
--SkillPageBase页面中的事件处理
----------------------------------------------
function SkillPageBase:onEnter(container)
	NewbieGuideManager.guide(GameConfig.NewbieGuide.FirstSkill)
	container:registerMessage(MSG_MAINFRAME_REFRESH)
	self:registerPackets(container)
	NodeHelper:initScrollView(container, "mContent", 3);
	container.scrollview=container:getVarScrollView("mContent");	
	
	if container.scrollview~=nil then
		container:autoAdjustResizeScrollview(container.scrollview);
	end		
	
	local mScale9Sprite = container:getVarScale9Sprite("mScale9Sprite")
	if mScale9Sprite ~= nil then
		container:autoAdjustResizeScale9Sprite( mScale9Sprite )
	end
	self:selectTab(container)
	self:rebuildAllItem(container);
end

function SkillPageBase:onExecute(container)
end

function SkillPageBase:onExit(container)
	self:removePackets(container)
	container:removeMessage(MSG_MAINFRAME_REFRESH)
	NodeHelper:deleteScrollView(container);
end

function SkillPageBase:selectTab( container )
	if currPageType == PageType.FIGHT_SKILL then
		container:getVarMenuItem("mFightSkill"):selected()
		container:getVarMenuItem("mFightSkill"):setEnabled(false)
		container:getVarMenuItem("mArenaSkill"):unselected()
		container:getVarMenuItem("mArenaSkill"):setEnabled(true) 
		container:getVarMenuItem("mDefenseSkill"):unselected()
		container:getVarMenuItem("mDefenseSkill"):setEnabled(true)
		
		container:getVarNode("mExplain1"):setVisible(true)
		container:getVarNode("mExplain2"):setVisible(false)
		container:getVarNode("mExplain3"):setVisible(false)
	elseif currPageType == PageType.ARENA_SKILL then
		container:getVarMenuItem("mFightSkill"):unselected()
		container:getVarMenuItem("mFightSkill"):setEnabled(true)
		container:getVarMenuItem("mArenaSkill"):selected() 
		container:getVarMenuItem("mArenaSkill"):setEnabled(false)
		container:getVarMenuItem("mDefenseSkill"):unselected()
		container:getVarMenuItem("mDefenseSkill"):setEnabled(true)
		
		container:getVarNode("mExplain1"):setVisible(false)
		container:getVarNode("mExplain2"):setVisible(true)
		container:getVarNode("mExplain3"):setVisible(false)
	else
		container:getVarMenuItem("mFightSkill"):unselected()
		container:getVarMenuItem("mFightSkill"):setEnabled(true)
		container:getVarMenuItem("mArenaSkill"):unselected() 
		container:getVarMenuItem("mArenaSkill"):setEnabled(true)
		container:getVarMenuItem("mDefenseSkill"):selected()
		container:getVarMenuItem("mDefenseSkill"):setEnabled(false)
		
		container:getVarNode("mExplain1"):setVisible(false)
		container:getVarNode("mExplain2"):setVisible(false)
		container:getVarNode("mExplain3"):setVisible(true)
	end
end

function SkillPageBase:onFightSkill( container )
	currPageType = PageType.FIGHT_SKILL
	self:selectTab( container )
	self:rebuildAllItem(container)
end

function SkillPageBase:onArenaSkill( container )
	currPageType = PageType.ARENA_SKILL
	self:selectTab( container )
	self:rebuildAllItem(container)
end

function SkillPageBase:onDefenseSkill( container )
	currPageType = PageType.DEFENSE_SKILL
	self:selectTab( container )
	self:rebuildAllItem( container )
end

----------------scrollview-------------------------
function SkillPageBase:rebuildAllItem(container)
	self:clearAllItem(container);
	self:buildItem(container);
end

function SkillPageBase:clearAllItem(container)
	NodeHelper:clearScrollView(container);
end

function SkillPageBase:buildItem(container)
	local iCount = 0
	local iMaxNode = container.m_pScrollViewFacade:getMaxDynamicControledItemViewsNum()
	local fOneItemHeight = 0
	local fOneItemWidth = 0
	
	--人物挂上技能的清单在RoleInfo里面的RoleSkill数组中
	UserInfo.sync()
	local assignedSkills
	if currPageType == PageType.FIGHT_SKILL then
		assignedSkills = SkillManager:getFightSkillList()
	elseif currPageType == PageType.ARENA_SKILL then
		assignedSkills = SkillManager:getArenaSkillList()
	elseif currPageType == PageType.DEFENSE_SKILL then
		assignedSkills = SkillManager:getDefenseSkillList()
	end
	
	if assignedSkills == nil then
		CCLuaLog("ERROR  in assignedSkills == null");
		return
	end
	
	--这里传过来几个表示开启了多少个，如果传过来的skillid = 0表示该位置没有放技能
	--如人物30级，开启了3个技能孔，但只有2个技能上阵
	--skillId是数据库的id
	--则传过来的是  {1001,1002,0}，skillId =0 对应  ccbiFile_empty
	--另外一个没开启的技能孔用ccbiFile_close
	local skillSize = #assignedSkills
	
	--肯定为4个位置		
	for i=4, 1, -1 do
		local pItemData = CCReViSvItemData:new_local()
		pItemData.mID = i
		pItemData.m_iIdx = i
		pItemData.m_ptPosition = ccp(0, fOneItemHeight * iCount)

		local pItem = {}
		if i<= skillSize then
			local skillItemId = SkillManager:getSkillItemIdUsingId(assignedSkills[i])
			-- create pItem
			if skillItemId > 0 then
				-- carried
				local ccbiFile = SkillItem.ccbiFile_open
				pItem = ScriptContentBase:create(ccbiFile)
				local newStr =GameMaths:stringAutoReturnForLua(skillCfg[skillItemId]["describe"], 17, 0)
				local lb2Str = {
					mSkillName 		= skillCfg[skillItemId]["name"],
					mConsumptionMp 	= skillCfg[skillItemId]["costMP"],
					mSkillTex 		= newStr,
					mNumber			= tostring(i)
				}
				NodeHelper:setStringForLabel(pItem, lb2Str);					
				local sprite2Img = {
					mChestPic = skillCfg[skillItemId]["icon"],
				}
				NodeHelper:setSpriteImage(pItem, sprite2Img);
				if i == 1 then
					pItem:getVarMenuItemImage("mMobile"):setEnabled(false)
				end
				pItem:registerFunctionHandler(SkillPageNormalContent.onFunction)
			else
				-- empty 
				--显示已开启
				local ccbiFile = SkillItem.ccbiFile_empty;
				pItem = ScriptContentBase:create(ccbiFile);
				pItem:registerFunctionHandler(SkillPageEmptyContent.onFunction)
			end
		else -- not open skills
			local ccbiFile = SkillItem.ccbiFile_close;
			pItem = ScriptContentBase:create(ccbiFile);
			local label = pItem:getVarLabelBMFont('mOpenLevel')
			if label then
				local openLevel = skillOpenCfg[i] and skillOpenCfg[i].openLevel or 0
				label:setString(common:getLanguageString('@OpenLevel', openLevel))
			end
		end

		if fOneItemHeight < pItem:getContentSize().height then
			fOneItemHeight = pItem:getContentSize().height
		end
		if fOneItemWidth < pItem:getContentSize().width then
			fOneItemWidth = pItem:getContentSize().width
		end

		if iCount < iMaxNode then
			container.m_pScrollViewFacade:addItem(pItemData, pItem.__CCReViSvItemNodeFacade__)
		else
			container.m_pScrollViewFacade:addItem(pItemData)
		end
		iCount = iCount + 1
	end

	local size = CCSizeMake(fOneItemWidth, fOneItemHeight * iCount)
	container.mScrollView:setContentSize(size)
	container.mScrollView:setContentOffset(ccp(0, container.mScrollView:getViewSize().height - container.mScrollView:getContentSize().height * container.mScrollView:getScaleY()))
	container.m_pScrollViewFacade:setDynamicItemsStartPosition(iCount - 1);
	container.mScrollView:forceRecaculateChildren();
end

----------------click event------------------------
function SkillPageBase:showHelp(container)
	PageManager.showHelp(GameConfig.HelpKey.HELP_SKILL)
end

--技能专精，暂未开放
function SkillPageBase:enhanceSkill(container)
	MessageBoxPage:Msg_Box('@CommingSoon')
end

function SkillPageBase:replaceSkill(container)
	ReplaceSkillPage_setBagId(currPageType)
	PageManager.pushPage("ReplaceSkillPage")
end

-- ----------------------- empty content----------------------- 
function SkillPageEmptyContent.onFunction(eventName,container)
	if eventName == "luaRefreshItemView" then
		SkillPageEmptyContent.refreshItemView(container)
	elseif eventName == "mSkillBtn" then
	    ReplaceSkillPage_setBagId(currPageType)
		PageManager.pushPage("ReplaceSkillPage")
	end
	
end

function SkillPageEmptyContent.refreshItemView(container)
end
-- ----------------------- open content----------------------- 
function SkillPageNormalContent.onFunction(eventName,container)
	if eventName == "luaRefreshItemView" then
	elseif eventName == "onMobile" then
		SkillPageNormalContent.onMobile(container);
	end
end

function SkillPageNormalContent.onMobile(container)
	local index = container:getItemDate().mID
	UserInfo.sync()
	local assignedSkills
	if currPageType == PageType.FIGHT_SKILL then
		assignedSkills = SkillManager:getFightSkillList()
	elseif currPageType == PageType.ARENA_SKILL then
		assignedSkills = SkillManager:getArenaSkillList()
	elseif currPageType == PageType.DEFENSE_SKILL then
		assignedSkills = SkillManager:getDefenseSkillList()
	end
	local msg = skillPb.HPSkillChangeOrder()
	msg.roleId = UserInfo.roleInfo.roleId
	msg.skillId = assignedSkills[index]
	if index < 2 then MessageBoxPage:Msg_Box('@error index in skill reorder') return end
	msg.srcOrder = index - 1
	msg.dstOrder = index - 2
	msg.skillBagId = currPageType
	local pb = msg:SerializeToString()
	PacketManager:getInstance():sendPakcet(hp.SKILL_CHANGE_ORDER_C, pb, #pb, true)
end
------------------------------- packet function -----------------------------------------

function SkillPageBase:registerPackets(container)
	container:registerPacket(hp.ROLE_CARRY_SKILL_S)
	container:registerPacket(hp.SKILL_CHANGE_ORDER_S)
end

function SkillPageBase:removePackets(container)
	container:removePacket(hp.ROLE_CARRY_SKILL_S)
	container:removePacket(hp.SKILL_CHANGE_ORDER_S)
end

function SkillPageBase:onReceivePacket(container)
	local opcode = container:getRecPacketOpcode()
	local msgBuff = container:getRecPacketBuffer()

	if opcode == hp.SKILL_CHANGE_ORDER_S then
		self:rebuildAllItem(container)
		return
	end

	if opcode == hp.ROLE_CARRY_SKILL_S then
		self:rebuildAllItem(container)
		return
	end
end

function SkillPageBase:onReceiveMessage(container)
	local message = container:getMessage()
	local typeId = message:getTypeId()
	if typeId == MSG_MAINFRAME_REFRESH then
		local pageName = MsgMainFrameRefreshPage:getTrueType(message).pageName;
		if pageName == thisPageName then
			self:rebuildAllItem(container)
		end
	end
end

-------------------------------------------------------------------------
------------------------------------------页面开关

local isUseScrollTab = VaribleManager:getInstance():getSetting("IsUseNewSkillPage") or 0
isUseScrollTab = tonumber(isUseScrollTab)

--------------------------------------------------
local CommonPage = require("CommonPage");
if isUseScrollTab == 0 then
    SkillPage = CommonPage.newSub(SkillPageBase, thisPageName, option)
else  
    SkillPage = CommonPage.newSub(SkillExPage, thisPageName, option)
end




function MainFrame_onSkillPageBtn()
	PageManager.changePage(thisPageName);
	PageManager.showRedNotice("Skill", false);
	return 0;
end
