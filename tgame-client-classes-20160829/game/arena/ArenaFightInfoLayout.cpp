#include "ArenaFightInfoLayout.h"
#include "defines.h"
#include "TimeEx.h"
#include "CCColorUtil.h"

CArenaFightInfoLayout::CArenaFightInfoLayout(){

}

CArenaFightInfoLayout::~CArenaFightInfoLayout(){

}

bool CArenaFightInfoLayout::init(){
	bool bRet = false;

	do {
		CC_BREAK_IF(!BaseLayout::init());

		initLayoutFromJsonFile("ui_v2/pvp_info_ui.ExportJson");
		addChild(m_pRootLayout);

		m_pInfoImg = dynamic_cast<ImageView*>(m_pRootLayout->getChildByName("def_info"));
		m_pOpImg = dynamic_cast<ImageView*>(m_pRootLayout->getChildByName("op_info_bg"));
		m_pTimeTxt = dynamic_cast<Label*>(m_pOpImg->getChildByName("op_info_txt"));
		m_pDefNumTxt = dynamic_cast<Label*>(m_pInfoImg->getChildByName("def_info_txt"));

		m_pTimeTxt->setVisible(false);
		m_pTimeTxt->setText("");
		m_pDefNumTxt->setText("");

		CCColorUtil::addStroke(m_pTimeTxt, 2.0f);
		CCColorUtil::addStroke(m_pDefNumTxt, 2.0f);

		CCColorUtil::addShadow(m_pTimeTxt);
		CCColorUtil::addShadow(m_pDefNumTxt);

		bRet = true;
	} while(0);

	return bRet;
}

void CArenaFightInfoLayout::setDefNum(unsigned int uNum, unsigned int uTotal){
	string str = intToString(uNum) + "/" + intToString(uTotal);
	m_pDefNumTxt->setText(str);
}

void CArenaFightInfoLayout::setTime(unsigned int uTime){
	m_uTime = uTime;

	m_pTimeTxt->setVisible(true);
	m_pOpImg->loadTexture("pvp/pvp_op_info_gray.png", UI_TEX_TYPE_PLIST);
	string str = TimeEx::getStringTimeForInt(m_uTime, "ms");
	m_pTimeTxt->setText(str);

	this->schedule(schedule_selector(CArenaFightInfoLayout::updateTime), 1);
}

void CArenaFightInfoLayout::updateTime(float dt){
	if(m_uTime > 1){
		m_uTime--;
		string str = TimeEx::getStringTimeForInt(m_uTime, "ms");
		m_pTimeTxt->setText(str);
	}else{
		m_pTimeTxt->setVisible(false);
		m_pOpImg->loadTexture("pvp/pvp_op_info.png", UI_TEX_TYPE_PLIST);
		this->unschedule(schedule_selector(CArenaFightInfoLayout::updateTime));
	}
}


