--encoding UTF-8 without BOM
--------------------------------------------------------------------------------
--  FILE:			RewardManagerLogic.lua
--  DESCRIPTION:	奖励内容填充区，服务器提供数据进行客户端基础数据同步以及部分客户端奖励展现

--
--
--  AUTHOR:		lvjc
--  CREATED:	2014-02-020
--------------------------------------------------------------------------------
--require "protobuf.UserFragmentInfo_pb"


RewardManagerLogic = {}

function RewardManagerLogic.updateToolCount(itemId, count)
	local userToolInfo = ServerDateManager:getInstance():getUserToolInfoByItemId(itemId)
	if userToolInfo ~= nil then
		userToolInfo.count = count
	end
end

function RewardManagerLogic.gotEquipment(info)
	if info~=nil then
		local userEquipInfo = ServerDateManager:getInstance():getAndCreatUserEquipInfo(info.id)
		userEquipInfo.itemid		= info.itemid
		userEquipInfo.level		    = info.level
		userEquipInfo.refinexp		= info.refinexp
		userEquipInfo.refinelevel	= info.refinelevel
		userEquipInfo.buffvalue     = info.buffvalue
		
		for index, propInfo in ipairs(info.pInfo) do
			if propInfo.type ~= nil then
				userEquipInfo:setPropertyInfoByKey(propInfo.type, propInfo.num)
			end
		end
		
		for index2, propInfo2 in ipairs(info.baptizeInfo) do
			if propInfo2.type ~= nil then
				userEquipInfo:setBaptizePropertyInfoByKey(propInfo2.type, propInfo2.num)
			end
		end
		
		for index3, propInfo3 in ipairs(info.tempBaptizeInfo) do
			if propInfo3.type ~= nil then
				userEquipInfo:setTmpBaptizePropertyInfoByKey(propInfo3.type, propInfo3.num)
			end
		end
		
		for pos, stoneInfo in ipairs(info.stoneInfo) do
			if stoneInfo ~= nil then
				userEquipInfo:setStoneInfoByPosition(pos, stoneInfo)
			end
		end
	end
end

function RewardManagerLogic.gotTool(info)
	if info~=nil then
		local userToolInfo = ServerDateManager:getInstance():getAndCreatToolInfo(info.id)
		userToolInfo.itemid	= info.itemid
		userToolInfo.count	= info.count
		ServerDateManager:getInstance():creatUserToolKeyMapByToolItem(userToolInfo)
	end
end

function RewardManagerLogic.gotSoul(info)
	if info~=nil then
		local soulInfo = ServerDateManager:getInstance():getAndCreatSoulInfo(info.id)
		soulInfo.itemid	= info.itemid
		soulInfo.count	= info.count
	end
end

function RewardManagerLogic.gotDisciple(info)
	if info~=nil then
		local userDiscipleInfo = ServerDateManager:getInstance():getAndCreatDiscipleInfo(info.id)
		userDiscipleInfo.itemid		= info.itemid
		userDiscipleInfo.level		= info.level
		userDiscipleInfo.exp		= info.exp
		userDiscipleInfo.health		= info.health
		userDiscipleInfo.attack		= info.attack
		userDiscipleInfo.theurgyDefence	= info.theurgyDefence
		userDiscipleInfo.physicsDefence			= info.physicsDefence
		userDiscipleInfo.upgradelevel	= info.upgradelevel
		userDiscipleInfo.potentiality	= info.potentiality
		userDiscipleInfo.skill1		= info.skill1
		userDiscipleInfo.skill2		= info.skill2
		userDiscipleInfo.preLevel		= 0 --没有用

		userDiscipleInfo.rp				= 0	--没有用

		userDiscipleInfo.battleid      = info.battleid
		userDiscipleInfo.stage      = info.stage
		userDiscipleInfo.physique      = info.physique
		userDiscipleInfo.stength      = info.strength
		userDiscipleInfo.armor      = info.armor
		userDiscipleInfo.isMain      = info.isMain
		userDiscipleInfo.healthPercentage      = info.healthPercentage
		userDiscipleInfo.attackPercentage      = info.attackPercentage
		userDiscipleInfo.physicsDefencePercentage      = info.physicsDefencePercentage
		userDiscipleInfo.theurgyDefencePercentage      = info.theurgyDefencePercentage
		
	end
end

----宝物
function RewardManagerLogic.gotTreasureInfo(info)
	if info~=nil then
		local treasureInfo = ServerDateManager:getInstance():getAndCreateUserTreasureInfo(info.id)
		treasureInfo.itemId			= tonumber(info.itemId)
		treasureInfo.level			= info.level
		treasureInfo.exp			= info.exp
		treasureInfo.refineLevel	= info.refineLevel
		
		for index, propInfo in ipairs(info.currentInfo) do
			if propInfo.type ~= nil then
				treasureInfo:insertCurrentAttr(propInfo.type, propInfo.num)
			end
		end
		
		for index2, propInfo2 in ipairs(info.refineInfo) do
			if propInfo2.type ~= nil then
				treasureInfo:insertRefineAttr(propInfo2.type, propInfo2.num)
			end
		end
	end
end

function RewardManagerLogic.gotFragmentBook(info)
	if ServerDateManager:getInstance():getFragmentMergeInfoMapTotalNum()==0 then
		if(ServerDateManager:getInstance():getFragmentBookInfoMapTotalNum()==0) then
		
		local msg = UserFragmentInfo_pb.OPGetUserFragmentInfo();
		msg.version = 1;
		local pb_data = msg:SerializeToString();
		local OPCODE_ACTIVITY_GET_USER_FRAGMENTINFO_C = 30101
		PacketManager:getInstance():sendPakcet(OPCODE_ACTIVITY_GET_USER_FRAGMENTINFO_C,pb_data,#pb_data,true);
	end
else
	if info~=nil then
		if info.increaseCount == nil then
			info.increaseCount = 0
		end
		if info.totalCount == nil then
			info.totalCount = 0
		end
		if info.fragmentId == nil then
			info.fragmentId = 0
		end
		ServerDateManager:getInstance():syncFragmentBookItemByFragmentId(info.id,info.fragmentId,info.totalCount,info.increaseCount)
	end
	
end
end

function RewardManagerLogic.addSoul(info)
	if info~=nil then
		local soulInfo = ServerDateManager:getInstance():getAndCreatSoulInfo(info.id)
		soulInfo.itemid	= info.itemid
		if soulInfo.count ~= nil then
			soulInfo.count	= soulInfo.count + info.count
		else
			soulInfo.count = info.count
		end
	end
end

function RewardManagerLogic.setRewardLogic(rewardMsg,showPopup,popTitle,popContent)
	
	---武将
	if rewardMsg.discipleInfo ~= nil then
		for _, info in ipairs(rewardMsg.discipleInfo) do
			RewardManagerLogic.gotDisciple(info)
		end
	end
	
	--武魂
	if rewardMsg.soulInfo ~= nil then
		for _, info in ipairs(rewardMsg.soulInfo) do
			RewardManagerLogic.gotSoul(info)
		end
	end
	
	--道具
	if rewardMsg.toolInfo ~= nil then
		for _, info in ipairs(rewardMsg.toolInfo) do
			RewardManagerLogic.gotTool(info)
		end
	end
	
	--装备
	if rewardMsg.equipInfo ~= nil then
		for _, info in ipairs(rewardMsg.equipInfo) do
			RewardManagerLogic.gotEquipment(info)
		end
	end
	--宝物
	if rewardMsg.treasureInfo ~= nil then
		for _, info in ipairs(rewardMsg.treasureInfo) do
			RewardManagerLogic.gotTreasureInfo(info)
		end
	end
	
	--碎片
	if rewardMsg.fragmentItem ~= nil then
		for _, info in ipairs(rewardMsg.fragmentItem) do
			RewardManagerLogic.gotFragmentBook(info)
		end
	end
	-------------------------------------------------------------------------
	-------------用户自身属性

	-------------------------------------------------------------------------
	--金币
	if rewardMsg:HasField("goldCoins") and rewardMsg.goldCoins~=nil then
		ServerDateManager:getInstance():getUserBasicInfo().goldcoins = tonumber(rewardMsg.goldCoins)
	end
	--银币
	if rewardMsg:HasField("silverCoins") and rewardMsg.silverCoins~=nil then
		local silverCoins=tonumber(rewardMsg.silverCoins)
		if silverCoins~=nil then
			ScriptMathToLua:modifySilverCoins(silverCoins)
		end
	end
	--体力
	if rewardMsg:HasField("power") and rewardMsg.power~=nil then
		ServerDateManager:getInstance():getUserBasicInfo().power=tonumber(rewardMsg.power)
	end
	--耐力
	if rewardMsg:HasField("vitality") and rewardMsg.vitality~=nil then
		ServerDateManager:getInstance():getUserBasicInfo().vitality=tonumber(rewardMsg.vitality)
	end
	--魂玉
	if rewardMsg:HasField("herosoul") and rewardMsg.herosoul~=nil then
		ServerDateManager:getInstance():getUserBasicInfo().heroSoul=tonumber(rewardMsg.herosoul)
	end
	
	--魂玉
	if rewardMsg:HasField("soulJade") and rewardMsg.soulJade~=nil then
		ServerDateManager:getInstance():getUserBasicInfo().SoulJade=tonumber(rewardMsg.soulJade)
	end
	
	
	--经验
	if rewardMsg:HasField("exp") and rewardMsg.exp~=nil then
		ServerDateManager:getInstance():getUserBasicInfo().exp=tonumber(rewardMsg.exp)
	end
	
	--level
	if rewardMsg:HasField("level") and rewardMsg.level~=nil then
		ServerDateManager:getInstance():getUserBasicInfo().level=tonumber(rewardMsg.level)
	end
	
	--vipLevel
	if rewardMsg:HasField("vipLevel") and rewardMsg.vipLevel~=nil then
		ServerDateManager:getInstance():getUserBasicInfo().viplevel=tonumber(rewardMsg.vipLevel)
	end
	
	--联盟贡献
	if rewardMsg:HasField("leaguePoint") and rewardMsg.leaguePoint~=nil then
		
	end
	
	
	--论剑积分
	if rewardMsg:HasField("reputation") and rewardMsg.reputation~=nil then
		
	end
	
	--论剑次数
	if rewardMsg:HasField("challengetimes") and rewardMsg.challengetimes~=nil then
		
	end
	
	if showPopup~=nil and showPopup then
		GoodsViewPage.mViewGoodsListInfo={}
		GoodsViewPage.mTitle=popTitle
		GoodsViewPage.mMsgContent=popContent
		
		for i = 1,#rewardMsg.reward do
			local reward=ResManager:getInstance():getResInfoByTypeAndId(rewardMsg.reward[i].itemType,rewardMsg.reward[i].itemId,rewardMsg.reward[i].count)
			GoodsViewPage.mViewGoodsListInfo[i]={}			
			GoodsViewPage.mViewGoodsListInfo[i].id = rewardMsg.reward[i].itemId
			GoodsViewPage.mViewGoodsListInfo[i].type=reward.type
			GoodsViewPage.mViewGoodsListInfo[i].name=reward.name
			GoodsViewPage.mViewGoodsListInfo[i].icon=reward.icon
			GoodsViewPage.mViewGoodsListInfo[i].count=reward.count
			GoodsViewPage.mViewGoodsListInfo[i].quality=reward.quality
			GoodsViewPage.mViewGoodsListInfo[i].typeName=reward.typeName
			GoodsViewPage.mViewGoodsListInfo[i].describe=reward.describe
		end
		
		MainFrame:getInstance():pushPage("GoodsShowListPage")
	end
	
	local msg = MsgTitleStatusChange:new()
	MessageManager:getInstance():sendMessageForScript(msg);
end

-----------------------------------------------------------
--author: cooper.xing
--function: 调用统一界面展示奖励信息（不获得奖励信息）
--attention: 1. 参数格式必须按照函数指定的格式传递，itemType、itemId、count属性必须具备；
-------------2. 本函数仅仅展示奖励信息，用户不会获得其中的奖励信息，如果想获得，可调用setRewardLogic函数
-----------------------------------------------------------
function RewardManagerLogic.onShowRewardInfo(rewardInfo, popTitle,popContent)
	GoodsViewPage.mViewGoodsListInfo={}
	GoodsViewPage.mTitle=popTitle
	GoodsViewPage.mMsgContent=popContent
	if type(rewardInfo) == "table" then
		for i = 1,#rewardInfo do
			local reward=ResManager:getInstance():getResInfoByTypeAndId(rewardInfo[i].itemType,rewardInfo[i].itemId,rewardInfo[i].count)
			GoodsViewPage.mViewGoodsListInfo[i]={}
			GoodsViewPage.mViewGoodsListInfo[i].type=reward.type
			GoodsViewPage.mViewGoodsListInfo[i].name=reward.name
			GoodsViewPage.mViewGoodsListInfo[i].icon=reward.icon
			GoodsViewPage.mViewGoodsListInfo[i].count=reward.count
			GoodsViewPage.mViewGoodsListInfo[i].quality=reward.quality
			GoodsViewPage.mViewGoodsListInfo[i].typeName=reward.typeName
			GoodsViewPage.mViewGoodsListInfo[i].describe=reward.describe
		end
	else
		local reward=ResManager:getInstance():getResInfoByTypeAndId(rewardInfo.itemType,rewardInfo.itemId,rewardInfo.count)
		GoodsViewPage.mViewGoodsListInfo[1]={}
		GoodsViewPage.mViewGoodsListInfo[1].type=reward.type
		GoodsViewPage.mViewGoodsListInfo[1].name=reward.name
		GoodsViewPage.mViewGoodsListInfo[1].icon=reward.icon
		GoodsViewPage.mViewGoodsListInfo[1].count=reward.count
		GoodsViewPage.mViewGoodsListInfo[1].quality=reward.quality
		GoodsViewPage.mViewGoodsListInfo[1].typeName=reward.typeName
		GoodsViewPage.mViewGoodsListInfo[1].describe=reward.describe
	end
	MainFrame:getInstance():pushPage("GoodsShowListPage")
	local msg = MsgTitleStatusChange:new()
	MessageManager:getInstance():sendMessageForScript(msg);
end


function RewardManagerLogic.setRewardContainerRender(rewardItems,container,maxSize)
	--奖励内容填充区，此部分可以后续剥离出去，抽成公共的部分

	local startSize=1
	for i=1,table.maxn(rewardItems) do
		local rewardItem=rewardItems[i]
		if rewardItem~=nil and startSize<=maxSize then
			local info=ResManager:getInstance():getResInfoByTypeAndId(rewardItem.type,rewardItem.itemId,rewardItem.count)
			if info~=nil then
				local _rewardIcon=container:getVarSprite("mRewardIcon"..startSize)
				local _rewardNode=container:getVarNode("mRewardNode"..startSize)
				if _rewardNode~=nil then
					_rewardNode:setVisible(true)
				end
				
				container:setDynamicIconTex("mRewardQuality"..startSize,"mRewardIcon"..startSize,info.icon)
				container:setFrameColorByQuality("mRewardQuality"..startSize,info.quality)
				
				local _rewardName=container:getVarLabelBMFont("mRewardName"..startSize)
				if _rewardName~=nil then
					_rewardName:setString(info.name)
				end
				
				local _rewardCount=container:getVarLabelBMFont("mRewardCount"..startSize)
				if _rewardCount~=nil then
					_rewardCount:setString("x"..info.count)
				end
				startSize=startSize+1
			end
		end
	end
	
	for i=startSize,maxSize+1,1 do
		local _rewardNode=container:getVarNode("mRewardNode"..i)
		if _rewardNode~=nil then
			_rewardNode:setVisible(false)
		end
	end
	
	
end

function RewardManagerLogic.setRewardContainerRenderNoName(rewardItems,container,maxSize)
	--奖励内容填充区，此部分可以后续剥离出去，抽成公共的部分(此部分用的是RewardItemContent2.ccbi)

	local startSize=1
	for i=1,table.maxn(rewardItems) do
		local rewardItem=rewardItems[i]
		if rewardItem~=nil and startSize<=maxSize then
			local info=ResManager:getInstance():getResInfoByTypeAndId(rewardItem.type,rewardItem.itemId,rewardItem.count)
			if info~=nil then
				local _rewardIcon=container:getVarSprite("mRewardIcon"..startSize)
				local _rewardNode=container:getVarNode("mRewardNode"..startSize)
				if _rewardNode~=nil then
					_rewardNode:setVisible(true)
				end
				
				container:setDynamicIconTex("mRewardQuality"..startSize,"mRewardIcon"..startSize,info.icon)
				container:setFrameColorByQuality("mRewardQuality"..startSize,info.quality)
				
				local _rewardCount=container:getVarLabelBMFont("mRewardCount"..startSize)
				if _rewardCount~=nil then
					_rewardCount:setString("x"..info.count)
				end
				startSize=startSize+1
			end
		end
	end
	for i=startSize,maxSize+1,1 do
		local _rewardNode=container:getVarNode("mRewardNode"..i)
		if _rewardNode~=nil then
			_rewardNode:setVisible(false)
		end
	end	
end
