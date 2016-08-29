#include "ChatObj.h"

ChatObj::ChatObj()
{
}

ChatObj::~ChatObj()
{
}

const std::list<vmsg::CSChatInfo>& ChatObj::getWorldMsgs() {
	return m_stWorldMsgs;
}

void ChatObj::pushWorldMsg(const vmsg::CSChatInfo &stChatInfo) {
	if(m_stWorldMsgs.size() >= MAX_RECORDS_NUM) {
		m_stWorldMsgs.pop_front();
	}

	m_stWorldMsgs.push_back(stChatInfo);
}

const std::list<vmsg::CSChatInfo>& ChatObj::getGuildMsgs() {
	return m_stGuildMsgs;
}

void ChatObj::pushGuildMsg(const vmsg::CSChatInfo &stChatInfo) {
	if(m_stGuildMsgs.size() >= MAX_RECORDS_NUM) {
		m_stGuildMsgs.pop_front();
	}

	m_stGuildMsgs.push_back(stChatInfo);
}

const std::list<vmsg::CSChatInfo>& ChatObj::getSecretMsgs(UIN_t uin) {
	return m_stSecretMsgMap[uin];
}

void ChatObj::pushSecretMsg(UIN_t uin, const vmsg::CSChatInfo &stChatInfo) {
	std::map<UIN_t, std::list<vmsg::CSChatInfo> >::iterator iter = m_stSecretMsgMap.find(uin);

	if(iter != m_stSecretMsgMap.end()) { // ÒÑ´æÔÚ
		std::list<UIN_t>::iterator iter;

		for(iter = m_stPlayers.begin(); iter != m_stPlayers.end(); iter++) {
			if(*iter == uin) {
				m_stPlayers.erase(iter);
				break;
			}
		}

		m_stPlayers.push_front(uin);

		std::list<vmsg::CSChatInfo> &stChatInfos = m_stSecretMsgMap[uin];

		if(stChatInfos.size() >= MAX_SECRET_MSGS) {
			stChatInfos.pop_front();
		}

		stChatInfos.push_back(stChatInfo);
	} else {
		if(m_stPlayers.size() >= MAX_SECRET_CHAT_PLAYERS) {
			UIN_t delUin = m_stPlayers.back();
			m_stPlayers.pop_back();
			std::map<UIN_t, std::list<vmsg::CSChatInfo> >::iterator delIter = m_stSecretMsgMap.find(delUin);

			if(delIter != m_stSecretMsgMap.end()) {
				m_stSecretMsgMap.erase(delIter);
			}
		}

		m_stPlayers.push_front(uin);
		m_stSecretMsgMap[uin].push_back(stChatInfo);
	}

}

const std::list<UIN_t>& ChatObj::getPlayers() {
	return m_stPlayers;
}

const std::map<UIN_t, std::list<vmsg::CSChatInfo> >& ChatObj::getSecretMsgMap() {
	return m_stSecretMsgMap;
}
