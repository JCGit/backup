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

	void sendQueryGuildListRqst(); // ��ѯ�����б�

	void sendGuildSearchRqst(const std::string& strName); // ���Ҳ���

	void sendGuildCreateRqst(const std::string& strName); // ��������

	void sendGuildQueryRqst(); // ��ѯ����

	void sendQueryOtherGuildRqst(const UNIT64_t uGuildGUID); // ��ѯ����������Ϣ

	void sendGuildJoinRqst(const UNIT64_t uGuildGUID); // ������벿��

	void sendGuildJoinReplyRqst(const vector<UIN_t> &stPlayerLst, bool isAgree); // ������������

	void sendGuildQueryMyJoinRqst(); // ��ѯ�ҵ���������

	void sendGuildQueryJoinListRqst(); // ��ѯ�������������

	void sendGuildDelMyJoinRqst(const UNIT64_t uGuildGUID); // ȡ������

	void sendGuildChgNoticeRqst(const std::string& strNocice); // �޸Ĳ��乫��

	void sendGuildChgPriviRqst(const UIN_t uUin, const int nNewPrive); // ���ְλ

	void sendGuildAssignChairmanRqst(const UIN_t uUin); // ת�ò���

	void sendGuildKickRqst(const UIN_t uUin); // �߳���Ա

	void sendGuildImpeachRqst(); // ����

	void sendGuildLeaveRqst(); // �˳�����

	void sendGuildDismissRqst(); // ��ɢ����

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

	unsigned int m_uAppliedGuildCnt; // �����벿������

};

#define P_GUILD_CTRL CVLib::CVSingleton<GuildControl>::Instance()

#endif // GuildControl_h__
