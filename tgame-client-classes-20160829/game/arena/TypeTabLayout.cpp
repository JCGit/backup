#include "TypeTabLayout.h"

#include "ArenaLvCfgMaster.h"

CTypeTabLayout::CTypeTabLayout(){
	m_pBtn = NULL;
	m_uType = 0;
	m_bIsSelect = false;
}

CTypeTabLayout::~CTypeTabLayout(){

}

CTypeTabLayout* CTypeTabLayout::create(UILayout *pRefLayout) {
	CTypeTabLayout *pRet = new CTypeTabLayout();

	if(pRet != NULL && pRet->initWithLayout(pRefLayout)) {
		pRet->autorelease();
	} else {
		CC_SAFE_DELETE(pRet);
	}

	return pRet;
}

bool CTypeTabLayout::initWithLayout(UILayout *pRefLayout) {
	bool bRet = false;

	do {
		CC_BREAK_IF(!BaseLayout::init());

		initLayoutWithUICopy(pRefLayout);
		addChild(m_pRootLayout);

		initUI();

		bRet = true;
	} while (0);

	return bRet;
}

bool CTypeTabLayout::init(){
	bool bRet = false;

	do 
	{
		CC_BREAK_IF(!BaseLayout::init());

		initLayoutFromJsonFile("ui_v2/arena_type_tab_ui.ExportJson");
		addChild(m_pRootLayout);

		initUI();

		bRet = true;
	} while (0);

	return bRet;
}

void CTypeTabLayout::initUI(){
	m_pBtn = dynamic_cast<UIButton*>(m_pRootLayout->getChildByName("tab_btn"));
	m_pBtn->setTitleColor(ccc3(0x7b, 0xbc, 0xe1));
}

void CTypeTabLayout::setType(unsigned int uType){
	m_uType = uType;
	const ArenaLvCfg* stCfg = ARENA_LV_CFG_MASTER->GetOneCfg(uType);
	m_pBtn->setTitleText(stCfg->name());
}


