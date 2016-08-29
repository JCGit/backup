#include "GuildObj.h"


GuildObj::GuildObj()
:m_uMyGuildID(0)
{
}

GuildObj::~GuildObj()
{
}

const vmsg::CSQueryGuildLstRsp& GuildObj::getGuildListInfo() {
	return m_stGuildListInfo;
}

void GuildObj::setGuildListInfo(const vmsg::CSQueryGuildLstRsp &stGuildListInfo) {
	m_stGuildListInfo = stGuildListInfo;
}

const vmsg::CSGuildBrief& GuildObj::getGuildSearchInfo() {
	return m_stGuildSearchInfo;
}

void GuildObj::setGuildSearchInfo(const vmsg::CSGuildBrief &stGuildSearchInfo) {
	m_stGuildSearchInfo = stGuildSearchInfo;
}

const vmsg::CSGuildInfoRsp& GuildObj::getGuildInfo() {
	return m_stGuildInfo;
}

void GuildObj::setGuildInfo(const vmsg::CSGuildInfoRsp &stGuildInfo) {
	m_stGuildInfo = stGuildInfo;
}

const vmsg::CSOtherGuildRsp& GuildObj::getOtherGuildInfo() {
	return m_stOtherGuildInfo;
}

void GuildObj::setOtherGuildInfo(const vmsg::CSOtherGuildRsp &stOtherGuildInfo) {
	m_stOtherGuildInfo = stOtherGuildInfo;
}

const vmsg::CSAllGuildReqRsp& GuildObj::getAllGuildRqst() {
	return m_stAllGuildRqst;
}

void GuildObj::setAllGuildRqst(const vmsg::CSAllGuildReqRsp &stAllGuildRqst) {
	m_stAllGuildRqst = stAllGuildRqst;
}

const vmsg::CSMyGuildReqRsp& GuildObj::getMyGuildRqst() {
	return m_stMyGuildRqst;
}

void GuildObj::setMyGuildRqst(const vmsg::CSMyGuildReqRsp &stMyGuildRqst) {
	m_stMyGuildRqst = stMyGuildRqst;
}
