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
	UINT64_t m_uMyGuildID; // �ҵĲ���ID

	vmsg::CSQueryGuildLstRsp m_stGuildListInfo; // �����б�

	vmsg::CSGuildBrief m_stGuildSearchInfo; // �鵽�Ĳ�����Ϣ

	vmsg::CSGuildInfoRsp m_stGuildInfo; // ������Ϣ

	vmsg::CSOtherGuildRsp m_stOtherGuildInfo; // ����������Ϣ

	vmsg::CSAllGuildReqRsp m_stAllGuildRqst; // �������������

	vmsg::CSMyGuildReqRsp m_stMyGuildRqst; // �ҵ���������
	
};

#endif // GuildObj_h__