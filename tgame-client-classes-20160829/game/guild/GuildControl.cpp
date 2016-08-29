#include "GuildControl.h"

#include "basic/PopTipLayer.h"
#include "basic/CommonDialog.h"
#include "basic/TextHelper.h"

#include "vlib/base/CVTime.h"

#include "game/Global.h"
#include "game/UserObj.h"
#include "game/login/LoginControl.h" 

GuildControl::GuildControl()
:m_bInitialized(false)
,m_pGuildListPanel(NULL)
,m_pGuildCreatePanel(NULL)
,m_pGuildFindPanel(NULL)
,m_pGuildPanel(NULL)
,m_pGuildAdminPanel(NULL)
,m_pGuildMemberAdminPanel(NULL)
,m_pGuildJoinReplyPanel(NULL)
,m_uAppliedGuildCnt(0)
{
}

GuildControl::~GuildControl()
{
	finalize();
}

void GuildControl::init() {
	if(!m_bInitialized) {
		m_pGuildListPanel = GuildListPanel::create();
		CC_SAFE_RETAIN(m_pGuildListPanel);

		m_pGuildCreatePanel = GuildCreatePanel::create();
		CC_SAFE_RETAIN(m_pGuildCreatePanel);

		m_pGuildFindPanel = GuildFindPanel::create();
		CC_SAFE_RETAIN(m_pGuildFindPanel);

		m_pGuildPanel = GuildPanel::create();
		CC_SAFE_RETAIN(m_pGuildPanel);

		m_pGuildAdminPanel = GuildAdminPanel::create();
		CC_SAFE_RETAIN(m_pGuildAdminPanel);

		m_pGuildMemberAdminPanel = GuildMemberAdminPanel::create();
		CC_SAFE_RETAIN(m_pGuildMemberAdminPanel);

		m_pGuildJoinReplyPanel = GuildJoinReplyPanel::create();
		CC_SAFE_RETAIN(m_pGuildJoinReplyPanel);

		m_bInitialized = true;
	}
}

void GuildControl::finalize() {
	if(m_bInitialized) {
		m_bInitialized = false;

		CC_SAFE_RELEASE_NULL(m_pGuildListPanel);

		CC_SAFE_RELEASE_NULL(m_pGuildCreatePanel);

		CC_SAFE_RELEASE_NULL(m_pGuildFindPanel);

		CC_SAFE_RELEASE_NULL(m_pGuildPanel);

		CC_SAFE_RELEASE_NULL(m_pGuildAdminPanel);

		CC_SAFE_RELEASE_NULL(m_pGuildMemberAdminPanel);

		CC_SAFE_RELEASE_NULL(m_pGuildJoinReplyPanel);
	}
}

void GuildControl::openGuild() {
	if(P_USER_OBJ->getUserInfo()->guildID > 0) { // 已经是一个帮派的成员
		m_pGuildPanel->open();
	} else {
		m_pGuildListPanel->open();
	}

}

const vmsg::CSQueryGuildLstRsp& GuildControl::getGuildListInfo() {
	return m_stGuildObj.getGuildListInfo();
}

const vmsg::CSGuildBrief& GuildControl::getGuildSearchInfo() {
	return m_stGuildObj.getGuildSearchInfo();
}

const vmsg::CSGuildInfoRsp& GuildControl::getGuildInfo() {
	return m_stGuildObj.getGuildInfo();
}

const vmsg::CSOtherGuildRsp& GuildControl::getOtherGuildInfo() {
	return m_stGuildObj.getOtherGuildInfo();
}

const vmsg::CSAllGuildReqRsp& GuildControl::getAllGuildRqst() {
	return m_stGuildObj.getAllGuildRqst();
}

GuildListPanel* GuildControl::getGuildListPanel() {
	return m_pGuildListPanel;
}

GuildCreatePanel* GuildControl::getGuildCreatePanel() {
	return m_pGuildCreatePanel;
}

GuildFindPanel* GuildControl::getGuildFindPanel() {
	return m_pGuildFindPanel;
}

GuildPanel* GuildControl::getGuildPanel() {
	return m_pGuildPanel;
}

GuildAdminPanel* GuildControl::getGuildAdminPanel() {
	return m_pGuildAdminPanel;
}

GuildMemberAdminPanel* GuildControl::getGuildMemberAdminPanel() {
	return m_pGuildMemberAdminPanel;
}

GuildJoinReplyPanel* GuildControl::getGuildJoinReplyPanel() {
	return m_pGuildJoinReplyPanel;
}

unsigned int GuildControl::getMemberPrivilege(UIN_t uin) {
	unsigned int privilege = 0;
	const vmsg::CSGuildInfoRsp &stGuildInfo = m_stGuildObj.getGuildInfo();

	for(int i = 0; i < stGuildInfo.guildmemberlst_size(); i++) {
		const vmsg::CSGuildMemberBrief &stMemberBrief = stGuildInfo.guildmemberlst(i).brief();
		
		if(stMemberBrief.uin() == uin) {
			privilege = stMemberBrief.type();
			break;
		}
	}
	
	return privilege;
}

unsigned int GuildControl::getAppliedGuildCnt() {
	return m_uAppliedGuildCnt;
}

void GuildControl::setAppliedGuildCnt(unsigned int uAppliedGuildCnt) {
	m_uAppliedGuildCnt = uAppliedGuildCnt;
}

string GuildControl::getLastLoginText(unsigned int uLastLoginTime) {
	string text = "";
	unsigned int uSvrTime = P_GLOBAL->getSvrTime();

	if(CVTimeUtils::IsSameDay(uSvrTime, uLastLoginTime)) {
		text = TextStr(TEXT_TODAY);
	} else {
		CVTime tmSvrTime(uSvrTime);
		CVTime tmLastLoginTime(uLastLoginTime);
		
		if(tmSvrTime.GetYear() == tmLastLoginTime.GetYear()
		&& tmSvrTime.GetMonth() == tmLastLoginTime.GetMonth()) {
			char buf[100];
			int days = tmSvrTime.GetDay() - tmLastLoginTime.GetDay();

			if(days < 7) {
				text = snprintf(buf, 100, TextStr(TEXT_A_FEW_DAYS_AGO), days);
			} else {
				text = snprintf(buf, 100, TextStr(TEXT_A_FEW_DAYS_AGO), 7);
			}
		} else {
			text = TextStr(TEXT_A_MONTH_AGO);
		}
	}

	return text;
}

void GuildControl::onRecvMsg(vmsg::VLogicSvr2CliMsg &stMsg) {
	unsigned int uCmd = stMsg.msg_head().msg_id();

	switch(uCmd) {
		case vmsg::CS_CMD_ERR:
			if(stMsg.has_errmsg()) {
				unsigned int uRqstCmd = stMsg.errmsg().rqstcmdid();

			}
			break;
		case vmsg::CS_CMD_GUILD_QUERY_GUILDLST:
		case vmsg::CS_CMD_GUILD_SEARCH:
		case vmsg::CS_CMD_GUILD_CREATE:
		case vmsg::CS_CMD_GUILD_QUERY_GUILD:
		case vmsg::CS_CMD_GUILD_JOIN:
		case vmsg::CS_CMD_GUILD_REPLY_JOIN:
		case vmsg::CS_CMD_GUILD_ALL_GUILD_REQ:
		case vmsg::CS_CMD_GUILD_QUERY_MYREQ:
		case vmsg::CS_CMD_GUILD_DELETE_MYGUILDREQ:
		case vmsg::CS_CMD_GUILD_CHG_PRIVI:
		case vmsg::CS_CMD_GUILD_ASSIGN_CHAIRMAN:
		case vmsg::CS_CMD_GUILD_KICK:
        case vmsg::CS_CMD_GUILD_STAT_NOTIFY:
		case vmsg::CS_CMD_GUILD_IMPEACH:
			if(stMsg.has_guildrespond()) {
				const vmsg::CSGuildRespond &stGuildRsp = stMsg.guildrespond();

				if(stGuildRsp.has_queryguildlst_rsp()) { // 查询部落列表
					m_stGuildObj.setGuildListInfo(stGuildRsp.queryguildlst_rsp());

					getGuildListPanel()->updateGuildList();

				} else if(stGuildRsp.has_guildsearch_rsp()) { // 查找部落
					m_stGuildObj.setGuildSearchInfo(stGuildRsp.guildsearch_rsp().guildbrief());

					getGuildListPanel()->updateFindGuild();

				} else if(stGuildRsp.has_guildcreate_rsp()) { // 创建部落
					m_stGuildObj.setGuildInfo(stGuildRsp.guildcreate_rsp());
					getGuildListPanel()->close();
					getGuildPanel()->open();
					getGuildPanel()->updateGuildInfo();

					P_LOGIN_CTRL->getProxy()->sendQueryPlayerInfoRqst(P_USER_OBJ->getUserInfo()->uin);
				} else if(stGuildRsp.has_guildinfo_rsp()) { // 查询我的部落信息
                    P_USER_OBJ->getMutableUserInfo()->guildID = stGuildRsp.guildinfo_rsp().guildbrief().guildid();
                    CCLOG("[trace]GuildControl::onRecvMsg, update guild id %llu", stGuildRsp.guildinfo_rsp().guildbrief().guildid() );
					m_stGuildObj.setGuildInfo(stGuildRsp.guildinfo_rsp());
					getGuildPanel()->updateGuildInfo();

					if(m_pGuildAdminPanel != NULL && m_pGuildAdminPanel->isOpen()) {
						m_pGuildAdminPanel->updateUI();
					}

				} else if(stGuildRsp.has_otherguild_rsp()) { // 查询其他的部落信息
					m_stGuildObj.setOtherGuildInfo(stGuildRsp.otherguild_rsp());

				} /*else if(stGuildRsp.has_guildinvite_rsp()) { // 邀请加入部落
					stGuildRsp.guildinvite_rsp(); 
				} */else if(stGuildRsp.has_guildjoin_rsp()) { // 申请加入部落
					stGuildRsp.guildjoin_rsp();
					sendQueryGuildListRqst();
				} else if(stGuildRsp.has_guildreplyjoin_rsp()) { // 审批加入请求
					stGuildRsp.guildreplyjoin_rsp();
					sendGuildQueryRqst();
				} else if(stGuildRsp.has_myguildreq_rsp()) { // 查询我的所有请求
					m_stGuildObj.setMyGuildRqst(stGuildRsp.myguildreq_rsp());

				} else if(stGuildRsp.has_allguildreq_rsp()) { // 查询部落的所有申请
					m_stGuildObj.setAllGuildRqst(stGuildRsp.allguildreq_rsp());

					if(m_pGuildJoinReplyPanel != NULL && m_pGuildJoinReplyPanel->isOpen()) {
						m_pGuildJoinReplyPanel->updateUI();
					}
				} else if(stGuildRsp.has_deletemyguildreq_rsp()) { // 取消申请
					stGuildRsp.deletemyguildreq_rsp();
					sendQueryGuildListRqst();
				} else if(stGuildRsp.has_guildchgprivi_rsp()) { // 变更职位
					stGuildRsp.guildchgprivi_rsp();
					sendGuildQueryRqst();
				} else if(stGuildRsp.has_assignchairman_rsp()) { // 转让部落
					stGuildRsp.assignchairman_rsp();
					sendGuildQueryRqst();
				} else if(stGuildRsp.has_guildkick_rsp()) { // 踢出成员
					stGuildRsp.guildkick_rsp();
					sendGuildQueryRqst();
				} else if(stGuildRsp.has_impeach_rsp()) { // 弹劾
					if(stGuildRsp.impeach_rsp() == 0) { // 弹劾成功
						sendGuildQueryRqst();
					}
				} else if(stGuildRsp.has_guildstatnotify_rsp()) {
                    P_USER_OBJ->getMutableUserInfo()->guildID = stGuildRsp.guildstatnotify_rsp().guildid();
                    const int iReason = stGuildRsp.guildstatnotify_rsp().reason();
                    CCLOG("[trace]GuildControl::onRecvMsg, update guild id %llu，reason %d", 
                        stGuildRsp.guildstatnotify_rsp().guildid(), iReason);

                    //show tips
                    if(vmsg::GUILD_NOTIFY_APPLY_JOIN == iReason) {
                        getGuildListPanel()->close();
                        COMMON_DIALOG->show(TextStr(TEXT_GUILD_NOTIFY_JOIN));
                    } else if(vmsg::GUILD_NOTIFY_DISMISS == iReason) {
                        if(m_bInitialized) {
							m_pGuildPanel->close();
							m_pGuildAdminPanel->close();
							m_pGuildMemberAdminPanel->close();
							m_pGuildJoinReplyPanel->close();
						}

                        COMMON_DIALOG->show(TextStr(TEXT_GUILD_NOTIFY_DISMISS));
                    } else if(vmsg::GUILD_NOTIFY_KICK == iReason) {
						if(m_bInitialized) {
							m_pGuildPanel->close();
							m_pGuildAdminPanel->close();
							m_pGuildMemberAdminPanel->close();
							m_pGuildJoinReplyPanel->close();
						}

                        COMMON_DIALOG->show(TextStr(TEXT_GUILD_NOTIFY_KICK));
                    }
                }
			}
			break;
		case vmsg::CS_CMD_GUILD_DISMISS:
			P_USER_OBJ->getMutableUserInfo()->guildID = 0;
			if(m_bInitialized) {
				m_pGuildPanel->close();
			}

            break;
		case vmsg::CS_CMD_GUILD_LEAVE:
			P_USER_OBJ->getMutableUserInfo()->guildID = 0;
			if(m_bInitialized) {
				m_pGuildPanel->close();
			}

			break;
		case vmsg::CS_CMD_GUILD_CHANGE_NOTICE:
			
			break;
		default:;
	}

}

void GuildControl::sendQueryGuildListRqst() {
	m_stGuildProxy.sendQueryGuildListRqst();
}

void GuildControl::sendGuildSearchRqst(const std::string& strName) {
	m_stGuildProxy.sendGuildSearchRqst(strName);
}

void GuildControl::sendGuildCreateRqst(const std::string& strName) {
	m_stGuildProxy.sendGuildCreateRqst(strName);
}

void GuildControl::sendGuildQueryRqst() {
	m_stGuildProxy.sendGuildQueryRqst();
}

void GuildControl::sendQueryOtherGuildRqst(const UNIT64_t uGuildGUID) {
	m_stGuildProxy.sendQueryOtherGuildRqst(uGuildGUID);
}

void GuildControl::sendGuildJoinRqst(const UNIT64_t uGuildGUID) {
	m_stGuildProxy.sendGuildJoinRqst(uGuildGUID);
}

void GuildControl::sendGuildJoinReplyRqst(const vector<UIN_t> &stPlayerLst, bool isAgree) {
	m_stGuildProxy.sendGuildJoinReplyRqst(stPlayerLst, isAgree);
}

void GuildControl::sendGuildQueryMyJoinRqst() {
	m_stGuildProxy.sendGuildQueryMyJoinRqst();
}

void GuildControl::sendGuildQueryJoinListRqst() {
	m_stGuildProxy.sendGuildQueryJoinListRqst();
}

void GuildControl::sendGuildDelMyJoinRqst(const UNIT64_t uGuildGUID) {
	m_stGuildProxy.sendGuildDelMyJoinRqst(uGuildGUID);
}

void GuildControl::sendGuildChgNoticeRqst(const std::string& strNocice) {
	m_stGuildProxy.sendGuildChgNoticeRqst(strNocice);
}

void GuildControl::sendGuildChgPriviRqst(const UIN_t uUin, const int nNewPrive) {
	m_stGuildProxy.sendGuildChgPriviRqst(uUin, nNewPrive);
}

void GuildControl::sendGuildAssignChairmanRqst(const UIN_t uUin) {
	m_stGuildProxy.sendGuildAssignChairmanRqst(uUin);
}

void GuildControl::sendGuildKickRqst(const UIN_t uUin) {
	m_stGuildProxy.sendGuildKickRqst(uUin);
}

void GuildControl::sendGuildImpeachRqst() {
	m_stGuildProxy.sendGuildImpeachRqst();
}

void GuildControl::sendGuildLeaveRqst() {
	m_stGuildProxy.sendGuildLeaveRqst();
}

void GuildControl::sendGuildDismissRqst() {
	m_stGuildProxy.sendGuildDismissRqst();
}
