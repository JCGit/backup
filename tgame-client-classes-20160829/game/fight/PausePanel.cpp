#include "PausePanel.h"
#include "instance/InstanceEventUtils.h"
using namespace std;
CPausePanel::CPausePanel()
:m_pContinueBtn(NULL)
,m_pQuitBtn(NULL)
,m_pBeforeTargetSets(NULL)
{

}

CPausePanel::~CPausePanel(){
	CC_SAFE_RELEASE_NULL(m_pBeforeTargetSets);
}

bool CPausePanel::init(){
	bool bRet = false;

	do 
	{
		CC_BREAK_IF(!BasePanel::init());

		initLayoutFromJsonFile("ui_v2/fight_pause_panel_ui.ExportJson");
		addChild(m_pRootLayout);

		m_pContinueBtn = dynamic_cast<UIButton*>(m_pRootLayout->getChildByName("resume_btn"));
		m_pQuitBtn = dynamic_cast<UIButton*>(m_pRootLayout->getChildByName("giveup_btn"));

		m_pContinueBtn->addTouchEventListener(this, toucheventselector(CPausePanel::touchEventHandler));
		m_pQuitBtn->addTouchEventListener(this, toucheventselector(CPausePanel::touchEventHandler));

		bRet = true;

	} while (0);

	return bRet;
}

void CPausePanel::touchEventHandler(CCObject *pSender, TouchEventType type){
	UIWidget* pWidget = dynamic_cast<UIWidget*>(pSender);
	const char* pName = pWidget->getName();
	string strName = pWidget->getName();
	switch (type)
	{
	case TOUCH_EVENT_ENDED:
		if(strcmp(pName, "resume_btn") == 0){
			this->close();
		}else if(strcmp(pName, "giveup_btn") == 0){
			this->close();
			CCNotificationCenter::sharedNotificationCenter()->postNotification(GIVEUP_FIGHT_EVENT, this);
		}
		break;
	default:
		break;
	}
}

void CPausePanel::open(){
	//m_pBeforeTargetSets = CCDirector::sharedDirector()->getScheduler()->pauseAllTargets();
	//CC_SAFE_RETAIN(m_pBeforeTargetSets);  

	CCDirector::sharedDirector()->pause();

	BasePanel::open();
}

void CPausePanel::close(){
    //CCDirector::sharedDirector()->getScheduler()->resumeTargets(m_pBeforeTargetSets);
    //CC_SAFE_RELEASE_NULL(m_pBeforeTargetSets);

	CCDirector::sharedDirector()->resume();
	BasePanel::close();
}