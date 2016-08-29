#include "FightControl.h"
#include "cocos2d.h"
#include "LevelControl.h"
#include "LevelObj.h"
#include "FightObj.h"
#include "UserObj.h"
#include "SceneManager.h"
#include "evtlog/EvtLog.h"
#include "WarMapCfgMaster.h"
#include "bag/BagControl.h"
#include "PopTipLayer.h"
#include "TextHelper.h"
#include "HideAwardLayout.h"
#include "BaseScene.h"

USING_NS_CC;

CFightControl::CFightControl()
:m_pFightProxy(NULL)
,m_pReliveLayout(NULL)
,m_pMedicalLayout(NULL)
,m_bLastFightFailed(false)
,m_bWarMapFight(false)
{

}

CFightControl::~CFightControl(){
	CC_SAFE_DELETE(m_pFightProxy);
	CC_SAFE_RELEASE_NULL(m_pReliveLayout);
	CC_SAFE_RELEASE_NULL(m_pMedicalLayout);
}

CFightProxy* CFightControl::getFightProxy(){
	if(m_pFightProxy == NULL) {
		m_pFightProxy = new CFightProxy();
	}

	return m_pFightProxy;
}

void CFightControl::doNormalFightEnd(){
	P_LEVEL_CTL->getResultPanel()->closeLoading();
	if(P_LEVEL_CTL->getPausePanel()->isOpen()){
		P_LEVEL_CTL->getPausePanel()->close();
	}

	//还原为临时枪械前的信息
	P_FIGHT_OBJ->restoreRealWeapon();
	setWarMapFight(true);

	if(P_FIGHT_OBJ->getScore() == 0){ //失败
		setLastFightFailed(true);

		if(P_FIGHT_OBJ->getIsForceQuit()){  //强制退出
			SceneManager::sharedSceneManager()->enterScene(P_USER_OBJ->getUserInfo()->sceneID);
		}else{
			P_LEVEL_CTL->getFailPanel()->open();
		}

        if(P_FIGHT_OBJ->getFightMapID() == WAR_MAP_CFG->GetCfg().warmapcfg(0).id()) {
            P_EVT_LOG->log(EVT_LOG_FIRST_WAR_RESULT, "lose");
        }
        
	}else{
		//胜利后，修改最新关卡
		P_LEVEL_OBJ->updateNewWarMapId();

		if(P_FIGHT_OBJ->getFightMapID() == WAR_MAP_CFG->GetCfg().warmapcfg(0).id()) {
			P_EVT_LOG->log(EVT_LOG_FIRST_WAR_RESULT, "win");
		}

		bool bIsShowPanel = false;

		if(P_FIGHT_OBJ->getAwardInfo().hidenawards_size() <= 0){
			bIsShowPanel = true;
		}else{
			bIsShowPanel = false;

			BaseScene *pScene = dynamic_cast<BaseScene*>(CCDirector::sharedDirector()->getRunningScene());
			if(pScene != NULL){
				UILayer *pPopLayer = dynamic_cast<UILayer*>(pScene->getLayer(LAYER_ID_POP));

				if(pPopLayer != NULL){
					if(pPopLayer->getChildByTag(12) != NULL){
						pPopLayer->removeChildByTag(12);
					}

					CHideAwardLayout* pHideLayout = CHideAwardLayout::create();
					pPopLayer->addChild(pHideLayout, 0, 12);
					pHideLayout->openAwardBox();
				}else{
					bIsShowPanel = true;
				}
			}else{
				bIsShowPanel = true;
			}
		}

		if(bIsShowPanel){
			//先结算,后抽卡，坑爹策划要求，立此为证
			P_LEVEL_CTL->getResultPanel()->open();
			P_LEVEL_CTL->getResultPanel()->updateUI();
		}
	}
}

void CFightControl::doEliteFightEnd(){
	P_LEVEL_CTL->getResultPanel()->closeLoading();
	if(P_LEVEL_CTL->getPausePanel()->isOpen()){
		P_LEVEL_CTL->getPausePanel()->close();
	}

	//还原为临时枪械前的信息
	P_FIGHT_OBJ->restoreRealWeapon();
	setWarMapFight(true);

	if(P_FIGHT_OBJ->getScore() == 0){ //失败
		setLastFightFailed(true);

		if(P_FIGHT_OBJ->getIsForceQuit()){  //强制退出
			SceneManager::sharedSceneManager()->enterScene(P_USER_OBJ->getUserInfo()->sceneID);
		}else{
			P_LEVEL_CTL->getFailPanel()->open();
		}
	}else{
		//先结算,后抽卡，坑爹策划要求，立此为证
		P_LEVEL_CTL->getResultPanel()->open();
		P_LEVEL_CTL->getResultPanel()->updateUI();
	}
}

void CFightControl::doDailyFightEnd(){
	P_LEVEL_CTL->getResultPanel()->closeLoading();
	if(P_LEVEL_CTL->getPausePanel()->isOpen()){
		P_LEVEL_CTL->getPausePanel()->close();
	}

	//还原为临时枪械前的信息
	P_FIGHT_OBJ->restoreRealWeapon();

	if(P_FIGHT_OBJ->getScore() == 0){ //失败
		setLastFightFailed(true);

		if(P_FIGHT_OBJ->getIsForceQuit()){  //强制退出
			SceneManager::sharedSceneManager()->enterScene(P_USER_OBJ->getUserInfo()->sceneID);
		}else{
			P_LEVEL_CTL->getFailPanel()->open();
		}
	}else{
		//先结算,后抽卡，坑爹策划要求，立此为证
		P_LEVEL_CTL->getResultPanel()->open();
		P_LEVEL_CTL->getResultPanel()->updateUI();
	}
}

void CFightControl::doBuyBulletRsp(const vmsg::CSBuyBulletRsp& stRsp){
	P_FIGHT_OBJ->setFillBulletCost(stRsp.nextbulletcost());
	if(stRsp.has_baginfo()){  //使用道具才返回背包信息
		P_BAG_CTRL->setBagInfo(stRsp.baginfo());

		POP_TIP_LAYER_MASTER->ShowTipStr(TextStr(TEXT_AUTO_FILL_BULLET_TIP));
	}

	if(P_LEVEL_CTL->getFillBulletPanel()->isOpen()){
		P_LEVEL_CTL->getFillBulletPanel()->doFillBulletDone();
	}
}

void CFightControl::doReliveRsp(const vmsg::CSReliveRsp& stRsp){
	P_FIGHT_OBJ->setReliveCost(stRsp.nextrelivecost());
	if(stRsp.has_baginfo()){  //使用道具才返回背包信息
		P_BAG_CTRL->setBagInfo(stRsp.baginfo());
	}

	getReliveLayout()->doReliveDone();
}

CFightHeroReliveLayer* CFightControl::getReliveLayout(){
	if(m_pReliveLayout == NULL) {
		m_pReliveLayout = CFightHeroReliveLayer::create();
		CC_SAFE_RETAIN(m_pReliveLayout);
	}

	return m_pReliveLayout;
}

CFightMedicalLayout* CFightControl::getMedicalLayout(){
	if(m_pMedicalLayout == NULL) {
		m_pMedicalLayout = CFightMedicalLayout::create();
		CC_SAFE_RETAIN(m_pMedicalLayout);
	}

	return m_pMedicalLayout;
}

bool CFightControl::isWarMapFight() {
	return m_bWarMapFight;
}

void CFightControl::setWarMapFight(bool isWarMapFight) {
	m_bWarMapFight = isWarMapFight;
}

bool CFightControl::isLastFightFailed() {
	return m_bLastFightFailed;
}

void CFightControl::setLastFightFailed(bool isFailed) {
	m_bLastFightFailed = isFailed;
}
