local common = {};
------------local variable for system api--------------------------------------
local tostring = tostring;
local tonumber = tonumber;
local string = string;
local pairs = pairs;
--------------------------------------------------------------------------------

common.platform = {
CC_PLATFORM_UNKNOWN       	  =0,
CC_PLATFORM_IOS               =1,
CC_PLATFORM_ANDROID           =2,
CC_PLATFORM_WIN32             =3,
CC_PLATFORM_MARMALADE         =4,
CC_PLATFORM_LINUX             =5,
CC_PLATFORM_BADA              =6,
CC_PLATFORM_BLACKBERRY        =7,
CC_PLATFORM_MAC               =8,
CC_PLATFORM_NACL              =9,
CC_PLATFORM_EMSCRIPTEN        =10,
CC_PLATFORM_TIZEN             =11,
CC_PLATFORM_WINRT             =12,
CC_PLATFORM_WP8               =13
}


function common:numberIsOdd(digit)
	local isOdd = ((digit % 2) == 1)
	return isOdd
end

function common:numberIsEven(digit)
	local isOdd = ((digit % 2) == 0)
	return isOdd
end


function common:getSettingVar(varName)
	local setting, name = VaribleManager:getInstance():getSetting(varName)
	return setting
end


function common:getColorFromSetting(varName)
	--parseColor3B returns multi value
	local color3B = StringConverter:parseColor3B(self:getSettingVar(varName))
	return color3B
end

function common:getImageFileLargeQuality(quality)
	if quality~=nil then
		local filePath=VaribleManager:getInstance():getSetting("ImageFileLargeQuality"..tostring(quality))
		return filePath
	end
	return nil
end

function common:getMemBgImageFileQuality(quality)
	if quality~=nil then
		local filePath=VaribleManager:getInstance():getSetting("MemBgImageFileQuality"..tostring(quality))
		return filePath
	end
	return nil
end

function common:getQualityMaskImageFile(quality)
	if quality~=nil then
		local filePath=VaribleManager:getInstance():getSetting("ImageFileQualityMask"..tostring(quality))
		return filePath
	end
	return nil
end

function common:setBlackBoardVariable(key, val)
	key = tostring(key)
	if BlackBoard:getInstance():hasVarible(key) then
		BlackBoard:getInstance():setVarible(key, val)
	else
		BlackBoard:getInstance():addVarible(key, val)
	end
end

--Be carefull: hasVarible, getVarible has double return value
function common:getBlackBoardVariable(key)
	if BlackBoard:getInstance():hasVarible(key) then
		return (BlackBoard:getInstance():getVarible(key))
	end
	return nil
end

function common:getAdventureIdByTag(tag)
	local item=ServerDateManager:getInstance():getAdventureItemInfoByID(tag)
	if item~=nil then
		return item.adventureId
	end
	return 0
end

function common:getLanguageString(key, ...)
	return self:fill(Language:getInstance():getString(key), ...)
end

function common:second2DateString(second, withSecond)
	local hms = Split(GameMaths:formatSecondsToTime(second), ":")
	local dateStr = ""
	
	local h = tonumber(hms[1])
	if h > 0 then
		if h >= 24 then
			local d = math.floor(h / 24)
			dateStr = d .. Language:getInstance():getString("@Days")
		end
		dateStr = dateStr .. (h % 24) .. Language:getInstance():getString("@Hour")
	end
	
	local m = tonumber(hms[2])
	if h > 0 or m > 0 then
		dateStr = dateStr .. m .. Language:getInstance():getString("@Minute")
	end
	
	if withSecond == nil or withSecond == true or dateStr == "" then
		local s = tonumber(hms[3])
		dateStr = dateStr .. s .. Language:getInstance():getString("@Second")
	end
	
	return dateStr
end

-- 添加某个时间分量到时间日期串，如 append（result, 10，分，不允许0（allowZero=false))
function common:appendTimePart(dateSet, value, unit, allowZero)
	if allowZero then
		if value >= 0 then
			dateSet[#dateSet + 1] = value .. unit
		end
	else
		if value > 0 then
			dateSet[#dateSet + 1] = value .. unit
		end
	end
end

-- 秒转日期，返回包含两个单位的串（如：10天20时、10小时20分（秒）...)
function common:secondToDateXXYY(second)
	local h = math.floor(second / 3600)
	local d = math.floor(h / 24)
	local m = math.floor((second - h * 3600) / 60)
	local s = math.floor(second - h*3600 - 60 * m)
	
	local result = {}
	
	local timeParts = {
	d, (h-(d*24)), m, s
	}
	
	local timeUnits = {
	''..Language:getInstance():getString("@Days"),
	''..Language:getInstance():getString("@Hour"),
	''..Language:getInstance():getString("@Minute"),
	''..Language:getInstance():getString("@Second")
	}
	
	for i=1, #timeParts do
		common:appendTimePart(result, timeParts[i], timeUnits[i], false)
		if (#result > 1) then
			return result[1]..result[2]
		end
	end
	
	if #result == 1 then
		return result[1]
	else
		return ''..Language:getInstance():getString('@TimeEnd')
	end
end
-- added by sunyj
function common:stringToInt(str)
	local sum = 0
	local len = string.len(str)
	for index=1, len do
		local byteValue = string.byte(str, index) - string.byte('0')
		local inc = byteValue * math.pow(10, len - index)
		sum = sum + inc
	end
	return sum
end
-- end sunyj

--[[
function common:secondToDateXXYY(second)
	h = math.floor(second / 3600)
	d = math.floor(h / 24)
	m = math.floor((second - h * 3600) / 60)
	s = (second - h*3600 - 60 * m)
	
	result = {}
	
	common:appendTimePart(result, d, Language:getInstance():getString("@Days"), false)
	
	h = h - (d * 24)
	common:appendTimePart(result, h, Language:getInstance():getString("@Hour"), false)
	if #result > 1 then
		return result[1]..result[2]
	end
	
	common:appendTimePart(result, m, Language:getInstance():getString("@Minute"), false)
	if #result > 1 then
		return result[1]..result[2]
	end
	
	common:appendTimePart(result, s, Language:getInstance():getString("@Second"), false)
	if #result > 1 then
		return result[1]..result[2]
	end
	
	if #result == 1 then
		return result[1]
	end
	
	return '' .. Language:getInstance():getString('@TimeEnd')
end
--]]
function common:second2DateHourString(second)
	local hms = Split(GameMaths:formatSecondsToTime(second), ":")
	local dateStr = ""
	
	local h = tonumber(hms[1])
	if h > 0 then
		if h >= 24 then
			local d = math.floor(h / 24)
			dateStr = d .. Language:getInstance():getString("@Days")
		end
		dateStr = dateStr .. (h % 24) .. Language:getInstance():getString("@Hour")
	end
	
	return dateStr
end

function common:isCrossDay(lastTime)
	if lastTime then
		return self:isDifferendDay(lastTime, GamePrecedure:getInstance():getServerTime())
	end
	return false
end

function common:isDifferendDay(time1, time2)
	local ymd1 = Split(GameMaths:formatTimeToDate(tonumber(time1)), " ")
	local ymd2 = Split(GameMaths:formatTimeToDate(tonumber(time2)), " ")
	
	return ymd1[1] ~= ymd2[1]
end

function common:showGiftPackage(itemId)
	local item = ToolTableManager:getInstance():getToolItemByID(itemId)
	if item.includeStr == "none" or self:trim(item.includeStr) == "" then return end
	local gifts = Split(item.includeStr, ",")
	
	GoodsViewPage.mViewGoodsListInfo = {}
	for index, giftStr in ipairs(gifts) do
		local giftInfo = Split(giftStr, ":")
		self:table_map(giftInfo, tonumber)
		local gift = ResManager:getInstance():getResInfoByTypeAndId(giftInfo[1], giftInfo[2], giftInfo[3])
		GoodsViewPage.mViewGoodsListInfo[index] = {
		type = ResManager:getInstance():getResMainType(gift.type),
		name = gift.name,
		icon = gift.icon,
		count = gift.count,
		quality = gift.quality
		}
	end
	
	GoodsViewPage.mTitle="@PackPreviewTitleView"
	GoodsViewPage.mMsgContent="@PackPreviewMsgView"
	
	MainFrame:getInstance():pushPage("GoodsShowListPage")
end

-- added by sunyj
function common:showGiftPackageWithTitle(itemId, packageTitle, packageDesc)
	local item = ToolTableManager:getInstance():getToolItemByID(itemId)
	if item.includeStr == "none" or self:trim(item.includeStr) == "" then return end
	local gifts = Split(item.includeStr, ",")
	
	GoodsViewPage.mViewGoodsListInfo = {}
	for index, giftStr in ipairs(gifts) do
		local giftInfo = Split(giftStr, ":")
		self:table_map(giftInfo, tonumber)
		local gift = ResManager:getInstance():getResInfoByTypeAndId(giftInfo[1], giftInfo[2], giftInfo[3])
		GoodsViewPage.mViewGoodsListInfo[index] = {
		type = ResManager:getInstance():getResMainType(gift.type),
		name = gift.name,
		icon = gift.icon,
		count = gift.count,
		quality = gift.quality
		}
	end
	
	GoodsViewPage.mTitle= packageTitle
	GoodsViewPage.mMsgContent= (packageDesc ~= nil) and packageDesc or '@PackPreviewMsgView'
	
	MainFrame:getInstance():pushPage("GoodsShowListPage")
end
-- end

function common:showResInfo(itemType, itemId)
	local resType = ResManager:getInstance():getResMainType(itemType)
	if resType == TOOLS_TYPE then
		local item = ToolTableManager:getInstance():getToolItemByID(itemId)
		if item.includeStr == "none" or self:trim(item.includeStr) == "" then
			PropInfoPage:showPropInfoPage(itemId, 2, false) --2: SHOW_TYPE
		else
			self:showGiftPackage(itemId)
		end
	elseif resType == EQUIP_TYPE then
		EquipHandInfoPage:showEquipPage(itemId, true)
	elseif resType == DISCIPLE_TYPE or resType == DISCIPLE_TYPE then
		DiscipleHandInfoPage:showDisciplePage(itemId, true)
	elseif resType == SKILL_TYPE then
		SkillHandInfoPage:showSkillPage(itemId, true)
	elseif resType == TREASURE_TYPE then
		if BlackBoard:getInstance():hasVarible("IsTreasureItem") == false then
			BlackBoard:getInstance():addVarible("IsTreasureItem",true);
		else
			BlackBoard:getInstance():setVarible("IsTreasureItem",true);
		end
		BlackBoard:getInstance().ShowTreasure = itemId;
		PageManager.pushPage("TreasureInfoPage");
	end
	--	using the following code after the big version released
	--	PopupPageManager:showResInfo(itemType, itemId)
end

function common:showResInfoByRewardContent(rewardContent,index)
	local rewardItems = getResTable(rewardContent)
	for i=1,table.maxn(rewardItems) do
		if index==i then
			local rewardItem=rewardItems[i]
			if rewardItem~=nil then
				PopupPageManager:showResInfo(rewardItem.type,rewardItem.itemId)
			end
		end
	end
end

function common:goHelpPage(helpString)
	local helpInfo = {}
	local attrs = {"title", "content"}
	for _, item in ipairs(Split(helpString, "#")) do
		table.insert(helpInfo, self:table_combine(attrs, Split(item, "_")))
	end
	
	CommonHelpPageVar.set(helpInfo)
	MainFrame:getInstance():pushPage("CommonHelpPage")
end

function common:table_combine(keys, values)
	local tb = {}
	for index, key in ipairs(keys) do
		tb[key] = values[index]
	end
	return tb
end

function common:table_combineNumber(keys, start, step)
	local tb = {};
	
	local val = start or 0;
	local step = step or 1;
	for _, key in ipairs(keys) do
		tb[key] = val;
		val = val + step;
	end
	
	return tb;
end

--simple and rough version, be careful
function common:table_merge(...)
	local tb = {}
	for i = 1, select("#", ...) do
		table.foreach((select(i, ...)), function(k, v)
			tb[k] = v
		end)
	end
	return tb
end

function common:table_map(tb, func)
	table.foreach(tb, function(k, v) tb[k] = func(v) end)
end

function common:table_reflect(tb, func)
	local _tb = {};
	table.foreach(tb, function(k, v) _tb[k] = func(v) end);
	return _tb;
end

function common:table_keys(tb)
	local keys = {}
	for k, _ in pairs(tb) do table.insert(keys, k) end
	return keys
end

function common:table_values(tb)
	local values = {}
	table.foreach(tb, function(k, v) table.insert(values, v) end)
	return values
end

function common:table_hasValue(tb, val)
	for _, v in pairs(tb) do
		if v == val then return true; end
	end
	return false;
end

--not deep copy
function common:table_removeFromArray(tb, val)
	local _tb = {};
	for _, v in ipairs(tb) do
		if v ~= val then
			table.insert(_tb, v);
		end
	end
	return _tb;
end

--not deep copy
function common:table_sub(tb, start, len)
	local _tb = {};
	for i = start, start + len - 1 do
		local v = tb[i];
		if v then
			table.insert(_tb, v);
		end
	end
	return _tb;
end

function common:table_tail(tb, len)
	local _tb = {};
	for i = #tb, #tb - len + 1, -1 do
		local v = tb[i];
		if v then
			table.insert(_tb, v);
		end
	end
	return _tb;
end

function common:table_isEmpty(tb)
	if tb then
		for _, v in pairs(tb) do
			return false;
		end
	end
	return true;
end

function common:table_filter(tb, filter)
	local _tb = {};
	for k, v in pairs(tb) do
		if filter(k, v) then
			_tb[k] = v;
		end
	end
	return _tb;
end

function common:table_arrayFilter(tb, filter)
	local _tb = {};
	for _, v in pairs(tb) do
		if filter(v) then
			table.insert(_tb, v);
		end
	end
	return _tb;
end

function common:table_flip(tb)
	local _tb = {};
	table.foreach(tb, function(k, v) _tb[v] = k; end);
	return _tb;
end

function common:table_implode(tb, glue)
	local str = "";
	for k, v in pairs(tb) do
		if str ~= "" then
			str = str .. glue;
		end
		str = str .. tostring(v);
	end
	return str;
end

function common:table_count(tb)
	local c = 0;
	for k, v in pairs(tb) do c = c + 1; end
	return c;
end

function common:table_arrayIndex(tb, v)
	for i, _v in ipairs(tb) do
		if _v == v then
			return i;
		end
	end
	return -1;
end

function common:table_isSame(tb_1, tb_2)
	--to be better
	for k, v in pairs(tb_1) do
		if v ~= tb_2[k] then
			return false;
		end
	end
	for k, v in pairs(tb_2) do
		if v ~= tb_1[k] then
			return false;
		end
	end
	return true;
end

function common:trim(s)
	return (tostring(s or ""):gsub("^%s+", ""):gsub("%s+$", ""))
end

function common:fill(s, ...)
	local o = tostring(s)
	for i = 1, select("#", ...) do
		o = o:gsub("#v" .. i .. "#", tostring(select(i, ...)))
	end
	return o
end

function common:getPowSize(num)
	local powSize = 1;
	while num ~= 1 do
		num = math.ceil(num / 2)
		powSize = powSize * 2
	end
	return powSize
end

function common:deepCopy(object)
	local lookup_table = {}
	
	local function _copy(object)
		if type(object) ~= "table" then
			return object
		elseif lookup_table[object] then
			return lookup_table[object]
		end
		
		local new_table = {}
		lookup_table[object] = new_table
		for index, value in pairs(object) do
			new_table[_copy(index)] = _copy(value)
		end
		return setmetatable(new_table, getmetatable(object))
	end
	
	return _copy(object)
end

function common:stringAutoReturn(s, width, glue)
	local glue = glue or "\n";
	local lines = self:split(tostring(s), "\n")
	for i, line in ipairs(lines) do
		if width~=nil and tonumber(width)>=0 then
			lines[i] = GameMaths:stringAutoReturnForLua(line, width, 0)
		end
	end
	return table.concat(lines, glue)
end

function common:setQualityColor(node, quality)
	if node == nil then
		CCLuaLog("Error in common:setQualityColor==> node is nil")
		return
	end
	
	quality=common:getQuality(quality)
	
	local color = self:getSettingVar("FrameColor_Quality" .. quality)
	local color3B = StringConverter:parseColor3B(color)
	node:setColor(color3B)
end

function common:setFrameQuality(node, quality)
	if node == nil then
		CCLuaLog("Error in common:setFrameQuality==> node is nil")
		return
	end
	
	quality=common:getQuality(quality)
	
	node:setNormalImage(getFrameNormalSpirte(quality))
	node:setSelectedImage(getFrameSelectedSpirte(quality))
end

function common:getQuality(quality)
	if quality>QualityInfo.MaxQuality or quality<QualityInfo.MinQuality then
		quality=QualityInfo.NoQuality
	end
	return quality
end

function common:setScaleByResInfoType(node, itemType, commonScale)
	if node == nil then
		CCLuaLog("node is Null for set scale")
		return
	end
	
	itemType = tonumber(itemType or 0)
	local resType = ResManager:getInstance():getResMainType(itemType)
	local scale = commonScale or 0.4
	
	if resType == DISCIPLE_TYPE or resType == DISCIPLE_BOOK then
		scale = scale * 3.0
	end
	node:setScale(scale)
end

function common:log(format, ...)
	CCLuaLog(string.format(format, ...))
end

function common:sendPacket(opcode, message, needWaiting)
	local pb_data = message:SerializeToString();
	local needWaiting = needWaiting == nil and true or needWaiting;
	PacketManager:getInstance():sendPakcet(opcode, pb_data, #pb_data, needWaiting);
end

function common:sendEmptyPacket(opcode, needWaiting)
	local needWaiting = needWaiting == nil and true or needWaiting;
	PacketManager:getInstance():sendPakcet(opcode, "", 0, needWaiting);
end

function common:getEquipPartName(partId)
	return self:getLanguageString(string.format("@EquipPart_" .. partId));
end

function common:getAttrName(attrId)
	return self:getLanguageString(string.format("@AttrName_" .. attrId));
end

function common:getGsubStr( infoTab , str )
	if infoTab == nil or #infoTab == 0 then return end
	for i = 1 , #infoTab ,1 do
		str = string.gsub( str , "#v" .. i .. "#", infoTab[i] )
	end
	return str
end

function common:split(str, delim, maxNb)
	if string.find(str, delim) == nil then
		return {str}
	end
	if maxNb == nil or maxNb < 1 then
		maxNb = 0    -- No limit
	end
	local result = {}
	local pat = "(.-)" .. delim .. "()"
	local nb = 0
	local lastPos
	for part, pos in string.gfind(str, pat) do
		nb = nb + 1
		result[nb] = part
		lastPos = pos
		if nb == maxNb then break end
	end
	-- Handle the last field
	if nb ~= maxNb then
		result[nb + 1] = string.sub(str, lastPos)
	end
	return result;
end

function common:fillHtmlStr(key, ...)
	local id = GameConfig.FreeTypeId[key];
	if id ~= nil then
		local cfg = FreeTypeConfig[id];
		if cfg then
			return common:fill(cfg.content, ...);
		end
	end
	return "";
end

function common:getColorFromConfig(key)
	local cfg = GameConfig.Color[key] or {};
	local r, g, b = unpack(cfg);
	return ccc3(r, b, b);
end

function common:popString(str, colorKey)
	local wordList = {str};
	local colorList = {GameConfig.ColorMap[colorKey]};
	insertMessageFlow(wordList, colorList);
end

function common:checkStringLegal( str )
	--str = RestrictedWord:getInstance():filterWordSentence(str)
	str = str:gsub("&","*")
	str = str:gsub("#","*")
	str = str:gsub("<","*")
	str = str:gsub(">","*")
	return str
end
--true 男 false 女
function common:checkPlayerSexByItemId( itemId )
	if itemId >= 1 and itemId <= 3 then
		return true
	else
		return false
	end
end

----------------------------------------------------------------
return common;