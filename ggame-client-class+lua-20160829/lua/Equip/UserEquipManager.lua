local UserEquipManager = {};
--------------------------------------------------------------------------------

------------local variable for system api--------------------------------------
local tostring = tostring;
local tonumber = tonumber;
local string = string;
local pairs = pairs;
local math = math;
--------------------------------------------------------------------------------
local Equip_pb = require("Equip_pb");

local UserMercenaryManager = require("UserMercenaryManager");
local ItemManager = require("ItemManager");
local PBHelper = require("PBHelper");
local UserInfo = require("UserInfo");
--------------------------------------------------------------------------------
--装备分类：部位、品质、神器、所有、装在身上的(Part,Quality,Godly,All都是指在背包中的装备)
local EquipCategory = {
	Part 	= {},
	Quality	= {},
	Godly	= {},
	All		= {},
	Dress	= {}
};
--装备同步类型
local SyncType = {
	Insert 	= 1,
	Delete	= 2,
	On 		= 3,
	Off		= 4,
	Init	= 5,
	Update	= 6
};
local UserEquipMap = {};
--装备红点相关数据缓存
local EquipStatus = {
	size	 		= 0,		--所有装备的个数
	init 			= false,    --所有装备已从服务器同步
	isFull 			= false,
	partInfo 		= {},
	partInfo2 		= {},
	noticeCount		= 0,
	noticeCount2	= 0,
	needNotice		= false,	--for equipment page
	needNotice2		= false		--for mercenary page
};
--佣兵装备部位
local PartForMercenary = {
	Const_pb.CUIRASS,
	Const_pb.LEGGUARD,
	Const_pb.WEAPON1,
	Const_pb.WEAPON2
};

--根据评分排序
local function sortByScore(id_1, id_2)
	local isAsc = false;
	
	if id_2 == nil then
		return isAsc;
	end		
	if id_1 == nil then
		return not isAsc;
	end
	
	local userEquip_1 = UserEquipManager:getUserEquipById(id_1);
	local userEquip_2 = UserEquipManager:getUserEquipById(id_2);
	
	if userEquip_1.score ~= userEquip_2.score then
		if userEquip_1.score > userEquip_2.score then
			return isAsc;
		end
		return not isAsc;
	end
	
	if id_1 > id_2 then
		return isAsc;
	end
	
	return not isAsc;
end

--根据品质、评分排序
local function sortByQualityScore(id_1, id_2)
	local isAsc = false;
	
	if id_2 == nil then
		return isAsc;
	end		
	if id_1 == nil then
		return not isAsc;
	end
		
	local userEquip_1 = UserEquipManager:getUserEquipById(id_1);
	local userEquip_2 = UserEquipManager:getUserEquipById(id_2);
	
	local quality_1 = EquipManager:getQualityById(userEquip_1.equipId);
	local quality_2 = EquipManager:getQualityById(userEquip_2.equipId);
	
	if quality_1 ~= quality_2 then
		if quality_1 > quality_2 then
			return isAsc;
		else
			return not isAsc;
		end
	end
	
	if userEquip_1.score ~= userEquip_2.score then
		if userEquip_1.score > userEquip_2.score then
			return isAsc;
		end
		return not isAsc;
	end
	
	if id_1 > id_2 then
		return isAsc;
	end
	
	return not isAsc;
end

----------------------------------------------------------------------------------
function UserEquipManager:getUserEquipByIndex(index)
	local equipStr = ServerDateManager:getInstance():getEquipInfoByIndexForLua(index);
	local userEquip = Equip_pb.EquipInfo();
	userEquip:ParseFromString(equipStr);
	return userEquip;
end

function UserEquipManager:getUserEquipById(userEquipId)
	return UserEquipMap[userEquipId] or {};	
end

function UserEquipManager:resetCatecory()
	EquipCategory = {
		Part 	= {},
		Quality	= {},
		Godly	= {},
		All		= {},
		Dress	= {}
	};
end

function UserEquipManager:resetEquipStatus()
	EquipStatus = {
		size	 		= 0,
		init 			= false,
		isFull 			= nil,
		partInfo 		= {},
		partInfo2		= {},
		noticeCount		= 0,
		noticeCount2	= 0,
		needNotice		= false,
		needNotice2		= false
	};
	self:cancelAllNotice();
	self:cancelAllMercenaryNotice();
end

--小退重置
function UserEquipManager:reset()
	UserEquipMap = {};
	self:resetCatecory();
	self:resetEquipStatus();
	self:checkPackage();
end

--是否所有装备已从服务器同步(不包括离线、上线后新获取装备）
function UserEquipManager:hasInited()
	return EquipStatus.init;
end

function UserEquipManager:setUninited()
	self:reset();
	EquipStatus.init = false;
end

--服务器推送完所有老装备后，同步装备数据，并把装备进行分类
function UserEquipManager:check()
	if EquipStatus.init then return; end
	
	local size = ServerDateManager:getInstance():getEquipInfoTotalSize();
	if EquipStatus.size ~= size then
		self:resetCatecory();
		if not UserInfo.roleInfo.level then
			UserInfo.syncRoleInfo();
		end
		for i = 0, size - 1 do
			local userEquip = self:getUserEquipByIndex(i);
			self:classify(userEquip, SyncType.Init);
			UserEquipMap[userEquip.id] = userEquip;
			EquipStatus.size = EquipStatus.size + 1;
		end
	end
	EquipStatus.init = true;
	self:checkPackage();
end

--装备分类
function UserEquipManager:classify(userEquip, syncType)
	local id = userEquip.id;
	
	--神器传承后重新归类,原神器移除，新神器添加
	if syncType == SyncType.Update then	
		local ids = common:table_removeFromArray(EquipCategory.Godly, id);
		if self:isEquipGodly(userEquip) 
			and not self:isDressedWithEquipInfo(userEquip)
		then
			table.insert(ids, id);
		end
		EquipCategory.Godly = ids;
		return;
	end
	
	--归类不在身上的装备（包括新获得的、卸下的）
	if syncType == SyncType.Off or 
		syncType == SyncType.Insert or
		 not self:isDressedWithEquipInfo(userEquip) 
	then
	--if not self:isDressedWithEquipInfo(userEquip) then
		--部位分类
		local part = EquipManager:getPartById(userEquip.equipId);
		if EquipCategory.Part[part] == nil then
			EquipCategory.Part[part] = {id};
		else
			table.insert(EquipCategory.Part[part], id);
		end
		
		--品质分类
		local quality = EquipManager:getQualityById(userEquip.equipId);
		if EquipCategory.Quality[quality] == nil then
			EquipCategory.Quality[quality] = {id};
		else
			table.insert(EquipCategory.Quality[quality], id);
		end
			
		--神器分类
		if self:isEquipGodly(userEquip) then
			table.insert(EquipCategory.Godly, id);
		end
		
		--所有分类
		table.insert(EquipCategory.All, id);
		
		--检查背包是否满
		if syncType ~= SyncType.Init then
			self:checkPackage();
		end
		
		--卸下时Dress分类移除
		if syncType == SyncType.Off then
			EquipCategory.Dress = common:table_removeFromArray(EquipCategory.Dress, id);
		end
	else
		table.insert(EquipCategory.Dress, id);
	end
end

--背包中装备删除或上装后从各分类中移除并检查背包
function UserEquipManager:unclassify(id, syncType)
	local userEquip = UserEquipMap[id];
	if userEquip == nil then return; end
	
	if syncType == SyncType.On or 
		 not self:isDressedWithEquipInfo(userEquip) 
	then
		local part = EquipManager:getPartById(userEquip.equipId);
		if EquipCategory.Part[part] ~= nil then
			EquipCategory.Part[part] = common:table_removeFromArray(EquipCategory.Part[part], id);
		end
		
		local quality = EquipManager:getQualityById(userEquip.equipId);
		if EquipCategory.Quality[quality] ~= nil then
			EquipCategory.Quality[quality] = common:table_removeFromArray(EquipCategory.Quality[quality], id);
		end
			
		if self:isEquipGodly(userEquip) then
			EquipCategory.Godly = common:table_removeFromArray(EquipCategory.Godly, id);
		end
		
		EquipCategory.All = common:table_removeFromArray(EquipCategory.All, id);
		self:checkPackage();
		
		if syncType == SyncType.On then
			table.insert(EquipCategory.Dress, id);
		end
	end
end

--删除装备：分类移除，红点检查，装备缓存移除
function UserEquipManager:deleteEquip(userEquipId)
	if UserEquipMap[userEquipId] == nil then return; end
	
	self:unclassify(userEquipId, SyncType.Delete);
	self:checkPartAfterDelete(userEquipId);
	UserEquipMap[userEquipId] = nil;
	EquipStatus.size = EquipStatus.size - 1;
end

--上装：分类移除，红点检查
function UserEquipManager:takeOn(userEquipId)
	self:unclassify(userEquipId, SyncType.On);
	self:cancelNoticeAfterTakeOn(userEquipId);
end

--卸装: 分类检查
function UserEquipManager:takeOff(userEquipId)
	local userEquip = UserEquipMap[userEquipId];
	if userEquip then
		self:classify(userEquip, SyncType.Off);
	end
end	

--同步装备: 分类、添加缓存、检查红点
function UserEquipManager:syncOneEquipInfo(userEquip)
	if userEquip and userEquip.id then
		if UserEquipMap[userEquip.id] == nil then
			self:classify(userEquip, SyncType.Insert);
			EquipStatus.size = EquipStatus.size + 1;
		else
			self:classify(userEquip, SyncType.Update);
		end
		UserEquipMap[userEquip.id] = userEquip;
		self:checkPart(userEquip);
	end
end

--[[
function UserEquipManager:syncOneEquip(userEquipId)
	local equipStr = ServerDateManager:getInstance():getEquipInfoByIdForLua(userEquipId);
	local userEquip = Equip_pb.EquipInfo();
	userEquip:ParseFromString(equipStr);
	UserEquipMap[userEquipId] = userEquip;
end
--]]
------------------------------------------------------------------------------------------
--检查背包是否满
function UserEquipManager:checkPackage()
	--if not EquipStatus.init then return; end
	
	local bagSize = UserInfo.stateInfo.currentEquipBagSize or 40;
	local isFull = self:countEquipAll() >= bagSize;
	--if EquipStatus.isFull ~= isFull then
		PageManager.showRedNotice("Package", isFull);
		EquipStatus.isFull = isFull;
	--end
end

--检查某个部位的红点（是否有更好的装备）
function UserEquipManager:checkPart(userEquip)
	if not EquipManager:isDressable(userEquip.equipId, UserInfo.roleInfo.prof) then
		--主角不能装备，继续检查佣兵
		self:checkPartForMercenary(userEquip);
		return;
	end
	
	local part = EquipManager:getPartById(userEquip.equipId);
	local partInfo = EquipStatus.partInfo[part] or {};
	local roleEquip = UserInfo.getEquipByPart(part);
	
	local needNotice = true;
	local score = userEquip.score;
	local id = partInfo.id or userEquip.id;
	
	local needCheckMercenary = true;
	--主角该部位是否在装备
	if roleEquip and roleEquip.equipId > 0 then
		--装备与主角身上的装备是否相同
		if userEquip.id == roleEquip.equipId then
			--相同则不用继续检查佣兵
			needCheckMercenary = false;
			if partInfo.needNotice then
				--if partInfo.score > userEquip.score and not self:isEquipGodly(userEquip) then return; end
				--取消神器优先级,hgs@20140906	
				
				--更新后的装备比红点提示的装备评分更高或相同，则取消红点，否则保留
				if partInfo.score > userEquip.score then return; end				
				self:cancelNotice(part);
				id = userEquip.id;				
			end
			needNotice = false;
		else
			--更新佣兵身上的装备不产生红点
			if common:table_hasValue(EquipCategory.Dress, userEquip.id) then return; end
			
			local currentEquip = self:getUserEquipById(roleEquip.equipId);
			if userEquip.score <= currentEquip.score then
				--更新后的装备比当前身上装备评分更低或相同
				score = math.max(partInfo.score or 0, currentEquip.score);
				if partInfo.needNotice then
					--装备与红点提示装备相同则取消红点，否则保留
					if partInfo.id == userEquip.id then
						self:cancelNotice(part);
					else
						return;
					end
				end
				id = currentEquip.id;
				needNotice = false;
			else
				--比当前身上装备更好，更新提示信息，添加红点
				id = userEquip.score > (partInfo.score or 0) and userEquip.id or partInfo.id;
				score = math.max(userEquip.score, partInfo.score or 0);
				if not partInfo.needNotice then
					--if not self:isEquipGodly(currentEquip) or self:isEquipGodly(userEquip) then
					--取消神器优先级,hgs@20140906
					if true then
						EquipStatus.noticeCount = EquipStatus.noticeCount + 1;
						self:setAllNotice();
					else
						needNotice = false;
					end
				end
			end
		end
	else
		--更新佣兵身上的装备不产生红点
		if common:table_hasValue(EquipCategory.Dress, userEquip.id) then return; end
		if not partInfo.needNotice then
			EquipStatus.noticeCount = EquipStatus.noticeCount + 1;
		end
		self:setAllNotice();
		id = userEquip.id;
	end	
	EquipStatus.partInfo[part] = {
		needNotice = needNotice,
		score = score,
		id = id
	};
	
	--继续检查佣兵红点
	if needCheckMercenary then
		self:checkPartForMercenary(userEquip);
	end
end

--检查佣兵某个部位的红点（是否有更好的装备）,逻辑大致与主角一样
function UserEquipManager:checkPartForMercenary(userEquip)
	--检查部位
	local part = EquipManager:getPartById(userEquip.equipId);
	if not common:table_hasValue(PartForMercenary, part) then return; end
	
	local partInfo2 = EquipStatus.partInfo2[part] or {};
	local size = ServerDateManager:getInstance():getRoleInfoTotalSize();
	for i = 0, size - 1 do
		local userMercenary = UserMercenaryManager:getUserMercenaryByIndex(i);
		--拥有佣兵且职业符合
		if userMercenary and EquipManager:isDressable(userEquip.equipId, userMercenary.prof) then
			local partInfo = partInfo2[userMercenary.roleId] or {};
			
			local needNotice = true;
			local score = userEquip.score;
			local id = partInfo.id or userEquip.id;
			
			local roleEquip = PBHelper:getRoleEquipByPart(userMercenary.equips, part);
			if roleEquip and roleEquip.equipId > 0 then
				if userEquip.id == roleEquip.equipId then
					if partInfo.needNotice then
						--if partInfo.score > userEquip.score and not self:isEquipGodly(userEquip) then return; end
						--取消神器优先级,hgs@20140906
						if partInfo.score > userEquip.score then return; end
						self:cancelNotice(part, userMercenary.roleId);
						id = userEquip.id;				
					end
					needNotice = false;
				else
					local currentEquip = self:getUserEquipById(roleEquip.equipId);
					if userEquip.score <= currentEquip.score then
						score = math.max(partInfo.score or 0, currentEquip.score);
						if partInfo.needNotice then
							if partInfo.id == userEquip.id then
								self:cancelNotice(part, userMercenary.roleId);
							else
								return;
							end
						end
						id = currentEquip.id;
						needNotice = false;
					else
						id = userEquip.score > (partInfo.score or 0) and userEquip.id or partInfo.id;
						score = math.max(userEquip.score, partInfo.score or 0);
						if not partInfo.needNotice then
							--if not self:isEquipGodly(currentEquip) or self:isEquipGodly(userEquip) then
							--取消神器优先级,hgs@20140906
							if true then
								EquipStatus.noticeCount2 = EquipStatus.noticeCount2 + 1;
								self:setAllMercenaryNotice();
							else
								needNotice = false;
							end
						end
					end
				end
			else
				if not partInfo.needNotice then
					EquipStatus.noticeCount2 = EquipStatus.noticeCount2 + 1;
				end
				self:setAllMercenaryNotice();
				id = userEquip.id;
			end
			partInfo2[userMercenary.roleId] = {
				needNotice = needNotice,
				score = score,
				id = id
			};
			EquipStatus.partInfo2[part] = partInfo2;
		end
	end
end

--装备删除后检查红点
function UserEquipManager:checkPartAfterDelete(userEquipId)
	local userEquip = self:getUserEquipById(userEquipId);
	local part = EquipManager:getPartById(userEquip.equipId);
	local partInfo = EquipStatus.partInfo[part] or {};
	local partScore = partInfo.score or 0;
	
	if not EquipManager:isDressable(userEquip.equipId, UserInfo.roleInfo.prof) then
		--主角不能装备，继续检查佣兵
		self:checkPartForMercenaryAfterDelete(userEquipId);
		return;
	end
	
	--有红点，且删除的是提示装备，则取消红点
	if partInfo.needNotice and userEquip.score >= partScore then
		--[[
		local roleEquip = UserInfo.getEquipByPart(part);
		local currentEquip = self:getUserEquipById(roleEquip.equipId);
		local ids = self:getEquipIdsByClass("Part", part);
		for _, id in ipairs(ids) do 
			if id ~= userEquip.id then
				local equipInfo = self:getUserEquipById(id);
				if equipInfo.score > currentEquip.score then
					EquipStatus.partInfo[part] = {
						needNotice = true,
						score = equipInfo.score;
					};
				end
			end
		end
		--]]
		EquipStatus.partInfo[part] = {
			needNotice = false,
			score = 0,
			id = nil
		};
		self:cancelNotice(part);
	end
	--继续检查佣兵
	self:checkPartForMercenaryAfterDelete(userEquipId);
end

--装备删除后检查佣兵红点，逻辑大致与主角一样
function UserEquipManager:checkPartForMercenaryAfterDelete(userEquipId)
	local userEquip = self:getUserEquipById(userEquipId);
	local part = EquipManager:getPartById(userEquip.equipId);
	if not common:table_hasValue(PartForMercenary, part) then return; end
	
	local partInfo2 = EquipStatus.partInfo2[part] or {};
	local size = ServerDateManager:getInstance():getRoleInfoTotalSize();
	for i = 0, size - 1 do
		local userMercenary = UserMercenaryManager:getUserMercenaryByIndex(i);
		if userMercenary and EquipManager:isDressable(userEquip.equipId, userMercenary.prof) then
			local partInfo = partInfo2[userMercenary.roleId] or {};
			local partScore = partInfo.score or 0;
			
			if partInfo.needNotice and userEquip.score >= partScore then
				partInfo2[userMercenary.roleId] = {
					needNotice = false,
					score = 0,
					id = nil
				};
				EquipStatus.partInfo2[part] = partInfo2;
				self:cancelNotice(part, userMercenary.roleId);
			end
		end
	end
end

--取消红点，若各部位都无红点，取消banner红点
--@Param roleId: 佣点roleId, 若为nil,则为主角身上的红点
function UserEquipManager:cancelNotice(part, roleId)
	--主角红点
	if roleId == nil and EquipStatus.partInfo[part] then
		EquipStatus.partInfo[part] = {
			needNotice = false,
			score = 0,
			id = 0
		};
		EquipStatus.noticeCount = EquipStatus.noticeCount - 1;
		if EquipStatus.noticeCount <= 0 then
			EquipStatus.noticeCount = 0;
			self:cancelAllNotice();
		end
		return;
	end
	
	--佣兵红点
	local partInfo = EquipStatus.partInfo2[part] or {};
	if roleId and partInfo[roleId] then
		partInfo[roleId] = {
			needNotice = false,
			score = 0,
			id = 0
		};
		EquipStatus.noticeCount2 = EquipStatus.noticeCount2 - 1;
		if EquipStatus.noticeCount2 <= 0 then
			EquipStatus.noticeCount2 = 0;
			self:cancelAllMercenaryNotice();
		end
	end
end

--上装后取消红点
function UserEquipManager:cancelNoticeAfterTakeOn(userEquipId)
	local userEquip = self:getUserEquipById(userEquipId);
	local part = EquipManager:getPartById(userEquip.equipId);
	local roleEquip = UserInfo.getEquipByPart(part);
	
	if roleEquip and roleEquip.equipId == userEquipId then
		for _part, _partInfo2 in pairs(EquipStatus.partInfo2) do
			if _part == part then
				for roleId, _partInfo in pairs(_partInfo2) do
					if _partInfo.id and _partInfo.id == userEquipId then
						self:cancelNotice(part, roleId);
					end
				end
			end
		end
	else
		self:cancelNotice(part);
	end
end

--显示MainFrameBottom中装备红点
function UserEquipManager:setAllNotice()
	EquipStatus.needNotice = true;
	PageManager.showRedNotice("Equipment", true);
end

--取消MainFrameBottom中装备红点
function UserEquipManager:cancelAllNotice()
	EquipStatus.needNotice = false;
	PageManager.showRedNotice("Equipment", false);
end

--显示MainFrameBottom中佣兵红点
function UserEquipManager:setAllMercenaryNotice()
	EquipStatus.needNotice2 = true;
	PageManager.showRedNotice("Mercenary", true);
end

--取消MainFrameBottom中佣兵红点
function UserEquipManager:cancelAllMercenaryNotice()
	EquipStatus.needNotice2 = false;
	PageManager.showRedNotice("Mercenary", false);
end

--某个部位是否有红点（有更好的装备）
--@param roleId: 佣兵id, nil为主角
function UserEquipManager:isPartNeedNotice(part, roleId)
	--主角
	if roleId == nil then
		local partInfo = EquipStatus.partInfo[part] or {};
		return partInfo.needNotice or false;
	end
	
	--佣兵
	local partInfo2 = EquipStatus.partInfo2[part] or {};
	local partInfo = partInfo2[roleId] or {};
	return partInfo.needNotice or false;
end
------------------------------------------------------------------------------------------
--***************获取各个分类的装备个数*****************--
function UserEquipManager:countByClass(_class, subClass)
	if _class == "All" then
		return self:countEquipAll();
	elseif _class == "Godly" then
		return self:countEquipGodly();
	elseif _class == "Part" then
		return self:countEquipWithPart(subClass);
	elseif _class == "Quality" then
		return self:countEquipWithQuality(subClass);	
	end
	return 0;
end

function UserEquipManager:countEquipWithPart(part)
	local tb = EquipCategory.Part[part];
	return tb and #tb or 0;
end

function UserEquipManager:countEquipWithQuality(quality)
	local tb = EquipCategory.Quality[quality];
	return tb and #tb or 0;
end

function UserEquipManager:countEquipForBatchSell(quality)
	local tb = EquipCategory.Quality[quality] or {};
	local i = 0;
	for _, id in ipairs(tb) do
		if not self:isGodly(id) and not self:hasGem(id) then
			i = i + 1;
		end
	end
	return i;
end

function UserEquipManager:countEquipGodly()
	return #EquipCategory.Godly;
end

function UserEquipManager:countEquipAll()
	return #EquipCategory.All;
end

------------------------------------------------------------------------------------------
--获取各分类对应装备id
function UserEquipManager:getEquipIdsByClass(_class, subClass)
	local _class = _class or "All";
	if _class == "All" or _class == "Godly" or _class == "Dress" then
		return EquipCategory[_class];
	else
		return EquipCategory[_class][subClass] or {};
	end
end

--装备是否装在身上（包括主角和佣兵）
function UserEquipManager:isEquipDressed(userEquipId)
	local userEquip = self:getUserEquipById(userEquipId);
	return self:isDressedWithEquipInfo(userEquip);
end

--装备是否装在主角身上
function UserEquipManager:isEquipDressedOnPlayer(userEquip)
	local part = EquipManager:getPartById(userEquip.equipId);
	
	local roleEquip = UserInfo.getEquipByPart(part);
	if roleEquip and roleEquip.equipId == userEquip.id then
		return true;
	end;
	return false;
end
	
function UserEquipManager:isDressedWithEquipInfo(userEquip)
	return self:isEquipDressedOnPlayer(userEquip) or UserMercenaryManager:isEquipDressed(userEquip.id);
end

--获取用户装备属性信息
--@Param isAll: 是否全部显示
--@Return: Html String
function UserEquipManager:getEquipInfoString(userEquipId, isAll)
	local userEquip = self:getUserEquipById(userEquipId);		
	return self:getDesciptionWithEquipInfo(userEquip, isAll);
end

--获取装备属性信息(装备限制、评分、主属性、副属性、神器属性、神器全身强化效果、宝石加成)
--@Param isAll: 是否全部显示（是否显示神器全身强化效果、宝石加成）,文体大小会不同
--@Param isViewingOther: 是否是查看别人阵容
--@Return: Html String
function UserEquipManager:getDesciptionWithEquipInfo(userEquip, isAll, isViewingOther)
	local retStr = "";
	local suffix = isAll and "" or "_1";	--使用不现的html配置，主要是字体大小不同
	local glue = '<br/>';					--字体串拼接符
	
	local strTb = {};
	
	local equipId = userEquip.equipId;
	
	--职业限制信息(如果有)
	local professionId = EquipManager:getProfessionById(equipId);
	if professionId and professionId > 0 then
		local professionName = common:getLanguageString("@ProfessionName_" .. professionId);
		--table.insert(strTb, common:getLanguageString("@EquipCondition", professionName));
		table.insert(strTb, common:fillHtmlStr("EquipCondition" .. suffix, professionName));
	end
	
	--评分
	local grade = userEquip.score;
	--table.insert(strTb, common:getLanguageString("@EquipGrade", grade));
	table.insert(strTb, common:fillHtmlStr("EquipGrade" .. suffix, grade));
	
	--从装备数据中分类出属性数据（主、副、神）
	local quality = EquipManager:getQualityById(userEquip.equipId);
	local attrTb = {
		[Const_pb.PRIMARY_ATTR] 	= {},
		[Const_pb.SECONDARY_ATTR] 	= {},
		[Const_pb.GODLY_ATTR] 		= {}
	};
	local dmg = {};
	--分类
	for _, equipAttr in ipairs(userEquip.attrInfos) do
		local grade = equipAttr.attrGrade;
		local attr = equipAttr.attrData;
		
		local attrStr = nil;
		--针对伤害特殊处理
		local doCheck = false;
		if attr.attrId == Const_pb.MINDMG then
			dmg.min = attr.attrValue;
			doCheck = true;
		elseif attr.attrId == Const_pb.MAXDMG then
			dmg.max = attr.attrValue;
			doCheck = true;
		else
			local name = common:getAttrName(attr.attrId);
			if grade == Const_pb.SECONDARY_ATTR and not isAll then
				local len = GameConfig.LineWidth.SecondaryAttrNum;
				len = tostring(attr.attrValue):len() % 2 == 0 and len or (len + 1);
				local fmt = "%s +%-" .. len .. "s";
				attrStr = string.format(fmt, name, attr.attrValue);
			else
				attrStr = common:getLanguageString("@EquipAttrVal", name, attr.attrValue);
			end
		end
		
		if doCheck and dmg.min and dmg.max then
			attrStr = common:getLanguageString("@EquipDMGVal", dmg.min, dmg.max);
			dmg = {};
		end
		if attrStr ~= nil then
			local key = grade == Const_pb.PRIMARY_ATTR and "MainAttr" or "SecondaryAttr_" .. quality;
			key = key .. suffix;
			attrTb[grade][attr.attrId] = common:fillHtmlStr(key, attrStr);
		end
	end
	--组合htmlString
	for attrGrade, subAttrTb in ipairs(attrTb) do
		local str = "";
		--非全显时，把副属性两两一行显示
		if attrGrade == Const_pb.SECONDARY_ATTR and not isAll then
			local format = "%s%s%s%s";
			local attrIds = common:table_keys(subAttrTb);
			for i, attrId in ipairs(attrIds) do
				if i % 2 == 1 then
					local nextAttrId = attrIds[i + 1] or 0;
					local prefix = i > 1 and glue or "";
					str = string.format(format, str, prefix, subAttrTb[attrId], subAttrTb[nextAttrId] or "");
				end
			end
		else
			str = common:table_implode(subAttrTb, glue);
		end
		if str ~= "" then			
			table.insert(strTb, str);
		end
	end
	
	local commonInfo = table.concat(strTb, glue);
	retStr = commonInfo;
	
	--神器属性、神器全身强化效果
	local godlyInfo = self:getGodlyInfo(userEquip, isAll, isViewingOther);
	if godlyInfo ~= "" then
		retStr = retStr .. glue .. godlyInfo;
	end
	
	--宝石加成
	if isAll then
		local gemInfo = self:getGemInfo(userEquip);
		if gemInfo ~= "" then
			retStr = retStr .. glue .. gemInfo;
		end
	end
	
	--通过margin设置不同的宽度
	local margin = isAll and GameConfig.Margin.EquipInfo or GameConfig.Margin.EquipSelect;
	return common:fillHtmlStr("EquipInfoWrap", margin, retStr);
end

--获取可熔炼装备id,并排序
--@Param isAuto: 是否是自动筛选，是否过滤掉戴宝石装备
function UserEquipManager:getEquipIdsForSmelt(isAuto)
	--self:classify();
	local ids = {};
	for _, id in ipairs(self:getEquipIdsByClass("All")) do
		--if not self:isEquipDressed(id) 
			--and not self:isGodly(id) 
			--and (not isAuto or not self:hasGem(id))
		--then
		if not self:isGodly(id)
			and (not isAuto or not self:hasGem(id))
		then
			table.insert(ids, id);
		end
	end
	table.sort(ids, sortByQualityScore);
	return ids;
end

--获取可被吞噬装备id,并排序
--规则：1.双神器可吞噬所有其它神器，单属性神器只能吞噬对应属性单神器;
--		2.星级到上限后不能吞噬,两条属性分别计算
--@Param excludeId: 吞噬者
--@Param isAuto: 是否是自动筛选，是否过滤掉戴宝石装备
function UserEquipManager:getEquipIdsForSwallow(excludeId, isAuto)
	local ids = {};
	
	--enable: 是否能吞噬，limitPos: 可吞噬属性部位
	local enable, limitPos = self:canSwallow(excludeId);
	if not enable then return ids; end
--[[	local userEquip = self:getUserEquipById(excludeId);
	local limitPos = userEquip.godlyAttrId > 0 and userEquip.starLevele and 1 or nil;
	if userEquip:HasField("godlyAttrId2") and userEquip.godlyAttrId2 > 0 then
		limitPos = limitPos == nil and 2 or nil;
	end--]]
	
	for _, id in ipairs(self:getEquipIdsByClass("Godly")) do
		if id ~= excludeId 
			and (not isAuto or not self:hasGem(id))
			and (limitPos == nil or self:isSwallowable(id, limitPos))
		then
			table.insert(ids, id);
		end
	end
	table.sort(ids, sortByScore);
	return ids;
end

--能否被吞噬
--@Param limitGodlyPos: nil时，两个部分都能被吞，否则必须是对应属性的单属性神器
function UserEquipManager:isSwallowable(userEquipId, limitGodlyPos)
	local userEquip = self:getUserEquipById(userEquipId);
	if limitGodlyPos == 1 then
		return not (userEquip:HasField("godlyAttrId2") and userEquip.godlyAttrId2 > 0);
	elseif limitGodlyPos == 2 then
		return userEquip.godlyAttrId <= 0;
	end
	return true;
end

--[[
function UserEquipManager:getEquipIdsForExtend(excludeId)
	local ids = {};
	for _, id in ipairs(self:getEquipIdsByClass("Godly")) do
		if id ~= excludeId then
			table.insert(ids, id);
		end
	end
		for _, id in ipairs(self:getEquipIdsByClass("Dress")) do
		if id ~= excludeId then
			table.insert(ids, id);
		end
	end
	return ids;
end	
--]]

--是否是神器
function UserEquipManager:isGodly(userEquipId)
	local userEquip = self:getUserEquipById(userEquipId);
	return self:isEquipGodly(userEquip);
end

--是否是神器
function UserEquipManager:isEquipGodly(userEquip)
	if userEquip then
		if userEquip.godlyAttrId > 0 then
			return true;
		end
		if userEquip:HasField("godlyAttrId2") then
			return userEquip.godlyAttrId2 > 0;
		end
	end
	return false;
end

--获取神器主要属性
--@Param isAll: 是否全部显示（字体大小不同）
--@Param pos: 第几条神器属性（nil 时显示第一条与第二条拼接，用到了递归）
--@Param noHtml: 不返回htmlString
--@Return: noHtml == true时，返回HtmlString,否则普通字符串
function UserEquipManager:getMainGodlyAttr(userEquip, isAll, pos, noHtml)
	if pos == nil then
		--两条属性String拼接
		local strTb = {};
		local glue = noHtml and "\n" or "<br/>";	--不同的拼接符
		local str = self:getMainGodlyAttr(userEquip, isAll, 1, noHtml);
		if str ~= "" then
			table.insert(strTb, str);
		end
		str = self:getMainGodlyAttr(userEquip, isAll, 2, noHtml);
		if str ~= "" then
			table.insert(strTb, str);
		end
		return table.concat(strTb, glue);
	end
	
	--获取属性id,神器星级
	local attrId, starLevel = 0, 0;
	if pos == 1 then
		attrId, starLevel = userEquip.godlyAttrId, userEquip.starLevel;
	elseif pos == 2 and userEquip:HasField("godlyAttrId2") then
		attrId, starLevel = userEquip.godlyAttrId2, userEquip.starLevel2;	
	end
			
	if attrId <= 0 then return ""; end
	local suffix = isAll and "" or "_1";	--不同的格式
	
	local starAttrName = common:getLanguageString("@AttrName_" .. attrId);
	if noHtml then
		local starAttrVal = EquipManager:getStarAttrByLevel(attrId, starLevel, "%.1f%%%%");
		return common:getLanguageString("@GodlyStarAttr", starAttrName, starAttrVal);
	end
	local starAttrVal = EquipManager:getStarAttrByLevel(attrId, starLevel);
	return common:fillHtmlStr("GodlyAttr" .. suffix, starAttrName .. ' +' .. starAttrVal);
end

--获取神器属性（包括主要属性、神器全身强化效果）
--@Param isAll: 是否全部显示（是否包括全身强化效果，字体大小不同）
--@Param isViewingOther: 是否是查看别人阵容
--@Return: HtmlString
function UserEquipManager:getGodlyInfo(userEquip, isAll, isViewingOther)
	local glue = "<br/>";					--字体串拼接符
	local suffix = isAll and "" or "_1";	--使用不现的html配置，主要是字体大小不同
	local attrId = userEquip.godlyAttrId;
	local attrId2 = userEquip:HasField("godlyAttrId2") and userEquip.godlyAttrId2 or -1;
	if attrId <= 0 and attrId2 <= 0 then return ""; end
	
	local strTb = {};
	
	--全身强化等级要与神器属性对应星级取最小值，所以有两个
	local allEnhanceLevel_1, allEnhanceLevel_2 = nil, nil;
	local showEnhanceAttr = nil;
		
	if attrId > 0 then
		--几星神器
		local title = common:getLanguageString("@GodlyStarLevel", userEquip.starLevel);
		table.insert(strTb, common:fillHtmlStr("GodlyAttr" .. suffix, title));
	
		--神器主属性
		table.insert(strTb, self:getMainGodlyAttr(userEquip, isAll, 1));
		
		--显示所有且是查看别人阵容或查看装备在主角身上的装备时才显示强化效果
		showEnhanceAttr = isAll and (isViewingOther or self:isEquipDressedOnPlayer(userEquip));
		
		if showEnhanceAttr then	
			local starAttrName = common:getLanguageString("@AttrName_" .. attrId);
			--获取全身强化等级
			allEnhanceLevel_1 = UserInfo.getAllEnhancedLevel(isViewingOther);
			allEnhanceLevel_2 = allEnhanceLevel_1;
			if allEnhanceLevel_1 > 0 then
				--全身强化等级与神器星级取小
				allEnhanceLevel_1 = math.min(allEnhanceLevel_1, userEquip.starLevel);
				local activeVal = EquipManager:getActiveValByLevel(attrId, allEnhanceLevel_1);
				--已激活属性
				local activeInfo = common:fillHtmlStr("GodlyActiveAttr", starAttrName, allEnhanceLevel_1, activeVal);
				table.insert(strTb, activeInfo);
			end
			
			--查看别人阵容或者星级达到最大时不显示下一等级强化效果
			if not isViewingOther and userEquip.starLevel < GameConfig.LevelLimit.GodlyLevelMax then
				local nextVal = EquipManager:getActiveValByLevel(attrId, allEnhanceLevel_1 + 1);
				if nextVal ~= 0 then
					--显示下一等级强化效果，控制是否提示"下一星级"
					local contentKey = allEnhanceLevel_1 < userEquip.starLevel and "GodlyUnactiveAttr" or "GodlyNextStarAttr";
					local unactiveInfo = common:fillHtmlStr(contentKey, starAttrName, allEnhanceLevel_1 + 1, nextVal);
					table.insert(strTb, unactiveInfo);
				end
			end
		end
	end
		
	--第二条神器属性强化效果，逻辑同第一条
	if attrId2 > 0 then
		local title = common:getLanguageString("@GodlyStarLevel", userEquip.starLevel2);
		table.insert(strTb, common:fillHtmlStr("GodlyAttr" .. suffix, title));
	
		table.insert(strTb, self:getMainGodlyAttr(userEquip, isAll, 2));
			
		if showEnhanceAttr == nil then
			showEnhanceAttr = isAll and (isViewingOther or self:isEquipDressedOnPlayer(userEquip));
		end
			
		if showEnhanceAttr then
			local starAttrName = common:getLanguageString("@AttrName_" .. attrId2);
			if allEnhanceLevel_2 == nil then
				allEnhanceLevel_2 = UserInfo.getAllEnhancedLevel();
			end
			
			if allEnhanceLevel_2 > 0 then
				allEnhanceLevel_2 = math.min(allEnhanceLevel_2, userEquip.starLevel2);
				local activeVal = EquipManager:getActiveValByLevel(attrId2, allEnhanceLevel_2);
				local activeInfo = common:fillHtmlStr("GodlyActiveAttr", starAttrName, allEnhanceLevel_2, activeVal);
				table.insert(strTb, activeInfo);
			end
	
			if not isViewingOther and userEquip.starLevel2 < GameConfig.LevelLimit.GodlyLevelMax then
				local nextVal = EquipManager:getActiveValByLevel(attrId2, allEnhanceLevel_2 + 1);
				if nextVal ~= 0 then
					local contentKey = allEnhanceLevel_2 < userEquip.starLevel2 and "GodlyUnactiveAttr" or "GodlyNextStarAttr";
					local unactiveInfo = common:fillHtmlStr(contentKey, starAttrName, allEnhanceLevel_2 + 1, nextVal);
					table.insert(strTb, unactiveInfo);
				end
			end
		end
	end
	
	return table.concat(strTb, glue);
end

--装备是否能融合，必须是单属性神器
function UserEquipManager:canCompound(userEquipId)
	local userEquip = self:getUserEquipById(userEquipId);
	local attrId_1 = userEquip.godlyAttrId;
	local attrId_2 = userEquip:HasField("godlyAttrId2") and userEquip.godlyAttrId2 or -1;
	
	if (attrId_1 > 0 and attrId_2 > 0)
		or (attrId_1 <= 0 and attrId_2 <= 0)
	then
		return false;
	end
	
	return true, (attrId_1 > 0 and 2 or 1);
end

--能否吞噬其它神器
--规则：必须是神器，且对应神器属性星级未达上限
--@Return: enable是否能吞噬，limitPos单属性神器吞噬属性部位限制(1或２或nil)
function UserEquipManager:canSwallow(userEquipId)
	local userEquip = self:getUserEquipById(userEquipId);
	
	local enable, limitPos = false, nil;
	if userEquip.godlyAttrId > 0 then
		if userEquip.starLevel < GameConfig.LevelLimit.GodlyLevelMax then
			enable = true;
			limitPos = 1;
		end
	end
	
	if userEquip:HasField("godlyAttrId2") and userEquip.godlyAttrId2 > 0 then
		if userEquip.starLevel2 < GameConfig.LevelLimit.GodlyLevelMax then
			enable = true;
			--若两个部分都有限制，则视为无限制
			limitPos = limitPos == nil and 2 or nil;
		end
	end
	
	return enable, limitPos;
end

--获取宝石加成信息
--@Return: HtmlString
function UserEquipManager:getGemInfoById(userEquipId)
	local userEquip = self:getUserEquipById(userEquipId);
	return self:getGemInfo(userEquip);
end

--获取宝石加成信息
--@Return: HtmlString
function UserEquipManager:getGemInfo(userEquip)
	local glue = "<br/>"
	--整理宝石id，剔除只有孔的部位(复制的代码吧，没有必要)
	local pos2GemId = {};
	for _, gemInfo in ipairs(userEquip.gemInfos) do
		local gemId = gemInfo.gemItemId;
		if gemId and gemId > 0 then
			pos2GemId[gemInfo.pos] = gemId;
		end
	end
	
	--接合加成信息
	local texTb = {};
	for _, gemId in pairs(pos2GemId) do
		local gemStr = ItemManager:getNameById(gemId) .. " " .. ItemManager:getGemAttrString(gemId);
		table.insert(texTb, common:fillHtmlStr("GemInfo", gemStr));
	end
	
	return table.concat(texTb, glue);
end	

--装备是否镶嵌有宝石
function UserEquipManager:hasGem(userEquipId)
	local userEquip = self:getUserEquipById(userEquipId);
	for _, gemInfo in ipairs(userEquip.gemInfos) do
		local gemId = gemInfo.gemItemId;
		if gemId and gemId > 0 then
			return true;
		end
	end
	return false;
end

--获取装备强化所需强化精华个数
--影响要素:强化等级、品质、等级、部位（权重不同）
function UserEquipManager:getItemNeedForEnhance(userEquipId)
	local userEquip = self:getUserEquipById(userEquipId);
	local targetLevel = userEquip.strength + 1;
	local factor = {
		targetLevel,
		EquipManager:getQualityById(userEquip.equipId),
		EquipManager:getLevelById(userEquip.equipId),
		EquipManager:getPartById(userEquip.equipId)
	};
	local count = 1;
	for i, v in ipairs(factor) do
		count = count * EquipManager:getWeightByIdAndType(v, i);
	end
	return math.floor(count);
end

--获取装备强化所需金币
function UserEquipManager:getEnhanceCoinCost(userEquipId, itemCost)
	if itemCost == nil then
		itemCost = self:getItemNeedForEnhance(userEquipId);
	end
	return itemCost * 2000;
end

--获取神器所有经验
--@Return: 所有经验，第二条属性经验,第一条属性
function UserEquipManager:getGodlyTotalExp(userEquipId)
	local userEquip = self:getUserEquipById(userEquipId);
	if userEquip == nil then return 0; end
	
	local exp = 0;
	
	if userEquip.godlyAttrId > 0 then
		exp = userEquip.starExp + 1;	--保证至少有１点经验
		for lv = 1, userEquip.starLevel - 1 do
			exp = exp + EquipManager:getExpNeedForLevelUp(lv);
		end
	end
	
	local exp2 = 0;
	if userEquip:HasField("godlyAttrId2") and userEquip.godlyAttrId2 > 0 then
		exp2 = userEquip.starExp2 + 1;	--保证至少有１点经验
		for lv = 1, userEquip.starLevel2 - 1 do
			exp2 = exp2 + EquipManager:getExpNeedForLevelUp(lv, true);
		end
	end
	return exp + exp2, exp2 ,exp;
end

--获取神器吞噬所需金币、道具(注灵之石，已经取消）
--@Param sourceIds: table 被吞噬装备id
--@Param targetId: 吞噬者id
--@Return coinCost, itemCost
function UserEquipManager:getSwallowCost(sourceIds, targetId)
	local starExp = 0;
	local starExp2 = 0;
	for _, sourceId in ipairs(sourceIds) do
		local totalExp, exp2 = self:getGodlyTotalExp(sourceId);
		starExp = starExp + totalExp;
		starExp2 = starExp2 + exp2;
	end
	
	local userEquip = self:getUserEquipById(targetId);
	local level = EquipManager:getLevelById(userEquip.equipId);
	local factor = EquipManager:getWeightByPart(EquipManager:getPartById(userEquip.equipId));
	
	starExp = starExp + starExp2;
	local coinCost = 100 * starExp * math.pow((level + factor), 2);
	local itemCost = starExp2 * 1;
	return coinCost, itemCost;
end

--获取神器传承所需金币
--跟等级相关，权重由部位决定
--@Param sourceIds: 被吞噬装备id
--@Param targetId: 吞噬者id
--@Return coinCost
function UserEquipManager:getExtendCoinCost(sourceId, targetId)
	local sourceEquip = self:getUserEquipById(sourceId);
	local targetEquip = self:getUserEquipById(targetId);
	
	local sourceLevel = EquipManager:getLevelById(sourceEquip.equipId);
	local targetLevel = EquipManager:getLevelById(targetEquip.equipId);
	local sourceFactor = EquipManager:getWeightByPart(EquipManager:getPartById(sourceEquip.equipId));
	local targetFactor = EquipManager:getWeightByPart(EquipManager:getPartById(targetEquip.equipId));
	local _,sourceExp2 , sourceExp1 = self:getGodlyTotalExp(sourceId);
	
	local cost = math.max(0, math.pow(targetLevel + targetFactor, 2) - math.pow(sourceLevel + sourceFactor, 2)) * 120 * sourceExp1 + 10000;
		
	if sourceExp2 ~= 0 then
		cost = cost + (math.max(0, math.pow(targetLevel + targetFactor, 2) - math.pow(sourceLevel + sourceFactor, 2)) * 120 * sourceExp2 + 10000)
	end
	return math.floor(cost);
end	

--获取神器融合所需金币、道具(注灵之石，已经取消）
--跟等级相关，权重由部位决定
--@Param sourceId: 被融合装备id, 融合后神器属性会消失
--@Param targetId: 融合目的装备id, 融合后成为双属性神器
--@Return coinCost, itemCost
function UserEquipManager:getCompoundCost(sourceId, targetId)
	local s_starExp, s_starExp2 = self:getGodlyTotalExp(sourceId);	
	local t_starExp, t_starExp2 = self:getGodlyTotalExp(targetId);	
	
	local maxExp2 = math.max(s_starExp2,t_starExp2)
	
	local userEquip = self:getUserEquipById(targetId);
	local level = EquipManager:getLevelById(userEquip.equipId);
	local factor = EquipManager:getWeightByPart(EquipManager:getPartById(userEquip.equipId));
	
	local coinCost = 100 * maxExp2 * math.pow((level + factor), 2);
	local itemCost = maxExp2 * 1;
	return coinCost, itemCost;
end	

--获取装备被穿戴者的角色名字，主角为“主角”
function UserEquipManager:getDressedRoleName(userEquipId)
	local userEquip = self:getUserEquipById(userEquipId);
	if userEquip == nil then return ""; end
	
	local part = EquipManager:getPartById(userEquip.equipId);
	local roleEquip = UserInfo.getEquipByPart(part);
	if roleEquip and roleEquip.equipId == userEquipId then
		return common:getLanguageString("@MainRole");
	end
	
	return UserMercenaryManager:getEquipDressedBy(userEquipId);
end	

--获取神器特效ccbi，双属性神器Key为"Double", 第一属性单神器为"First", 第二属性为"Second"
function UserEquipManager:getGodlyAni(userEquipId, userEquip)
	local userEquip = userEquip or self:getUserEquipById(userEquipId);
	local aniKey = "First";
	if userEquip:HasField('godlyAttrId2') and userEquip.godlyAttrId2 > 0 then
		aniKey = userEquip.godlyAttrId > 0 and "Double" or "Second";
	end
	return GameConfig.GodlyEquipAni[aniKey];
end
--------------------------------------------------------------------------------
return UserEquipManager;