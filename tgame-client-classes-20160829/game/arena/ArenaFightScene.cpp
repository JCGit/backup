#include "ArenaFightScene.h"
#include "ArenaControl.h"
#include "ConstHelper.h"
#include "ToolUtils.h"
#include "ArenaStartPanel.h"
#include "ArenaFightInfoLayout.h"
#include "Multiresolution.h"
#include "control/AudioManager.h"
#include "instance/AnimationUtils.h"
#include "nihility/NihilityUtils.h"
#include "instance/InstanceUtils.h"
#include "instance/InstanceFightUtils.h"
#include "fight/FightObj.h"
#include "set/SetSysControl.h"
#include "fight/FightWeaponArrLayout.h"
#include "game/fight/FightControl.h"

#include "ArenaLvCfgMaster.h"
#include "HeroCfgMaster.h"
#include "WeaponCfgMaster.h"
#include "ArenaBossLevCfgMaster.h"
#include "ArenaKillNumCfgMaster.h"
#include "UserObj.h"

CArenaFightScene::CArenaFightScene()
:m_uArenaLv(0)
,m_uMonsterLev(0)
,m_fLimitTime(0)
,m_uStageIdx(0)
,m_uLastKillID(0)
,m_uOppDieNum(0)
,m_fOppDieTime(0)
,m_uTotalStage(0)
,m_uKillBossNum(0)
,m_uBossTotalNum(0)
,m_uOpRandWeaponID(1)
,m_bIsWin(false)
,m_bIsFirstEnter(false)
,m_bIsOppDying(false)
,m_bIsBossHited(false)
,m_bIsWinComplete(false)
,m_pOpponentHero(NULL)
,m_stAllBossData(NULL)
,m_stPowerGrids(NULL)
,m_pInfoLayout(NULL)
{

}

CArenaFightScene::~CArenaFightScene(){
	CC_SAFE_RELEASE_NULL(m_pOpponentHero);

	if(m_stAllBossData){
		m_stAllBossData->removeAllObjects();
		CC_SAFE_RELEASE_NULL(m_stAllBossData);
	}

	if(m_stPowerGrids){
		m_stPowerGrids->removeAllObjects();
		CC_SAFE_RELEASE_NULL(m_stPowerGrids);
	}
}

void CArenaFightScene::onEnterTransitionDidFinish(){
	CBaseFightScene::onEnterTransitionDidFinish();

	if(!P_SET_CTRL->isSet(SetSysControl::ARENA_FIRST_TIME_TYPE)){  //第一次进入竞技场才打开面板
		m_bIsFirstEnter = true;
		P_ARENA_CTRL->getStartPanel()->open();
		P_ARENA_CTRL->getStartPanel()->AddEventListener(this, CArenaStartPanel::START_PVP, eventListner_selector(CArenaFightScene::startPvp));
		P_SET_CTRL->setOpStat(SetSysControl::ARENA_FIRST_TIME_TYPE, true);
	}else{
		m_bIsFirstEnter = false;
		startPvp(CArenaStartPanel::START_PVP, this);
	}
}

void CArenaFightScene::startPvp(const string& stEvent, CCObject* pstEventObj){
	if(m_bIsFirstEnter){
		P_ARENA_CTRL->getStartPanel()->RemoveEventListener(this, CArenaStartPanel::START_PVP, eventListner_selector(CArenaFightScene::startPvp));
	}

	this->scheduleUpdate();
}

void CArenaFightScene::setMapRes(){
	const CSArenaFindOpponentRsp& stArenaInfo = P_ARENA_CTRL->getArenaObj()->getFindInfo();
	const ArenaLvCfg* stCfg = ARENA_LV_CFG_MASTER->GetCfgByScore(stArenaInfo.score());
	m_uArenaLv = stCfg->level();
	m_stMapCfg = *ARENA_MAP_LIB_CFG_MASTER->GetArenaMapByLv(m_uArenaLv);
	m_fLimitTime = (float)m_stMapCfg.limittime();
	m_stFrontStr = m_stMapCfg.frontid();
	m_stForeStr = m_stMapCfg.foreid();
	m_stMidStr = m_stMapCfg.midid();
	m_stBackStr = m_stMapCfg.backid();
	m_stMid2Str = m_stMapCfg.midid2();
	_cfgFileName = m_stMapCfg.cfgid();
	m_uTotalStage = m_stMapCfg.stagenum();
}

void CArenaFightScene::setWalkHight(){
	m_uWalkHight = m_stMapCfg.midwalkh();
	m_fMidPos = m_stMapCfg.midpos();
	m_fStageW = _screenW;
}

void CArenaFightScene::loadHeroRes(){
	CBaseFightScene::loadHeroRes();


}

void CArenaFightScene::loadItemRes(){
	CBaseFightScene::loadItemRes();

	//电网特效
	string str = EFFECT_RES_PATH + "ut00054/ut00054.ExportJson";
	m_stRes.insert(str);

	//击杀特效
	str = EFFECT_RES_PATH + "ut00079/ut00079.ExportJson";
	m_stRes.insert(str);

	//对手身上特效
	str = EFFECT_RES_PATH + "ut00062/ut00062.ExportJson";
	m_stRes.insert(str);

	str = EFFECT_RES_PATH + "ut00063/ut00063.ExportJson";
	m_stRes.insert(str);

	//摧毁建筑特效
	str = EFFECT_RES_PATH + "ut00073/ut00073.ExportJson";
	m_stRes.insert(str);
}

void CArenaFightScene::loadMonsterRes(){
	m_stPosVec.clear();

	m_stAllBossData = CCArray::create();
	m_stAllBossData->retain();

	CCArray* stArr = CCArray::create();
	CCObject* pstObj;
	CCARRAY_FOREACH(_monsterData, pstObj) {
		CEnemyData* monsterObj = (CEnemyData*)pstObj;
		rectifyPos(monsterObj);
		if(monsterObj->id == 99999){  //对手的位置
			stArr->addObject(pstObj);
		}else{
			MonsterCfg stMonsterCfg;
			CNihilityUtils::genMonsterCfg(monsterObj->id, stMonsterCfg);
			loadMonsterResByCfg(&stMonsterCfg, monsterObj->num);

			if(stMonsterCfg.monstertype() == ROLE_TYPE_SENIOR_BOSS){   //boss建筑
				m_stAllBossData->addObject(monsterObj);
			}
		}
	}

	//记录对手刷怪点
	CCARRAY_FOREACH(stArr, pstObj) {
		CEnemyData* monsterObj = (CEnemyData*)pstObj;
		m_stPosVec.push_back(ccp(monsterObj->x, monsterObj->y));
		_monsterData->removeObject(pstObj);
	}

	//在刷怪数据中去掉boss建筑
	for(unsigned int i = 0; i < m_stAllBossData->count(); i++){
		CEnemyData* monsterObj = dynamic_cast<CEnemyData*>(m_stAllBossData->objectAtIndex(i));
		_monsterData->removeObject(monsterObj);
	}

	//对手相关资源
	//对手随机一把同星级、同阶级的枪
	vmsg::CSPlayerDetailInfo stInfo = P_ARENA_CTRL->getArenaObj()->getFightStartInfo().opponentdetail();
	m_uOpRandWeaponID = stInfo.weapon().cfgid();

	if(!P_ARENA_CTRL->getArenaObj()->getArenaInfo().isfirstfight() && m_uArenaLv > 1){   //第一次战斗或者最低级军衔，不随机枪
		const WeaponCfg* stCfg = WEAPON_CFG_MASTER->GetWeaponCfgByID(stInfo.weapon().cfgid());
		m_uOpRandWeaponID = WEAPON_CFG_MASTER->getWeaponRandByStarAndStep(stCfg->initstar(), stCfg->initstep(), stCfg->weaponid());
		CCLOG("CArenaFightScene::the opponent rand weapon cfg id == %u", m_uOpRandWeaponID);
		//m_uOpRandWeaponID = 115;
	}


	loadHeroResByShowID(stInfo.showid(), m_uOpRandWeaponID);
	loadWeaponeResByCfgID(m_uOpRandWeaponID);
	loadSkillRes(stInfo.evolutionskillid());

	unsigned int param1 = ConstHelper(ARENA_MONSTER_LV_PARAM_ONE);
	unsigned int param2 = ConstHelper(ARENA_MONSTER_LV_PARAM_TWO);
	int oppoFight = stInfo.fightpower();  //对手战力
	int mineFight = P_USER_OBJ->getUserInfo()->fightPower;  //自己战力
	int uOpLv = stInfo.level();
	int uMineLv = P_USER_OBJ->getUserInfo()->level;

	int uLv = int(uOpLv + (oppoFight - mineFight) * 1.0 / (param1 - param2 * (uOpLv - uMineLv)) + 0.5);
	if (uLv < 5)
	{
		uLv = 5;
	}else if (uLv > 150)
	{
		uLv = 150;
	}
	m_uMonsterLev = uLv;
}

void CArenaFightScene::rectifyPos(CEnemyData* pstObj){
	float limitW;
	unsigned int iStage = pstObj->stage;
	if(iStage < m_uTotalStage){
		limitW = m_fStageW*iStage;
	}else{
		limitW = m_fStageW*m_uTotalStage;
	}

	if(pstObj->x > limitW){
		pstObj->x = limitW - 10;
	}
}

void CArenaFightScene::initMap(){
	CBaseFightScene::initMap();

	walkableRect = CCRect(0, 0, m_fStageW, m_uWalkHight);
}

void CArenaFightScene::initFightData(){
	CBaseFightScene::initFightData();

	m_uStageIdx = 0;
	m_uOppDieNum = 0;
	m_uLastKillID = 0;
	m_fOppDieTime = 0;
	m_uBossTotalNum = 0;
	m_uKillBossNum = 0;
	m_bIsWin = false;
	m_bIsOppDying = false;
	m_bIsBossHited = false;
	m_bIsWinComplete = false;
	m_bIsAddCoinEff = false;
}

void CArenaFightScene::initSceneItem(){
	if(_itemData->count() <= 0){
		updateMapBlocks();
		return;
	}

	CCObject* pstObj;
	CItemObj* itemObj;
	CCARRAY_FOREACH(_itemData, pstObj) {
		itemObj = (CItemObj*)pstObj;
		if(itemObj->touchId != 0){  //触发id不为0，表示按触发线触发
			continue;
		}else{
			//添加物件
			addSceneItem(itemObj);
		}
	}

	updateMapBlocks();
}

void CArenaFightScene::initMapUI(){
	CBaseFightScene::initMapUI();

	CCPoint visibleOrigin = CCDirector::sharedDirector()->getVisibleOrigin();
	CCSize stSize = CCDirector::sharedDirector()->getVisibleSize(); 

	m_pInfoLayout = CArenaFightInfoLayout::create();
	float x = (stSize.width - m_pInfoLayout->getContentSize().width)/2;
	float y = stSize.height - m_pInfoLayout->getSize().height - 25;
	m_pInfoLayout->setPosition(visibleOrigin + ccp(x, y));
	m_pUiLayer->addWidget(m_pInfoLayout);

	m_pArrLayout->setEnabled(false);
	m_pChangeWeaponBtn->setEnabled(false);
}

void CArenaFightScene::initMainHero(){
	CBaseFightScene::initMainHero();
	_mainHero->ForceIdle();
	_mainHero->setIsBulletLimit(false);		//子弹无尽
}

void CArenaFightScene::initMonster(){
	initOpponent();

	m_uStageIdx = 0;
	setCurStageBoss();

	//构建所有boss建筑
	CCObject* pstObj;
	CCARRAY_FOREACH(m_stAllBossData, pstObj) {
		CEnemyData* monsterObj = (CEnemyData*)pstObj;
		addOneMonster(monsterObj, false);
	}

	setCurBossAI();

	//boss数量
	m_uBossTotalNum = m_stAllBossData->count();
	m_pInfoLayout->setDefNum(m_uKillBossNum, m_uBossTotalNum);

	initPowerGrid(); 
}

void CArenaFightScene::setCurBossAI(){
	CCObject* pstObj;
	CCARRAY_FOREACH(_stageEntities, pstObj){
		CQStageEntity* pstEntity = dynamic_cast<CQStageEntity*>(pstObj);
		if(pstEntity->getRoleParty() != ROLE_PARTY_HOSTILE || pstEntity->m_uID == m_pOpponentHero->m_uID){
			continue;
		}

		CEnemy* pMonster = dynamic_cast<CEnemy*>(pstObj);
		if(pMonster->getRoleType() == ROLE_TYPE_SENIOR_BOSS){
			if(pMonster->getMonsterObj()->stage != m_uStageIdx + 1){
				pMonster->SetIsControlledByAI(false);
			}else{
				pMonster->SetIsControlledByAI(true);
			}
		}
	}
}

void CArenaFightScene::initPowerGrid(){
	m_stPowerGrids = CCArray::create();
	m_stPowerGrids->retain();

	//构建电网
	for(unsigned int idx = 1; idx < m_uTotalStage; idx++){
		CCArmature* pArm = CCArmature::create("ut00054");
		float fScaleY = m_uWalkHight*2/pArm->getContentSize().height;
		pArm->setScaleY(fScaleY);
		pArm->setPosition(ccp(m_fStageW*idx, m_uWalkHight/2));
		int iNewZorder = CInstanceUtils::GetZOrderByType(FLOOR_BUFF_Z_ORDER, pArm->getPositionY());
		_sceneForegound->addChild(pArm, iNewZorder);
		m_stPowerGrids->addObject(pArm);
		pArm->getAnimation()->play("fly", -1, -1, 1);

		//CCSize stSize = CAnimationUtils::getBlockRect(pArm).size;
		//if(stSize.equals(CCSizeZero)){
		//	getBlocksInSize(stSize, (CCPoint)pArm->getPosition(), true);
		//	updateMapBlocks();
		//}
	}
}

void CArenaFightScene::initOpponent(){
	//对手
	vmsg::CSPlayerDetailInfo stInfo = P_ARENA_CTRL->getArenaObj()->getFightStartInfo().opponentdetail();
	const string* pHeroStr = HERO_CFG_MASTER->GetResName(stInfo.showid());
	if(NULL == pHeroStr) {
		CCLOG("[error]CArenaFightMapScene::initMonster, get hero res name fail! cfg id %u", stInfo.showid());
		return;
	}
	const string& heroStr = *pHeroStr;

	m_pOpponentHero = CMainHero::create();
	m_pOpponentHero->retain();

	string stBodyName;
	int iRet = CMainHero::GetResourceNames(stInfo.weapon().cfgid(), stInfo.showid(), stBodyName);
	if (iRet){
		CCLog("no resource name is found for hero %s", pHeroStr->c_str());
		return;
	}

	m_pOpponentHero->setShowID(stInfo.showid());
	m_pOpponentHero->setResPath(stBodyName);

	m_pOpponentHero->SetEnityPosition(getOpponentPos());
	m_pOpponentHero->setDesiredPosition(m_pOpponentHero->getPosition());
	m_pOpponentHero->setHurtActionCD(HERO_CFG_MASTER->GetHeroCfgByID(stInfo.showid())->stifftime());

	vmsg::CSPlayerDetailInfo* pDetailInfo = &stInfo;

	if(!P_ARENA_CTRL->getArenaObj()->getArenaInfo().isfirstfight()){   //第一次战斗，不随机枪
		vmsg::CSWeapon stTempWeapon;
		stTempWeapon.set_cfgid(m_uOpRandWeaponID);
		stTempWeapon.set_weaponguid(pDetailInfo->weapon().weaponguid());
		stTempWeapon.set_level(pDetailInfo->weapon().level());
		stTempWeapon.set_exp(pDetailInfo->weapon().exp());
		stTempWeapon.set_totalexp(pDetailInfo->weapon().totalexp());
		stTempWeapon.set_quality(pDetailInfo->weapon().quality()); 
		P_ARENA_CTRL->updateDetailInfoByWeapon(pDetailInfo, stTempWeapon);
	}

	CQStageEntityObj* heroInfo = CQStageEntityObj::create();
	heroInfo->setInfoByDetaiInfo(*pDetailInfo, ROLE_PARTY_HOSTILE);
	m_pOpponentHero->setObj(heroInfo);
	//m_pOpponentHero->setEffAtk(1);

	m_pOpponentHero->AddEventListener(this, CQStageEntity::SKILL_CAST_FRAME, eventListner_selector(CArenaFightScene::handleSkillKeyFrameEvent));
	m_pOpponentHero->AddEventListener(this, CQStageEntity::FIRE_EVENT, eventListner_selector(CArenaFightScene::handleFireABulletEvent));

	m_pOpponentHero->AddEventListener(this, HERO_LOSEVALUE_CHANGE, eventListner_selector(CArenaFightScene::handleOpponentHurtValueEvent));
	m_pOpponentHero->AddEventListener(this, HERO_KILLED_EVENT, eventListner_selector(CArenaFightScene::handleOpponentKilledEvent));
	m_pOpponentHero->AddEventListener(this, HERO_EPVALUE_ADD_EVENT, eventListner_selector(CArenaFightScene::handleHeroEPAddEvent));	//主角ep增加

	//枪械上的技能
	CQSkillCell* pstSkill = CQSkillCell::create();	
	pstSkill->m_uSkillID = WEAPON_CFG_MASTER->GetWeaponCfgByID(stInfo.weapon().cfgid())->weaponsuperskill();	
	pstSkill->m_uLastCastTime = 0.1f;	
	m_pOpponentHero->getSkillArray()->addObject(pstSkill);

	CQSkillCell* pstSkill2 = CQSkillCell::create();	
	pstSkill2->m_uSkillID = stInfo.evolutionskillid();	
	pstSkill2->m_uLastCastTime = 0.1f;	
	m_pOpponentHero->getSkillArray()->addObject(pstSkill2);

	m_pOpponentHero->LoadActionRes();
	m_pOpponentHero->setLevel(stInfo.level());
	m_pOpponentHero->getBlood()->setVisible(true);
	m_pOpponentHero->setShadowVisiable(true);
	m_pOpponentHero->setScaleX(-1);
	m_pOpponentHero->ForceIdle();
	int iNewZorder = CInstanceUtils::GetZOrderByType(ROLE_Z_ORDER, m_pOpponentHero->getPositionY());
	_sceneForegound->addChild(m_pOpponentHero, iNewZorder);
	_stageEntities->addObject(m_pOpponentHero);

	//设置AI
	m_pOpponentHero->SetIsControlledByAI(true);

	addOpTipEff();
}

void CArenaFightScene::addOpTipEff(){
	//脚底特效
	CCArmature* pArm = CCArmature::create("ut00062");
	m_pOpponentHero->addChild(pArm, -2);
	pArm->getAnimation()->play("fly", -1, -1, 1);

	//头顶特效
	CCArmature* pHeadArm = CCArmature::create("ut00063");
	pHeadArm->setPosition(ccp(0, m_pOpponentHero->getBlood()->getPositionY() + 40));
	m_pOpponentHero->addChild(pHeadArm, 2);
	pHeadArm->getAnimation()->play("fly", -1, -1, 1);
}

void CArenaFightScene::update(float delta){
	CBaseFightScene::update(delta);

	setViewpointCenter(_mainHero->getPosition());

	checkLimitTime();
	checkOppInRevive();
}

void CArenaFightScene::checkLimitTime(){
	if(m_bIsPreOver || m_bIsGameOver){
		return;
	}

	if(m_fLimitTime != 0 && m_fFightTime > m_fLimitTime){
		//超过时间,失败
		m_bIsWin = false;
		m_bIsGameOver = true;
		handleGameOver();
	}
}

void CArenaFightScene::checkOppInRevive(){
	if(m_bIsPreOver || m_bIsGameOver || !m_bIsOppDying){  //结束了或者未死亡
		return;
	}

	//5+3*死亡次数，复活时间范围【5,20】
	float fNeedTime = getReiviveTime();
	if((m_fFightTime - m_fOppDieTime) > fNeedTime){  //复活
		m_bIsOppDying = false;
		doReviveOpp();
	}
}

void CArenaFightScene::doReviveOpp(){
	m_uOppDieNum++;
	m_pOpponentHero->setVisible(true);
	m_pOpponentHero->Resume();
	m_pOpponentHero->setHP(m_pOpponentHero->getMaxHP());
	m_pOpponentHero->ForceIdle();
	m_pOpponentHero->setScaleX(-1.0f); //面向右
	m_pOpponentHero->SetIsControlledByAI(true);
	m_pOpponentHero->getBlood()->setVisible(true);
	m_pOpponentHero->setShadowVisiable(true);
	m_pOpponentHero->getBlood()->setPercentage(100);
	m_pOpponentHero->SetEnityPosition(getOpponentPos());
	m_pOpponentHero->setDesiredPosition(getOpponentPos());
	_stageEntities->addObject(m_pOpponentHero);
}

void CArenaFightScene::brushMonsterLogic(float dt){
	if(m_bIsGameOver || m_bIsPreOver){
		return;
	}

	CCObject* pstObj;
	CCARRAY_FOREACH(_monsterData, pstObj) {
		CEnemyData* monsterObj = (CEnemyData*)pstObj;
		if(monsterObj->stage != m_uStageIdx + 1){
			continue;
		}

		if(monsterObj->type == APPEAR_ARENA_SPECIAL){
			brushMonsterSpecial(monsterObj);
			continue;
		}

		CBurshObj stBrushObj;
		stBrushObj.pLayer = this;
		stBrushObj.pstEnemyObj = monsterObj;
		stBrushObj.iBrushTime = m_fFightTime;
		bool isBrush = CInstanceFightUtils::BrushMonsterLogic(stBrushObj);
	}
}

void CArenaFightScene::brushMonsterSpecial(CEnemyData* pstObj){
	if(!m_bIsBossHited){   //boss未被击中
		return;
	}

	if(getMonsterNumOnStage() > ConstHelper(FIGHT_STAGE_MAX_MONSTERS)){
		return;
	}

	int num = pstObj->num;
	bool bRepeated = false;		//是否重复刷怪
	bool isCanBrush = false;
	float lastBrushTime;

	if(pstObj->brushTime == 0){
		bRepeated = false;
	}else{
		bRepeated = true;
		lastBrushTime = pstObj->brushTime;
	}

	if(bRepeated == false){  //第一次刷怪
		if(pstObj->time <= (int)ceil(m_fFightTime)){
			isCanBrush = true;
		}
	}else{
		if(int(m_fFightTime - lastBrushTime) >= pstObj->inter){   //间隔时间刷怪
			isCanBrush = true;
		}
	}

	if(isCanBrush){
		brushOneMonster(pstObj);
		pstObj->num = (--num);
		pstObj->brushTime = m_fFightTime;
	}
}

void CArenaFightScene::setViewpointCenter(const CCPoint& position){
	float x;
	CCDirector *pDirector = CCDirector::sharedDirector();
	CCPoint screenOrigin = pDirector->getVisibleOrigin(); 
	CCSize screenSize = pDirector->getVisibleSize();
	CCSize innerSize = Multiresolution::getInnerSize();
	float scale = screenSize.height / innerSize.height;
	float visibleW = screenSize.width/scale;

	if(position.x <= visibleW/2){
		x = walkableRect.getMinX();
	}else if(position.x >= _screenW*m_uTotalStage - visibleW/2){
		x = _screenW*m_uTotalStage - visibleW;
	}else{
		x = position.x - visibleW/2;
	}

	CCPoint fgViewPoint = ccp(-x, 0);
	_sceneForegound->setPosition(fgViewPoint);

	updateBgViewCenter(fgViewPoint.x);
}

bool CArenaFightScene::checkArrowCondition(CQStageEntity* pstEntity){
	if(pstEntity->checkIsDied() || pstEntity->getRoleType() == ROLE_TYPE_SENIOR_BOSS
		|| pstEntity->getRoleParty() != ROLE_PARTY_HOSTILE){
		return false;
	}

	return true;
}

void CArenaFightScene::handleGiveUpLogic(CCObject* pstObj){
	P_FIGHT_OBJ->setIsForceQuit(true);
	m_bIsWin = false;

	handleGameOver();
}

void CArenaFightScene::handleGameOver(){
	if(!m_bIsWin){  //失败
		stopAllMonsters();
		m_pOpponentHero->stateChangeByAttack(false);
		m_pOpponentHero->ForceIdle();
		m_pOpponentHero->winAct(true);
		P_FIGHT_CTRL->setLastFightFailed(true);
	}else{  //清除未消失的怪物
		clearAllMonsters();
	}

	m_bIsGameOver = true;
	m_bIsPreOver = true;
	unscheduleUpdate();
	this->unschedule(schedule_selector(CArenaFightScene::updateCoinPos));

	UIN_t opponentUin = P_ARENA_CTRL->getArenaObj()->getOpponentUin();
	unsigned int result = (unsigned int)m_bIsWin;
	P_ARENA_CTRL->getArenaObj()->setIsMyWin(result);
	P_ARENA_CTRL->sendArenaEndFightRqst(opponentUin, result);

	P_ARENA_CTRL->getResultPanel()->showLoading();
}

void CArenaFightScene::brushOneMonster(CEnemyData* pstObj){
	MonsterCfg stCfg;
	CNihilityUtils::genMonsterCfg(pstObj->id, stCfg, m_uMonsterLev);
	if(stCfg.actions().emerge() != "null"){
		addOneMonster(pstObj, true);
	}else{
		//通用出场特效
		CCArmature* pArm = CCArmature::create("ut00038");
		pArm->getAnimation()->play("fly", -1, -1, 0);
		pArm->setUserObject(pstObj);
		pArm->setPosition(ccp(pstObj->x, _sceneForegound->getContentSize().height - pstObj->y));
		int iNewZorder = CInstanceUtils::GetZOrderByType(ROLE_Z_ORDER, pArm->getPositionY());
		_sceneForegound->addChild(pArm, iNewZorder);
		pArm->getAnimation()->setMovementEventCallFunc(this, movementEvent_selector(CArenaFightScene::handleComEmergeComplete));
	}
}

CEnemy* CArenaFightScene::addOneMonster(CEnemyData* pstObj, bool isEmerge){
	CEnemy* pstMonster = initOneMonster(pstObj);
	if(pstMonster == NULL){
		return NULL;
	}

	pstMonster->LoadActionRes(isEmerge);
	int iNewZorder = CInstanceUtils::GetZOrderByType(ROLE_Z_ORDER, pstMonster->getPositionY());
	pstMonster->setScaleX(-1);
	_sceneForegound->addChild(pstMonster, iNewZorder);

	if(!pstMonster->getBlockSize().equals(CCSizeZero)){
		getBlocksInSize(pstMonster->getBlockSize(), (CCPoint)pstMonster->getPosition(), true);
		updateMapBlocks();
	}

	if(pstMonster->getRoleType() == ROLE_TYPE_SENIOR_BOSS){   //boss炮台增加攻击提示特效
		addBossTipEff(pstMonster);   //todo
	}

	return pstMonster;
}

void CArenaFightScene::addBossTipEff(CEnemy* pEnemy){
	CCArmature* pArm = CCArmature::create("ut00073");
	pArm->setPosition(ccp(0, pArm->getContentSize().height + 30));
	pEnemy->addChild(pArm);
	pArm->getAnimation()->play("fly", -1, -1, 1);
}

CEnemy* CArenaFightScene::initOneMonster(CEnemyData* monsterObj){
	MonsterCfg stCfg;
	CNihilityUtils::genMonsterCfg(monsterObj->id, stCfg, m_uMonsterLev);
	CEnemy* pstMonster = CBaseFightScene::initOneMonster(monsterObj, &stCfg);
	if(stCfg.monstertype() == ROLE_TYPE_SENIOR_BOSS){   //boss炮台血量、攻击力
		unsigned int uOppentLv = P_ARENA_CTRL->getArenaObj()->getFightStartInfo().opponentdetail().level();
		const ArenaBossLevCfg* stBossCfg = ARENA_BOSS_LEV_CFG->GetArenaBossLevCfgByLv(m_uMonsterLev);
		pstMonster->setAIType(AI_TYPE_STANDSTILL_ATTACK);
		pstMonster->setHP(stBossCfg->hp());
		pstMonster->setMaxHP(stBossCfg->hp());
		pstMonster->setEffAtk(stBossCfg->atk());
		pstMonster->setLevel(uOppentLv);
	}
	return pstMonster;
}

bool CArenaFightScene::checkMonsterOnStage(CEnemyData* pstObj){
	unsigned int key = pstObj->key;

	CCObject* pObject;
	CQStageEntity* pstEntity;
	CCARRAY_FOREACH(_stageEntities, pObject){
		pstEntity = dynamic_cast<CQStageEntity*>(pObject);
		if(pstEntity->m_uID != m_pOpponentHero->m_uID && pstEntity->getRoleParty() == ROLE_PARTY_HOSTILE){//敌方
			CEnemy* pstEnemy = dynamic_cast<CEnemy*>(pObject);
			CEnemyData* pstEnemyData = pstEnemy->getMonsterObj();
			if(pstEnemyData->key == key){
				return true;
			}
		}
	}

	return false;
}

void CArenaFightScene::stopAllMonsters(){
	CCObject* enimy;
	CCARRAY_FOREACH(_stageEntities, enimy){
		unsigned int uID = enimy->m_uID;
		if(uID != _mainHero->m_uID && uID != m_pOpponentHero->m_uID){
			((CQStageEntity*)enimy)->ForceIdle();
			((CQStageEntity*)enimy)->Stop();
		}
	}
}

CCPoint CArenaFightScene::getOpponentPos(){
	if(m_uStageIdx >= m_stPosVec.size()){
		return CCPointZero;
	}

	CCPoint stPos = m_stPosVec.at(m_uStageIdx);
	return ccp(stPos.x, _sceneForegound->getContentSize().height - stPos.y);
}

void CArenaFightScene::setCurStageBoss(){
	google::protobuf::RepeatedField<google::protobuf::uint32> stBossIDs;
	switch (m_uStageIdx)
	{
	case 0:
		stBossIDs = m_stMapCfg.passbossid1();
		break;
	case 1:
		stBossIDs = m_stMapCfg.passbossid2();
		break;
	case 2:
		stBossIDs = m_stMapCfg.passbossid3();
		break;
	default:
		stBossIDs = m_stMapCfg.passbossid3();
		break;
	}
	
	m_stBossIDs.Clear();
	for(int i = 0; i < stBossIDs.size(); i++){
		m_stBossIDs.AddOneItem(stBossIDs.Get(i));
	}
}

bool CArenaFightScene::checkWinLogic(unsigned int cfgID){
	if(m_stBossIDs.Size() <= 0){
		return true;
	}

	int iDx = m_stBossIDs.DelValue(cfgID);
	if(iDx == -1){    //不是bossID
		return false;
	}else{
		if(m_stBossIDs.Size() > 0){
			return false;
		}else{
			return true;  //boss被杀完
		}
	}
		
}

void CArenaFightScene::delBrushByStage(){
	CCArray* stArr = CCArray::create();
	CCObject* pstObj;
	CCARRAY_FOREACH(_monsterData, pstObj) {
		CEnemyData* monsterObj = (CEnemyData*)pstObj;
		if(monsterObj->stage == (m_uStageIdx + 1)){
			stArr->addObject(pstObj);
		}
	}

	CCARRAY_FOREACH(stArr, pstObj) {
		_monsterData->removeObject(pstObj);
	}

	m_bIsBossHited = false;		//过阶段后重置
}

void CArenaFightScene::nextStage(unsigned int uID){
	if(m_uStageIdx + 1 >= m_uTotalStage){   //最后一个阶段，胜利
		if(m_uLastKillID == uID){
			if(m_bIsWinComplete){
				doWin();
			}else{
				m_bIsWinComplete = true;
			}
		}
		return;
	}

	//去掉电网  
	if(m_stPowerGrids->count() > 0){
		CCArmature* pArm = dynamic_cast<CCArmature*>(m_stPowerGrids->objectAtIndex(0));
		pArm->removeFromParentAndCleanup(true);
		m_stPowerGrids->removeObjectAtIndex(0);
	}

	//下一阶段
	m_uStageIdx++;
	setCurStageBoss();
	setCurBossAI();

	walkableRect = CCRect(0, 0, m_fStageW*(m_uStageIdx + 1), m_uWalkHight);
	updateMapBlocks();

	CCLOG("now the stage is %u, uID ==== %u", m_uStageIdx, uID);
}

void CArenaFightScene::doDieMovement(unsigned int cfgID, unsigned int uID){
	if(!checkWinLogic(cfgID)){
		return;
	}

	if(!m_bIsPreOver && m_uStageIdx + 1 >= m_uTotalStage){   //最后一个阶段，胜利
		m_bIsPreOver = true;
		m_uLastKillID = uID;		//记录胜利时候杀死的怪物id
		_mainHero->ForceIdle();
		_mainHero->setIsInvincible(true);
		_mainHero->winAct(true);
		unSetMainHeroAsTheOne();
		mainHeroCeaseShoot();
		clearAllSkills();
		clearAllBullet();
		clearAllArrows();
		forceOhterMonstersDie();			//其他怪物都死
		setAttackOpBtnVisible(false);		//隐藏技能攻击按钮
		CToolUtils::setTimeScale(0.5f);    //放慢1倍

		//播放胜利特效
		CCSize stSize = CCDirector::sharedDirector()->getVisibleSize(); 
		CCPoint visibleOrigin = CCDirector::sharedDirector()->getVisibleOrigin();
		CCArmature* pWinEff = CCArmature::create("ut00036");
		AudioManager::playEffect("MissionComplete.ogg");		//音效
		m_pUiLayer->addChild(pWinEff, 2014);
		pWinEff->setPosition(visibleOrigin + ccp(stSize.width/2, stSize.height/2));
		pWinEff->getAnimation()->setMovementEventCallFunc(this, movementEvent_selector(CArenaFightScene::handleWinEffComplete));
		pWinEff->getAnimation()->play("fly", -1, -1, 0);

		return;
	}

	//去掉通过阶段的刷怪点
	delBrushByStage();
}

void CArenaFightScene::doWin(){
	CToolUtils::setTimeScale(1.0f);
	P_FIGHT_OBJ->setIsForceQuit(false);
	m_bIsGameOver = true;
	m_bIsWin = true;
	handleGameOver();
}

void CArenaFightScene::doWinLogic(unsigned int uID){
	if(m_stBossIDs.Size() > 0){
		return;
	}

	nextStage(uID);
}

/************************************************************************/
/*                         事件处理                                     */
/************************************************************************/

void CArenaFightScene::doHeroDieFail(){
	//猪脚死亡
	m_bIsGameOver = true;
	m_bIsWin = false;
	P_FIGHT_OBJ->setIsForceQuit(false);
	handleGameOver();

	if(_stageEntities->indexOfObject(_mainHero) != UINT_MAX){
		_stageEntities->removeObject(_mainHero);
	}
}

void CArenaFightScene::handleWinEffComplete(CCArmature * armature, MovementEventType type, const char * name){
	if (type == COMPLETE){
		armature->removeFromParentAndCleanup(true);

		if(m_bIsWinComplete){  //与死亡动作比较，谁后播完，就结束
			doWin();
		}else{
			m_bIsWinComplete = true;
		}
	}	
}

void CArenaFightScene::handleEnemyHurtEvent(const string& stEvent, CCObject* pstEventObj){
	CBaseFightScene::handleEnemyHurtEvent(stEvent, pstEventObj);

	CEnemy* pstMonster = dynamic_cast<CEnemy*>(pstEventObj);
	if(pstMonster->getRoleType() == ROLE_TYPE_SENIOR_BOSS && !m_bIsBossHited){  //boss第一次受伤，开始刷特殊方式怪物
		m_bIsBossHited = true;
	}
}

void CArenaFightScene::handleEnemyDieMovementEvent(const string& stEvent, CCObject* pstEventObj){
	CEnemy* pstEntity = dynamic_cast<CEnemy*>(pstEventObj);
	unsigned int uCfgID = pstEntity->getMonsterObj()->id;
	unsigned int uID = pstEntity->m_uID;

	//怪物死亡，数据变化
	handleHeroKillValueChange(stEvent, pstEventObj);

	doDieMovement(uCfgID, uID);
}

void CArenaFightScene::handleEnemyDieCompleteEvent(const string& stEvent, CCObject* pstEventObj){
	CEnemy* pstEntity = dynamic_cast<CEnemy*>(pstEventObj);
	unsigned int uCfgID = pstEntity->getMonsterObj()->id;
	unsigned int uID = pstEntity->m_uID;

	//去阻挡
	removeEntityBlock(pstEntity);
	doWinLogic(uID);

	if (pstEntity->getParent()){
		_sceneForegound->removeChild(pstEntity, true);
		
		if(_stageEntities->indexOfObject(pstEntity) != UINT_MAX){
			_stageEntities->removeObject(pstEntity);
		}
	}
}

void CArenaFightScene::handleHeroKillValueChange(const string& stEvent, CCObject* pstEventObj){
	CBaseFightScene::handleHeroKillValueChange(stEvent, pstEventObj);

	CEnemy* pstEntity = dynamic_cast<CEnemy*>(pstEventObj);
	CEnemyData* pstEnemyData = pstEntity->getMonsterObj();

	if(pstEnemyData->type == APPEAR_AFTER_DIE){
		pstEnemyData->brushTime = m_fFightTime;
	}

	if(pstEntity->getRoleType() == ROLE_TYPE_SENIOR_BOSS){
		if(m_pBossHPUI->isVisible()){
			m_pBossHPUI->setVisible(false);
		}

		m_uKillBossNum++;
		m_pInfoLayout->setDefNum(m_uKillBossNum, m_uBossTotalNum);
	}
}

void CArenaFightScene::handleOpponentEPAddEvent(const string& stEvent, CCObject* pstEventObj){
	if(m_pOpponentHero->getEP() >= m_pOpponentHero->getMaxEP()){  //已经满了
		return;
	}

	unsigned int temp = m_pOpponentHero->getEP() + ConstHelper(FIGHT_EP_ADD_VALUE);
	if(temp >= m_pOpponentHero->getMaxEP()){
		m_pOpponentHero->setEP(m_pOpponentHero->getMaxEP());
	}else{
		m_pOpponentHero->setEP(temp);
	}
}

void CArenaFightScene::handleOpponentHurtValueEvent(const string& stEvent, CCObject* pstEventObj){
	//血量改变
	m_pOpponentHero->getBlood()->setPercentage(100.0*m_pOpponentHero->getHP()/m_pOpponentHero->getMaxHP());
}

void CArenaFightScene::handleOpponentKilledEvent(const string& stEvent, CCObject* pstEventObj){
	//对手死掉
	m_bIsOppDying = true;
	m_fOppDieTime = m_fFightTime;

	//5+3*死亡次数，复活时间范围【5,20】
	float fNeedTime = getReiviveTime();
	if(!m_bIsPreOver){
		m_pInfoLayout->setTime(fNeedTime);  //开始倒计时
	}

	playKillHeroEff();

	m_pOpponentHero->SetIsControlledByAI(false);
	m_pOpponentHero->setVisible(false);
	m_pOpponentHero->Stop();

	if(_stageEntities->indexOfObject(m_pOpponentHero) != UINT_MAX){
		_stageEntities->removeObject(m_pOpponentHero);
	}
}

float CArenaFightScene::getReiviveTime(){
	float fNeedTime = 8 + 3*m_uOppDieNum;
	if(fNeedTime > 20){
		fNeedTime = 20;
	}

	return fNeedTime;
}

void CArenaFightScene::playKillHeroEff(){
	if(m_bIsPreOver){
		return;
	}

	CCSize stSize = CCDirector::sharedDirector()->getVisibleSize(); 
	CCPoint visibleOrigin = CCDirector::sharedDirector()->getVisibleOrigin();
	CCArmature* pArm = CCArmature::create("ut00079");
	pArm->setPosition(visibleOrigin + ccp(stSize.width/2, stSize.height/2));
	m_pUiLayer->addChild(pArm, 1000);
	pArm->getAnimation()->setMovementEventCallFunc(this, movementEvent_selector(CArenaFightScene::handleKillEffComplete));

	const ArenaKillNumCfg* stCfg = ARENA_KILL_NUM_CFG->GetCfgByKillNum(m_uOppDieNum + 1);
	string actName = "fly_" + intToString(stCfg->lev() - 1);
	pArm->getAnimation()->play(actName.c_str(), -1, -1, 0);

	AudioManager::playEffect(stCfg->killsound().c_str());

	//主角血量、ep回复
	doHeroReplyHp();
}

void CArenaFightScene::handleKillEffComplete(CCArmature * armature, MovementEventType type, const char * name){
	if (type == COMPLETE){
		armature->removeFromParentAndCleanup(true);
	}
}

void CArenaFightScene::doHeroReplyHp(){
	_mainHero->setHP(_mainHero->getMaxHP());
	_mainHero->setEP(_mainHero->getMaxEP());

	m_pHeroHeadUI->setHPPercent(_mainHero->getHP(), _mainHero->getMaxHP());
	m_pHeroHeadUI->setEPPercent(_mainHero->getEP(), _mainHero->getMaxEP());
}

