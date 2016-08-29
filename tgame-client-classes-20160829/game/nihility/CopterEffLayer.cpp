#include "CopterEffLayer.h"
#include "defines.h"
#include "Resources.h"
#include "MapBinReader.h"
#include "Multiresolution.h"
#include "role/Enemy.h"
#include "NihilityUtils.h"
#include "instance/InstanceUtils.h"

#include "MonsterCfgMaster.h"

const string CopterEffLayer::COPTER_FLY_OUT_EVENT = "copter_fly_out_event";

CopterEffLayer::CopterEffLayer()
:m_pMapBg(NULL)
,m_pCopterArm(NULL)
,m_stMonsterData(NULL)
,m_stStageMonsters(NULL)
,m_stBgSize(CCSizeZero)
,m_fSpeed(0)
,m_bIsCopterOut(false)
{

}

CopterEffLayer::~CopterEffLayer(){
	if (m_stStageMonsters){
		m_stStageMonsters->removeAllObjects();
		m_stStageMonsters->release();
		m_stStageMonsters = NULL;
	}
}

bool CopterEffLayer::init(){
	bool bRet = false;

	do {
		CC_BREAK_IF(!CCLayer::init());

		m_fSpeed = 600;

		//解析数据
		analyseInfo();

		bRet = true;
	} while(0);

	return bRet;
}

void CopterEffLayer::onEnter(){
	CCLayer::onEnter();
}

void CopterEffLayer::onExit(){
	CCLayer::onExit();

	this->unscheduleUpdate();
}

void CopterEffLayer::initUI(){
	CCSize size = CCDirector::sharedDirector()->getVisibleSize(); 
	CCPoint point = CCDirector::sharedDirector()->getVisibleOrigin();

	//背景
	m_pMapBg = CCSprite::create();
	m_pMapBg->setAnchorPoint(ccp(0, 0));
	float disX = 0;
	for(int idx = 0; idx < 2; idx++){
		CCSprite* pSprite = CCSprite::create("ui_bg/sky.png");
		pSprite->setAnchorPoint(ccp(0, 0));
		m_pMapBg->addChild(pSprite);
		disX = idx*pSprite->getContentSize().width;
		pSprite->setPosition(ccp(disX, 0));
		m_stBgSize.width += pSprite->getContentSize().width;
		m_stBgSize.height = pSprite->getContentSize().height;
	}

	addChild(m_pMapBg, -2);

	m_pMapBg->setContentSize(m_stBgSize);
	m_pMapBg->setScale(size.height/OPTIMAL_HEIGHT);

	//直升机特效
	m_pCopterArm = CCArmature::create("ma00053");
	m_pCopterArm->setPosition(ccp(0, m_stBgSize.height/2));
	int iNewZorder = CInstanceUtils::GetZOrderByType(FLOOR_BUFF_Z_ORDER, m_pCopterArm->getPositionY());
	m_pMapBg->addChild(m_pCopterArm, iNewZorder);
	m_pCopterArm->getAnimation()->play("run", -1, -1, -1);

	initMonsters();
}

void CopterEffLayer::start(){
	this->scheduleUpdate();
}

void CopterEffLayer::analyseInfo(){
	//第一先获取文件的路径  
	string realPath = MAP_CFG_RES + "coptermap" + MAP_RES_TYPE;

	CMapBin stMapBin;
	int iRet = MapBinReader::Init(stMapBin, realPath.c_str());

	if(iRet != 0){
		CCLOG("CopterEffLayer::analyseInfo failed");
		return;
	}

	CC_SAFE_RELEASE_NULL(m_stMonsterData);
	m_stMonsterData = CCArray::create();
	m_stMonsterData->retain();
	CEnemyData* monsterObj;
	for(int i = 0; i < stMapBin.monsterlist_size(); ++i){   //如果不是数组，这一行会报错  
		monsterObj = CEnemyData::create();
		monsterObj->id = stMapBin.monsterlist(i).id();
		monsterObj->x = stMapBin.monsterlist(i).x();
		monsterObj->y = stMapBin.monsterlist(i).y();
		m_stMonsterData->addObject(monsterObj);
	}  
}

void CopterEffLayer::initMonsters(){
	m_stStageMonsters = CCArray::create();
	m_stStageMonsters->retain();

	int iNewZorder;
	CCObject* pstObj;
	CCARRAY_FOREACH(m_stMonsterData, pstObj){
		CEnemyData* monsterObj = (CEnemyData*)pstObj;
		CEnemy* pstMonster = CEnemy::create();
		MonsterCfg stCfg;
		CNihilityUtils::genMonsterCfg(monsterObj->id, stCfg);
		pstMonster->setResPath(stCfg.avatar());
		pstMonster->SetEnityPosition(ccp(monsterObj->x, m_stBgSize.height - monsterObj->y));
		pstMonster->setDesiredPosition(ccp(monsterObj->x, m_stBgSize.height - monsterObj->y ));
		pstMonster->setPosition(ccp(monsterObj->x, m_stBgSize.height - monsterObj->y));
		pstMonster->LoadActionRes(false);
		pstMonster->setMyState(kActionStateIdle);
		pstMonster->getBlood()->setVisible(false);
		pstMonster->setShadowVisiable(false);
		pstMonster->getArmature()->getAnimation()->play("run", -1, -1, 1);
		pstMonster->AddEventListener(this, CQStageEntity::DIE_COMPLETE_EVENT, eventListner_selector(CopterEffLayer::handleMonsterDieCompleteEvent));
		iNewZorder = CInstanceUtils::GetZOrderByType(FLOOR_BUFF_Z_ORDER, pstMonster->getPositionY());
		m_pMapBg->addChild(pstMonster, iNewZorder - 50);
		m_stStageMonsters->addObject((CCObject*)pstMonster);
	}
}

void CopterEffLayer::update(float delta){
	setViewpointCenter();

	fly(delta);

	checkMonsterDie();
	checkCopterOut();
}

void CopterEffLayer::setViewpointCenter(){
	if(!m_pCopterArm){
		return;
	}

	float x;
	float posX = m_pCopterArm->getPositionX();

	CCDirector *pDirector = CCDirector::sharedDirector();
	CCPoint screenOrigin = pDirector->getVisibleOrigin(); 
	CCSize screenSize = pDirector->getVisibleSize();
	CCSize innerSize = Multiresolution::getInnerSize();
	float scale = screenSize.height / innerSize.height;
	float visibleW = screenSize.width/scale;

	if(posX <= visibleW/2){
		x = 0;
	}else if(posX >= m_stBgSize.width - visibleW/2){
		x = m_stBgSize.width - visibleW;
	}else{
		x = posX - visibleW/2;
	}

	CCPoint fgViewPoint = ccp(-x, 0);
	m_pMapBg->setPosition(fgViewPoint);
}

void CopterEffLayer::fly(float dt){
	m_pCopterArm->setPosition(ccpAdd(m_pCopterArm->getPosition(), ccpMult(ccp(m_fSpeed, 0), dt)));
}

void CopterEffLayer::checkMonsterDie(){
	CCObject* pstObj;
	CCARRAY_FOREACH(m_stStageMonsters, pstObj){
		CEnemy* pstMonster = dynamic_cast<CEnemy*>(pstObj);
		if(pstMonster->getPositionX() - 50 <= m_pCopterArm->getPositionX() + pstMonster->getCenterToSides()
			&& pstMonster->getMyState() != kActionStateDie){
			pstMonster->setMyState(kActionStateDie);
			pstMonster->getArmature()->getAnimation()->play("die", -1, -1, 0);
		}
	}
}

void CopterEffLayer::handleMonsterDieCompleteEvent(const string& stEvent, CCObject* pstEventObj){
	CEnemy* pstEntity = dynamic_cast<CEnemy*>(pstEventObj);

	m_pMapBg->removeChild(pstEntity, true);
	if(m_stStageMonsters->indexOfObject(pstEntity) != UINT_MAX){
		m_stStageMonsters->removeObject(pstEntity);
	}
}

void CopterEffLayer::checkCopterOut(){
	if(!m_bIsCopterOut){
		if(m_pCopterArm->getPositionX() - m_pCopterArm->getContentSize().width/2 >= m_stBgSize.width){
			m_bIsCopterOut = true;
			DispatchEvent(COPTER_FLY_OUT_EVENT, this);	
			this->unscheduleUpdate();
		}
	}
}

