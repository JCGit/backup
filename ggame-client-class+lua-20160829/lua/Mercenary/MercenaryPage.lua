----------------------------------------------------------------------------------
--[[
	FILE:			MercenaryPage.lua
	ENCODING:		UTF-8, no-bomb
	DESCRIPTION:	能量之核: 探索、拼合、列表页面
	AUTHOR:			hgs
	CREATED:		2014-01-21
--]]
----------------------------------------------------------------------------------
registerScriptPage("MercenarySkillPage");
local trainPage = "MercenaryEnhancePage";
registerScriptPage(trainPage);
local UserInfo = require("UserInfo");
local Player_pb = require "Player_pb"
local RoleOpr_pb = require "RoleOpr_pb"
local thisPageName = "MercenaryPage";
local MercenaryHaloManager = require("MercenaryHaloManager")
local UserMercenaryManager = require("UserMercenaryManager");
local SkillManager = require("SkillManager")
local MercenaryEquipNames = {
	["Chest"]		= Const_pb.CUIRASS,
	["Legs"]		= Const_pb.LEGGUARD,
	["MainHand"]	= Const_pb.WEAPON1,
	["OffHand"]		= Const_pb.WEAPON2
};

local attributeOpenState = true

local containerRef = {}
local COUNT_MERCENARY = 3;

local opcodes = {
	ROLE_FIGHT_S		= HP_pb.ROLE_FIGHT_S,
	EQUIP_DRESS_S		= HP_pb.EQUIP_DRESS_S,
	ROLE_REPLACE_S 		= HP_pb.ROLE_REPLACE_S,
	ROLE_CARRY_SKILL_S	= HP_pb.ROLE_CARRY_SKILL_S
};
local option = {
	ccbiFile = "MercenaryPage.ccbi",
	ccbiPadFile = "MercenaryPage_ipad.ccbi",
	handlerMap = {
		onCulture		= "onCulture",
		onState			= "onJoinBattle",
		onSkillHand1	= "adjustSkill",
		onUpgradeState  = "onUpgradeState",
		onHalo			= "onHalo",
		onSkillHand2	= "adjustSkill",
		onSkillHand3	= "adjustSkill",
		onSkillHand4	= "adjustSkill",
		onAttributeOpen = "onAttributeOpen",
		onHelp			= "onHelp"
	},
	opcode = opcodes
};
for equipName, _ in pairs(MercenaryEquipNames) do
	option.handlerMap["on" .. equipName] = "showEquipDetail";
end

for i = 1, COUNT_MERCENARY do
	option.handlerMap["onHand" .. i] = "showMercenaryDetail";
end

local MercenaryPageBase = {};

local NodeHelper = require("NodeHelper");
local PBHelper	= require("PBHelper");
local ItemManager = require("ItemManager");

local roleCfg = ConfigManager.getRoleCfg();
local skillCfg = ConfigManager.getSkillCfg();
local selectedMercenary = {};

local g_mercenary_current_index = 1;
local needRefresh = false;
local thisRoleId = 0
local tab = 1


local m_BegainX = 0
local m_EndX = 0

-----------------------------------------------
--MercenaryPageBase页面中的事件处理
----------------------------------------------


function MercenaryPageBase:synAndGetRoleInfoByIndex(index)
	local maxSize = ServerDateManager:getInstance():getRoleInfoTotalSize();
	if index > (maxSize-1) then
		CCLuaLog("ERROR in synAndGetRoleInfoByIndex");
		return;
	end
	local mercenaryStr = ServerDateManager:getInstance():getRoleInfoByIndexForLua(index)
	local data = Player_pb.RoleInfo()
	data:ParseFromString(mercenaryStr);
	return data
end


function MercenaryPageBase:onLoad(container)
	local height = CCDirector:sharedDirector():getWinSize().height
	if height<900 then
		container:loadCcbiFile(option.ccbiPadFile);
	else
		container:loadCcbiFile(option.ccbiFile);
	end
	local mScale9Sprite = container:getVarScale9Sprite("mScale9Sprite")
	if mScale9Sprite ~= nil then
		container:autoAdjustResizeScale9Sprite( mScale9Sprite )
	end
end

function MercenaryPageBase:addTouchLayer(container)
	local layerTag = 100302
	local mScale9Sprite = container:getVarScale9Sprite("mScale9Sprite")
	if mScale9Sprite then
		local touchLayer = mScale9Sprite:getParent():getChildByTag(layerTag);
		if not touchLayer then
			touchLayer = CCLayer:create();
			touchLayer:setTag(layerTag);
			mScale9Sprite:getParent():addChild(touchLayer);
			touchLayer:setTouchEnabled(true)			
			touchLayer:setContentSize(CCSize(mScale9Sprite:getContentSize().width,mScale9Sprite:getContentSize().height))
			touchLayer:setPosition(mScale9Sprite:getPosition());
			touchLayer:registerScriptTouchHandler(MercenaryPageBase.onTouchHandler,false,0,false);
		end	
	end
		
end

function MercenaryPageBase:MoveToLeft()
	local newIndex = g_mercenary_current_index -1 
	--index should be in range [1-3] 
	if(newIndex<1) then newIndex= 3 end
	if(newIndex>3) then newIndex = 1 end
	newIndex = math.max(newIndex,1)
	newIndex = math.min(newIndex,3)
	self:changeMercenaryPageByIndex(containerRef,newIndex)
end

function MercenaryPageBase:MoveToRight()
	newIndex = g_mercenary_current_index +1 
	--index should be in range [1-3] 
	if(newIndex<1) then newIndex = 3 end
	if(newIndex>3) then newIndex = 1 end
	newIndex = math.max(newIndex,1)
	newIndex = math.min(newIndex,3)
	self:changeMercenaryPageByIndex(containerRef,newIndex)
end

function MercenaryPageBase.onTouchHandler(eventName,pTouch)
	if eventName == "began" then
		local point = pTouch:getLocationInView();
		point = CCDirector:sharedDirector():convertToGL(point);
		m_BegainX = point.x;
	elseif eventName == "moved" then
		
	elseif eventName == "ended" then
		local point = pTouch:getLocationInView();
		point = CCDirector:sharedDirector():convertToGL(point);
		m_EndX = point.x
		--[[if m_EndX>640 or m_EndX<0 or point.y<svMinHeight or point.y>svMaxHeight  then
			return
		end--]]
		local dis = m_EndX-m_BegainX
		CCLuaLog("m_EndX-m_BegainX"..m_EndX.."m_BegainX"..m_BegainX.."dis.."..dis)
		if dis>200 then
			return MercenaryPageBase:MoveToLeft()
		elseif dis<-200 then
			return MercenaryPageBase:MoveToRight()		
		end
	elseif eventName == "cancelled" then		
	end
end

function MercenaryPageBase:onEnter(container)
	local MercenaryHaloManager = require("MercenaryHaloManager")
	MercenaryHaloManager:classifyGroup(ConfigManager.getMercenaryRingCfg())
	self:registerPacket(container);
	containerRef = container;
	container:registerMessage(MSG_MAINFRAME_POPPAGE);	
	container:registerMessage(MSG_SEVERINFO_UPDATE);
	container:registerMessage(MSG_MAINFRAME_REFRESH);
	----版本前改需求，先这样
	attributeOpenState = true
	container:runAnimation("Open")	
	container:getVarMenuItemImage("mAttributeBtn"):selected()
	----------------------------
	-- 获取当前所有佣兵个数,默认选中第一个
	local totalSize  = ServerDateManager:getInstance():getRoleInfoTotalSize();	
	g_mercenary_current_index = UserMercenaryManager:getFightMercenaryIndex();
	selectedMercenary = self:synAndGetRoleInfoByIndex(g_mercenary_current_index - 1)
	MercenaryPageBase:addTouchLayer(container)
	self:refreshPage(container)	
end

function MercenaryPageBase:onExit(container)
	self:removePacket(container);
	container:removeMessage(MSG_MAINFRAME_POPPAGE)
	container:removeMessage(MSG_SEVERINFO_UPDATE)
	container:removeMessage(MSG_MAINFRAME_REFRESH)
	--清除纹理缓存
	GameUtil:purgeCachedData()
end

function MercenaryPageBase:onHelp( container )
	PageManager.showHelp(GameConfig.HelpKey.HELP_MERCENARY)
end
----------------------------------------------------------------

function MercenaryPageBase:refreshPage(container)
	self:showMercenaryList(container);
	self:showCurrentMercenary(container);
	
	local haloRedPoint = container:getVarNode("mHintNode")
	if haloRedPoint then
		local hasActiveFlag = MercenaryHaloManager:checkHasActivedHaloByMercenary(selectedMercenary)  
		haloRedPoint:setVisible(hasActiveFlag)		
	end
end

function MercenaryPageBase:showCurrentMercenary(container)
	self:showMercenaryInfo(container);
	self:showFightAttrInfo(container);	
	self:showEquipInfo(container);
	self:showSkillInfo(container);
	needRefresh = false;
end

function MercenaryPageBase:isSlotOpen(container,index)
	local levelLimit = {
		 15,
		 20,
		 25
	}
	local level = tonumber(UserInfo.roleInfo.level);
	local isOpen = false
	if level>=levelLimit[index] then
		isOpen = true
	else 
		isOpen = false			
	end
	return isOpen
end

function MercenaryPageBase:showMercenaryList(container)
	local lb2Str = {};
	local sprite2Img = {};
	UserInfo.sync(); 
	local levelLimit = {
		 15,
		 20,
		 25
	}
		
	for i = 1, COUNT_MERCENARY do
		container:getVarSprite("mHand" .. i .. "BG"):setVisible( g_mercenary_current_index == i )
		local isOpen = self:isSlotOpen(container,i);
		local strLimit = levelLimit[i]
		if isOpen then
			lb2Str["mOpenLevel" .. i] = "";		
            local iconIndex = 6 + i	
			sprite2Img["mPic" .. i] = roleCfg[iconIndex]["icon"];
			container:getVarMenuItemImage("mHand" .. i):setEnabled( true )
		else
			lb2Str["mOpenLevel" .. i] = common:getLanguageString("@OpenLevel", tostring(strLimit));
			container:getVarMenuItemImage("mHand" .. i):setEnabled( false )
		end
		
		--sprite2Img["mPic" .. i] = roleCfg[iconIndex]["icon"];
		
	end
	--[[
	for i = 1, COUNT_MERCENARY do
		
	end
	]]--
	
	NodeHelper:setStringForLabel(container, lb2Str);
	NodeHelper:setSpriteImage(container, sprite2Img);
end

function MercenaryPageBase:showMercenaryInfo(container)
	if selectedMercenary == nil then
		return 
	end
			
		local professionStr = FreeTypeConfig[11].content;
		if selectedMercenary.itemId == 7 then
			professionStr = FreeTypeConfig[11].content;
		elseif selectedMercenary.itemId == 8 then
			professionStr = FreeTypeConfig[12].content;
		elseif selectedMercenary.itemId == 9 then
			professionStr = FreeTypeConfig[13].content;		
		end
	local merName = roleCfg[selectedMercenary.itemId]["name"]
	local backgroundPic = GameConfig.MercenaryBackgroundPic[selectedMercenary.itemId]
	container:getVarSprite("mMervenaryPic"):setTexture( backgroundPic )
	local lb2Str = {
		mHpNum 					= PBHelper:getAttrById(selectedMercenary.attribute.attribute, Const_pb.HP),
		mMpNum 					= PBHelper:getAttrById(selectedMercenary.attribute.attribute, Const_pb.MP),
		mLV						= "LV." .. selectedMercenary.level,
		mName				 	= common:getLanguageString("@MercenaryLvName", selectedMercenary.level, merName, professionStr),
		mFightingNum		 	= common:getLanguageString("@FightingNum", selectedMercenary.fight)
	};
	NodeHelper:setStringForLabel(container, lb2Str);
	local step = selectedMercenary.starLevel
	for i = 1,10,1 do
	    if i == 10 then
	        container:getVarSprite("mYellowStar" .. i ):setVisible( i <= step )
	    else
	        container:getVarSprite("mYellowStar0" .. i ):setVisible( i <= step )
	    end
	end
end



function MercenaryPageBase:showFightAttrInfo(container)
	local state = selectedMercenary.status
	local onBattleStr = nil
	if state == Const_pb.FIGHTING then
		onBattleStr = FreeTypeConfig[14].content;
	else
		onBattleStr = FreeTypeConfig[15].content;
	end
	local count = #selectedMercenary.attribute.attribute;
	
	local lb2Str = {
		mStrengthNum 			= PBHelper:getAttrById(selectedMercenary.attribute.attribute, Const_pb.STRENGHT),
		mDamageNum 				= PBHelper:getAttrById(selectedMercenary.attribute.attribute, Const_pb.MINDMG).."~"..PBHelper:getAttrById(selectedMercenary.attribute.attribute, Const_pb.MAXDMG),
		mDexterityNum			= PBHelper:getAttrById(selectedMercenary.attribute.attribute, Const_pb.AGILITY),
		mArmorNum				= PBHelper:getAttrById(selectedMercenary.attribute.attribute, Const_pb.ARMOR),
		mCritRatingNum			= PBHelper:getAttrById(selectedMercenary.attribute.attribute, Const_pb.CRITICAL),
		mIntelligenceNum	 	= PBHelper:getAttrById(selectedMercenary.attribute.attribute, Const_pb.INTELLECT),
		mCreateRoleNum			= PBHelper:getAttrById(selectedMercenary.attribute.attribute, Const_pb.MAGDEF),
		mDodgeNum				= PBHelper:getAttrById(selectedMercenary.attribute.attribute, Const_pb.DODGE),
		mStaminaNum				= PBHelper:getAttrById(selectedMercenary.attribute.attribute, Const_pb.STAMINA),
		mHitRatingNum			= PBHelper:getAttrById(selectedMercenary.attribute.attribute, Const_pb.HIT),
		mTenacityNum 			= PBHelper:getAttrById(selectedMercenary.attribute.attribute, Const_pb.RESILIENCE),
		mPhysicsResistanceNum 	= PBHelper:getAttrById(selectedMercenary.attribute.attribute, Const_pb.PHYDEF),
		mJointBattleLabel  		= onBattleStr
	};
	NodeHelper:setStringForLabel(container, lb2Str);
end

function MercenaryPageBase:showEquipInfo(container)
	local lb2Str = {};
	local sprite2Img = {};
	local itemImg2Qulity = {};
	local nodesVisible = {};
	
	for equipName, part in pairs(MercenaryEquipNames) do	
		local levelStr = "";
		local enhanceLvStr = "";
		local icon = GameConfig.Image.ClickToSelect;
		local quality = GameConfig.Default.Quality;
		local aniVisible = false;
		local gemVisible = false;
		local showNotice = UserEquipManager:isPartNeedNotice(part, selectedMercenary.roleId);
		
		local name 	= "m" .. equipName;
		local roleEquip = PBHelper:getRoleEquipByPart(selectedMercenary.equips, part);
		if roleEquip then
			local equipId = roleEquip.equipItemId;
			levelStr = "Lv" .. EquipManager:getLevelById(equipId);
			enhanceLvStr = "+" .. roleEquip.strength;
			icon = EquipManager:getIconById(equipId);
			quality = EquipManager:getQualityById(equipId);
			
			aniVisible = UserEquipManager:isGodly(roleEquip.equipId);
			local userEquip = UserEquipManager:getUserEquipById(roleEquip.equipId);		
			local gemInfo = PBHelper:getGemInfo(userEquip.gemInfos);
			if table.maxn(gemInfo) > 0 then
				gemVisible = true;
				for i = 1, 4 do
					local gemId = gemInfo[i];
					nodesVisible[name .. "GemBG" .. i] = gemId ~= nil;
					local gemSprite = name .. "Gem0" .. i;
					nodesVisible[gemSprite] = false;
					if gemId ~= nil and gemId > 0 then
						local icon = ItemManager:getGemSmallIcon(gemId);
						if icon then
							nodesVisible[gemSprite] = true;
							sprite2Img[gemSprite] = icon;
						end
					end
				end
			end
		end
		
		lb2Str[name .. "Lv"] 			= levelStr;
		lb2Str[name .. "LvNum"]			= enhanceLvStr;
		sprite2Img[name .. "Pic"] 		= icon;
		itemImg2Qulity[name] 			= quality;
		--scaleMap[name .. "Pic"] 		= 1.0;
		nodesVisible[name .. "Ani"]		= aniVisible;
		nodesVisible[name .. "GemNode"]	= gemVisible;
		nodesVisible[name .. "Point"] 	= showNotice;
		
		NodeHelper:addEquipAni(container, name .. "Ani", aniVisible, roleEquip and roleEquip.equipId);
	end
	
	NodeHelper:setStringForLabel(container, lb2Str);
	NodeHelper:setSpriteImage(container, sprite2Img);
	NodeHelper:setQualityFrames(container, itemImg2Qulity);
	NodeHelper:setNodesVisible(container, nodesVisible);
end
	
function MercenaryPageBase:showSkillInfo(container)
	
	local skills = SkillManager:getMerEquipSkillListByMerId( selectedMercenary.roleId )
	
	for i = 1 , 4 ,1 do
		if i <= #skills then
			container:getVarMenuItemImage( "mSkillHand" .. i ):setEnabled( true )
			if skills[i] > 0 then
				local itemInfo = SkillManager:getSkillInfoBySkillId( skills[i] )
				container:getVarSprite("mSkillHandPic" .. i ):setTexture(itemInfo.icon)
			else
				container:getVarSprite("mSkillHandPic" .. i ):setTexture(GameConfig.MercenarySkillState.canSelect)
			end	
		else
			container:getVarSprite("mSkillHandPic" .. i ):setTexture(GameConfig.MercenarySkillState.forbidden)
			container:getVarMenuItemImage( "mSkillHand" .. i ):setEnabled( false )
		end
	end
	
end	

function MercenaryPageBase:onAttributeOpen( container )
	if attributeOpenState then
		container:runAnimation("Close")
		container:getVarMenuItemImage("mAttributeBtn"):unselected()
	else
		container:getVarMenuItemImage("mAttributeBtn"):selected()
		container:runAnimation("Open")	
	end
	attributeOpenState = not attributeOpenState
end

----------------click event------------------------
function MercenaryPageBase:changeMercenaryPageByIndex(container,index)
	local isOpen = self:isSlotOpen(container,index)	
	for i = 1, COUNT_MERCENARY do
		container:getVarSprite("mHand" .. i .. "BG"):setVisible( index == i )
	end
	
	
	if g_mercenary_current_index == index then
		RegisterLuaPage("MercenaryPopPage");
		MercenaryPop_SetRoleId(selectedMercenary.itemId)
		PageManager.pushPage("MercenaryPopPage")
		return;
	end
	
	if isOpen then
		selectedMercenary = self:synAndGetRoleInfoByIndex(index-1)
		if selectedMercenary~=nil then
			g_mercenary_current_index = index
			self:refreshPage(container)
		else
			MessageBoxPage:Msg_Box_Lan("@LevelForMercenaryNotOpen")
		end
	else
		MessageBoxPage:Msg_Box_Lan("@LevelForMercenaryNotOpen")
	end
end

function MercenaryPageBase:showMercenaryDetail(container,eventName)
	
	local index = tonumber(string.sub(eventName, -1))
	self:changeMercenaryPageByIndex(container,index)
	
end

function MercenaryPageBase:onCulture(container)
	MercenaryEnhancePage_setRoleId(selectedMercenary.roleId);
	PageManager.pushPage(trainPage);
end

function MercenaryPageBase:onJoinBattle(container)
	if selectedMercenary~=nil then
		local message = RoleOpr_pb.HPRoleFight();
	
		if message~=nil then	
			message.roleId = selectedMercenary.roleId;			
			local pb_data = message:SerializeToString();
			PacketManager:getInstance():sendPakcet(HP_pb.ROLE_FIGHT_C,pb_data,#pb_data,false);						
			
		end
	end
end

function MercenaryPageBase:showEquipDetail(container, eventName)
	local partName = string.sub(eventName, 3);
	local part = MercenaryEquipNames[partName];
	UserEquipManager:cancelNotice(part, selectedMercenary.roleId);
	NodeHelper:setNodesVisible(container, {["m" .. partName .. "Point"] = false});
	local roleEquip = UserMercenaryManager:getEquipByPart(selectedMercenary.roleId, part);
	if roleEquip then
		PageManager.showEquipInfo(roleEquip.equipId, selectedMercenary.roleId);
	else
		EquipSelectPage_setPart(part, selectedMercenary.roleId);
		PageManager.pushPage("EquipSelectPage");
	end
end

--回包处理

function MercenaryPageBase:onReceivePacket(container)
	local opcode = container:getRecPacketOpcode()
	local msgBuff = container:getRecPacketBuffer()

	if opcode == opcodes.ROLE_FIGHT_S then
		--刷新出战、休息按钮文字
		selectedMercenary = self:synAndGetRoleInfoByIndex(g_mercenary_current_index-1);
		self:showFightAttrInfo(container);
		return;
	end	
	
	if opcode == opcodes.EQUIP_DRESS_S then
		selectedMercenary = self:synAndGetRoleInfoByIndex(g_mercenary_current_index-1);
		self:showCurrentMercenary(container);
		return;
	end
	
	if opcode == opcodes.ROLE_REPLACE_S then
		needRefresh = true;
	end
end

function MercenaryPageBase:onReceiveMessage(container)
	local message = container:getMessage();
	local typeId = message:getTypeId();
	if typeId == MSG_MAINFRAME_POPPAGE then
		local pageName = MsgMainFramePopPage:getTrueType(message).pageName;
		if pageName == trainPage and needRefresh then
			selectedMercenary = self:synAndGetRoleInfoByIndex(g_mercenary_current_index-1);
			self:showCurrentMercenary(container);
		end
	elseif typeId == MSG_SEVERINFO_UPDATE then
		local opcode = MsgSeverInfoUpdate:getTrueType(message).opcode;
		if opcode == HP_pb.ROLE_INFO_SYNC_S then
			selectedMercenary = self:synAndGetRoleInfoByIndex(g_mercenary_current_index-1);
			self:showCurrentMercenary(container);
		end
	elseif typeId == MSG_MAINFRAME_REFRESH then
		local pageName = MsgMainFrameRefreshPage:getTrueType(message).pageName;
		if pageName == thisPageName then
			selectedMercenary = self:synAndGetRoleInfoByIndex(g_mercenary_current_index-1);
			self:showCurrentMercenary(container);
		elseif	pageName == "MercenaryPage_RefreshSkill" then
			self:showSkillInfo( container )
		end
	end
end

function MercenaryPageBase:registerPacket(container)
	for key, opcode in pairs(opcodes) do
		if string.sub(key, -1) == "S" then
			container:registerPacket(opcode)
		end
	end
end

function MercenaryPageBase:removePacket(container)
	for key, opcode in pairs(opcodes) do
		if string.sub(key, -1) == "S" then
			container:removePacket(opcode)
		end
	end
end

function MercenaryPageBase:onUpgradeState( container )
	MercenaryUpStepPage_SetMercenary(  g_mercenary_current_index - 1 )
	PageManager.pushPage("MercenaryUpStepPage")
end	

function MercenaryPageBase:onHalo( container )
	RegisterLuaPage("MercenaryHaloPage")
	MercenaryHaloPage_setSelectedMer(selectedMercenary)
	PageManager.pushPage("MercenaryHaloPage")
end

function MercenaryPageBase:adjustSkill( container )
	if selectedMercenary == nil then
		return 
	end
	local roleId = selectedMercenary.roleId
	MercenarySkillPage_SetInfo( roleId ,g_mercenary_current_index - 1 )
	PageManager.pushPage("MercenarySkillPage")
end	



-------------------------------------------------------------------------
local CommonPage = require("CommonPage");
MercenaryPage = CommonPage.newSub(MercenaryPageBase, thisPageName, option);

function MainFrame_onMercenaryPageBtn()
    UserInfo.sync()
    if UserInfo.roleInfo.level < GameConfig.LevelLimit.MecenaryLvLimit then
        MessageBoxPage:Msg_Box("@MercenaryLevelNotEnough")
    else
    	PageManager.changePage("MercenaryPage");
    end
	return 0;
end