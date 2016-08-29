GameUtil = {};

------------local variable for system api--------------------------------------
local tostring = tostring;
local tonumber = tonumber;
local string = string;
local pairs = pairs;
--------------------------------------------------------------------------------
------------local variable for font api--------------------------------------
local defaultfontcolor = 0xffffffff
local isFontLoad = false
local NodeHelper = require("NodeHelper");
--------------------------------------------------------------------------------
RechargePlatformNames = {};

--增加字体
function GameUtil:createFont()
    if isFontLoad then return end
    local bdSize24 = 24;
    FontFactory:instance():create_font_forLua("HelveticaBD24",
        get_system_default_fontfile(),
        defaultfontcolor,
        bdSize24);
        
        
        
    isFontLoad = true
end


function GameUtil:genRechargePlatformNames()
	if RechargePlatformNames==nil or table.maxn(RechargePlatformNames)<=0 then
        local tabel = TableReaderManager:getInstance():getTableReader("PlatformName.txt")
        local count = tabel:getLineCount()-1;
        for i = 1,count do
            if RechargePlatformNames[index] == nil then
			local id = tonumber(tabel:getData(i,0))
                RechargePlatformNames[id] = tabel:getData(i,1)
            end        
        end
    end
end

function GameUtil:getRechargeList()
	local Recharge_pb = require("Recharge_pb");
	local msg = Recharge_pb.HPFetchShopList()

	if BlackBoard:getInstance().PLATFORM_TYPE_FOR_LUA == 1 then
		self:genRechargePlatformNames();
		local  pName = RechargePlatformNames[libPlatformManager:getPlatform():getPlatformId()]
		if pName == nil then
			msg.platform = "Android_Default"
		else
			msg.platform = pName --读取配置表
		end			
	elseif BlackBoard:getInstance().PLATFORM_TYPE_FOR_LUA == 2 then
		msg.platform = "ios_appstore"
	else
		msg.platform = ""
	end 
	CCLuaLog( "PlatformName2:" .. msg.platform )
	
	local HP_pb = require("HP_pb");
	common:sendPacket(HP_pb.FETCH_SHOP_LIST_C, msg);
end

function GameUtil:doRecharge(goodsId)
	local shopItem = g_RechargeItemList[goodsId];
	if shopItem == nil then return; end
	
	if shopItem.productType == 1  then
		local HP_pb = require("HP_pb");
		common:sendEmptyPacket(HP_pb.FETCH_SHOP_LIST_C, false);
	end
		
	local buyInfo = BUYINFO:new_local();
	    
	local productName = "";
		
	if shopItem.productType == 0 then
		productName = shopItem.productName;
	else
		productName = common:getLanguageString("@MonthCard");
	end
	    
	buyInfo.productCount 		= 1;
	buyInfo.productName 		= productName;
	buyInfo.productId 			= shopItem.productId;
	buyInfo.productPrice 		= shopItem.productPrice;
	buyInfo.productOrignalPrice = shopItem.gold;
	buyInfo.description 		= GamePrecedure:getInstance():getServerID();
	libPlatformManager:getPlatform():buyGoods(buyInfo);
end


function GameUtil:getTipStr(itemCfg)
	local resInfo = ResManagerForLua:getResInfoByTypeAndId(itemCfg.type, itemCfg.itemId, itemCfg.count or 1);
	local Const_pb = require("Const_pb");
	local strTb = {};
	local isEquip = false
	
	if resInfo.mainType == Const_pb.PLAYER_ATTR and resInfo.itemId == 2001 then
	    table.insert(strTb, common:fillHtmlStr('TipForGuild'));
	    return table.concat(strTb, '<br/>'),isEquip,strTb[1];
	end
	
	if resInfo.mainType == Const_pb.TOOL or resInfo.mainType == Const_pb.PLAYER_ATTR then
		table.insert(strTb, common:fillHtmlStr('TipName_' .. resInfo.quality, resInfo.name));
		--CCHTMLLabel:createWithString(common:fillHtmlStr('TipName_' .. resInfo.quality, resInfo.name), size, "Helvetica")
		table.insert(strTb, common:fillHtmlStr('TipCommon', resInfo.describe));
	elseif resInfo.mainType == Const_pb.EQUIP then
		isEquip = true
		local equipId = resInfo.itemId;
		local title = common:getLanguageString('@LevelName', EquipManager:getLevelById(equipId), resInfo.name);
		table.insert(strTb, common:fillHtmlStr('TipName_' .. resInfo.quality, title));
		table.insert(strTb, common:fillHtmlStr('TipCommon', EquipManager:getPartNameById(equipId)));
		local professionId = EquipManager:getProfessionById(equipId);
		if professionId and professionId > 0 then
			local professionName = common:getLanguageString("@ProfessionName_" .. professionId);
			table.insert(strTb, common:fillHtmlStr("TipCondition", professionName));
		end
		table.insert(strTb, common:fillHtmlStr('TipCommon', EquipManager:getInitAttr(equipId)));
		if itemCfg.buyTip then
			local attrNum = GameConfig.Quality2AttrNum[resInfo.quality or 1];
			if attrNum > 0 and itemCfg.hideBuyNum==nil then
				table.insert(strTb, common:fillHtmlStr('TipBuyEquip', attrNum));
			end	
			if itemCfg.starEquip then			
				table.insert(strTb, common:fillHtmlStr('TipStarEquip'));
			end
			table.insert(strTb, common:fillHtmlStr('TipSmeltEquip'));
		end
	else
		return nil;
	end
	
	return table.concat(strTb, '<br/>'),isEquip,strTb[1];
end

function GameUtil:showTip(relativeNode, itemCfg,hideCallBackExt)
	local tipStr,isEquip , nameLabel = self:getTipStr(itemCfg);
	if tipStr == nil or tipStr == '' or relativeNode==nil then return; end
	
	local layerTag = GameConfig.Tag.TipLayer;
	local winSize = CCDirector:sharedDirector():getWinSize();
	local mainFrame = tolua.cast(MainFrame:getInstance(), 'CCNode');
	local layer = mainFrame:getChildByTag(layerTag);
	
	if not layer then
		layer = CCLayer:create();
		layer:setTag(layerTag);
		mainFrame:addChild(layer);
		layer:setContentSize(winSize);
		layer:registerScriptTouchHandler(function(eventName,pTouch)
		    if eventName=="ended" then
                GameUtil:hideTip();
                if(hideCallBackExt ~= nil ) then 
                    hideCallBackExt()
                end
                return true;
            end
		end
		, false, 0, false);
		layer:setTouchEnabled(true);
	end
	layer:setVisible(true);

	local tipTag = layerTag + 1;
	layer:removeChildByTag(tipTag, true);
	
	local tipNode = ScriptContentBase:create('Tips.ccbi');
	tipNode:setAnchorPoint(ccp(0, 0));	
	
	local tipWidth = GameConfig.LineWidth.Tip;
	local label = NodeHelper:addHtmlLable(tipNode:getVarNode('mTipsText'), tipStr, GameConfig.Tag.HtmlLable, CCSizeMake(tipWidth, 50));
	local mScale9Sprite = tipNode:getVarScale9Sprite('mScale9Sprite');
	if mScale9Sprite then
		local labelSize = label:getContentSize();
		local margin = 5;
		mScale9Sprite:setContentSize(CCSizeMake(tipWidth, labelSize.height + margin * 2));
	end
	
	local posX, posY = relativeNode:getPosition();
	local size = relativeNode:getContentSize();
	local pos = relativeNode:convertToWorldSpace(ccp(posX + size.width, posY));
	if pos.x + tipWidth > winSize.width then
		pos = relativeNode:convertToWorldSpace(ccp(posX, posY));
		pos.x = pos.x - tipWidth;
	end
	local newPos = layer:convertToNodeSpace(pos);
	tipNode:setPosition(newPos);
	tipNode:setTag(tipTag);
	layer:addChild(tipNode);
	
	local _star = tipNode:getVarSprite("mStar")
	if isEquip then
		local _label = CCHTMLLabel:createWithString(nameLabel, tipNode:getVarNode('mTipsText'):getContentSize(), "Helvetica")
		_star:setAnchorPoint(ccp(0, 1))
		local stepLevel = EquipManager:getEquipStepById(itemCfg.itemId)	
		local posX = _label:getContentSize().width-30
		EquipManager:setStarPosition(_star, stepLevel == GameConfig.ShowStepStar, posX, posY)
	else
		_star:setVisible( false )
		
	end
	
	tipNode:release();
end

function GameUtil:hideTip()
	local layerTag = GameConfig.Tag.TipLayer;
	local mainFrame = tolua.cast(MainFrame:getInstance(), 'CCNode');
	local layer = mainFrame:getChildByTag(layerTag);
	if layer then
		layer:setVisible(false);
	end
end

function GameUtil:showLevelUpAni()
	local mainFrame = tolua.cast(MainFrame:getInstance(), 'CCNode');	
	if mainFrame == nil or (GamePrecedure:getInstance():isInLoadingScene()) then
		return
	end
	local levelupTag = GameConfig.Tag.TipLevelUp;
	local levelUpNode = mainFrame:getChildByTag(levelupTag);
	if not levelUpNode then
		levelUpNode = CCNode:create();
		levelUpNode:setTag(levelupTag);
		local winSize = CCDirector:sharedDirector():getWinSize();
		local posX = winSize.width / 2
		local posY = winSize.height /2
		levelUpNode:setPosition(posX,posY)
		mainFrame:addChild(levelUpNode);	
	end				
	levelUpNode:removeAllChildren()
	local levelUpCCB = ScriptContentBase:create('A_LevelUp.ccbi');	
	levelUpCCB:runAnimation("Default Timeline");	
	levelUpNode:addChild(levelUpCCB);
	levelUpCCB:release();
end


--清楚纹理内存相关	by zhenhui 2014/10/25
function GameUtil:purgeCachedData()
	CCSpriteFrameCache:sharedSpriteFrameCache():removeUnusedSpriteFramesPerFrame();
	CCTextureCache:sharedTextureCache():removeUnusedTexturesPerFrame();
end
