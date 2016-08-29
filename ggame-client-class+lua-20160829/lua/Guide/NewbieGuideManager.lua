registerScriptPage("NewbieGuidePopUp")
local UserInfo = require("UserInfo");
local NewbieGuideManager = {}
------------------------------------------------------------------------------------------

function NewbieGuideManager.guide( key )
	UserInfo.syncPlayerInfo()
	local privateKey = string.format(key .. "_%d_%d", UserInfo.serverId, UserInfo.playerInfo.playerId)
	local hasTip = CCUserDefault:sharedUserDefault():getBoolForKey( privateKey )
	if not hasTip then
		NewbieGuidePage_SetInfo( NewbieGuideManager.getPopUpInfo(key) )
		PageManager.pushPage("NewbieGuidePopUp")
		CCUserDefault:sharedUserDefault():setBoolForKey(privateKey, true)
		CCUserDefault:sharedUserDefault():flush()
	end
end	

function NewbieGuideManager.getPopUpInfo( key )
	if key == GameConfig.NewbieGuide.FirstSmelt then
		return { title = common:getLanguageString("@NewbieSmeltTitle") , content = FreeTypeConfig[88].content }
	elseif key == GameConfig.NewbieGuide.FirstFight then
		return { title = common:getLanguageString("@NewbieFightTitle") , content = FreeTypeConfig[89].content }
	elseif key == GameConfig.NewbieGuide.FirstFailBoss then
		return { title = common:getLanguageString("@NewbieFailBossTitle") , content = FreeTypeConfig[90].content }
	elseif key == GameConfig.NewbieGuide.FirstSkill then
		return { title = common:getLanguageString("@NewbieSkillTitle") , content = FreeTypeConfig[91].content }
	elseif key == GameConfig.NewbieGuide.FirstFightBoss then
		return { title = common:getLanguageString("@NewbieBossTitle") , content = FreeTypeConfig[92].content }
	end
end



------------------------------------------------------------------------------------------
return NewbieGuideManager