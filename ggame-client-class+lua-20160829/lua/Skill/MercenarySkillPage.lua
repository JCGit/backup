----------------------------------------------------------------------------------
--[[
FILE:			MercenarySkillPage.lua
ENCODING:		UTF-8, no-bomb
DESCRIPTION:	佣兵技能界面
AUTHOR:			dmq
CREATED:		2014-10-23
--]]
----------------------------------------------------------------------------------
registerScriptPage("ReplaceMercenarySkillPage");
local hp = require('HP_pb')
local UserInfo = require("UserInfo");
local skillPb = require("Skill_pb")
local SkillManager = require("SkillManager")
local NewbieGuideManager = require("NewbieGuideManager")

local thisPageName = "MercenarySkillPage";
local NodeHelper = require("NodeHelper");
local thisIndex = 1
local option = {
	ccbiFile = "MercenarySkillPopUp.ccbi",
	handlerMap = {
		onHelp				= "showHelp",
		onSkillspecialty	= "enhanceSkill",
		onReplaceSkill		= "replaceSkill",
		onClose				= "onClose",
		onReplace 			= "onReplace"
	},
	opcode = opcodes
}

local skillCfg = ConfigManager.getSkillCfg();
local skillOpenCfg = ConfigManager.getMercenarySkillOpenCfg()

local SkillPageBase = {}
local SkillPageNormalContent = {}
local SkillPageEmptyContent = {}
local thisRoleId = 0
--------------------------------------------------------------
local SkillItem = {
	ccbiFile_empty 	= "MercenarySkillEmptyContent.ccbi",
	ccbiFile_close	 	= "MercenarySkillNotOpenContent.ccbi",
	ccbiFile_open	 	= "MercenarySkillOpenContent.ccbi",
}


----------------------------------------------------------------------------------

-----------------------------------------------
--SkillPageBase页面中的事件处理
----------------------------------------------
function SkillPageBase:onEnter(container)
	NewbieGuideManager.guide(GameConfig.NewbieGuide.FirstSkill)
	self:registerPackets(container)
	container:registerMessage(MSG_MAINFRAME_REFRESH)
	NodeHelper:initScrollView(container, "mContent", 3);
	container.scrollview=container:getVarScrollView("mContent");	
	
	if container.scrollview~=nil then
		--container:autoAdjustResizeScrollview(container.scrollview);
	end		
	
	local mScale9Sprite = container:getVarScale9Sprite("mScale9Sprite")
	if mScale9Sprite ~= nil then
		container:autoAdjustResizeScale9Sprite( mScale9Sprite )
	end
	self:rebuildAllItem(container);
end

function SkillPageBase:onExecute(container)
end

function SkillPageBase:onExit(container)
	self:removePackets(container)
	container:removeMessage(MSG_MAINFRAME_REFRESH)
	NodeHelper:deleteScrollView(container);
end		

function SkillPageBase:onClose( container )
	PageManager.popPage( thisPageName )
end

function SkillPageBase:onReplace( container )
	ReplaceMercenarySkillPage_setBagId(thisRoleId , thisIndex)
	PageManager.pushPage("ReplaceMercenarySkillPage")
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
	local assignedSkills = SkillManager:getMerEquipSkillListByMerId(thisRoleId)
	
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
				label:setString(common:getLanguageString('@MercenaryOpenLevel', openLevel))
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
	PageManager.showHelp(GameConfig.HelpKey.HELP_MERCENARYSKILL)
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
	    ReplaceMercenarySkillPage_setBagId(thisRoleId , thisIndex)
		PageManager.pushPage("ReplaceMercenarySkillPage")
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
	local assignedSkills = SkillManager:getMerEquipSkillListByMerId(thisRoleId)
	
	local msg = skillPb.HPSkillChangeOrder()
	msg.roleId = thisRoleId
	msg.skillId = assignedSkills[index]
	if index < 2 then MessageBoxPage:Msg_Box('@error index in skill reorder') return end
	msg.srcOrder = index - 1
	msg.dstOrder = index - 2
	msg.skillBagId = 1
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
			PageManager.refreshPage("MercenaryPage_RefreshSkill")
		end
	end
end

function MercenarySkillPage_SetInfo( roleId ,index )
	thisRoleId = roleId
	thisIndex = index
end

-------------------------------------------------------------------------
local CommonPage = require("CommonPage");
SkillPage = CommonPage.newSub(SkillPageBase, thisPageName, option)
