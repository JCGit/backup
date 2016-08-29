----------------------------------------------------------------------------------
--[[
	FILE:			ClientSettingManager.lua
	ENCODING:		UTF-8, no-bomb
	DESCRIPTION:	接受服务器传过来的ClientSetting，用来做某些特殊处理，如appstore审核标志位，shader 开启，
					优化相关的开关。配合修改服务器配置文件"clientSetting.xml"配置文件
	AUTHOR:			zhenhui
	CREATED:		2014/11/4
	
	MODIFY 1:		zhenhui at 2014/11/4
					add notes
--]]

local ClientSettingManager = {}

ClientSettingManager.params = {}


--return found flag and data 
function ClientSettingManager:findAndGetValueByKey(key)
	if ClientSettingManager.params[key] ~= nil then 
		return true,ClientSettingManager.params[key]
	else
		return false,nil
	end
end

function ClientSettingManager:onReceivePacket(msg)
	ClientSettingManager.params = msg.params
end


return ClientSettingManager 