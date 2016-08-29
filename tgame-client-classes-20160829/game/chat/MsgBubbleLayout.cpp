#include "MsgBubbleLayout.h"

#include "UserObj.h"

MsgBubbleLayout::MsgBubbleLayout()
:m_pBgChatMsgImg(NULL)
,m_pChatMsgTxt(NULL)
{
}

MsgBubbleLayout::~MsgBubbleLayout()
{
	CC_SAFE_RELEASE_NULL(m_pChatMsgTxt); // retain at MsgBubbleLayout::initWithLayout(UILayout *pRefLayout)
}

MsgBubbleLayout* MsgBubbleLayout::create(UILayout *pRefLayout) {
	MsgBubbleLayout *pRet = new MsgBubbleLayout();

	if(pRet != NULL && pRet->initWithLayout(pRefLayout)) {
		pRet->autorelease();
	} else {
		CC_SAFE_DELETE(pRet);
	}

	return pRet;
}

bool MsgBubbleLayout::init() {
	return false;
}

bool MsgBubbleLayout::initWithLayout(UILayout *pRefLayout) {
	bool bRet = false;

	do {
		CC_BREAK_IF(!BaseLayout::init());

		initLayoutWithUICopy(pRefLayout);
		addChild(m_pRootLayout);

		m_pBgChatMsgImg = dynamic_cast<UIImageView*>(m_pRootLayout->getChildByName("bg_chat_msg_img"));

		m_pChatMsgTxt = ExRichText::create();
		CC_SAFE_RETAIN(m_pChatMsgTxt); // release at MsgBubbleLayout::~MsgBubbleLayout()
		m_pChatMsgTxt->setAnchorPoint(ccp(0.0f, 0.0f));
		m_pChatMsgTxt->ignoreContentAdaptWithSize(false);
		m_pChatMsgTxt->setSize(CCSizeMake(400, 30));
		m_pRootLayout->addChild(m_pChatMsgTxt);

		bRet = true;
	} while(0);

	return bRet;
}

std::string MsgBubbleLayout::msg_to_rich_str(const std::string &msg) {
	std::string rich_str = "";
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
			rich_str += "<txt face='fonts/tgamefont.ttf' size='22' color='#ffffff'>" + text + "</txt>";
		}

		std::string idx_str = msg.substr(posArr[i] + 2, posArr[i+1] - posArr[i] - 2);
		rich_str += "<anim src='ani/e_" + idx_str + ".jpg' width='40' height='40' frames='1'/>";
		//rich_str += "<img src='ani/e_" + idx_str + ".jpg' width='24' height='24'/>";
	}

	if(parsePos != std::string::npos) {
		const std::string text = msg.substr(parsePos, msg.size() - parsePos);

		if(!text.empty()) {
			rich_str += "<txt face='fonts/tgamefont.ttf' size='22' color='#ffffff'>" + text + "</txt>";
		}

	}

	rich_str += "<br/>";

	return rich_str;
}

void MsgBubbleLayout::setChatInfo(const vmsg::CSChatInfo &stChatInfo) {
	m_pChatMsgTxt->setText(msg_to_rich_str(stChatInfo.msgcontent()));

	const CCSize &size = m_pChatMsgTxt->getSize();
	float w = size.width + 35 + 20;
	float h = size.height + 32;
	w = w > 100 ? w : 100;
	h = h > 72 ? h : 72;
	m_pBgChatMsgImg->setSize(CCSizeMake(w, h));
	m_pRootLayout->setSize(CCSizeMake(m_pRootLayout->getSize().width, h));
	setSize(m_pRootLayout->getSize());

	if(P_USER_OBJ->getUserInfo()->uin == stChatInfo.senderuin()) { // 自己的消息，显示在右边
		m_pBgChatMsgImg->setScaleX(1.0f);
		m_pBgChatMsgImg->setPositionX(m_pRootLayout->getSize().width - m_pBgChatMsgImg->getSize().width);
		m_pChatMsgTxt->setPosition(ccp(m_pRootLayout->getSize().width - m_pBgChatMsgImg->getSize().width + 20, getSize().height - 15));
	} else {
		m_pBgChatMsgImg->setScaleX(-1.0f);
		m_pBgChatMsgImg->setPositionX(m_pBgChatMsgImg->getSize().width);
		m_pChatMsgTxt->setPosition(ccp(35, getSize().height - 15));
	}
}
