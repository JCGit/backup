local HP_pb = require "HP_pb"
local Battle_pb = require "Battle_pb"
local Const_pb = require "Const_pb"
local UserInfo = require("UserInfo");
--encoding UTF-8 without BOM
--------------------------------------------------------------------------------
--  FILE:			BattleFightSceneHelper.lua
--  DESCRIPTION:	BattleFightSceneHelper 
--
--
--  AUTHOR:		Zhen Hui
--  CREATED:	2014/8/3
--  MODIFY2:
--
--  MODIFY1:
--
--------------------------------------------------------------------------------

local BattleFightSceneHelper = {}

--战斗场景数据

local mainNode =  {
chaNode = {},
personHitOrBufCCB = {},
normalNumCCB = {},
critsNumCCB = {},
dodgeNumCCB = {},
healNumCCB = {},
gainMPNumCCB = {},
chaCCB = {}
}

local mercenaryNode = {
chaNode = {},
personHitOrBufCCB = {},
normalNumCCB = {},
critsNumCCB = {},
dodgeNumCCB = {},
healNumCCB = {},
gainMPNumCCB = {},
chaCCB = {}
}


local enemyPos1 = {
chaNode = {},
personHitOrBufCCB = {},
normalNumCCB = {},
critsNumCCB = {},
dodgeNumCCB = {},
healNumCCB = {},
gainMPNumCCB = {},
chaCCB = {}
}

local enemyPos3 = {
chaNode = {},
personHitOrBufCCB = {},
normalNumCCB = {},
critsNumCCB = {},
dodgeNumCCB = {},
healNumCCB = {},
gainMPNumCCB = {},
chaCCB = {}
}

local enemyPos5 = {
chaNode = {},
personHitOrBufCCB = {},
normalNumCCB = {},
critsNumCCB = {},
dodgeNumCCB = {},
healNumCCB = {},
gainMPNumCCB = {},
chaCCB = {}
}


local battleHintNode  = {}
local battleHintCCB = {}
 
local nextPromptHint = {}

local roleCfg = {}
local buffCfg = ConfigManager:getBuffCfg();
local monsterCfg = ConfigManager:getMonsterCfg()
local NodeHelper = require("NodeHelper");


function BattleFightSceneHelper:initSceneNode(container)
	--战斗位置节点
	mainNode.chaNode = container:getVarNode("mLeadNode");
	mercenaryNode.chaNode = container:getVarNode("mMercenaryNode");
	enemyPos1.chaNode = container:getVarNode("mEnemyNode1");
	enemyPos3.chaNode = container:getVarNode("mEnemyNode2");
	enemyPos5.chaNode = container:getVarNode("mEnemyNode3");
	
	mainNode.chaNode:removeAllChildren();
	mercenaryNode.chaNode:removeAllChildren();
	enemyPos1.chaNode:removeAllChildren();
	enemyPos3.chaNode:removeAllChildren();
	enemyPos5.chaNode:removeAllChildren();
	

	--组装节点及CCB	
	mainNode.chaCCB = ScriptContentBase:create("BattleLead.ccbi");
	mainNode.chaNode:addChild(mainNode.chaCCB);
	mainNode.chaNode:setVisible(false)
	BattleFightSceneHelper:assembleAniNodeInPersonNode(mainNode)
	mainNode.chaCCB:release();	

	mercenaryNode.chaCCB = ScriptContentBase:create("BattleLeadMercenary.ccbi");
	mercenaryNode.chaNode:addChild(mercenaryNode.chaCCB);
	mercenaryNode.chaNode:setVisible(false)
	BattleFightSceneHelper:assembleAniNodeInPersonNode(mercenaryNode)
	mercenaryNode.chaCCB:release();	

--[[	enemyPos1.chaCCB = ScriptContentBase:create("BattleCommonEnemy.ccbi");
	enemyPos1.chaNode:addChild(enemyPos1.chaCCB);--]]
	enemyPos1.chaNode:setVisible(false)	
	--BattleFightSceneHelper:assembleAniNodeInPersonNode(enemyPos1)
	--enemyPos1.chaCCB:release();
		

	enemyPos3.chaCCB = ScriptContentBase:create("BattleCommonEnemy.ccbi");
	enemyPos3.chaNode:addChild(enemyPos3.chaCCB);
	enemyPos3.chaNode:setVisible(false)
	BattleFightSceneHelper:assembleAniNodeInPersonNode(enemyPos3)	
	enemyPos3.chaCCB:release();	

	enemyPos5.chaCCB = ScriptContentBase:create("BattleCommonEnemy.ccbi");
	enemyPos5.chaNode:addChild(enemyPos5.chaCCB);		
	enemyPos5.chaNode:setVisible(false)
	BattleFightSceneHelper:assembleAniNodeInPersonNode(enemyPos5)	
	enemyPos5.chaCCB:release();	
	
	--战斗开始，结束动画、倒计时动画
	battleHintNode = container:getVarNode("mHintAniNode");
	battleHintNode:removeAllChildren();
	battleHintCCB = CCBScriptContainer:create("BattleHintAni");
	battleHintCCB:load();
	--battleHintCCB:runAnimation("start");
	battleHintNode:addChild(battleHintCCB);
	battleHintCCB:release();
	
	--下一关挑战boss 下一关挑战竞技	正在搜索地图
	local nextBattleHintNode = container:getVarNode("mNextPromptAniNode");
	
	nextPromptHint = ScriptContentBase:create("NextPromptAni.ccbi");	
	nextBattleHintNode:removeAllChildren();
	nextBattleHintNode:addChild(nextPromptHint);
	nextPromptHint:release();
		
	
	roleCfg = ConfigManager.getRoleCfg();
	--CCBScriptContainer:getVarNode
end

-----------------下一关挑战动画---------------
function BattleFightSceneHelper:resetNextPromptAni(container)
	nextPromptHint:runAnimation("Default Timeline");
end

function BattleFightSceneHelper:showFindEnemyAni(container)
	nextPromptHint:runAnimation("VS");
end

function BattleFightSceneHelper:ShowNextFightBossAni(container)
	nextPromptHint:runAnimation("BOOS");
end

function BattleFightSceneHelper:ShowNextFightArenaAni(container)
	nextPromptHint:runAnimation("Arena");
end

-----------------战斗动画---------------
function BattleFightSceneHelper:resetHintAni(container)
	battleHintCCB:runAnimation("Default Timeline");
end

function BattleFightSceneHelper:startBattleAni(container)
	battleHintCCB:runAnimation("start");
end

function BattleFightSceneHelper:winAni(container)
	battleHintCCB:runAnimation("win");
end

function BattleFightSceneHelper:loseAni(container)
	battleHintCCB:runAnimation("lose");
end

function BattleFightSceneHelper:countDownAni(container)
	battleHintCCB:runAnimation("Countdown");
end

function BattleFightSceneHelper:refreshCountDownNum(container,str)
	local countDownLabel = battleHintCCB:getVarLabelBMFont("mCoutdown");
	countDownLabel:setString(str)
	
end

function BattleFightSceneHelper:showWhichBattleFlag(container,flag)
	local boss = container:getVarSprite("mFontBOSS")
	local arena = container:getVarSprite("mFontArena")
	local vs = container:getVarSprite("mFontVS")
	boss:setVisible(false)
	arena:setVisible(false)
	vs:setVisible(false)
	if flag == Battle_pb.BATTLE_PVE_MONSTER then
		vs:setVisible(true)
	elseif flag == Battle_pb.BATTLE_PVE_BOSS or flag == Battle_pb.BATTLE_PVE_ELITE_BOSS	then
		boss:setVisible(true)
	else 
		arena:setVisible(true)	
	end
	
end


function BattleFightSceneHelper:onShowActionEffect(action)
	
	local active = action.active
				
	
	local hitFlag = false
	local buffFlag = false;
	--伤害者,有可能为多个
	local passive = action.passive
	if passive~=nil then
		local size = #passive		
		for i =1,size do				
			BattleFightSceneHelper:onShowPersonEffect(passive[i],true)					
		end
		--如果没有被击方，则personEffect为攻击者闪光
		if size ==0 then
			hitFlag = true
		end
	else
		hitFlag = true
	end	
	
	--如果action.type 为回蓝或者回血，不显示道光
	if action.type ==Battle_pb.ACTION_RECOVERMP or action.type ==Battle_pb.ACTION_RECOVERHP  then
		hitFlag = false
		buffFlag = true;
	end
	
	if active~=nil then
		BattleFightSceneHelper:onShowPersonEffect(active,hitFlag, buffFlag)
	end
end


function BattleFightSceneHelper:onShowPersonEffect(person,hitAni, buffAni)
	--飘字
	local personNode = {}
	--CCLuaLog("@BattleFightSceneHelper:onShowPersonEffect -- "..tostring(person));
	if person.pos == nil then
		CCLuaLog("Error in BattleFightSceneHelper:onShowPersonEffect, pos not right");		
		return 
	end
	if person.pos  == 0 then
		personNode = mainNode		
	elseif	person.pos== 1 then
		personNode = enemyPos1	
	elseif	person.pos== 2 then
		personNode = mercenaryNode
	elseif	person.pos== 3 then
		personNode = enemyPos3
	elseif	person.pos== 5 then	
		personNode = enemyPos5	
	else
		CCLuaLog("Error in BattleFightSceneHelper:onShowPersonEffect, pos not right"..person.pos);
		return
	end		
	local action = person.effect
	if action~=nil and person:HasField("effect") then
		BattleFightSceneHelper:showActionNumber(personNode,action.effectValue,action.effectType)
	end
	
	
	--显示被击效果
	if hitAni then
		BattleFightSceneHelper:showHitAni(personNode)
	end	
	
	--刷新人物血量和buff效果
	local BattleFightHelper = require("BattleFightHelper")
	local char = BattleFightHelper.FightChar[person.pos]
	if char == nil then
		CCLuaLog("@BattleFightSceneHelper:onShowPersonEffect -- char is null");
		return 
	end
	
	--buff动画
	if buffAni then
		BattleFightSceneHelper:showBUFFAni(personNode)
	end
	
	--buff文字
	local buffFreeType = FightLogConfig["AddBuff"]
	local debuffFreeType = FightLogConfig["DeBuff"]
	if buffFreeType ==nil then
		CCLuaLog("buff free type font not found");
	end
	local buffInfo = person.buffInfo
	local finalBuffStr = ""
	if buffInfo~=nil then
		local buffStrAdd = ""
		local buffStrMinu = ""
		local buffSize = #buffInfo
		for i=1,buffSize do
			local buffId = buffInfo[i].buffId
			if buffCfg[buffId]["isAdd"] == 1 then
				buffStrAdd = buffStrAdd.." "..buffCfg[buffId]["name"]..buffInfo[i].buffRound.." "
			else 
				buffStrMinu = buffStrMinu.." "..buffCfg[buffId]["name"]..buffInfo[i].buffRound.." "
			end
		end
		if buffSize>0 then
			local addStr = ""
			local deStr = ""
			if buffStrAdd ~= "" then
				addStr = common:fill(buffFreeType.content,buffStrAdd)
			end
			if buffStrMinu ~= "" then
				deStr = common:fill(debuffFreeType.content,buffStrMinu)
			end

			finalBuffStr = addStr..deStr
		end
		
	else
		finalBuffStr = ""
	end
	
	--如果已经死亡，清空buff状态
	if person.status ~= Battle_pb.PERSON_FIGHT	then
		finalBuffStr = ""
	end
		
	--CCLuaLog("finalBuffStr".. finalBuffStr);
	local buffNode  = personNode.chaCCB:getVarLabelBMFont("mBuff")
	if buffNode ~= nil then					
			buffNode:getParent():removeChildByTag(10011,true)
			local label = CCHTMLLabel:createWithString(finalBuffStr,CCSizeMake(500,100),"Helvetica")
			
			local anchorpoint = buffNode:getAnchorPoint()
			label:setPosition(ccp(buffNode:getPositionX(),buffNode:getPositionY()))
			label:setAnchorPoint(ccp(anchorpoint.x,anchorpoint.y))
			label:setTag(10011);
			buffNode:getParent():addChild(label)			
	end	
	local lb2Str = {		
	mHP						= person.hp.."/"..char.hp,
	mMp						= person.mp.."/"..char.mp
	};
	NodeHelper:setStringForLabel(personNode.chaCCB, lb2Str);	
	
	--血量、mp 九宫
	local hpBar = personNode.chaCCB:getVarScale9Sprite("mHpBar")
	local hpPercent = person.hp/char.hp
	if hpBar~=nil and hpPercent>=0 then
		hpBar:setScaleX(math.min(hpPercent, 1.0))
	end			
	local mpBar = personNode.chaCCB:getVarScale9Sprite("mMpBar")
	local mpPercent = person.mp/char.mp
	if mpBar~=nil and mpPercent>=0 then
		mpBar:setScaleX(math.min(mpPercent, 1.0))
	end		
	
	--死亡、逃跑动画
	if person.status == Battle_pb.PERSON_DEAD then
		BattleFightSceneHelper:showDieAni(personNode)
	elseif person.status == Battle_pb.PERSON_ESCAPE then 
		BattleFightSceneHelper:showEscapeAni(personNode)
	end
	
	
end


--主角位置刷新
function BattleFightSceneHelper:onInitWithPos(container,charData)
	local char = charData
	if char == nil then
		assert(false,"@BattleFightSceneHelper:onInitPos0 -- char == nil");
		return 
	end				
	local headSprite = container:getVarSprite("mHeadCircle");	
	local itemId = char.itemId;
	if headSprite~=nil then	
		local iconPath = ""
		if char.type == Const_pb.MAIN_ROLE or char.type == Const_pb.MERCENARY then
			iconPath = roleCfg[itemId]["icon"];
		else
			iconPath = monsterCfg[itemId]["icon"];
			--如果是BOSS，整体放大1/0.8倍
			--[[local isBoss = monsterCfg[itemId]["isBoss"]
			if isBoss == 1 then
				container:setScale(1/0.8)
			else
				container:setScale(1)
			end--]]
		end			
		--头像设置
		
		headSprite:setTexture(iconPath)
		--遮罩处理
		headSprite:getParent():removeChildByTag(10010,true);
		local size = headSprite:getContentSize();
		local cover = CoverSprite:new()
		local coverSprite = CCSprite:create("UI/Career/u_BattleCircle.png")
		cover:initWithTexture(headSprite:getTexture(),coverSprite:getTexture(),CCRectMake(0.0,0.0,size.width,size.height));
		cover:setTag(10010)		
		headSprite:getParent():addChild(cover)
		cover:setPosition(headSprite:getPosition())
		headSprite:setVisible(false)
		cover:release();
	end
	local charLevel = 0
	local charName = ""
	local charColor = ""
	local GameConfig = require("GameConfig");
	if char.type == Const_pb.MAIN_ROLE or char.type == Const_pb.MERCENARY then
		UserInfo.syncRoleInfo()
		charLevel = char.level
		charName = char.name.." Lv."..tostring(charLevel)
		charColor = GameConfig.ColorMap.COLOR_PURPLE
	else
		charLevel = monsterCfg[itemId]["level"];
		charName = " Lv."..tostring(charLevel).." "..char.name
		charColor = GameConfig.ColorMap.COLOR_WHITE
	end
		
	local lb2Str = {
	mName				 	= charName,
	mBuff				 	="",
	mHP						= char.hp.."/"..char.hp,
	mMp						= char.mp.."/"..char.mp
	};
	NodeHelper:setStringForLabel(container, lb2Str);	
	
	local lb2Color = {
	mName				 	= charColor
	};
	NodeHelper:setColorForLabel(container, lb2Color);
	local hpBar = container:getVarScale9Sprite("mHpBar")	
	if hpBar~=nil  then
		hpBar:setScaleX(1)
	end
	
	
	local mpBar = container:getVarScale9Sprite("mMpBar")	
	if mpBar~=nil  then
		mpBar:setScaleX(1)
	end
	
	container:runAnimation("Default Timeline")	
end


function BattleFightSceneHelper:onPrepareCharacter(container,oneCha)
	if oneCha == nil then
		assert(false,"cha data is nil")
		return 
	end
	local pos = oneCha.pos	
	--位置，取值空间未[0, +~), 主角：0，佣兵2，奇数为怪物，1位置一般为boss
	if pos == 0 then	
		mainNode.chaNode:setVisible(true)
		BattleFightSceneHelper:onInitWithPos(mainNode.chaCCB,oneCha)
	elseif pos == 2  then		
		mercenaryNode.chaNode:setVisible(true)
		BattleFightSceneHelper:onInitWithPos(mercenaryNode.chaCCB,oneCha)
	elseif pos == 1  then		
		--enemyPos1.chaNode:removeChildByTag(20010,true)
		if enemyPos1.chaCCB ~=nil then
			BattleFightSceneHelper:showNormalState(enemyPos1)
			local  convertedcontent =  tolua.cast(enemyPos1.chaCCB,"CCNode")
			if convertedcontent~=nil then
				convertedcontent:removeAllChildren()
			end				
		end
		enemyPos1.chaNode:removeAllChildren()						
		if oneCha.type == Const_pb.MAIN_ROLE or oneCha.type == Const_pb.MERCENARY then
			enemyPos1.chaCCB = ScriptContentBase:create("BattleEnemyLead.ccbi");
		else
			--如果是BOSS，
			local isBoss = monsterCfg[oneCha.itemId]["isBoss"]
			if isBoss == 1 then
				enemyPos1.chaCCB = ScriptContentBase:create("BattleBossEnemy.ccbi");
			else
				enemyPos1.chaCCB = ScriptContentBase:create("BattleCommonEnemy.ccbi");
			end
		end
		enemyPos1.chaCCB:setTag(20010);
		enemyPos1.chaNode:addChild(enemyPos1.chaCCB);
		enemyPos1.chaCCB:release();
		BattleFightSceneHelper:assembleAniNodeInPersonNode(enemyPos1)
		enemyPos1.chaNode:setVisible(true)		
		BattleFightSceneHelper:onInitWithPos(enemyPos1.chaCCB,oneCha)
	elseif pos == 3  then		
		enemyPos3.chaNode:setVisible(true)
		BattleFightSceneHelper:onInitWithPos(enemyPos3.chaCCB,oneCha)
	elseif pos == 5  then		
		enemyPos5.chaNode:setVisible(true)
		BattleFightSceneHelper:onInitWithPos(enemyPos5.chaCCB,oneCha)
	else 
		CCLuaLog("ERROR in BattleFightSceneHelper:onPrepareCharacter , pos not correct "..pos);
	end
end

function BattleFightSceneHelper:assembleAniNodeInPersonNode(personNode)
	local personHitOrBufNode  = personNode.chaCCB:getVarNode("mPersonHitOrBufNode");
	personHitOrBufNode:removeAllChildren()
	personNode.personHitOrBufCCB = ScriptContentBase:create("BattleHitOrBuff.ccbi");
	if personHitOrBufNode~=nil and personNode.personHitOrBufCCB ~= nil then
		personHitOrBufNode:addChild(personNode.personHitOrBufCCB)
		personNode.personHitOrBufCCB:release();
	else 
		assert(false,"personHitOrBufNode~=nil and personNode.personHitOrBufCCB ~= nil")
	end	
	
	
	local personHitNumberNode = personNode.chaCCB:getVarNode("mPersonHitNumberNode");
	personHitNumberNode:removeAllChildren()
	personNode.normalNumCCB = ScriptContentBase:create("BattleNormalNum.ccbi");	
	if personHitNumberNode ~=nil and personNode.normalNumCCB~=nil then
		personHitNumberNode:addChild(personNode.normalNumCCB)
		personNode.normalNumCCB:release();
		--personNode.normalNumCCB:setVisible(false)	
	end		
	
	personNode.critsNumCCB = ScriptContentBase:create("BattleCritsNum.ccbi");	
	if personHitNumberNode ~=nil and personNode.critsNumCCB~=nil then
		personHitNumberNode:addChild(personNode.critsNumCCB)
		personNode.critsNumCCB:release();
		--personNode.critsNumCCB:setVisible(false)			
	end
	
	personNode.dodgeNumCCB = ScriptContentBase:create("BattleDodgeNum.ccbi");	
	if personHitNumberNode ~=nil and personNode.dodgeNumCCB~=nil then
		personHitNumberNode:addChild(personNode.dodgeNumCCB)
		personNode.dodgeNumCCB:release();
		--personNode.dodgeNumCCB:setVisible(false)			
	end		
	
	personNode.healNumCCB = ScriptContentBase:create("BattleHealNum.ccbi");	
	if personHitNumberNode ~=nil and personNode.healNumCCB~=nil then
		personHitNumberNode:addChild(personNode.healNumCCB)
		personNode.healNumCCB:release();
		--personNode.healNumCCB:setVisible(false)		
	end
	
	personNode.gainMPNumCCB = ScriptContentBase:create("BattleGainMPNum.ccbi");	
	if personHitNumberNode ~=nil and personNode.gainMPNumCCB~=nil then
		personHitNumberNode:addChild(personNode.gainMPNumCCB)
		personNode.gainMPNumCCB:release();		
		--personNode.gainMPNumCCB:setVisible(false)		
	end
end
	--[[enum ActionEffectType
	{
		NORMAL		= 0;						// 正常攻击，红色
		CRITS 		= 1;						// 暴击，黄色
		DODGE 		= 2;						// 闪避，白色
		HEAL		= 3;						// 治疗，绿色
		GAINMP		= 4;						// 回复魔法，蓝色
	}--]]

function BattleFightSceneHelper:showActionNumber(personNode,number,actionType)
	local actionCCB = {};	
	if actionType == 0 then
		actionCCB = personNode.normalNumCCB;
	elseif actionType ==1 then
		actionCCB = personNode.critsNumCCB;
	elseif actionType ==2 then
		actionCCB = personNode.dodgeNumCCB;
	elseif actionType ==3 then
		actionCCB = personNode.healNumCCB;
	elseif actionType ==4 then
		actionCCB = personNode.gainMPNumCCB;
	else
		actionCCB = personNode.normalNumCCB;
	end

	if actionCCB~= nil then 
		if actionType ==2 then
			actionCCB:runAnimation("showNum")		
		elseif actionType == 0 then
			number= "-"..number
			local lb2Str = {
				mNumLabel				 	= number	
			};
			NodeHelper:setStringForLabel(actionCCB, lb2Str);
			actionCCB:runAnimation("showNum")	
		elseif actionType == 1 then
			number= "-"..number
			local lb2Str = {
				mNumLabel				 	= number	
			};
			NodeHelper:setStringForLabel(actionCCB, lb2Str);
			actionCCB:runAnimation("showNum")	
		elseif actionType == 3 then
			number= "+"..number
			local lb2Str = {
				mNumLabel				 	= number	
			};
			NodeHelper:setStringForLabel(actionCCB, lb2Str);
			actionCCB:runAnimation("showNum")
		elseif actionType == 4 then
			number= "+"..number
			local lb2Str = {
				mNumLabel				 	= number	
			};
			NodeHelper:setStringForLabel(actionCCB, lb2Str);
			actionCCB:runAnimation("showNum")		
		end			
	end
end

function BattleFightSceneHelper:showNormalState(personNode)
	if personNode.chaCCB~=nil then		
		local  covertedcontent =  tolua.cast(personNode.chaCCB,"ScriptContentBase")
		if covertedcontent~=nil then
			--covertedcontent:runAnimation("Default Timeline");
			NodeHelper:setNodesVisible(personNode.chaCCB,
			{mDeathNode = false,mEscapeNode = false	})
		end			
	end		
end


function BattleFightSceneHelper:showDieAni(personNode)
	if personNode.chaCCB~=nil then
		local  covertedcontent =  tolua.cast(personNode.chaCCB,"ScriptContentBase")
		if covertedcontent~=nil then		
			NodeHelper:setNodesVisible(personNode.chaCCB,
		{mDeathNode = true,mEscapeNode = false	})
		end	
		
		--personNode.chaCCB:runAnimation("Death");
	end
end

function BattleFightSceneHelper:showEscapeAni(personNode)
	if personNode.chaCCB~=nil then
		local  covertedcontent =  tolua.cast(personNode.chaCCB,"ScriptContentBase")
		if covertedcontent~=nil then		
			NodeHelper:setNodesVisible(personNode.chaCCB,
			{mDeathNode = false,mEscapeNode = true	})
		end	
		
		--personNode.chaCCB:runAnimation("Escape");
	end
end

function BattleFightSceneHelper:showHitAni(personNode)
	personNode.personHitOrBufCCB:runAnimation("Hit")
end

function BattleFightSceneHelper:showBUFFAni(personNode)
	personNode.personHitOrBufCCB:runAnimation("BUFF")
end

function BattleFightSceneHelper:resetAllCharacter(container, keepSelf)
	local BattleFightHelper = require("BattleFightHelper")
	BattleFightHelper.FightChar = {}
	if keepSelf == nil or keepSelf == false then
		mainNode.chaNode:setVisible(false)
		mercenaryNode.chaNode:setVisible(false)
	end
	enemyPos1.chaNode:setVisible(false)
	enemyPos3.chaNode:setVisible(false)
	enemyPos5.chaNode:setVisible(false)
	BattleFightSceneHelper:showNormalState(mainNode)
	BattleFightSceneHelper:showNormalState(mercenaryNode)
	BattleFightSceneHelper:showNormalState(enemyPos1)
	BattleFightSceneHelper:showNormalState(enemyPos3)
	BattleFightSceneHelper:showNormalState(enemyPos5)
	BattleFightSceneHelper:resetNextPromptAni(container)
	BattleFightSceneHelper:resetHintAni(container)
end

function BattleFightSceneHelper:clearAllCharacter(container)
	mainNode.chaNode:removeAllChildren()
	mercenaryNode.chaNode:removeAllChildren()
	enemyPos1.chaNode:removeAllChildren()
	enemyPos3.chaNode:removeAllChildren()
	enemyPos5.chaNode:removeAllChildren()
end	

return BattleFightSceneHelper;