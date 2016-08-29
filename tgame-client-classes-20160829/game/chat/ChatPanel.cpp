#include "ChatPanel.h"

#include "ChatControl.h"
#include "AnimSprite.h"
#include "defines.h"

ChatPanel::ChatPanel()
:m_pCloseBtn(NULL)
,m_pChannelListView(NULL)
,m_pExpressionBtn(NULL)
,m_pChatInputTxt(NULL)
,m_pSendBtn(NULL)
,m_pChatRecordsScrollView(NULL)
,m_pChatRecordsTxt(NULL)
,m_pExpressionLayout(NULL)
,m_nChannelSelectedIdx(-1)
,m_uChannelType(0)
{
}

ChatPanel::~ChatPanel()
{
}

ChatPanel* ChatPanel::create() {
	ChatPanel *pRet = new ChatPanel();

	if(pRet != NULL && pRet->init()) {
		pRet->autorelease();
	} else {
		CC_SAFE_DELETE(pRet);
	}

	return pRet;
}

bool ChatPanel::init() {
	bool bRet = false;

	do {
		CC_BREAK_IF(!BasePanel::init());

		setClickOutToClose(true);

		bRet = true;
	} while(0);

	return bRet;
}

void ChatPanel::open() {
	BasePanel::open();

	showLoading();

	if(!m_bUIInited) {
		initUI();
	}

	updateUI();
}

void ChatPanel::close(){
	BasePanel::close();

	m_pExpressionLayout->setVisible(false);
	m_pExpressionLayout->setTouchEnabled(false);
}

void ChatPanel::updateUI() {
	std::string text = "";

	if(m_uChannelType == vmsg::CHAT_CHANNEL_WORLD) {
		const std::list<vmsg::CSChatInfo> &stWorldMsgs = P_CHAT_CTRL->getWorldMsgs();
		std::list<vmsg::CSChatInfo>::const_iterator iter;

		for(iter = stWorldMsgs.begin(); iter != stWorldMsgs.end(); iter++) {
			text.append(msg_to_rich_str(iter->sendernick(), iter->msgcontent()));
		}
	} else if(m_uChannelType == vmsg::CHAT_CHANNEL_GUILD) {
		const std::list<vmsg::CSChatInfo> &stGuildMsgs = P_CHAT_CTRL->getGuildMsgs();
		std::list<vmsg::CSChatInfo>::const_iterator iter;

		for(iter = stGuildMsgs.begin(); iter != stGuildMsgs.end(); iter++) {
			text.append(msg_to_rich_str(iter->sendernick(), iter->msgcontent()));
		}
	}
	
	m_pChatRecordsTxt->setText(text);
	const CCSize &txtSize = m_pChatRecordsTxt->getSize();
	const CCSize &viewSize = m_pChatRecordsScrollView->getSize();
	float scrollW = viewSize.width;
	float scrollH = txtSize.height > viewSize.height ? txtSize.height : viewSize.height;
	m_pChatRecordsScrollView->setInnerContainerSize(CCSizeMake(scrollW, scrollH));
	m_pChatRecordsTxt->setPosition(ccp(0, scrollH));
	m_pChatRecordsScrollView->jumpToBottom();

	closeLoading();
}

std::string ChatPanel::msg_to_rich_str(const std::string &nick, const std::string &msg) {
	std::string rich_str;
	rich_str += "<txt name='test' face='fonts/tgamefont.ttf' size='24' color='#8ffad8' touchable='true'>[" + nick+ "]: </txt>";

	std::vector<std::string::size_type> posArr;

	std::string::size_type pos = 0;
	std::string::size_type startPos = 0;
	std::string::size_type endPos = 0;

	while(pos < msg.size()) {
		pos = msg.find("[", pos, 1);

		if(pos != std::string::npos){
			startPos = pos;
			pos = msg.find("]", startPos + 2, 1);

			if(pos != std::string::npos) {
				endPos = pos;
				posArr.push_back(startPos);
				posArr.push_back(endPos);
			} else {
				break;
			}
		} else {
			break;
		}

		pos++;
	}

	std::string::size_type parsePos = 0;

	for(unsigned int i = 0; i < posArr.size(); i += 2) {
		const std::string text = msg.substr(parsePos, posArr[i] - parsePos);
		parsePos = posArr[i+1] + 1;

		if(!text.empty()) {
			rich_str += "<txt face='fonts/tgamefont.ttf' size='24' color='#8ffad8'>" + text + "</txt>";
		}
		
		std::string idx_str = msg.substr(posArr[i] + 2, posArr[i+1] - posArr[i] - 2);
		rich_str += "<anim src='ani/e_" + idx_str + ".jpg' width='40' height='40' frames='1'/>";
	}

	if(parsePos != std::string::npos) {
		const std::string text = msg.substr(parsePos, msg.size() - parsePos);

		if(!text.empty()) {
			rich_str += "<txt face='fonts/tgamefont.ttf' size='24' color='#8ffad8'>" + text + "</txt>";
		}
		
	}

	rich_str += "<br/>";

	return rich_str;
}

void ChatPanel::initUI() {
	initLayoutFromJsonFile("ui_v2/ChatPanelUI.ExportJson");
	addChild(m_pRootLayout);

	m_pCloseBtn = dynamic_cast<UIButton*>(m_pRootLayout->getChildByName("close_btn"));
	m_pCloseBtn->addTouchEventListener(this, toucheventselector(ChatPanel::touchEventHandler));

	m_pChannelListView = dynamic_cast<UIListView*>(m_pRootLayout->getChildByName("channel_list"));
	m_pChannelListView->addEventListenerListView(this, listvieweventselector(ChatPanel::listViewEventHandler));

	m_pExpressionBtn = dynamic_cast<UIButton*>(m_pRootLayout->getChildByName("expression_btn"));
	m_pExpressionBtn->addTouchEventListener(this, toucheventselector(ChatPanel::touchEventHandler));

	m_pChatInputTxt = dynamic_cast<UITextField*>(m_pRootLayout->getChildByName("chat_input_txt"));

	m_pSendBtn = dynamic_cast<UIButton*>(m_pRootLayout->getChildByName("send_btn"));
	m_pSendBtn->addTouchEventListener(this, toucheventselector(ChatPanel::touchEventHandler));

	m_pChatRecordsScrollView = dynamic_cast<UIScrollView*>(m_pRootLayout->getChildByName("chat_records_scroll"));

	m_pChatRecordsTxt = ExRichText::create();
	m_pChatRecordsTxt->setAnchorPoint(ccp(0.0f, 0.0f));
	m_pChatRecordsTxt->ignoreContentAdaptWithSize(false);
	m_pChatRecordsTxt->setSize(m_pChatRecordsScrollView->getSize());
	m_pChatRecordsTxt->addElmtTouchEventListener(this, toucheventselector(ChatPanel::elmtTouchEventHandler));
	m_pChatRecordsTxt->setLimitWidth(640);
	m_pChatRecordsScrollView->addChild(m_pChatRecordsTxt);

	m_pExpressionLayout = dynamic_cast<UILayout*>(m_pRootLayout->getChildByName("expression_layout"));
	m_pExpressionLayout->addTouchEventListener(this, toucheventselector(ChatPanel::touchEventHandler));

	CCSize animSize = CCSizeMake(40, 40);
	CCSize viewSize = m_pExpressionLayout->getSize();

	int col = 12;

	float dx = 9;
	float dy = 9;

	for(int i = 0; i < 24; i++) {
		std::string file = std::string("ani/e_") + intToString(i+1) + ".jpg";
		AnimSprite *pAnim = AnimSprite::create(file.c_str(), animSize, 1);
		float posX = dx + animSize.width / 2 + (animSize.width + dx) * (i % col);
		float posY = 360 - dy - animSize.height / 2 - (animSize.height + dy) * (i / col);
		pAnim->setPosition(ccp(posX, posY));
		m_pExpressionLayout->addNode(pAnim);
	}

	UIButton *pBtn = dynamic_cast<UIButton*>(m_pChannelListView->getItem(0));
	pBtn->setBrightStyle(BRIGHT_HIGHLIGHT);
	pBtn->setTouchEnabled(false);
	pBtn->setTitleColor(ccc3(0xff, 0xfa, 0xae));
	pBtn->setTitleFontSize(24);
	m_nChannelSelectedIdx = 0;
	m_uChannelType = vmsg::CHAT_CHANNEL_WORLD;

	m_bUIInited = true;
}

void ChatPanel::touchEventHandler(CCObject *pSender, TouchEventType type) {
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
				std::string msg = m_pChatInputTxt->getStringValue();
				
				if(!msg.empty()) {
					//m_pChatRecordsTxt->setText(rich_str);
					
					P_CHAT_CTRL->sendChatRqst(m_uChannelType, msg);
					m_pChatInputTxt->setText("");
				}
			} else if(std::strcmp(pszWidgetName, m_pExpressionLayout->getName()) == 0) {
				CCPoint pos = m_pExpressionLayout->convertToNodeSpace(m_pExpressionLayout->getTouchEndPos());
				CCLOG("x=%f y=%f", pos.x, pos.y);
				int idx = 0;
				CCSize animSize = CCSizeMake(40, 40);
				CCSize viewSize = m_pExpressionLayout->getSize();

				int col = 12;

				float dx = 9;
				float dy = 9;

				for(int i = 0; i < 24; i++) {
					float posX = dx + animSize.width / 2 + (animSize.width + dx) * (i % col);
					float posY = 360 - dy - animSize.height / 2 - (animSize.height + dy) * (i / col);

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
		default:;
	}
}

void ChatPanel::listViewEventHandler(CCObject *pSender, ListViewEventType type) {
	UIWidget *pWidget = dynamic_cast<UIWidget*>(pSender);
	const char *pszWidgetName = pWidget->getName();

	switch(type) {
		case LISTVIEW_ONSELECTEDITEM_END:
			if(std::strcmp(pszWidgetName, m_pChannelListView->getName()) == 0) {
				int nSelectedIdx = m_pChannelListView->getCurSelectedIndex();
				UIButton *pBtn = NULL;

				if(m_nChannelSelectedIdx >= 0) {
					pBtn = dynamic_cast<UIButton*>(m_pChannelListView->getItem(m_nChannelSelectedIdx));
					pBtn->setBrightStyle(BRIGHT_NORMAL);
					pBtn->setTouchEnabled(true);
					pBtn->setTitleColor(ccc3(0x7b, 0xbc, 0xe1));
					pBtn->setTitleFontSize(22);
				}

				pBtn = dynamic_cast<UIButton*>(m_pChannelListView->getItem(nSelectedIdx));
				pBtn->setBrightStyle(BRIGHT_HIGHLIGHT);
				pBtn->setTouchEnabled(false);
				pBtn->setTitleColor(ccc3(0xff, 0xfa, 0xae));
				pBtn->setTitleFontSize(24);
				m_nChannelSelectedIdx = nSelectedIdx;

				const char *pBtnName = pBtn->getName();

				if(std::strcmp(pBtnName, "channel_world_btn") == 0) { // ÊÀ½ç
					m_uChannelType = vmsg::CHAT_CHANNEL_WORLD;
					updateUI();
				} else if(std::strcmp(pBtnName, "channel_guild_btn") == 0) { // ²¿Âä
					m_uChannelType = vmsg::CHAT_CHANNEL_GUILD;
					updateUI();
				}
			}
			break;
		default:
			break;
	}
}

void ChatPanel::elmtTouchEventHandler(CCObject *pSender, TouchEventType type) {
	UIWidget *pWidget = dynamic_cast<UIWidget*>(pSender);
	const char *pszWidgetName = pWidget->getName();

	switch(type) {
		case TOUCH_EVENT_ENDED:
			if(std::strcmp(pszWidgetName, "test") == 0) {
				CCLOG("click %s", dynamic_cast<UILabel*>(pSender)->getStringValue());
			}
			break;
		default:
			break;
	}
}
