require "HP_pb"
require "Battle_pb"
--encoding UTF-8 without BOM
--------------------------------------------------------------------------------
--  FILE:			ViewBattleSceneHelpter.lua
--  DESCRIPTION:	ViewBattleSceneHelpter page
--
--
--  AUTHOR:		Zhen Hui
--  CREATED:	2014/11/5
--  MODIFY2:
--
--  MODIFY1:
--
--------------------------------------------------------------------------------

local ViewBattleSceneHelpter = {}


FightState = {
FightRequired = 1;			--请求战斗
BeforeFightStart = 2;		--战斗准备。人物准备和战斗开始动画播放
Fighting = 3;				--战斗序列
FightEnd = 4;				--战斗结束
Waiting =  5;				--等待状态
}
local ResManager = require("ResManagerForLua");

local GameConfig = require("GameConfig");
--战斗数据相关逻辑处理
local BattleDataHelper = require "BattleDataHelper"

local BattleFightSceneHelper = require "BattleFightSceneHelper"

--战斗角色数据，key为pos,value 为cha
ViewBattleSceneHelpter.FightChar = {};

local m_samePlayerRelogin = false
  
local currentContentIndex =0;
local maxContentSize = 100;
local currentSVPos = 0;




local wordList = {};

--战斗时间
local fightList = {};
local fightingTime =0;
local fightState= FightState.BeforeFightStart ; --参见战斗状态FightState
local fightActionLastTime = 1;
local oneFightLastTime =0

--战斗结算持续时间
local fightEndTime = 0

--等待时间
local waitingTime = 0;
local waitingTimeSecond = 0;
local waitingFirstFlag = false;
local beforeFightWaitTime = 0;
local beforeFightTotalWaitTime = 0;
local beforeFightWaitTimeSecond = 0;

local beforeFightBattleColdTime = 0
beforeFightBattleTotalColdTime = 0
--下场战斗默认等待时间
g_waitingFightTime = 10000;

local skillCfg = ConfigManager:getSkillCfg()
local htmlLabelList = {}
local htmlLabelSize = 50
local htmlLabelIndex = 0;

function ViewBattleSceneHelpter:initSceneNode(container)
	--初始化战斗场景
	BattleFightSceneHelper:initSceneNode(container)	
end


function ViewBattleSceneHelpter:getOneHtmlContent(contentStr)
	htmlLabelIndex = htmlLabelIndex %htmlLabelSize +1
	local content = htmlLabelList[htmlLabelIndex]
	local  covertedcontent =  tolua.cast(content,"CCHTMLLabel")
	--covertedcontent:initWithString(contentStr,CCSize(630,200),"Helvetica")
	covertedcontent:setString(contentStr)
	
	return covertedcontent
end

----------------------战斗日志相关开始--------------------

function ViewBattleSceneHelpter:AddCotentIntoScrollView(container,content)
	
	content:setPosition(ccp(0,currentSVPos));
	local tag = currentContentIndex % maxContentSize;
	--只维护maxContentSize大小的内容
	content:setTag(tag)
	local taggedChild = container.battleScrollViewRootNode:getChildByTag(tag)
	if taggedChild~=nil then
		container.battleScrollViewRootNode:removeChildByTag(tag,true)
	end
	currentSVPos = currentSVPos + content:getContentSize().height;
	container.battleScrollViewRootNode:addChild(content)
	container.battleScrollview:setContentSize(CCSize(640,currentSVPos))	;
	container.battleScrollview:setContentOffset(ccp(0,container.battleScrollview:getViewSize().height - currentSVPos ));
	currentContentIndex = currentContentIndex +1;
	
end


function ViewBattleSceneHelpter:RefreshWordContentInScrollView(container)
	
	container.battleScrollViewRootNode:removeAllChildren();
	local currentPos = 0
	local size = #wordList
	for i = size, 1,-1 do
		local oneContent  =   wordList[i]
		if oneContent~=nil then
			local content = CCHTMLLabel:createWithString(oneContent,CCSize(570,64),"Helvetica");
			content:setPosition(ccp(0,currentPos));
			content:setTag(i);
			currentPos = currentPos + content:getContentSize().height +GameConfig.FightLogSlotWidth;
			container.battleScrollViewRootNode:addChild(content)
		end
	end
	
	container.battleScrollview:setContentSize(CCSize(640,currentPos));
	local viewHeight = container.battleScrollview:getViewSize().height
	if currentPos< viewHeight then
		container.battleScrollview:setContentOffset(ccp(0,viewHeight - currentPos ));
	else
		container.battleScrollview:setContentOffset(ccp(0,0));
	end
	
end

function ViewBattleSceneHelpter:InsertWordContentInScrollView(container,strContent)
	local size = #wordList
	local currentPos = 0
	if size>htmlLabelSize then
		--超过最大限制，删除第一个元素，插入最后一个元素，顺移前面所有节点的位置和tag
		--step.1 删除第一个元素
		size = htmlLabelSize
		table.remove(wordList,1)
		container.battleScrollViewRootNode:removeChildByTag(1,true)
		--step.2 创建并插入最新的到节点中
		local content = CCHTMLLabel:createWithString(strContent,CCSize(570,64),"Helvetica");
		content:setPosition(ccp(0,currentPos));
		content:setTag(htmlLabelSize);
		local height = content:getContentSize().height + GameConfig.FightLogSlotWidth;
		currentPos = currentPos + height
		--step.3设置其他子节点的位置信息
		for i = size, 2, -1 do
			local taggedchild = container.battleScrollViewRootNode:getChildByTag(i);
			if taggedchild~= nil then
				taggedchild:setPosition(ccp(0,currentPos));
				local height = taggedchild:getContentSize().height + GameConfig.FightLogSlotWidth;
				currentPos = currentPos + height
			end
			
		end			
		--step.4 设置其他子节点的tag信息
		for i=2,size do
			local taggedchild = container.battleScrollViewRootNode:getChildByTag(i);
			if taggedchild~= nil then
				taggedchild:setTag(i-1);
			end
		end
		--step.5 最后addchild  新来的
		container.battleScrollViewRootNode:addChild(content)
	else
		--在限制范围内，只是插入最后一个，位移其他相关的
		for i = size, 1,-1 do
			local oneContent  =   wordList[i]
			if oneContent~=nil then
				local taggedchild = container.battleScrollViewRootNode:getChildByTag(i);
				if taggedchild~= nil then
					taggedchild:setPosition(ccp(0,currentPos));
					local height = taggedchild:getContentSize().height + GameConfig.FightLogSlotWidth;
					currentPos = currentPos + height
				else
					local content = CCHTMLLabel:createWithString(strContent,CCSize(570,64),"Helvetica");
					content:setPosition(ccp(0,currentPos));
					content:setTag(i);
					local height = content:getContentSize().height + GameConfig.FightLogSlotWidth;
					if size<=2 then
						height = height*2
					end
					currentPos = currentPos + height
					container.battleScrollViewRootNode:addChild(content)
				end
				
			end
		end
	end		
	container.battleScrollview:setContentSize(CCSize(640,currentPos));
	--if is in touch move, don't goto next line
	if chatPageInMove then
		return 
	end
	local viewHeight = container.battleScrollview:getViewSize().height
	if currentPos< viewHeight then
		container.battleScrollview:setContentOffset(ccp(0,viewHeight - currentPos ));
	else
		container.battleScrollview:setContentOffset(ccp(0,0));
	end		
end


function ViewBattleSceneHelpter:RefreshOrInsertLastCotent(container,strContent)
	local size = #wordList
	wordList[size] = strContent
	local taggedchild = container.battleScrollViewRootNode:getChildByTag(size);
	if taggedchild~= nil then
		taggedchild:setString(strContent);
	else
		ViewBattleSceneHelpter:AddCotentIntoSVByString(container,strContent)
	end
end


function ViewBattleSceneHelpter:AddEmptyLineCotentIntoSV(container)
	if FreeTypeConfig~=nil and FreeTypeConfig[22]~=nil and FreeTypeConfig[22].content ~= nil then
		ViewBattleSceneHelpter:AddCotentIntoSVByString(container,FreeTypeConfig[22].content)
	end
end

function ViewBattleSceneHelpter:AddCotentIntoSVByString(container,strContent)
	
	table.insert(wordList,strContent)
	ViewBattleSceneHelpter:InsertWordContentInScrollView(container,strContent)
	--[[local size = #wordList
	if size>htmlLabelSize then
		--当数量超过一定数量后，清空前10个
		for i = 1,htmlLabelSize/2 do
			table.remove(wordList,1)
		end
		ViewBattleSceneHelpter:RefreshWordContentInScrollView(container)
	else
		--否则，只是修改位置
		ViewBattleSceneHelpter:InsertWordContentInScrollView(container,strContent)
	end--]]
	
end


--加入掉落物品的战斗日志
function ViewBattleSceneHelpter:AddDropItemBattleLog(container,award)
	--如果有掉落
	if award.drop ~=nil then
		local drop = award.drop
		--装备掉落情况
		for i=1,#drop.equip  do
			local oneEquip = drop.equip[i]
			local equipStr = nil
			if oneEquip.quality == 1 then
				equipStr = FightLogConfig["WhiteEquip"].content
			elseif oneEquip.quality == 2 then
				equipStr = FightLogConfig["GreenEquip"].content
			elseif oneEquip.quality == 3 then
				equipStr = FightLogConfig["BlueEquip"].content
			elseif oneEquip.quality == 4 then
				equipStr = FightLogConfig["PurpleEquip"].content
			elseif oneEquip.quality == 5 then
				equipStr = FightLogConfig["OrangeEquip"].content
			end
			local newEquipStr = common:fill(equipStr,tostring(oneEquip.count))
			--table.insert(wordList,newEquipStr)
			ViewBattleSceneHelpter:AddCotentIntoSVByString(container,newEquipStr)
			if oneEquip.sellCoin>0 then
				local sellStr = common:fill(FightLogConfig["SellEquipGot"].content,tostring(oneEquip.sellCoin))
				--table.insert(wordList,sellStr)
				ViewBattleSceneHelpter:AddCotentIntoSVByString(container,sellStr)
			end
			
		end
		
		--详细装备掉落情况
		for i=1,#drop.detailEquip  do
			local oneEquip = drop.detailEquip[i]
			local equipStr = nil
			local rewardStr = ""
			local resInfo = ResManager:getResInfoByTypeAndId(40000, oneEquip.itemId, oneEquip.count);
			local equipCfg = ConfigManager.getEquipCfg();
			local equipLevel = equipCfg[oneEquip.itemId]["level"]
			rewardStr = rewardStr .."LV"..equipLevel.." ".. resInfo.name .."*"..oneEquip.count.." ";
			if resInfo.quality == 1 then
				equipStr = FightLogConfig["WhiteDetailEquip"].content
			elseif resInfo.quality == 2 then
				equipStr = FightLogConfig["GreenDetailEquip"].content
			elseif resInfo.quality == 3 then
				equipStr = FightLogConfig["BlueDetailEquip"].content
			elseif resInfo.quality == 4 then
				equipStr = FightLogConfig["PurpleDetailEquip"].content
			elseif resInfo.quality == 5 then
				equipStr = FightLogConfig["OrangeDetailEquip"].content
			end
			local newEquipStr = common:fill(equipStr,rewardStr)
			--table.insert(wordList,newEquipStr)
			ViewBattleSceneHelpter:AddCotentIntoSVByString(container,newEquipStr)
			if oneEquip.sellCoin>0 then
				local sellStr = common:fill(FightLogConfig["SellEquipGot"].content,tostring(oneEquip.sellCoin))
				--table.insert(wordList,sellStr)
				ViewBattleSceneHelpter:AddCotentIntoSVByString(container,sellStr)
			end
			
		end
		
		--物品掉落情况
		for i=1,#drop.item  do
			local oneEquip = drop.item[i]
			local equipStr = nil
			local rewardStr = ""
			local resInfo = ResManager:getResInfoByTypeAndId(oneEquip.itemType, oneEquip.itemId, oneEquip.itemCount);
			rewardStr = rewardStr .. resInfo.name .."*"..oneEquip.itemCount.." ";
			if resInfo.quality == 1 then
				equipStr = FightLogConfig["WhiteDetailEquip"].content
			elseif resInfo.quality == 2 then
				equipStr = FightLogConfig["GreenDetailEquip"].content
			elseif resInfo.quality == 3 then
				equipStr = FightLogConfig["BlueDetailEquip"].content
			elseif resInfo.quality == 4 then
				equipStr = FightLogConfig["PurpleDetailEquip"].content
			elseif resInfo.quality == 5 then
				equipStr = FightLogConfig["OrangeDetailEquip"].content
			end
			local newEquipStr = common:fill(equipStr,rewardStr)
			--table.insert(wordList,newEquipStr)
			ViewBattleSceneHelpter:AddCotentIntoSVByString(container,newEquipStr)
			
		end
		
		--宝箱掉落情况
		for i=1,#drop.treasure  do
			local oneBox = drop.treasure[i]
			if oneBox.state == Battle_pb.TREASURE_DISCARD  then
				--如果没有钥匙，逃走了
				local boxStr = nil
				if oneBox.itemId == 40001 then
					boxStr = FightLogConfig["FindTongBoxFail"].content
				elseif oneBox.itemId == 40002 then
					boxStr = FightLogConfig["FindYinBoxFail"].content
				elseif oneBox.itemId == 40003 then
					boxStr = FightLogConfig["FindJinBoxFail"].content
				else
					CCLuaLog("Error in box's itemId")
				end
				--table.insert(wordList,boxStr)
				local newBoxStr = common:fill(boxStr,tostring(oneBox.count))
				ViewBattleSceneHelpter:AddCotentIntoSVByString(container,newBoxStr)
			else
				--如果有钥匙
				local boxStr = nil
				if oneBox.itemId == 40001 then
					boxStr = FightLogConfig["FindTongBoxWin"].content
				elseif oneBox.itemId == 40002 then
					boxStr = FightLogConfig["FindYinBoxWin"].content
				elseif oneBox.itemId == 40003 then
					boxStr = FightLogConfig["FindJinBoxWin"].content
				else
					CCLuaLog("Error in box's itemId")
				end
				local itemStr = ""
				for i=1,#oneBox.item do
					local oneBoxItem = oneBox.item[i]
					local ResManagerForLua = require("ResManagerForLua")
					local resInfo = ResManagerForLua:getResInfoByTypeAndId(oneBoxItem.itemType, oneBoxItem.itemId, oneBoxItem.itemCount);
					itemStr = itemStr..resInfo.name.."*"..oneBoxItem.itemCount..""
				end
				local newBoxStr = common:fill(boxStr,itemStr)
				--table.insert(wordList,newBoxStr)
				ViewBattleSceneHelpter:AddCotentIntoSVByString(container,newBoxStr)
			end				
		end 
		
	end
end

function ViewBattleSceneHelpter:AddMingXiangSkillBattleLog(container,action)
	if ViewBattleSceneHelpter.FightChar[action.active.pos] == nil then return end
	local skillId = action.skillId
	local isOdd = common:numberIsOdd(action.active.pos)
	local freeTypeStr = isOdd and FightLogConfig["UseMingXiangEnemy"].content or FightLogConfig["UseMingXiang"].content
	local content = common:fill(freeTypeStr,action.round,ViewBattleSceneHelpter.FightChar[action.active.pos].name,action.active.effect.effectValue)	
	ViewBattleSceneHelpter:AddCotentIntoSVByString(container,content)
end


function ViewBattleSceneHelpter:AddHealSkillBattleLog(container,action)
	if ViewBattleSceneHelpter.FightChar[action.active.pos] == nil then return end	
	local skillId = action.skillId
	local skillName = skillCfg[skillId]["name"]
	for i=1,#action.passive do
		local passive = action.passive[i]
		local passivePos = passive.pos
		local isOdd = common:numberIsOdd(action.active.pos)
		local freeTypeStr = isOdd and FightLogConfig["HealSkillEnemy"].content or FightLogConfig["HealSkill"].content
		if ViewBattleSceneHelpter.FightChar[passivePos] == nil then return end
		local content = common:fill(freeTypeStr,action.round,ViewBattleSceneHelpter.FightChar[action.active.pos].name,
		ViewBattleSceneHelpter.FightChar[passivePos].name,passive.effect.effectValue)
		--CCLuaLog("@ViewBattleSceneHelpter:AddHealSkillBattleLog -- content"..content);
		ViewBattleSceneHelpter:AddCotentIntoSVByString(container,content)
	end
end

function ViewBattleSceneHelpter:AddSelfBuffSkillBattleLog(container,action)
	if ViewBattleSceneHelpter.FightChar[action.active.pos] == nil then return end	
	local skillId = action.skillId
	local skillName = skillCfg[skillId]["name"]
	local isOdd = common:numberIsOdd(action.active.pos)
	local freeTypeStr = isOdd and FightLogConfig["UseBuffSelfEnemy"].content or FightLogConfig["UseBuffSelf"].content
	local content = common:fill(freeTypeStr,action.round,ViewBattleSceneHelpter.FightChar[action.active.pos].name,skillName)
	--CCLuaLog("@ViewBattleSceneHelpter:AddSelfBuffSkillBattleLog -- content"..content);
	ViewBattleSceneHelpter:AddCotentIntoSVByString(container,content)
end

function ViewBattleSceneHelpter:AddSkipBattleBattleLog(container,action)
	if ViewBattleSceneHelpter.FightChar[action.active.pos] == nil then return end	
	local buffCfg = ConfigManager:getBuffCfg();
	local buffStr = buffCfg[action.buffId]["name"]
	local isOdd = common:numberIsOdd(action.active.pos)
	local freeTypeStr = isOdd and FightLogConfig["SkipBattleEnemy"].content or FightLogConfig["SkipBattle"].content
	local content = common:fill(freeTypeStr,action.round,ViewBattleSceneHelpter.FightChar[action.active.pos].name,buffStr)	
	ViewBattleSceneHelpter:AddCotentIntoSVByString(container,content)
end

function ViewBattleSceneHelpter:AddAvoidBuffLog(container,action)
	if ViewBattleSceneHelpter.FightChar[action.active.pos] == nil then return end	
	if ViewBattleSceneHelpter.FightChar[action.active.pos] == nil then return end	
	if #action.passive == 0 or action.passive[1].pos == nil or ViewBattleSceneHelpter.FightChar[action.passive[1].pos]==nil then return end
	if not action:HasField("avoidType") then return end 
	local buffCfg = ConfigManager:getBuffCfg();
	local buffStr = buffCfg[action.buffId]["name"]
	local isOdd = common:numberIsOdd(action.active.pos)
	local freeTypeStr = isOdd and FightLogConfig["AvoidBuffLogEnemy"].content or FightLogConfig["AvoidBuffLog"].content
	local content = ''
	local avoidReason = ''
	if action.avoidType == Battle_pb.AVOID_REASON_RAGE then
		avoidReason = common:getLanguageString("@AvoidRage")
	else
		avoidReason = common:getLanguageString("@AvoidGodEquip")
	end
	
	if isOdd then
		content = common:fill(freeTypeStr,action.round,avoidReason,ViewBattleSceneHelpter.FightChar[action.active.pos].name,buffStr)	
	else
		content = common:fill(freeTypeStr,action.round,buffStr,ViewBattleSceneHelpter.FightChar[action.passive[1].pos].name,avoidReason)	
	end
	
	ViewBattleSceneHelpter:AddCotentIntoSVByString(container,content)
end

function ViewBattleSceneHelpter:AddBuffForAllSelfBattleLog(container,action)
	if ViewBattleSceneHelpter.FightChar[action.active.pos] == nil then return end	
	local skillId = action.skillId
	local skillName = skillCfg[skillId]["name"]
	for i=1,#action.passive do
		local passive = action.passive[i]
		local passivePos = passive.pos
		if ViewBattleSceneHelpter.FightChar[passivePos] == nil then return end	
		local content = common:fill(FightLogConfig["UseBuffForOther"].content,action.round,ViewBattleSceneHelpter.FightChar[action.active.pos].name,
		ViewBattleSceneHelpter.FightChar[passivePos].name,skillName)
		CCLuaLog("@ViewBattleSceneHelpter:AddBuffForAllSelfBattleLog -- content"..content);
		ViewBattleSceneHelpter:AddCotentIntoSVByString(container,content)
	end
end

function ViewBattleSceneHelpter:AddRageDamageLog(container,action)
	if ViewBattleSceneHelpter.FightChar[action.active.pos] == nil then return end			
	local freeTypeStr = FightLogConfig["RageDamageLog"].content 
	local content = common:fill(freeTypeStr,action.round,ViewBattleSceneHelpter.FightChar[action.active.pos].name)
	ViewBattleSceneHelpter:AddCotentIntoSVByString(container,content)
end


--加入战斗日志
function ViewBattleSceneHelpter:AddActionIntoBattleLog(container,action)
	
	
	--[[// 行为类型
	enum actionType
	{
	// 回蓝
	ACTION_RECOVERMP = 1;
	// 回血
	ACTION_RECOVERHP = 2;
	// 攻击
	ACTION_SKILL = 3;
	}--]]
	
	if action.type == Battle_pb.ACTION_AVOID_BUFF then
		if action:HasField("buffId") then
		--如果处于眩晕、冻结状态，显示[#v1#]#v2#处于#v3#状态跳过战斗
			local buffId = action.buffId
			if buffId == 1 or  buffId == 2 or  buffId == 3 then
				ViewBattleSceneHelpter:AddAvoidBuffLog(container,action)
				return
			end
		end
	end
	
	
	if action.type == Battle_pb.ACTION_RAGE_DAMAGE then	
		ViewBattleSceneHelpter:AddRageDamageLog(container,action)
		return		
	end
	
	if action.type == Battle_pb.ACTION_BUFF_SKIP then
		if action:HasField("buffId") then
		--如果处于眩晕、冻结状态，显示[#v1#]#v2#处于#v3#状态跳过战斗
		local buffId = action.buffId
		if buffId == 1 or  buffId == 2 then
			ViewBattleSceneHelpter:AddSkipBattleBattleLog(container,action)
			return
		end
		end
	end
	
	if action.type == Battle_pb.ACTION_SKILL then
		if FightLogConfig~=nil  then
			
			local active = action.active
			local activePerson  = nil
			if active~=nil then
				activePerson = active.pos
			end
			
			
			if action:HasField("skillId") then
				local skillId = action.skillId
				
				--如果自己给自己加buff，显示[#v1#]#v2#使用#v3#
				local buffSkillTable = {3,10,15,17,20,30,23}
				if common:table_hasValue(buffSkillTable, skillId) then
					ViewBattleSceneHelpter:AddSelfBuffSkillBattleLog(container,action)
					return
				end
				
				--治疗，显示[#v1#]#v2#使用治疗为#v3#恢复了#v4#血
				if skillId == 2 then
					ViewBattleSceneHelpter:AddHealSkillBattleLog(container,action)
					return
				end
				
				--冥想，[#v1#]#v2#使用了冥想恢复了#v3#魔法值
				if skillId == 25 then
					ViewBattleSceneHelpter:AddMingXiangSkillBattleLog(container,action)
					return
				end
				
				--[#v1#]#v2#为#v3#使用#v4$  如战斗怒吼，某某为某某使用了战斗怒吼技能
				if skillId == 5 then
					ViewBattleSceneHelpter:AddBuffForAllSelfBattleLog(container,action)
					return
				end
				
			end
			
			--普通技能、普通攻击走以下通用文字说明
			local passives = action.passive
			local passivePerson  = nil
			if passives~=nil then
				for i=1,#passives do
					passive = passives[i]
					if passive~=nil then
						passivePerson = passive.pos
					end
					
					if ViewBattleSceneHelpter.FightChar[activePerson]~=nil and  ViewBattleSceneHelpter.FightChar[passivePerson]~=nil then
						
						--技能
						if action:HasField("skillId") then
							local skillId = action.skillId
							local skillName = skillCfg[skillId]["name"]
							local isOdd = common:numberIsOdd(activePerson)
							local skillFreeTypeStr = isOdd and FightLogConfig["AttactSkillEnemy"].content or FightLogConfig["AttactSkill"].content						
							local content = common:fill(skillFreeTypeStr,action.round,ViewBattleSceneHelpter.FightChar[activePerson].name,
							skillName,ViewBattleSceneHelpter.FightChar[passivePerson].name,passive.effect.effectValue)
							--CCLuaLog("@ViewBattleSceneHelpter:AddActionIntoBattleLog --skill-- content"..content);
							if passive.effect.effectType == Battle_pb.EFFECT_CRITS then
								content = content..FightLogConfig["EFFECT_CRITS"].content
							end
							if passive.effect.effectType == Battle_pb.EFFECT_MISS then
								content = content..FightLogConfig["EFFECT_MISS"].content
							end
							--[[EFFECT_NOCONTROL_A	<font color="#000000" face = "HelveticaBD32" >（#v1#被#v2#免疫）</font>
							EFFECT_NOCONTROL_B	<font color="#000000" face = "HelveticaBD32" >（#v1#免疫对方#v2#）</font>--]]
							ViewBattleSceneHelpter:AddCotentIntoSVByString(container,content)
							
							--如果是吸血技能，额外加入吸取了多少血量
							if skillId == 13 then
								local skillId = action.skillId
								local content = common:fill(FightLogConfig["XiXueSkill"].content,action.round,ViewBattleSceneHelpter.FightChar[action.active.pos].name,action.active.effect.effectValue)
								ViewBattleSceneHelpter:AddCotentIntoSVByString(container,content)
							end
						else
							--普通攻击
							local isOdd = common:numberIsOdd(activePerson)
							local attackFreeTypeStr = isOdd and FightLogConfig["AttactNormalEnemy"].content or FightLogConfig["AttactNormal"].content						
							local content = common:fill(attackFreeTypeStr,action.round,ViewBattleSceneHelpter.FightChar[activePerson].name,
							ViewBattleSceneHelpter.FightChar[passivePerson].name,passive.effect.effectValue)
							if passive.effect.effectType == Battle_pb.EFFECT_CRITS then
								content = content..FightLogConfig["EFFECT_CRITS"].content
							end
							if passive.effect.effectType == Battle_pb.EFFECT_MISS then
								content = content..FightLogConfig["EFFECT_MISS"].content
							end
							--CCLuaLog("@ViewBattleSceneHelpter:AddActionIntoBattleLog -- normal --content"..content);
							ViewBattleSceneHelpter:AddCotentIntoSVByString(container,content)
						end
						
					end
				end
			end
		end
	end
	
	
	if action.type == Battle_pb.ACTION_RECOVERMP then
		
	end
	
	
	
end
----------------------战斗日志相关结束--------------------


----------------------战斗状态相关开始--------------------


--进入状态之后的处理
function ViewBattleSceneHelpter:EnterState(container,state)
	fightState = state
	if fightState == FightState.FightRequired then
		-- 全由服务器发过来，不需要请求
		
		--应该是收到包之后进入战斗页面，否则，等待页面
		ViewBattleSceneHelpter:EnterState(container,FightState.BeforeFightStart)
		
		--ViewBattleSceneHelpter:EnterState(container,FightState.Fighting)
		beforeFightBattleColdTime = 0
		beforeFightBattleTotalColdTime = 0;
	elseif fightState == FightState.BeforeFightStart then
		--如果有冷却时间
		if BattleDataHelper.hasBattleColdTime then
			local strWaitingSecond = tostring(  math.floor((BattleDataHelper.nextBattleLeftTime) / 1000))
			if FreeTypeConfig~=nil and FreeTypeConfig[6]~=nil and FreeTypeConfig[6].content ~= nil then
				local content = GameMaths:replaceStringWithCharacterAll(FreeTypeConfig[6].content,"#v1#",strWaitingSecond)
				ViewBattleSceneHelpter:AddCotentIntoSVByString(container,content)
			end
		end
	elseif fightState == FightState.Fighting then
		--初始化战斗序列
		BattleDataHelper.hasBattleColdTime = false
		local battleData =  BattleDataHelper.BattleDataSequence.battleData
		if battleData~=nil then
			local actions = battleData.actions;
			if actions~=nil then
				local actionSize = #actions
				for i=1,actionSize do
					table.insert(fightList,actions[i])
				end
			end
		end
	elseif fightState == FightState.FightEnd then
		--战斗结算
		fightEndTime = 0
		ViewBattleSceneHelpter:doFightEnd(container);
		
	elseif fightState == FightState.Waiting then
		--CCLuaLog("@ViewBattleSceneHelpter:UpdateState -- one fight list time is "..tostring(oneFightLastTime))
		waitingTime  = 0
		waitingTimeSecond = 0
		BattleFightSceneHelper:countDownAni(container)
		BattleFightSceneHelper:refreshCountDownNum(container,tostring(g_waitingFightTime - 1))
		BattleFightSceneHelper:showFindEnemyAni(container)
	end
end



--状态中刷新情况
function ViewBattleSceneHelpter:UpdateState(container)
	local dt = GamePrecedure:getInstance():getFrameTime() * 1000;
	oneFightLastTime =oneFightLastTime + dt
	
	if m_samePlayerRelogin == true then
		ViewBattleSceneHelpter:smallExitReset(container,true)
		m_samePlayerRelogin = false
	end
	
	if fightState == FightState.FightRequired then
		--TODO
	elseif fightState == FightState.BeforeFightStart then
		--战斗准备
		ViewBattleSceneHelpter:doBeforeFight(container,dt)
	elseif fightState == FightState.Fighting then
		--战斗中
		ViewBattleSceneHelpter:doFight(container,dt);
	elseif fightState == FightState.FightEnd then
		--战斗结算
		fightEndTime = fightEndTime + dt
		--如果在结算页面持续了2秒，自动进入等待状态，防止卡死状态，另一个进入等待状态是在播放完win or lose 动画后
		if fightEndTime > 2000 then
			ViewBattleSceneHelpter:EnterState(container,FightState.Waiting)
			fightEndTime = 0
		end
	elseif fightState == FightState.Waiting then
		--战斗等待
		ViewBattleSceneHelpter:doWaitingFight(container,dt)
		
	end
	
end

function ViewBattleSceneHelpter:onAnimationDone(container)
	
end

function ViewBattleSceneHelpter:doWaitingFight(container,dt)
	waitingTime  = waitingTime + dt;
	waitingTimeSecond = waitingTimeSecond + dt
	--等待N秒钟,根据上次发过来的数据，默认等待10秒
	local maxWaitTime = g_waitingFightTime * 1000
	if waitingTime > maxWaitTime then
		waitingTime = 0;
		waitingFirstFlag = false;
		local content = GameMaths:replaceStringWithCharacterAll(FreeTypeConfig[5].content,"#v1#","0")
		ViewBattleSceneHelpter:RefreshOrInsertLastCotent(container,content)
		--战斗冷却结束后，请求战斗
		ViewBattleSceneHelpter:EnterState(container,FightState.FightRequired)
		
	else
		--每秒刷新等待文字
		if waitingTimeSecond>1000 then
			--添加一条信息到content中
			if FreeTypeConfig~=nil and FreeTypeConfig[5]~=nil and FreeTypeConfig[5].content ~= nil then
				local strWaitingSecond = tostring(  math.floor((maxWaitTime-waitingTime) / 1000))
				if waitingFirstFlag == false then
					local content = GameMaths:replaceStringWithCharacterAll(FreeTypeConfig[5].content,"#v1#",strWaitingSecond)
					ViewBattleSceneHelpter:AddCotentIntoSVByString(container,content)
					BattleFightSceneHelper:refreshCountDownNum(container,strWaitingSecond)
					waitingFirstFlag = true;
				else
					local content = GameMaths:replaceStringWithCharacterAll(FreeTypeConfig[5].content,"#v1#",strWaitingSecond)
					ViewBattleSceneHelpter:RefreshOrInsertLastCotent(container,content)
					BattleFightSceneHelper:refreshCountDownNum(container,strWaitingSecond)
				end
				
			end
			waitingTimeSecond = 0;
		end
	end
end

function ViewBattleSceneHelpter:doBeforeFight(container,dt)
	--step.1 初始化战斗场景东西
	
	local initFlag = BattleDataHelper:onInitBattleData(container);
	if initFlag == true then
		oneFightLastTime =0
		beforeFightWaitTime = 0;
		beforeFightWaitTimeSecond =0;
		beforeFightTotalWaitTime =0
		beforeFightBattleColdTime = 0
		beforeFightBattleTotalColdTime = 0
		BattleDataHelper.hasBattleColdTime = false
		local battleData =  BattleDataHelper.BattleDataSequence.battleData
		if battleData~=nil then
			--CCLuaLog("@ViewBattleSceneHelpter:doBeforeFight -- "..tostring(BattleDataHelper.BattleDataSequence));
			g_waitingFightTime = BattleDataHelper.BattleDataSequence.waitTime
			
			--显示旌旗boss arena vs
			BattleFightSceneHelper:showWhichBattleFlag(container,BattleDataHelper.BattleDataSequence.battleType)
			
			local chars = battleData.character;
			if chars~=nil then
				ViewBattleSceneHelpter:doPrepareCharacter(container,chars)
				--step.2 播放
				BattleFightSceneHelper:startBattleAni(container);
				ViewBattleSceneHelpter:EnterState(container,FightState.Fighting)
			end
			--清空状态
			nextChanllengeName = nil
			BattleFightSceneHelper:resetNextPromptAni(container)
			BattleMainPageHelper:refreshNextFightName(container)
		end
	else
		
		if BattleDataHelper.hasBattleColdTime then
			beforeFightBattleColdTime = beforeFightBattleColdTime + dt
			beforeFightBattleTotalColdTime = beforeFightBattleTotalColdTime + dt
			local leftTime = BattleDataHelper.nextBattleLeftTime-beforeFightBattleTotalColdTime
			--大于十秒，播战斗冷却，10秒以内播正在休息
			leftTime = math.max(0,leftTime)
			if leftTime>10000 then
				leftTime = leftTime - 10
				leftTime = math.max(0,leftTime)
				local strWaitingSecond = tostring(  math.floor((leftTime) / 1000))
				if beforeFightBattleColdTime>1000 then
					if FreeTypeConfig~=nil and FreeTypeConfig[6]~=nil and FreeTypeConfig[6].content ~= nil then
						local content = GameMaths:replaceStringWithCharacterAll(FreeTypeConfig[6].content,"#v1#",strWaitingSecond)
						ViewBattleSceneHelpter:RefreshOrInsertLastCotent(container,content)
					end
					beforeFightBattleColdTime = 0
				end
			else
				leftTime = math.max(0,leftTime)
				local strWaitingSecond = tostring(  math.floor((leftTime) / 1000))
				if beforeFightBattleColdTime>1000 then
					if FreeTypeConfig~=nil and FreeTypeConfig[5]~=nil and FreeTypeConfig[5].content ~= nil then
						local content = GameMaths:replaceStringWithCharacterAll(FreeTypeConfig[5].content,"#v1#",strWaitingSecond)
						ViewBattleSceneHelpter:RefreshOrInsertLastCotent(container,content)
					end
					beforeFightBattleColdTime = 0
				end
			end
			
		else
			--如果没有冷却时间，播搜索地图怪，然后接网络异常
			beforeFightWaitTime = beforeFightWaitTime + dt
			beforeFightTotalWaitTime = beforeFightTotalWaitTime + dt
			beforeFightWaitTimeSecond = beforeFightWaitTimeSecond + dt
			if beforeFightWaitTime>10000 then
				
				if beforeFightTotalWaitTime<10000 then
					if FreeTypeConfig~=nil and FreeTypeConfig[20]~=nil and FreeTypeConfig[20].content ~= nil then
						local content = GameMaths:replaceStringWithCharacterAll(FreeTypeConfig[20].content,"#v1#","10")
						ViewBattleSceneHelpter:AddCotentIntoSVByString(container,content)
					end
					
				else
					if FreeTypeConfig~=nil and FreeTypeConfig[8]~=nil and FreeTypeConfig[8].content ~= nil then
						local content = GameMaths:replaceStringWithCharacterAll(FreeTypeConfig[8].content,"#v1#","10")
						ViewBattleSceneHelpter:AddCotentIntoSVByString(container,content)
					end
				end
				
				beforeFightWaitTime = 0
			else
				if beforeFightWaitTimeSecond>1000 then
					local strWaitingSecond = tostring(  math.floor((10000-beforeFightWaitTime) / 1000))
					if beforeFightTotalWaitTime<10000 then
						if FreeTypeConfig~=nil and FreeTypeConfig[20]~=nil and FreeTypeConfig[20].content ~= nil then
							local content = GameMaths:replaceStringWithCharacterAll(FreeTypeConfig[20].content,"#v1#",strWaitingSecond)
							ViewBattleSceneHelpter:RefreshOrInsertLastCotent(container,content)
						end	
						
					else
						if FreeTypeConfig~=nil and FreeTypeConfig[8]~=nil and FreeTypeConfig[8].content ~= nil then
							local content = GameMaths:replaceStringWithCharacterAll(FreeTypeConfig[8].content,"#v1#",strWaitingSecond)
							ViewBattleSceneHelpter:RefreshOrInsertLastCotent(container,content)
						end
					end
					beforeFightWaitTimeSecond = 0
				end					
			end
		end				
	end		
end



function ViewBattleSceneHelpter:doPrepareCharacter(container,chars)
	--清空角色状态
	BattleFightSceneHelper:resetAllCharacter(container)
	if chars ~=nil then
		local charSize = #chars;
		for i=1,charSize do
			local oneCha = chars[i];
			if oneCha~=nil and oneCha.pos ~=nil then
				--保存一份战斗角色数据，key为pos,value为cha
				ViewBattleSceneHelpter.FightChar[oneCha.pos] = oneCha
				BattleFightSceneHelper:onPrepareCharacter(container,oneCha)
				--你遇到了lv.#v1# <font  color="#ff00ff" size ="100">#v2#</font> HP:#v3#
				if FreeTypeConfig~=nil and FreeTypeConfig[21]~=nil and FreeTypeConfig[21].content ~= nil and (oneCha.pos % 2) ==1 then
					local content = GameMaths:replaceStringWithCharacterAll(FreeTypeConfig[21].content,"#v1#",oneCha.level)
					content = GameMaths:replaceStringWithCharacterAll(content,"#v2#",oneCha.name)
					content = GameMaths:replaceStringWithCharacterAll(content,"#v3#",oneCha.hp)
					ViewBattleSceneHelpter:AddCotentIntoSVByString(container,content)
				end
			end
			
		end
		ViewBattleSceneHelpter:AddEmptyLineCotentIntoSV(container)
	end		
end

function ViewBattleSceneHelpter:doFight(container,dt)
	fightingTime = fightingTime + dt;
	--根据时间判断延迟多少秒
	if fightingTime> fightActionLastTime then
		local listCount = table.getn(fightList)
		if listCount>0 then
			--获取第一个队列中的元素,加入战斗序列，并删除第一个
			
			local headAction = table.remove(fightList,1)
						
			--CCLuaLog("ViewBattleSceneHelpter:doFight.."..tostring(headAction));
			--CCLuaLog("ViewBattleSceneHelpter:doFight -- action type is "..headAction.type.."round"..headAction.round)
			--加入战斗日志
			ViewBattleSceneHelpter:AddActionIntoBattleLog(container,headAction)			
			--显示战斗场景
			--攻击者			
			BattleFightSceneHelper:onShowActionEffect(headAction)						
			fightActionLastTime = headAction.costTime			
		else
			--CCLuaLog("@ViewBattleSceneHelpter:UpdateState -- fight action last time is  "..tostring(oneFightLastTime))
			ViewBattleSceneHelpter:EnterState(container,FightState.FightEnd);			
		end
		fightingTime = 0;
	end		
end

function ViewBattleSceneHelpter:doFightEnd(container)
	
	--判断战斗胜利或者失败
	if BattleDataHelper.BattleDataSequence.fightResult == 0 then
		BattleFightSceneHelper:loseAni()
		if FightLogConfig["Lose"]~=nil and FightLogConfig["Lose"].content ~= nil then
			ViewBattleSceneHelpter:AddCotentIntoSVByString(container,FightLogConfig["Lose"].content)
		end
	else
		BattleFightSceneHelper:winAni()
		if FightLogConfig["Victory"]~=nil and FightLogConfig["Victory"].content ~= nil then
			ViewBattleSceneHelpter:AddCotentIntoSVByString(container,FightLogConfig["Victory"].content)
		end
		if BattleDataHelper.BattleDataSequence.award~=nil then
			if FightLogConfig["GotCoin"]~=nil and BattleDataHelper.BattleDataSequence.award.coin>0 then
				local content = common:fill(FightLogConfig["GotCoin"].content,tostring(BattleDataHelper.BattleDataSequence.award.coin))
				ViewBattleSceneHelpter:AddCotentIntoSVByString(container,content)
				
			end
			
			if FightLogConfig["GotExp"]~=nil and BattleDataHelper.BattleDataSequence.award.exp>0 then
				local content = common:fill(FightLogConfig["GotExp"].content,tostring(BattleDataHelper.BattleDataSequence.award.exp))
				ViewBattleSceneHelpter:AddCotentIntoSVByString(container,content)
				
			end
			--战斗掉落物品结算
			ViewBattleSceneHelpter:AddDropItemBattleLog(container,BattleDataHelper.BattleDataSequence.award)
		end
	end	
	--当前不在战斗页面，飘战斗结果的窗口
	if MainFrame:getInstance():getCurShowPageName() ~="" then
		if BattleDataHelper.BattleDataSequence.battleType == Battle_pb.BATTLE_PVE_BOSS or BattleDataHelper.BattleDataSequence.battleType == Battle_pb.BATTLE_PVP_ARENA then
			RegisterLuaPage("BattleResultPopPage")
			BattleResultPopPage_setBossResult(BattleDataHelper.BattleDataSequence)
			PageManager.pushPage("BattleResultPopPage")
		end		
	end	
	
	if BattleDataHelper.BattleDataSequence.battleType == Battle_pb.BATTLE_PVE_ELITE_BOSS  then
		if BattleDataHelper.BattleDataSequence.fightResult == 0 then	
			RegisterLuaPage("EliteMapLosePopup")
			if BattleDataHelper.BattleDataSequence:HasField("mapId") then
				EliteMapLosePopup_setMapId(BattleDataHelper.BattleDataSequence.mapId)	
			end				
			PageManager.pushPage("EliteMapLosePopup")
		else
			RegisterLuaPage("EliteMapWinPopup")
			local mapId = 0
			if BattleDataHelper.BattleDataSequence:HasField("mapId") then
				mapId = BattleDataHelper.BattleDataSequence.mapId
			end
			EliteMapWinPopup_setAward(BattleDataHelper.BattleDataSequence.award,mapId)
			PageManager.pushPage("EliteMapWinPopup")
		end
	end
	
	if BattleDataHelper.BattleDataSequence.battleType == Battle_pb.BATTLE_PVE_BOSS and BattleDataHelper.BattleDataSequence.fightResult == 0 then
			NewbieGuideManager.guide(GameConfig.NewbieGuide.FirstFailBoss)
	end	
	
    if BattleDataHelper.BattleDataSequence:HasField("award") and BattleDataHelper.BattleDataSequence.award:HasField("coin") and BattleDataHelper.BattleDataSequence.award:HasField("exp") then
		local MainScenePage = require("MainScenePage")
		MainScenePage.setAddAwardData(BattleDataHelper.BattleDataSequence.award.coin,BattleDataHelper.BattleDataSequence.award.exp)
		PageManager.refreshPage("MainScenePage","runAddCoinAni")
	end
	
	--换1行
	ViewBattleSceneHelpter:AddEmptyLineCotentIntoSV(container)	
	ViewBattleSceneHelpter:AddEmptyLineCotentIntoSV(container)		
end

function ViewBattleSceneHelpter:showResultDirectly(container,hasWelcomeLog)
	ViewBattleSceneHelpter:AddEmptyLineCotentIntoSV(container)
	
	if BattleDataHelper.BattleDataSequence ~= nil and fightState ~= FightState.Waiting and fightState ~= FightState.FightEnd then
		if BattleDataHelper.BattleDataSequence.fightResult == 0 then	
			if FightLogConfig["Lose"]~=nil and FightLogConfig["Lose"].content ~= nil then
				ViewBattleSceneHelpter:AddCotentIntoSVByString(container,FightLogConfig["Lose"].content)
			end
		else		
			if FightLogConfig["Victory"]~=nil and FightLogConfig["Victory"].content ~= nil then
				ViewBattleSceneHelpter:AddCotentIntoSVByString(container,FightLogConfig["Victory"].content)
			end
			if BattleDataHelper.BattleDataSequence.award~=nil then
				if FightLogConfig["GotCoin"]~=nil and BattleDataHelper.BattleDataSequence.award.coin>0 then
					local content = common:fill(FightLogConfig["GotCoin"].content,tostring(BattleDataHelper.BattleDataSequence.award.coin))
					ViewBattleSceneHelpter:AddCotentIntoSVByString(container,content)
					
				end
				
				if FightLogConfig["GotExp"]~=nil and BattleDataHelper.BattleDataSequence.award.exp>0 then
					local content = common:fill(FightLogConfig["GotExp"].content,tostring(BattleDataHelper.BattleDataSequence.award.exp))
					ViewBattleSceneHelpter:AddCotentIntoSVByString(container,content)
					
				end
				--战斗掉落物品结算
				ViewBattleSceneHelpter:AddDropItemBattleLog(container,BattleDataHelper.BattleDataSequence.award)
			end
		end	
	end		
	if hasWelcomeLog  ~= nil and hasWelcomeLog == true then
		ViewBattleSceneHelpter:AddCotentIntoSVByString(container,FightLogConfig["WelcomeBack"].content)
	end
	
	ViewBattleSceneHelpter:AddEmptyLineCotentIntoSV(container)	
end

function ViewBattleSceneHelpter:smallExitReset(container,hasWelcomeLog)
	--如果是小退，而且是playerId 没有改变，当前如果有战斗，直接切战斗结果，然后进入等待状态,当前如果没有战斗，直接进入战斗冷却	
	BattleFightSceneHelper:resetAllCharacter(container, true)
	fightingTime = 0;
	fightList = {};
	ViewBattleSceneHelpter:showResultDirectly(container,hasWelcomeLog)		
	ViewBattleSceneHelpter:EnterState(container,FightState.FightRequired)		
end

function ViewBattleSceneHelpter:fightBossOrArena(container)
	--如果是小退，而且是playerId 没有改变，当前如果有战斗，直接切战斗结果，然后进入等待状态,当前如果没有战斗，直接进入战斗冷却	
	BattleFightSceneHelper:resetAllCharacter(container, true)
	fightingTime = 0;
	fightList = {};
	ViewBattleSceneHelpter:showResultDirectly(container,false)		
	ViewBattleSceneHelpter:EnterState(container,FightState.Waiting)		
end

function ViewBattleSceneHelpter_reloginPlayerUnchange()
	m_samePlayerRelogin  = true
end

function ViewBattleSceneHelpter:resetData()
	for i=1,#wordList do
		table.remove(wordList)
	end
	ViewBattleSceneHelpter.FightChar={}
end


return ViewBattleSceneHelpter;