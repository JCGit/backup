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

	void sendQueryGuildListRqst(); // 查询部落列表

	void sendGuildSearchRqst(const std::string& strName); // 查找部落

	void sendGuildCreateRqst(const std::string& strName); // 创建部落

	void sendGuildQueryRqst(); // 查询部落信息

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

};

#endif // GuildProxy_h__
