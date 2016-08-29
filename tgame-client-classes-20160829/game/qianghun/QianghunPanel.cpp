#include "QianghunPanel.h"
#include "QianghunControl.h"
#include "QianghunInfoItem.h"
#include "defines.h"
#include "CommonDialog.h"
#include "NounHelper.h"
#include "TextHelper.h"
#include "bag/BagControl.h"
#include "game/UserObj.h"
#include "ItemUtils.h"
#include "ToolUtils.h"
#include "vlib/CVRand.h"
#include "tip/TipControl.h"
#include "bag/BagControl.h"
#include "bag/ItemInfoPanel.h"

#include "GlobalCfgMaster.h"
#include "QiangHunCfgMaster.h"

using namespace vmsg;
using namespace std;

#define MIN_SCROLL_DISTANCE 16.0f

CQianghunPanel::CQianghunPanel()
:m_pFloorImg(NULL)
,m_pPrevBtn(NULL)
,m_pNextBtn(NULL)
,m_nWholeScrollDirection(0)
{
	m_uCurLayer = 0;
	m_uMaxLayer = 0;
	m_uFreeNum = 0;
	m_uCurFlopNum = 0;
	m_uCardIdx = 0;
	m_uGotoLayer = 0;
}

CQianghunPanel::~CQianghunPanel(){

}

bool CQianghunPanel::init(){
	bool bRet = false;

	do 
	{
		CC_BREAK_IF(!BasePanel::init());

		CCDirector *pDirector = CCDirector::sharedDirector();
		CCPoint screenOrigin = pDirector->getVisibleOrigin();
		CCSize screenSize = pDirector->getVisibleSize();

		initLayoutFromJsonFile("ui_v2/QiangHunPanelUI.ExportJson");
		addChild(m_pRootLayout);

		m_pFloorImg = dynamic_cast<ImageView*>(m_pRootLayout->getChildByName("level_img"));
		m_pNumTxt = dynamic_cast<Label*>(m_pRootLayout->getChildByName("time_txt"));

		m_pCloseBtn = dynamic_cast<UIButton*>(m_pRootLayout->getChildByName("close_btn"));
		m_pCloseBtn->addTouchEventListener(this, toucheventselector(CQianghunPanel::touchEventHandler));

		m_pPrevBtn = dynamic_cast<UIButton*>(m_pRootLayout->getChildByName("left_flip_btn"));
		m_pPrevBtn->addTouchEventListener(this, toucheventselector(CQianghunPanel::touchEventHandler));

		m_pNextBtn = dynamic_cast<UIButton*>(m_pRootLayout->getChildByName("right_flip_btn"));
		m_pNextBtn->addTouchEventListener(this, toucheventselector(CQianghunPanel::touchEventHandler));

		float cellX;
		float cellY;
		for(unsigned int i = 0 ; i < MAX_CARD_NUM; i++){
			CQianghunCard* pItem = CQianghunCard::create();
			string str = "card_" + to_string(i);
			pItem->setName(str.c_str());
			pItem->initClose();
			cellX = 455 + (i%3)*(pItem->getSize().width + 24);
			cellY = 430 - (i/3)*(pItem->getSize().height + 10);
			pItem->setPosition(ccp(cellX, cellY));
			m_stCellPos.push_back(ccp(cellX, cellY));
			m_stItemLst.AddOneItem(pItem);
			m_pRootLayout->addChild(pItem);
			pItem->addTouchEventListener(this, toucheventselector(CQianghunPanel::touchEventHandler));

			if(i == 4){
				m_stMidCellPos = ccp(cellX, cellY);
			}
		}

		for(unsigned int i = 0; i < 2; i++){
			string str = "cell_" + intToString(i);
			ImageView* pIconBg = dynamic_cast<ImageView*>(m_pRootLayout->getChildByName(str.c_str()));
			IconCell* pCell = IconCell::create();
			pCell->setZOrder(5);
			m_stCellLst.AddOneItem(pCell);
			pIconBg->addChild(pCell);
			pIconBg->addTouchEventListener(this, toucheventselector(CQianghunPanel::touchEventHandler));
			if(i == 0){
				m_stInitCellPos = pIconBg->getPosition();
			}
		}

		for(unsigned int i = 0; i < MAX_LEVEL; i++){
			string str = "level_bg_" + intToString(i);
			ImageView* pImg = dynamic_cast<ImageView*>(m_pRootLayout->getChildByName(str.c_str()));
			pImg->setTouchEnabled(true);
			pImg->addTouchEventListener(this, toucheventselector(CQianghunPanel::touchEventHandler));
			m_stLevLst.AddOneItem(pImg);
		}

		m_uFreeNum = GLOBAL_CFG_MASTER->GetCfg().freeqianghuncnt();
		updateInfoLst();

		bRet = true;
	} while (0);

	return bRet;
}

void CQianghunPanel::updateInfoLst(){
	unsigned int size = QIANGHUN_CFG_MASTER->GetMaxLayer();
	for(unsigned int i = 0; i < size; i++){
		/*CQianghunInfoItem* pItem = CQianghunInfoItem::create();
		pItem->setLayer(i + 1);
		m_pAwardsListView->pushBackCustomItem(pItem);*/
	}
}

void CQianghunPanel::updateUI(){
	if(this->getIsInLoading()){
		this->closeLoading();
	}

	reset();

	const CSQiangHunInfo& stInfo = P_QIANGHUN_CTRL->getQianghunInfo();
	m_uCurLayer = stInfo.layer();
	m_uMaxLayer = stInfo.maxlayer();
	m_uCurFlopNum = stInfo.elmts_size();

	if(stInfo.elmts_size() <= 0){  //未翻过牌，播放洗牌特效
		playShuffle();
	}else{
		unschedule(schedule_selector(CQianghunPanel::showAllCard));
		unschedule(schedule_selector(CQianghunPanel::hideAllCard));
		unschedule(schedule_selector(CQianghunPanel::showAllCard));
		unschedule(schedule_selector(CQianghunPanel::shuffleCard));
		unschedule(schedule_selector(CQianghunPanel::dealDone));

		reset();

		for(int i = 0; i < stInfo.elmts_size(); i++){
			CSQiangHunElmt stElmt = stInfo.elmts(i);
			CQianghunCard* pItem = m_stItemLst[stElmt.pos()];
			pItem->openByInfo(stElmt, true);
		}
	}

	string str = "qianghun/level_" + intToString(m_uCurLayer - 1) + ".png";
	m_pFloorImg->loadTexture(str.c_str(), UI_TEX_TYPE_PLIST);

	checkFlopFree();
	updateBtn();
	updateAwards(m_uCurLayer);

}

void CQianghunPanel::updateCard(unsigned int pos){
	if(pos < 0 || pos > MAX_CARD_NUM) {
		return;
	}

	const CSQiangHunElmt& stInfo = P_QIANGHUN_CTRL->getOpenCardInfo();
	CQianghunCard* pItem = m_stItemLst[pos];
	pItem->openByInfo(stInfo, false);

	checkFlopFree();
	m_uMaxLayer = P_QIANGHUN_CTRL->getQianghunInfo().maxlayer();
	updateBtn();
}

void CQianghunPanel::updateAwards(int floor) {
	unsigned int lev = P_USER_OBJ->getUserInfo()->level;
	const QiangHunCfg* stCfg = QIANGHUN_CFG_MASTER->GetCfgByLayerAndLev(floor, lev);
	if(stCfg == NULL){
		return;
	}

	unsigned int uNum = 0;
	for(int i = 0; i < 2; i++){
		string str = "cell_" + intToString(i);
		ImageView* pIconBg = dynamic_cast<ImageView*>(m_pRootLayout->getChildByName(str.c_str()));
		IconCell* pCell = m_stCellLst[i];
		bool bIsVis = true;
		if(i < stCfg->getelmts_size()){
			uNum++;
			const QiangHunGetElement &stElmt = stCfg->getelmts(i);
			bIsVis = true;
			pCell->setInfo(stElmt.type(), 1, stElmt.cfgid());

			//碎片类型要显示图标
			CToolUtils::adjustIconBgSize(stElmt.type(), stElmt.cfgid(), pIconBg);
			CToolUtils::adjustFragIcon(stElmt.type(), pIconBg);
		}else{
			bIsVis = false;
		}

		pIconBg->setVisible(bIsVis);
		pIconBg->setTouchEnabled(bIsVis);
		pCell->setVisible(bIsVis);
	}

	ImageView* pIconBg = dynamic_cast<ImageView*>(m_pRootLayout->getChildByName("cell_0"));
	if(uNum == 1){
		pIconBg->setPositionX(225);
	}else{
		pIconBg->setPositionX(m_stInitCellPos.x);
	}
}

void CQianghunPanel::updateBtn(){
	//按钮状态改变
	m_pPrevBtn->setBright(true);
	m_pPrevBtn->setTouchEnabled(true);
	m_pNextBtn->setBright(true);
	m_pNextBtn->setTouchEnabled(true);
	if(m_uCurLayer <= 1){
		m_pPrevBtn->setBright(false);
		m_pPrevBtn->setTouchEnabled(false);
	}

	if(m_uCurLayer >= m_uMaxLayer){
		m_pNextBtn->setBright(false);
		m_pNextBtn->setTouchEnabled(false);
	}

	for(unsigned int i = 0; i < MAX_LEVEL; i++){
		ImageView* pImg = m_stLevLst[i];
		ImageView* pLockImg = dynamic_cast<ImageView*>(pImg->getChildByName("lock_img"));
		if(i < m_uMaxLayer){
			string str = "";
			if(i == m_uCurLayer - 1){
				str = "qianghun/level_light_img.png";
			}else{
				str = "qianghun/level_normal_img.png";
			}
			pImg->loadTexture(str.c_str(), UI_TEX_TYPE_PLIST);
			pLockImg->setVisible(false);
		}else{
			pLockImg->setVisible(true);
		}
	}
}

void CQianghunPanel::checkFlopFree(){
	const CSQiangHunInfo& stInfo = P_QIANGHUN_CTRL->getQianghunInfo();
	m_uCurFlopNum = stInfo.elmts_size();
	if(m_uCurFlopNum >= m_uFreeNum){
		m_pNumTxt->setText("0/" + intToString(m_uFreeNum));
	}else{
		m_pNumTxt->setText(intToString(m_uFreeNum - m_uCurFlopNum) + "/" + intToString(m_uFreeNum));
	}

	CQianghunCard* pItem = NULL;
	for(unsigned int i = 0 ; i < MAX_CARD_NUM; i++){
		pItem = m_stItemLst[i];
		if(!pItem->getIsOpen()){
			if(pItem->getIsFree() && m_uCurFlopNum >= m_uFreeNum){ 
				pItem->setIsFree(false);
			}else if(!pItem->getIsFree() && m_uCurFlopNum < m_uFreeNum){
				pItem->setIsFree(true);
			}
			pItem->updateCost(stInfo.nextcost());
		}
	}
}

void CQianghunPanel::touchEventHandler(CCObject* pSender, TouchEventType type){
	UIWidget *pWidget = dynamic_cast<UIWidget*>(pSender);
	const char *pszWidgetName = pWidget->getName();
	string str(pszWidgetName);

	switch (type) {
	case TOUCH_EVENT_ENDED:
		if(str.find("card_") != std::string::npos){
			//先检查背包看看
			if(P_BAG_CTRL->isFull()) {
				COMMON_DIALOG->show(TextStr(TEXT_BAG_FULL_OP_TIP));
				return;
			}

			 string sub = str.substr(str.find_first_not_of("card_"));
			 int iIdx = atoi(sub.c_str());
			  if(iIdx >= 0 && iIdx < MAX_CARD_NUM) {
				   CQianghunCard* pItem = m_stItemLst[iIdx];
				   m_uCardIdx = iIdx;
				   if(pItem->getLocked() == false && pItem->getIsOpen() == false){
					   if(pItem->getIsFree()){
						   flopFun();
					   }else{
						   P_TIP_CTRL->GoldCostTip(pItem->getCost(), this, dialogbtneventselector(CQianghunPanel::flopFun));
					   }
				   }
			  }
		} else if(strcmp(pszWidgetName, m_pPrevBtn->getName()) == 0){
			preLayer();
		} else if(strcmp(pszWidgetName, m_pNextBtn->getName()) == 0){
			nextLayer();
		} else if(std::strcmp(pszWidgetName, m_pCloseBtn->getName()) == 0) {
			this->close();
		} else if(str.find("cell_") != string::npos){
			string sub = str.substr(str.find_first_not_of("cell_"));
			int iIdx = atoi(sub.c_str());
			if(iIdx >= 0 && iIdx < 2){
				IconCell* pCell = m_stCellLst[iIdx];
				P_BAG_CTRL->getItemInfoPanel()->open();
				P_BAG_CTRL->getItemInfoPanel()->updateInfo(pCell->getItemCfgid(), pCell->getItemType(), false);
			}
		} else if(str.find("level_bg_") != string::npos){
			string sub = str.substr(str.find_first_not_of("level_bg_"));
			int iIdx = atoi(sub.c_str());
			if(iIdx >= 0 && iIdx < MAX_LEVEL){
				gotoLayer(iIdx + 1);
			}
		}
	default:

		break;
	}
}

std::string CQianghunPanel::getCountDesc(unsigned int count){
	switch (count)
	{
	case 1:
		return TextStr(TEXT_QIANGHUN_FEW);
		break;
	case 2:
		return TextStr(TEXT_QIANGHUN_BIT);
		break;
	case 3:
		return TextStr(TEXT_QIANGHUN_MORE);
		break;
	case 4:
		return TextStr(TEXT_QIANGHUN_MASS);
		break;
	default:
		return TextStr(TEXT_QIANGHUN_FEW);
		break;
	}
}

void CQianghunPanel::flopFun(){
	P_QIANGHUN_CTRL->sendQiangHunFlopRqst(m_uCardIdx, m_uCurLayer);
	lock();
}

void CQianghunPanel::preLayer(){
	if(m_uCurLayer <= 1){
		return;
	}

	unsigned int layer = m_uCurLayer - 1;
	P_QIANGHUN_CTRL->sendQiangHunLayerQueryRqst(layer);
}

void CQianghunPanel::nextLayer(){
	if(m_uCurLayer >= m_uMaxLayer){
		return;
	}

	if(m_uCurFlopNum < MAX_CARD_NUM){  //还有牌没有翻
		COMMON_DIALOG->show(TextStr(TEXT_QIANGHUN_NEXT_LEV_TIP), this, NounStr(OK), 
			NounStr(CANCEL), dialogbtneventselector(CQianghunPanel::nextFun));
		return;
	}

	unsigned int layer = m_uCurLayer + 1;
	P_QIANGHUN_CTRL->sendQiangHunLayerQueryRqst(layer);
}

void CQianghunPanel::nextFun(){
	unsigned int layer = m_uCurLayer + 1;
	P_QIANGHUN_CTRL->sendQiangHunLayerQueryRqst(layer);
}

void CQianghunPanel::gotoLayer(unsigned int uLayer){
	if(uLayer > m_uMaxLayer || uLayer == m_uCurLayer){
		return;
	}

	m_uGotoLayer = uLayer;

	bool bIsTip = true;
	if(uLayer < m_uCurLayer){
		bIsTip = false;
	}

	if(bIsTip && m_uCurFlopNum < MAX_CARD_NUM){  //还有牌没有翻
		COMMON_DIALOG->show(TextStr(TEXT_QIANGHUN_NEXT_LEV_TIP), this, NounStr(OK), 
			NounStr(CANCEL), dialogbtneventselector(CQianghunPanel::toLayer));
		return;
	}

	m_uCurLayer = uLayer;
	P_QIANGHUN_CTRL->sendQiangHunLayerQueryRqst(m_uCurLayer);
}

void CQianghunPanel::toLayer(){
	m_uCurLayer = m_uGotoLayer;
	P_QIANGHUN_CTRL->sendQiangHunLayerQueryRqst(m_uCurLayer);
}

void CQianghunPanel::lock(){
	for(unsigned int i = 0 ; i < MAX_CARD_NUM; i++){
		CQianghunCard* pItem = m_stItemLst[i];
		pItem->lock();
	}
}

void CQianghunPanel::unLock(){
	for(unsigned int i = 0 ; i < MAX_CARD_NUM; i++){
		CQianghunCard* pItem = m_stItemLst[i];
		pItem->unLock();
	}
}

void CQianghunPanel::reset(){
	for(unsigned int i = 0 ; i < MAX_CARD_NUM; i++){
		CQianghunCard* pItem = m_stItemLst[i];
		CCPoint stPos = m_stCellPos.at(i);
		pItem->setPosition(stPos);
		pItem->initClose();

		if(pItem->numberOfRunningActions() > 0){
			pItem->stopAllActions();
		}
	}
}

void CQianghunPanel::open(){
    //先检查背包看看
    if(P_BAG_CTRL->isFull()) {
        COMMON_DIALOG->show(TextStr(TEXT_BAG_FULL_OP_TIP));
        return;
    }

	BasePanel::open();

	CCDirector::sharedDirector()->setProjection(kCCDirectorProjection2D);		//注意,设置为正投影

	P_QIANGHUN_CTRL->sendQiangHunQueryRqst();
}

void CQianghunPanel::close(){
	CCDirector::sharedDirector()->setProjection(kCCDirectorProjection3D);		//重置为默认值

    BasePanel::close();
    P_BAG_CTRL->sendQueryBagInfoRqst();
}

void CQianghunPanel::playShuffle(){
	lock();

	scheduleOnce(schedule_selector(CQianghunPanel::showAllCard), 0.5f);
}

void CQianghunPanel::showAllCard(float dt){
	

	unsigned int lev = P_USER_OBJ->getUserInfo()->level;
	const QiangHunCfg* stCfg = QIANGHUN_CFG_MASTER->GetCfgByLayerAndLev(m_uCurLayer, lev);

	for(int i = 0; i < stCfg->elmtcfgs_size(); i++){
		const QiangHunElementCfg& stElmtCfg = stCfg->elmtcfgs(i);
		CSQiangHunElmt stElmt;
		stElmt.set_iskey(stElmtCfg.iskey());
		stElmt.set_type(stElmtCfg.type());
		stElmt.set_cfgid(stElmtCfg.cfgid());
		stElmt.set_cnt(stElmtCfg.cnt());
		stElmt.set_pos(i);

		if(i < MAX_CARD_NUM){
			CQianghunCard* pItem = m_stItemLst[i];
			pItem->openByInfo(stElmt, false, false);
		}
	}

	scheduleOnce(schedule_selector(CQianghunPanel::hideAllCard), 2.0f);
}

void CQianghunPanel::hideAllCard(float dt){
	for(unsigned int i = 0 ; i < MAX_CARD_NUM; i++){
		CQianghunCard* pItem = m_stItemLst[i];
		pItem->closeCard();
	}

	scheduleOnce(schedule_selector(CQianghunPanel::shuffleCard), 1.0f);
}

void CQianghunPanel::shuffleCard(float dt){
	float delay = 0;
	CCPoint stPos = CCPointZero;
	for(unsigned int i = 0 ; i < MAX_CARD_NUM; i++){
		CQianghunCard* pItem = m_stItemLst[i];
		if(i != 4){
			stPos = m_stCellPos.at(i);
			delay = (50 + CRandom::GetRand(150))*1.0/1000;
			CCMoveTo* pMove = CCMoveTo::create(0.3f, m_stMidCellPos);
			CCMoveTo* pMoveBack = CCMoveTo::create(0.3f, stPos);
			CCSequence* pSeq = CCSequence::create(CCDelayTime::create(delay), pMove, CCDelayTime::create(0.3f), pMoveBack, NULL);
			pItem->runAction(pSeq);
		}
	}

	scheduleOnce(schedule_selector(CQianghunPanel::dealDone), 1.0f);
}

void CQianghunPanel::dealDone(float dt){
	unLock();
}

