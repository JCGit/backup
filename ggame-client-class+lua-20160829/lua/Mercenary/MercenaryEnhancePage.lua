----------------------------------------------------------------------------------
--[[
FILE:			MercenaryEnhancePage.lua
ENCODING:		UTF-8, no-bomb
DESCRIPTION:	换技能
AUTHOR:			zhenhui
CREATED:		2014-08-6
--]]
----------------------------------------------------------------------------------

local thisPageName = "MercenaryEnhancePage"

local HP_pb = require("HP_pb");
local RoleOpr_pb = require("RoleOpr_pb");
local UserInfo = require("UserInfo");
local opcodes = {
	ROLE_BAPTIZE_C = HP_pb.ROLE_BAPTIZE_C,
	ROLE_BAPTIZE_S = HP_pb.ROLE_BAPTIZE_S,
	ROLE_REPLACE_C = HP_pb.ROLE_REPLACE_C,
	ROLE_REPLACE_S = HP_pb.ROLE_REPLACE_S
};
local option = {
	ccbiFile = "MercenaryEnhancePopUp.ccbi",
	handlerMap = {
		onCancel		= "onCancel",
		onClose			= "onClose",
		onConfirm		= "onSave",
		onTrainee		= "onCommonTrain",
		onNormal		= "onGoldTrainNormal",
		onPro			= "onGoldTrainMedium",
		onMaster		= "onGoldTrainSenior"
	},
	opcode = opcodes
};

local TrainType = {
	Coin 		= 1,
	GoldNormal	= 2,
	GoldMedium	= 3,
	GoldSenior	= 4
};
local attr2Index = {
	mStrengthNum 		= 1,
	mDexterityNum		= 2,
	mIntelligenceNum	= 3,		
	mStaminaNum			= 4
};

local MercenaryEnhancePageBase = {}
local PBHelper = require("PBHelper");
local NodeHelper = require("NodeHelper");
local UserMercenaryManager = require("UserMercenaryManager");
--------------------------------------------------------------

local roleCfg = ConfigManager:getRoleCfg();

--确定，取消按钮
local enhanceNode = {}
--普通、钻石等按钮
local beforeEnhanceNode = {};

local thisRoleId = 0;
local userMercenary = {};
local currentAttrs = {};
local vipTab = {}

--如果是至尊培养
local mIsAfterSenior = false
----------------------------------------------------------------------------------

-----------------------------------------------
--MercenaryEnhancePageBase页面中的事件处理
----------------------------------------------
function MercenaryEnhancePageBase:onEnter(container)
	self:registerPacket(container)
	UserInfo.sync()
	vipTab = ConfigManager.getVipCfg()
	beforeEnhanceNode = container:getVarNode("mBeforeEnhanceBtn")
	enhanceNode = container:getVarNode("mEnhanceBtn");	
	
	self:showTrainConfirm(false);
	self:refreshBasicPage(container);	
end

function MercenaryEnhancePageBase:showTrainConfirm(doShow)
	beforeEnhanceNode:setVisible(not doShow);
	enhanceNode:setVisible(doShow);
end

function MercenaryEnhancePageBase:showCost(container)
	local coinCost = tonumber(GameConfig.Cost.RoleTrain.Common) * userMercenary.level;
	local goldStr = common:getLanguageString("@Gold")
	local lb2Str = {
		mCostCoin 	= coinCost .. common:getLanguageString("@Coin"),
		mCostGold1	= GameConfig.Cost.RoleTrain.GoldNormal .. goldStr,
		mCostGold2	= GameConfig.Cost.RoleTrain.GoldMedium .. goldStr,
		mCostGold3	= GameConfig.Cost.RoleTrain.GoldSenior .. goldStr
	};
	NodeHelper:setStringForLabel(container, lb2Str);
end

function MercenaryEnhancePageBase:onExit(container)
	self:removePacket(container);
	beforeEnhanceNode = nil;
	enhanceNode = nil;
end
----------------------------------------------------------------
function MercenaryEnhancePageBase:refreshLeftPanel(container)
	userMercenary = UserMercenaryManager:getUserMercenaryById(thisRoleId);
	currentAttrs = {
		PBHelper:getAttrById(userMercenary.baseAttr.attribute, Const_pb.STRENGHT),
		PBHelper:getAttrById(userMercenary.baseAttr.attribute, Const_pb.AGILITY),
		PBHelper:getAttrById(userMercenary.baseAttr.attribute, Const_pb.INTELLECT),
		PBHelper:getAttrById(userMercenary.baseAttr.attribute, Const_pb.STAMINA)
	};
	local lb2Str = {};
	for name, index in pairs(attr2Index) do
		lb2Str[name .. "1"] = currentAttrs[index];
	end
	NodeHelper:setStringForLabel(container, lb2Str);
end

function MercenaryEnhancePageBase:refreshRightPanel(container, vals)
	local lb2Str = {}
	local colorMap = {}
	for name, index in pairs(attr2Index) do
		lb2Str[name .. "2"] = string.format("%d(%+d)", vals[index], vals[index] - currentAttrs[index]);
		if vals[index] - currentAttrs[index] > 0 then
		    colorMap[name .. "2"] = GameConfig.ColorMap.COLOR_GREEN
		elseif vals[index] - currentAttrs[index] < 0 then
		    colorMap[name .. "2"] = GameConfig.ColorMap.COLOR_RED
		else
		    colorMap[name .. "2"] = GameConfig.ColorMap.COLOR_WHITE
		end
		
	end
	NodeHelper:setStringForLabel(container, lb2Str)
	NodeHelper:setColorForLabel( container, colorMap )
end

function MercenaryEnhancePageBase:resetRightPanel(container)
	local lb2Str = {};
	for name, index in pairs(attr2Index) do
		lb2Str[name .. "2"] = "";
	end
	NodeHelper:setStringForLabel(container, lb2Str);
end

function MercenaryEnhancePageBase:refreshBasicPage(container)
	userMercenary = UserMercenaryManager:getUserMercenaryById(thisRoleId);
	--获取富文本中的职业、主属性等字段，将来方便颜色扩展
	local professionStr = FreeTypeConfig[11].content;
	if userMercenary.itemId == 7 then
		professionStr = FreeTypeConfig[11].content;
	elseif userMercenary.itemId == 8 then
		professionStr = FreeTypeConfig[12].content;
	elseif userMercenary.itemId == 9 then
		professionStr = FreeTypeConfig[13].content;		
	end
		
	local mainAttribute = FreeTypeConfig[17].content;
	if userMercenary.itemId == 7 then
		mainAttribute = FreeTypeConfig[17].content;
	elseif userMercenary.itemId == 8 then
		mainAttribute = FreeTypeConfig[18].content;
	elseif userMercenary.itemId == 9 then
		mainAttribute = FreeTypeConfig[19].content;		
	end
	
	local lb2Str = {
		mLv 					= "LV." .. userMercenary.level,
		mMercenaryLv 			= "LV." .. userMercenary.level,
		mMercenaryName 			= roleCfg[userMercenary.itemId]["name"],
		mOccupation				= professionStr,
		mMainAttribute			= mainAttribute
	};
	NodeHelper:setStringForLabel(container, lb2Str);
	
	local sprite2Img = {};
	sprite2Img["mPic"] 	= roleCfg[userMercenary.itemId]["icon"];	
	NodeHelper:setSpriteImage(container, sprite2Img);
	
	self:showCost(container);
	self:refreshLeftPanel(container);
	self:resetRightPanel(container);
end	

function MercenaryEnhancePageBase:tryTrain(trainType)
	local msg = RoleOpr_pb.HPRoleBaptize();
	msg.roleId = thisRoleId;
	msg.type = trainType;
	local pb_data = msg:SerializeToString();
	PacketManager:getInstance():sendPakcet(opcodes.ROLE_BAPTIZE_C, pb_data, #pb_data, true);		
	--common:sendPacket(opcodes.ROLE_BAPTIZE_C, msg);
end

function MercenaryEnhancePageBase:saveTrain()
	local msg = RoleOpr_pb.HPRoleAttrReplace()
	msg.roleId = thisRoleId
	local pb_data = msg:SerializeToString()
	PacketManager:getInstance():sendPakcet(opcodes.ROLE_REPLACE_C, pb_data, #pb_data, true)		
	--common:sendPacket(opcodes.ROLE_REPLACE_C, msg);	
end

----------------click event------------------------
function MercenaryEnhancePageBase:onClose(container)
	PageManager.popPage(thisPageName);
end

function MercenaryEnhancePageBase:onCancel(container)
	self:resetRightPanel(container);
	self:showTrainConfirm(false);
end

function MercenaryEnhancePageBase:onSave(container)
	self:saveTrain();
end	

function MercenaryEnhancePageBase:onCommonTrain(container)
	local coinCost = tonumber(GameConfig.Cost.RoleTrain.Common) * userMercenary.level;
	if UserInfo.isCoinEnough(coinCost) then
		self:tryTrain(TrainType.Coin);
	end
end

function MercenaryEnhancePageBase:onGoldTrainNormal(container)
    local vipLevelLimit = self:getVipLimitLevel(1)
	if UserInfo.playerInfo.vipLevel < vipLevelLimit then
		MessageBoxPage:Msg_Box_Lan(common:getLanguageString("@TranVipLimit", vipLevelLimit, common:getLanguageString("@Normal")))
		return
	end
	self:onGoldTrain(GameConfig.Cost.RoleTrain.GoldNormal, TrainType.GoldNormal);
end

function MercenaryEnhancePageBase:onGoldTrainMedium(container)
	local vipLevelLimit = self:getVipLimitLevel(2)
	if UserInfo.playerInfo.vipLevel < vipLevelLimit then
		MessageBoxPage:Msg_Box_Lan(common:getLanguageString("@TranVipLimit", vipLevelLimit, common:getLanguageString("@Pro")))
		return
	end
	self:onGoldTrain(GameConfig.Cost.RoleTrain.GoldMedium, TrainType.GoldMedium);
end

function MercenaryEnhancePageBase:onGoldTrainSenior(container)
	local vipLevelLimit = self:getVipLimitLevel(3)
	if UserInfo.playerInfo.vipLevel < vipLevelLimit then
		MessageBoxPage:Msg_Box_Lan(common:getLanguageString("@TranVipLimit", vipLevelLimit, common:getLanguageString("@Master")))
		return
	end
	self:onGoldTrain(GameConfig.Cost.RoleTrain.GoldSenior, TrainType.GoldSenior);
end

function MercenaryEnhancePageBase:onGoldTrain(gold, trainType)
	if UserInfo.isGoldEnough(gold) then
	    if trainType == TrainType.GoldSenior then
	        mIsAfterSenior = true
	    end
		self:tryTrain(trainType);
	end
end

--回包处理
function MercenaryEnhancePageBase:onReceivePacket(container)
	local opcode = container:getRecPacketOpcode()
	local msgBuff = container:getRecPacketBuffer()
	
	if opcode == opcodes.ROLE_BAPTIZE_S then
		local msg = RoleOpr_pb.HPRoleBaptizeRet();
		msg:ParseFromString(msgBuff);
		local vals = {};
		for _, val in ipairs(msg.values) do
			table.insert(vals, val);
		end
		self:refreshRightPanel(container, vals);
		if mIsAfterSenior then
		    self:saveTrain()
		    return
		end
		self:showTrainConfirm(true);
	elseif opcode == opcodes.ROLE_REPLACE_S then
		self:refreshLeftPanel(container);
		if not mIsAfterSenior then
		    self:resetRightPanel(container);
		end
		self:showTrainConfirm(false);
		mIsAfterSenior = false
	end
end

function MercenaryEnhancePageBase:getVipLimitLevel( typeId )
	for i = 0,#vipTab,1 do
		if vipTab[i].maxMercenaryTime == typeId then
			return i
		end
	end
end

function MercenaryEnhancePageBase:registerPacket(container)
	for key, opcode in pairs(opcodes) do
		if string.sub(key, -1) == "S" then
			container:registerPacket(opcode)
		end
	end
end

function MercenaryEnhancePageBase:removePacket(container)
	for key, opcode in pairs(opcodes) do
		if string.sub(key, -1) == "S" then
			container:removePacket(opcode)
		end
	end
end

-------------------------------------------------------------------------
local CommonPage = require("CommonPage");
local MercenaryEnhancePage = CommonPage.newSub(MercenaryEnhancePageBase, thisPageName, option);

function MercenaryEnhancePage_setRoleId(roleId)
	thisRoleId = roleId;
end