GoodsViewPage={}
GoodsViewPage.mTitle="@PackPreviewTitleView"
GoodsViewPage.mMsgContent="@PackPreviewMsgView"
GoodsViewPage.mViewGoodsListInfo={}
local GoodListContent = {}
function GoodListContent.onFunction(eventName,container)
    if eventName == "luaRefreshItemView" then
        GoodListContent.onRefreshItemView(container)
	elseif eventName == "onFrameButton"  then 		
		GoodListContent.onFrameButton(container)
    end
end

function GoodListContent.onRefreshItemView(container)
	
	
	
    local itemData = GoodsViewPage.mViewGoodsListInfo[container:getItemDate().mID]	
    container:setDynamicIconTex("mFrameBack","mPropPic",itemData.icon)
    container:setFrameColorByQuality("mFrameBack",itemData.quality)
	container:setLabelColorByQuality("mPropName",itemData.quality)
    container:getVarLabelBMFont("mNumber"):setString(tostring(itemData.count))
    container:getVarLabelBMFont("mPropName"):setString(tostring(itemData.name))
	container:getVarLabelBMFont("mPropTag"):setString(tostring(itemData.typeName))
	container:getVarLabelBMFont("mPropExplain"):setString(tostring(GameMaths:stringAutoReturnForLua(itemData.describe,19,0)))
end

function GoodListContent.onFrameButton(container)
	local itemData = GoodsViewPage.mViewGoodsListInfo[container:getItemDate().mID]

	PopupPageManager:showResInfo(itemData.type,itemData.id)			
end

local GoodsShowListPage = {}

function luaCreat_GoodsShowListPage(container)
    CCLuaLog("OnCreat_GoodsShowListPage")
    container:registerFunctionHandler(GoodsShowListPage.onFunction)
end

--冒号有隐含参数
function GoodsShowListPage:onFunction(eventName,container)
    if eventName == "luaEnter" then
        GoodsShowListPage.onEnter(container)
    elseif eventName == "luaExit" then
        GoodsShowListPage.onExit(container)
    elseif eventName == "luaLoad" then
        GoodsShowListPage.onLoad(container)
	elseif eventName == "onConfirm" then
		GoodsShowListPage.onConfirm(container)
    elseif eventName == "onClose" then
        GoodsShowListPage.closePage(container)
	elseif eventName == "onShareBtn" then
        GoodsShowListPage.onSharePage(container)
    end
end

function GoodsShowListPage.closePage(container)
	local msg = MsgMainFramePopPage:new()
	msg.pageName = "GoodsShowListPage"
	MessageManager:getInstance():sendMessageForScript(msg)
end

function GoodsShowListPage.onSharePage(container)
	PopupWeixinPage:Pop_Box()
end

function GoodsShowListPage.onConfirm(container)
	GoodsShowListPage.closePage(container)
end

function GoodsShowListPage.onEnter(container)
		
	local platform = PlatformRoleTableManager:getInstance():getPlatformRoleByName(libPlatformManager:getPlatform():getClientChannel())
	local node1 = container:getVarNode("mShareNode")
	local node2 = container:getVarNode("mNoShareNode")
	if node1 ~= nil and node2 ~= nil and platform ~= nil then
		if platform.shareSwitch == 0 then
			node1:setVisible(false)
			node2:setVisible(true)
		else
			node1:setVisible(true)
			node2:setVisible(false)
		end
	end	
	
    container:getVarLabelBMFont("mPackPageMsg"):setString(tostring(Language:getInstance():getString(GoodsViewPage.mMsgContent)))
	container:getVarLabelTTF("mTitle"):setString(tostring(Language:getInstance():getString(GoodsViewPage.mTitle)))
    container.m_pScrollViewFacade = CCReViScrollViewFacade:new(container.mScrollView)
	container.m_pScrollViewFacade:init(6,6)
    GoodsShowListPage.rebuildAllItem(container)
end

function GoodsShowListPage.onExit(container)	
    GoodsShowListPage.clearAllItem(container)
	container.m_pScrollViewFacade:delete()
	container.m_pScrollViewFacade = nil
end

function GoodsShowListPage.onLoad(container)
	container:loadCcbiFile("PackPreivew.ccbi")
	container.mScrollView = container:getVarScrollView("mContent")
	container.mScrollViewRootNode = container.mScrollView:getContainer()
end

function GoodsShowListPage.rebuildAllItem(container)
    GoodsShowListPage.clearAllItem(container);
	GoodsShowListPage.buildItem(container);
end

function GoodsShowListPage.buildItem(container)
    local iMaxNode = container.m_pScrollViewFacade:getMaxDynamicControledItemViewsNum();
	local iCount = 0;
	local fOneItemHeight = 0;
	local fOneItemWidth = 0;

	for i=#GoodsViewPage.mViewGoodsListInfo, 1, -1 do
		local pItemData = CCReViSvItemData:new()
		pItemData.mID = i
		pItemData.m_iIdx = i
		pItemData.m_ptPosition = ccp(0, fOneItemHeight*iCount)

		if iCount < iMaxNode then
			local pItem = ScriptContentBase:create("PackPreviewContent.ccbi")
			pItem.id = iCount
			pItem:registerFunctionHandler(GoodListContent.onFunction)
			if  fOneItemHeight < pItem:getContentSize().height then
				fOneItemHeight = pItem:getContentSize().height
			end
			if fOneItemWidth < pItem:getContentSize().width then
				fOneItemWidth = pItem:getContentSize().width
			end
			container.m_pScrollViewFacade:addItem(pItemData, pItem.__CCReViSvItemNodeFacade__)
		else
               container.m_pScrollViewFacade:addItem(pItemData)
        end
		iCount = iCount+1
	end
	local size = CCSizeMake(fOneItemWidth, fOneItemHeight*iCount)
	container.mScrollView:setContentSize(size);
	container.mScrollView:setContentOffset(ccp(0, container.mScrollView:getViewSize().height - container.mScrollView:getContentSize().height*container.mScrollView:getScaleY()));
	container.m_pScrollViewFacade:setDynamicItemsStartPosition(iCount-1);
end

function GoodsShowListPage.clearAllItem(container)
    container.m_pScrollViewFacade:clearAllItems()
    container.mScrollViewRootNode:removeAllChildren()
end
