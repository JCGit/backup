local NodeHelper = {}
------------local variable for system api--------------------------------------
local tostring = tostring;
local tonumber = tonumber;
local string = string;
local pairs = pairs;
local titleTag = 1000;
--------------------------------------------------------------------------------
local GameConfig = require("GameConfig");
local ViewHolder = {}

function NodeHelper:setStringForLabel(container, strMap)
	for name, str in pairs(strMap) do
		local node = container:getVarLabelBMFont(name)
		if node then
			node:setString(tostring(str))
		else
			local nodeTTF=container:getVarLabelTTF(name)
			if nodeTTF then
				nodeTTF:setString(tostring(str))
			else
				CCLuaLog("NodeHelper:setStringForLabel====>" .. name)		
			end
			
		end
	end
end

function NodeHelper:setStringForTTFLabel(container, strMap)
	for name, str in pairs(strMap) do
		local node = container:getVarLabelTTF(name)
		if node then
			node:setString(tostring(str))
		else
			CCLuaLog("NodeHelper:setStringForTTFLabel====>" .. name)
		end
	end
end

function NodeHelper:setColorForLabel( container, colorMap )
	for name, colorName in pairs(colorMap) do		
		local node = container:getVarLabelBMFont(name)
		if node then
			local color3B = self:_getColorFromSetting(colorName);
			node:setColor(color3B);
		else
			local nodeTTF=container:getVarLabelTTF(name)
			if nodeTTF then
				local color3B = self:_getColorFromSetting(colorName);
				nodeTTF:setColor(color3B);
			else
				CCLuaLog("NodeHelper:setStringForLabel====>" .. name)		
			end
			
		end
	end
end

function NodeHelper:setColorForLayerColor( container, colorMap )
	--[[
	for name, colorName in pairs(colorMap) do		
		local node = container:getVarLayerColor(name)
		if node then
			local color3B = self:_getColorFromSetting(colorName);
			node:setColor(color3B);
		else
			CCLuaLog("NodeHelper:setStringForLayerColor====>" .. name)		
		end
	end
	--]]
end


function NodeHelper:_getColorFromSetting( colorName )
	local color3B = StringConverter:parseColor3B(colorName)
	return color3B;
end

function NodeHelper:setQualityColor(node, quality)
	if node == nil then
		CCLuaLog("Error in NodeHelper:setQualityColor==> node is nil")
		return
	end

	quality=NodeHelper:getQuality(quality)

	local color = self:getSettingVar("FrameColor_Quality" .. quality)
	local color3B = StringConverter:parseColor3B(color)
	node:setColor(color3B)
end

function NodeHelper:setMenuItemQuality( container, itemName, quality )
	self:setQualityFrames(container, {[itemName] = quality});
end

--this function is not used
function NodeHelper:setFrameQuality(node, quality)
	if node == nil then
		CCLuaLog("Error in NodeHelper:setFrameQuality==> node is nil")
		return
	end

	quality=NodeHelper:getQuality(quality)

	node:setNormalImage(getFrameNormalSpirte(quality))
	node:setSelectedImage(getFrameSelectedSpirte(quality))
end

function NodeHelper:setQualityBMFontLabels(container, qualityMap)
	for frameName, quality in pairs(qualityMap) do
		local node = container:getVarLabelBMFont(frameName);
		if node == nil then
			CCLuaLog("Error in NodeHelper:setLabelsQuality==> node is nil");
		else
			local colorName = GameConfig.QualityColor[tonumber(quality)];
			local color3B = self:_getColorFromSetting(colorName);
			node:setColor(color3B);
		end
	end
end

function NodeHelper:getImageByQuality(quality)
	local normalImage = GameConfig.QualityImage[tonumber(quality)];
	if normalImage == nil then
		normalImage = GameConfig.QualityImage[1];
	end
	return normalImage;
end

function NodeHelper:setQualityFrames(container, qualityMap)
	for frameName, quality in pairs(qualityMap) do
		local node = container:getVarMenuItemImage(frameName);
		if node == nil then
			CCLuaLog("Error in NodeHelper:setFrameQuality==> node is nil");
		else
			local normalImage = self:getImageByQuality(quality);
			node:setNormalImage(CCSprite:create(normalImage));
		end
	end
end

function NodeHelper:setNormalImages(container, imgMap)
	for itemName, image in pairs(imgMap) do
		local menuItem = container:getVarMenuItemImage(itemName);
		if menuItem == nil then
			CCLuaLog("Error in NodeHelper:setFrameQuality==> menuItem is nil");
		else
			menuItem:setNormalImage(CCSprite:create(image));
		end
	end
end

function NodeHelper:setMenuItemSelected( container, selectedMap )
	for menuItemName, selected in pairs(selectedMap) do
		local item = container:getVarMenuItemImage(menuItemName);

		if item == nil then
			CCLuaLog("Error in NodeHelper:setMenuItemSelected==> node is nil");
		else
			if selected then
				item:selected();
			else
				item:unselected();
			end
		end
	end
end

function NodeHelper:getQuality(quality)
	if quality>QualityInfo.MaxQuality or quality<QualityInfo.MinQuality then
		quality=QualityInfo.NoQuality
	end
	return quality
end

function NodeHelper:setScaleByResInfoType(node, itemType, NodeHelperScale)
	if node == nil then
		CCLuaLog("node is Null for set scale")
		return
	end

	itemType = tonumber(itemType or 0)
	local resType = ResManager:getInstance():getResMainType(itemType)
	local scale = NodeHelperScale or 0.4

	if resType == DISCIPLE_TYPE or resType == DISCIPLE_BOOK then
		scale = scale * 3.0
	end
	node:setScale(scale);
end

function NodeHelper:setMenuEnabled(menuItem, isEnabled)
	if menuItem then
		menuItem:setEnabled(isEnabled);
	end
end

function NodeHelper:setMenuItemEnabled( container, menuItemName, isEnabled )
	local item = container:getVarMenuItemImage(menuItemName);

	if item ~= nil then
		item:setEnabled(isEnabled);
	end
end

function NodeHelper:setNodeVisible(node, isVisible)
	if node then
		node:setVisible(isVisible)
	end
end

function NodeHelper:setNodesVisible(container, visibleMap)
	for name, visible in pairs(visibleMap) do
		self:setNodeVisible(container:getVarNode(name), visible);
	end
end

function NodeHelper:setSpriteImage(container, imgMap, scaleMap)
	local scaleMap = scaleMap or {};
	for spriteName, image in pairs(imgMap) do
		local sprite = container:getVarSprite(spriteName);
		if sprite then
			sprite:setTexture(tostring(image));
			if scaleMap[spriteName] then
				sprite:setScale(scaleMap[spriteName]);
			end
		else
			CCLuaLog("Error:::NodeHelper:setSpriteImage====>" .. spriteName);
		end
	end
end

function NodeHelper:createTouchLayerByScrollView(container,onTouchBegin,onTouchMove,onTouchEnd,onTouchCancel,isScrollViewTouch)
    isScrollViewTouch = isScrollViewTouch or false
    local layer = container.mScrollView:getParent():getChildByTag(51001);
	if not layer then
		layer = CCLayer:create();
		layer:setTag(51001);
		container.mScrollView:getParent():addChild(layer);
		layer:setContentSize(container.mScrollView:getViewSize());
		layer:setPosition(container.mScrollView:getPosition())
		layer:registerScriptTouchHandler(function(eventName,pTouch)
            if eventName == "began" then
                if onTouchBegin then
                    onTouchBegin(container,eventName,pTouch)
                end
            elseif eventName == "moved" then
                if onTouchMove then
                    onTouchMove(container,eventName,pTouch)
                end
            elseif eventName == "ended" then
                if onTouchEnd then
                    onTouchEnd(container,eventName,pTouch)
                end
            elseif eventName == "cancelled" then
                if onTouchCancel then
                    onTouchCancel(container,eventName,pTouch)
                end
            end
        end
        ,false,0,false);
		layer:setTouchEnabled(true);
		container.mScrollView:setTouchEnabled(isScrollViewTouch)
		layer:setVisible(true);
	end
end



function NodeHelper:initScrollView(container, svName, size)
	local size = size or 3;
	container.mScrollView = container:getVarScrollView(svName);	
	if container.mScrollView == nil then return end
	container.mScrollViewRootNode = container.mScrollView:getContainer();
	container.m_pScrollViewFacade = CCReViScrollViewFacade:new_local(container.mScrollView);
	container.m_pScrollViewFacade:init(size, 3);
end

function NodeHelper:setScrollViewStartOffset(container,offset)
    if container==nil or container.mScrollView==nil then return end
	if container.mScrollViewRootNode==nil then return end
	container.mScrollViewRootNode:setPosition(offset)
	local children = container.mScrollViewRootNode:getChildren()
	if children~=nil then
        if children:count()<1 then return end
        local child = children:objectAtIndex(0)
        local childSize = child:getContentSize().height * child:getScaleY()
        if offset.y < 0 then
            offset.y = math.abs(offset.y)
        end
        local index = math.ceil(offset.y / childSize)
        index = index < 1 and 1 or index
        container.m_pScrollViewFacade:setDynamicItemsStartPosition(index - 1);
    end
end

--支持多ccbi加载的横向scrollview并支持一项一项移动
--[[
    调用示例
    local buildTable = {}
	local buildOne = {
        ccbiFile = PackageChildPage.ccbiFile,
        size = 2,
        funcCallback = PackageChildPage.onFunction
    }
    table.insert(buildTable,buildOne)
    NodeHelper:buildMultiColumnScrollView(container,buildTable,100) --100为项与项的间隔
--]]

function NodeHelper:buildScrollViewHorizontal(container, size, ccbiFile, funcCallback,interval)
	if size == 0 or ccbiFile == nil or ccbiFile == '' or funcCallback == nil then return end
	local iMaxNode = container.m_pScrollViewFacade:getMaxDynamicControledItemViewsNum()
	local iCount = 0
	local fOneItemHeight = 0
	local fOneItemWidth = 0
	interval = interval or 100

	for i=size, 1, -1 do
		local pItemData = CCReViSvItemData:new_local()
		pItemData.mID = i
		pItemData.m_iIdx = i
		pItemData.m_ptPosition = ccp((fOneItemWidth+interval) * iCount,0)

		if iCount < iMaxNode then
			local pItem = ScriptContentBase:create(ccbiFile)
			pItem.id = iCount
			pItem:registerFunctionHandler(funcCallback)
			if fOneItemHeight < pItem:getContentSize().height then
				fOneItemHeight = pItem:getContentSize().height
			end

			if fOneItemWidth < pItem:getContentSize().width then
				fOneItemWidth = pItem:getContentSize().width
			end
			container.m_pScrollViewFacade:addItem(pItemData, pItem.__CCReViSvItemNodeFacade__)
		else
			container.m_pScrollViewFacade:addItem(pItemData)
		end
		iCount = iCount + 1
	end

	local size = CCSizeMake(fOneItemWidth* iCount + interval * (iCount-1), fOneItemHeight)
	container.mScrollView:setContentSize(size)
	container.mScrollView:setContentOffset(ccp(0, 0))
	container.m_pScrollViewFacade:setDynamicItemsStartPosition(iCount - 1);
	container.mScrollView:forceRecaculateChildren();
	ScriptMathToLua:setSwallowsTouches(container.mScrollView)
end

function NodeHelper:buildScrollViewHorizontalOther(container, size, ccbiFile, funcCallback,interval)
	if size == 0 or ccbiFile == nil or ccbiFile == '' or funcCallback == nil then return end
	local iMaxNode = container.m_pScrollViewFacadeR:getMaxDynamicControledItemViewsNum()
	local iCount = 0
	local fOneItemHeight = 0
	local fOneItemWidth = 0
	interval = interval or 100

	for i=size, 1, -1 do
		local pItemData = CCReViSvItemData:new_local()
		pItemData.mID = i
		pItemData.m_iIdx = i
		pItemData.m_ptPosition = ccp((fOneItemWidth+interval) * iCount,0)

		if iCount < iMaxNode then
			local pItem = ScriptContentBase:create(ccbiFile)
			pItem.id = iCount
			pItem:registerFunctionHandler(funcCallback)
			if fOneItemHeight < pItem:getContentSize().height then
				fOneItemHeight = pItem:getContentSize().height
			end

			if fOneItemWidth < pItem:getContentSize().width then
				fOneItemWidth = pItem:getContentSize().width
			end
			container.m_pScrollViewFacadeR:addItem(pItemData, pItem.__CCReViSvItemNodeFacade__)
		else
			container.m_pScrollViewFacadeR:addItem(pItemData)
		end
		iCount = iCount + 1
	end

	local size = CCSizeMake(fOneItemWidth* iCount + interval * (iCount-1), fOneItemHeight)
	container.mScrollViewR:setContentSize(size)
	container.mScrollViewR:setContentOffset(ccp(-fOneItemWidth-interval, 0))
	container.m_pScrollViewFacadeR:setDynamicItemsStartPosition(iCount - 1);
	container.mScrollViewR:forceRecaculateChildren();
	ScriptMathToLua:setSwallowsTouches(container.mScrollViewR)
end

function NodeHelper:buildScrollView(container, size, ccbiFile, funcCallback)
	if size == 0 or ccbiFile == nil or ccbiFile == '' or funcCallback == nil then return end
	local iMaxNode = container.m_pScrollViewFacade:getMaxDynamicControledItemViewsNum()
	local iCount = 0
	local fOneItemHeight = 0
	local fOneItemWidth = 0

	for i=size, 1, -1 do
		local pItemData = CCReViSvItemData:new_local()
		pItemData.mID = i
		pItemData.m_iIdx = i
		pItemData.m_ptPosition = ccp(0, fOneItemHeight * iCount)

		if iCount < iMaxNode then
			local pItem = ScriptContentBase:create(ccbiFile)
			pItem.id = iCount
			pItem:registerFunctionHandler(funcCallback)
			if fOneItemHeight < pItem:getContentSize().height then
				fOneItemHeight = pItem:getContentSize().height
			end

			if fOneItemWidth < pItem:getContentSize().width then
				fOneItemWidth = pItem:getContentSize().width
			end
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
	ScriptMathToLua:setSwallowsTouches(container.mScrollView)  
end

function NodeHelper:buildScrollViewWithCache(container , size , ccbiFile , funcCallback ,funcCallbackExtra )
	if size == 0 or ccbiFile == nil or ccbiFile == '' or funcCallback == nil then return end
	local iMaxNode = container.m_pScrollViewFacade:getMaxDynamicControledItemViewsNum()
	local iCount = 0
	local fOneItemHeight = 0
	local fOneItemWidth = 0

	for i=1, size, 1 do
		local pItemData = CCReViSvItemData:new_local()
		pItemData.mID = i
		pItemData.m_iIdx = i
		pItemData.m_ptPosition = ccp(0, fOneItemHeight * iCount)
		
		if iCount < iMaxNode then
			local pItem 
			if ViewHolder[iCount + 1] ~= nil then
				pItem = ViewHolder[iCount + 1]
			else
				pItem = ScriptContentBase:create(ccbiFile)
				pItem.id = iCount
				pItem:registerFunctionHandler(funcCallback)
				if fOneItemHeight < pItem:getContentSize().height then
					fOneItemHeight = pItem:getContentSize().height
				end

				if fOneItemWidth < pItem:getContentSize().width then
					fOneItemWidth = pItem:getContentSize().width
				end
				container.m_pScrollViewFacade:addItem(pItemData, pItem.__CCReViSvItemNodeFacade__)
				iCount = iCount + 1
			end
		else
			iCount = iCount + 1
			container.m_pScrollViewFacade:addItem(pItemData)
		end
		
	end
		
	if iCount <= 0 then
		return
	end
	
	local size = CCSizeMake(fOneItemWidth, fOneItemHeight * iCount)
	container.mScrollView:setContentSize(size)
	container.mScrollView:setContentOffset(ccp(0, container.mScrollView:getViewSize().height - container.mScrollView:getContentSize().height * container.mScrollView:getScaleY()))
	container.m_pScrollViewFacade:setDynamicItemsStartPosition(iCount - 1);
	container.mScrollView:forceRecaculateChildren();
	ScriptMathToLua:setSwallowsTouches(container.mScrollView)
end

function NodeHelper:buildScrollViewR(container, size, ccbiFile, funcCallback)
	if size == 0 or ccbiFile == nil or ccbiFile == '' or funcCallback == nil then return end
	local iMaxNode = container.m_pScrollViewFacade:getMaxDynamicControledItemViewsNum()
	local iCount = 0
	local fOneItemHeight = 0
	local fOneItemWidth = 0

	for i=1, size, 1 do
		local pItemData = CCReViSvItemData:new_local()
		pItemData.mID = i
		pItemData.m_iIdx = i
		pItemData.m_ptPosition = ccp(0, fOneItemHeight * iCount)

		if iCount < iMaxNode then
			local pItem = ScriptContentBase:create(ccbiFile)
			pItem.id = iCount
			pItem:registerFunctionHandler(funcCallback)
			if fOneItemHeight < pItem:getContentSize().height then
				fOneItemHeight = pItem:getContentSize().height
			end

			if fOneItemWidth < pItem:getContentSize().width then
				fOneItemWidth = pItem:getContentSize().width
			end
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
	ScriptMathToLua:setSwallowsTouches(container.mScrollView)
end

function NodeHelper:clearScrollView(container)
	if container.m_pScrollViewFacade then
		container.m_pScrollViewFacade:clearAllItems();
	end
	if container.mScrollViewRootNode then
		container.mScrollViewRootNode:removeAllChildren();
	end
end

function NodeHelper:deleteScrollView(container)
	ViewHolder = {}
	self:clearScrollView(container);
	if container.m_pScrollViewFacade then
		container.m_pScrollViewFacade:delete();
		container.m_pScrollViewFacade = nil;
	end
	container.mScrollViewRootNode = nil;
	container.mScrollView = nil;
end
--------------------------------------------------------------------------------
------scrollview without Facade----
function NodeHelper:initRawScrollView(container, svName)
	container.mScrollView = container:getVarScrollView(svName);
	--container:autoAdjustResizeScrollview(container.mScrollView);
	container.mScrollViewRootNode = container.mScrollView:getContainer();
end

function NodeHelper:buildRawScrollView(container, size, ccbiFile, funcCallback)
	if size == 0 or ccbiFile == nil or ccbiFile == '' or funcCallback == nil then return end

	local width = container.mScrollView:getContentSize().width;
	local height = 0;
	
	for i = size, 1, -1 do
		--local pItem = ScriptContentBase:create(ccbiFile, i);
		local pItem = CCBManager:getInstance():createAndLoad2(ccbiFile);
		pItem:setTag(i);
		pItem:registerFunctionHandler(funcCallback);
		pItem.__CCReViSvItemNodeFacade__:initItemView();
		container.mScrollView:addChild(pItem);
		--pItem:release();
		pItem:setAnchorPoint(ccp(0, 0));
		pItem:setPosition(ccp(0, height));
		height = height + pItem:getContentSize().height;
	end

	local size = CCSizeMake(width, height);
	container.mScrollView:setContentSize(size);
	container.mScrollView:setContentOffset(ccp(0, container.mScrollView:getViewSize().height - height * container.mScrollView:getScaleY()))
	container.mScrollView:forceRecaculateChildren();
end

function NodeHelper:setCCHTMLLabel( node, _CCSize,_ccp , str )
	
	local label = CCHTMLLabel:createWithString(str,_CCSize , "Helvetica")
	
	label:setPosition(_ccp)
	
	if node ~= nil then
		node:getParent():addChild(label)
	end
end	

function NodeHelper:setCCHTMLLabelAutoFixPosition( node, _CCSize , str )
	
	local label = CCHTMLLabel:createWithString(str,_CCSize , "Helvetica")
	
	
	
	if node ~= nil then
		local _ccp = ccp(node:getPositionX(),node:getPositionY())
		label:setAnchorPoint(node:getAnchorPoint())
		label:setPosition(_ccp)
		node:getParent():removeChildByTag(titleTag,true)
		node:getParent():addChild(label,1,titleTag)
	end
end	

function NodeHelper:setCCHTMLLabelDefaultPos( node, _CCSize , str )
	
	local label = CCHTMLLabel:createWithString(str,_CCSize , "Helvetica")
	
	local posX = node:getPositionX() 
	local posY = node:getPositionY()
	
	
	
	label:setPosition(ccp(posX,posY))
	
	if node ~= nil then
		node:getParent():removeChildByTag(titleTag,true)
		node:getParent():addChild(label,1,titleTag)
	end		
	
	return label
end

function NodeHelper:addHtmlLable(node, str, tag, size, _parent)
	local size = size or node:getContentSize();
	local posX, posY = node:getPosition();
	local anchor = node:getAnchorPoint();
	
	local label = CCHTMLLabel:createWithString(str, size, "Helvetica");
	label:setPosition(ccp(posX, posY));
	label:setAnchorPoint(anchor);
	label:setScale(node:getScale());
	
	if node ~= nil then
		local _parent = _parent or node:getParent();
		if _parent then
			if tag then
				_parent:removeChildByTag(tag, true);
			end
			_parent:addChild(label);
			if tag then
				label:setTag(tag);
			end
		end
	end		
	return label;
end

function NodeHelper:setColor3BForLabel( container, colorMap )
	for name, color3B in pairs(colorMap) do		
		local node = container:getVarLabelBMFont(name)
		if node then
			node:setColor(color3B);
		else
			local nodeTTF=container:getVarLabelTTF(name)
			if nodeTTF then
				nodeTTF:setColor(color3B);
			else
				CCLuaLog("NodeHelper:setStringForLabel====>" .. name)		
			end
		end
	end
end

function NodeHelper:fillRewardItemWithParams(container, rewardCfg, maxSize,params)
    local maxSize = maxSize or 4;

    local nodesVisible = {};
	local lb2Str = {};
	local sprite2Img = {};
	local menu2Quality = {};
	local btnSprite = {};
	
	local mainNode = params.mainNode or "mRewardNode"
	local countNode = params.countNode or "mNum"
    local nameNode = params.nameNode or "mName"
    local frameNode = params.frameNode or "mFrame"
    local picNode   = params.picNode  or "mPic"
    local startIndex = params.startIndex or 1
	
	for i = startIndex, maxSize+startIndex-1 do
		local cfg = rewardCfg[i - startIndex + 1];
		nodesVisible[mainNode .. i] = cfg ~= nil;
		if cfg ~= nil then
			local resInfo = ResManagerForLua:getResInfoByTypeAndId(cfg.type, cfg.itemId, cfg.count);
			if resInfo ~= nil then
				sprite2Img[picNode .. i] 		= resInfo.icon;
				lb2Str[countNode .. i]				= "x" .. cfg.count;
				lb2Str[nameNode .. i]			= resInfo.name;
				menu2Quality[frameNode .. i]		= resInfo.quality;
			else
				CCLuaLog("Error::***reward item not found!!");
			end
		end
	end
	self:setNodesVisible(container, nodesVisible);
	self:setStringForLabel(container, lb2Str);
	self:setSpriteImage(container, sprite2Img);
	self:setQualityFrames(container, menu2Quality);
end


function NodeHelper:fillRewardItem(container, rewardCfg, maxSize)
	local maxSize = maxSize or 4;
	
	local nodesVisible = {};
	local lb2Str = {};
	local sprite2Img = {};
	local menu2Quality = {};
	
	for i = 1, maxSize do
		local cfg = rewardCfg[i];
		nodesVisible["mRewardNode" .. i] = cfg ~= nil;
		
		if cfg ~= nil then
			local resInfo = ResManagerForLua:getResInfoByTypeAndId(cfg.type, cfg.itemId, cfg.count);
			if resInfo ~= nil then
				sprite2Img["mPic" .. i] 		= resInfo.icon;
				lb2Str["mNum" .. i]				= "x" .. cfg.count;
				lb2Str["mName" .. i]			= resInfo.name;
				menu2Quality["mFrame" .. i]		= resInfo.quality;
			else
				CCLuaLog("Error::***reward item not found!!");
			end
		end
	end
	
	self:setNodesVisible(container, nodesVisible);
	self:setStringForLabel(container, lb2Str);
	self:setSpriteImage(container, sprite2Img);
	self:setQualityFrames(container, menu2Quality);
end

function NodeHelper:addEquipAni(container, nodeName, aniVisible, userEquipId, userEquip)
	local aniNode = container:getVarNode(nodeName);
	if aniNode then
		aniNode:removeAllChildren();
		if aniVisible then
			local ccbiFile = UserEquipManager:getGodlyAni(userEquipId, userEquip);
			local ani = CCBManager:getInstance():createAndLoad2(ccbiFile);
			ani:unregisterFunctionHandler();
			aniNode:addChild(ani);
		end
		aniNode:setVisible(aniVisible);
	end
end
-----------------------------------------------------------------
return NodeHelper;
