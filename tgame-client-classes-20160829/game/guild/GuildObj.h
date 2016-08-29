#ifndef GuildObj_h__
#define GuildObj_h__

#include "CSGuildMsg.pb.h"
#include "basic/BasicTypes.h"

class GuildObj 
{
public:
	GuildObj();
	~GuildObj();

	UINT64_t getMyGuildID();

	void setMyGuildID(UINT64_t uMyGuildID);

	const vmsg::CSQueryGuildLstRsp& getGuildListInfo();

	void setGuildListInfo(const vmsg::CSQueryGuildLstRsp &stGuildListInfo);

	const vmsg::CSGuildBrief& getGuildSearchInfo();

	void setGuildSearchInfo(const vmsg::CSGuildBrief &stGuildSearchInfo);

	const vmsg::CSGuildInfoRsp& getGuildInfo();

	void setGuildInfo(const vmsg::CSGuildInfoRsp &stGuildInfo);

	const vmsg::CSOtherGuildRsp& getOtherGuildInfo();

	void setOtherGuildInfo(const vmsg::CSOtherGuildRsp &stOtherGuildInfo);

	const vmsg::CSAllGuildReqRsp& getAllGuildRqst();

	void setAllGuildRqst(const vmsg::CSAllGuildReqRsp &stAllGuildRqst);

	const vmsg::CSMyGuildReqRsp& getMyGuildRqst();

	void setMyGuildRqst(const vmsg::CSMyGuildReqRsp &stMyGuildRqst);

private:
	UINT64_t m_uMyGuildID; // 我的部落ID

	vmsg::CSQueryGuildLstRsp m_stGuildListInfo; // 部落列表

	vmsg::CSGuildBrief m_stGuildSearchInfo; // 查到的部落信息

	vmsg::CSGuildInfoRsp m_stGuildInfo; // 部落信息

	vmsg::CSOtherGuildRsp m_stOtherGuildInfo; // 其他部落信息

	vmsg::CSAllGuildReqRsp m_stAllGuildRqst; // 部落的所有请求

	vmsg::CSMyGuildReqRsp m_stMyGuildRqst; // 我的所有请求
	
};

#endif // GuildObj_h__