#include "QianghunCard.h"
#include "defines.h"
#include "ItemUtils.h"
#include "CommonDialog.h"
#include "bag/BagControl.h"
#include "bag/ItemInfoPanel.h"
#include "TextHelper.h"
#include "NounHelper.h"
#include "GlobalCfgMaster.h"
#include "ToolUtils.h"
#include "QianghunControl.h"

using namespace std;

#define kInAngleZ        270 //里面卡牌的起始Z轴角度  
#define kInDeltaZ        90  //里面卡牌旋转的Z轴角度差  

#define kOutAngleZ       0   //封面卡牌的起始Z轴角度  
#define kOutDeltaZ       90  //封面卡牌旋转的Z轴角度差  

CQianghunCard::CQianghunCard(){
	m_uCost = 0;
	m_bIsFree = true;
	m_bIsOpen = false;
	m_bLocked = false;
	m_bIsTip = false;
	m_openAnimIn = NULL;
	m_openAnimOut = NULL;
}

CQianghunCard::~CQianghunCard(){
	CC_SAFE_RELEASE_NULL(m_openAnimIn);
	CC_SAFE_RELEASE_NULL(m_openAnimOut);
}

bool CQianghunCard::init(){
	bool bRet = false;

	do 
	{
		CC_BREAK_IF(!BaseLayout::init());

		initLayoutFromJsonFile("ui_v2/QiangHunCardUI.ExportJson");
		addChild(m_pRootLayout);

		m_pBackImg = dynamic_cast<ImageView*>(m_pRootLayout->getChildByName("back_card_img"));
		m_pFrontImg = dynamic_cast<ImageView*>(m_pRootLayout->getChildByName("front_card_img"));

		m_pCostLightImg = dynamic_cast<ImageView*>(m_pBackImg->getChildByName("light_img"));
		m_pCostTxt = dynamic_cast<UILabel*>(m_pCostLightImg->getChildByName("gold_val_txt"));
		m_pItemBg = dynamic_cast<ImageView*>(m_pFrontImg->getChildByName("bg_img"));
		ImageView* pIconImg = dynamic_cast<ImageView*>(m_pFrontImg->getChildByName("item_img"));

		m_pFrontImg->setVisible(false);

		m_pItem = IconCell::create();
		m_pItem->setName("card_item");
		m_pItem->showNumTxt();
		m_pItem->setZOrder(5);
		pIconImg->addChild(m_pItem);

		float duration = 0.25;
		CCFiniteTimeAction* keyTipFun = CCCallFunc::create(this, callfunc_selector(CQianghunCard::openKeyCom));
		m_openAnimIn = (CCActionInterval*)CCSequence::create(CCDelayTime::create(duration),  
			CCShow::create(),  
			CCOrbitCamera::create(duration, 1, 0, kInAngleZ, kInDeltaZ, 0, 0), 
			NULL);  
		m_openAnimIn->retain();  

		m_openAnimOut = (CCActionInterval*)CCSequence::create(CCOrbitCamera::create(duration, 1, 0, kOutAngleZ, kOutDeltaZ, 0, 0),  
			CCHide::create(),  
			CCDelayTime::create(duration),
			keyTipFun,
			NULL);  
		m_openAnimOut->retain();

		m_uCost = GLOBAL_CFG_MASTER->GetCfg().goldqianghun();
		m_pCostTxt->setText(intToString(m_uCost));		//读配置

		this->setAnchorPoint(ccp(0, 0));
		this->ignoreContentAdaptWithSize(false);
		this->setSize(m_pRootLayout->getContentSize());
		this->setTouchEnabled(true);
		m_pItemBg->addTouchEventListener(this, toucheventselector(CQianghunCard::touchEventHandler));

		bRet = true;
	} while (0);

	return bRet;
}

void CQianghunCard::openKeyCom(){
	if(!m_bIsTip){
		return;
	}

	COMMON_DIALOG->show(TextStr(TEXT_QIANGHUN_KEY_TIP), NULL, NounStr(OK));
}

void CQianghunCard::initClose(){
	if(m_pFrontImg->numberOfRunningActions() > 0){
		m_pFrontImg->stopAllActions();
	}
	if(m_pBackImg->numberOfRunningActions() > 0){
		m_pBackImg->stopAllActions();
	}

	m_pFrontImg->setVisible(false);
	m_pBackImg->setVisible(true);
	m_pItemBg->setTouchEnabled(false);
	m_pItem->clearCell();
	m_bIsOpen = false;
	m_bIsTip = false;

	//重置卡背面z轴视角
	float eyeX;
	float eyeY;
	float eyeZ;
	m_pBackImg->getCamera()->getEyeXYZ(&eyeX, &eyeY, &eyeZ);
	m_pBackImg->getCamera()->setEyeXYZ(eyeX, eyeY, 90);

	//重置卡正面z轴视角
	m_pFrontImg->getCamera()->getEyeXYZ(&eyeX, &eyeY, &eyeZ);
	m_pFrontImg->getCamera()->setEyeXYZ(eyeX, eyeY, 270);

	setIsFree(true);
	unLock();
}

void CQianghunCard::setIsFree(bool isFree){
	m_bIsFree = isFree;
	if(isFree){
		m_pCostLightImg->setVisible(false);
	}else{
		m_pCostLightImg->setVisible(true);
	}
}

void CQianghunCard::updateCost(unsigned int uCost){
	if(uCost){
		m_pCostTxt->setText(intToString(uCost));
		m_uCost = uCost;
	}else{
		m_pCostTxt->setText(intToString(GLOBAL_CFG_MASTER->GetCfg().goldqianghun()));		
		m_uCost = GLOBAL_CFG_MASTER->GetCfg().goldqianghun();
	}
}

void CQianghunCard::openByInfo(const CSQiangHunElmt& stInfo, bool isOpened, bool isKeyTip){
	m_stInfo = stInfo;
	m_bIsTip = false;

	updateUI();

	if(!isOpened){  //未打开
		if(isKeyTip && stInfo.iskey() == 1){	//翻开钥匙的提示
			m_bIsTip = true;
		}
		openCard();
	}else{
		m_pFrontImg->setVisible(true);
		m_pBackImg->setVisible(false);
		m_pItemBg->setTouchEnabled(true);
	}

	m_bIsOpen = true;
}

void CQianghunCard::updateUI(){
	m_pItem->setInfo(m_stInfo.type(), m_stInfo.cnt(), m_stInfo.cfgid(), ccGREEN);

	//string str = "qianghun/card_light_" + intToString(ItemUtils::getItemColor(m_stInfo.type(), m_stInfo.cfgid())) + ".png";
	//m_pFrontImg->loadTexture(str.c_str(), UI_TEX_TYPE_PLIST);

	//碎片类型要显示图标
	CToolUtils::adjustIconBgSize(m_stInfo.type(), m_stInfo.cfgid(), m_pItemBg);
	CToolUtils::adjustFragIcon(m_stInfo.type(), m_pItemBg);
}

void CQianghunCard::openCard(){
	if(m_bIsOpen){
		return;
	}

	m_pFrontImg->stopAllActions();
	m_pBackImg->stopAllActions();
	m_pFrontImg->runAction(m_openAnimIn);  
	m_pBackImg->runAction(m_openAnimOut);  
	m_pItemBg->setTouchEnabled(true);
}

void CQianghunCard::closeCard(){
	if(!m_bIsOpen){
		return;
	}

	m_pFrontImg->stopAllActions();
	m_pBackImg->stopAllActions();
	m_pFrontImg->runAction(m_openAnimIn->reverse());  
	m_pBackImg->runAction(m_openAnimOut->reverse());  
	m_pItemBg->setTouchEnabled(false);

	m_bIsOpen = false;
}

void CQianghunCard::lock(){
	this->setTouchEnabled(false);
	m_bLocked = true;
}

void CQianghunCard::unLock(){
	this->setTouchEnabled(true);
	m_bLocked = false;
}

void CQianghunCard::touchEventHandler(CCObject* pSender, TouchEventType type){
	UIWidget *pWidget = dynamic_cast<UIWidget*>(pSender);
	const char *pszWidgetName = pWidget->getName();
	string str(pszWidgetName);

	switch (type) {
	case TOUCH_EVENT_ENDED:
		if(strcmp(pszWidgetName, "bg_img") == 0) {
			if(!(m_pItem->getItemCfgid() == 0 && m_pItem->getItemType() == 0 && m_pItem->getItemNum() == 0)){
				P_BAG_CTRL->getItemInfoPanel()->open();
				P_BAG_CTRL->getItemInfoPanel()->updateInfo(m_pItem->getItemCfgid(), m_pItem->getItemType(), false);
			}		
		} 
	default:;
	}
}