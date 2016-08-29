#include "WeaponArrayPanel.h"
#include "WeaponLibControl.h"
#include "firearms/FireArmsObj.h"
#include "defines.h"
#include "ItemUtils.h"
#include "TextHelper.h"

CWeaponArrayPanel::CWeaponArrayPanel(){

}

CWeaponArrayPanel::~CWeaponArrayPanel(){
	if(m_stCellLst){
		m_stCellLst->removeAllObjects();
		CC_SAFE_RELEASE_NULL(m_stCellLst);
	}
}

bool CWeaponArrayPanel::init(){
	bool bRet = false;

	do 
	{
		CC_BREAK_IF(!BasePanel::init());

		initLayoutFromJsonFile("ui_v2/weapon_arr_ui.ExportJson");
		addChild(m_pRootLayout);

		initUI();

		bRet = true;
	} while (0);

	return bRet;
}

CCNode* CWeaponArrayPanel::getGuideNodeByName(const std::string &name) {
	CCNode *pRet = NULL;

	if(name == "back_btn") {
		if(m_pCloseBtn != NULL && m_pCloseBtn->isRunning() && m_pCloseBtn->isTouchEnabled()) {
			pRet = m_pCloseBtn;
		}
	} else if(name == "sub_weapon_cell") {
		CWeaponArrCell* pCell = m_stCellLst->count() > 0 ? dynamic_cast<CWeaponArrCell*>(m_stCellLst->objectAtIndex(0)) : NULL;

		if(pCell != NULL) {
			ImageView *pWeaponBgImg = pCell->getBgImg();
			
			if(pWeaponBgImg != NULL && pWeaponBgImg->isRunning() && pWeaponBgImg->isTouchEnabled()) {
				pRet = pWeaponBgImg;
			}
		}
	} else if (name == "main_weapon_cell")
	{
		pRet = m_pEquipWeaponBg;
	}

	return pRet;
}

void CWeaponArrayPanel::initUI(){
	m_pCloseBtn = dynamic_cast<Button*>(m_pRootLayout->getChildByName("close_btn"));
	m_pCloseBtn->addTouchEventListener(this, toucheventselector(CWeaponArrayPanel::touchEventHandler));

	m_pRuleBtn = dynamic_cast<Button*>(m_pRootLayout->getChildByName("rule_btn"));
	m_pRuleBtn->addTouchEventListener(this, toucheventselector(CWeaponArrayPanel::touchEventHandler));

	ImageView* pTextBg = dynamic_cast<ImageView*>(m_pRootLayout->getChildByName("txt_bg"));
	m_pAddTxt = dynamic_cast<Label*>(pTextBg->getChildByName("add_txt"));

	Layout* pEquipWeaponLayout = dynamic_cast<Layout*>(m_pRootLayout->getChildByName("weapon_0"));
	m_pEquipWeaponBg = dynamic_cast<ImageView*>(pEquipWeaponLayout->getChildByName("weapon_bg"));
	ImageView* pFrame = dynamic_cast<ImageView*>(pEquipWeaponLayout->getChildByName("frame_bg"));
	m_pEquipWeaponImg = dynamic_cast<ImageView*>(pFrame->getChildByName("weapon_icon"));
	m_pEquipWeaponBg->loadTexture("weapon_arr/weapon_equip_bg.png", UI_TEX_TYPE_PLIST);
	m_pEquipWeaponBg->setTouchEnabled(true);
	m_pEquipWeaponBg->addTouchEventListener(this, toucheventselector(CWeaponArrayPanel::touchEventHandler));

	m_stCellLst = CCArray::create();
	m_stCellLst->retain();

	for(unsigned int i = 1; i < MAX_ARR_CELL_NUM; i++){
		string str = "weapon_" + intToString(i);
		CWeaponArrCell* pCell = CWeaponArrCell::create(dynamic_cast<Layout*>(m_pRootLayout->getChildByName(str.c_str())));
		pCell->setName(str.c_str());
		m_stCellLst->addObject(pCell);
	}
}

void CWeaponArrayPanel::updateUI(){
	if(this->getIsInLoading()){
		this->closeLoading();
	}

	const vmsg::CSWeaponInBattleRsp& stWeaponInfo = P_ARM_OBJ->getWeaponInBattleInfo();

	//Ö÷ÎäÆ÷
	string iconUrl = ItemUtils::getItemIcon(THING_TYPE_WEAPON, stWeaponInfo.equippedweapon().cfgid());
	m_pEquipWeaponImg->loadTexture(iconUrl.c_str());

	//¸±ÎäÆ÷
	CWeaponArrCell* pCell = NULL;
	for(unsigned int i = 0; i < MAX_ARR_CELL_NUM - 1; i++){
		const vmsg::CSInbattleWeaponElmt& stInfo = stWeaponInfo.inbattleweapons(i);
		pCell = dynamic_cast<CWeaponArrCell*>(m_stCellLst->objectAtIndex(i));
		if(pCell){
			pCell->setInfo(stInfo);
		}
	}

	char str[150];
	snprintf(str, 150, TextStr(TEXT_WEAPON_ARR_ADD_TIP), stWeaponInfo.atkaddeff()/100, stWeaponInfo.hpaddeff()/100, stWeaponInfo.defaddeff()/100);
	m_pAddTxt->setText(str);

	GUIDE_SHOW(GUIDE_UI_WEAPON_ARRAY);
}

void CWeaponArrayPanel::touchEventHandler(CCObject *pSender, TouchEventType type){
	UIWidget *pWidget = dynamic_cast<UIWidget*>(pSender);
	const char *pszWidgetName = pWidget->getName();
	string strWidgetName = pWidget->getName();

	switch (type) {
	case TOUCH_EVENT_ENDED:
		if(strcmp(pszWidgetName, "close_btn") == 0) {
			GUIDE_TOUCH(pWidget);
			this->close();
		}else if(strcmp(pszWidgetName, "rule_btn") == 0) {
			P_WEAPON_CTRL->getArrRulePanel()->open();
		}else if(strcmp(pszWidgetName, "weapon_bg") == 0) {  //Ö÷ÎäÆ÷
			GUIDE_TOUCH(pWidget);
			const vmsg::CSWeaponInBattleRsp& stWeaponInfo = P_ARM_OBJ->getWeaponInBattleInfo();
			P_WEAPON_CTRL->getArrInfoPanel()->setInfo(stWeaponInfo.equippedweapon(), -1);
			P_WEAPON_CTRL->getArrInfoPanel()->open();
		}
		break;
	default:

		break;
	}
}

void CWeaponArrayPanel::doUpdateUI(){
	P_WEAPON_CTRL->sendQueryWeaponInBattleRqst();

	this->showLoading();
}

void CWeaponArrayPanel::open(){
	BasePanel::open();

	P_WEAPON_CTRL->sendQueryWeaponInBattleRqst();

	this->showLoading();
}

void CWeaponArrayPanel::close(){
	BasePanel::close();

	if(this->getIsInLoading()){
		this->closeLoading();
	}
}

