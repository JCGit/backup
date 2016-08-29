#include "ResultPanel.h"
#include "FightObj.h"
#include "WarMapCfgMaster.h"
#include "TimeEx.h"
#include "defines.h"
#include "SceneManager.h"
#include "LevelControl.h"
#include "FightControl.h"
#include "UserObj.h"
#include "HeroCfgMaster.h"
#include "IconCell.h"
#include "CSEquipItemMsg.pb.h"
#include "PopTipLayer.h"
#include "control/AudioManager.h"
#include "instance/InstanceUtils.h"
#include "LabelChange.h"
#include "bag/BagControl.h"
#include "Global.h"
#include "ToolUtils.h"
#include "NounHelper.h"

using namespace std;

const static int AWARD_ITEM_MAX = 5;

CResultPanel::CResultPanel(){
	m_uScore = 0;
	m_uMaxKill = 0;
	m_fInitX = 0;
    m_iShowTotalStarNum = 0;
    m_iNowShowStarIdx = 0;
	m_uCellTotalW = 0;
}

CResultPanel::~CResultPanel(){
	if(m_pCellArray) {
		m_pCellArray->removeAllObjects();
		m_pCellArray->release();
	}
	
	if(m_pUICellArray) {
		m_pUICellArray->removeAllObjects();
		m_pUICellArray->release();
	}
}

bool CResultPanel::init(){
	bool bRet = false;

	do 
	{
		CC_BREAK_IF(!BasePanel::init());
		
		initLayoutFromJsonFile("ui_v2/result_panel_ui.ExportJson");
		addChild(m_pRootLayout);

		setClickOutToClose(false);

		initUI();

		bRet = true;
	} while (0);

	return bRet;
}

void CResultPanel::initUI(){

    m_pInfoLayout = dynamic_cast<UILayout*>(m_pRootLayout->getChildByName("info_layout"));
    m_pSureBtn = dynamic_cast<UIButton*>(m_pInfoLayout->getChildByName("sure_btn"));
    m_pScoreTxt = dynamic_cast<UILabel*>(m_pInfoLayout->getChildByName("score_txt"));
    m_pExpTxt = dynamic_cast<UILabel*>(m_pInfoLayout->getChildByName("exp_txt"));
    m_pTimeTxt = dynamic_cast<UILabel*>(m_pInfoLayout->getChildByName("time_txt"));
    m_pCoinTxt = dynamic_cast<UILabel*>(m_pInfoLayout->getChildByName("coin_txt"));
    m_pDropLabel = dynamic_cast<ImageView*>(m_pInfoLayout->getChildByName("drop_label"));

	m_pWinBg = dynamic_cast<ImageView*>(m_pRootLayout->getChildByName("win_icon"));
	m_pStar0 = dynamic_cast<ImageView*>(m_pRootLayout->getChildByName("star_0"));
	m_pStar1 = dynamic_cast<ImageView*>(m_pRootLayout->getChildByName("star_1"));
	m_pStar2 = dynamic_cast<ImageView*>(m_pRootLayout->getChildByName("star_2"));
    m_pWinBg->setVisible(false); //只是用来获取坐标的


	m_fInitX = m_pExpTxt->getPositionX();

	m_pCellArray = CCArray::create();
	m_pCellArray->retain();
	m_pUICellArray = CCArray::create();
	m_pUICellArray->retain();

	UIImageView *pImg;
	IconCell *stCell;
	for(int i = 0; i < AWARD_ITEM_MAX; ++i) {
		string strUIName = "cell_" + intToString(i);
		pImg = dynamic_cast<UIImageView*>(m_pInfoLayout->getChildByName(strUIName.c_str()));
		stCell = IconCell::create();
		stCell->showNumTxt();
		pImg->addChild(stCell);
		pImg->setVisible(false);
		m_pCellArray->addObject(stCell);
		m_pUICellArray->addObject(pImg);
		pImg->addTouchEventListener(this, toucheventselector(CResultPanel::touchEventHandler));
	}
    m_pSureBtn->setTitleText(NounStr(OK));
	m_pSureBtn->addTouchEventListener(this, toucheventselector(CResultPanel::touchEventHandler));
}

void CResultPanel::updateUI(){
	m_pTimeTxt->setText(TimeEx::getStringTimeForInt(P_FIGHT_OBJ->getTotalTime()).c_str());
	m_pExpTxt->setText(intToString(P_FIGHT_OBJ->getAwardInfo().awardexp()).c_str());
	m_pCoinTxt->setText(intToString(P_FIGHT_OBJ->getAwardInfo().awardcoin()).c_str());

	m_uScore = P_FIGHT_OBJ->getMark();
	m_uMaxKill = P_FIGHT_OBJ->getMaxKill();

    m_pScoreTxt->setText(intToString(m_uScore));

    m_iShowTotalStarNum = P_FIGHT_OBJ->getScore();
    m_iNowShowStarIdx = 0;
    CCLOG("[trace]CResultPanel::updateUI,score %d", m_iShowTotalStarNum);

	winAction();
}

void CResultPanel::winAction(){
    
    //先播放胜利特效,然后星星特效，然后 nextFun
    //1. ut00027
    CCArmature* pstArmature = dynamic_cast<CCArmature*>(m_pRootLayout->getChildByTag(TAG_WIN_EFF));
    if(pstArmature) {
        m_pRootLayout->removeNodeByTag(TAG_WIN_EFF);
    }
    CToolUtils::loadArmatureRGBA4444("effect/ut00072/ut00072.ExportJson");
    pstArmature = CCArmature::create("ut00072");

    //pstArmature->setAnchorPoint(ccp(0.5f, 0.5f));

    pstArmature->setPosition(m_pRootLayout->getSize().width/2, m_pRootLayout->getSize().height/2);
    pstArmature->getAnimation()->setMovementEventCallFunc(this, movementEvent_selector(CResultPanel::onWinEffDone));

    m_pRootLayout->addNode(pstArmature, 20, TAG_WIN_EFF);
    pstArmature->getAnimation()->play("ks", -1, -1, 0); //not loop
    
}


void CResultPanel::onWinEffDone(CCArmature * armature, MovementEventType type, const char * name) {
    if (COMPLETE == type){
        armature->getAnimation()->play("fly", -1, -1, 1); //loop
        //播放星星
        showNextStarEff();
    }     
}
void CResultPanel::showNextStarEff() {
    if(m_iNowShowStarIdx >= m_iShowTotalStarNum) {
        //星星播放完毕，播放数字
        rollNumber();
        return;
    }
    const int iNowStarFlag = TAG_STAR_EFF + m_iNowShowStarIdx;

    CCArmature* pstArmature = dynamic_cast<CCArmature*>(m_pRootLayout->getChildByTag(iNowStarFlag));
    if(pstArmature) {
        m_pRootLayout->removeNodeByTag(iNowStarFlag);
    }
    CCTexture2D::PVRImagesHavePremultipliedAlpha(true);
    CCArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo("effect/ut00034/ut00034.ExportJson");
    pstArmature = CCArmature::create("ut00034");

    pstArmature->setAnchorPoint(ccp(0.5f, 0.0f));
    CCPoint pt;
    if(0 == m_iNowShowStarIdx) {
        pt = m_pStar0->getPosition();
    } else if(1 == m_iNowShowStarIdx) {
        pt = m_pStar1->getPosition();
    } else if(2 == m_iNowShowStarIdx) {
        pt = m_pStar2->getPosition();
    }
    pt.y = pt.y - 150;

    pstArmature->setPosition(pt);
    pstArmature->getAnimation()->setMovementEventCallFunc(this, movementEvent_selector(CResultPanel::onStarEffDone));
    m_pRootLayout->addNode(pstArmature, 20, iNowStarFlag);
    pstArmature->getAnimation()->play("fly", -1, -1, 0); //not loop

    m_iNowShowStarIdx++;
}

void CResultPanel::onStarEffDone(CCArmature * armature, MovementEventType type, const char * name) {
    if (COMPLETE == type){
        //播放星星
        shake();
        showNextStarEff();
    }     
}

void CResultPanel::rollNumber(){

    m_pInfoLayout->setVisible(true);
    updateCells();

    /*
	float duration = 0.5f;
	CCFiniteTimeAction* nextFun = CCCallFunc::create(this, callfunc_selector(CResultPanel::txtAction));
	CLabelChange* pScoreChange  = CLabelChange::create(duration, 0, m_uScore);
	m_pScoreTxt->getVirtualRenderer()->runAction(CCSequence::create(pScoreChange, nextFun, NULL));

	CLabelChange* pKillChange  = CLabelChange::create(duration, 0, m_uMaxKill);
    */
}

void CResultPanel::txtAction(){
    /*
	float inter = 400;
	float duration = 0.3f;
	float delay = 0.1f;
	CCFiniteTimeAction* nextFun = CCCallFunc::create(this, callfunc_selector(CResultPanel::updateCells));
	CCFiniteTimeAction* spawn1 = CCSpawn::create(CCFadeIn::create(duration/2), CCMoveBy::create(duration, ccp(inter, 0)), NULL);
	CCFiniteTimeAction* spawn2 = CCSpawn::create(CCFadeIn::create(duration/2), CCMoveBy::create(duration, ccp(inter, 0)), NULL);
	CCFiniteTimeAction* spawn3 = CCSpawn::create(CCFadeIn::create(duration/2), CCMoveBy::create(duration, ccp(inter, 0)), NULL);

	m_pTimeTxt->runAction(CCSequence::create(spawn1, NULL));
	m_pExpTxt->runAction(CCSequence::create(CCDelayTime::create(delay), spawn2, NULL));
	m_pCoinTxt->runAction(CCSequence::create(CCDelayTime::create(delay*2), spawn3, nextFun, NULL));
    */
}

void CResultPanel::updateCells(){
	//物件奖励
	IconCell *stCell;
	UIImageView *pImg;
	bool bIsBigElt;
	vector<vmsg::CSAwardElement> stBigElts;
	vector<vmsg::CSAwardElement> stNormalElts;

	vmsg::CSWarMapEndRsp stAwardInfo = P_FIGHT_OBJ->getAwardInfo();

	//整理数据
	for(int i = 0; i < stAwardInfo.packawards_size(); i++){
		const vmsg::CSAwardElement& elmt = stAwardInfo.packawards(i);
		bIsBigElt = CToolUtils::adjustIconBgSize(elmt.elementtype(), elmt.elementid(), NULL);

		if(bIsBigElt){
			stBigElts.push_back(elmt);
		}else{
			stNormalElts.push_back(elmt);
		}
	}

	m_uCellTotalW = 0;

	for(int i = 0; i < AWARD_ITEM_MAX; i++){
		stCell = dynamic_cast<IconCell*>(m_pCellArray->objectAtIndex(i));
		pImg = dynamic_cast<UIImageView*>(m_pUICellArray->objectAtIndex(i));
		pImg->setVisible(true);
		pImg->setTouchEnabled(true);
		if(i >= stAwardInfo.packawards_size()) {
			stCell->clearCell();
			pImg->setVisible(false);
			pImg->setTouchEnabled(false);
            continue;
		}

		vmsg::CSAwardElement elmt;
		unsigned int uCellW = 100;
		if(i < (int)stNormalElts.size()){  //正常大小
			uCellW = 100;
			elmt = stNormalElts.at(i);
		}else{
			uCellW = 180;
			elmt = stBigElts.at(i - stNormalElts.size());
		}

        stCell->setInfo(elmt.elementtype(), elmt.elementcnt(), elmt.elementid());
		float disX = 165 + i*20 + m_uCellTotalW + uCellW/2;
		pImg->setPositionX(disX);

		m_uCellTotalW += uCellW;

        //碎片类型要显示图标
        CToolUtils::adjustIconBgSize(elmt.elementtype(), elmt.elementid(), pImg);
        CToolUtils::adjustFragIcon(elmt.elementtype(),  pImg);

        POP_TIP_LAYER_MASTER->ShowAwardTip(elmt.elementtype(), elmt.elementid(), elmt.elementcnt());
	}

	if(stAwardInfo.packawards_size() <= 0){
		m_pDropLabel->setVisible(false);
	}else{
		m_pDropLabel->setVisible(true);
	}

	//POP_TIP_LAYER_MASTER->ShowAwardTip(vmsg::THING_TYPE_COIN, 0, P_FIGHT_OBJ->getAwardInfo().awardcoin());

	m_pSureBtn->setVisible(true);
	m_pSureBtn->setTouchEnabled(true);
}

void CResultPanel::resetAction(){

	m_pStar0->setScale(0);
	m_pStar1->setScale(0);
	m_pStar2->setScale(0);

    m_pInfoLayout->setVisible(false);
    m_pScoreTxt->setText("");
    /*
	float inter = 400;
	m_pTimeTxt->setPositionX(m_fInitX - inter);
	m_pExpTxt->setPositionX(m_fInitX - inter);
	m_pCoinTxt->setPositionX(m_fInitX - inter);
	m_pTimeTxt->setOpacity(0x00);
	m_pExpTxt->setOpacity(0x00);
	m_pCoinTxt->setOpacity(0x00);
    */

	m_pDropLabel->setVisible(false);
	m_pSureBtn->setVisible(false);
	m_pSureBtn->setTouchEnabled(false);

	IconCell *stCell;
	UIImageView *pImg;
	for(int i = 0; i < AWARD_ITEM_MAX; i++){
		stCell = dynamic_cast<IconCell*>(m_pCellArray->objectAtIndex(i));
		pImg = dynamic_cast<UIImageView*>(m_pUICellArray->objectAtIndex(i));
		pImg->setVisible(false);
		stCell->clearCell();
	}
}

void CResultPanel::shake(){
	AudioManager::playEffect("perfect_result.ogg");		//盖章音效

	CCDirector *pDirector = CCDirector::sharedDirector();
	BaseScene *pRunningScene = dynamic_cast<BaseScene*>(pDirector->getRunningScene());
	CInstanceUtils::ShakeScene(pRunningScene, 0.3f, 5, LAYER_ID_UI);

}

void CResultPanel::touchEventHandler(CCObject *pSender, TouchEventType type){
	UIWidget* pWidget = dynamic_cast<UIWidget*>(pSender);
	const char* pName = pWidget->getName();
	string strName = pWidget->getName();
	switch (type)
	{
		case TOUCH_EVENT_ENDED:
			if(strcmp(pName, "sure_btn") == 0){
				End();
			}else if(strName.find("cell_") != string::npos){
				string sub = strName.substr(strName.find_first_not_of("cell_"));
				unsigned int iIdx = atoi(sub.c_str());
				if(iIdx >= 0 && iIdx < m_pCellArray->count()){
					IconCell* pCell = dynamic_cast<IconCell*>(m_pCellArray->objectAtIndex(iIdx));
					P_BAG_CTRL->getItemInfoPanel()->open();
					P_BAG_CTRL->getItemInfoPanel()->updateInfo(pCell->getItemCfgid(), pCell->getItemType(), false);
				}
			}
			break;
		default:
			break;
	}
}

void CResultPanel::End(){
	this->close();

	//if(P_FIGHT_OBJ->getFightType() == NORMAL_FIGHT_TYPE){
	//	 P_LEVEL_CTL->getCardPanel()->open();
	//}else{
	//	SceneManager::sharedSceneManager()->enterScene(P_USER_OBJ->getUserInfo()->sceneID);
	//}

	//策划要求干掉抽卡  2014-12-30
	SceneManager::sharedSceneManager()->enterScene(P_USER_OBJ->getUserInfo()->sceneID);
   

    //清理特效资源
    m_pRootLayout->removeNodeByTag(TAG_WIN_EFF);
    m_pRootLayout->removeNodeByTag(TAG_STAR_EFF);
    m_pRootLayout->removeNodeByTag(TAG_STAR_EFF + 1);
    m_pRootLayout->removeNodeByTag(TAG_STAR_EFF + 2);
    CToolUtils::removeArmatureFile("effect/ut00034/ut00034.ExportJson");
    //CToolUtils::removeArmatureFile("effect/ut00027/ut00027.ExportJson");
}

void CResultPanel::open(){
	BasePanel::open();

	resetAction();
}

void CResultPanel::close(){
    BasePanel::close();
    P_BAG_CTRL->sendQueryBagInfoRqst();

    CToolUtils::removeArmatureFile("effect/ut00072/ut00072.ExportJson");
}