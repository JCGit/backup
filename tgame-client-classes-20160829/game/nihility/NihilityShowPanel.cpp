#include "NihilityShowPanel.h"
#include "HBox.h"
#include "ToolUtils.h"
#include "ItemUtils.h"
#include "IconCell.h"
#include "NihilityControl.h"
#include "lottery/WeaponLotResultCell.h"
#include "WeaponCfgMaster.h"
#include "ItemCfgMaster.h"

CNihilityShowPanel::CNihilityShowPanel(){
	m_pBaoxiangAni = NULL;
	m_pCurCellAni = NULL;
}

CNihilityShowPanel::~CNihilityShowPanel(){

}

bool CNihilityShowPanel::init() {
	bool bRet = false;

	do {
		CC_BREAK_IF(!BasePanel::init());

		initLayoutFromJsonFile("ui_v2/endless_show_panel_ui.ExportJson");
		addChild(m_pRootLayout);
		setUseBackForClose(false);

		m_pCellHBox = HBox::create();
		m_pCellHBox->initUI(CCSize(500, 180), 3, 0, 0);
		m_pCellHBox->setPosition(ccp(360, 50));
		m_pCellHBox->setTouchEnabled(false);
		addChild(m_pCellHBox);

		m_pDoOKBtn = dynamic_cast<UIButton*>(m_pRootLayout->getChildByName("sure_btn"));
		m_pDoOKBtn->addTouchEventListener(this, toucheventselector(CNihilityShowPanel::touchEventHandler));

		bRet = true;
	} 
	while(0);

	return bRet;
}

void CNihilityShowPanel::open(){
	BasePanel::open();
	CCArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo("effect/ut00012/ut00012.ExportJson");
	CCArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo("effect/ut00018/ut00018.ExportJson");

	m_pDoOKBtn->setVisible(false);
	m_pDoOKBtn->setTouchEnabled(false);
}


void CNihilityShowPanel::close(){
	BasePanel::close();
	m_stElmtDataLst.clear();

	m_pCellHBox->clearBox();

	if(m_pBaoxiangAni) {
		m_pBaoxiangAni->removeFromParentAndCleanup(true);
	}
    m_pBaoxiangAni = NULL;

	CToolUtils::removeArmatureFile("effect/ut00012/ut00012.ExportJson");
	CToolUtils::removeArmatureFile("effect/ut00018/ut00018.ExportJson");
}

void CNihilityShowPanel::touchEventHandler(CCObject *pSender, TouchEventType type) {
	UIWidget *pWidget = dynamic_cast<UIWidget*>(pSender);
	const char *pszWidgetName = pWidget->getName();

	switch(type) {
	case TOUCH_EVENT_ENDED:
		if(std::strcmp(pszWidgetName, "sure_btn") == 0) {
			close();
			P_NIHILITY_CTRL->getResultPanel()->open();
			P_NIHILITY_CTRL->getResultPanel()->updateUI();
		} 
	default:
		break;
	}
}

void CNihilityShowPanel::showResult(const CSNothingLandEndRsp& stRsp){
	if(!isOpen()) {
		return;
	}

	m_stElmtDataLst.clear();
	for(int i = 0; i < stRsp.awardlist_size(); ++i) {
		m_stElmtDataLst.push_back(stRsp.awardlist(i));
	}

	//±¦ÏäÌØÐ§
	CCPoint stPt = this->getSize();
	if(m_pBaoxiangAni) {
		m_pBaoxiangAni->removeFromParentAndCleanup(true);
	}

	m_pBaoxiangAni = CCArmature::create("ut00012");  //±¦Ïä
	if(m_pBaoxiangAni) {
		m_pBaoxiangAni->setPosition(stPt.x/2, stPt.y - 270);
		m_pBaoxiangAni->setScale(0.75f);
		m_pBaoxiangAni->getAnimation()->setMovementEventCallFunc(this, movementEvent_selector(CNihilityShowPanel::HandleBaoxiangEffEvent));
		m_pBaoxiangAni->getAnimation()->play("baoxiang", -1, -1, 0);
		addNode(m_pBaoxiangAni);
	} else {
		CCLOG("[error]fail to load baoxiang armature");
	}
}

void CNihilityShowPanel::HandleBaoxiangEffEvent(CCArmature * armature, MovementEventType type, const char * name) {
	if (type == COMPLETE){
		showNextElmt();
		if(m_pBaoxiangAni) {
			m_pBaoxiangAni->getAnimation()->play("baoxiang_xh", -1, -1, 1);
			m_pBaoxiangAni->getAnimation()->setMovementEventCallFunc(NULL, NULL);
		}
	}
}


void CNihilityShowPanel::HandleCellEffEvent(CCArmature * armature, MovementEventType type, const char * name) {
	if (type == COMPLETE){
		if(m_pCurCellAni) {
			m_pCurCellAni->getAnimation()->play(getLastCellAniName(true).c_str(), -1, -1, 1);
			m_pCurCellAni->getAnimation()->setMovementEventCallFunc(NULL, NULL);
		}
		m_pCurCellAni = NULL;
		showNextElmt();
	}
}

void CNihilityShowPanel::showNextElmt() {
	if(m_pCellHBox->getItemObjLst()->count() >= ENDLESS_MAX_ITEM) {
		m_pDoOKBtn->setVisible(true);
		m_pDoOKBtn->setTouchEnabled(true);
		return;
	}

	if(m_pCellHBox->getItemObjLst()->count() >= m_stElmtDataLst.size()){
		m_pDoOKBtn->setVisible(true);
		m_pDoOKBtn->setTouchEnabled(true);
		return;
	}

	const unsigned int idx = m_pCellHBox->getItemObjLst()->count();
	WeaponLotResultCell *pstCell = WeaponLotResultCell::create();
	const CSAwardElement& elmt = m_stElmtDataLst[idx];
	CCLOG("[trace]WeaponLotResultPanel::showNextElmt,type=%d,cnt=%d,id=%d", elmt.elementtype(), elmt.elementcnt(), elmt.elementid());
	pstCell->setInfo(elmt.elementtype(), elmt.elementcnt(), elmt.elementid());
	pstCell->hideName();
	m_pCellHBox->addItem(pstCell);


	m_pCurCellAni = CCArmature::create("ut00018"); 
	if(m_pCurCellAni) {
		CCPoint stPt = pstCell->getCellBg()->getPosition(); 
		m_pCurCellAni->setPosition(stPt.x, stPt.y);
		m_pCurCellAni->getAnimation()->setMovementEventCallFunc(this, movementEvent_selector(CNihilityShowPanel::HandleCellEffEvent));
		m_pCurCellAni->getAnimation()->play(getLastCellAniName(false).c_str(), -1, -1, 0);
		pstCell->addNode(m_pCurCellAni);
	} else {
		CCLOG("[error]fail to load cell armature");
	}
}

const string CNihilityShowPanel::getLastCellAniName(const bool isLoopAni) {
	int idx = m_pCellHBox->getItemObjLst()->count() - 1;
	WeaponLotResultCell* pCell = (WeaponLotResultCell*)(m_pCellHBox->getItemObj(idx) );
	if(NULL == pCell) {
		return "x_white";
	}
	string strName("");
	const int iElmtType = pCell->getCell()->getItemType();
	const int iElmtID = pCell->getCell()->getItemCfgid();
	if(THING_TYPE_WEAPON == iElmtType) {
		strName = "d_" + getColorString(WEAPON_CFG_MASTER->getWeaponLv(iElmtID));
	} else if(THING_TYPE_ITEM == iElmtType) {
		strName = "x_" + getColorString(ITEM_CFG_MASTER->GetItemColor(iElmtID));
	} else if(THING_TYPE_EQUIP == iElmtType) {
		strName = "x_" + getColorString(ItemUtils::getItemColor(THING_TYPE_EQUIP, iElmtID));
	} else {
		strName = "x_white";
	}
	if(isLoopAni) {
		strName += "_xh";
	}
	return strName;
}


const std::string CNihilityShowPanel::getColorString(const int iColor) {
	switch(iColor) {
	case 1:
		return "white";
	case 2:
		return "green";
	case 3:
		return "blue";
	case 4:
		return "purple";
	case 5:
		return "gold";
	default:
		return "white";
	}
}

