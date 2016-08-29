#ifndef ChatObj_h__
#define ChatObj_h__

#include "CSChatMsg.pb.h"
#include <list>
#include "basic/BasicTypes.h"

class ChatObj 
{
public:
	ChatObj();
	~ChatObj();

	const std::list<vmsg::CSChatInfo>& getWorldMsgs();

	void pushWorldMsg(const vmsg::CSChatInfo &stChatInfo);

	const std::list<vmsg::CSChatInfo>& getGuildMsgs();

	void pushGuildMsg(const vmsg::CSChatInfo &stChatInfo);

	const std::list<vmsg::CSChatInfo>& getSecretMsgs(UIN_t uin);

	void pushSecretMsg(UIN_t uin, const vmsg::CSChatInfo &stChatInfo);

	const std::list<UIN_t>& getPlayers();

	const std::map<UIN_t, std::list<vmsg::CSChatInfo> >& getSecretMsgMap();

private:
	std::list<vmsg::CSChatInfo> m_stWorldMsgs;

	std::list<vmsg::CSChatInfo> m_stGuildMsgs;

	std::list<UIN_t> m_stPlayers;

	std::map<UIN_t, std::list<vmsg::CSChatInfo> > m_stSecretMsgMap;

};

#define MAX_RECORDS_NUM 50 // ����¼��������Ϣ
#define MAX_SECRET_CHAT_PLAYERS 20 // ����¼���ٸ�˽�Ĺ�����
#define MAX_SECRET_MSGS 50 // ����¼������˽����Ϣ

#endif // ChatObj_h__