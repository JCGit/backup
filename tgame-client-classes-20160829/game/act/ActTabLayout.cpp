#include "ActTabLayout.h"
#include "Resources.h"

#include "ActCfgMaster.h"

CActTabLayout::CActTabLayout(){
	m_bIsSelected = false;
}

CActTabLayout::~CActTabLayout(){

}

CActTabLayout* CActTabLayout::create(UILayout *pRefLayout) {
	CActTabLayout *pRet = new CActTabLayout();

	if(pRet != NULL && pRet->initWithLayout(pRefLayout)) {
		pRet->autorelease();
	} else {
		CC_SAFE_DELETE(pRet);
	}

	return pRet;
}

bool CActTabLayout::init(){
	bool bRet = false;

	do 
	{
		CC_BREAK_IF(!BaseLayout::init());

		initLayoutFromJsonFile("ui_v2/act_tab_ui.ExportJson");
		addChild(m_pRootLayout);

		initUI();

		bRet = true;
	} while (0);

	return bRet;
}

bool CActTabLayout::initWithLayout(UILayout *pRefLayout) {
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

void CActTabLayout::initUI(){
	m_pBgImg = dynamic_cast<ImageView*>(m_pRootLayout->getChildByName("bg_img"));
	m_pIconImg = dynamic_cast<ImageView*>(m_pRootLayout->getChildByName("icon_img"));

	m_pNameTxt = dynamic_cast<Label*>(m_pRootLayout->getChildByName("name_txt"));
}

void CActTabLayout::setSelected(bool var){
	if(m_bIsSelected == var){
		return;
	}

    m_bIsSelected = var;
    
	if(m_bIsSelected){
		m_pBgImg->loadTexture("act_ui/act_tab_down.png", UI_TEX_TYPE_PLIST);
	}else{
		m_pBgImg->loadTexture("act_ui/act_tab_up.png", UI_TEX_TYPE_PLIST);
	}
}

bool CActTabLayout::getSelected(){
	return m_bIsSelected;
}

void CActTabLayout::setInfo(const CSCouponActStatusItem& stMsg){
	m_stInfo = stMsg;

	updateUI();
}

void CActTabLayout::updateUI(){
	const ActCfg* stCfg = ACT_CFG_MASTER->GetActCfgByType(m_stInfo.type());
	if(stCfg == NULL){
		return;
	}

	m_pNameTxt->setText(stCfg->name());

	string iconStr = ICON_PATH + stCfg->acticon() + ".png";
	m_pIconImg->loadTexture(iconStr.c_str());
}

