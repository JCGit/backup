#include "NoticePanel.h"

#include "basic/ToolUtils.h"

NoticePanel::NoticePanel()
:m_pNoticeScrollView(NULL)
,m_pNoticeTxt(NULL)
,m_pOKBtn(NULL)
{
}

NoticePanel::~NoticePanel()
{
}

NoticePanel* NoticePanel::create() {
	NoticePanel *pRet = new NoticePanel();

	if(pRet != NULL && pRet->init()) {
		pRet->autorelease();
	} else {
		CC_SAFE_DELETE(pRet);
	}

	return pRet;
}

bool NoticePanel::init() {
	bool bRet = false;

	do {
		CC_BREAK_IF(!BasePanel::init());

		bRet = true;
	} while(0);

	return bRet;
}

void NoticePanel::open() {
	BasePanel::open();

	showLoading();

	if(!m_bUIInited) {
		initUI();
	}

	closeLoading();
}

void NoticePanel::initUI() {
	initLayoutFromJsonFile("ui_v2/NoticePanelUI.ExportJson");
	addChild(m_pRootLayout);

	m_pNoticeScrollView = dynamic_cast<UIScrollView*>(m_pRootLayout->getChildByName("notice_scroll"));

	m_pNoticeTxt = dynamic_cast<UILabel*>(m_pNoticeScrollView->getChildByName("notice_txt"));

	m_pOKBtn = dynamic_cast<UIButton*>(m_pRootLayout->getChildByName("ok_btn"));
	m_pOKBtn->addTouchEventListener(this, toucheventselector(NoticePanel::touchEventHandler));

	string notice = "";

	string url = CCFileUtils::sharedFileUtils()->fullPathForFilename("notice");
	unsigned char *buf = NULL;
	unsigned long bufLen = 0;
	buf = CCFileUtils::sharedFileUtils()->getFileData(url.c_str(), "r", &bufLen);

	if(bufLen > 0) {
		notice.assign((char*)buf, bufLen);
	}

	if(buf != NULL) {
		delete[] buf;
		buf = NULL;
	}

	limitWidth(notice, 26);
	m_pNoticeTxt->setText(notice);

	const CCSize &textSize = m_pNoticeTxt->getSize();
	const CCSize &viewSize = m_pNoticeScrollView->getSize();
	float scrollW = viewSize.width;
	float scrollH = textSize.height > viewSize.height ? textSize.height : viewSize.height;
	m_pNoticeScrollView->setInnerContainerSize(CCSizeMake(scrollW, scrollH));
	m_pNoticeTxt->setPosition(ccp(0, scrollH));
	m_pNoticeScrollView->jumpToTop();

	m_bUIInited = true;
}

void NoticePanel::touchEventHandler(CCObject *pSender, TouchEventType type) {
	switch(type) {
		case TOUCH_EVENT_ENDED:
			if(pSender == m_pOKBtn) {
				close();
			}
			break;
		default:;
	}
}

void NoticePanel::limitWidth(std::string &text, unsigned int width) {
	int cnt = 0;
	vector<string> vecText;

	CToolUtils::parseUTFStr(text, vecText);
	text = "";

	for(unsigned int i = 0; i < vecText.size(); i++) {
		string ch = vecText[i];
		text += ch;
		cnt++;

		if(ch == "\n") {
			cnt = 0;
		} else {
			// 达到限定宽度
			if(cnt != 0 && cnt % width == 0 && i < vecText.size() - 1) {
				text += "\n";
				cnt = 0;
			}
		}

		/*if(cnt > 1) {
			text += vecText[i];
		} else if(vecText[i] != " ") {
			text += vecText[i];
		}


		if(vecText[i] == "\n") {
			cnt = 0;
		}

		if(cnt == 0 && vecText[i] == " ") {
			unsigned int n = 0;

			while(n < vecText.size() - i - 1) {
				n++;

				if(vecText[i + n] == " ") {
					break;
				}

			}

			if(cnt + n >= width) {
				text += "\n";
				cnt = 0;
			}
		}

		if(cnt != 0 && cnt % width == 0 && i < vecText.size() - 1) {
			text += "\n";
			cnt = 0;
		}*/
	}
}
