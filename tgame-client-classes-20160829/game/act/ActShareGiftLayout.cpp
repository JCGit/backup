#include "ActShareGiftLayout.h"
#include "Macros.pb.h"
#include "ActControl.h"

#include "ActCfgMaster.h"
#include "FaceBookSDK.h"
#include "FBShareCfgMaster.h"
#include "../shop/ShopControl.h"
#include "../shop/RmbShopPanel.h"
//#include "../task/TaskControl.h"//test
ActShareGiftLayout::ActShareGiftLayout(){
	m_pRefLayout = NULL;
	m_uPkgType = 0;
	m_uState = 0;
}

ActShareGiftLayout::~ActShareGiftLayout(){

}

bool ActShareGiftLayout::init() {
	bool bRet = false;

	do {
		CC_BREAK_IF(!BaseLayout::init());

		initUI();

		bRet = true;
	} while(0);

	return bRet;
}

void ActShareGiftLayout::initLayout(UILayout* pRefLayout){
	if(NULL == pRefLayout) {
		return;
	}
	m_pRefLayout = pRefLayout;

	m_pTimeTxt = dynamic_cast<Label*>(m_pRefLayout->getChildByName("time_txt"));
	m_pContentTxt = dynamic_cast<Label*>(m_pRefLayout->getChildByName("content_txt"));

	m_pGetBtn = dynamic_cast<Button*>(m_pRefLayout->getChildByName("get_btn"));
	m_pBtnImg = dynamic_cast<ImageView*>(m_pGetBtn->getChildByName("label_img"));
	m_pGetBtn->addTouchEventListener(this, toucheventselector(ActShareGiftLayout::touchEventHandler));
}

void ActShareGiftLayout::touchEventHandler(CCObject *pSender, TouchEventType type) {
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

void ActShareGiftLayout::updateInfo(const vmsg::CSCouponActStatusItem& info) {
	setInfo(info);
}

void ActShareGiftLayout::setInfo(const CSCouponActStatusItem& stMsg){
	m_uPkgType = stMsg.type();
	m_uState = stMsg.status();
	
	const ActCfg* stCfg = ACT_CFG_MASTER->GetActCfgByType(m_uPkgType);
	if(stCfg == NULL){
		return;
	}

	m_pGetBtn->setTouchEnabled(true);
	m_pTimeTxt->setText(stCfg->time());
	m_pContentTxt->setText(stCfg->content());

	string str ;
	if (m_uPkgType == vmsg::SHARE_GIFT_ACT)
	{
		str = "act_ui/share_label.png";
	}else if (m_uPkgType == vmsg::FIRST_CHARGEDOUBLE_ACT)
	{
		str = "act_ui/charge_btn_label.png";
	}
	m_pBtnImg->loadTexture(str.c_str(), UI_TEX_TYPE_PLIST);
}

void ActShareGiftLayout::doGet(){
	//if(m_uState != 0){
	//	return;
	//}
	//P_TASK_CTRL->sendTriggerDailyTaskRqst(CS_CMD_DAILYTASK_TRIGGER);//test
	if (m_uPkgType == vmsg::SHARE_GIFT_ACT)
	{
		const FBShareCfg* pShareCfg = FBShare_CFG->GetFBShareCfg(1);
		if (!pShareCfg)
			return;

		FaceBookSDK::onShared(pShareCfg->name(), pShareCfg->link(), pShareCfg->picture(), pShareCfg->caption(), pShareCfg->description());
	}else if (m_uPkgType == vmsg::FIRST_CHARGEDOUBLE_ACT)
	{
		P_SHOP_CTRL->getRmbShopPanel()->open();
	}
	
}

void ActShareGiftLayout::setLayoutEnabel(bool bIsEnabel){
	m_pRefLayout->setVisible(bIsEnabel);
	m_pRefLayout->setEnabled(bIsEnabel);
	m_pGetBtn->setTouchEnabled(bIsEnabel);
}

void ActShareGiftLayout::initUI() {
	initLayoutFromJsonFile("ui_v2/ActNormalLayout.ExportJson");
	addChild(m_pRootLayout);

	m_pTimeTxt = dynamic_cast<Label*>(m_pRootLayout->getChildByName("time_txt"));
	m_pContentTxt = dynamic_cast<Label*>(m_pRootLayout->getChildByName("content_txt"));

	m_pGetBtn = dynamic_cast<Button*>(m_pRootLayout->getChildByName("get_btn"));
	m_pBtnImg = dynamic_cast<ImageView*>(m_pGetBtn->getChildByName("label_img"));
	m_pGetBtn->addTouchEventListener(this, toucheventselector(ActShareGiftLayout::touchEventHandler));
}
