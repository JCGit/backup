#include "ActNormalLayout.h"
#include "Macros.pb.h"
#include "ActControl.h"

#include "ActCfgMaster.h"

CActNormalLayout::CActNormalLayout(){
	m_pRefLayout = NULL;
	m_uPkgType = 0;
	m_uState = 0;
}

CActNormalLayout::~CActNormalLayout(){

}

bool CActNormalLayout::init() {
	bool bRet = false;

	do {
		CC_BREAK_IF(!BaseLayout::init());

		initUI();

		bRet = true;
	} while(0);

	return bRet;
}

void CActNormalLayout::initLayout(UILayout* pRefLayout){
	if(NULL == pRefLayout) {
		return;
	}
	m_pRefLayout = pRefLayout;

	m_pTimeTxt = dynamic_cast<Label*>(m_pRefLayout->getChildByName("time_txt"));
	m_pContentTxt = dynamic_cast<Label*>(m_pRefLayout->getChildByName("content_txt"));

	m_pGetBtn = dynamic_cast<Button*>(m_pRefLayout->getChildByName("get_btn"));
	m_pBtnImg = dynamic_cast<ImageView*>(m_pGetBtn->getChildByName("label_img"));
	m_pGetBtn->addTouchEventListener(this, toucheventselector(CActNormalLayout::touchEventHandler));
}

void CActNormalLayout::touchEventHandler(CCObject *pSender, TouchEventType type) {
	UIWidget *pWidget = dynamic_cast<UIWidget*>(pSender);
	const char *pszWidgetName = pWidget->getName();

	switch(type) {
	case TOUCH_EVENT_ENDED:
		if(std::strcmp(pszWidgetName, m_pGetBtn->getName()) == 0) {
			doGet();
		} 
		break;
	default:;
	}
}

void CActNormalLayout::updateInfo(const vmsg::CSCouponActStatusItem& info) {
	setInfo(info);
}

void CActNormalLayout::setInfo(const CSCouponActStatusItem& stMsg){
	m_uPkgType = stMsg.type();
	m_uState = stMsg.status();
	
	const ActCfg* stCfg = ACT_CFG_MASTER->GetActCfgByType(m_uPkgType);
	if(stCfg == NULL){
		return;
	}

	m_pGetBtn->setTouchEnabled(true);
	m_pTimeTxt->setText(stCfg->time());
	m_pContentTxt->setText(stCfg->content());

	string str = "act_ui/";
	if(stCfg->type() == PLATFORM_PKG){   //平台特权礼包
		str += "cd_key_label.png";
		//if(m_uState == 0){  //未领取
		//	str += "cd_key_label.png";
		//}else{
		//	str += "claimed_btn_label.png";
		//	m_pGetBtn->setTouchEnabled(false);
		//}
	}else if(stCfg->type() == NEWBIE_PKG){	//新手礼包
		str += "cd_key_label.png";
		//if(m_uState == 0){  //未领取
		//	str += "cd_key_label.png";
		//}else{
		//	str += "claimed_btn_label.png";
		//	m_pGetBtn->setTouchEnabled(false);
		//}
	}

	m_pBtnImg->loadTexture(str.c_str(), UI_TEX_TYPE_PLIST);
}

void CActNormalLayout::doGet(){
	//if(m_uState != 0){
	//	return;
	//}

	if(m_uPkgType == PLATFORM_PKG){   //平台特权礼包
		P_ACT_CTRL->getCDKPanel()->open();
	}else if(m_uPkgType == NEWBIE_PKG){	//新手礼包
		P_ACT_CTRL->getCDKPanel()->open();
	}
}

void CActNormalLayout::setLayoutEnabel(bool bIsEnabel){
	m_pRefLayout->setVisible(bIsEnabel);
	m_pRefLayout->setEnabled(bIsEnabel);
	m_pGetBtn->setTouchEnabled(bIsEnabel);
}

void CActNormalLayout::initUI() {
	initLayoutFromJsonFile("ui_v2/ActNormalLayout.ExportJson");
	addChild(m_pRootLayout);

	m_pTimeTxt = dynamic_cast<Label*>(m_pRootLayout->getChildByName("time_txt"));
	m_pContentTxt = dynamic_cast<Label*>(m_pRootLayout->getChildByName("content_txt"));

	m_pGetBtn = dynamic_cast<Button*>(m_pRootLayout->getChildByName("get_btn"));
	m_pBtnImg = dynamic_cast<ImageView*>(m_pGetBtn->getChildByName("label_img"));
	m_pGetBtn->addTouchEventListener(this, toucheventselector(CActNormalLayout::touchEventHandler));
}
