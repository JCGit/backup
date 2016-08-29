#include "SecretChatPanel.h"

#include "MsgBubbleLayout.h"
#include "AnimSprite.h"
#include "defines.h"
#include "PlayerChatItem.h"
#include "ChatControl.h"
#include "UserObj.h"
#include "basic/GlobalTime.h"

SecretChatPanel::SecretChatPanel()
:m_pCloseBtn(NULL)
,m_pPlayerListView(NULL)
,m_pExpressionBtn(NULL)
,m_pChatInputTxt(NULL)
,m_pSendBtn(NULL)
,m_pChatRecordsScrollView(NULL)
,m_pExpressionLayout(NULL)
,m_nPlayerSelectedIdx(-1)
,m_uCurChatPlayerUin(0)
{
}

SecretChatPanel::~SecretChatPanel()
{
	for(unsigned int i = 0; i < m_stPlayerItems.size(); i++) {
		CC_SAFE_RELEASE_NULL(m_stPlayerItems[i]); // retain at SecretChatPanel::updateUI()
	}

	m_stPlayerItems.clear();

	for(unsigned int i = 0; i < m_stMsgBubbles.size(); i++) {
		CC_SAFE_RELEASE_NULL(m_stMsgBubbles[i]); // retain at SecretChatPanel::updateChatRecords()
	}

	m_stMsgBubbles.clear();

}

SecretChatPanel* SecretChatPanel::create() {
	SecretChatPanel *pRet = new SecretChatPanel();

	if(pRet != NULL && pRet->init()) {
		pRet->autorelease();
	} else {
		CC_SAFE_DELETE(pRet);
	}

	return pRet;
}

bool SecretChatPanel::init() {
	bool bRet = false;

	do {
		CC_BREAK_IF(!BasePanel::init());

		setClickOutToClose(true);

		bRet = true;
	} while(0);

	return bRet;
}

void SecretChatPanel::open() {
	BasePanel::open();

	showLoading();

	if(!m_bUIInited) {
		initUI();
	}

	P_CHAT_CTRL->notifySecretChat(false);

	updateUI();
}

void SecretChatPanel::close(){
	BasePanel::close();

	m_pExpressionLayout->setVisible(false);
	m_pExpressionLayout->setTouchEnabled(false);
}

void SecretChatPanel::updateUI() {
	m_pPlayerListView->removeAllItems();

	const std::list<UIN_t> &stPlayers = P_CHAT_CTRL->getPlayers();
	std::list<UIN_t>::const_iterator iter = stPlayers.begin();
	int idx = 0;

	while(iter != stPlayers.end()) {
		PlayerChatItem *pItem = NULL;

		if(m_stPlayerItems.size() < stPlayers.size()) {
			pItem = PlayerChatItem::create(dynamic_cast<UILayout*>(m_pRootLayout->getChildByName("player_list_item_layout")));
			m_stPlayerItems.push_back(pItem);
			CC_SAFE_RETAIN(pItem); // release at SecretChatPanel::~SecretChatPanel()
		}

		UIN_t uin = *iter;
		const vmsg::CSChatInfo &stChatInfo = P_CHAT_CTRL->getPlayerChatInfo(uin);
		pItem = m_stPlayerItems[idx];
		pItem->selected(false);
		pItem->updateUI(stChatInfo.senderuin(), stChatInfo.sendernick(), stChatInfo.showid());
		m_pPlayerListView->pushBackCustomItem(pItem);

		idx++;
		iter++;
	}

	if(m_pPlayerListView->getItems()->count() > 0) {
		m_nPlayerSelectedIdx = 0;
		PlayerChatItem *pItem = dynamic_cast<PlayerChatItem*>(m_pPlayerListView->getItem(m_nPlayerSelectedIdx));
		pItem->selected(true);
		m_uCurChatPlayerUin = pItem->getPlayerUin();
	}

	updateChatRecords();

	closeLoading();
}

void SecretChatPanel::updateChatRecords() {
	//cacheBubbles();
	m_pInnerLayout->removeAllChildren();

	int nSelectedIdx = m_pPlayerListView->getCurSelectedIndex();
	PlayerChatItem *pItem = dynamic_cast<PlayerChatItem*>(m_pPlayerListView->getItem(nSelectedIdx));

	if(pItem != NULL) {
		UIN_t uin = pItem->getPlayerUin();

		const std::list<vmsg::CSChatInfo> &stChatInfos = P_CHAT_CTRL->getSecretMsgs(uin);
		std::list<vmsg::CSChatInfo>::const_iterator iter = stChatInfos.begin();

		float h = 0.0f;

		while(iter != stChatInfos.end()) {
			if(iter->has_msgcontent()) {
				MsgBubbleLayout *pItem = MsgBubbleLayout::create(dynamic_cast<UILayout*>(m_pRootLayout->getChildByName("msg_bubble_layout")));
				//MsgBubbleLayout *pItem = getBubbleLayout();
				pItem->setChatInfo(*iter);
				h += pItem->getSize().height;
				m_pInnerLayout->addChild(pItem);
				m_pInnerLayout->setSize(CCSizeMake(m_pChatRecordsScrollView->getSize().width, h));
				pItem->setPosition(ccp(0, -h));
				h += 8;
			}
			
			iter++;
		}

		CCSize viewSize = m_pChatRecordsScrollView->getSize();
		float innerW = viewSize.width;
		float innerH = m_pInnerLayout->getSize().height > viewSize.height ? m_pInnerLayout->getSize().height : viewSize.height;
		m_pChatRecordsScrollView->setInnerContainerSize(CCSizeMake(innerW, innerH));
		m_pInnerLayout->setPosition(ccp(0, innerH));
		m_pChatRecordsScrollView->jumpToBottom();
	}

}

void SecretChatPanel::pushChatRecord(UIN_t uin, const vmsg::CSChatInfo &stChatInfo) {
	if(uin == m_uCurChatPlayerUin) {
		MsgBubbleLayout *pItem = MsgBubbleLayout::create(dynamic_cast<UILayout*>(m_pRootLayout->getChildByName("msg_bubble_layout")));
		//MsgBubbleLayout *pItem = getBubbleLayout();
		pItem->setChatInfo(stChatInfo);
		float h = m_pInnerLayout->getSize().height;
		h += pItem->getSize().height + 8;
		m_pInnerLayout->addChild(pItem);
		m_pInnerLayout->setSize(CCSizeMake(m_pChatRecordsScrollView->getSize().width, h));
		pItem->setPosition(ccp(0, -h));

		CCSize viewSize = m_pChatRecordsScrollView->getSize();
		float innerW = viewSize.width;
		float innerH = m_pInnerLayout->getSize().height > viewSize.height ? m_pInnerLayout->getSize().height : viewSize.height;
		m_pChatRecordsScrollView->setInnerContainerSize(CCSizeMake(innerW, innerH));
		m_pInnerLayout->setPosition(ccp(0, innerH));
		m_pChatRecordsScrollView->jumpToBottom();
	}
}

UIN_t SecretChatPanel::getCurChatPlayerUin() {
	return m_uCurChatPlayerUin;
}

MsgBubbleLayout* SecretChatPanel::getBubbleLayout() {
	MsgBubbleLayout *pRet = NULL;

	if(m_stMsgBubbles.size() > 0) {
		pRet = m_stMsgBubbles.back();
		m_stMsgBubbles.pop_back();
		CC_SAFE_RELEASE(pRet);
	} else {
		pRet = MsgBubbleLayout::create(dynamic_cast<UILayout*>(m_pRootLayout->getChildByName("msg_bubble_layout")));
	}

	return pRet;
}

void SecretChatPanel::cacheBubbles() {
	/*for(unsigned int i = 0; i < m_pChatRecordsListView->getItems()->count(); i++) {
	MsgBubbleLayout *pBubble = dynamic_cast<MsgBubbleLayout*>(m_pChatRecordsListView->getItem(i));
	m_stMsgBubbles.push_back(pBubble);
	CC_SAFE_RETAIN(pBubble);
	}*/
}

void SecretChatPanel::initUI() {
	initLayoutFromJsonFile("ui_v2/SecretChatPanelUI.ExportJson");
	addChild(m_pRootLayout);

	m_pCloseBtn = dynamic_cast<UIButton*>(m_pRootLayout->getChildByName("close_btn"));
	m_pCloseBtn->addTouchEventListener(this, toucheventselector(SecretChatPanel::touchEventHandler));

	m_pPlayerListView = dynamic_cast<UIListView*>(m_pRootLayout->getChildByName("player_list"));
	m_pPlayerListView->addEventListenerListView(this, listvieweventselector(SecretChatPanel::listViewEventHandler));

	m_pExpressionBtn = dynamic_cast<UIButton*>(m_pRootLayout->getChildByName("expression_btn"));
	m_pExpressionBtn->addTouchEventListener(this, toucheventselector(SecretChatPanel::touchEventHandler));

	m_pChatInputTxt = dynamic_cast<UITextField*>(m_pRootLayout->getChildByName("chat_input_txt"));

	m_pSendBtn = dynamic_cast<UIButton*>(m_pRootLayout->getChildByName("send_btn"));
	m_pSendBtn->addTouchEventListener(this, toucheventselector(SecretChatPanel::touchEventHandler));

	m_pChatRecordsScrollView = dynamic_cast<UIScrollView*>(m_pRootLayout->getChildByName("chat_records_scroll"));
	m_pInnerLayout = UILayout::create();
	m_pChatRecordsScrollView->addChild(m_pInnerLayout);

	m_pExpressionLayout = dynamic_cast<UILayout*>(m_pRootLayout->getChildByName("expression_layout"));
	m_pExpressionLayout->addTouchEventListener(this, toucheventselector(SecretChatPanel::touchEventHandler));

	CCSize animSize = CCSizeMake(40, 40);
	CCSize viewSize = m_pExpressionLayout->getSize();

	int col = 10;

	float dx = 15;
	float dy = 15;

	for(int i = 0; i < 24; i++) {
		std::string file = std::string("ani/e_") + intToString(i+1) + ".jpg";
		AnimSprite *pAnim = AnimSprite::create(file.c_str(), animSize, 1);
		float posX = dx + animSize.width / 2 + (animSize.width + dx) * (i % col);
		float posY = 340 - dy - animSize.height / 2 - (animSize.height + dy) * (i / col);
		pAnim->setPosition(ccp(posX, posY));
		m_pExpressionLayout->addNode(pAnim);
	}

	m_bUIInited = true;
}

void SecretChatPanel::touchEventHandler(CCObject *pSender, TouchEventType type) {
	UIWidget *pWidget = dynamic_cast<UIWidget*>(pSender);
	const char *pszWidgetName = pWidget->getName();

	switch(type) {
		case TOUCH_EVENT_ENDED:
			if(std::strcmp(pszWidgetName, m_pCloseBtn->getName()) == 0) {
				close();
			} else if(std::strcmp(pszWidgetName, m_pExpressionBtn->getName()) == 0) {
				m_pExpressionLayout->setVisible(!m_pExpressionLayout->isVisible());
				m_pExpressionLayout->setTouchEnabled(m_pExpressionLayout->isVisible());
			} else if(std::strcmp(pszWidgetName, m_pSendBtn->getName()) == 0) {
				int nSelectedIdx = m_pPlayerListView->getCurSelectedIndex();
				PlayerChatItem *pItem = dynamic_cast<PlayerChatItem*>(m_pPlayerListView->getItem(nSelectedIdx));

				UIN_t uin = pItem->getPlayerUin();
				const UserInfo *pUserInfo = P_USER_OBJ->getUserInfo();
				std::string text = m_pChatInputTxt->getStringValue();
				
				if(!text.empty()) {
					m_pChatInputTxt->setText("");

					vmsg::CSChatInfo stChatInfo;
					stChatInfo.set_sendtime(GlobalTime::getTime());
					stChatInfo.set_senderuin(pUserInfo->uin);
					stChatInfo.set_sendernick(pUserInfo->nick);
					stChatInfo.set_viplv(pUserInfo->vipLv);
					stChatInfo.set_msgcontent(text);

					P_CHAT_CTRL->pushSecretMsg(uin, stChatInfo);
					pushChatRecord(uin, stChatInfo);
					P_CHAT_CTRL->sendSecretChatRqst(uin, text);
					
				}
			} else if(std::strcmp(pszWidgetName, m_pExpressionLayout->getName()) == 0) {
				CCPoint pos = m_pExpressionLayout->convertToNodeSpace(m_pExpressionLayout->getTouchEndPos());
				CCLOG("x=%f y=%f", pos.x, pos.y);
				int idx = 0;
				CCSize animSize = CCSizeMake(40, 40);
				CCSize viewSize = m_pExpressionLayout->getSize();

				int col = 10;

				float dx = 15;
				float dy = 15;

				for(int i = 0; i < 24; i++) {
					float posX = dx + animSize.width / 2 + (animSize.width + dx) * (i % col);
					float posY = 340 - dy - animSize.height / 2 - (animSize.height + dy) * (i / col);
					
					CCRect rect = CCRectMake(posX - animSize.width / 2, posY - animSize.height / 2, animSize.width, animSize.height);

					if(rect.containsPoint(pos)) {
						idx = i + 1;
					}
				}

				if(idx > 0 && idx <= 24) {
					std::string msg = m_pChatInputTxt->getStringValue();
					msg += "[#" + intToString(idx) + "]";
					m_pChatInputTxt->setText(msg);
				}
				
				m_pExpressionLayout->setVisible(!m_pExpressionLayout->isVisible());
				m_pExpressionLayout->setTouchEnabled(m_pExpressionLayout->isVisible());
			}
			break;
		default:
			break;
	}
}

void SecretChatPanel::listViewEventHandler(CCObject *pSender, ListViewEventType type) {
	UIListView *pListView = dynamic_cast<UIListView*>(pSender);
	const char *pszListViewName = pListView->getName();

	switch(type) {
		case LISTVIEW_ONSELECTEDITEM_END:
			if(std::strcmp(pszListViewName, m_pPlayerListView->getName()) == 0) {
				int nSelectedIdx = m_pPlayerListView->getCurSelectedIndex();
				PlayerChatItem *pItem = NULL;

				if(m_nPlayerSelectedIdx >= 0) {
					pItem = dynamic_cast<PlayerChatItem*>(m_pPlayerListView->getItem(m_nPlayerSelectedIdx));
					pItem->selected(false);
				}

				pItem = dynamic_cast<PlayerChatItem*>(m_pPlayerListView->getItem(nSelectedIdx));
				pItem->selected(true);
				m_uCurChatPlayerUin = pItem->getPlayerUin();
				m_nPlayerSelectedIdx = nSelectedIdx;

				updateChatRecords();
			}
			break;
		default:
			break;
	}
}
