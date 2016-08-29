#include "GuardActRankUI.h"
#include <string>
#include "defines.h"

using namespace std;


CGuardActRankUI::CGuardActRankUI()
{
    m_iTime = 0;
}

CGuardActRankUI::~CGuardActRankUI(){
    this->unschedule(schedule_selector(CGuardActRankUI::updateTime));
}

void CGuardActRankUI::addShadow(UILabel *pLabel){
#ifndef WIN32
    CCLabelTTF* pLabelTTF = dynamic_cast<CCLabelTTF*>(pLabel->getVirtualRenderer());
    pLabelTTF->enableShadow(CCSizeMake(2,-2), 1.0f, 1.0f);
#endif // !WIN32
}

bool CGuardActRankUI::init(){
	bool bRet = false;

	do 
	{
		CC_BREAK_IF(!BaseLayout::init());

		initLayoutFromJsonFile("ui_v2/act_guard_rank_ui.ExportJson");
		addChild(m_pRootLayout);

        m_pTimeTipTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("time_label"));
        m_pTimeAtlasTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("tip_atlas_txt"));

        addShadow(m_pTimeTipTxt);
		addShadow(m_pTimeAtlasTxt);

		for(unsigned int i = 0; i < 3; i++){
			string idxStr = intToString(i);
			UILabel* pRankLabel = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName(("rank_" + idxStr).c_str()));
			UILabel* pNameLabel = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName(("name_" + idxStr).c_str()));
			UILabel* pLabel = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName(("label_" + idxStr).c_str()));
			UILabel* pDamageLabel = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName(("damage_" + idxStr).c_str()));
			addShadow(pRankLabel);
			addShadow(pNameLabel);
			addShadow(pLabel);
			addShadow(pDamageLabel);
			pRankLabel->setVisible(false);
			pNameLabel->setVisible(false);
			pLabel->setVisible(false);
			pDamageLabel->setVisible(false);
		}

        updateTimeTip(0);

		bRet = true;
	} while (0);

	return bRet;
}

void CGuardActRankUI::setParent(CCNode* parent){
	BaseLayout::setParent(parent);
}



void CGuardActRankUI::updateRankInfo(const vmsg::CSCVZRankInfo& stRankInfo) {
	for(int i = 0; i < 3; i++){
		string idxStr = intToString(i);
		UILabel* pRankLabel = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName(("rank_" + idxStr).c_str()));
		UILabel* pNameLabel = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName(("name_" + idxStr).c_str()));
		UILabel* pLabel = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName(("label_" + idxStr).c_str()));
		UILabel* pDamageLabel = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName(("damage_" + idxStr).c_str()));
		
		bool bIsVisible = true;
		if(i < stRankInfo.rankdatas_size()){
			bIsVisible = true;
			const vmsg::CSCVZRankData& oneData = stRankInfo.rankdatas(i);
			pRankLabel->setText(intToString(i + 1));
			pNameLabel->setText(oneData.name());
			pDamageLabel->setText(intToString(oneData.hit()));
		}else{
			bIsVisible = false;
		}

		pRankLabel->setVisible(bIsVisible);
		pNameLabel->setVisible(bIsVisible);
		pLabel->setVisible(bIsVisible);
		pDamageLabel->setVisible(bIsVisible);
	}
}

void CGuardActRankUI::updateTimeTip(const int iTime) {
    
    this->unschedule(schedule_selector(CGuardActRankUI::updateTime));
    if(iTime > 0) {
        m_iTime = iTime;
        this->schedule(schedule_selector(CGuardActRankUI::updateTime),1);
    } else {
        m_iTime = 0;
    }
    showTimeTip(m_iTime);
}


void CGuardActRankUI::showTimeTip(const int iTime) {
    char char_attckmun[100] = {0};
    snprintf(char_attckmun, 100, "%d", iTime);
    m_pTimeAtlasTxt->setText(char_attckmun);
}

void CGuardActRankUI::updateTime(float dt) {
    int iTime = m_iTime - 1;
    if(iTime <= 0) {
        iTime = 0;
        this->unschedule(schedule_selector(CGuardActRankUI::updateTime));
    }
    m_iTime = iTime;
    showTimeTip(iTime);
}
