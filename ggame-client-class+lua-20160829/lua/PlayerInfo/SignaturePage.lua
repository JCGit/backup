----------------------------------------------------------------------------------
--[[
	FILE:			SignaturePageBase.lua
	ENCODING:		UTF-8, no-bomb
	DESCRIPTION:	SignaturePageBase
	AUTHOR:			zhaozhen
	CREATED:		2014-08-08
--]]
----------------------------------------------------------------------------------
--require "ExploreEnergyCore_pb"
local HP = require("HP_pb");
local GameConfig = require("GameConfig");
local player_pb = require("Player_pb");
local UserInfo = require("UserInfo");
local NodeHelper = require("NodeHelper");
local thisPageName = "SignaturePage"
local SignaturePageBase = {};
local SIGNATURE_VIEW_COUNT = 15;

local opcodes = {
	ROLE_CHANGE_SIGNATURE_C = HP.ROLE_CHANGE_SIGNATURE_C;
	ROLE_CHANGE_SIGNATURE_S = HP.ROLE_CHANGE_SIGNATURE_S;
}

local singnatureLenLimit = 20;
local signatureStr = "";

local option = {
	ccbiFile = "ChangeSignaturePopUp.ccbi",
	handlerMap = {
		onContentBtn = "showInputBox",
		onConfirmation = "onChangeSignature",
		onCancel = "onClose",
		onHelp = "onHelp"
	},
	opcode = opcodes
};

--------------------------------------------------------------
--显示签名输入框
function SignaturePageBase:showInputBox( container )
	container:registerLibOS();
	Flag_Chat = 1
	libOS:getInstance():showInputbox(false);
end

----------------------------------------------
function SignaturePageBase:onEnter(container)
	self:registerPacket(container)
	--container:registerMessage(MSG_SEVERINFO_UPDATE)
	signatureStr = UserInfo.playerInfo.signature;
	self:refreshPage(container);
end

function SignaturePageBase:onExecute(container)
end

function SignaturePageBase:onExit(container)
	container:removeLibOS();
	self:removePacket(container)
	--container:removeMessage(MSG_SEVERINFO_UPDATE)	
end
----------------------------------------------------------------

function SignaturePageBase:refreshPage(container)
	self:changeSignatureInfo(container);
end

--根据输入信息，刷新签名页面
function SignaturePageBase:changeSignatureInfo(container)

	local str = "";
	local colorStr = "";
	if string.len(signatureStr) > 0 then
		str = GameMaths:getStringSubCharacters(signatureStr,0,SIGNATURE_VIEW_COUNT) .. "..."
		colorStr = GameConfig.ColorMap.COLOR_WHITE;
	else
		str = Language:getInstance():getString("@SignatureInput");
		colorStr = GameConfig.ColorMap.COLOR_GREEN;
	end

	NodeHelper:setColorForLabel( container, {mLabelTex = colorStr} );
	NodeHelper:setStringForLabel( container, {mLabelTex = str} );
end

----------------click event------------------------
function SignaturePageBase:onClose(container)
	PageManager.popPage(thisPageName);
end

--检查是否符合签名条件，若符合请求服务端修改签名
function SignaturePageBase:onChangeSignature(container)
	local length = GameMaths:calculateStringCharacters(signatureStr);
	if  length > singnatureLenLimit then 

		MessageBoxPage:Msg_Box_Lan("@SignatureCountLimit");
		return;
	end

	if string.len(signatureStr) > 0 then
		local msg = player_pb.HPChangeSignature();
		msg.signature = signatureStr;
		local pb_data = msg:SerializeToString();
		PacketManager:getInstance():sendPakcet(opcodes.ROLE_CHANGE_SIGNATURE_C, pb_data, #pb_data, true);
	else
		MessageBoxPage:Msg_Box("@SignatureISNull");
	end
end




function SignaturePageBase:onReceivePacket(container)
	local opcode = container:getRecPacketOpcode()
	local msgBuff = container:getRecPacketBuffer()

	if opcode == opcodes.ROLE_CHANGE_SIGNATURE_S then
		PageManager.refreshPage("PlayerInfoPage");
		self:onClose();
	end

end

--输入框输入完成回掉，刷新签名信息
function SignaturePageBase:onInputboxEnter(container)
	 
	local content = container:getInputboxContent();
	local nameOK = true
    if GameMaths:isStringHasUTF8mb4(content) then
		nameOK = false
    end
	--[[
	if not RestrictedWord:getInstance():isStringOK(content) then
		nameOK = false
	end
	--]]
    if content == "" then
		nameOK = false
    end
	if not nameOK then
        MessageBoxPage:Msg_Box("@NameHaveForbbidenChar")
        content = nil
        return
    end
	
	signatureStr = RestrictedWord:getInstance():filterWordSentence(content);
	local contentLabel = container:getVarLabelTTF("mLabelTex");
	if contentLabel ~= nil then
		local length = GameMaths:calculateStringCharacters(signatureStr);
		if  length > singnatureLenLimit then 

			MessageBoxPage:Msg_Box_Lan("@SignatureCountLimit");
			signatureStr = "";
			return;
		end
		--signatureStr = content;
		local viewStr = signatureStr;
		if length > SIGNATURE_VIEW_COUNT then
			viewStr = GameMaths:getStringSubCharacters(viewStr,0,SIGNATURE_VIEW_COUNT) .. "..."
		end

		contentLabel:setString(tostring(viewStr));
	end
end

function SignaturePageBase:registerPacket(container)
	for key, opcode in pairs(opcodes) do
		if string.sub(key, -1) == "S" then
			container:registerPacket(opcode)
		end
	end
end

function SignaturePageBase:onHelp( container )
	PageManager.showHelp(GameConfig.HelpKey.SIGNATURE_PAGE_HELP);
end

function SignaturePageBase:removePacket(container)
	for key, opcode in pairs(opcodes) do
		if string.sub(key, -1) == "S" then
			container:removePacket(opcode)
		end
	end
end

-------------------------------------------------------------------------
local CommonPage = require("CommonPage");
local SignaturePage = CommonPage.newSub(SignaturePageBase, thisPageName, option);