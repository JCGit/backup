#include "ChatControl.h"

#include "UserObj.h"
#include "basic/GlobalTime.h"
#include "game/city/CityControl.h"

ChatControl::ChatControl()
:m_pChatPanel(NULL)
,m_pSecretChatPanel(NULL)
,m_bInitialized(false)
,m_bIsNotifySecretChat(false)
{
}

ChatControl::~ChatControl()
{
	finalize();
}

void ChatControl::init() {
	if(!m_bInitialized) {
		m_pChatPanel = ChatPanel::create();
		CC_SAFE_RETAIN(m_pChatPanel);

		m_pSecretChatPanel = SecretChatPanel::create();
		CC_SAFE_RETAIN(m_pSecretChatPanel);

		m_bInitialized = true;
	}
}

void ChatControl::finalize() {
	if(m_bInitialized) {
		CC_SAFE_RELEASE_NULL(m_pChatPanel);

		CC_SAFE_RELEASE_NULL(m_pSecretChatPanel);

		m_bInitialized = false;
	}
}

const list<vmsg::CSChatInfo>& ChatControl::getWorldMsgs() {
	return m_stChatObj.getWorldMsgs();
}

const list<vmsg::CSChatInfo>& ChatControl::getGuildMsgs() {
	return m_stChatObj.getGuildMsgs();
}

const list<vmsg::CSChatInfo>& ChatControl::getSecretMsgs(UIN_t uin) {
	return m_stChatObj.getSecretMsgs(uin);
}

const list<UIN_t>& ChatControl::getPlayers() {
	return m_stChatObj.getPlayers();
}

const map<UIN_t, list<vmsg::CSChatInfo> >& ChatControl::getSecretMsgMap() {
	return m_stChatObj.getSecretMsgMap();
}

const vmsg::CSChatInfo& ChatControl::getPlayerChatInfo(UIN_t uin) {
	const map<UIN_t, list<vmsg::CSChatInfo> > &stMsgMap = m_stChatObj.getSecretMsgMap();

	const list<vmsg::CSChatInfo> &stChatInfos = stMsgMap.at(uin);
	list<vmsg::CSChatInfo>::const_iterator iter;

	for(iter = stChatInfos.begin(); iter != stChatInfos.end(); iter++) {
		if(iter->senderuin() == uin) {

			break;
		}
	}

	return *iter;
}

void ChatControl::pushSecretMsg(UIN_t uin, const vmsg::CSChatInfo &stChatInfo) {
	m_stChatObj.pushSecretMsg(uin, stChatInfo);
}

ChatPanel* ChatControl::getChatPanel() {
	//CCAssert(m_bInitialized, "Not initialized");
    if(!m_bInitialized) {
        init();
    }
	return m_pChatPanel;
}

SecretChatPanel* ChatControl::getSecretChatPanel() {
	//CCAssert(m_bInitialized, "Not initialized");
    if(!m_bInitialized) {
        init();
    }

	return m_pSecretChatPanel;
}

void ChatControl::chatWith(UIN_t uin, const string &name, unsigned int level, unsigned int showID) {
    if(!m_bInitialized) {
        init();
    }

	if(m_pSecretChatPanel != NULL) {
		vmsg::CSChatInfo stChatInfo;
		stChatInfo.set_senderuin(uin);
		stChatInfo.set_sendernick(name);
		stChatInfo.set_viplv(level);
		stChatInfo.set_showid(showID);
		m_stChatObj.pushSecretMsg(stChatInfo.senderuin(), stChatInfo);

		m_pSecretChatPanel->open();
	}
}

void ChatControl::notifySecretChat(bool bIsNotify) {
	m_bIsNotifySecretChat = bIsNotify;
}

bool ChatControl::isNotifySecretChat() {
	return m_bIsNotifySecretChat;
}

void ChatControl::onRecvMsg(vmsg::VLogicSvr2CliMsg &stMsg) {
	if(stMsg.has_errmsg()) {

	} else if(stMsg.has_chatrespond()) {
		const vmsg::CSChatRespond &stChatRsp = stMsg.chatrespond();

		if(stChatRsp.has_chatmsg_notify()) { // ÁÄÌìÍ¨Öª
			const vmsg::CSChatMsgNotify &stChatMsgNotify = stChatRsp.chatmsg_notify();
			unsigned int uChannelType = stChatMsgNotify.channeltype();

			for(int i = 0; i < stChatMsgNotify.chatinfos_size(); i++) {
				const vmsg::CSChatInfo &stChatInfo = stChatMsgNotify.chatinfos(i);

				if(uChannelType == vmsg::CHAT_CHANNEL_WORLD) {
					m_stChatObj.pushWorldMsg(stChatInfo);
				} else if(uChannelType == vmsg::CHAT_CHANNEL_GUILD) {
					m_stChatObj.pushGuildMsg(stChatInfo);
				} else if(uChannelType == vmsg::CHAT_CHANNEL_SECRET) {
					m_stChatObj.pushSecretMsg(stChatInfo.senderuin(), stChatInfo);

					if(m_pSecretChatPanel != NULL && m_pSecretChatPanel->isOpen()) {
						m_bIsNotifySecretChat = false;
						m_pSecretChatPanel->pushChatRecord(stChatInfo.senderuin(), stChatInfo);
					} else {
						m_bIsNotifySecretChat = true;
						P_CITY_CTRL->getCityScreenView()->updateEntry();
					}
				}
			}

			if(uChannelType != vmsg::CHAT_CHANNEL_SECRET && m_pChatPanel != NULL && m_pChatPanel->isOpen()) {
				m_pChatPanel->updateUI();
			}
		}
	}
}

void ChatControl::sendChatRqst(const unsigned int uChannelType, const string &msg) {
	m_stChatProxy.sendChatRqst(uChannelType, msg);
}

void ChatControl::sendSecretChatRqst(const UIN_t uin, const string &msg) {
	m_stChatProxy.sendSecretChatRqst(uin, msg);
}
