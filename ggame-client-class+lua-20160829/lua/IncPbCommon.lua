
GameConfig = require("GameConfig");
ConfigManager = require("ConfigManager");
PageManager = require("PageManager");
EquipManager = require("Equip.EquipManager");
require("Activity.ActivityInfo");

ViewPlayerInfo = require("PlayerInfo.ViewPlayerInfo");
UserEquipManager = require("Equip.UserEquipManager");
ResManagerForLua = require("ResManagerForLua");

function getRewardInfo(msg)
    if msg~=nil then
        local contentTable=getResTable(msg)
        if contentTable~=nil then
            local contentStr=""
            local j=1
            for i=1,table.maxn(contentTable) do
                local info=ResManager:getInstance():getResInfoByTypeAndId(contentTable[i].type,contentTable[i].itemId,contentTable[i].count)
                if j~=1 then
                    contentStr=contentStr..","
                end
                contentStr=contentStr..info.name.."x"..info.count
                j=j+1
            end
            return contentStr
        end
        return msg
    else
        return msg
    end
end

function autoReturn(s, width)
    local les = string.len(s)
    local ret = ""
    local count = 0
    for i=1,les do
        local v = string.byte(s,i)
        if bit:band(v,128)==0 then
            count = count + 0.5
            if(count>width)then
                ret = ret .. "\n"
                count = 0
            end
        end
        if bit:band(v,128)~=0 and bit:band(v,64)~=0 then
            count = count + 1
            if(count>width)then
                ret = ret .. "\n"
                count = 0
            end
        end
        ret = ret .. string.char(v)
    end
    return ret
end

function Split(str, delim, maxNb)
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
    return result
end
--分割字符串的函数
--[[
	用法:
	local list = SplitEx("abc,123,345", ",")
	然后list里面就是
	abc
	123
	345
	了。第二个参数可以是多个字符，但是不能是Lua正则表达式。例如. ，或者 %w 之类的。
	增强版等以后再放出来吧，这个应该大部分够用了。
--]]
function SplitEx(szFullString, szSeparator)  
	local nFindStartIndex = 1  
	local nSplitIndex = 1  
	local nSplitArray = {}  
	while true do  
	   local nFindLastIndex = string.find(szFullString, szSeparator, nFindStartIndex)  
	   if not nFindLastIndex then  
		nSplitArray[nSplitIndex] = string.sub(szFullString, nFindStartIndex, string.len(szFullString))  
		break  
	   end  
	   nSplitArray[nSplitIndex] = string.sub(szFullString, nFindStartIndex, nFindLastIndex - 1)  
	   nFindStartIndex = nFindLastIndex + string.len(szSeparator)  
	   nSplitIndex = nSplitIndex + 1  
	end  
	return nSplitArray  
end  

function getResTable(str)
    local  _tableItem=Split(str,",")
    local _tableRes={}
    if _tableItem==str then
       local _resItem=Split(str,"_")
       if _resItem==str then
           _resItem=nil
       else
           _tableRes[1]={}
            if table.maxn(_resItem)==3 then
                _tableRes[1].type=_resItem[1]
                _tableRes[1].itemId=_resItem[2]
                _tableRes[1].count=_resItem[3]
            else
                _tableRes[1]=nil
            end
       end
    else
        for i=1, table.maxn(_tableItem) do
            _tableRes[i]={}
            local _resItem=Split(_tableItem[i],"_")
            if _resItem==str then
                _tableRes[i]=nil
            else
                if table.maxn(_resItem)==3 then
                    _tableRes[i].type=_resItem[1]
                    _tableRes[i].itemId=_resItem[2]
                    _tableRes[i].count=_resItem[3]
                else
                    _tableRes[i]=nil
                end
            end
        end
    end
    return _tableRes
end

function getToolsCount(itemId)
    local _toolsInfo=ServerDateManager:getInstance():getUserToolInfoByItemId(itemId)
    local _count=0
    if _toolsInfo~=nil then
        _count=_toolsInfo.count
    end
    return _count
end	
-------------------------------------------------------------
QualityInfo =
{
	MaxQuality=5,
	MinQuality=1,
	NoQuality=4,
}

MemProfiler = { currMem = 0 }

function MemProfiler:start()
	self.currMem = collectgarbage("count")
	common:log("Current Lua Memory Size::  %d", self.currMem)
end

function MemProfiler:record()
	local mem = collectgarbage("count")
	common:log("Current Lua Memory Size::  %d ,Added Count::  %d", mem, mem - self.currMem)
	self.currMem = mem
end
