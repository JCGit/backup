----------------------------------------------------------------------------------
--[[
	FILE:			ClientSettingManager.lua
	ENCODING:		UTF-8, no-bomb
	DESCRIPTION:	���ܷ�������������ClientSetting��������ĳЩ���⴦����appstore��˱�־λ��shader ������
					�Ż���صĿ��ء�����޸ķ����������ļ�"clientSetting.xml"�����ļ�
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