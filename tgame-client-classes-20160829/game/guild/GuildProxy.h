#ifndef GuildProxy_h__
#define GuildProxy_h__

#include "basic/QBaseProxy.h"
#include "basic/BasicTypes.h"

class GuildProxy: public QBaseProxy
{
public:
	GuildProxy();
	virtual ~GuildProxy();

	// override start

    virtual void DoRsp(vmsg::VLogicSvr2CliMsg& stRsp);
    virtual void fault(const vmsg::CSCommonErrMsg &err);

	// override end

	void sendQueryGuildListRqst(); // ��ѯ�����б�

	void sendGuildSearchRqst(const std::string& strName); // ���Ҳ���

	void sendGuildCreateRqst(const std::string& strName); // ��������

	void sendGuildQueryRqst(); // ��ѯ������Ϣ

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

};

#endif // GuildProxy_h__
