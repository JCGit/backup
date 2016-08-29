----------------------------------------------------------------------------------
--[[
FILE:			EliteMapManager.lua
ENCODING:		UTF-8, no-bomb
DESCRIPTION:	精英副本管理页面
AUTHOR:			zhenhui
CREATED:		2014-10-16
--]]
----------------------------------------------------------------------------------
local UserInfo = require("UserInfo");

local EliteMapManager = {}

local eliteMapLevelIndexCfg = ConfigManager.getEliteMapLevelIndex()

--const for W,H,M
EliteMapManager.WarriorCareer = 1
EliteMapManager.HunterCareer = 2
EliteMapManager.MasterCareer = 3

--current carrer index
EliteMapManager.curCareerIndex = 0 

--战士组
EliteMapManager.WGroup = {}
EliteMapManager.WGroupSize =0 
--猎人组
EliteMapManager.HGroup = {}
EliteMapManager.HGroupSize =0
--法师组
EliteMapManager.MGroup = {}
EliteMapManager.MGroupSize = 0 



EliteMapManager.passedEliteMapId = 0
EliteMapManager.curFightMap = 0
EliteMapManager.stageName = {
		[1] = {},
		[2] = {},
		[3] = {}
}
EliteMapManager.stageLevel = {
		[1] = {},
		[2] = {},
		[3] = {}
}

EliteMapManager.passedIdMap = {}


EliteMapManager.lastFightMapId = 0

function EliteMapManager:enterEliteMapInfoByProfId()
	if UserInfo.roleInfo.prof == 1 then 
		EliteMapManager:setCurCareerIndex(EliteMapManager.WarriorCareer)
	elseif UserInfo.roleInfo.prof == 2 then 
		EliteMapManager:setCurCareerIndex(EliteMapManager.HunterCareer) 
	elseif UserInfo.roleInfo.prof == 3 then 
		EliteMapManager:setCurCareerIndex(EliteMapManager.MasterCareer)
	end
	PageManager.changePage("EliteMapInfoPage")
end


function EliteMapManager:getEliteAttrById(mapId, attrName)
	local EliteMapCfg = ConfigManager.getEliteMapCfg();
	local config = EliteMapCfg[mapId];
	if config then
		return config[attrName];
	end
	return "";
end

function EliteMapManager:setCurCareerIndex(index)
	EliteMapManager.curCareerIndex = index
end

function EliteMapManager:onRecieveInfoSync(msg)
	for i=1,#msg.eliteMapInfos do
		local oneInfo = msg.eliteMapInfos[i] 
		EliteMapManager.passedIdMap[oneInfo.level] = oneInfo.mapId
	end
end

function EliteMapManager:getPassedMapIdByLevel(level)
	if EliteMapManager.passedIdMap[level] ~=nil  then
		return EliteMapManager.passedIdMap[level]
	else
		return 0
	end
end


function EliteMapManager:syncPaseedMapId()
	UserInfo.sync()
	EliteMapManager.passedEliteMapId = UserInfo.stateInfo.passEliteMapId
end


function EliteMapManager:getCurFightLevelIndex()
	local maxLevelIndex = EliteMapManager:getNextFightLevelIndex()
	if EliteMapManager.lastFightMapId ~= 0  then		
		local levelIndex = self:getLevelIndexById(EliteMapManager.lastFightMapId)
		if levelIndex~="" and levelIndex <=maxLevelIndex and levelIndex>=1 then
			return levelIndex	
		end
	end
	return maxLevelIndex
end

function EliteMapManager:getNextFightLevelIndex()
	-- if has last fight success map id ,use this id, else use curLevel id
	local userLevel = UserInfo.roleInfo.level
		if eliteMapLevelIndexCfg[userLevel]~=nil then
			local levelIndex = eliteMapLevelIndexCfg[userLevel]["index"]
			return levelIndex		
		else
			return 1
		end
end


function EliteMapManager:getNextMapId(mapId)
	if mapId == 0 or mapId<100000 then
		return 100101
	end
	return self:getEliteAttrById(mapId, "nextMapId");
end

function EliteMapManager:getDependMapId(mapId)
	return self:getEliteAttrById(mapId, "dependMap");
end

function EliteMapManager:getExpById(mapId)
	return self:getEliteAttrById(mapId, "exp");
end

function EliteMapManager:getCoinById(mapId)
	return self:getEliteAttrById(mapId, "coin");
end

function EliteMapManager:getStageName(mapId)
	return self:getEliteAttrById(mapId, "stageName");
end

function EliteMapManager:getBigGroupId(mapId)
	return self:getEliteAttrById(mapId, "group");
end

function EliteMapManager:getLevelById(mapId)
	return self:getEliteAttrById(mapId, "level");
end

function EliteMapManager:getLevelIndexById(mapId)
	return self:getEliteAttrById(mapId, "levelIndex");
end

function EliteMapManager:getNameById(mapId)
	return self:getEliteAttrById(mapId, "name");
end

function EliteMapManager:getQualityById(mapId)
	return self:getEliteAttrById(mapId, "quality");
end

function EliteMapManager:getMonsterLevelById(mapId)
	return self:getEliteAttrById(mapId, "monsterLevel");
end

function EliteMapManager:getisBossById(mapId)
	return self:getEliteAttrById(mapId, "isBoss");
end

function EliteMapManager:getIndexById(mapId)
	local index = math.mod(mapId,10)	
	return index
end


--获取精英地图总掉落组
function EliteMapManager:getDropItemCfg(mapId)
	local cfgStr = self:getEliteAttrById(mapId, "rewardItem");
	if cfgStr == "" or cfgStr == "0" then return nil; end
	
	local cfg = {};
	for _, item in ipairs(common:split(cfgStr, ",")) do
		local _type, _id, _count = unpack(common:split(item, "_"));
		table.insert(cfg, {
			type 	= tonumber(_type),
			itemId	= tonumber(_id),
			count 	= tonumber(_count)
		});
	end		
	return cfg;
end


--获取精英地图获得物品组
function EliteMapManager:getAwardItems(award)
	if award  == nil or award.drop == nil then return nil end
	local awardMap = {};
	local drop = award.drop  
	for i=1,#drop.detailEquip  do
		local oneEquip = drop.detailEquip[i]
		table.insert(awardMap, {
			type 	= 40000,
			itemId	= oneEquip.itemId,
			count 	= oneEquip.count
		});
	end
	
	for i=1,#drop.item  do
		local oneItem = drop.item[i]
		table.insert(awardMap, {
			type 	= oneItem.itemType,
			itemId	= oneItem.itemId,
			count 	= oneItem.itemCount
		});
	end
	
	for i=1,#drop.treasure  do
		local oneBox = drop.treasure[i]
		if oneBox.state ~= Battle_pb.TREASURE_DISCARD  then
			for i=1,#oneBox.item do
				local oneBoxItem = oneBox.item[i]
				table.insert(awardMap, {
					type 	= oneBoxItem.itemType,
					itemId	= oneBoxItem.itemId,
					count 	= oneBoxItem.itemCount
				});					
			end
		end
	end
	
	return awardMap;
end

function EliteMapManager:getMonsterInfo(mapId)
	local monsterCfg = ConfigManager.getMonsterCfg()
	local skillCfg = ConfigManager.getSkillCfg()
	
	
	local monsterInfo = {}
	
	for i=1,3 do
        local monster = {}
        monster.id = tonumber(self:getEliteAttrById(mapId, "monster"..i))
        
        if monster.id~=0 then
            monster.name = monsterCfg[monster.id]["name"]
            monster.pic = monsterCfg[monster.id]["icon"]
            monster.level = monsterCfg[monster.id]["level"]
            monster.skillName = {}
            local skillId = {}
            for _, item in ipairs(common:split(monsterCfg[monster.id]["skills"], ",")) do
                table.insert(skillId,tonumber(item))
            end
            
            for k,v in pairs(skillId) do
                if skillCfg[v]~=nil then 
                    table.insert(monster.skillName,skillCfg[v]["name"])
                end    
            end
            table.insert(monsterInfo,monster)
        end
    end
    return monsterInfo
end


	
function EliteMapManager:getMonsterPic(mapId)
	local monsterCfg = ConfigManager.getMonsterCfg()
	local monsterId = tonumber(self:getEliteAttrById(mapId, "monsterId"));
	local monsterInfo = monsterCfg[monsterId]
	if monsterInfo~=nil then
		local headPic = monsterInfo["icon"]
		return headPic
	else 
		return nil
	end
	
	
end


function EliteMapManager:getMapIdByLevelAndIndex(level,index)
	local mapId = 0
	if UserInfo.roleInfo.prof == 1 then
		mapId = EliteMapManager.WGroup[level][index]
	elseif UserInfo.roleInfo.prof == 2 then
		mapId = EliteMapManager.HGroup[level][index]
	elseif UserInfo.roleInfo.prof == 3 then
		mapId = EliteMapManager.MGroup[level][index]
	end
	return mapId
end

function EliteMapManager:classifyGroup(cfg)
	local size = #cfg
	
	if #EliteMapManager.WGroup > 0 then
		return 
	end
	for key, value in pairs(cfg) do
		local mapId = value["mapId"]
		local groupId = value["group"]
		local level = value["levelIndex"]
		local stageName = value["stageName"]
		local levelLimit = value["level"]
		if groupId == nil or level == nil then assert(false,"error in eliteMapcfg"..key) end
		if groupId == 1 then 
			local index = self:getIndexById(mapId)
			if EliteMapManager.WGroup[level] == nil then
				local levelGroup = {}				
				levelGroup[index] = mapId								
				EliteMapManager.WGroup[level] = levelGroup
				EliteMapManager.WGroupSize = EliteMapManager.WGroupSize +1
			else
				EliteMapManager.WGroup[level][index] = mapId			
			end				
			EliteMapManager.stageName[1][level] = stageName
			EliteMapManager.stageLevel[1][level] = levelLimit						
		elseif groupId == 2 then 
			local index = self:getIndexById(mapId)
			if EliteMapManager.HGroup[level] == nil then
				local levelGroup = {}				
				levelGroup[index] = mapId								
				EliteMapManager.HGroup[level] = levelGroup
				EliteMapManager.HGroupSize = EliteMapManager.HGroupSize +1
			else
				EliteMapManager.HGroup[level][index] = mapId			
			end			
			EliteMapManager.stageName[2][level] = stageName
			EliteMapManager.stageLevel[2][level] = levelLimit
		elseif groupId == 3 then 
			local index = self:getIndexById(mapId)
			if EliteMapManager.MGroup[level] == nil then
				local levelGroup = {}				
				levelGroup[index] = mapId								
				EliteMapManager.MGroup[level] = levelGroup
				EliteMapManager.MGroupSize = EliteMapManager.MGroupSize +1
			else
				EliteMapManager.MGroup[level][index] = mapId			
			end				
			EliteMapManager.stageName[3][level] = stageName
			EliteMapManager.stageLevel[3][level] = levelLimit
		end	
	end
	local validateFlag = (EliteMapManager.HGroupSize == EliteMapManager.WGroupSize) and (EliteMapManager.HGroupSize == EliteMapManager.MGroupSize)
	assert( validateFlag,"Error in elite map cfg.")
end

return EliteMapManager 