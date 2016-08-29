

local thisPageName = "MercenaryPopPage"

local option = {
	ccbiFile = "MercenaryPopUp.ccbi",
	handlerMap = {
		onClose 		= "onClose"
	}
}

local MercenaryPopBase = {}

------------------------------------------------
local PageInfo = {
	roldId = 0
}

function MercenaryPopBase:onEnter( container )
	local info = ConfigManager.getRoleCfg()[PageInfo.roldId]
	--local pathPic = GameConfig.MercenaryPopPic[PageInfo.index]
	container:getVarSprite("mMercenary"):setTexture(info["poster"])
	--local name = 
	container:getVarLabelBMFont("mMercenaryName"):setString(info["name"])
end

function MercenaryPopBase:onExecute( container )

end

function MercenaryPopBase:onExit( container )

end

function MercenaryPopBase:onClose( container )
	PageManager.popPage(thisPageName)
end

function MercenaryPop_SetRoleId( roleId )
	PageInfo.roldId = roleId
end

-------------------------------------------------
local CommonPage = require("CommonPage")
local HelpPage = CommonPage.newSub(MercenaryPopBase, thisPageName, option)