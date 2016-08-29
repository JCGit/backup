local MercenaryHaloManager = {}

MercenaryHaloManager.HaloMap = {}


MercenaryHaloManager.WGroup = {}
MercenaryHaloManager.HGroup = {}
MercenaryHaloManager.MGroup = {}

function MercenaryHaloManager:checkMerHasRingById(roleId)  
	if MercenaryHaloManager.HaloMap[roleId] == nil then
		return false
	else
		return true
	end
end

--检测是否有已经达到星级却没有激活的光环，用来红点显示
function MercenaryHaloManager:checkHasActivedHaloByMercenary(selectedMercenary)  
	local groupMap = {}
	if selectedMercenary.itemId == 7 then
		groupMap = MercenaryHaloManager.WGroup
	elseif selectedMercenary.itemId == 8 then
		groupMap = MercenaryHaloManager.HGroup
	elseif selectedMercenary.itemId == 9 then
		groupMap = MercenaryHaloManager.MGroup
	end	
	local starLevel = tonumber(selectedMercenary.starLevel)
	for i=1,#groupMap do
		local oneRing = groupMap[i]
		local ringItemId = oneRing["ringId"]	
		if starLevel>= tonumber(MercenaryHaloManager:getStarLimitByRingId(ringItemId)) then			
			if MercenaryHaloManager:checkActivedByItemId(selectedMercenary.roleId,ringItemId) == false then			
				return true
			end		
		end
	end
	return false
end

function MercenaryHaloManager:checkActivedByItemId(roleId,itemId)  
	if MercenaryHaloManager.HaloMap[roleId] == nil then
		return false
	end
	for _,value in pairs(MercenaryHaloManager.HaloMap[roleId]) do	
		local oneInfo = value
		if oneInfo.itemId == itemId then
			return true
		end
	end
	return false
end

function MercenaryHaloManager:onRecieveSyncMsg(msg)  
	if msg~=nil and msg.ringInfos ~=nil then
		local size = #msg.ringInfos
		for i=1,size do
			local oneInfo = msg.ringInfos[i]
			local roleId = oneInfo.roleId
			if MercenaryHaloManager.HaloMap[roleId] == nil then
				MercenaryHaloManager.HaloMap[roleId] = {}
				table.insert(MercenaryHaloManager.HaloMap[roleId],oneInfo)
			else
				table.insert(MercenaryHaloManager.HaloMap[roleId],oneInfo)
			end		
		end
	end
end


function MercenaryHaloManager:getAttrByRingId(id, attrName)
	local ringCfg = ConfigManager.getMercenaryRingCfg();
	local config = ringCfg[id];
	if config then
		return config[attrName];
	end
	return "";
end

function MercenaryHaloManager:getNameByRingId(id)
	return MercenaryHaloManager:getAttrByRingId(id, "name")
end

function MercenaryHaloManager:getIconByRingId(id)
	return MercenaryHaloManager:getAttrByRingId(id, "icon")
end

function MercenaryHaloManager:getStarLimitByRingId(id)
	return MercenaryHaloManager:getAttrByRingId(id, "starLimit")
end

function MercenaryHaloManager:getConsumeByRingId(id)
	return MercenaryHaloManager:getAttrByRingId(id, "consume")
end

function MercenaryHaloManager:getDiscribeByRingId(id)
	return MercenaryHaloManager:getAttrByRingId(id, "discribe")
end

function MercenaryHaloManager:getConditionByRingId(id)
	return MercenaryHaloManager:getAttrByRingId(id, "condition")
end

function MercenaryHaloManager:classifyGroup(cfg)  
	local size = #cfg
	
	if #MercenaryHaloManager.WGroup > 0 then
		return 
	end
	for key, value in pairs(cfg) do	
		local ringId = value["ringId"]
		local merId = value["merId"]
		if merId == 7 then
			table.insert(MercenaryHaloManager.WGroup,value)
		elseif merId == 8 then
			table.insert(MercenaryHaloManager.HGroup,value)
		elseif merId == 9 then
			table.insert(MercenaryHaloManager.MGroup,value)
		end
	end
end

return MercenaryHaloManager;