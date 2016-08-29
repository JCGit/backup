local thisPageName = "NewbieGuidePopUp"
local NodeHelper = require("NodeHelper");
local option = {
	ccbiFile = "GeneralDecisionPopUp5.ccbi",
	handlerMap = {	
		onCancel = "onCancel"
	}
	--opcode = opcodes
}
local NewbieGuidePageBase = {}
local PageInfo = {
	title = "",
	content = ""
}
---------------------------------------------------------------------
function NewbieGuidePageBase:onEnter( container )
	container:getVarLabelBMFont("mTitle"):setString(PageInfo.title)
	labelNode = container:getVarLabelBMFont("mDecisionTex")
	local tag = GameConfig.Tag.HtmlLable
	local size = CCSizeMake(510, 200);
	
	if labelNode ~= nil then
		NodeHelper:addHtmlLable(labelNode, PageInfo.content ,tag, size)
	end
end

function NewbieGuidePageBase:onCancel( container )
	PageManager.popPage( thisPageName )
end

function NewbieGuidePage_SetInfo( tab )
	PageInfo.title = tab.title
	PageInfo.content = tab.content
end
---------------------------------------------------------------------
local CommonPage = require("CommonPage")
NewbieGuidePopUp = CommonPage.newSub(NewbieGuidePageBase, thisPageName, option)