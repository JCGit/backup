local ConfigManager = {
	configs = {}
};
------------local variable for system api--------------------------------------
local tostring = tostring;
local tonumber = tonumber;
local string = string;
local pairs = pairs;
local type = type;
local unpack = unpack;
local table = table;
--------------------------------------------------------------------------------
local Const_pb = require("Const_pb");

local m_fileName = ""

--在win32下，检测该文件路径是否存在
function checkFilePathValidate(filePath)
	if CC_TARGET_PLATFORM_LUA ~= nil then 
		if CC_TARGET_PLATFORM_LUA == common.platform.CC_PLATFORM_WIN32 then
			local path = CCFileUtils:sharedFileUtils():fullPathForFilename(filePath)			
			if string.len(path) ==string.len(filePath) then
			    local sprite = CCSpriteFrameCache:sharedSpriteFrameCache():spriteFrameByName(filePath)
			    if sprite==nil then
			        local msg = "Error, pic path:"..filePath.." in file["..m_fileName.."] is not found\n"
				    CCMessageBox(msg,"Table is not right")
			    end
				return filePath
			else
				return path
			end
		end
	end		
	return filePath
end

function checkAllConfigFile()
	for key, value in pairs(ConfigManager) do
		if value and type(value) == "function" and key ~="loadCfg" and key ~= "getHelpCfg" and key ~= "getConfig"
            and key ~= "_getItemCfg"  and key~="getRewardById" and key ~= "getConsumeById" then
			pcall(value);
		end	
	end
end

function ConfigManager.loadCfg(fileName, attrMap, indexPos, convertMap)
	m_fileName = fileName
	local tableCfg = TableReaderManager:getInstance():getTableReader(fileName)
	local count = tableCfg:getLineCount() - 1
	local convertMap = convertMap or {};
	local Cfg = {}
	for i = 1, count do
		local index = i;
		if indexPos ~= nil then
			if type(indexPos) == "table" then
				local indexTb = common:table_reflect(indexPos, function(pos) return tableCfg:getData(i, pos); end);
				index = table.concat(indexTb, "_");
			else
				index = tonumber(tableCfg:getData(i, indexPos));
			end
		end
		
		if index then
			Cfg[index] = {}
			for attr, pos in pairs(attrMap) do
				local val = tableCfg:getData(i, pos);
				local func = convertMap[attr];
				if func and type(func) == "function" then
					val = func(val);
				end
				Cfg[index][attr] = val;
			end
		end
	end
	return Cfg
end

function ConfigManager.getConfig(key, loader)
	local cfg = ConfigManager.configs[key];
	if cfg == nil then
		cfg = loader();
		ConfigManager.configs[key] = cfg;
	end
	return cfg;
end



function ConfigManager.getRoleCfg()
	local key = "RoleCfg";
	local cfg = ConfigManager.configs[key];
	if cfg == nil then
		local attrMap = common:table_combineNumber({"id", "profession", "initLevel", "initExp", "name", "skills", "poster", "icon"});
		local convertMap = {
			["id"] = tonumber,
			["profession"] = tonumber,
			["initLevel"] = tonumber,
			["initExp"] = tonumber,
			["poster"] = checkFilePathValidate,
			["icon"] = checkFilePathValidate
		};
		cfg =  ConfigManager.loadCfg("role.txt", attrMap, 0, convertMap);
		ConfigManager.configs[key] = cfg;
	end
	return cfg;
end

function ConfigManager.getRoleLevelExpCfg()
	local key = "RoleLevelCfg";
	local cfg = ConfigManager.configs[key];
	if cfg == nil then
		local attrMap = common:table_combineNumber({"level", "exp"});
		local convertMap = {
			["level"] = tonumber,
			["exp"] = tonumber
		};
		cfg =  ConfigManager.loadCfg("roleLevelExp.txt", attrMap, 0, convertMap);
		ConfigManager.configs[key] = cfg;
	end
	return cfg;
end

function ConfigManager.getMapCfg()
	local key = "MapCfg";
	local cfg = ConfigManager.configs[key];
	if cfg == nil then
		local attrMap = common:table_combineNumber({"id", "name", "bossId", "path", "monsterLevel", "equipRate"});
		local convertMap = {
			["id"] = tonumber,
			["path"] = checkFilePathValidate,
			["bossId"] = tonumber
			
		};
		cfg =  ConfigManager.loadCfg("map.txt", attrMap, 0, convertMap);
		ConfigManager.configs[key] = cfg;
	end
	return cfg;
end

function ConfigManager.getVipCfg()
	local key = "vipCfg";
	local cfg = ConfigManager.configs[key];
	if cfg == nil then
		local attrMap = common:table_combineNumber({"vipLevel", "buyDiamon", "shopItemCount", "buyCoinTime", "fastFightTime", 
		"bossFightTime","buyBossFightTime","bossExtraAward","maxMercenaryTime" , "hasBossMopUp" , "hasUnionBoss"
		,"eliteFightTime","buyEliteFightTime","soulStoneNum","expBuffer"});
		local convertMap = {
			["bossFightTime"] = tonumber,
			["buyDiamon"] = tonumber,
			["shopItemCount"] = tonumber,
			["buyCoinTime"] = tonumber,
			["fastFightTime"] = tonumber,
			["bossFightTime"] = tonumber,
			["buyBossFightTime"] = tonumber,			
			["maxMercenaryTime"] = tonumber,
			["hasBossMopUp"] = tonumber,
			["hasUnionBoss"] = tonumber,
			["eliteFightTime"] = tonumber,
			["buyEliteFightTime"] = tonumber,
			["soulStoneNum"] = tonumber,
			["expBuffer"] = tonumber			
		};
		cfg =  ConfigManager.loadCfg("vipPrivilege.txt", attrMap, 0, convertMap);
		ConfigManager.configs[key] = cfg;
	end
	return cfg;
end

function ConfigManager.getEquipCfg()
	local key = "EquipCfg";
	local cfg = ConfigManager.configs[key];
	if cfg == nil then
		
		local attrMap = common:table_combineNumber({
			"id", "isOpen", "name", "partName", "part", 
			"quality", "level",	"profession", "smeltGain", "forgeSmeltNeed",
			"washCoinCost",	"equipAttr", "icon", "punchConsume","stepLevel"
		});
		local convertMap = {
		    ["stepLevel"] = tonumber,
			["part"] = tonumber,
			["quality"] = tonumber,
			["sellPrice"] = tonumber,
			["smeltGain"] = tonumber,
			["forgeSmeltNeed"] = tonumber,
			["washCoinCost"] = tonumber,
			["icon"] = checkFilePathValidate,
			["profession"]	= function(str)
				return common:split(str, ",");
			end,
			["punchConsume"] = function(str)
				local consumes = {};
				for i, subStr in ipairs(common:split(str, ",")) do
					local _type, _id, _count = unpack(common:split(subStr, "_"));
					consumes[i] = {
						type 	= tonumber(_type),
						id 		= tonumber(_id),
						count	= tonumber(_count)
					};
				end
				return consumes;
			end
		};
		cfg =  ConfigManager.loadCfg("equip.txt", attrMap, 0, convertMap);
		ConfigManager.configs[key] = cfg;
	end
	return cfg;
end

function ConfigManager.getItemCfg()
	local key = "ItemCfg";
	local cfg = ConfigManager.configs[key];
	if cfg == nil then
		
		local attrMap = common:table_combineNumber({
			"id", "name", "type", "levelLimit", "profLimit",
			"price", "needItem", "containItem", "levelUpCost", "levelUpRate",
			"levelUpItem", "strength", "agility", "intellect", "stamina",
			"icon", "levelUpCostMax", "description", "smeltCost","quality",
			"jumpPage","soulStoneExp"
		});
		local convertMap = {
			["id"] = tonumber,
			["type"] = tonumber,
			["levelLimit"] = tonumber,
			["profLimit"] = tonumber,
			["price"] = tonumber,
			["strength"] = tonumber,
			["agility"] = tonumber,
			["icon"] = checkFilePathValidate,
			["intellect"] = tonumber,
			["stamina"] = tonumber,
			["quality"] = tonumber,
			["smeltCost"] = tonumber,
			["soulStoneExp"] = tonumber,
			["levelUpCostMax"] = function(str)
				local max, step = unpack(common:split(str, ","));
				return tonumber(max);
			end,
			["levelUpCost"] = function(str)
				--egg ache
				local _coin, _itemId, _count = unpack(common:split(str, "_"));
				local consumes = {};
				_coin = tonumber(_coin);
				-- 消耗金币
				if _coin >= 30000 then
					table.insert(consumes, {
						type 	= Const_pb.PLAYER_ATTR * 10000,
						id		= Const_pb.COIN,
						count	= _coin
					});
				-- 消耗钻石
				elseif _coin >0 and _coin < 1000 then
					table.insert(consumes, {
						type 	= Const_pb.PLAYER_ATTR * 10000,
						id		= Const_pb.GOLD,
						count	= _coin
					});
				end
				
				if _itemId ~= nil and _count ~= nil and _itemId~="0" and _count~="0" then
					table.insert(consumes, {
						type 	= Const_pb.TOOL * 10000,
						id 		= tonumber(_itemId),
						count	= tonumber(_count)
					});
				end
				return consumes;
			end
		};
		cfg =  ConfigManager.loadCfg("item.txt", attrMap, 0, convertMap);
		ConfigManager.configs[key] = cfg;
	end
	return cfg;
end


function ConfigManager.getResPropertyCfg()
	local key = "ResPropCfg";
	local cfg = ConfigManager.configs[key];
	if cfg == nil then
		
		local attrMap = common:table_combineNumber({"id", "name", "type", "discribe"
		});
		local convertMap = {
			["id"] = tonumber,
			["type"] = tonumber			
		};
		cfg =  ConfigManager.loadCfg("ResPropertyConfig.txt", attrMap, 0, convertMap);
		ConfigManager.configs[key] = cfg;
	end
	return cfg;
end

function ConfigManager.getUserPropertyCfg()
	local key = "UserPropCfg";
	local cfg = ConfigManager.configs[key];
	if cfg == nil then
		
		local attrMap = common:table_combineNumber({"id", "name", "quality", "discribe","icon"
		});
		local convertMap = {
			["id"] = tonumber,
			["icon"] = checkFilePathValidate,
			["quality"] = tonumber			
		};
		cfg =  ConfigManager.loadCfg("UserProperty.txt", attrMap, 0, convertMap);
		ConfigManager.configs[key] = cfg;
	end
	return cfg;
end

function ConfigManager._getItemCfg(itemStr)
	local cfgAttr = {"type", "id", "count"}
	local cfg = {}
	for index, val in ipairs(Split(itemStr, ":")) do
		cfg[cfgAttr[index]] = tonumber(val)
	end
	return cfg
end

function ConfigManager.getMonsterCfg()
	local key = "monster"
	local cfg = ConfigManager.configs[key]
	if cfg == nil then
		
		local attrMap = common:table_combineNumber({"id", "profession", "level", "name", "skills",
			"icon", "isBoss"
		})
		local convertMap = {
			["id"] = tonumber,
			["profession"] = tonumber,
			["level"] = tonumber,
			["icon"] = checkFilePathValidate,
			["isBoss"] = tonumber
		}
		cfg =  ConfigManager.loadCfg("monster.txt", attrMap, 0, convertMap)
		ConfigManager.configs[key] = cfg
	end
	return cfg
end

function ConfigManager.getSkillCfg()
	local key = "skillCfg"
	local cfg = ConfigManager.configs[key]
	if cfg == nil then
		
		local attrMap = common:table_combineNumber({"id", "name", "profession", "openLevel", "costMP",
			"coldRound", "skillScript", "describe","icon","battleAction","battleRecord","roleId"
		})
		local convertMap = {
			["id"] = tonumber,
			["profession"] = tonumber,
			["openLevel"] = tonumber,
			["costMP"] = tonumber,
			["roleId"] = tonumber
		}
		cfg =  ConfigManager.loadCfg("skill.txt", attrMap, 0, convertMap)
		ConfigManager.configs[key] = cfg
	end
	--local SkillManager = require ("SkillManager")
	
	return cfg
end

function ConfigManager.getBuffCfg()
	local key = "buffCfg"
	local cfg = ConfigManager.configs[key]
	if cfg == nil then
		
		local attrMap = common:table_combineNumber({"id", "name","isAdd"
		})
		local convertMap = {
			["id"] = tonumber,
			["isAdd"] = tonumber
		}
		cfg =  ConfigManager.loadCfg("buff.txt", attrMap, 0, convertMap)
		ConfigManager.configs[key] = cfg
	end
	return cfg
end

function ConfigManager.getHelpCfg( key )
	--local key = "helpCfg"
	local cfg = ConfigManager.configs[key]
	if cfg == nil or cfg == {} then
		
		local attrMap = common:table_combineNumber({"id", "content"
		})
		local convertMap = {
			["id"] = tonumber
		}
		cfg =  ConfigManager.loadCfg(key .. ".txt", attrMap, 0, convertMap)
		ConfigManager.configs[key] = cfg
	end
	return cfg
end

-- added by sunyj for alliance
function ConfigManager.getAllianceBossCfg()
	local key = "AllianceBossConfig"
	local cfg = ConfigManager.configs[key]
	if cfg == nil then
		local attrMap = common:table_combineNumber({'id', 'bossName', 'level', 'bossExp','bossId', 'bossBlood' })
		local convertMap = {
			["id"] = tonumber,
			['level'] = tonumber,
			['bossExp'] = tonumber,
			['bossId'] = tonumber,
			['bossBlood'] = tonumber
		}
		cfg =  ConfigManager.loadCfg("alliance.txt", attrMap, 0, convertMap)
		ConfigManager.configs[key] = cfg
	end
	return cfg
end

-- added by sunyj for mission gift
function ConfigManager.getGiftConfig()
	local key = "Mission"
	local cfg = ConfigManager.configs[key]
	if cfg == nil then
		local attrMap = common:table_combineNumber({'id', 'name', 'type', 'reward', 'icon', 'desc'})
		local convertMap = {
			['id'] = tonumber,
			['type'] = tonumber,
			["icon"] = checkFilePathValidate,
		}
		cfg =  ConfigManager.loadCfg("mission.txt", attrMap, 0, convertMap)
		ConfigManager.configs[key] = cfg
	end
	return cfg
end

-- added by sunyj for skill open 
function ConfigManager.getSkillOpenCfg()
	local key = "SkillOpenCfg"
	local cfg = ConfigManager.configs[key]
	if cfg == nil then
		local attrMap = common:table_combineNumber({'id', 'openLevel'})
		local convertMap = {
			['id'] = tonumber,
			['openLevel'] = tonumber,
		}
		cfg =  ConfigManager.loadCfg("SkillOpenCfg.txt", attrMap, 0, convertMap)
		ConfigManager.configs[key] = cfg
	end
	return cfg
end

function ConfigManager.getMercenarySkillOpenCfg()
	local key = "MercenarySkillOpenCfg"
	local cfg = ConfigManager.configs[key]
	if cfg == nil then
		local attrMap = common:table_combineNumber({'id', 'openLevel'})
		local convertMap = {
			['id'] = tonumber,
			['openLevel'] = tonumber,
		}
		cfg =  ConfigManager.loadCfg("MercenarySkillOpenCfg.txt", attrMap, 0, convertMap)
		ConfigManager.configs[key] = cfg
	end
	return cfg
end

-- added by zhaolu for background music 
function ConfigManager.getBGMusicCfg()
	local key = "BGMusicCfg"
	local cfg = ConfigManager.configs[key]
	if cfg == nil then
		local attrMap = common:table_combineNumber({'id', 'chineseName', 'englishName', 'musicPath'})
		local convertMap = {
			['id'] = tonumber,
			['musicPath'] = checkFilePathValidate,
		}
		cfg =  ConfigManager.loadCfg("musicCfg.txt", attrMap, 0, convertMap)
		ConfigManager.configs[key] = cfg
	end
	return cfg
end

-- added by zhaolu for playerTitle
function ConfigManager.getPlayerTitleCfg()
	local key = "PlayerTitleCfg"
	local cfg = ConfigManager.configs[key]
	if cfg == nil then
		local attrMap = common:table_combineNumber({'id', 'name', 'describe', 'type', 'picPath', 'BGPath'})
		local convertMap = {
			['id'] = tonumber,
			['type'] = tonumber,
			['picPath'] = checkFilePathValidate,
			['BGPath'] = checkFilePathValidate,
		}
		cfg =  ConfigManager.loadCfg("PlayerTitle.txt", attrMap, 0, convertMap)
		ConfigManager.configs[key] = cfg
	end
	return cfg
end

-- added by zhaolu for day login
function ConfigManager.getLoginCfg()
	local key = "ActDayLoginCfg"
	local cfg = ConfigManager.configs[key]
	if cfg == nil then
		local attrMap = common:table_combineNumber({'day', 'description'})
		local convertMap = {
			['day'] = tonumber,
		}
		cfg =  ConfigManager.loadCfg("activityDayLogin.txt", attrMap, 0, convertMap)
		ConfigManager.configs[key] = cfg
	end
	return cfg
end

function ConfigManager.getGodlyAttrCfg()
	local key = "GodlyAttrCfg";
	local cfg = ConfigManager.configs[key];
	if cfg == nil then
		local attrMap = common:table_combineNumber({"id", "attrId", "enhanceAttrs", "baseAttr", "growAttr"});
		local convertMap = {
			["attrId"] = tonumber,
			["baseAttr"] = tonumber,
			["growAttr"] = tonumber,
			["enhanceAttrs"] = function(attrStr)
				return common:split(attrStr, ",");
			end
		};
		cfg =  ConfigManager.loadCfg("godlyAttr.txt", attrMap, 1, convertMap);
		ConfigManager.configs[key] = cfg;
	end
	return cfg;
end

function ConfigManager.getEquipEnhanceAttrCfg()
	local key = "EquipEnhanceAttrCfg";
	local cfg = ConfigManager.configs[key];
	if cfg == nil then
		local attrMap = {mainAttr = 1};
		local convertMap = {
			mainAttr = function(attrStr)
				return common:split(attrStr, ",");
			end
		};
		cfg =  ConfigManager.loadCfg("equipStrength.txt", attrMap, 0, convertMap);
		ConfigManager.configs[key] = cfg;
	end
	return cfg;
end

function ConfigManager.getEquipEnhanceWeightCfg()
	local key = "EquipEnhanceWeightCfg";
	local cfg = ConfigManager.configs[key];
	if cfg == nil then
		local attrMap = {weight = 2};
		local convertMap = {
			["weight"] = tonumber
		};
		cfg =  ConfigManager.loadCfg("equipStrengthRatio.txt", attrMap, {0, 1}, convertMap);
		ConfigManager.configs[key] = cfg;
	end
	return cfg;
end

function ConfigManager.getGoldlyLevelExpCfg()
	local key = "GoldlyLevelExpCfg";
	local cfg = ConfigManager.configs[key];
	if cfg == nil then
		local attrMap = {exp = 1, exp2 = 2};
		local convertMap = {
			["exp"] = tonumber,
			["exp2"] = tonumber
		};
		cfg =  ConfigManager.loadCfg("godlyLevelExp.txt", attrMap, 0, convertMap);
		ConfigManager.configs[key] = cfg;
	end
	return cfg;
end

function ConfigManager.getBattleParamCfg()
	local key = "BattleParamCfg";
	local cfg = ConfigManager.configs[key];
	if cfg == nil then
		local attrMap = common:table_combineNumber({
			"level", "Attr_105", "Attr_106", "Attr_107", "Attr_108",
			"Attr_111", "Attr_109", "Attr_110", "Attr_1007", "Attr_2103",
			"Attr_2104"
		});			
		cfg =  ConfigManager.loadCfg("battleParameter.txt", attrMap, 0);
		ConfigManager.configs[key] = cfg;
	end
	return cfg;
end

-- add by zhaozhen for market
function ConfigManager.getMarketBuyCoinCountCfg()
	local key = "MarketBuyCoinCountCfg";
	local cfg = ConfigManager.configs[key]
	if cfg == nil then
		local attrMap = common:table_combineNumber({'vipLevel', 'buyCount' })
		local convertMap = {
			['vipLevel'] = tonumber,
			['buyCount'] = tonumber
		}
		cfg =  ConfigManager.loadCfg("MarketCoinCount.txt", attrMap, 0, convertMap)
		ConfigManager.configs[key] = cfg
	end
	return cfg
end

-- add by zhaozhen for team battle kick
function ConfigManager.getTeamBattleKickCfg()
	local key = "TeamBattleKickCfg";
	local cfg = ConfigManager.configs[key]
	if cfg == nil then
		local attrMap = common:table_combineNumber({'kickCount', 'goldCost' })
		local convertMap = {
			['kickCount'] = tonumber,
			['goldCost'] = tonumber
		}
		cfg =  ConfigManager.loadCfg("TeamBattleKick.txt", attrMap, 0, convertMap)
		ConfigManager.configs[key] = cfg
	end
	return cfg
end

-- add by zhaozhen for email
function ConfigManager.getMailContentCfg()
	local key = "MailContentCfg";
	local cfg = ConfigManager.configs[key]
	if cfg == nil then
		local attrMap = common:table_combineNumber({'mailId', 'content' })
		local convertMap = {
			['mailId'] = tonumber
		}
		cfg =  ConfigManager.loadCfg("MailIdConfig.txt", attrMap, 0, convertMap)
		ConfigManager.configs[key] = cfg
	end
	return cfg
end

-- add by zhaozhen for refresh market cost
function ConfigManager.getRefreshMarketCostCfg()
	local key = "RefreshMarketCostCfg";
	local cfg = ConfigManager.configs[key]
	if cfg == nil then
		local attrMap = common:table_combineNumber({'id', 'cost' })
		local convertMap = {
			['id'] = tonumber,
			['cost'] = tonumber
		}			
		cfg =  ConfigManager.loadCfg("ShopRefreshCost.txt", attrMap, 0, convertMap)
		ConfigManager.configs[key] = cfg
	end
	return cfg
end

--[[
function ConfigManager.getPlatformName()
	local key = "PlatformName"
	local cfg = ConfigManager.configs[key]
	if cfg == nil then
		local attrMap = common:table_combineNumber({'id' , 'name'})
		local convertMap = {
			['id'] = tonumber
		}
		cfg = ConfigManager.loadCfg("PlatformName.txt")
	end

end
--]]

function ConfigManager.getRewardCfg()
	local key = "RewardCfg";
	local cfg = ConfigManager.configs[key]
	if cfg == nil then
		local attrMap = {rewardItems = 1};
		cfg =  ConfigManager.loadCfg("reward.txt", attrMap, 0);
		ConfigManager.configs[key] = cfg;
	end
	return cfg;
end

function ConfigManager.getRewardById(rewardId)
	local allCfg = ConfigManager.getRewardCfg();
	local cfg = allCfg[rewardId] or {};
	
	local rewardItems = {};
	if cfg["rewardItems"] ~= nil then
		for _, item in ipairs(common:split(cfg["rewardItems"], ",")) do
			local _type, _id, _count = unpack(common:split(item, "_"));
			table.insert(rewardItems, {
				type 	= tonumber(_type),
				itemId	= tonumber(_id),
				count 	= tonumber(_count)
			});
		end
	end
	return rewardItems;
end

function ConfigManager.getConsumeCfg()
	local key = "ConsumeCfg";
	local cfg = ConfigManager.configs[key]
	if cfg == nil then
		local attrMap = {type = 1, consumeItems = 2};
		cfg =  ConfigManager.loadCfg("consume.txt", attrMap, 0);
		ConfigManager.configs[key] = cfg;
	end
	return cfg;
end

function ConfigManager.getConsumeById(consumeId)
	local allCfg = ConfigManager.getConsumeCfg();
	local cfg = allCfg[consumeId] or {};
	
	local consumeItems = {};
	if cfg["consumeItems"] ~= nil then
		for _, item in ipairs(common:split(cfg["consumeItems"], ",")) do
			local _type, _id, _count = unpack(common:split(item, "_"));
			table.insert(consumeItems, {
				type 	= tonumber(_type),
				itemId	= tonumber(_id),
				count 	= tonumber(_count)
			});
		end
	end
	return {
			type = tonumber(cfg["type"]),
			items = consumeItems
		};
end

--error code
function ConfigManager.getErrorCodeCfg()
	local key = "ErrorCode";
	local cfg = ConfigManager.configs[key];
	if cfg == nil then
		local attrMap = common:table_combineNumber({"id","content"});
		local convertMap = {
			["id"] = tonumber
		};
		cfg =  ConfigManager.loadCfg("ErrorCode.txt", attrMap, 0, convertMap);
		ConfigManager.configs[key] = cfg;
	end
	return cfg;
end

function ConfigManager.getGodlyEquipCanBuild()
	local key = "GodlyEquip";
	local cfg = ConfigManager.configs[key];
	if cfg == nil then
		local attrMap = common:table_combineNumber({"id", "equipId", "attrCount", "reputation", "smeltValue", "minLv", "maxLv"});
		local convertMap = {
			["id"] = tonumber,
			["equipId"] = tonumber,
			["attrCount"] = tonumber,
			["reputation"] = tonumber,
			["smeltValue"] = tonumber,
			["minLv"] = tonumber,
			["maxLv"] = tonumber
		};
		cfg =  ConfigManager.loadCfg("godlyEquip.txt", attrMap, nil, convertMap);
		ConfigManager.configs[key] = cfg;
	end
	return cfg;
end

function ConfigManager.getCampWarWinStreak()
	local key = "CampWarStreak";
	local cfg = ConfigManager.configs[key];
	if cfg == nil then
		local attrMap = common:table_combineNumber({"id", "winStreak", "winCoinsRatio", "winReputation", "loseCoinsRatio", "loseReputation"});
		local convertMap = {
			["id"] = tonumber,
			["winStreak"] = tonumber,
			["winCoinsRatio"] = tonumber,
			["winReputation"] = tonumber,
			["loseCoinsRatio"] = tonumber,
			["loseReputation"] = tonumber
		};
		cfg =  ConfigManager.loadCfg("campWarWinStreak.txt", attrMap, nil, convertMap);
		ConfigManager.configs[key] = cfg;
	end
	return cfg;
end

function ConfigManager.getVipWelfareCfg()
	local key = "VipWelfare";
	local cfg = ConfigManager.configs[key];
	if cfg == nil then
		local attrMap = common:table_combineNumber({"id", "reward"});
		local convertMap = {
			["id"] = tonumber			
		};
		cfg =  ConfigManager.loadCfg("VipWelfare.txt", attrMap, nil, convertMap);
		ConfigManager.configs[key] = cfg;
	end
	return cfg;
end
function ConfigManager.getEliteMapCfg()
	local key = "EliteMapCfg";
	local cfg = ConfigManager.configs[key];
	if cfg == nil then
		local attrMap = common:table_combineNumber({"mapId", "group","levelIndex", "level", "nextMapId","dependMap", 
		"rewardItem", "monsterId","name", "monsterLevel", "isBoss", "quality","stageName","exp","coin","monster1","monster2","monster3"});
		local convertMap = {
			["mapId"] = tonumber,
			["group"] = tonumber,
			["levelIndex"] = tonumber,
			["level"] = tonumber,
			["nextMapId"] = tonumber,			
			["dependMap"] = tonumber,
			["monsterId"] = tonumber,
			["monsterLevel"] = tonumber,
			["isBoss"] = tonumber,
			["quality"] = tonumber,
			["exp"] = tonumber,
			["coin"] = tonumber,
			["monster1"] = tonumber,
			["monster2"] = tonumber,
			["monster3"] = tonumber
		};
		cfg =  ConfigManager.loadCfg("EliteMapCfg.txt", attrMap, 0, convertMap);
		ConfigManager.configs[key] = cfg;
	end
	--classify the data
	local EliteMapManager = require("EliteMapManager")
	EliteMapManager:classifyGroup(cfg)
	return cfg;
end

function ConfigManager.getMercenaryUpStepTable()
	local key = "starLevelExp";
	local cfg = ConfigManager.configs[key];
	if cfg == nil then
		local attrMap = common:table_combineNumber({"id","roleId", "stepLevel","exp", "hitCoe", "dodgeCoe","critCoe", 
		"tenacityCoe", "phyResistanceCoe","magResistanceCoe", "hpCoe", "harmCoe","magicCoe"});
		local convertMap = {
            ["roleId"] = tonumber,
            ["stepLevel"] = tonumber
        }
		cfg =  ConfigManager.loadCfg("starLevelExp.txt", attrMap, 0, convertMap);
		ConfigManager.configs[key] = cfg;
	end
	return cfg;
end

function ConfigManager.getMercenaryRingCfg()
	local key = "mercenaryRing";
	local cfg = ConfigManager.configs[key];
	if cfg == nil then
		local attrMap = common:table_combineNumber({"ringId", "merId","starLimit", "consume", "name","discribe", 
		"icon","condition"});
		local convertMap = {
			["ringId"] = tonumber,
			["merId"] = tonumber,
			["starLimit"] = tonumber,
			["icon"] = checkFilePathValidate,
			["consume"] = tonumber
		};
		cfg =  ConfigManager.loadCfg("ring.txt", attrMap, 0, convertMap);
		ConfigManager.configs[key] = cfg;
	end
	
	local MercenaryHaloManager = require("MercenaryHaloManager")
	MercenaryHaloManager:classifyGroup(cfg)
	return cfg;
end



function ConfigManager.getEliteMapLevelIndex()
	local key = "EliteMapLevelIndex";
	local cfg = ConfigManager.configs[key];
	if cfg == nil then
		local attrMap = common:table_combineNumber({"userLevel", "index"});
		local convertMap = {
			["userLevel"] = tonumber,
			["index"] = tonumber
		};
		cfg =  ConfigManager.loadCfg("eliteMapLevelIndex.txt", attrMap, 0, convertMap);
		ConfigManager.configs[key] = cfg;
	end	
	return cfg;
end

function ConfigManager.getTimeLimitPurcgaseItem()
	local key = "TimeLimitPurchase";
	local cfg = ConfigManager.configs[key];
	if cfg == nil then
		local attrMap = common:table_combineNumber({"id", "goodsId","originalPrice","salePrice","vipLimit","buyType","maxBuyTimes"});
		local convertMap = {
			["id"] = tonumber,
			["goodsId"] = tonumber
		};
		cfg =  ConfigManager.loadCfg("TimeLimitPurchase.txt", attrMap, 0, convertMap);
		ConfigManager.configs[key] = cfg;
	end	
	return cfg;
end

--[[if CC_TARGET_PLATFORM_LUA ~= nil then 
	if CC_TARGET_PLATFORM_LUA == common.platform.CC_PLATFORM_WIN32 then
		pcall(checkAllConfigFile);
	end
end--]]

-----------------------------------------------------------------
return ConfigManager;
