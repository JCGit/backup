----------------------------------------------------------------------------------
--[[
	FILE:			CommonPage.lua
	ENCODING:		UTF-8, no-bomb
	DESCRIPTION:	luaҳ�����ݽӿڻ���
	AUTHOR:			zhoutong
	CREATED:		2014-10-30
	
--]]
----------------------------------------------------------------------------------

local BaseDataHelper = {}

function BaseDataHelper:new(config)
    local new = {}
    self.__index = self
    setmetatable(new,self)
    new.super = self
    new.config = config
    return new
end
--�հ�
function BaseDataHelper:onReceivePacket(container,page)
end
--���ȫ�ֱ���
function BaseDataHelper:getVariableByKey(key)
    if self[key]~=nil then
        return self[key]
    end
    return nil
end
--��ñ���ĳ������
function BaseDataHelper:getVariableByKeyAndIndex(key,index)
    if self[key]~=nil and self[key][index]~=nil then
        return self[key][index]
    end
    return nil
end
--�޸�����
function BaseDataHelper:addOrSetVariableByKey(key,value)
    self[key] = value
end

function BaseDataHelper:removeVariableByKey(key,value)
	if self[key]~=nil then
		table.remove(self[key] , value)
	end
end
----------------------------------------------------------------
--������ñ���Ϣ
function BaseDataHelper:getConfigDataByKey(key)
    return self:getVariableByKeyAndIndex("config",key)
end

function BaseDataHelper:getConfigDataByKeyAndIndex(key,index)
    local configData = self:getConfigDataByKey(key)
	if configData~=nil then
		return configData[index]
	end
	return nil
end

return BaseDataHelper