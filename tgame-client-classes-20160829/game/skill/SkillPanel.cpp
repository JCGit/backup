#include "SkillPanel.h"

CSkillPanel::CSkillPanel(){

}

CSkillPanel::~CSkillPanel(){

}

bool CSkillPanel::init() {
	bool bRet = false;

	do {
		CC_BREAK_IF(!BasePanel::init());

		setOpenWithBgImg(true);
		setShowMoneyTopBar(true);

		initLayoutFromJsonFile("ui_v2/skill_panel_ui.ExportJson");
		addChild(m_pRootLayout);

		initUI();

		bRet = true;
	} while(0);

	return bRet;
}

void CSkillPanel::initUI(){
	ImageView* m_pPointImg;
	ImageView* m_pListTitleImg;
	ImageView* m_pSkillEffImg;

	m_pCloseBtn = dynamic_cast<Button*>(m_pRootLayout->getChildByName("close_btn"));
	m_pTabWpSkillBtn = dynamic_cast<Button*>(m_pRootLayout->getChildByName("weapon_skill_btn"));
	m_pTabEvSkillBtn = dynamic_cast<Button*>(m_pRootLayout->getChildByName("evolution_skill_btn"));
	m_pSetBtn = dynamic_cast<Button*>(m_pRootLayout->getChildByName("set_btn"));

	m_pUpLayout = dynamic_cast<Layout*>(m_pRootLayout->getChildByName("up_layout"));

	m_pTimeTxt = dynamic_cast<Label*>(m_pRootLayout->getChildByName("time_txt"));
	m_pRemainTxt = dynamic_cast<Label*>(m_pRootLayout->getChildByName("skill_point_txt"));
	m_pSkillEffTxt = dynamic_cast<Label*>(m_pRootLayout->getChildByName("skill_desc_txt"));
	m_pUpEffTxt = dynamic_cast<Label*>(m_pUpLayout->getChildByName("up_desc_txt "));
	m_pCoinTxt = dynamic_cast<Label*>(m_pUpLayout->getChildByName("cost_coin_txt"));
	m_pCostPointTxt = dynamic_cast<Label*>(m_pUpLayout->getChildByName("cost_point_txt"));

	m_pListTitleImg = dynamic_cast<ImageView*>(m_pRootLayout->getChildByName("skill_type_img"));
	m_pPointImg = dynamic_cast<ImageView*>(m_pRootLayout->getChildByName("skill_point_icon"));
	m_pSkillEffImg = dynamic_cast<ImageView*>(m_pRootLayout->getChildByName("skill_effect_title"));

	m_pCloseBtn->addTouchEventListener(this, toucheventselector(CSkillPanel::touchEventHandler));
	m_pTabWpSkillBtn->addTouchEventListener(this, toucheventselector(CSkillPanel::touchEventHandler));
	m_pTabEvSkillBtn->addTouchEventListener(this, toucheventselector(CSkillPanel::touchEventHandler));
	m_pSetBtn->addTouchEventListener(this, toucheventselector(CSkillPanel::touchEventHandler));
}

void CSkillPanel::touchEventHandler(CCObject* pSender, TouchEventType type){
	UIWidget *pWidget = dynamic_cast<UIWidget*>(pSender);
	const char *pszWidgetName = pWidget->getName();
	string nameStr = string(pszWidgetName);

	if(TOUCH_EVENT_ENDED != type) {
		return;
	}

	if("tab_btn_weapon" == nameStr) {
		
	} 
}

void CSkillPanel::updateUI(){

}

void CSkillPanel::open(){

}

void CSkillPanel::close(){

}

