#ifndef GuildControl_h__
#define GuildControl_h__

#include "vlib/base/CVSingleton.h"

#include "GuildProxy.h"
#include "GuildObj.h"
#include "GuildListPanel.h"
#include "GuildCreatePanel.h"
#include "GuildFindPanel.h"
#include "GuildPanel.h"
#include "GuildAdminPanel.h"
#include "GuildMemberAdminPanel.h"
#include "GuildJoinReplyPanel.h"

class GuildControl
{
public:
	GuildControl();
	virtual ~GuildControl();

	void init();

	void finalize();

	void openGuild();

	const vmsg::CSQueryGuildLstRsp& getGuildListInfo();

	const vmsg::CSGuildBrief& getGuildSearchInfo();

	const vmsg::CSGuildInfoRsp& getGuildInfo();

	const vmsg::CSOtherGuildRsp& getOtherGuildInfo();

	const vmsg::CSAllGuildReqRsp& getAllGuildRqst();

	GuildListPanel* getGuildListPanel();

	GuildCreatePanel* getGuildCreatePanel();

	GuildFindPanel* getGuildFindPanel();

	GuildPanel* getGuildPanel();

	GuildAdminPanel* getGuildAdminPanel();

	GuildMemberAdminPanel* getGuildMemberAdminPanel();

	GuildJoinReplyPanel* getGuildJoinReplyPanel();

	unsigned int getMemberPrivilege(UIN_t uin);

	unsigned int getAppliedGuildCnt();

	void setAppliedGuildCnt(unsigned int uAppliedGuildCnt);

	string getLastLoginText(unsigned int uLastLoginTime);

	void onRecvMsg(vmsg::VLogicSvr2CliMsg &stMsg);

	void sendQueryGuildListRqst(); // 查询部落列表

	void sendGuildSearchRqst(const std::string& strName); // 查找部落

	void sendGuildCreateRqst(const std::string& strName); // 创建部落

	void sendGuildQueryRqst(); // 查询部落

	void sendQueryOtherGuildRqst(const UNIT64_t uGuildGUID); // 查询其他部落信息

	void sendGuildJoinRqst(const UNIT64_t uGuildGUID); // 申请加入部落

	void sendGuildJoinReplyRqst(const vector<UIN_t> &stPlayerLst, bool isAgree); // 审批加入请求

	void sendGuildQueryMyJoinRqst(); // 查询我的所有请求

	void sendGuildQueryJoinListRqst(); // 查询部落的所有请求

	void sendGuildDelMyJoinRqst(const UNIT64_t uGuildGUID); // 取消申请

	void sendGuildChgNoticeRqst(const std::string& strNocice); // 修改部落公告

	void sendGuildChgPriviRqst(const UIN_t uUin, const int nNewPrive); // 变更职位

	void sendGuildAssignChairmanRqst(const UIN_t uUin); // 转让部落

	void sendGuildKickRqst(const UIN_t uUin); // 踢出成员

	void sendGuildImpeachRqst(); // 弹劾

	void sendGuildLeaveRqst(); // 退出部落

	void sendGuildDismissRqst(); // 解散部落

private:
	bool m_bInitialized;

	GuildProxy m_stGuildProxy;

	GuildObj m_stGuildObj;

	GuildListPanel *m_pGuildListPanel;

	GuildCreatePanel *m_pGuildCreatePanel;

	GuildFindPanel *m_pGuildFindPanel;

	GuildPanel *m_pGuildPanel;

	GuildAdminPanel *m_pGuildAdminPanel;

	GuildMemberAdminPanel *m_pGuildMemberAdminPanel;

	GuildJoinReplyPanel *m_pGuildJoinReplyPanel;

	unsigned int m_uAppliedGuildCnt; // 已申请部落数量

};

#define P_GUILD_CTRL CVLib::CVSingleton<GuildControl>::Instance()

#endif // GuildControl_h__
