local CampWar_pb = require("CampWar_pb")
local UserInfo = require("UserInfo");
local CampWarManager = {}

--阵营战主页面状态相关 
CampWarManager.state = CampWar_pb.CAMP_WAR_CLOSED 
CampWarManager.leftTime  = -1 
CampWarManager.isAuto = 0
CampWarManager.isJoin = -1

--只有在战斗中会变化，否则在其他状态，为空
CampWarManager.campReportList = {}

--[[// 阵营战信息同步
message HPCampWarInfoSyncS
{
	// 所属阵营
	required int32 campId = 1;
	// 阵营战剩余时间
	required int32 campRemainTime = 2;
	// 已鼓舞次数
	required int32 inspireTimes = 3;
	// 阵营信息
	repeated CampInfo campInfo = 4;
	// 连杀排行信息
	repeated MultiKillRankInfo rankInfo = 5;
	// 战报信息增量
	repeated CampReportInfo campReportInfo = 6;
	// 个人状态
	required PersonalCampWarInfo personalState = 7;
}--]]
--阵营战战斗状态相关
CampWarManager.m_curCampFightInfo = {}

CampWarManager.m_showAllReport = true

CampWarManager.m_lastCampWarInfo = {}

function CampWarManager.EnterPageByState()
	--根据当前的状态，判断进入哪个页面	
	if UserInfo.roleInfo.fight < GameConfig.FightLimit.CampWarLimit then
		MessageBoxPage:Msg_Box_Lan("@CampWarLimitFightValue")
		return
	end	
	if CampWarManager.isJoin<0 then
		MessageBoxPage:Msg_Box_Lan("@CommingSoon");
		return 
	end
	
	--如果已经加入，进入战斗页面，否则，进入主页面
	if CampWarManager.isJoin == 1  then
		PageManager.changePage("CampFightPage");
	else
		PageManager.changePage("CampMainPage");
	end
end

function CampWarManager.isShowCampRedPoint()
	--还没有加入,没有自动挂机，战力大于设置，显示红点
	if CampWarManager.isJoin == 0 
		and  CampWarManager.isAuto == 0 
		and UserInfo.roleInfo.fight > GameConfig.FightLimit.CampWarLimit 
		and  (CampWarManager.state == CampWar_pb.CAMP_WAR_PREPARE or CampWarManager.state == CampWar_pb.CAMP_WAR_FIGHT )
		then
		return true
	else
		return false
	end
end


function CampWarManager.getKillRankInfoByRank( rank )
	if rank == 1 or rank ==2 or rank ==3  then
		for i=1,#CampWarManager.m_curCampFightInfo.rankInfo do
			if CampWarManager.m_curCampFightInfo.rankInfo[i].rank == rank then
				return CampWarManager.m_curCampFightInfo.rankInfo[i]
			end
		end
		return nil
	else
		return nil
	end
end

function CampWarManager.getCurrentPointByCampId( id )
	if CampWarManager.m_curCampFightInfo~=nil and CampWarManager.m_curCampFightInfo.campInfo~=nil then
		for i=1,#CampWarManager.m_curCampFightInfo.campInfo do
			if CampWarManager.m_curCampFightInfo.campInfo[i].campId == id then
				return CampWarManager.m_curCampFightInfo.campInfo[i].totalScore
			end
		end
	end
	return 0
end

function CampWarManager.RecieveCampWarInFightState(msg)
	if msg == nil  then
		return 
	end						
	if #msg.campInfo  ~=2  then
		CCLuaLog("error in #msg.campInfo  ~=2 ")
		return 		
	end	
	CampWarManager.m_curCampFightInfo = msg
		
	--增量插入战报相关信息
	local reportSize = #CampWarManager.m_curCampFightInfo.campReportInfo
	local maxSize = GameConfig.CampWar.reportMax;
	for i=1,reportSize do	
		local oneMsg = CampWarManager.m_curCampFightInfo.campReportInfo[i]
		if oneMsg~=nil then
			if #CampWarManager.campReportList>maxSize then
				table.remove(CampWarManager.campReportList,1);
			end
			table.insert(CampWarManager.campReportList,oneMsg);
		end			
	end
end	

function CampWarManager.generateHtmlReport(report)
	if report.reportType == CampWar_pb.CONTINUE_WIN_TYPE then
		local templateStr = FightLogConfig['CampReportContinueBlue'].content
		if report.campId == 1 then
			templateStr = FightLogConfig['CampReportContinueBlue'].content			
		else
			templateStr = FightLogConfig['CampReportContinueRed'].content			
		end
		if report.argsNum >0 then
			templateStr = common:fill(templateStr,unpack(report.argString))
			--common:fill(templateStr,report.argString[1],report.argString[2],report.argString[3],report.argString[4],report.argString[5])
			local htmlText = CCHTMLLabel:createWithString(templateStr,CCSize(570,200));
			return htmlText
		else 
			CCLuaLog("Error in report.argsNum >0")
			return nil
		end
	elseif report.reportType == CampWar_pb.END_WIN_TYPE then
		local templateStr = FightLogConfig['CampReportEndWinBlue'].content
		if report.campId == 1 then
			templateStr = FightLogConfig['CampReportEndWinBlue'].content			
		else
			templateStr = FightLogConfig['CampReportEndWinRed'].content			
		end
		if report.argsNum > 0  then
			templateStr = common:fill(templateStr,unpack(report.argString))
			--common:fill(templateStr,report.argString[1],report.argString[2],report.argString[3],report.argString[4],report.argString[5])
			local htmlText = CCHTMLLabel:createWithString(templateStr,CCSize(570,200));
			return htmlText
		else 
			CCLuaLog("Error in report.argsNum == 5")
			return nil
		end
	elseif report.reportType == CampWar_pb.PERSONAL_WIN_TYPE then
		local templateStr = FightLogConfig['CampReportPersonalWin'].content
		if report.argsNum >0 then
			templateStr = common:fill(templateStr,unpack(report.argString))
			--common:fill(templateStr,report.argString[1],report.argString[2],report.argString[3],report.argString[4],report.argString[5])
			local htmlText = CCHTMLLabel:createWithString(templateStr,CCSize(570,200));
			return htmlText
		else 
			CCLuaLog("Error in report.argsNum == 5")
			return nil
		end
	elseif report.reportType == CampWar_pb.PERSONAL_LOSE_TYPE then
		local templateStr = FightLogConfig['CampReportPersonalLose'].content
		if report.argsNum >0  then
			templateStr = common:fill(templateStr,unpack(report.argString))
			--common:fill(templateStr,report.argString[1],report.argString[2])
			local htmlText = CCHTMLLabel:createWithString(templateStr,CCSize(570,200));
			return htmlText
		else 
			CCLuaLog("Error in report.argsNum == 2")
			return nil
		end
	end
end

function CampWarManager.RecieveCampLastRankInfo(msg)
	CampWarManager.m_lastCampWarInfo = msg
end

function CampWarManager.RecieveCampWarMainState(msg)
	if msg.mainInfo ~=nil then
		CampWarManager.state = msg.mainInfo.state
		CampWarManager.leftTime = msg.mainInfo.leftTime
		CampWarManager.isAuto = msg.mainInfo.isAuto
		CampWarManager.isJoin = msg.mainInfo.isJoin	
		
		PageManager.refreshPage("CampMainPage","SyncState");
	end		
end

return CampWarManager