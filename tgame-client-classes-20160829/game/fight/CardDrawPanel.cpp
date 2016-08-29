#include "CardDrawPanel.h"
#include "FightControl.h"
#include "LevelObj.h"
#include "LevelControl.h"
#include "FightObj.h"
#include "WarMapCfgMaster.h"
#include "GlobalTime.h"
#include "defines.h"
#include "PopTipLayer.h"
#include "ToolUtils.h"
#include "UserObj.h"
#include "SceneManager.h"
#include "tip/TipControl.h"
#include "evtlog/EvtLog.h"

using namespace std;

CCardDrawPanel::CCardDrawPanel()
:timeTxt(NULL),
sureBtn(NULL),
pCards(NULL),
curCard(NULL),
costCurCard(NULL),
timeValueTxt(NULL),
b_isOpenFreeCard(false)
{
	freeTimes = DEFAULT_TIMES;
	costTimes = DEFAULT_TIMES;
}

CCardDrawPanel::~CCardDrawPanel(){
	CC_SAFE_RELEASE_NULL(pCards);
}

bool CCardDrawPanel::init(){
	bool bRet = false;

	do{
		CC_BREAK_IF(!BasePanel::init());

		initLayoutFromJsonFile("ui_v2/card_panel_ui.ExportJson");
		addChild(m_pRootLayout);

		pCards = CCArray::create();
		pCards->retain();

		initUI();

		bRet = true;
	}while(0);

	return bRet;
}

void CCardDrawPanel::initUI(){
	timeTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("time_txt"));
	timeValueTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("time_value_txt"));
	sureBtn = dynamic_cast<UIButton*>(m_pRootLayout->getChildByName("sure_btn"));

    m_pPanelBg = dynamic_cast<UIImageView*>(m_pRootLayout->getChildByName("bg"));
    m_pPanelBg->loadTexture("ui_bg/loading_ui_bg.png", UI_TEX_TYPE_PLIST);

	sureBtn->setVisible(false);

	sureBtn->addTouchEventListener(this, toucheventselector(CCardDrawPanel::touchEventHandler));
}

void CCardDrawPanel::touchEventHandler(CCObject *pSender, TouchEventType type){
	UIWidget* pWidget = dynamic_cast<UIWidget*>(pSender);
	const char* pName = pWidget->getName();
	string strName = pWidget->getName();
	switch (type)
	{
	case TOUCH_EVENT_ENDED:
			if(strcmp(strName.substr(0, 9).c_str(), "card_cell") == 0){
				CCardCell* pCell = dynamic_cast<CCardCell*>(pWidget);
				openCard(pCell);
			}else if(strcmp(pName, "sure_btn") == 0){
				if(freeTimes <= 0){
					this->close();
					SceneManager::sharedSceneManager()->enterScene(P_USER_OBJ->getUserInfo()->sceneID);

                    if(P_FIGHT_OBJ->getFightMapID() == WAR_MAP_CFG->GetCfg().warmapcfg(0).id()) {
                        P_EVT_LOG->log(EVT_LOG_FIRST_WAR_END, 0);
                    }
				}else{
					srand((unsigned int)GlobalTime::getTime());
					unsigned int idx = 5*(rand()/(RAND_MAX + 1.0f));
					CCardCell* pCell = (CCardCell*)pCards->objectAtIndex(idx);
					openCard(pCell);
				}
			}
			break;
		default:
			break;
	}
}


void CCardDrawPanel::openCard(CCardCell* pCell){
	string strName = pCell->getName();
	
	if(pCell->getCostType() == CCardCell::FREE_CARD){
		if(freeTimes <= 0){  //没有免费次数
			CCLOG("you have no free times to draw card!!");
			return;
		}

		curCard = pCell;
		b_isOpenFreeCard = true;
        lockCards();
        P_FIGHT_CTRL->getFightProxy()->sendWarMapCardRqst(P_FIGHT_OBJ->getFightMapID(), 1);
	} else if(pCell->getCostType() == CCardCell::COST_CARD){
		if(costTimes <= 0){  //没有花费次数
			CCLOG("you have no cost times to draw card!!");
			return;
		}
		costCurCard = pCell;
		b_isOpenFreeCard = false;
		
		P_TIP_CTRL->GoldCostTip(10, this, dialogbtneventselector(CCardDrawPanel::openCostCard));
	}
}

void CCardDrawPanel::openCostCard(){
	lockCards();
	P_FIGHT_CTRL->getFightProxy()->sendWarMapCardRqst(P_FIGHT_OBJ->getFightMapID(), 0);
}

void CCardDrawPanel::openOtherFreeCards(){
	CCardCell* pCell;
	unsigned int idx = 0;
	vector<CardElement> otherCardsInfo = getOtherCardsInfo();
	for(unsigned int i = 0; i < pCards->count(); i++){
		pCell = (CCardCell*)pCards->objectAtIndex(i);
		if(pCell->getCostType() == CCardCell::FREE_CARD){
			if(pCell->getIsOpen() == false){
				pCell->openCard(false);

				if(idx < otherCardsInfo.size()){
					CardElement cardInfo = otherCardsInfo[idx];
					pCell->setItemInfo(cardInfo.elementid(), cardInfo.elementtype(), cardInfo.elementcnt());
					idx++;
				}
			}
		}
	}

    //sureBtn->setVisible(true); //del

	unLockCards();
}

void CCardDrawPanel::openOtherCostCards(){
	CCardCell* pCell;
	unsigned int idx = 0;
	vector<CardElement> otherCardsInfo = getOtherCardsInfo(false);
	for(unsigned int i = 0; i < pCards->count(); i++){
		pCell = (CCardCell*)pCards->objectAtIndex(i);
		if(pCell->getCostType() == CCardCell::COST_CARD){
			if(pCell->getIsOpen() == false){
				pCell->openCard(false);

				if(idx < otherCardsInfo.size()){
					CardElement cardInfo = otherCardsInfo[idx];
					pCell->setItemInfo(cardInfo.elementid(), cardInfo.elementtype(), cardInfo.elementcnt());
					idx++;
				}
			}
		}
	}

	unLockCards();
}

vector<CardElement> CCardDrawPanel::getOtherCardsInfo(bool isFree){
	unsigned int cardID = WAR_MAP_CFG->GetWarMapCfgByID(P_FIGHT_OBJ->getFightMapID())->cardid();
	CardCfg* cfg = CARD_CFG_MASTER->GetCardCfgByID(cardID);
	vmsg::CSWarMapCardRsp openCardInfo = P_LEVEL_OBJ->getCardInfo();  //翻开卡中item的信息
	
	vector<CardElement> otherCardsInfo;

    bool bIsFind = false;
    bool bIsEqual = false;

	if(isFree){
		for(int i = 0; i < cfg->freeelementlst_size(); i++){
			CardElement element = cfg->freeelementlst(i);
/*
            bIsEqual = ( element.elementid() == openCardInfo.awardid() || element.elementtype() == openCardInfo.awardtype()
                || element.elementcnt() == openCardInfo.awardcnt() );
            if(!bIsEqual) {
                otherCardsInfo.push_back(element);
            } else {
                if(bIsFind) {
                    continue;
                }
                otherCardsInfo.push_back(element);
                bIsFind = true;
            }            
*/
            
			if(element.elementid() != openCardInfo.awardid() || element.elementtype() != openCardInfo.awardtype()
				|| element.elementcnt() != openCardInfo.awardcnt()){
				otherCardsInfo.push_back(element);
			}
		}
	}else{
		for(int i = 0; i < cfg->costelementlst_size(); i++){
			CardElement element = cfg->costelementlst(i);
            /*bIsEqual = ( element.elementid() == openCardInfo.awardid() || element.elementtype() == openCardInfo.awardtype()
                || element.elementcnt() == openCardInfo.awardcnt() );
            if(!bIsEqual) {
                otherCardsInfo.push_back(element);
            } else {
                if(bIsFind) {
                    continue;
                }
                otherCardsInfo.push_back(element);
                bIsFind = true;
            } 
            */
			if(element.elementid() != openCardInfo.awardid() || element.elementtype() != openCardInfo.awardtype()
				|| element.elementcnt() != openCardInfo.awardcnt()){
				otherCardsInfo.push_back(element);
			}
            
		}
	}

	int n = otherCardsInfo.size();
	int cnt = n;
	CardElement temp;
	int p1, p2;
	while(cnt--){   //随机交换两个位置的数，共交换cnt次
		p1 = rand()%n;
		p2 = rand()%n;
		temp = otherCardsInfo[p1];
		otherCardsInfo[p1] = otherCardsInfo[p2];
		otherCardsInfo[p2] = temp;
	}
    CCLOG("[trace]CCardDrawPanel::getOtherCardsInfo,other card num %d,card cfg id %d", otherCardsInfo.size(), cardID );
	return otherCardsInfo;
}

void CCardDrawPanel::updateCurCard(){

	CCPoint stPos;
	if(P_LEVEL_OBJ->getCardInfo().isfree() == 1){
		curCard->setItemInfo(P_LEVEL_OBJ->getCardInfo().awardid(), 
			P_LEVEL_OBJ->getCardInfo().awardtype(), P_LEVEL_OBJ->getCardInfo().awardcnt());
		stPos = curCard->getPosition();
		freeTimes--;
		if(totalTime > 0){
			unschedule(schedule_selector(CCardDrawPanel::downTime));
		}

		curCard->openCard(true);
	}else{
		costCurCard->setItemInfo(P_LEVEL_OBJ->getCardInfo().awardid(), 
			P_LEVEL_OBJ->getCardInfo().awardtype(), P_LEVEL_OBJ->getCardInfo().awardcnt());
		stPos = costCurCard->getPosition();
		costTimes--;

		costCurCard->openCard(true);
	}
}

void CCardDrawPanel::handleOpenOtherEvent(const string& stEvent, CCObject* pstEventObj){
    if(b_isOpenFreeCard){
        if(freeTimes == 0){  //翻开其他四张免费牌
            openOtherFreeCards();
        }
    }else{
        if(costTimes == 0){ //翻开其他四张牌
            openOtherCostCards();
        }
    }

	CCardCell* pCell = dynamic_cast<CCardCell*>(pstEventObj);
	if(!pCell->getIsGeted()){
		sureBtn->setVisible(true);
		sureBtn->setEnabled(true);
	}else{
		POP_TIP_LAYER_MASTER->ShowAwardTip(P_LEVEL_OBJ->getCardInfo().awardtype() ,
			P_LEVEL_OBJ->getCardInfo().awardid(),P_LEVEL_OBJ->getCardInfo().awardcnt());
	}
}

void CCardDrawPanel::open(){
	BasePanel::open();

	CToolUtils::loadArmatureRGBA4444("effect/ut00071/ut00071.ExportJson");

	updateUI();

	schedule(schedule_selector(CCardDrawPanel::downTime),1.0f);

    if(P_FIGHT_OBJ->getFightMapID() == WAR_MAP_CFG->GetCfg().warmapcfg(0).id()) {
        P_EVT_LOG->log(EVT_LOG_FIRST_WAR_CARD, 0);
    }
}

void CCardDrawPanel::downTime(float dt){
	totalTime--;
	timeValueTxt->setText(to_string(totalTime));

	if(totalTime == 0){
		unschedule(schedule_selector(CCardDrawPanel::downTime));
		srand((unsigned int)GlobalTime::getTime());
		unsigned int idx = 5*(rand()/(RAND_MAX + 1.0f));
		CCardCell* pCell = (CCardCell*)pCards->objectAtIndex(idx);
		openCard(pCell);
	}
}

void CCardDrawPanel::close(){
	CCardCell* pCell;
	for(unsigned int i = 0; i < pCards->count(); i++){
		pCell = (CCardCell*)pCards->objectAtIndex(i);
		pCell->removeFromParentAndCleanup(true);
	}
	pCards->removeAllObjects();
    costCurCard = NULL;

	BasePanel::close();

	CToolUtils::removeArmatureFile("effect/ut00071/ut00071.ExportJson");
}

void CCardDrawPanel::updateUI(){
	if(!pCards){
		return;
	}

	totalTime = DOWN_TIME;
	freeTimes = DEFAULT_TIMES;
	costTimes = DEFAULT_TIMES;

	sureBtn->setVisible(false);
    sureBtn->setEnabled(false);

	timeValueTxt->setText(intToString(totalTime));

	float cellX;
	float cellY;
	for(unsigned int i = 0; i < 10; i++){
		CCardCell* pCell = CCardCell::create();

		if(i/5 == 0){
			pCell->setCostType(CCardCell::FREE_CARD);
		}else{
			pCell->setCostType(CCardCell::COST_CARD);
		}
        
		string str = "card_cell_" + to_string(i + 1);
		pCell->setName(str.c_str());
		cellX = 160 + (i%5)*(pCell->getContentSize().width + 30);
		if(i/5 == 0){
			cellY = 513;
		}else{
			cellY = 213;
		}
		pCell->setPosition(ccp(cellX, cellY));
		m_pRootLayout->addChild(pCell, 7);
		pCell->addTouchEventListener(this, toucheventselector(CCardDrawPanel::touchEventHandler));
		pCell->AddEventListener(this, CCardCell::OPEN_OTHER_CARDS_EVENT, eventListner_selector(CCardDrawPanel::handleOpenOtherEvent));

		pCards->addObject(pCell);
	}
}

void CCardDrawPanel::lockCards(){
    //this->showLoading();
	CCardCell* pCell;
	for(unsigned int i = 0; i < pCards->count(); i++){
		pCell = (CCardCell*)pCards->objectAtIndex(i);
		pCell->setTouchEnabled(false);
	}
}

void CCardDrawPanel::unLockCards(){
    //this->closeLoading();
	CCardCell* pCell;
	for(unsigned int i = 0; i < pCards->count(); i++){
		pCell = (CCardCell*)pCards->objectAtIndex(i);
		pCell->setTouchEnabled(true);
	}
}

