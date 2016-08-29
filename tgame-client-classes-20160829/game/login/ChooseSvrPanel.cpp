#include "ChooseSvrPanel.h" 
#include "LoginObj.h"
#include "SvrCell.h"
#include "LoginControl.h"
#include "defines.h"
#include "control/PanelSoundType.h"
#include "LoginScreenView.h"
#include "QEnvSet.h"
#include "GameInf.h"
using namespace std;

ChooseSvrPanel::ChooseSvrPanel()
:m_pBgImg(NULL)
,m_pBackBtn(NULL)
,m_pRecentLoginListView(NULL)
,m_pSvrListScrollView(NULL)
,m_stScrollViewSize(CCSizeZero)
{
}

ChooseSvrPanel::~ChooseSvrPanel()
{
}

ChooseSvrPanel* ChooseSvrPanel::create() {
	ChooseSvrPanel *pRet = new ChooseSvrPanel();

	if(pRet != NULL && pRet->init()) {
		pRet->autorelease();
	} else {
		CC_SAFE_DELETE(pRet);
	}

	return pRet;
}

bool ChooseSvrPanel::init() {
	bool bRet = false;

	do {
		CC_BREAK_IF(!BasePanel::init());

		CCDirector *pDirector = CCDirector::sharedDirector();
		CCPoint screenOrigin = pDirector->getVisibleOrigin();
		CCSize screenSize = pDirector->getVisibleSize();

		initLayoutFromJsonFile("ui_v2/ChooseSvrPanelUI.ExportJson");
		/*m_pRootLayout->setSize(screenSize);
		setSize(screenSize);*/
		addChild(m_pRootLayout);

        m_pBgImg = dynamic_cast<UIImageView*>(m_pRootLayout->getChildByName("bg_img"));

        CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("ui_bg/bg_svr_list.plist");
		m_pBgImg->loadTexture("ui_bg/bg_svr_list.png", UI_TEX_TYPE_PLIST);
		float scaleX = screenSize.width / m_pBgImg->getSize().width;
		float scaleY = screenSize.height / m_pBgImg->getSize().height;
		float scale = scaleX > scaleY ? scaleX : scaleY;
		m_pBgImg->setScale(scale);

		m_uSoundType = NONE_PANEL_TYPE;

		m_pBackBtn = dynamic_cast<UIButton*>(m_pRootLayout->getChildByName("back_btn"));
		m_pBackBtn->addTouchEventListener(this, toucheventselector(ChooseSvrPanel::touchEventHandler));

		m_pRecentLoginListView = dynamic_cast<UIListView*>(m_pRootLayout->getChildByName("recent_login_list"));

		m_pSvrListScrollView = dynamic_cast<UIScrollView*>(m_pRootLayout->getChildByName("svr_list_scroll"));
		m_stScrollViewSize = m_pSvrListScrollView->getSize();

		bRet = true;
	} while(0);

	return bRet;
}

string ChooseSvrPanel::getDescription() const {
	return "ChooseSvrPanel";
}

void ChooseSvrPanel::updateSvrList() {
    if(!this->isOpen()) {
    
        return;
    }
	// 1 show recent login svr
	vector<SvrInfo> &stRecentLoginSvrInfos = P_LOGIN_OBJ->getRecentLoginSvrInfos();
	vector<SvrInfo>::const_reverse_iterator iter;

	m_pRecentLoginListView->removeAllItems();

	for(iter = stRecentLoginSvrInfos.rbegin(); iter != stRecentLoginSvrInfos.rend(); iter++) {
		SvrCell *pCell = SvrCell::create();
		pCell->setInfo(*iter);
		pCell->setTouchEnabled(true);

		m_pRecentLoginListView->pushBackCustomItem(pCell);
		pCell->addTouchEventListener(this, toucheventselector(ChooseSvrPanel::touchEventHandler));
	}

	// 2 show all svr
	vector<SvrInfo> &svrInfoList = P_LOGIN_OBJ->getSvrInfoList();
	
	CCSize scrollViewSize = m_stScrollViewSize;

	unsigned int len = 0;

	if(svrInfoList.size() > m_pSvrListScrollView->getChildrenCount()) {
		len = svrInfoList.size();

		for(unsigned int i = 0; i < len; i++) {
			SvrCell *pCell = NULL;

			if(i < (int)m_pSvrListScrollView->getChildrenCount()) {
				string strItemName = "svr_cell";
				strItemName = strItemName + to_string(i);
				pCell = dynamic_cast<SvrCell*>(m_pSvrListScrollView->getChildByName(strItemName.c_str()));
				pCell->setInfo(svrInfoList.at(i));
			} else {
				pCell = SvrCell::create();
				pCell->setInfo(svrInfoList.at(i));
				pCell->setTouchEnabled(true);
				string strCellName = "svr_cell";
				strCellName = strCellName + to_string(i);
				pCell->setName(strCellName.c_str());
				pCell->addTouchEventListener(this, toucheventselector(ChooseSvrPanel::touchEventHandler));

				float paddingX = 0;
				float paddingY = 0;
				float marginX = (m_stScrollViewSize.width - COLS * pCell->getSize().width) / (COLS - 1);
				float marginY = (m_stScrollViewSize.height - (ROWS + 0.5) * pCell->getSize().height) / (ROWS + 1);

				float posX = paddingX + i % COLS * (pCell->getSize().width + marginX);
				float posY = scrollViewSize.height - (paddingY + (i / COLS + 1) * (pCell->getSize().height + marginY));

				pCell->setPosition(ccp(posX, posY));
				m_pSvrListScrollView->addChild(pCell);

				// reposition after add
				int rows = (i + 1) / COLS + ((i + 1) % COLS == 0 ? 0 : 1); 
				float nowScrollHeight = rows * pCell->getSize().height + (rows + 1) * marginY + 2 * paddingY;

				if(nowScrollHeight > scrollViewSize.height) {
					scrollViewSize.height = nowScrollHeight;

					m_pSvrListScrollView->setInnerContainerSize(CCSizeMake(scrollViewSize.width, scrollViewSize.height));

					for(unsigned int j = 0; j < m_pSvrListScrollView->getChildrenCount(); j++) {
						string strItemName = "svr_cell";
						strItemName = strItemName + to_string(j);
						SvrCell *pItem = dynamic_cast<SvrCell*>(m_pSvrListScrollView->getChildByName(strItemName.c_str()));
						posX = paddingX + j % COLS * (pItem->getSize().width + marginX);
						posY = scrollViewSize.height - (paddingY + (j / COLS + 1) * (pItem->getSize().height + marginY));
						pItem->setPosition(ccp(posX, posY));
					}
				}
			}
		}
	} else {
		len = m_pSvrListScrollView->getChildrenCount();

		for(unsigned int i = 0; i < len; i++) {
			SvrCell *pCell = NULL;
			string strItemName = "svr_cell";
			strItemName = strItemName + to_string(i);

			if(i < (int)svrInfoList.size()) {
				pCell = dynamic_cast<SvrCell*>(m_pSvrListScrollView->getChildByName(strItemName.c_str()));
				pCell->setInfo(svrInfoList.at(i));
			} else {
				m_pSvrListScrollView->removeChild(m_pSvrListScrollView->getChildByName(strItemName.c_str()));
			}
		}
	}
}

void ChooseSvrPanel::touchEventHandler(CCObject *pSender, TouchEventType type) {
	UIWidget *pWidget = dynamic_cast<UIWidget*>(pSender);
	const char *pszWidgetName = pWidget->getName();

	switch(type) {
		case TOUCH_EVENT_ENDED:
			if(strcmp(pszWidgetName, "back_btn") == 0) {
				close();
			} else if(strcmp(string(pszWidgetName).substr(0, 8).c_str(), "svr_cell") == 0) {
				string widgetName = pszWidgetName;
				
				P_LOGIN_OBJ->setCurSvrInfo(dynamic_cast<SvrCell*>(pWidget)->getInfo());
				close();
				P_LOGIN_CTRL->getLoginScreenView()->updateUI();
			} else if(strcmp(pWidget->getDescription().c_str(), "SvrCell") == 0) {
				string widgetName = pszWidgetName;

				P_LOGIN_OBJ->setCurSvrInfo(dynamic_cast<SvrCell*>(pWidget)->getInfo());
				close();
				P_LOGIN_CTRL->getLoginScreenView()->updateUI();
			}
			break;
		default:
			break;
	}
}

void ChooseSvrPanel::open() {
    BasePanel::open();

    P_LOGIN_CTRL->getProxy()->sendGetSvrListHttpRqst(P_GAME_INF->getDirURL());
}


