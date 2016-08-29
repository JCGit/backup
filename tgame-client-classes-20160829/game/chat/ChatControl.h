#ifndef ChatControl_h__
#define ChatControl_h__

#include "vlib/base/CVSingleton.h"

#include "ChatProxy.h"
#include "ChatObj.h"
#include "ChatPanel.h"
#include "SecretChatPanel.h"

class ChatControl 
{
public:
	ChatControl();
	~ChatControl();

	void init();

	void finalize();

	const list<vmsg::CSChatInfo>& getWorldMsgs();

	const list<vmsg::CSChatInfo>& getGuildMsgs();

	const list<vmsg::CSChatInfo>& getSecretMsgs(UIN_t uin);

	const list<UIN_t>& getPlayers();

	const map<UIN_t, list<vmsg::CSChatInfo> >& getSecretMsgMap();

	const vmsg::CSChatInfo& getPlayerChatInfo(UIN_t uin);

	void pushSecretMsg(UIN_t uin, const vmsg::CSChatInfo &stChatInfo);

	ChatPanel* getChatPanel();

	SecretChatPanel* getSecretChatPanel();

	void chatWith(UIN_t uin, const string &name, unsigned int level, unsigned int showID);

	void notifySecretChat(bool bIsNotify);

	bool isNotifySecretChat();

	void onRecvMsg(vmsg::VLogicSvr2CliMsg &stMsg);

	void sendChatRqst(const unsigned int uChannelType, const string &msg); // адлЛ

	void sendSecretChatRqst(const UIN_t uin, const string &msg); // к╫ад

private:
	ChatProxy m_stChatProxy;

	ChatObj m_stChatObj;

	ChatPanel *m_pChatPanel;

	SecretChatPanel *m_pSecretChatPanel;

	bool m_bInitialized;

	bool m_bIsNotifySecretChat;

};

#define P_CHAT_CTRL CVLib::CVSingleton<ChatControl>::Instance()

#endif // ChatControl_h__