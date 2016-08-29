#include "DailyActListItem.h"
#include "DailyActControl.h"
#include "SceneManager.h"
#include "TimeEx.h"
#include "TextHelper.h"
#include "tip/TipControl.h"
#include "bosswar/BossWarControl.h"
#include "CCColorUtil.h"
#include "UserObj.h"

#include "DailyActCfgMaster.h"
#include "BossChallengeInstanceCfgMaster.h"

DailyActListItem::DailyActListItem(){
	m_bIsSelect = false;
}

DailyActListItem::~DailyActListItem()
{
}

DailyActListItem* DailyActListItem::create() {
	DailyActListItem* pitem = new DailyActListItem();

	if(pitem != NULL && pitem->init()) {
		pitem->autorelease();
	} else {
		CC_SAFE_DELETE(pitem);
	}

	return pitem;
}

DailyActListItem* DailyActListItem::create(UILayout *pRefLayout) {
	DailyActListItem *pRet = new DailyActListItem();

	if(pRet != NULL && pRet->initWithLayout(pRefLayout)) {
		pRet->autorelease();
	} else {
		CC_SAFE_DELETE(pRet);
	}

	return pRet;
}

bool DailyActListItem::init() {
	bool bRet = false;

	do {
		CC_BREAK_IF(!BaseLayout::init());

		initLayoutFromJsonFile("ui_v2/daily_act_item_ui.ExportJson");
		addChild(m_pRootLayout);

		initUI();

		bRet = true;
	} while(false);

	return bRet;
}

bool DailyActListItem::initWithLayout(UILayout *pRefLayout) {
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

void DailyActListItem::initUI(){
	m_pIconImg = dynamic_cast<UIImageView*>(m_pRootLayout->getChildByName("icon"));
	m_pTitleImg = dynamic_cast<UIImageView*>(m_pRootLayout->getChildByName("title_img"));

	m_pActTimeTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("act_time_txt"));
	m_pActPrizeTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("act_prize_txt"));
	m_pActStatTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("act_stat_txt"));

	m_pDummyBtn = dynamic_cast<UIButton*>(m_pRootLayout->getChildByName("dummy_btn"));
	m_pDummyBtn->addTouchEventListener(this, toucheventselector(DailyActListItem::touchEventHandler));

	m_pInfoLayout = dynamic_cast<Layout*>(m_pRootLayout->getChildByName("info_layout"));
	m_pTishenImg = dynamic_cast<UIImageView*>(m_pInfoLayout->getChildByName("tishen_ing"));
	m_pSelectImg = dynamic_cast<UIImageView*>(m_pInfoLayout->getChildByName("select_img"));
	m_pTishenLabelImg = dynamic_cast<UIImageView*>(m_pSelectImg->getChildByName("label_img"));
	m_pSelectImg->setTouchEnabled(true);
	m_pTishenLabelImg->setTouchEnabled(true);
	m_pTishenLabelImg->addTouchEventListener(this, toucheventselector(DailyActListItem::touchEventHandler));
	m_pSelectImg->addTouchEventListener(this, toucheventselector(DailyActListItem::touchEventHandler));
}

std::string DailyActListItem::getDescription() const {
	return "DailyActListItem";
}

void DailyActListItem::setActInfo(const CSActInfo& stMsg){
	m_stActInfo = stMsg;
	updateUI();
}

void DailyActListItem::updateStat(unsigned int uStat){
	if(m_stActInfo.actstat() == uStat){
		return;
	}

	m_stActInfo.set_actstat(uStat);

	updateUI();
}

void DailyActListItem::updateUI(){
	const DailyActCfg* stCfg = DAILY_ACT_CFG_MASTER->GetDailyActCfgByID(m_stActInfo.actid());

	string resStr = "daily_act_ui/" + stCfg->titleimg();
	m_pTitleImg->loadTexture(resStr.c_str(), UI_TEX_TYPE_PLIST);

	string iconStr = "icon/" + stCfg->acticon();
	m_pIconImg->loadTexture(iconStr.c_str());

	string fromStr = TimeEx::getStringTimeForInt(m_stActInfo.starttime(), "hm");
	string toStr = TimeEx::getStringTimeForInt(m_stActInfo.overtime(), "hm");
	m_pActTimeTxt->setText(fromStr + "-" + toStr);
	m_pActPrizeTxt->setText(stCfg->awarddesc());

	if(m_stActInfo.actid() == ACT_ID_CVZ){
		updateGuardAct();
	}else if(m_stActInfo.actid() == ACT_ID_MANUAL){
		updateMaualAct();
	}else if(m_stActInfo.actid() == ACT_ID_WORLD_BOSS){			//世界boss战
		updateWorldBossAct();
	}
}

void DailyActListItem::updateGuardAct(){
	m_pInfoLayout->setEnabled(false);
	m_pInfoLayout->setVisible(false);
	m_pDummyBtn->setBright(true);
	m_pDummyBtn->setTouchEnabled(true);
	m_pDummyBtn->setTitleText(TextStr(TEXT_DAILY_ACT_ENTER));
	CCColorUtil::setLabelColor(m_pActStatTxt, ccWHITE);

	unsigned int uStat = m_stActInfo.actstat();
	if(uStat == ACT_STAT_PREPARE || uStat == ACT_STAT_START){
		m_pDummyBtn->setEnabled(true);
		m_pDummyBtn->setVisible(true);
		m_pActStatTxt->setVisible(false);
	}else{
		m_pActStatTxt->setVisible(true);
		m_pDummyBtn->setEnabled(false);
		m_pDummyBtn->setVisible(false);
		m_pActStatTxt->setText(TextStr(TEXT_DAILY_ACT_END_STAT));
	}
}

void DailyActListItem::updateMaualAct(){
	m_pInfoLayout->setEnabled(false);
	m_pInfoLayout->setVisible(false);
	m_pDummyBtn->setBright(true);
	m_pDummyBtn->setTouchEnabled(true);
	m_pDummyBtn->setTitleText(TextStr(TEXT_DAILY_MANUAL_GET));
	CCColorUtil::setLabelColor(m_pActStatTxt, ccWHITE);

	unsigned int uStat = m_stActInfo.actstat();
	if(uStat == ACT_STAT_START){
		m_pDummyBtn->setEnabled(true);
		m_pDummyBtn->setVisible(true);
		m_pActStatTxt->setVisible(false);
	}else{
		m_pActStatTxt->setVisible(true);
		m_pDummyBtn->setEnabled(false);
		m_pDummyBtn->setVisible(false);
		m_pActStatTxt->setText(TextStr(TEXT_DAILY_ACT_END_STAT));
		if(uStat == ACT_STAT_OVER){
			CCColorUtil::setLabelColor(m_pActStatTxt, ccWHITE);
			m_pActStatTxt->setText(TextStr(TEXT_DAILY_ACT_END_STAT));
		}else if(uStat == ACT_STAT_PARTED){
			CCColorUtil::setLabelColor(m_pActStatTxt, ccGREEN);
			m_pActStatTxt->setText(TextStr(TEXT_DAILY_MANUAL_GETED));
		}
	}
}

void DailyActListItem::updateWorldBossAct(){
	CCColorUtil::setLabelColor(m_pActStatTxt, ccWHITE);
	m_pActStatTxt->setVisible(false);
	m_pDummyBtn->setBright(true);
	m_pDummyBtn->setTouchEnabled(true);
	m_pDummyBtn->setTitleText(TextStr(TEXT_DAILY_ACT_ENTER));

	m_bIsSelect = (m_stActInfo.worldbossact().delegated() == 1);
	unsigned int uStat = m_stActInfo.actstat();
	if(uStat == ACT_STAT_PREPARE || uStat == ACT_STAT_START){

		if(m_bIsSelect){
			m_pInfoLayout->setEnabled(true);
			m_pInfoLayout->setVisible(true);
			m_pSelectImg->setEnabled(false);
			m_pSelectImg->setVisible(false);
			m_pTishenImg->setVisible(true);
			m_pDummyBtn->setEnabled(false);
			m_pDummyBtn->setVisible(false);
		}else{
			m_pInfoLayout->setEnabled(false);
			m_pInfoLayout->setVisible(false);
			m_pDummyBtn->setEnabled(true);
			m_pDummyBtn->setVisible(true);
		}

	}else{
		m_pInfoLayout->setEnabled(true);
		m_pInfoLayout->setVisible(true);
		m_pTishenImg->setVisible(false);

		if(P_USER_OBJ->getUserInfo()->vipLv > 0){   //vip才显示替身
			m_pDummyBtn->setEnabled(false);
			m_pDummyBtn->setVisible(false);
			m_pSelectImg->setEnabled(true);
			m_pSelectImg->setVisible(true);
		}else{
			m_pDummyBtn->setBright(false);
			m_pDummyBtn->setTouchEnabled(false);
			m_pDummyBtn->setEnabled(true);
			m_pDummyBtn->setVisible(true);
			m_pSelectImg->setEnabled(false);
			m_pSelectImg->setVisible(false);
		}

		//m_pActStatTxt->setVisible(true);
		//m_pActStatTxt->setText(TextStr(TEXT_DAILY_ACT_END_STAT));
	}

	if(m_bIsSelect){
		m_pSelectImg->loadTexture("share/checkbox_blue_selected.png", UI_TEX_TYPE_PLIST);
		m_pTishenLabelImg->loadTexture("daily_act_ui/tishen_label_select.png", UI_TEX_TYPE_PLIST);
	}else{
		m_pSelectImg->loadTexture("share/checkbox_blue_normal.png", UI_TEX_TYPE_PLIST);
		m_pTishenLabelImg->loadTexture("daily_act_ui/tishen_label.png", UI_TEX_TYPE_PLIST);
	}
}

void DailyActListItem::updateWorldBossTishen(unsigned int uMark){
	if(!m_stActInfo.has_worldbossact()){
		return;
	}

	m_stActInfo.mutable_worldbossact()->set_delegated(uMark);
	m_bIsSelect = (m_stActInfo.worldbossact().delegated() == 1);
	if(uMark == 1){
		m_pSelectImg->loadTexture("share/checkbox_blue_selected.png", UI_TEX_TYPE_PLIST);
		m_pTishenLabelImg->loadTexture("daily_act_ui/tishen_label_select.png", UI_TEX_TYPE_PLIST);
	}else{
		m_pSelectImg->loadTexture("share/checkbox_blue_normal.png", UI_TEX_TYPE_PLIST);
		m_pTishenLabelImg->loadTexture("daily_act_ui/tishen_label.png", UI_TEX_TYPE_PLIST);
	}
}

void DailyActListItem::touchEventHandler(CCObject *pSender, TouchEventType type) {
    UIWidget *pWidget = dynamic_cast<UIWidget*>(pSender);
    const char *pszWidgetName = pWidget->getName();
    switch(type) {
    case TOUCH_EVENT_ENDED:
        if(std::strcmp(pszWidgetName, "dummy_btn") == 0) {
            btnClick();
		}else if(std::strcmp(pszWidgetName, "select_img") == 0 || std::strcmp(pszWidgetName, "label_img") == 0 ) {
			btnSelect();
		} 
        break;
    default:
        break;
    }
}

void DailyActListItem::btnClick(){
	if(m_stActInfo.actid() == ACT_ID_CVZ){
		//SceneManager::sharedSceneManager()->enterScene(50001);
	}else if(m_stActInfo.actid() == ACT_ID_MANUAL){
		P_DAILYACT_CTRL->getProxy()->sendGetManualActAwardRqst();
	}else if(m_stActInfo.actid() == ACT_ID_WORLD_BOSS){
		P_BOSS_WAR_CTRL->sendAttendOrLeaveRqst(1);
	}
}

void DailyActListItem::btnSelect(){
	unsigned int uCost = BOSS_CHALLENGE_CFG->GetBossChallengeCfgByType(BOSS_CHALLENGE_WORLD)->delegatecost();
	if(m_bIsSelect){  //取消
		P_BOSS_WAR_CTRL->sendDelegateRqst(0);
	}else{  //选中
		P_TIP_CTRL->GoldCostTip(uCost, this, dialogbtneventselector(DailyActListItem::doSelect), TextStr(TEXT_WORLD_BOSS_TISHEN_TIP));
	}
}

void DailyActListItem::doSelect(){
	P_BOSS_WAR_CTRL->sendDelegateRqst(1);
}

