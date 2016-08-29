#include "BossEmergePanel.h"
#include "Resources.h"
#include "ToolUtils.h"
#include "control/PanelSoundType.h"

#include "MonsterCfgMaster.h"

const CCPoint CBossEmergePanel::initRolePos = ccp(950, 200);

CBossEmergePanel::CBossEmergePanel()
:m_pBossDescLayer(NULL)
,m_pBossNameTxt(NULL)
,m_pBossDescTxt(NULL)
,m_uShowState(SHOW_STATE_INIT)
,m_pHurtImg(NULL)
,m_pLine0(NULL)
,m_pLine1(NULL)
,m_pLine2(NULL)
,m_pHurtLayer(NULL)
,m_pHurtDescImg(NULL)
,m_pArmature(NULL)
,m_pTouchLayer(NULL)
,m_uTime(0)
,m_pHurtArmature(NULL)
,m_pTarget(NULL)
,m_pEventSelector(NULL)
,_cfgID(1)
{
	
}

CBossEmergePanel::~CBossEmergePanel(){
	
}

bool CBossEmergePanel::init(){
	bool bRet = false;

	do{
		CC_BREAK_IF(!BasePanel::init());

		m_pArmature = NULL;

		initLayoutFromJsonFile("ui/boss_emerge_ui.json");
		addChild(m_pRootLayout);

		m_uSoundType = BOSS_EMERGENCY_PANEL_TYPE;

		initUI();

		bRet = true;
	}while(0);

	return bRet;
}

void CBossEmergePanel::initUI(){
	m_pBossDescLayer = dynamic_cast<UIPanel*>(m_pRootLayout->getChildByName("boss_desc_layer"));
	m_pBossNameTxt = dynamic_cast<UILabel*>(m_pBossDescLayer->getChildByName("boss_name_txt"));
	m_pBossDescTxt = dynamic_cast<UILabel*>(m_pBossDescLayer->getChildByName("boss_desc_txt"));

	m_pHurtImg = dynamic_cast<ImageView*>(m_pRootLayout->getChildByName("hurt_img"));
	m_pLine0 = dynamic_cast<ImageView*>(m_pRootLayout->getChildByName("line_0"));
	m_pLine1 = dynamic_cast<ImageView*>(m_pRootLayout->getChildByName("line_1"));
	m_pLine2 = dynamic_cast<ImageView*>(m_pRootLayout->getChildByName("line_2"));
	m_pHurtLayer = dynamic_cast<UIPanel*>(m_pRootLayout->getChildByName("hurt_desc_layer"));
	m_pHurtDescImg = dynamic_cast<ImageView*>(m_pHurtLayer->getChildByName("hurt_desc_img"));
	m_pHurtImg->setVisible(false);

	m_pTouchLayer = dynamic_cast<UIPanel*>(m_pRootLayout->getChildByName("touch_layer"));

	m_stDescPos = m_pBossDescLayer->getPosition();
	m_stHurtPos = m_pHurtLayer->getPosition();

	m_pTouchLayer->addTouchEventListener(this, toucheventselector(CBossEmergePanel::touchEventHandler));
}

void CBossEmergePanel::setCfgID(unsigned int var){
	_cfgID = var;
	m_uShowState = SHOW_STATE_INIT;

	MonsterCfg* monsterCfg = MONSTER_CFG_MASTER->GetMonsterCfgByID(_cfgID);
	m_stResPath = monsterCfg->avatar();
	string str = MONSTER_RES_PATH + m_stResPath + "/" + m_stResPath + JSON_TYPE_NAME;	
	CCArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo(str.c_str());

	m_pBossNameTxt->setText(monsterCfg->monstername());
	m_stDesc = monsterCfg->desc();

	loadArmature();
}

void CBossEmergePanel::loadArmature(){
	if(m_pArmature == NULL || m_pArmature->getParent() == NULL){
		m_pArmature = CCArmature::create(m_stResPath.c_str());
	}
	m_pArmature->setPosition(initRolePos);
	m_pRootLayout->addNode(m_pArmature);
	m_pArmature->getAnimation()->stop();

	//确定受击区域
	CCArray* coliArray = m_pArmature->getBone("body")->getColliderBodyList();
	if(coliArray->count() <= 0){
		setHurtPos(ccp(986, 277));   //设个默认位置
		return;
	}

	ColliderBody* body = (ColliderBody*)coliArray->objectAtIndex(0);
	CCContourVertex2* LeftUp = (CCContourVertex2*)body->getContourData()->vertexList.objectAtIndex(0);
	CCContourVertex2* LeftDown = (CCContourVertex2*)body->getContourData()->vertexList.objectAtIndex(1);
	CCContourVertex2* RightDown = (CCContourVertex2*)body->getContourData()->vertexList.objectAtIndex(2);
	CCContourVertex2* RightUp = (CCContourVertex2*)body->getContourData()->vertexList.objectAtIndex(3);

	CCPoint point = ccp((RightUp->x + LeftUp->x)/2, (LeftUp->y + LeftDown->y)/2);

	setHurtPos(point + m_pArmature->getPosition() + ccp(0, m_pArmature->getContentSize().height/2));
}

unsigned int CBossEmergePanel::getCfgID(){
	return _cfgID;
}

void CBossEmergePanel::setHurtPos(const CCPoint& point){
	m_pHurtImg->setPosition(point);
	updateHurtArmaturePos();

	CCSize size = m_pHurtImg->getSize();

	m_pLine2->setPosition(ccp(point.x - size.width/2, point.y + 5));

	if(point.y >= m_pBossDescLayer->getPositionY() - 100){
		m_pHurtLayer->setPosition(ccp(m_pHurtLayer->getPositionX(), m_pBossDescLayer->getPositionY() - 100 - m_pHurtLayer->getSize().height));
		m_pLine0->setPosition(ccp(m_pLine0->getPositionX(), m_pHurtLayer->getPositionY() + m_pHurtLayer->getSize().height/2));

	}else{
		m_pLine0->setPosition(ccp(m_pLine0->getPositionX(), point.y - 75));
		m_pHurtLayer->setPosition(ccp(m_pHurtLayer->getPositionX(), m_pLine0->getPositionY() - m_pHurtLayer->getSize().height/2));
	}

	CCPoint pointA = ccp(m_pLine2->getPositionX() - m_pLine2->getSize().width + 12, m_pLine2->getPositionY() + 7);
	CCPoint pointB = ccp(m_pLine0->getPositionX() + m_pLine0->getSize().width - 12, m_pLine0->getPositionY() - 7);

	m_pLine1->setSize(CCSize(ccpDistance(pointA, pointB), 19));
	m_pLine1->setPosition(ccpMidpoint(pointA, pointB));
	m_pLine1->setRotation(-CC_RADIANS_TO_DEGREES(ccpToAngle(pointA - pointB)));

	m_stHurtPos = m_pHurtLayer->getPosition();
}

void CBossEmergePanel::startAction(){
	m_uShowState = SHOW_STATE_ACTING;
	float duration = 0.25;

	//boss介绍的动作
	m_pBossDescLayer->setPosition(ccp(-450, m_stDescPos.y));		//设置起始位置
	CCActionInterval* move = CCMoveTo::create(duration*2, m_stDescPos);
	CCFiniteTimeAction* stepAction = CCCallFunc::create(this, callfunc_selector(CBossEmergePanel::showDescByStep));
	CCSequence* stAction = CCSequence::create(CCDelayTime::create(duration*0.25),  
		CCShow::create(),  
		move,
		stepAction,
		NULL);

	//m_pBossDescLayer->setActionManager(createNewCScheduler());
	m_pBossDescLayer->runAction(stAction);
}

void CBossEmergePanel::showDescByStep(){
	m_uIndex = 1;
	this->schedule(schedule_selector(CBossEmergePanel::stepUpdate), 0.1f);
}

void CBossEmergePanel::stepUpdate(float dt){
	string stTemp = m_stDesc.substr(0, m_uIndex*3);
	m_pBossDescTxt->setText(stTemp.c_str());
	m_uIndex++;

	if(m_uIndex*3 > m_stDesc.size()){
		this->unschedule(schedule_selector(CBossEmergePanel::stepUpdate));

		showHurtLocation();
	}
}

void CBossEmergePanel::showHurtLocation(){
	/*float duration = 0.5;
	CCActionInterval* scaleto = CCScaleBy::create(duration, 1.5f);
	CCActionInterval* rotateTo = CCRotateBy::create(duration, 360);
	CCFiniteTimeAction* arrowCallback = CCCallFunc::create(this, callfunc_selector(CBossEmergePanel::showHurtArrow));
	CCFiniteTimeAction* spawn = CCSpawn::create(scaleto, rotateTo, NULL);
	CCFiniteTimeAction* spawnRev = CCSpawn::create(scaleto->reverse(), rotateTo->reverse(), NULL);
	CCSequence* stSequence = CCSequence::create(CCShow::create(), spawn, spawnRev, arrowCallback, NULL);

	m_pHurtImg->setActionManager(createNewCScheduler());
	m_pHurtImg->runAction(stSequence);*/

	m_pHurtArmature->setVisible(true);
	showHurtArrow();
}

void CBossEmergePanel::showHurtArrow(){
	m_pLine0->setVisible(true);
	m_pLine1->setVisible(true);
	m_pLine2->setVisible(true);

	showHurtDesc();
}

void CBossEmergePanel::showHurtDesc(){
	m_pHurtLayer->setPosition(ccp(-510, m_stHurtPos.y));		//设置起始位置

	CCActionInterval* move = CCMoveTo::create(0.5f, m_stHurtPos);
	CCFiniteTimeAction* hurtCallback = CCCallFunc::create(this, callfunc_selector(CBossEmergePanel::showEnd));
	CCSequence* stAction = CCSequence::create(CCShow::create(), move, hurtCallback, NULL);

	//m_pHurtLayer->setActionManager(createNewCScheduler());
	m_pHurtLayer->runAction(stAction);
}

CCActionManager* CBossEmergePanel::createNewCScheduler(){
	CCScheduler* sched = new CCScheduler();  
	CCDirector::sharedDirector()->getScheduler()->scheduleUpdateForTarget(sched, 0, false); 
	CCActionManager* actionManager = new CCActionManager();  
	sched->scheduleUpdateForTarget(actionManager, 0, false); 

	return actionManager;
}

void CBossEmergePanel::showEnd(){
	m_uTime = 3;

	this->schedule(schedule_selector(CBossEmergePanel::closeCountDown), 1.0f);
}

void CBossEmergePanel::closeCountDown(float dt){
	m_uTime--;

	if(m_uTime == 2){
		m_uShowState = SHOW_STATE_END;
	}

	if(m_uTime <= 0){
		this->unschedule(schedule_selector(CBossEmergePanel::closeCountDown));
		this->close();
	}
}

void CBossEmergePanel::showAllImmediately(){
	if(m_pBossDescLayer->numberOfRunningActions() > 0){
		m_pBossDescLayer->stopAllActions();
	}

	/*if(m_pHurtImg->numberOfRunningActions() > 0){
	m_pHurtImg->stopAllActions();
	}*/

	if(m_pHurtLayer->numberOfRunningActions() > 0){
		m_pHurtLayer->stopAllActions();
	}

	this->unschedule(schedule_selector(CBossEmergePanel::stepUpdate));

	m_pHurtArmature->setVisible(true);
	m_pBossDescLayer->setVisible(true);
	//m_pHurtImg->setVisible(true);
	m_pLine0->setVisible(true);
	m_pLine1->setVisible(true);
	m_pLine2->setVisible(true);
	m_pHurtLayer->setVisible(true);

	m_pBossDescTxt->setText(m_stDesc);

	m_pBossDescLayer->setPosition(m_stDescPos);
	m_pHurtLayer->setPosition(m_stHurtPos);

	showEnd();
}

void CBossEmergePanel::updateHurtArmaturePos(){
	if(m_pHurtArmature == NULL){
		return;
	}

	m_pHurtArmature->setPosition(m_pHurtImg->getPosition());
}

void CBossEmergePanel::openWithCallFun(CCObject* target, SEL_CallFunc callbackFun){
	m_pTarget = target;
	m_pEventSelector = callbackFun;

	open();
}

void CBossEmergePanel::open(){
	CCArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo("effect/ut00016/ut00016.ExportJson");

	if(m_pHurtArmature != NULL && m_pHurtArmature->getParent() != NULL){
		m_pHurtArmature->removeFromParentAndCleanup(true);
		m_pHurtArmature = NULL;
	}

	m_pHurtArmature = CCArmature::create("ut00016");
	m_pHurtArmature->setVisible(false);
	m_pRootLayout->addNode(m_pHurtArmature, 0);
	updateHurtArmaturePos();
	m_pHurtArmature->getAnimation()->play("fly", -1, -1, 1);

	reset();

	BasePanel::open();

	startAction();
}

void CBossEmergePanel::close(){
	if(m_pArmature != NULL){
		m_pArmature->removeFromParentAndCleanup(true);
	}

	if(m_pBossDescLayer->numberOfRunningActions() > 0){
		m_pBossDescLayer->stopAllActions();
	}

	/*if(m_pHurtImg->numberOfRunningActions() > 0){
	m_pHurtImg->stopAllActions();
	}*/

	if(m_pHurtLayer->numberOfRunningActions() > 0){
		m_pHurtLayer->stopAllActions();
	}

	if(m_pHurtArmature != NULL && m_pHurtArmature->getParent() != NULL){
		m_pHurtArmature->removeFromParentAndCleanup(true);
		m_pHurtArmature = NULL;
	}

	this->unschedule(schedule_selector(CBossEmergePanel::stepUpdate));
	this->unschedule(schedule_selector(CBossEmergePanel::closeCountDown));

	BasePanel::close();

	CToolUtils::removeArmatureFile("effect/ut00016/ut00016.ExportJson");

	if(m_pTarget && m_pEventSelector){
		(m_pTarget->*m_pEventSelector)();
	} 

	m_pTarget = NULL;
	m_pEventSelector = NULL;
}

void CBossEmergePanel::reset(){
	m_pBossDescLayer->setVisible(false);
	m_pBossDescTxt->setText("");

	m_pHurtArmature->setVisible(false);
	m_pLine0->setVisible(false);
	m_pLine1->setVisible(false);
	m_pLine2->setVisible(false);
	m_pHurtLayer->setVisible(false);
}

void CBossEmergePanel::touchEventHandler(CCObject *pSender, TouchEventType type) {
	UIWidget *pWidget = dynamic_cast<UIWidget*>(pSender);
	const char *pszWidgetName = pWidget->getName();

	switch(type) {
	case TOUCH_EVENT_ENDED:
		if(strcmp(pszWidgetName, "touch_layer") == 0) {
			if(m_uShowState == SHOW_STATE_END){
				close();
			}else{
				//showAllImmediately(); 
			}
		}
		break;
	default:
		break;
	}
}