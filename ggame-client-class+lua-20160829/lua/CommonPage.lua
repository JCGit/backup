----------------------------------------------------------------------------------
--[[
	FILE:			CommonPage.lua
	ENCODING:		UTF-8, no-bomb
	DESCRIPTION:	提供页面通用lua事件注册"类",通过new来创建页面，生成全局函数luaCreat_Page
					(Page为变量),提供通用事件处理函数,可覆盖定义;
					或者通过newSub函数，可实现"继承"
	AUTHOR:			hgs
	CREATED:		2013-09-03
	
	MODIFY 1:		hgs at 2013-12-09
					add notes
--]]
----------------------------------------------------------------------------------
require("CocoDebug")

local CommonPage = {};



local function addPacketFunction(page, tb_opcode)
	if page == nil or type(page) ~= "table" then return end

	page.registerPacket = function(container)
		for key, opcode in pairs(tb_opcode) do
			if string.sub(key, -1) == "S" then
				container:registerPacket(opcode)
			end
		end
	end

	page.removePacket = function(container)
		for key, opcode in pairs(tb_opcode) do
			if string.sub(key, -1) == "S" then
				container:removePacket(opcode)
			end
		end
	end
end

CommonPage.new = function(pageName, opt)
	opt = opt or {}
	local page = {}
	page.handlerMap = opt.handlerMap or {}
	page.handlerMap = common:table_merge(page.handlerMap, CommonPage.handlerMap)
	local showLog = opt.showLog == true
	page.onFunction = function(eventName, container)
		if page.handlerMap[eventName] ~= nil then
			local funcName = page.handlerMap[eventName]			
			xpcall(function()			
			  if page[funcName] then
				page[funcName]( container, eventName)
			 end
			end,CocoLog)
			
		else
			CCLuaLog("error===>unExpected event Name : " .. pageName .. "->" .. eventName)
		end
	end
	table.foreach(page.handlerMap, function(eventName, funcName)
		page[funcName] = function(container)
			if showLog then CCLuaLog(pageName .. "   " .. funcName .. "  called!") end
			if funcName == "onLoad" and opt.ccbiFile ~= nil then
				container:loadCcbiFile(opt.ccbiFile)
			end
		end
	end)

	_G["luaCreat_" .. pageName] = function(container)
		CCLuaLog("OnCreate__" .. pageName)
		container:registerFunctionHandler(page.onFunction)
	end

	if opt.opcode then
		addPacketFunction(page, opt.opcode)
	end

	return page
end

CommonPage.handlerMap = {
	luaInit = "onInit",
	luaLoad = "onLoad",
	luaUnLoad = "onUnload",
	luaExecute = "onExecute",
	luaEnter = "onEnter",
	luaExit = "onExit",
	luaOnAnimationDone = "onAnimationDone",
	luaReceivePacket = "onReceivePacket",
	luaGameMessage = "onReceiveMessage",
	luaInputboxEnter = "onInputboxEnter",
	luaSendPacketFailed = "onPacketError",
	luaConnectFailed = "onPacketError",
	luaTimeout = "onPacketError",
	luaPacketError = "onPacketError"
}

CommonPage.newSub = function (parent, pageName, option, func)
	local page = {}
	setmetatable(page, {__index = parent})

	if option == nil then 
        return page 
    end

	page.handlerMap = common:table_merge(option.handlerMap, CommonPage.handlerMap)

	local showLog = option.showLog == true

	page.onFunction = function(eventName, container)

		if page.handlerMap[eventName] ~= nil then
			local funcName = page.handlerMap[eventName]
			xpcall(function()
			  if page[funcName] then
				page[funcName](page, container, eventName)
			  end
			end,CocoLog)

		else
			CCLuaLog("error===>unExpected event Name : " .. pageName .. "->" .. eventName)
		end
		if func~=nil then
		    func(eventName,container)
		end
	end

	table.foreach(page.handlerMap, function(eventName, funcName)
		if not page[funcName] then
			page[funcName] = function(pageSelf, container)
				if showLog then CCLuaLog(pageName .. "   " .. funcName .. "  called!") end
				if funcName == "onLoad" and option.ccbiFile ~= nil then
					container:loadCcbiFile(option.ccbiFile)
				end
			end
		end
	end)
	
	

	_G["luaCreat_" .. pageName] = function(container)
		CCLuaLog("OnCreate__" .. pageName)
		container:registerFunctionHandler(page.onFunction)
	end

	return page
end

-------------------------------------------------------------
return CommonPage;