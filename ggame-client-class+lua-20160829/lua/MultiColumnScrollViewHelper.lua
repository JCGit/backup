----------------------------------------------------------------------------------
--[[
	FILE:			MultiColumnScrollViewHelper.lua
	ENCODING:		UTF-8, no-bomb
	DESCRIPTION:	
	AUTHOR:			zt
	CREATED:		2014-10-21
	多列scrollView接口，为实现scrollView的嵌套，暂时屏蔽了scrollView中_setChildMenu方法
--]]
----------------------------------------------------------------------------------

local MultiColumnScrollViewHelper = {}
local NodeHelper = require("NodeHelper");
--默认的竖向scrollView子节点
local ChildScrollViewContent = {
    ccbiFile = "BackpackContent.ccbi"
};

function ChildScrollViewContent.onFunction(eventName, container)
	if eventName == "luaRefreshItemView" then
		ChildScrollViewContent.onRefreshItemView(container);
	end
end


function ChildScrollViewContent.onRefreshItemView(container)
    if container.mScrollView == nil then
        NodeHelper:initScrollView(container, "mBackpackContent", 4);
        if container.ParentContainer~=nil then
            container.ParentContainer:autoAdjustResizeScrollview(container.mScrollView);
        end
        ChildScrollViewContent.clearAllItem(container)
        ChildScrollViewContent.buildAllItem(container)
    end
end

function ChildScrollViewContent.clearAllItem(container)
    NodeHelper:clearScrollView(container);
end

function ChildScrollViewContent.buildAllItem(container)
    local contentId = container:getItemDate().mID;
    if container.BuildData~=nil then
         local size = container.BuildData.size or 1
         local ccbiFile = container.BuildData.ccbiFile
         local funcCallback = container.BuildData.funcCallback
         NodeHelper:buildScrollView(container, size, ccbiFile,funcCallback);
         
         if container.BuildData.thisScrollViewOffset~=nil then
            local y = container.BuildData.thisScrollViewOffset.y
            local offsetY = container.mScrollView:minContainerOffset().y
            if y >= offsetY and y <=0 and offsetY<0 then
                NodeHelper:setScrollViewStartOffset(container,container.BuildData.thisScrollViewOffset)
            end
         end
    end
end
---------------------------------------------------------------
--buildTable[i]单项中至少要包含ccbiFile
--添加到buildTable的项目使用table.insert进行添加
--其他参数说明：buildTable.onTabChange滑动结束时候调用，参数说container和index:当前scrollView处于的项目
--[[
    调用示例
    local buildTable = {
        onTabChange = PackagePageBase.changeTab,
        totalSize = 2
    }
	local buildOne = {
        ccbiFile = ChildScrollViewContent.ccbiFile,
        size = 2,
        funcCallback = ChildScrollViewContent.onFunction
    }
    table.insert(buildTable,buildOne)
    NodeHelper:buildMultiColumnScrollView(container,buildTable,100) --100为项与项的间隔
--]]

function MultiColumnScrollViewHelper.buildScrollViewHorizontal(container,buildTable,interval)
    if buildTable==nil or container==nil or container.mScrollView==nil or (#buildTable)<1 then return end
    --初始化数据
    container.fOneItemWidth = container.mScrollView:getViewSize().width  --个体宽度
    interval = interval or 0 --间隔
    container.ScrollInterval = interval
    container.onScrollIndexChange = buildTable.onTabChange  --scroll改移动结束回调
    container.ScrollSize = buildTable.totalSize or 0 --结点数
    if container.ScrollSize == 0 then
        for i=1,#buildTable do
            local oneSize = buildTable[i].size or 1
            container.ScrollSize = container.ScrollSize + oneSize
        end
    end
    container.ChildContentBase = {}
    --结束初始化
    local iCount = 0
    local iMaxNode = container.m_pScrollViewFacade:getMaxDynamicControledItemViewsNum()
    local totalSize = container.ScrollSize
    local fOneItemWidth = container.fOneItemWidth
    for i=1,#buildTable do
        local ccbiFile
        local funcCallback
        local size
        local BuildData = {}
        if buildTable[i].type~=nil or buildTable[i].type == "default" then
            ccbiFile = ChildScrollViewContent.ccbiFile
            funcCallback = ChildScrollViewContent.onFunction
            size = buildTable[i].defaultSize or 1
            
            BuildData.ccbiFile = buildTable[i].ccbiFile
            BuildData.funcCallback = buildTable[i].funcCallback
            BuildData.size = buildTable[i].size or 1
            BuildData.thisScrollViewOffset = buildTable[i].thisScrollViewOffset
        else
            ccbiFile = buildTable[i].ccbiFile
            funcCallback = buildTable[i].funcCallback
            size = buildTable[i].size or 1
        end
        if ccbiFile==nil then return end

        for j=size,1,-1 do
            local pItemData = CCReViSvItemData:new_local()
            pItemData.mID = totalSize - iCount
            pItemData.m_iIdx = totalSize - iCount
            pItemData.m_ptPosition = ccp((fOneItemWidth+interval) * iCount,0)
            if iCount < iMaxNode then
                local pItem = ScriptContentBase:create(ccbiFile)
                CCLuaLog("**************************************************begin pItem:"..pItem:retainCount())
                pItem.id = iCount
                if funcCallback~=nil then
                    pItem:registerFunctionHandler(funcCallback)
                    CCLuaLog("**************************************************pItem:"..pItem:retainCount())
                end
                if buildTable[i].type~=nil and buildTable[i].type == "default" then
                    pItem.BuildData = BuildData
                    pItem.ParentContainer = container
                    table.insert(container.ChildContentBase,pItem)
                    CCLuaLog("**************************************************pItem:"..pItem:retainCount())
                end
                container.m_pScrollViewFacade:addItem(pItemData, pItem.__CCReViSvItemNodeFacade__)
                CCLuaLog("**************************************************end pItem:"..pItem:retainCount())
            else
                container.m_pScrollViewFacade:addItem(pItemData)
            end
            iCount = iCount + 1
        end
    end
    --设置大小
    local originHeight = container.mScrollView:getContentSize().height
    local size = CCSizeMake(fOneItemWidth* iCount + interval * (iCount-1), originHeight)
	container.mScrollView:setContentSize(size)
	container.mScrollView:setContentOffset(ccp(0, 0))
	container.m_pScrollViewFacade:setDynamicItemsStartPosition(iCount - 1);
	container.mScrollView:forceRecaculateChildren();
	ScriptMathToLua:setSwallowsTouches(container.mScrollView)
	
	container.maxScrollWidth = size.width or 0
end

function MultiColumnScrollViewHelper.setTouchLayer(container,onTouchBegin,onTouchMove,onTouchEnd,onTouchCancel,isScrollViewTouch)
    isScrollViewTouch = isScrollViewTouch or false
    local layer = container.mScrollView:getParent():getChildByTag(51001);
	if not layer then
		layer = CCLayer:create();
		layer:setTag(51001);
		layer:setAnchorPoint(container.mScrollView:getAnchorPoint())
		container.mScrollView:getParent():addChild(layer);
		layer:setContentSize(container.mScrollView:getViewSize());
		layer:setPosition(container.mScrollView:getPosition())
		layer:registerScriptTouchHandler(function(eventName,pTouch)
            if eventName == "began" then
                if onTouchBegin then
                    onTouchBegin(container,eventName,pTouch)
                else
                    MultiColumnScrollViewHelper.onTouchBegin(container,eventName,pTouch)
                end
            elseif eventName == "moved" then
                if onTouchMove then
                    onTouchMove(container,eventName,pTouch)
                else
                    MultiColumnScrollViewHelper.onTouchMove(container,eventName,pTouch)
                end
            elseif eventName == "ended" then
                if onTouchEnd then
                    onTouchEnd(container,eventName,pTouch)
                else
                    MultiColumnScrollViewHelper.onTouchEnd(container,eventName,pTouch)
                end
            elseif eventName == "cancelled" then
                if onTouchCancel then
                    onTouchCancel(container,eventName,pTouch)
                else
                    MultiColumnScrollViewHelper.onTouchCancel(container,eventName,pTouch)
                end
            end
        end
        ,false,0,false);
		layer:setTouchEnabled(false);
		layer:setTouchEnabled(true);
		container.mScrollView:setTouchEnabled(isScrollViewTouch)
		layer:setVisible(true);
	end
	--touch参数设置
	container.mCanHScroll = true;
	container.mCanVScroll = true;
	container.mCanTouch = false
	container.m_BegainX = 0
	container.m_BegainY = 0
	container.startX = 0
end

function MultiColumnScrollViewHelper.setMoveOnByOn(container,value)
    MultiColumnScrollViewHelper.setTouchLayer(container)
end

function MultiColumnScrollViewHelper.onTouchBegin(container,eventName,pTouch)
    local point = pTouch:getLocation();
    
    
    container.mCanTouch = false
    local layer = container.mScrollView:getParent():getChildByTag(51001);
    if layer~=nil then
        point = layer:getParent():convertToNodeSpace(point)
        local m_obPosition = ccp(layer:getPositionX(),layer:getPositionY())
        local m_obAnchorPoint = layer:getAnchorPoint()
        local m_obContentSize = layer:getContentSize()
        local rect = CCRectMake( m_obPosition.x - m_obContentSize.width * m_obAnchorPoint.x,
                      m_obPosition.y - m_obContentSize.height * m_obAnchorPoint.y,
                      m_obContentSize.width, m_obContentSize.height);
        if rect:containsPoint(point) then
            container.mCanTouch = true
        end
    end
    
    if container.mCanTouch==false then return end
    
    container.m_BegainX = point.x;
    container.m_BegainY = point.y;
    local mainOffset = container.mScrollView:getContentOffset()
    container.startX = mainOffset.x * (-1)
    container.mScrollView:getContainer():stopAllActions()
end

function MultiColumnScrollViewHelper.onTouchMove(container,eventName,pTouch)
    if container.mCanTouch==false then return end

    local point = pTouch:getLocation();
    
    local layer = container.mScrollView:getParent():getChildByTag(51001);
    if layer~=nil then
        point = layer:getParent():convertToNodeSpace(point)
    end
    
    local moveDisX =  point.x - container.m_BegainX
    local moveDisY =  point.y - container.m_BegainY
    container.m_BegainX = point.x;
    container.m_BegainY = point.y
    
    if container.mCanHScroll and container.mCanVScroll then
        if math.abs(moveDisX) <= math.abs(moveDisY) / 2 then
            container.mCanHScroll = false;
            container.mCanVScroll = true
        else    
            container.mCanHScroll = true;
            container.mCanVScroll = false
            if container.ChildContentBase~=nil then
                for i=1,#container.ChildContentBase do
                    container.ChildContentBase[i].mScrollView:setTouchEnabled(false)
                end
            end
        end
    end
    
    if container.mCanHScroll then
        local mainOffset = container.mScrollView:getContentOffset()
        if moveDisX>0 and mainOffset.x<0 then
            mainOffset.x = mainOffset.x + moveDisX;
        elseif  moveDisX<0 and mainOffset.x>(-(container.maxScrollWidth-container.fOneItemWidth)) then
            mainOffset.x = mainOffset.x + moveDisX;
        end
        container.mScrollView:setContentOffset(mainOffset)
    end
    --container.mCanHScroll = true;
end

function MultiColumnScrollViewHelper.onTouchEnd(container,eventName,pTouch)
    --横向判断
    if container.mCanTouch==false then return end    
        
    if container.mScrollView == nil then return end
    if container.mCanHScroll then
        local ceilWidth = container.fOneItemWidth or 0
        local interval = container.ScrollInterval or 0
        local size = container.ScrollSize or 0
        local startX = container.startX or 0
        local mainOffset = container.mScrollView:getContentOffset()
        mainOffset.x = mainOffset.x * (-1)
        local adjustX = (ceilWidth + interval)
        local index = math.floor(mainOffset.x / adjustX)
        local offset = (mainOffset.x - index*adjustX)
        --1为往左滑动，0为往右滑动
        local direction = (mainOffset.x - startX)>0 and 1 or 0 
        if direction==1 then
            if offset > ceilWidth/2 then
                index = index + 1
            end
        else
            if offset > (ceilWidth/2 + interval) then
                index = index + 1
            end
        end
        
        index = index<0 and 0 or index
        index = index>(size-1) and (size-1) or index
        mainOffset.x = index * adjustX *(-1)
        container.mScrollView:setContentOffsetInDuration(mainOffset,0.3)
        if container.onScrollIndexChange then
            container.onScrollIndexChange(container,index)
        end
        if container.ChildContentBase~=nil then
            for i=1,#container.ChildContentBase do
                container.ChildContentBase[i].mScrollView:setTouchEnabled(true)
            end
        end
    end
    container.mCanHScroll = true;
    container.mCanVScroll = true
end

function MultiColumnScrollViewHelper.onTouchCancel(container,eventName,pTouch)
    MultiColumnScrollViewHelper.onTouchEnd(container,eventName,pTouch)
end

function MultiColumnScrollViewHelper.clearMultiColumnScrollView(container,index)
    local thisScrollViewOffset
    if index==nil then
        index = -1
    end
    if container.ChildContentBase~=nil then
        for i=1,#container.ChildContentBase do
            if container.ChildContentBase[i]~=nil and container.ChildContentBase[i].mScrollView~=nil then
                if index == i then
                    thisScrollViewOffset = container.ChildContentBase[i].mScrollView:getContentOffset()
                end
                NodeHelper:deleteScrollView(container.ChildContentBase[i]);
            end
        end
    end
    return thisScrollViewOffset
end


return MultiColumnScrollViewHelper