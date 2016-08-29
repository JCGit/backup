local PageManager = {};
--------------------------------------------------------------
local confirmPageName = "DecisionPage";
--------------------------------------------------------------
local UserInfo = require("UserInfo");

PageManager.changePage = function(pageName, popAll)
	local currPage = MainFrame:getInstance():getCurShowPageName();
	if currPage == pageName then return; end
	
	local msg = MsgMainFrameChangePage:new()	
	registerScriptPage(pageName)
	msg.pageName = pageName
	msg.needPopAllPage = popAll == nil and true or popAll
	MessageManager:getInstance():sendMessageForScript(msg)
end

PageManager.pushPage = function(pageName, noCheck)
	local msg = MsgMainFramePushPage:new()
	if noCheck == nil or noCheck == false then
		registerScriptPage(pageName)
	end
	msg.pageName = pageName
	MessageManager:getInstance():sendMessageForScript(msg)
end

PageManager.showFightPage = function(pageName)
	MainFrame:getInstance():showFightPage();
end

PageManager.popPage = function(pageName)
	local msg = MsgMainFramePopPage:new()
	msg.pageName = pageName
	MessageManager:getInstance():sendMessageForScript(msg)
end

PageManager.popAllPage = function()
	MainFrame:getInstance():popAllPage()
end

PageManager.refreshPage = function(pageName,extraParam)
	local msg = MsgMainFrameRefreshPage:new()
	msg.pageName = pageName
	msg.extraParam = extraParam == nil and "" or extraParam
	MessageManager:getInstance():sendMessageForScript(msg)
end

PageManager.showCover = function(pageName)
	local msg = MsgMainFrameCoverShow:new()
	registerScriptPage(pageName)
	msg.pageName = pageName
	MessageManager:getInstance():sendMessageForScript(msg)
end

PageManager.hideCover = function(pageName)
	local msg = MsgMainFrameCoverHide:new()
	msg.pageName = pageName
	MessageManager:getInstance():sendMessageForScript(msg)
end

PageManager.removeAdventure = function(advenIndex)
	if advenIndex == nil then return end
	local msg = MsgAdventureRemoveItem:new()
	msg.index = tonumber(advenIndex)
	MessageManager:getInstance():sendMessageForScript(msg)
end	

PageManager.showNotice = function(title, message)
	RegisterLuaPage("NoticePage");
	NoticePage_setNotice(title, message);
	PageManager.pushPage("NoticePage");
end

--@param autoClose: has default value [true]
PageManager.showConfirm = function(title, message, callback, autoClose)
	RegisterLuaPage(confirmPageName);
	DecisionPage_setDecision(title, message , callback, autoClose);
	PageManager.pushPage(confirmPageName);
end

PageManager.confirmUseItem = function(title, message, itemId)
	RegisterLuaPage("ConfirmUseItemPage");
	local callback = function(isSure)
		if isSure then
			local ItemOprHelper = require("ItemOprHelper");
			ItemOprHelper:useItem(itemId);
		end
	end;
	ConfirmUseItemPage_setDecision(title, message , callback);
	PageManager.pushPage("ConfirmUseItemPage");
end

--show info of equip from user self
PageManager.showEquipInfo = function(equipId, roleId)
	RegisterLuaPage("EquipInfoPage");
	EquipInfoPage_setEquipId(equipId, roleId);
	PageManager.pushPage("EquipInfoPage");
end

PageManager.viewPlayerInfo = function(playerId, flagShowButton)
	ViewPlayerInfoPage_setPlayerId(playerId, flagShowButton);
	ViewPlayerInfo:getInfo(playerId);
end

--show info of equip from other user
PageManager.viewEquipInfo = function(equipId)
	RegisterLuaPage("EquipInfoPage");
	EquipInfoPage_viewEquipId(equipId);
	PageManager.pushPage("EquipInfoPage");
end

PageManager.showItemInfo = function(itemId)
	RegisterLuaPage("ItemInfoPage");
	ItemInfoPage_setItemId(itemId);
	PageManager.pushPage("ItemInfoPage");
end

PageManager.showResInfo = function(itemType, itemId, itemCount)
	local mainType = ResManagerForLua:getResMainType(itemType);
	local Const_pb = require("Const_pb");
	local ItemManager = require("ItemManager");
	if mainType == Const_pb.TOOL and ItemManager:getContainCfg(itemId) then
		PageManager.showGiftPackage(itemId);
		return;
	end
	RegisterLuaPage("ResInfoPage");
	ResInfoPage_setItemInfo(itemType, itemId, itemCount);
	PageManager.pushPage("ResInfoPage");
end

PageManager.showGiftPackage = function(itemId, callback)
	local ItemManager = require("ItemManager");
	local cfg = ItemManager:getContainCfg(itemId);
	if cfg ~= nil and #cfg > 0 then
		RegisterLuaPage("ResListPage");
		ResListPage_setList(cfg, callback);
		PageManager.pushPage("ResListPage");
	end
end

PageManager.showHelp = function( key ,title ,flag)
	if flag then
		RegisterLuaPage("HelpPageSpecial")
	    Help_SetHelpConfigSpecial(key , title )
		PageManager.pushPage("HelpPageSpecial")
	else
		RegisterLuaPage("HelpPage")
	    Help_SetHelpConfig(key , title )
		PageManager.pushPage("HelpPage")
	end
	
end

--活动跳转
PageManager.showActivity = function(id)
	ActivityPage_goActivity(id);
end

--提示缺少金币
PageManager.notifyLackCoin = function()
	local title = common:getLanguageString("@HintTitle");
	local msg = common:getLanguageString("@LackCoin");
	PageManager.showConfirm(title, msg, function(isSure)
		if isSure then
			MarketPage_showBuyCoin();
			PageManager.popPage(confirmPageName);
		end
	end, false);
end

--提示缺少钻石
PageManager.notifyLackGold = function()
	local title = common:getLanguageString("@HintTitle");
	local msg = common:getLanguageString("@LackGold");
	PageManager.showConfirm(title, msg, function(isSure)
		if isSure then
			RegisterLuaPage("RechargePage");
			RechargePage_showPage();
			PageManager.popPage(confirmPageName);
		end
	end, false);
end
-------------------------------------------------------------
local noticeCache = {
	Package 	= false,
	Equipment 	= false,
	Mercenary 	= false,
	Skill 		= false
};
local key2Node = {
	Package 	= "mBackpackPagePoint",
	Equipment 	= "mEquipmentPagePoint",
	Mercenary 	= "mMercenaryPagePoint",
	Skill 		= "mSkillPagePoint"
};

PageManager.showRedNotice = function(key, visible)
	local nodeName = key2Node[key];
	if nodeName then
		MainFrame:getInstance():setChildVisible(nodeName, visible);
	end
	if key == "Equipment" then
		PageManager.refreshPage("EquipmentPage");
	elseif key == "Mercenary" then
		PageManager.refreshPage("MercenaryPage");
	elseif key == "Skill" then
		if UserInfo.skillUnlock.hasNew then
			local key = string.format("Skill_%d_%d_%d", UserInfo.serverId, UserInfo.playerInfo.playerId, UserInfo.skillUnlock.level);
			CCUserDefault:sharedUserDefault():setBoolForKey(key, true);
			CCUserDefault:sharedUserDefault():flush();
			UserInfo.skillUnlock.hasNew = false;
		end
	end
	noticeCache[key] = visible;
end

PageManager.refreshRedNotice = function()
	for key, nodeName in pairs(key2Node) do
		MainFrame:getInstance():setChildVisible(nodeName, noticeCache[key]);
	end
end

PageManager.showCoinNotEnough = function(title, message)
	RegisterLuaPage("ConfirmUseItemPage");
	local callback = function(isSure)
		if isSure then
			local ItemOprHelper = require("ItemOprHelper");
			ItemOprHelper:useItem(itemId);
		end
	end;
	ConfirmUseItemPage_setDecision(title, message , callback);
	PageManager.pushPage("ConfirmUseItemPage");
end

--------------------------------------------------------------

return PageManager;