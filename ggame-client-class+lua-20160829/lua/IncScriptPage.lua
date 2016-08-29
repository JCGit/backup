local pageMap = {};

function RegisterLuaPage(pageName)
	if pageMap[pageName] == nil then
		registerScriptPage(pageName);
		pageMap[pageName] = 1;
	end
end

local deviceTable = {
	"iPhone3,1",
	"iPhone3,2",
	"iPhone3,3",
	"iPhone4,1",
}
local platformInfo = libOS:getInstance():getPlatformInfo()
local posOfFirstUnderline = string.find(platformInfo, "#") or 1
local iosDeviceName = string.sub(platformInfo,1,posOfFirstUnderline-1)
local bIsLowDevice = false 

table.foreach(deviceTable, function(i, v)
	if iosDeviceName == v then
		bIsLowDevice = true
	else
		bIsLowDevice = false
	end
end)

local tPages = {}
local function registerInCoroutine( luaPage )
	local co = coroutine.create(function(  )
		registerScriptPage(luaPage)
	end)
	table.insert(tPages,co)
end
function dispatch()
	for i=1,#tPages do
		coroutine.resume(tPages[i])
	end
end

if bIsLowDevice == false then
	--竞技场页面
	registerInCoroutine("ArenaPage");
	--礼包页面
	registerInCoroutine("GiftPage");
	--邮件页面
	registerInCoroutine("MailPage");
	--充值页面
	registerInCoroutine("RechargePage");
	--帮助页面
	registerInCoroutine("HelpPage");
	--熔炼页面
	registerInCoroutine("MeltPage");
	--公会页面
	registerInCoroutine("GuildPage");
	--活动
	registerInCoroutine("ActivityPage");
	registerInCoroutine("MercenaryUpStepPage")
end
--主页面
registerInCoroutine("MainScenePage")
--战斗、聊天页面
registerInCoroutine("BattlePage")
--选角色时提示
registerInCoroutine("PromptPage");
--选人页面
registerInCoroutine("ChooseRolePage")
--装备页面
registerInCoroutine("EquipmentPage");
--背包页面
registerInCoroutine("PackagePage");
--技能页面
registerInCoroutine("SkillPage");
--商城页面
 registerInCoroutine("MarketPage");
--佣兵页面
registerInCoroutine("MercenaryPage");
--个人信息页面
registerInCoroutine("PlayerInfoPage");
--装备选择页面
registerInCoroutine("EquipSelectPage");
--多人团战页面
registerInCoroutine("RegimentWarPage");

dispatch()


