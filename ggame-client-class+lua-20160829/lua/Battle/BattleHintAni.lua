--------------------------------------------------------------------------------
--  FILE:			BattleHintAni.lua
--  DESCRIPTION:	
--
--
--  AUTHOR:		zhenhui 
--  CREATED:	2014-08-4
--------------------------------------------------------------------------------
local BattleHintAni = {}


function luaCreat_BattleHintAni(container)
	container:registerFunctionHandler(BattleHintAni.onFunction)
end	

function BattleHintAni.onFunction(eventName,container)
	if eventName == "luaInit" then
		BattleHintAni.onInit(container)
	elseif eventName == "luaEnter" then
		BattleHintAni.onEnter(container)
	elseif eventName == "luaExit" then
		BattleHintAni.onExit(container)
	elseif eventName == "luaExecute" then
		BattleHintAni.onExecute(container)
	elseif eventName == "luaLoad" then
		BattleHintAni.onLoad(container)
	elseif eventName == "luaUnLoad" then
		BattleHintAni.onUnLoad(container)	
	elseif eventName == "luaGameMessage" then
		BattleHintAni.onGameMessage(container)
	elseif eventName == "luaReceivePacket" then
		BattleHintAni.onReceivePacket(container)		
	elseif eventName =="luaOnAnimationDone" then
		BattleHintAni.onAnimationDone(container)		
	end
end



function BattleHintAni.onAnimationDone(container)
	local animationName=tostring(container:getCurAnimationDoneName())
	if animationName=="win" or animationName=="lose" then
		BattleFightHelper:EnterState(container,FightState.Waiting);
	end
end

function BattleHintAni.onLoad(container)
	
end


function BattleHintAni.onInit(container)
	container:loadCcbiFile("BattleHintAni.ccbi",false)
end

function BattleHintAni.onEnter(container)
	
end


function BattleHintAni.onExit(container)
	
end


function BattleHintAni.onExecute(container)
		
end

function BattleHintAni.onUnLoad(container)
end

function BattleHintAni.onGameMessage(container)
	
end



