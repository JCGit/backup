#include "BaseFightScene.h"
#include "instance/InstanceUtils.h"
#include "instance/InstanceAI.h"
#include "UserObj.h"
#include "CCColorUtil.h"
#include "FightObj.h"
#include "CommonDialog.h"
#include "SceneBuff.h"
#include "LevelControl.h"
#include "GlobalTime.h"
#include "WeaponBuff.h"
#include "PunctureTrap.h"
#include "vlib/CVRand.h"
#include "ToolUtils.h"
#include "QEnvSet.h"
#include "BossWarning.h"
#include "ConstHelper.h"
#include "TextHelper.h"
#include "PopTipLayer.h"
#include "Multiresolution.h"
#include "control/AudioManager.h"
#include "weaponlib/WeaponLibControl.h"
#include "instance/AnimationUtils.h"
#include "instance/InstanceSkillUtils.h"
#include "instance/InstanceFightUtils.h"
#include "fight/AutoFightLayer.h"
#include "ConstHelper.h"
#include "FightWeaponArrLayout.h"
#include "FightHeroReliveLayer.h"
#include "bag/BagControl.h"
#include "FightControl.h"

#include "MapCfgMaster.h"
#include "HeroCfgMaster.h"
#include "WeaponCfgMaster.h"
#include "SkillCfgMaster.h"
#include "BulletCfgMaster.h"
#include "WeaponBuffCfgMaster.h"
#include "BuffCfgMaster.h"
#include "FightKillNumCfgMaster.h"

const unsigned int CBaseFightScene::HURT_ARM_TAG = 1001;
const unsigned int CBaseFightScene::KILL_EFF_TAG = 2001;

CBaseFightScene::CBaseFightScene()
:_stageEntities(NULL)
,_sceneItems(NULL)
,_skills(NULL)
,_hero_bullets(NULL)
,_mainHero(NULL)
,m_fFightTime(0)
,m_fLastAniTime(0)
,m_fMidPos(0)
,m_pGameLayer(NULL)
,m_pWidgeLayer(NULL)
,m_pUiLayer(NULL)
,_hudLayer(NULL)
,m_pHeroHeadUI(NULL)
,m_pBossHPUI(NULL)
,m_pSkillLayout(NULL)
,m_pHitLayout(NULL)
,fireBtn(NULL)
,pauseBtn(NULL)
,_sceneBuffs(NULL)
,_sceneTraps(NULL)
,m_trickBtn(NULL)
,m_skillBtn(NULL)
,m_dodgeBtn(NULL)
,m_pBossWarning(NULL)
,_moveEntities(NULL)
,_sleepMonsters(NULL)
,m_stCoinEffs(NULL)
,m_stAddCoins(NULL)
,m_pAutoLayer(NULL)
,m_pArrLayout(NULL)
,m_uSkillEffCnt(0)
,m_uCurSKillCnt(0)
,m_uCurAniTotalNum(0)
,m_uBulletSkillBuffNum(0)
,m_uAniMaxNum(0)
,m_fAniInter(0)
{
	_sceneForegound = NULL;
	_sceneFrontgound = NULL;
	_sceneBackground = NULL;
	_sceneMidground = NULL;

	_stageEntities = CCArray::create();
	_stageEntities->retain();

	_sleepMonsters = CCArray::create();
	_sleepMonsters->retain();

	_moveEntities = CCArray::create();
	_moveEntities->retain();

	_sceneItems = CCArray::create();
	_sceneItems->retain();

	_skills = CCArray::create();
	_skills->retain();

	_hero_bullets = CCArray::create();
	_hero_bullets->retain();

	_sceneBuffs = CCArray::create();
	_sceneBuffs->retain();

	_sceneTraps = CCArray::create();
	_sceneTraps->retain();

	m_stCoinEffs = CCArray::create();
	m_stCoinEffs->retain();

	m_stAddCoins = CCArray::create();
	m_stAddCoins->retain();

	m_stRightArrowArr = CCArray::create();
	m_stRightArrowArr->retain();

	m_stLeftArrowArr = CCArray::create();
	m_stLeftArrowArr->retain();
}

CBaseFightScene::~CBaseFightScene(){
	CC_SAFE_RELEASE_NULL(_sceneForegound);
	CC_SAFE_RELEASE_NULL(_sceneFrontgound);
	CC_SAFE_RELEASE_NULL(_sceneBackground);
	CC_SAFE_RELEASE_NULL(_sceneMidground);
	CC_SAFE_RELEASE_NULL(_sceneMid2ground);
	CC_SAFE_RELEASE_NULL(m_pHitLayout);
	CC_SAFE_RELEASE_NULL(m_pArrLayout);

	if (_stageEntities){
		_stageEntities->removeAllObjects();
        _stageEntities->release();
		_stageEntities = NULL;
	}

	if (_sleepMonsters){
		_sleepMonsters->removeAllObjects();
		_sleepMonsters->release();
		_sleepMonsters = NULL;
	}

	if (_moveEntities){
		_moveEntities->removeAllObjects();
		_moveEntities->release();
		_moveEntities = NULL;
	}

	if (_skills){
        _skills->removeAllObjects();
        _skills->release();
		_skills = NULL;
	}

	if(_sceneItems){
        _sceneItems->removeAllObjects();
        _sceneItems->release();
		_sceneItems = NULL;
	}

	if(_hero_bullets){
        _hero_bullets->removeAllObjects();
        _hero_bullets->release();
		_hero_bullets = NULL;
	}

	if(_sceneBuffs){
		_sceneBuffs->removeAllObjects();
		_sceneBuffs->release();
		_sceneBuffs = NULL;
	}

	if(_sceneTraps){
		_sceneTraps->removeAllObjects();
		_sceneTraps->release();
		_sceneTraps = NULL;
	}

	if(m_stRightArrowArr){
		m_stRightArrowArr->removeAllObjects();
		m_stRightArrowArr->release();
		m_stRightArrowArr = NULL;
	}
	
	if(m_stLeftArrowArr){
		m_stLeftArrowArr->removeAllObjects();
		m_stLeftArrowArr->release();
		m_stLeftArrowArr = NULL;
	}

	if(m_stCoinEffs){
		m_stCoinEffs->removeAllObjects();
		CC_SAFE_RELEASE_NULL(m_stCoinEffs);
	}

	if(m_stAddCoins){
		m_stAddCoins->removeAllObjects();
		CC_SAFE_RELEASE_NULL(m_stAddCoins);
	}

	CCNotificationCenter::sharedNotificationCenter()->removeAllObservers(this);
}

bool CBaseFightScene::init(){
	bool bRet = false;

	do {
		CC_BREAK_IF(!CBaseMapScene::init());

		addLayer(LAYER_ID_GAME);
		addLayer(LAYER_ID_WIDGET);
		addLayer(LAYER_ID_HUD);
		addLayer(LAYER_ID_FIGHT);
		addLayer(LAYER_ID_UI);
		addLayer(LAYER_ID_POP);
		addLayer(LAYER_ID_TOP);

		m_uAniMaxNum = ConstHelper(FIGHT_ANI_EFF_MAX);
		m_fAniInter = 1.0f*ConstHelper(FIGHT_ANI_EFF_INTER)/1000;

		bRet = true;  
	} while(0);

	return bRet;
}

void CBaseFightScene::onEnterTransitionDidFinish(){
	CBaseMapScene::onEnterTransitionDidFinish();
	CCTouchDispatcher *pDispatcher = CCDirector::sharedDirector()->getTouchDispatcher();
	dynamic_cast<CCTargetedTouchHandler*>(pDispatcher->findHandler(m_pUiLayer))->setSwallowsTouches(false);
}

void CBaseFightScene::setWalkHight(){
	m_fMidPos = MAP_CFG_MASTER->GetMapCfgByID(getSceneID())->midpos();
	m_uWalkHight = MAP_CFG_MASTER->GetMapCfgByID(m_uSceneID)->midwalkh();
}

void CBaseFightScene::loadHeroRes(){
	const UserInfo* pUserInfo = P_USER_OBJ->getUserInfo();	//玩家信息
	
	loadHeroResByShowID(pUserInfo->showID, pUserInfo->weapon.cfgid());

	//todo  
	string str = "skill/sanguang/sanguang.ExportJson";
	m_stRes.insert(str);

	//主角受伤特效
	str = EFFECT_RES_PATH + "ut00021/ut00021.ExportJson";
	m_stRes.insert(str);

	//技能预警特效
	str = SKILL_RES_PATH + "warning/warning.ExportJson";
	m_stRes.insert(str);

	//胜利特效
	str = EFFECT_RES_PATH + "ut00036/ut00036.ExportJson";
	m_stRes.insert(str);

	//怪物通用出场
	str = EFFECT_RES_PATH + "ut00038/ut00038.ExportJson";
	m_stRes.insert(str);

	str = EFFECT_RES_PATH + "ut00091/ut00091.ExportJson";
	m_stRes.insert(str);
	//miss complete音效
	str = SOUND_RES_PATH + "MissionComplete.ogg";
	addEffSound(str);

	//技能预警音效
	str = SOUND_RES_PATH + "skill_warning.ogg";
	addEffSound(str);

	//金币特效
	str = EFFECT_RES_PATH + "ut00042/ut00042.ExportJson";
	m_stRes.insert(str);

	//托管特效
	str = EFFECT_RES_PATH + "ut00078/ut00078.ExportJson";
	m_stRes.insert(str);

	//金币音效
	str = SOUND_RES_PATH + "goldcoin.ogg";
	addEffSound(str);

	//金币消失特效
	str = SOUND_RES_PATH + "goldcoin_die.ogg";
	addEffSound(str);

	//主角复活无敌buff
	const BuffCfg* stCfg = BUFF_CFG_MASTER->GetBuffCfgByID(ConstHelper(FIGHT_RELIVE_INVINCIBLE_BUFF_ID));
	if(stCfg && stCfg->animation() != "null"){
		str = BUFF_RES_PATH + stCfg->animation() + "/" + stCfg->animation() + ".ExportJson";
		m_stRes.insert(str);
	}

	//主角闪避无敌buff
	stCfg = BUFF_CFG_MASTER->GetBuffCfgByID(ConstHelper(FIGHT_DASH_INVINCIBLE_BUFF_ID));
	if(stCfg && stCfg->animation() != "null"){
		str = BUFF_RES_PATH + stCfg->animation() + "/" + stCfg->animation() + ".ExportJson";
		m_stRes.insert(str);
	}

	//主角补充弹药箱无敌buff
	stCfg = BUFF_CFG_MASTER->GetBuffCfgByID(ConstHelper(FIGHT_BULLET_INVINCIBLE_BUFF_ID));
	if(stCfg && stCfg->animation() != "null"){
		str = BUFF_RES_PATH + stCfg->animation() + "/" + stCfg->animation() + ".ExportJson";
		m_stRes.insert(str);
	}

	//枪械
	loadWeaponeResByCfgID(pUserInfo->weapon.cfgid());

	//猪脚技能
	loadSkillRes(pUserInfo->evolutionSkillID);

	//连杀特效、音效资源
	str = EFFECT_RES_PATH + "ut00077/ut00077.ExportJson";
	m_stRes.insert(str);

	//隐藏宝箱开启特效
	str = EFFECT_RES_PATH + "ut00092/ut00092.ExportJson";
	m_stRes.insert(str);

	for(int i = 0; i < FIGHT_KILL_NUM_CFG->GetCfg().fightkillnumcfg_size(); i++){
		const FightKillNumCfg& stCfg = FIGHT_KILL_NUM_CFG->GetCfg().fightkillnumcfg(i);
		if(stCfg.killsound() != "null"){
			string str = SOUND_RES_PATH + stCfg.killsound();
			addEffSound(str);
		}
	}
}

void CBaseFightScene::loadItemRes(){
	for(unsigned int i = 0; i < _itemData->count(); i++){
		CItemObj* itemObj = (CItemObj*)_itemData->objectAtIndex(i);
		loadItemResByID(itemObj->id, itemObj->dropType, itemObj->dropId);
	}
}

void CBaseFightScene::loadFloorBuffRes(){
	for(unsigned int i = 0; i < _floorBuffData->count(); i++){
		CFloorBuffObj* buffObj = (CFloorBuffObj*)_floorBuffData->objectAtIndex(i);
		loadFloorBuffByID(buffObj->id, true);
	}
}

void CBaseFightScene::loadTrapRes(){
	for(unsigned int i = 0; i < _trapData->count(); i++){
		CTrapObj* trapObj = (CTrapObj*)_trapData->objectAtIndex(i);
		loadTrapResByID(trapObj->id, true);
	}
}

void CBaseFightScene::loadItemResByID(unsigned int itemID, unsigned int dropType, unsigned int dropID, bool isPreLoad /* = true */){
	SceneItemCfg* itemCfg = SCENE_ITEM_CFG->GetSceneItemCfgByID(itemID);

	string str = SCENE_ITEM_RES_PATH + itemCfg->itemres() + "/" + itemCfg->itemres() + JSON_TYPE_NAME;

	if(isPreLoad){
		m_stRes.insert(str);
	}else{
		addResToList(str);
	}

	if(dropID != 0){
		loadFloorBuffByID(dropID, isPreLoad);
	}
}

void CBaseFightScene::loadHeroResByShowID(unsigned int showID, unsigned int weaponCfgID, bool isPreLoad){
	const HeroCfg* heroCfg = HERO_CFG_MASTER->GetHeroCfgByID(showID);

	string str = heroCfg->resname();
	string stBodyName;
	int iRet = CMainHero::GetResourceNames(weaponCfgID, showID, stBodyName);
	string bodyStr = HERO_RES_PATH + stBodyName + "/" + stBodyName + JSON_TYPE_NAME;

	if(isPreLoad){
		m_stRes.insert(bodyStr);
	}else{
		addResToList(bodyStr);
	}

	//把主角所有的身体资源预加载
	stBodyName = str + "_body_sq";
	bodyStr = HERO_RES_PATH + stBodyName + "/" + stBodyName + JSON_TYPE_NAME;
	if(isPreLoad){
		m_stRes.insert(bodyStr);
	}else{
		addResToList(bodyStr);
	}

	stBodyName = str + "_body_cq";
	bodyStr = HERO_RES_PATH + stBodyName + "/" + stBodyName + JSON_TYPE_NAME;
	if(isPreLoad){
		m_stRes.insert(bodyStr);
	}else{
		addResToList(bodyStr);
	}

	//人物音效
	string soundStr;
	if(heroCfg->emergevoice() != "null"){
		soundStr = SOUND_RES_PATH + heroCfg->emergevoice();
		addEffSound(soundStr);
	}

	if(heroCfg->hurtvoice() != "null"){
		soundStr = SOUND_RES_PATH + heroCfg->hurtvoice();
		addEffSound(soundStr);
	}

	if(heroCfg->dievoice() != "null"){
		soundStr = SOUND_RES_PATH + heroCfg->dievoice();
		addEffSound(soundStr);
	}

	if(heroCfg->switchweaponvoice() != "null"){
		soundStr = SOUND_RES_PATH + heroCfg->switchweaponvoice();
		addEffSound(soundStr);
	}
}

void CBaseFightScene::loadWeaponeResByCfgID(unsigned int cfgID, bool isPreLoad){
	const WeaponCfg* cfg = WEAPON_CFG_MASTER->GetWeaponCfgByID(cfgID);

	//武器技能
	loadSkillRes(cfg->weaponsuperskill(), isPreLoad);

	//子弹
	const BulletCfg* bulletCfg = BULLET_CFG_MASTER->GetBulletCfgByID(cfg->bulletid());
	string str;
	if(bulletCfg->oridinaryani() != "null"){
		str = BULLET_RES_PATH + bulletCfg->oridinaryani() + "/" + bulletCfg->oridinaryani() + JSON_TYPE_NAME;
		if(isPreLoad){
			m_stRes.insert(str);
		}else{
			addResToList(str);
		}
	}

	if(bulletCfg->superani() != "null"){
		str = BULLET_RES_PATH + bulletCfg->superani() + "/" + bulletCfg->superani() + JSON_TYPE_NAME;
		if(isPreLoad){
			m_stRes.insert(str);
		}else{
			addResToList(str);
		}
	}

	if(bulletCfg->beatanis() != "null"){
		str = BULLET_RES_PATH + bulletCfg->beatanis() + "/" + bulletCfg->beatanis() + JSON_TYPE_NAME;
		if(isPreLoad){
			m_stRes.insert(str);
		}else{
			addResToList(str);
		}
	}

	if(bulletCfg->shootanimation() != "null"){
		str = BULLET_RES_PATH + bulletCfg->shootanimation() + "/" + bulletCfg->shootanimation() + JSON_TYPE_NAME;
		if(isPreLoad){
			m_stRes.insert(str);
		}else{
			addResToList(str);
		}
	}

	if(bulletCfg->papapa() != "null"){
		str = EFFECT_RES_PATH + bulletCfg->papapa() + "/" + bulletCfg->papapa() + JSON_TYPE_NAME;
		if(isPreLoad){
			m_stRes.insert(str);
		}else{
			addResToList(str);
		}
	}

	if(cfg->firesound() != "null"){
		str = SOUND_RES_PATH + cfg->firesound();
		addEffSound(str);
	}

	if(bulletCfg->additionaleffs_size() > 0){  //子弹附加效果，一般为buff
		for(int i = 0; i < bulletCfg->additionaleffs_size(); i++){
			if(bulletCfg->additionaleffs(i).efftype() == SKILL_EFFECT_TYPE::SKILL_EFFECT_ADD_BUFF){  //buff资源
				BuffCfg* stBuffCfg = BUFF_CFG_MASTER->GetBuffCfgByID(bulletCfg->additionaleffs(i).effval());
				if(stBuffCfg != NULL){
					str = BUFF_RES_PATH + stBuffCfg->animation() + "/" + stBuffCfg->animation() + JSON_TYPE_NAME;
					if(isPreLoad){
						m_stRes.insert(str);
					}else{
						addResToList(str);
					}
				}
			}
		}
	}

}

void CBaseFightScene::loadSkillRes(unsigned int skillID, bool isPreLoad){
	const SkillCfg* skillCfg = SKILL_CFG_MASTER->GetSkillCfgByID(skillID);
	string skillStr;

	if(skillCfg->gunanimation() != "null" && skillCfg->gunanimation().length()){  //验证
		skillStr = SKILL_RES_PATH + skillCfg->gunanimation() + "/" + skillCfg->gunanimation() + JSON_TYPE_NAME;
		if(isPreLoad){
			m_stRes.insert(skillStr);
		}else{
			addResToList(skillStr);
		}
	}

	if(skillCfg->castanimation() != "null"){  //验证
		skillStr = SKILL_RES_PATH + skillCfg->castanimation() + "/" + skillCfg->castanimation() + JSON_TYPE_NAME;
		if(isPreLoad){
			m_stRes.insert(skillStr);
		}else{
			addResToList(skillStr);
		}
	}

	if(skillCfg->flyanimation() != "null"){  //验证
		skillStr = SKILL_RES_PATH + skillCfg->flyanimation() + "/" + skillCfg->flyanimation() + JSON_TYPE_NAME;
		if(isPreLoad){
			m_stRes.insert(skillStr);
		}else{
			addResToList(skillStr);
		}
	}

	if(skillCfg->hurtanimation() != "null"){  //验证
		skillStr = SKILL_RES_PATH + skillCfg->hurtanimation() + "/" + skillCfg->hurtanimation() + JSON_TYPE_NAME;
		if(isPreLoad){
			m_stRes.insert(skillStr);
		}else{
			addResToList(skillStr);
		}
	}

	if(skillCfg->floorbuffid() != 0){
		loadFloorBuffByID(skillCfg->floorbuffid(), isPreLoad);
	}

	//技能音效
	string soundStr;
	if(skillCfg->castsound() != "null" && skillCfg->castsound() != ""){
		soundStr = SOUND_RES_PATH + skillCfg->castsound();
		addEffSound(soundStr);
	}

	if(skillCfg->atksound() != "null" && skillCfg->atksound() != ""){
		soundStr = SOUND_RES_PATH + skillCfg->atksound();
		addEffSound(soundStr);
	}

	if(skillCfg->hitsound() != "null" && skillCfg->hitsound() != ""){
		soundStr = SOUND_RES_PATH + skillCfg->hitsound();
		addEffSound(soundStr);
	}

	//增加技能相关的buff
	for (int i = 0; i < skillCfg->effects_size(); i++){
		if (skillCfg->effects(i).effecttype() == SKILL_EFFECT_ADD_BUFF){
			loadBuffRes(skillCfg->effects(i).effectval(), isPreLoad);
		}
	}

	updateResToNum(skillCfg->skillid(), SKILL_TYPE, true);
}

void CBaseFightScene::loadFloorBuffByID(unsigned int id, bool isPreLoad /* = true */){
	const FloorBuffCfg* floorBuffCfg = FLOOR_BUFF_CFG->GetFloorBuffCfgByID(id);
	string str = SCENE_ITEM_RES_PATH + floorBuffCfg->res() + "/" + floorBuffCfg->res() + JSON_TYPE_NAME;
	if(isPreLoad){
		m_stRes.insert(str);
	}else{
		addResToList(str);
	}

	if(floorBuffCfg->bufftype() == BUFF_TYPE_WEAPON){  //临时枪械固有的底部特效
		string bottomStr = CWeaponBuff::WEAPON_BUFF_BOTTOM_RES;
		str = SCENE_ITEM_RES_PATH + bottomStr + "/" + bottomStr + JSON_TYPE_NAME;

		if(isPreLoad){
			m_stRes.insert(str);
		}else{
			addResToList(str);
		}

		//加载枪械相关的资源
		const WeaponBuffCfg* weaponBuffCfg = WEAPON_BUFF_CFG->GetWeaponBuffCfgByID(floorBuffCfg->buffid());
		loadHeroResByShowID(P_USER_OBJ->getUserInfo()->showID, weaponBuffCfg->weaponcfgid(), isPreLoad);
		loadWeaponeResByCfgID(weaponBuffCfg->weaponcfgid(), isPreLoad);
		loadWeaponStarUpRes(weaponBuffCfg->weaponcfgid(), isPreLoad);  //把临时枪械的所有星级的资源都加载上
	}else if(floorBuffCfg->bufftype() == BUFF_TYPE_WEAPON_UP || floorBuffCfg->bufftype() == BUFF_TYPE_WEAPON_MAX){  
		//升星、满星buff
		loadWeaponStarUpRes(P_USER_OBJ->getUserInfo()->weaponCfgID, isPreLoad);
	}else{  //其他类型的地表buff所带来的buff资源
		loadBuffRes(floorBuffCfg->buffid(), isPreLoad);
	}
}

void CBaseFightScene::loadWeaponStarUpRes(unsigned int weaponCfgID, bool isPreLoad /* = true */){
	const WeaponCfg* stWeaponCfg = WEAPON_CFG_MASTER->GetWeaponCfgByID(weaponCfgID);
	unsigned int uWeaponID = stWeaponCfg->weaponid();
	unsigned int uStar = stWeaponCfg->initstar();
	unsigned int uMaxStar = stWeaponCfg->maxstar();
	if(uStar >= uMaxStar){  //已经满星
		return;
	}

	WeaponCfg stCfg;
	for(unsigned int i = uStar + 1; i <= uMaxStar; i++){
		WEAPON_CFG_MASTER->GetWeaponCfgByStarAndStep(uWeaponID, i, 0, stCfg);
		loadHeroResByShowID(P_USER_OBJ->getUserInfo()->showID, stCfg.cfgid(), isPreLoad);
		loadWeaponeResByCfgID(stCfg.cfgid(), isPreLoad);
	}
}

void CBaseFightScene::loadTrapResByID(unsigned int cfgId, bool isPreLoad /* = true */){
	const TrapCfg* trapCfg = TRAP_CFG_MASTER->GetTrapCfgByID(cfgId);
	string str = SCENE_ITEM_RES_PATH + trapCfg->res() + "/" + trapCfg->res() + JSON_TYPE_NAME;

	if(isPreLoad){
		m_stRes.insert(str);
	}else{
		addResToList(str);
	}

	loadBuffRes(trapCfg->buffid(), isPreLoad);
}

void CBaseFightScene::loadBuffRes(unsigned int uBuffID, bool isPreLoad /* = true */){
	const BuffCfg* pstBuffCfg = BUFF_CFG_MASTER->GetBuffCfgByID(uBuffID);
	if (!pstBuffCfg){
		return;
	}

	loadBuffRes(pstBuffCfg->animation(), isPreLoad);

}

void CBaseFightScene::loadBuffRes(const string& stBuffRes, bool isPreLoad /* = true */){
	string stResPath = BUFF_RES_PATH + stBuffRes + "/" + stBuffRes + JSON_TYPE_NAME;
	if(isPreLoad){
		m_stRes.insert(stResPath);

	}else{
		addResToList(stResPath);
	}
}

void CBaseFightScene::loadMonsterResByCfg(MonsterCfg* stCfg, int num, bool isPreLoad){
	string str = MONSTER_RES_PATH + stCfg->avatar() + "/" + stCfg->avatar() + JSON_TYPE_NAME;	
	if(isPreLoad){
		m_stRes.insert(str);
	}else{
		addResToList(str);
	}

	//怪物资源使用数量
	updateResToNumByStr(str, true, num);

	//死亡特效
	if(stCfg->dieanimation() != "null"){
		str = SKILL_RES_PATH + stCfg->dieanimation() + "/" + stCfg->dieanimation() + JSON_TYPE_NAME;
		if(isPreLoad){
			m_stRes.insert(str);
		}else{
			addResToList(str);
		}
	}

	//死亡后地表buff
	if(stCfg->floorbuffid() != 0){
		loadFloorBuffByID(stCfg->floorbuffid(), isPreLoad);
	}

	//受击特效
	if(stCfg->hurtanimation() != "null"){
		str = SKILL_RES_PATH + stCfg->hurtanimation() + "/" + stCfg->hurtanimation() + JSON_TYPE_NAME;
		if(isPreLoad){
			m_stRes.insert(str);
		}else{
			addResToList(str);
		}

		//死亡特效使用数量
		updateResToNumByStr(str, true, num);
	}

	//怪物音效
	string soundStr;
	if(stCfg->emergevoice() != "null"){
		soundStr = SOUND_RES_PATH + stCfg->emergevoice();
		addEffSound(soundStr);
	}

	if(stCfg->hurtvoice() != "null"){
		soundStr = SOUND_RES_PATH + stCfg->hurtvoice();
		addEffSound(soundStr);
	}

	if(stCfg->dievoice() != "null"){
		soundStr = SOUND_RES_PATH + stCfg->dievoice();
		addEffSound(soundStr);
	}

	//技能资源，读配置得到对应的技能
	for(int j = 0; j < stCfg->skills_size(); j++){
		loadSkillRes(stCfg->skills(j), isPreLoad);
	}
}

void CBaseFightScene::loadMonsterResByCfg(const MonsterShowCfg* stCfg, int num, bool isPreLoad){
	string str = MONSTER_RES_PATH + stCfg->avatar() + "/" + stCfg->avatar() + JSON_TYPE_NAME;	
	if(isPreLoad){
		m_stRes.insert(str);
	}else{
		addResToList(str);
	}

	//怪物资源使用数量
	updateResToNumByStr(str, true, num);

	//死亡特效
	str = SKILL_RES_PATH + stCfg->dieanimation() + "/" + stCfg->dieanimation() + JSON_TYPE_NAME;
	if(isPreLoad){
		m_stRes.insert(str);
	}else{
		addResToList(str);
	}

	//死亡特效使用数量
	updateResToNumByStr(str, true, num);

	//怪物音效
	string soundStr;
	if(stCfg->emergevoice() != "null"){
		soundStr = SOUND_RES_PATH + stCfg->emergevoice();
		addEffSound(soundStr);
	}

	if(stCfg->hurtvoice() != "null"){
		soundStr = SOUND_RES_PATH + stCfg->hurtvoice();
		addEffSound(soundStr);
	}

	if(stCfg->dievoice() != "null"){
		soundStr = SOUND_RES_PATH + stCfg->dievoice();
		addEffSound(soundStr);
	}

	//技能资源，读配置得到对应的技能
	for(int j = 0; j < stCfg->skills_size(); j++){
		loadSkillRes(stCfg->skills(j), isPreLoad);
	}
}

void CBaseFightScene::initMap(){
	CBaseMapScene::initMap();

	CCDirector *pDirector = CCDirector::sharedDirector();
	CCPoint screenOrigin = pDirector->getVisibleOrigin(); 
	CCSize screenSize = pDirector->getVisibleSize();
	_winSize = Multiresolution::getInnerSize();
	//_winSize.height *= 2.0 / 3;
	//_winSize.width *= 2.0 / 3;

	float scale = screenSize.height / _winSize.height;

	_sceneForegound = CBgTextureNode::create();
	_sceneForegound->retain();

	string str;
	for(int idx = 0; idx < MAP_RES_NUM; idx++){
		str = m_stForeStr + "/bgfm" + to_string(idx) + PNG_TYPE_NAME;
		_sceneForegound->AddSubTextureName(str);
	}

	_sceneForegound->ComposeTexture();
	_sceneForegound->setAnchorPoint(ccp(0,0));

	if(m_stBackStr != "null"){  //包含中景层
		_sceneBackground = CBgTextureNode::create();
		_sceneBackground->retain();
		for(int idx = 0; idx < MAP_RES_NUM; idx++){
			string str = m_stBackStr + "/bgfm" + to_string(idx) + PNG_TYPE_NAME;
			_sceneBackground->AddSubTextureName(str, true);
		}

		_sceneBackground->ComposeTexture();
		_sceneBackground->setAnchorPoint(ccp(0,0));
		_sceneBackground->setPosition(ccp(0, _winSize.height));
	}

	if(m_stMidStr != "null"){  //包含中景层
		_sceneMidground = CBgTextureNode::create();
		_sceneMidground->retain();
		for(int idx = 0; idx < MAP_RES_NUM; idx++){

			string str = m_stMidStr + "/bgfm" + to_string(idx) + PNG_TYPE_NAME;
			_sceneMidground->AddSubTextureName(str, true);
		}

		_sceneMidground->ComposeTexture();
		_sceneMidground->setAnchorPoint(ccp(0,0));
		_sceneMidground->setPosition(ccp(0, _winSize.height - m_fMidPos));
	}

	if(m_stMid2Str != "null"){  //包含中景层
		_sceneMid2ground = CBgTextureNode::create();
		_sceneMid2ground->retain();
		for(int idx = 0; idx < MAP_RES_NUM; idx++){

			string str = m_stMid2Str + "/bgfm" + to_string(idx) + PNG_TYPE_NAME;
			_sceneMid2ground->AddSubTextureName(str, true);
		}

		_sceneMid2ground->ComposeTexture();
		_sceneMid2ground->setAnchorPoint(ccp(0,0));
		_sceneMid2ground->setPosition(ccp(0, _winSize.height - m_fMidPos));
	}

	_sceneFrontgound = CBgTextureNode::create();
	_sceneFrontgound->retain();
	for(int idx = 0; idx < MAP_RES_NUM; idx++){
		string str = m_stFrontStr + "/bgfm" + to_string(idx) + PNG_TYPE_NAME;
		_sceneFrontgound->AddSubTextureName(str);
	}

	_sceneFrontgound->ComposeTexture();
	_sceneFrontgound->setAnchorPoint(ccp(0,0));

	m_pGameLayer = dynamic_cast<CCLayer*>(getLayer(LAYER_ID_GAME));
	m_pGameLayer->removeAllChildren();
	m_pGameLayer->addChild(_sceneFrontgound, -2);
	m_pGameLayer->addChild(_sceneForegound, -4);

	if(_sceneMidground){
		m_pGameLayer->addChild(_sceneMidground, -6);
	}

	if(_sceneMid2ground){
		m_pGameLayer->addChild(_sceneMid2ground, -8);
	}

	if(_sceneBackground){
		m_pGameLayer->addChild(_sceneBackground, -10);
	}

	m_pGameLayer->setAnchorPoint(ccp(0, 0));
	m_pGameLayer->setPosition(screenOrigin);
	m_pGameLayer->setScale(scale);
}

void CBaseFightScene::buildMap(){
	//init data
	initFightData();

	//场景item
	initSceneItem();

	//地表buff
	initFloorBuff();

	//陷阱
	initTrap();

	//构建关卡UI
	initMapUI();

	//猪脚
	initMainHero();

	//友军
	initFriend();

	//monster
	initMonster();
	
	m_pArrLayout->initOwnerHero(_mainHero);
}

void CBaseFightScene::initTrap(){

}

void CBaseFightScene::initFloorBuff(){

}

void CBaseFightScene::initSceneItem(){

}

void CBaseFightScene::initMapUI(){
	//摇杆层
	_hudLayer = dynamic_cast<HudLayer*>(getLayer(LAYER_ID_HUD));
	_hudLayer->setDelegate(this);

	if(_hudLayer != NULL){
		if(_hudLayer->active){
			_hudLayer->Inactive();
			_hudLayer->Active();
		}else{
			_hudLayer->Active();
		}
	}

	//UI层
	m_pUiLayer = dynamic_cast<UILayer*>(getLayer(LAYER_ID_FIGHT));
	m_pUiLayer->clear();

	//场景小物件层
	m_pWidgeLayer = dynamic_cast<UILayer*>(getLayer(LAYER_ID_WIDGET));
	m_pWidgeLayer->clear();

	//按钮
	CCSize stSize = CCDirector::sharedDirector()->getVisibleSize(); 
	CCPoint visibleOrigin = CCDirector::sharedDirector()->getVisibleOrigin();
	m_pHeroHeadUI = CFightHeroHeadUI::create();
	m_pUiLayer->addWidget(m_pHeroHeadUI);
	m_pHeroHeadUI->setPosition(ccp(visibleOrigin.x, stSize.height - m_pHeroHeadUI->getSize().height - 10));

	CFightMedicalLayout* pMedicalLayout = P_FIGHT_CTRL->getMedicalLayout();
	if(pMedicalLayout->getParent() != NULL){
		pMedicalLayout->removeFromParent();
	}
	pMedicalLayout->setPosition(visibleOrigin + ccp(10, 285));
	m_pUiLayer->addWidget(pMedicalLayout);
	pMedicalLayout->setEnabled(true);
	pMedicalLayout->updateUI();
	Button* pMedicalBtn = pMedicalLayout->getMedicalBtn();
	pMedicalBtn->addTouchEventListener(this, toucheventselector(CBaseFightScene::btnTouchEvent));

	m_pBossHPUI = CFightBossHeadUI::create();
	m_pBossHPUI->setBossType(ROLE_TYPE_SENIOR_BOSS, "");
	m_pBossHPUI->setPosition(visibleOrigin + ccp(10, 10));
	m_pBossHPUI->setVisible(false);
	m_pBossHPUI->setAllChildOpacity(200);
	m_pWidgeLayer->addWidget(m_pBossHPUI);

	m_pPauseLayout = dynamic_cast<UILayout*>(GUIReader::shareReader()->widgetFromJsonFile("ui_v2/fight_pause_ui.ExportJson"));
	m_pPauseLayout->setPosition(visibleOrigin + ccp(stSize.width - m_pPauseLayout->getContentSize().width, stSize.height - m_pPauseLayout->getSize().height - 10));
	m_pUiLayer->addWidget(m_pPauseLayout);

	m_pArrLayout = CFightWeaponArrLayout::create(dynamic_cast<Layout*>(m_pPauseLayout->getChildByName("weapon_arr_layout")));
	m_pArrLayout->AddEventListener(this, CFightWeaponArrLayout::HERO_CHANGE_WEAPON_EVENT, eventListner_selector(CBaseFightScene::handleHeroWeaponChange));
	m_pArrLayout->retain();

	pauseBtn = dynamic_cast<UIButton*>(m_pPauseLayout->getChildByName("pause_btn"));

	m_pSkillLayout = dynamic_cast<UILayout*>(GUIReader::shareReader()->widgetFromJsonFile("ui_v2/fight_skill_btn_ui.ExportJson"));
	m_pSkillLayout->setPosition(visibleOrigin + ccp(stSize.width - m_pSkillLayout->getContentSize().width, 20));
	m_pSkillLayout->setTouchEnabled(true);
	m_pUiLayer->addWidget(m_pSkillLayout);

	m_pAutoBtn = dynamic_cast<UIButton*>(m_pSkillLayout->getChildByName("auto_btn"));
	m_pAutoBtn->setTouchEnabled(false);
	m_pAutoBtn->setVisible(false);
	m_pAutoBtn->addTouchEventListener(this, toucheventselector(CBaseFightScene::autoBtnEvent));

	m_pChangeWeaponBtn = dynamic_cast<UIButton*>(m_pSkillLayout->getChildByName("change_weapon_btn"));
	m_pChangeWeaponBtn->addTouchEventListener(this, toucheventselector(CBaseFightScene::btnTouchEvent));

	m_pHitLayout = CFightHitLayout::create();
	m_pHitLayout->retain();
	float hitX = stSize.width - m_pHitLayout->getContentSize().width - 60;
	float hitY = m_pPauseLayout->getPositionY() - m_pHitLayout->getSize().height;
	m_pHitLayout->AddEventListener(this, CFightHitLayout::HIT_OUT_TIME_EVENT, eventListner_selector(CBaseFightScene::handleHitTimeOutEvent));
	m_pHitLayout->setPosition(visibleOrigin + ccp(hitX, hitY));

	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("ui_v2/fight.plist");
	UIButton* btn = dynamic_cast<UIButton*>(m_pSkillLayout->getChildByName("fire_btn"));
	CCPoint stBtnPos = btn->getPosition();
	int btnZorder = btn->getZOrder();
	btn->removeFromParentAndCleanup(true);
	fireBtn = FireButton::create();
	fireBtn->setName("fire_button");
	fireBtn->loadTextures("fight/fight_shot_btn_normal.png", "fight/fight_shot_btn_over.png", "", UI_TEX_TYPE_PLIST);
	fireBtn->setPosition(stBtnPos);
	m_pSkillLayout->addChild(fireBtn, btnZorder);
	fireBtn->setTouchPassedEnable(true);
	
	pauseBtn->addTouchEventListener(this, toucheventselector(CBaseFightScene::btnTouchEvent));
	fireBtn->addTouchEventListener(this, toucheventselector(CBaseFightScene::btnTouchEvent));

	const UserInfo* pUserInfo = P_USER_OBJ->getUserInfo();	//玩家信息

	//枪械节能
	int skillID = (int)WEAPON_CFG_MASTER->GetWeaponCfgByID(pUserInfo->weapon.cfgid())->weaponsuperskill();
	int cdTime = (int)SKILL_CFG_MASTER->GetSkillCfgByID(skillID)->cd();
	m_trickBtn = SkillButton::create(dynamic_cast<UILayout*>(m_pSkillLayout->getChildByName("trick_layout")), 0, cdTime, skillID);
	m_trickBtn->AddEventListener(this, SkillButton::SKILL_BTN_CLICK_EVENT, eventListner_selector(CBaseFightScene::handleSkillClickEvent));

	//进化技能
	skillID = pUserInfo->evolutionSkillID;
	cdTime = (int)SKILL_CFG_MASTER->GetSkillCfgByID(skillID)->cd();
	m_skillBtn = SkillButton::create(dynamic_cast<UILayout*>(m_pSkillLayout->getChildByName("skill_layout")), 1, cdTime, skillID);
	m_skillBtn->AddEventListener(this, SkillButton::SKILL_BTN_CLICK_EVENT, eventListner_selector(CBaseFightScene::handleSkillClickEvent));

	//闪避技能
	if(QEnvSet::beck()){
		cdTime = 3;
	}else{
		cdTime = (int)ceil(pUserInfo->dodgeCD/1000);
	}
	m_dodgeBtn = SkillButton::create(dynamic_cast<UILayout*>(m_pSkillLayout->getChildByName("dodge_layout")), 2, cdTime, 0);
	m_dodgeBtn->AddEventListener(this, SkillButton::SKILL_BTN_CLICK_EVENT, eventListner_selector(CBaseFightScene::handleSkillClickEvent));

	//强制退出战斗
	CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(CBaseFightScene::handleGiveUpLogic), GIVEUP_FIGHT_EVENT, NULL);
}

void CBaseFightScene::initMainHero(){
    CCPoint stPlayerPt(300, m_uWalkHight/2);

	const UserInfo* pUserInfo = P_USER_OBJ->getUserInfo();	//玩家信息
    const string* pHeroStr = HERO_CFG_MASTER->GetResName(pUserInfo->showID);
    if(NULL == pHeroStr) {
        CCLOG("[error]CBaseFightScene::initMainHero, get hero res name fail! cfg id %d", pUserInfo->showID );
        return;
    }
	const string& heroStr = *pHeroStr;

	_mainHero = CMainHero::create();
    //_mainHero->autorelease();
	
	string stBodyName;
	int iRet = CMainHero::GetResourceNames(pUserInfo->weapon.cfgid(), pUserInfo->showID, stBodyName);
	if (iRet){
		CCLog("no resource name is found for hero %s", pHeroStr->c_str());
		return;
	}

	_mainHero->setShowID(pUserInfo->showID);
	_mainHero->setResPath(stBodyName);
	_mainHero->SetEnityPosition(stPlayerPt);
	_mainHero->setDesiredPosition(_mainHero->getPosition());
	_mainHero->setHurtActionCD(HERO_CFG_MASTER->GetHeroCfgByID(P_USER_OBJ->getUserInfo()->showID)->stifftime());

	CQStageEntityObj* heroInfo = CQStageEntityObj::create();
	heroInfo->setInfoByUserInfo(pUserInfo);
	_mainHero->setObj(heroInfo);
	_mainHero->setIsBulletLimit(true);		//弹药限制

	_mainHero->AddEventListener(this, CQStageEntity::EXCLUSIVE_ONE_BEGIN_FRAME, eventListner_selector(CBaseFightScene::handleHeroTheOneBeginEvent));
	_mainHero->AddEventListener(this, CQStageEntity::EXCLUSIVE_ONE_END_FRAME, eventListner_selector(CBaseFightScene::handleHeroTheOneEndEvent));
	_mainHero->AddEventListener(this, CQStageEntity::SKILL_CAST_FRAME, eventListner_selector(CBaseFightScene::handleSkillKeyFrameEvent));
	_mainHero->AddEventListener(this, CQStageEntity::FIRE_EVENT, eventListner_selector(CBaseFightScene::handleFireABulletEvent));
	_mainHero->AddEventListener(this, CQStageEntity::SKILL_CAST_IN_ACTION, eventListner_selector(CBaseFightScene::handleSkillBtnStateEvent));
	_mainHero->AddEventListener(this, CQStageEntity::HERO_DASH_COMPLETE_EVENT, eventListner_selector(CBaseFightScene::handleHeroDashCompleteEvent));
	_mainHero->AddEventListener(this, CQStageEntity::KICK_EVENT, eventListner_selector(CBaseFightScene::handleKickEvent));
	_mainHero->AddEventListener(this, CQStageEntity::WIN_COMPLETE_EVENT, eventListner_selector(CBaseFightScene::hanldeWinCompleteFun));
	_mainHero->AddEventListener(this, CMainHero::TRY_KICK_ENIMY_EVENT, eventListner_selector(CBaseFightScene::handleTryKickEvent));
	_mainHero->AddEventListener(this, CMainHero::HERO_HP_CHANGE_EVENT, eventListner_selector(CBaseFightScene::handleHeroHPChangeEvent));
	_mainHero->AddEventListener(this, CMainHero::HERO_EMERGE_COMPLETE_EVENT, eventListner_selector(CBaseFightScene::handleHeroEmergeCompleteEvent));
	_mainHero->AddEventListener(this, CMainHero::HERO_CAST_SKILL_EVENT, eventListner_selector(CBaseFightScene::handleHeroSkillCastEvent));

	_mainHero->AddEventListener(this, HERO_LOSEVALUE_CHANGE, eventListner_selector(CBaseFightScene::handleHeroHurtValueEvent));
	_mainHero->AddEventListener(this, HERO_KILLED_EVENT, eventListner_selector(CBaseFightScene::handleHeroKilledEvent));
	_mainHero->AddEventListener(this, HERO_EPVALUE_COST_EVENT, eventListner_selector(CBaseFightScene::handleHeroEPCostEvent));

	//枪械上的技能
	CQSkillCell* pstSkill = CQSkillCell::create();	
	pstSkill->m_uSkillID = WEAPON_CFG_MASTER->GetWeaponCfgByID(heroInfo->weapon.cfgid())->weaponsuperskill();	
	pstSkill->m_uLastCastTime = 0;	
	_mainHero->getSkillArray()->addObject(pstSkill);

	CQSkillCell* pstSkill2 = CQSkillCell::create();	
	pstSkill2->m_uSkillID = heroInfo->evolutionSkillID;	
	pstSkill2->m_uLastCastTime = 0;	
	_mainHero->getSkillArray()->addObject(pstSkill2);

	_mainHero->LoadActionRes();
	_mainHero->setLevel(pUserInfo->level);
	int iNewZorder = CInstanceUtils::GetZOrderByType(ROLE_Z_ORDER, _mainHero->getPositionY());
	_sceneForegound->addChild(_mainHero, iNewZorder);
	_stageEntities->addObject(_mainHero);

	//更新头像上的血条，能量值
	m_pHeroHeadUI->setHPPercent(_mainHero->getHP(), _mainHero->getMaxHP());
	m_pHeroHeadUI->setEPPercent(_mainHero->getEP(), _mainHero->getMaxEP());

	//玩家自己,添加换枪消息
	//CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(CBaseFightScene::handleHeroWeaponChange), 
	//																HERO_WEAPON_CHANGE_EVENT, NULL);

	CCArmature* pArm = CCArmature::create("ut00091");
	_mainHero->addChild(pArm, -1);
	pArm->getAnimation()->play("fly", -1, -1, 1);
}

void CBaseFightScene::initFriend(){

}

void CBaseFightScene::initMonster(){

}

CEnemy* CBaseFightScene::initOneMonster(CEnemyData* monsterObj){
	
	MonsterCfg* monsterCfg = MONSTER_CFG_MASTER->GetMonsterCfgByID(monsterObj->id);
	if(monsterObj->type != APPEAR_AFTER_DIE){   //死亡后不停刷怪机制输出的怪血量伤害不计入总伤害
		m_uTotalMonsterHp += monsterCfg->hp();  
	}

	CEnemy* pstMonster = CEnemy::create();
	pstMonster->setNameStr(monsterCfg->monstername());
	pstMonster->setResPath(monsterCfg->avatar());
	pstMonster->setMonsterObj(monsterObj);
	pstMonster->setIsNeedPath(monsterCfg->walktype() == 1);
	pstMonster->setPatrolArea(monsterCfg->patrolarea());
	pstMonster->setAIType(monsterCfg->aiid());
	pstMonster->setHeadRes(monsterCfg->headimg());

	CQStageEntityObj* monsterInfo = CQStageEntityObj::create();
	monsterInfo->setInfoByMonsterCfg(monsterCfg);
	pstMonster->setObj(monsterInfo);

	pstMonster->SetEnityPosition(ccp(monsterObj->x, _sceneForegound->getContentSize().height - monsterObj->y));
	pstMonster->setDesiredPosition(ccp(monsterObj->x, _sceneForegound->getContentSize().height - monsterObj->y ));
	pstMonster->setPosition(ccp(monsterObj->x, _sceneForegound->getContentSize().height - monsterObj->y));
	pstMonster->setHurtActionCD(monsterCfg->hurtactioncd());
	pstMonster->setLevel(monsterCfg->level());

	for(int j = 0; j < monsterCfg->skills_size(); j++){
		CQSkillCell* pstSkill = CQSkillCell::create();	
		pstSkill->m_uSkillID = monsterCfg->skills(j);
		pstSkill->m_uLastCastTime = 0.1f;
		pstMonster->getSkillArray()->addObject(pstSkill);
	}

	pstMonster->AddEventListener(this, CQStageEntity::ATK_ACTION_KEY_FRAME, eventListner_selector(CBaseFightScene::handleCloseAtkKeyFrameEvent));
	pstMonster->AddEventListener(this, CQStageEntity::SKILL_CAST_FRAME, eventListner_selector(CBaseFightScene::handleSkillKeyFrameEvent));
	pstMonster->AddEventListener(this, CQStageEntity::FIRE_EVENT, eventListner_selector(CBaseFightScene::handleFireABulletEvent));
	pstMonster->AddEventListener(this, CQStageEntity::DIE_EFF_FRAME, eventListner_selector(CBaseFightScene::handleDieEffKeyFrameEvent));
	pstMonster->AddEventListener(this, CQStageEntity::SKILL_CAST_IN_ACTION, eventListner_selector(CBaseFightScene::handleEnemySkillActionEvent));
	pstMonster->AddEventListener(this, CQStageEntity::DIE_MOMENT_EVENT, eventListner_selector(CBaseFightScene::handleEnemyDieMovementEvent));
	pstMonster->AddEventListener(this, CQStageEntity::DIE_COMPLETE_EVENT, eventListner_selector(CBaseFightScene::handleEnemyDieCompleteEvent));
	pstMonster->AddEventListener(this, CQStageEntity::DASH_COMPLETE_EVENT, eventListner_selector(CBaseFightScene::handleEnemyDashCompleteEvent));
	pstMonster->AddEventListener(this, CQStageEntity::DIE_EFF_SOUND_EVENT, eventListner_selector(CBaseFightScene::handleEnemyDieEffectEvent));
	pstMonster->AddEventListener(this, CQStageEntity::ACTIVE_COMPLETE_EVENT, eventListner_selector(CBaseFightScene::activeEnemyEvent));
	pstMonster->AddEventListener(this, CQStageEntity::ADD_ANI_EFF_EVENT, eventListner_selector(CBaseFightScene::handleAniEffCountEvent));

	pstMonster->AddEventListener(this, ENEMY_HURT_EVENT, eventListner_selector(CBaseFightScene::handleEnemyHurtEvent));
	pstMonster->AddEventListener(this, HERO_HARMVALUE_CHANGE, eventListner_selector(CBaseFightScene::handleEnemyHarmValueEvent));
	pstMonster->AddEventListener(this, HERO_EPVALUE_ADD_EVENT, eventListner_selector(CBaseFightScene::handleHeroEPAddEvent));

	if(monsterObj->type != APPEAR_CAR){   //普通类型的刷怪直接放入舞台的数组, //雕像可被攻击，攻击后激活
		_stageEntities->addObject((CCObject*)pstMonster);
	}

	return pstMonster;
}

CEnemy* CBaseFightScene::initOneMonster(CEnemyData* monsterObj, MonsterCfg* monsterCfg){
	if(monsterObj->type != APPEAR_AFTER_DIE){   //死亡后不停刷怪机制输出的怪血量伤害不计入总伤害
		m_uTotalMonsterHp += monsterCfg->hp();  
	}

	CEnemy* pstMonster = CEnemy::create();
	pstMonster->setDesiredPosition(_mainHero->getPosition());
	pstMonster->setNameStr(monsterCfg->monstername());
	pstMonster->setResPath(monsterCfg->avatar());
	pstMonster->setMonsterObj(monsterObj);
	pstMonster->setIsNeedPath(monsterCfg->walktype() == 1);
	pstMonster->setAIType(monsterCfg->aiid());
	pstMonster->setHeadRes(monsterCfg->headimg());
	pstMonster->setPatrolArea(monsterCfg->patrolarea());

	CQStageEntityObj* monsterInfo = CQStageEntityObj::create();
	monsterInfo->setInfoByMonsterCfg(monsterCfg);
	pstMonster->setObj(monsterInfo);

	pstMonster->setCenterToBottom(55);
	pstMonster->setCenterToSides(55);
	pstMonster->SetEnityPosition(ccp(monsterObj->x, _sceneForegound->getContentSize().height - monsterObj->y));
	pstMonster->setDesiredPosition(ccp(monsterObj->x, _sceneForegound->getContentSize().height - monsterObj->y ));
	pstMonster->setPosition(ccp(monsterObj->x, _sceneForegound->getContentSize().height - monsterObj->y));
	pstMonster->setHurtActionCD(monsterCfg->hurtactioncd());
	pstMonster->setLevel(monsterCfg->level());

	for(int j = 0; j < monsterCfg->skills_size(); j++){
		CQSkillCell* pstSkill = CQSkillCell::create();	
		pstSkill->m_uSkillID = monsterCfg->skills(j);
		pstMonster->getSkillArray()->addObject(pstSkill);
	}

	pstMonster->AddEventListener(this, CQStageEntity::ATK_ACTION_KEY_FRAME, eventListner_selector(CBaseFightScene::handleCloseAtkKeyFrameEvent));
	pstMonster->AddEventListener(this, CQStageEntity::SKILL_CAST_FRAME, eventListner_selector(CBaseFightScene::handleSkillKeyFrameEvent));
	pstMonster->AddEventListener(this, CQStageEntity::FIRE_EVENT, eventListner_selector(CBaseFightScene::handleFireABulletEvent));
	pstMonster->AddEventListener(this, CQStageEntity::DIE_EFF_FRAME, eventListner_selector(CBaseFightScene::handleDieEffKeyFrameEvent));
	pstMonster->AddEventListener(this, CQStageEntity::SKILL_CAST_IN_ACTION, eventListner_selector(CBaseFightScene::handleEnemySkillActionEvent));
	pstMonster->AddEventListener(this, CQStageEntity::DIE_MOMENT_EVENT, eventListner_selector(CBaseFightScene::handleEnemyDieMovementEvent));
	pstMonster->AddEventListener(this, CQStageEntity::DIE_COMPLETE_EVENT, eventListner_selector(CBaseFightScene::handleEnemyDieCompleteEvent));
	pstMonster->AddEventListener(this, CQStageEntity::DASH_COMPLETE_EVENT, eventListner_selector(CBaseFightScene::handleEnemyDashCompleteEvent));
	pstMonster->AddEventListener(this, CQStageEntity::DIE_EFF_SOUND_EVENT, eventListner_selector(CBaseFightScene::handleEnemyDieEffectEvent));
	pstMonster->AddEventListener(this, CQStageEntity::ACTIVE_COMPLETE_EVENT, eventListner_selector(CBaseFightScene::activeEnemyEvent));
	pstMonster->AddEventListener(this, CQStageEntity::ADD_ANI_EFF_EVENT, eventListner_selector(CBaseFightScene::handleAniEffCountEvent));

	pstMonster->AddEventListener(this, ENEMY_HURT_EVENT, eventListner_selector(CBaseFightScene::handleEnemyHurtEvent));
	pstMonster->AddEventListener(this, HERO_HARMVALUE_CHANGE, eventListner_selector(CBaseFightScene::handleEnemyHarmValueEvent));
	pstMonster->AddEventListener(this, HERO_EPVALUE_ADD_EVENT, eventListner_selector(CBaseFightScene::handleHeroEPAddEvent));

	if(monsterObj->type != APPEAR_CAR){   //普通类型的刷怪直接放入舞台的数组； //雕像可被攻击，攻击后激活
		_stageEntities->addObject((CCObject*)pstMonster);
	}

	return pstMonster;
}

void CBaseFightScene::initFightData(){
	m_bDisturbShoot = false;
	m_bShootAfterSkill = false;
	m_bIsInTheOneState = false;
	m_bIsGameOver = false;
	m_bIsPreOver = false;
	m_bIsPlayKillEff = true;
	m_bIsAddCoinEff = true;
	m_bIsAniEffOn = false;
	m_bIsMainHeroShooting = false;
	m_bIsShowBulletOut = false;
	m_bIsGamePause = false;
	m_bIsWalkNextStage = false;
	m_uTheOneBeginSec = 0;
	m_uTotalMonsterHp = 0;
	m_uTotalHarmValue = 0;
	m_uTotalEnemyKilled = 0;
	m_uTotalLoseValue = 0;
	m_uMaxKillValue = 0;
	m_uCurKillValue = 0;
	m_uKillTime = 0;
	m_uMark = 0;
	m_uScore = 0;
	m_uScorePercent = 0;
	m_uBossID = 0;
	m_uWarLevel = 0;
	m_uLimiteTime = 0;
	m_uEvolutionCnt = 0;
	m_uWinType = 0;
	m_uWinValue1 = 0;
	m_uWinValue2 = 0;
	m_uDropCoin = 0;
	m_uCurAniTotalNum = 0;
	m_uBulletSkillBuffNum = 0;
	m_uUseMedicalCnt = 0;
	m_uUseReliveCnt = 0;
	m_uUseBulletCnt = 0;
}

void CBaseFightScene::startFight(){
	if(_mainHero->getMyState() == kActionStateEmerge){  //主角有出场动作
		setAttackOpBtnVisible(false);
	}else{
		startUpdate();
	}
}

void CBaseFightScene::startUpdate(){
	setAttackOpBtnVisible(true);
	this->scheduleUpdate();

	this->schedule(schedule_selector(CBaseFightScene::updateCoinPos), 0.02f);
}

void CBaseFightScene::update(float delta){
	m_fFightTime += delta;

	//实时释放资源
	CBaseMapScene::update(delta);

	//刷怪逻辑
	brushMonsterLogic(delta);

	//猪脚位置更新
	updateMainHeroPosition();

	//技能
	updateSkillEffPos();

	//地表buff更新
	updateSceneBuffs(delta);

	//陷阱
	updateSceneTraps(delta);

	//更新子弹相关信息，主要是计算伤害，弹道自己会计算，这个要放在怪物之前，主要是为了处理哪些追踪型弹道，以免其target是野地址
	updateBullets(delta);

	//AI
	updateInstnaceByAI();

	//过关逻辑
	handlePassLogic();

	//检查放技能的时间
	checkTheOneState();

	//按钮更新
	updateBtnState();

	//怪物指引箭头更新
	updateMonsterArrow();		

	//更新舞台上移动，计算伤害
	updateMoveEntities();

	//更新当前怪物身上特效总数
	updateAniEffNum(delta);

	//更新主角的弹药
	updateMainHeroBulletTime(delta);
}

void CBaseFightScene::updateMainHeroBulletTime(float dt){
	if(!m_pArrLayout->isEnabled() || !_mainHero->getIsBulletLimit() || _mainHero->getBulletTime() <= 0){  //没有弹药限制 or 没有弹药了
		if(_mainHero->getBulletTime() <= 0){    //弹药没了，检测是否所有武器都没弹药
			checkAllWeaponOutBullet();
		}
		return;
	}

	if(_mainHero->isShooting()){
		float preTime = _mainHero->getBulletTime();
		float nowTime = (preTime - dt < 0)?0:(preTime - dt);
		_mainHero->setBulletTime(nowTime);

		//更新面板
		m_pArrLayout->updateCurCell(_mainHero->getBulletTime());
	}
}

void CBaseFightScene::autoChangeNextWeapon(){
	if(m_bIsPreOver){
		return;
	}

	if(!_mainHero->IsActionInterruptable()){   //动作不可打断
		return;
	}

	m_pArrLayout->autoChangeNextWeapon();
}

bool CBaseFightScene::isAllWeaponOutBullet(){
	return m_pArrLayout->IsAllWeaponOutBullet();
}

void CBaseFightScene::checkAllWeaponOutBullet(){
	if(m_pArrLayout->IsAllWeaponOutBullet() && !P_LEVEL_CTL->getFillBulletPanel()->isOpen()){
		doAllBulletOut();
	}
}

void CBaseFightScene::updateAniEffNum(float dt){
	if(m_fFightTime - m_fLastAniTime < m_fAniInter){  //在一定时间内特效总数
		if(m_uCurAniTotalNum < (int)m_uAniMaxNum){
			m_bIsAniEffOn = true;
		}else{
			m_bIsAniEffOn = false;
		}
	}else{
		m_bIsAniEffOn = true; 
		m_uCurAniTotalNum = 0;
		m_uBulletSkillBuffNum = 0;
		m_fLastAniTime = m_fFightTime;
	}
}

void CBaseFightScene::brushMonsterLogic(float dt){

}

void CBaseFightScene::handlePassLogic(){

}

void CBaseFightScene::handleGiveUpLogic(CCObject* pstObj){

}

void CBaseFightScene::updateMainHeroPosition(){
	float posX;
	float posY;
	bool bReachEdge = false;
	if(_mainHero->getDesiredPosition().x - _mainHero->getCenterToSides() <= walkableRect.getMinX()){
		posX = walkableRect.getMinX() + _mainHero->getCenterToSides();
		bReachEdge = true;
	}else if(_mainHero->getDesiredPosition().x + _mainHero->getCenterToSides() >= walkableRect.getMaxX()){
		posX = walkableRect.getMaxX() - _mainHero->getCenterToSides();
		bReachEdge = true;
	}else{
		posX = _mainHero->getDesiredPosition().x;
	}

	if(_mainHero->getDesiredPosition().y <= walkableRect.getMinY()){
		posY = walkableRect.getMinY();
		//bReachEdge = true;
	}else if(_mainHero->getDesiredPosition().y >= walkableRect.getMaxY()){
		posY = walkableRect.getMaxY();
		//bReachEdge = true;
	}else{
		posY = _mainHero->getDesiredPosition().y;
	}

	float realX = posX;
	float realY = posY;
	if(_mainHero->getVelocity().x > 0){
		realX = posX + getGridW();
	}else if(_mainHero->getVelocity().x < 0){
		realX = posX - getGridW();
	}else{
		realX = posX;
	}

	if(_mainHero->getVelocity().y > 0){
		realY = posY + getGridH();
	}else if(_mainHero->getVelocity().y < 0){
		realY = posY - getGridH();
	}else{
		realY = posY;
	}

	float tempX = posX;
	float tempY = posY;
	
	CCRect rect = getPointInRect(ccp(realX, realY));
	if(rect.size.equals(CCSizeZero) == false){
		if(_mainHero->getVelocity().x > 0){  //前进
			if(realY >= rect.getMinY() && realY <= rect.getMaxY()){
				if(posX < rect.getMinX() && realX >= rect.getMinX()){
					tempX = rect.getMinX() - getGridW();
					bReachEdge = true;
				}
			}
		}else if(_mainHero->getVelocity().x < 0){
			if(realY >= rect.getMinY() && realY <= rect.getMaxY()){
				if(posX > rect.getMaxX()  && realX <= rect.getMaxX()){
					tempX = rect.getMaxX() + getGridW();
					bReachEdge = true;
				}
			}
		}

		if(_mainHero->getVelocity().y > 0){  //向上
			if(realX >= rect.getMinX() && realX <= rect.getMaxX()){
				if(realY >= rect.getMinY() && posY < rect.getMinY()){
					tempY = rect.getMinY() - getGridH();
					bReachEdge = true;
				}
			}
		}else if(_mainHero->getVelocity().y < 0){
			if(realX >= rect.getMinX() && realX <= rect.getMaxX()){
				if(realY <= rect.getMaxY() && posY > rect.getMaxY()){
					tempY = rect.getMaxY() + getGridH();
					bReachEdge = true;
				}
			} 
		}
	}

	posX = tempX;
	posY = tempY;
	_mainHero->SetEnityPosition(ccp(posX, posY));
	if (bReachEdge && _mainHero->getMyState() == kActionStateBackDash){
		
		//_mainHero->Idle();
	}

	if (!isPointCanWalk(_mainHero->getPosition()) && !_mainHero->IsDead()){
		_mainHero->RollBackPosition(false);
	}
}

void CBaseFightScene::updateInstnaceByAI(){
	CInstanceAI::UpdateInstanceByAI(this);

	CQStageEntity* pstMonster;
	CCObject* pstObj;
	CCARRAY_FOREACH(_stageEntities, pstObj){
		pstMonster = dynamic_cast<CQStageEntity*>(pstObj);
		if(pstMonster->GetIsControlledByAI()){
			if (!pstMonster->getIsBlock() && !isPointCanWalk(pstMonster->getPosition()) && !pstMonster->IsDead()){
				pstMonster->RollBackPosition();
			}

			//检测冲撞是否出屏
			if(!walkableRect.containsPoint(pstMonster->getPosition()) && !pstMonster->IsDead() && pstMonster->getIsCheckOutSize()){
				pstMonster->CheckRollBack(walkableRect);
			}
		}
	}
}

void CBaseFightScene::updateSkillEffPos(){
	CSkillEffLayer* pstSkillEff;
	//处理自己方的特效
	for (unsigned int iIdx = 0; iIdx < this->getSkills()->count(); iIdx++){
		pstSkillEff = (CSkillEffLayer*)this->getSkills()->objectAtIndex(iIdx);
		//检查这个技能是否已经飞出目标区域
		CCPoint stWorldPos = ccpAdd(pstSkillEff->getPosition(), getSceneForeground()->getPosition());
		if ((stWorldPos.x + pstSkillEff->getContentSize().width / 2 < 0 || stWorldPos.x - pstSkillEff->getContentSize().width / 2 > _winSize.width)
			&&(stWorldPos.y + pstSkillEff->getContentSize().height / 2 < 0 || stWorldPos.y - pstSkillEff->getContentSize().height / 2 > _winSize.height)){
				this->getSceneForeground()->removeChild(pstSkillEff, true);
				this->getSkills()->removeObjectAtIndex(iIdx);
				iIdx --;
				continue;
		}
        if(!pstSkillEff->getArmature() || pstSkillEff->getArmature()->getAnimation() == NULL) {
            CCLOG("-----------");
        }
		if (pstSkillEff->getParent() && pstSkillEff->getArmature()->getAnimation()->getIsComplete()){
			this->getSceneForeground()->removeChild(pstSkillEff, true);
			this->getSkills()->removeObjectAtIndex(iIdx);
			iIdx --;
			continue;
		}

		if (pstSkillEff->IsFollowType()
			&& pstSkillEff->IsChasing()){
				if((pstSkillEff->getTarget() && pstSkillEff->getTarget()->IsDead()) || !_stageEntities->containsObject(pstSkillEff->getTarget())){
					this->getSceneForeground()->removeChild(pstSkillEff, true);
					this->getSkills()->removeObjectAtIndex(iIdx);
					iIdx --;
					continue;
				}
		}

		if (pstSkillEff->IsCollideType() && pstSkillEff->getFlyState() == SKILL_ANIMATION_STATE_DONE
			|| pstSkillEff->IsFollowType() &&  pstSkillEff->getFlyState() == SKILL_ANIMATION_STATE_DONE){
			this->getSceneForeground()->removeChild(pstSkillEff, true);
			this->getSkills()->removeObjectAtIndex(iIdx);
			iIdx --;
			continue;
		}

		if (pstSkillEff->IsShouldClear()){
			pstSkillEff->removeFromParentAndCleanup(true);
			this->getSkills()->removeObjectAtIndex(iIdx);
			iIdx --;
			continue;
		}

		if (pstSkillEff->IsCollideType()){
			//检查是否有撞击发生,如果这是个攻击型技能，检查敌人,目前只有撞击类技能需要这样检测，范围攻击类技能特效在触发点瞬间判断			
			CInstanceUtils::SkillCollides(pstSkillEff, _stageEntities, this);			

		}else{
			//如果这是辅助型，检查自己人, 辅助型一般没有飞行特效吧。。。
		}		
		if (!_mainHero->getIsTheOne()){
			int iNewZorder = CInstanceUtils::GetZOrderByType(ROLE_Z_ORDER, pstSkillEff->getZalias());
			pstSkillEff->setZOrder(iNewZorder);
		}
		
	}
}

void CBaseFightScene::updateSceneBuffs(float dt){
	static float m_fBuffInter = 0;
 	if(m_fBuffInter > 2){   //2帧更新一次
 		m_fBuffInter = 0;
 		return;
 	}

	m_fBuffInter ++;

	CCObject* pObject;
	CSceneBuff* pstBuff;
	CCARRAY_FOREACH(_sceneBuffs, pObject){
		pstBuff = dynamic_cast<CSceneBuff*>(pObject);

		if(pstBuff->getIsDie()){  //buff已经在播放消失动画
			continue;
		}
		
		CCObject* pEntityObj;
		CCARRAY_FOREACH(_stageEntities, pEntityObj){
			bool isSelf = false;
			CQStageEntity* pstTarget = dynamic_cast<CQStageEntity*>(pEntityObj);
			if(pstTarget->m_uID == _mainHero->m_uID){
				isSelf = true;
			}

			if(CInstanceUtils::IsTargetType(pstBuff->getTargetType(), 
				pstBuff->getOwnerParty(),
				pstTarget->getRoleParty(), 
				isSelf)){

				if(CInstanceUtils::IsContoursCollide(pstBuff, pstTarget)){
					//在buff区域内
					unsigned int uBuffType = pstBuff->m_stCfg.bufftype();
					if(uBuffType == BUFF_TYPE_WEAPON){
						if(_mainHero->GetCurSkillCell() != NULL){  //主角正在放技能，不能换枪
							continue;
						}

						//更换枪械
						changeTempWeapon(pstBuff->m_stCfg.buffid());
						pstBuff->Die();
						continue;
					}else if(uBuffType == BUFF_TYPE_WEAPON_UP || uBuffType == BUFF_TYPE_WEAPON_MAX){  //升星、满星buff
						weaponUpStar(pstBuff->m_stCfg.bufftype());
						pstBuff->Die();
						continue;
					}

					if(BUFF_CFG_MASTER->GetBuffCfgByID(pstBuff->m_stCfg.buffid())->addordec() == BUFF_DEC_EFF && pstTarget->IsImmune()){
						continue;
					}

					if(!pstTarget->CheckHasBuff(pstBuff->getBuffID(), pstBuff->m_uID) && pstBuff->m_stCfg.bufftype() != BUFF_TYPE_WEAPON){
						pstTarget->AddBuff(pstBuff->getBuffID(), pstBuff->m_uID);
					}

					if(pstBuff->m_stCfg.bufftype() == BUFF_TYPE_ONCE){  //1次性的buff
						pstBuff->Die();
					}
				}else{
					//buff区域外，角色身上有buff就去掉
					if(pstBuff->m_stCfg.bufftype() != BUFF_TYPE_WEAPON && pstTarget->CheckHasBuff(pstBuff->getBuffID(), pstBuff->m_uID)){
						pstTarget->RemoveBuff(pstBuff->getBuffID(), pstBuff->m_uID);
					}
				}
			}

		}
	}
}

void CBaseFightScene::updateSceneTraps(float dt){
	static float m_fTrapInter = 0;
	if(m_fTrapInter > 2){   //2帧更新一次
		m_fTrapInter = 0;
		return;
	}

	m_fTrapInter ++;

	CCObject* pObject;
	CSceneTrap* pstTrap;
	CCARRAY_FOREACH(_sceneTraps, pObject){
		pstTrap = dynamic_cast<CSceneTrap*>(pObject);

		if(pstTrap->getState() == CSceneTrap::TRAP_STATE_ATTACKING || 
			pstTrap->getState() == CSceneTrap::TRAP_STATE_DIE
			|| pstTrap->m_stCfg.type() == TRAP_TYPE_PUNCTURE){		
			continue;
		}

		CCObject* pEntityObj;
		CCARRAY_FOREACH(_stageEntities, pEntityObj){
			bool isSelf = false;
			CQStageEntity* pstTarget = dynamic_cast<CQStageEntity*>(pEntityObj);
			if(pstTarget->m_uID == _mainHero->m_uID){
				isSelf = true;
			}

			if(CInstanceUtils::IsTargetType(pstTrap->getTargetType(), 
				pstTrap->getOwnerParty(),
				pstTarget->getRoleParty(), 
				isSelf)){
				if(CInstanceUtils::IsContoursCollide(pstTrap, pstTarget)){
					if(pstTrap->m_stCfg.type() == TRAP_TYPE_ONCE){
						pstTrap->Attack(true);
					}
				}else{
					pstTrap->resetDamageCnt();
				}
			}
		}
	}
}

void CBaseFightScene::updateBullets(float dt){
	static float dLastCheckTime = 0;
	dLastCheckTime += dt;
	if (dLastCheckTime <= 0.03){
		//做个流控，每秒钟只算30次，有必要的话还要进行间隔分开计算
        return;
	}
    dLastCheckTime = 0;
    
    //主角的子弹
    CCObject* pObject;
    CCARRAY_FOREACH(_hero_bullets, pObject){
        CBullet* pstBullet = dynamic_cast<CBullet*>(pObject);
		if (!pstBullet){
			continue;
		}

		if (pstBullet->getShouldClear()){
			pstBullet->removeFromParentAndCleanup(true);
			_hero_bullets->removeObject(pObject, true);
			continue;
		}

		if (pstBullet->getTrail() == TRA_CON_SPREAD){
			continue;
		}
        //重置子弹的打击距离为射击距离
        pstBullet->setHitDist(pstBullet->getShootDistance());
        CCObject* pEnimyObj;
        CQStageEntity* pstFinalTarget = NULL;
        float fNearestDist = 5000.0;
        CCARRAY_FOREACH(_stageEntities, pEnimyObj){
            CQStageEntity* pstTarget = dynamic_cast<CQStageEntity*>(pEnimyObj);	

			if(!checkPosInWalkable(pstTarget->getPosition())){  //当怪物在阶段外，不可被攻击
				continue;
			}

			unsigned int uRoleParty = ROLE_PARTY_HOSTILE;
			if(pstBullet->getParty() == ROLE_PARTY_FRIEND){
				uRoleParty = ROLE_PARTY_HOSTILE;
			}else if(pstBullet->getParty() == ROLE_PARTY_HOSTILE){
				uRoleParty = ROLE_PARTY_FRIEND;
			}

  
            if (pstBullet->canTargetBeAdded(pstTarget->m_uID) 
				&& CInstanceUtils::IsBulletHitEntity(pstBullet, uRoleParty,  pstTarget)){
                float fTempDist = ccpDistance(pstBullet->getPosition(), pstTarget->getPosition());
                if (pstBullet->getTrail() == TRA_CON_HORI_LINE || pstBullet->getTrail() == TRA_JTL_LINE){
					 CInstanceUtils::HandleBulletHitTargetEffect(pstBullet, pstTarget, this, dt);
                }else if (pstBullet->IsPenatrating()){						
                    //范围扩散型的，这个范围以内所有人都受伤，不会因为撞到人就退出
                    CInstanceUtils::HandleBulletHitTargetEffect(pstBullet, pstTarget, this, dt);
					pstBullet->setPenatrate(pstBullet->getPenatrate() + 1);

					pstBullet->AddTargetToPenatrate(pstTarget->m_uID);
				}
            }
        }

        if (pstFinalTarget){
            CInstanceUtils::HandleBulletHitTargetEffect(pstBullet, pstFinalTarget, this, dt);
            pstBullet->setHitDist(fNearestDist);
            pstBullet->setPenatrate(pstBullet->getPenatrate() + 1);
            if (!pstBullet->IsPenatrating()
				&& (pstBullet->getTrail() == TRA_ISO_FORWARD_TRACE || pstBullet->getTrail() == TRA_ISO_FULL_TRACE || pstBullet->getTrail() == TRA_ISO_HORI_LINE)){
                pstBullet->setShouldClear(true);					
            }
			CCLOG("target %p", pstFinalTarget);
        }else if (pstBullet->getTrail() == TRA_CON_HORI_LINE){
			float fHitDist = 0;
			CCPoint stWorldPos = getSceneForeground()->convertToWorldSpace(pstBullet->getPosition());
			if (pstBullet->getScaleX() > 0){
				fHitDist = CCDirector::sharedDirector()->getVisibleSize().width - stWorldPos.x;
			}else{
				fHitDist = stWorldPos.x;
			}
			pstBullet->setHitDist(fHitDist);
		}

		if (pstBullet->getShouldClear()){
			getSceneForeground()->removeChild(pstBullet);
			_hero_bullets->removeObject(pstBullet, true);
			continue;
		}

		CCObject* pstItemObj;
		CCARRAY_FOREACH(_sceneItems, pstItemObj){
			CSceneItem* pstItem = dynamic_cast<CSceneItem*>(pstItemObj);	
			if (!pstItem && !checkPosInWalkable(pstItem->getPosition())){
				continue;
			}
			if (CInstanceUtils::IsBulletHitItem(pstBullet, pstItem)){
				CInstanceUtils::HandleBulletHitItemEffect(pstBullet, pstItem, dt);
				pstBullet->setPenatrate(pstBullet->getPenatrate() + 1);
				if (!pstBullet->IsPenatrating()
					&& (pstBullet->getTrail() == TRA_ISO_FORWARD_TRACE || pstBullet->getTrail() == TRA_ISO_FULL_TRACE || pstBullet->getTrail() == TRA_ISO_HORI_LINE)){
					pstBullet->setShouldClear(true);	
					getSceneForeground()->removeChild(pstBullet);
					_hero_bullets->removeObject(pstBullet, true);
					break;
				}
			}
		}
    }
}

void CBaseFightScene::updateMonsterArrow(){
	CCSize stSize = CCDirector::sharedDirector()->getVisibleSize(); 
	CCPoint visibleOrigin = CCDirector::sharedDirector()->getVisibleOrigin();
	float visibleW = stSize.width;
	float rightX;
	if(_mainHero->getPositionX() - walkableRect.getMinX() <= visibleW/2){
		rightX = walkableRect.getMinX() + visibleW;
	}else{
		rightX = _mainHero->getPositionX() + visibleW/2;
	}

	float leftX;
	if(_mainHero->getPositionX() >= walkableRect.getMaxX() - visibleW/2){
		leftX = walkableRect.getMaxX() - visibleW;
	}else{
		leftX = _mainHero->getPositionX() - visibleW/2;
	}

	vector<CCPoint> stLeftPoints;
	vector<CCPoint> stRightPoints;
	stLeftPoints.clear();
	stRightPoints.clear();
	
	CCObject* pstObj;
	CCARRAY_FOREACH(_stageEntities, pstObj){
		CQStageEntity* pstEntity = dynamic_cast<CQStageEntity*>(pstObj);

		if(!checkArrowCondition(pstEntity)){
			continue;
		}

		if(pstEntity->getPositionX() - pstEntity->getCenterToSides() > rightX){
			stRightPoints.push_back(pstEntity->getPosition());
		}

		if(pstEntity->getPositionX() + pstEntity->getCenterToSides() < leftX){
			stLeftPoints.push_back(pstEntity->getPosition());
		}
	}
		
	unsigned int leftSize = stLeftPoints.size();
	unsigned int rightSize = stRightPoints.size();

	//删除多余的箭头
	while(leftSize < m_stLeftArrowArr->count()){
		ImageView* pImg = dynamic_cast<ImageView*>(m_stLeftArrowArr->lastObject());
		pImg->removeFromParentAndCleanup(true);
		m_stLeftArrowArr->removeLastObject(true);
	}

	while(rightSize < m_stRightArrowArr->count()){
		ImageView* pImg = dynamic_cast<ImageView*>(m_stRightArrowArr->lastObject());
		pImg->removeFromParentAndCleanup(true);
		m_stRightArrowArr->removeLastObject(true);
	}

	//更新箭头位置
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("ui_v2/fight.plist");
	for(unsigned int i = 0; i < leftSize; i++){
		ImageView* pImg = NULL;
		if(i < m_stLeftArrowArr->count()){
			pImg = dynamic_cast<ImageView*>(m_stLeftArrowArr->objectAtIndex(i));
		}else{
			pImg = ImageView::create();
			pImg->loadTexture("fight/arrow_tip.png", UI_TEX_TYPE_PLIST);
			pImg->setScaleX(-1);
			m_pUiLayer->addWidget(pImg);
			m_stLeftArrowArr->addObject(pImg);
		}
		
		pImg->setPosition(visibleOrigin + ccp(0 + pImg->getSize().width/2, stLeftPoints.at(i).y));
	}

	for(unsigned int i = 0; i < rightSize; i++){
		ImageView* pImg = NULL;
		if(i < m_stRightArrowArr->count()){
			pImg = dynamic_cast<ImageView*>(m_stRightArrowArr->objectAtIndex(i));
		}else{
			pImg = ImageView::create();
			pImg->loadTexture("fight/arrow_tip.png", UI_TEX_TYPE_PLIST);
			m_pUiLayer->addWidget(pImg);
			m_stRightArrowArr->addObject(pImg);
		}

		pImg->setPosition(visibleOrigin + ccp(visibleW - pImg->getSize().width/2, stRightPoints.at(i).y));
	}
}

bool CBaseFightScene::checkArrowCondition(CQStageEntity* pstEntity){
	if(pstEntity->checkIsDied() || pstEntity->getRoleParty() != ROLE_PARTY_HOSTILE
		|| typeid(*pstEntity) != typeid(CEnemy)){
		return false;
	}

	return true;
}

void CBaseFightScene::updateMoveEntities(){
	static float m_fMoveInter = 0;
	if(m_fMoveInter > 5){   //5帧更新一次
		m_fMoveInter = 0;
		return;
	}

	m_fMoveInter ++;

	CCObject* pstObj;
	CCARRAY_FOREACH(_moveEntities, pstObj){
		CQStageEntity* pstEntity = dynamic_cast<CQStageEntity*>(pstObj);

		if(pstEntity->getMyState() != kActionStateForwardDash){
			continue;
		}

		CInstanceUtils::DashSkillAtk(pstEntity, _stageEntities, this);
	}
}

void CBaseFightScene::setMainHeroAsTheOne(){
	pauseAllMonsters();
	_mainHero->getParent()->reorderChild(_mainHero, ROLE_Z_ORDER);
	_mainHero->setIsTheOne(true);
	//_hudLayer->Inactive();		//摇杆不能按
	m_pstBlackScreen = CCSprite::create();
    m_pstBlackScreen->initWithSpriteFrameName("share/black.png");
	m_pstBlackScreen->setScaleX(CCDirector::sharedDirector()->getVisibleSize().width / m_pstBlackScreen->getContentSize().width);
	m_pstBlackScreen->setScaleY(CCDirector::sharedDirector()->getVisibleSize().height / m_pstBlackScreen->getContentSize().height);
	m_pstBlackScreen->setAnchorPoint(ccp(0, 0));
	_sceneForegound->addChild(m_pstBlackScreen, ROLE_Z_ORDER - 1);
	m_pstBlackScreen->setPosition(ccpSub(ccp(0,0), _sceneForegound->getPosition()));
	m_bIsInTheOneState = true;
	m_uTheOneBeginSec = GlobalTime::getTime();	
	_mainHero->PauseMovement();
	_sceneForegound->reorderChild(_mainHero, ROLE_Z_ORDER);

	m_pstSanguang = CCArmature::create("sanguang");
	_sceneForegound->addChild(m_pstSanguang, ROLE_Z_ORDER + 10);
	m_pstSanguang->setPosition(_mainHero->GetCenterPos());
	m_pstSanguang->getAnimation()->play("sanguang", -1, -1, 0);
	m_pstSanguang->getAnimation()->setMovementEventCallFunc(this,movementEvent_selector(CBaseFightScene::handleSanguangActionEvent));
}

void CBaseFightScene::unSetMainHeroAsTheOne(){
	if (!m_bIsInTheOneState){
		return;
	}
	_mainHero->setIsTheOne(false);
	//_hudLayer->Active();		//摇杆激活
	resumeAllMonsters();	
	_sceneForegound->removeChild(m_pstBlackScreen, true);
	_sceneForegound->removeChild(m_pstSanguang, true);
	_mainHero->ResumeMovement();
	m_bIsInTheOneState = false;
}

void CBaseFightScene::handleSanguangActionEvent(CCArmature * armature, MovementEventType type, const char * name){
	if (type == COMPLETE){
		unSetMainHeroAsTheOne();
	}
}

void CBaseFightScene::handleDieEffActionEvent(CCArmature * armature, MovementEventType type, const char * name){
	if (type == COMPLETE){
		armature->removeFromParentAndCleanup(true);
	}
}

void CBaseFightScene::pauseAllMonsters(){
	CCObject* enimy;
	CCARRAY_FOREACH(_stageEntities, enimy){
		if (dynamic_cast<CQStageEntity*>(enimy)->m_uID != _mainHero->m_uID){
			((CQStageEntity*)enimy)->Pause();
		}
	}
}

void CBaseFightScene::resumeAllMonsters(){
	CCObject* enimy;
	CCARRAY_FOREACH(_stageEntities, enimy){
		if (dynamic_cast<CQStageEntity*>(enimy)->m_uID != _mainHero->m_uID){
			((CQStageEntity*)enimy)->Resume();
		}
	}
}

void CBaseFightScene::stopAllMonsters(){
	CCObject* enimy;
	CCARRAY_FOREACH(_stageEntities, enimy){
		if (dynamic_cast<CQStageEntity*>(enimy)->m_uID != _mainHero->m_uID){
			((CQStageEntity*)enimy)->ForceIdle();
			((CQStageEntity*)enimy)->Stop();
		}
	}
}

void CBaseFightScene::pauseFight(){
	CCObject* enimy;
	CCARRAY_FOREACH(_stageEntities, enimy){
		((CQStageEntity*)enimy)->Pause();
	}

	CCObject* pObj;
	CCARRAY_FOREACH(_skills, pObj){
		((CSkillEffLayer*)pObj)->pauseSkill();
	}

	m_trickBtn->pauseCD();
	m_skillBtn->pauseCD();
	m_dodgeBtn->pauseCD();

	this->pauseSchedulerAndActions();

	m_bIsGamePause = true;
}

void CBaseFightScene::resumeFight(){
	CCObject* enimy;
	CCARRAY_FOREACH(_stageEntities, enimy){
		((CQStageEntity*)enimy)->Resume();
	}

	CCObject* pObj;
	CCARRAY_FOREACH(_skills, pObj){
		((CSkillEffLayer*)pObj)->resumeSkill();
	}

	m_trickBtn->resumeCD();
	m_skillBtn->resumeCD();
	m_dodgeBtn->resumeCD();

	this->resumeSchedulerAndActions();

	m_bIsGamePause = false;
}

void CBaseFightScene::checkTheOneState(){
	unsigned int uNow = GlobalTime::getTime();
	if (uNow - m_uTheOneBeginSec >= 10){
		unSetMainHeroAsTheOne();
	}
}

void CBaseFightScene::mainHeroShoot(){
	if(m_bDisturbShoot){		//技能释放的时候，点击射击，需要在技能释放完了立马射击
		m_bShootAfterSkill = true;
		//CCLOG("return here w/o shoot m_bDisturbShoot %d!", m_bDisturbShoot);
		return;
	}

	if (needTurnBack()){
		//_mainHero->setScaleX(-1 * _mainHero->getScaleX());
		_mainHero->turnBackShooting();
	}else{
		_mainHero->stateChangeByAttack(true);
	}
	
	m_bShootAfterSkill = false;
	m_bIsMainHeroShooting = true;
}

void CBaseFightScene::mainHeroCeaseShoot(){
	_mainHero->stateChangeByAttack(false);
	m_bShootAfterSkill = false;
	m_bIsMainHeroShooting = false;

	cancelshowBulletOutTip();
}

void CBaseFightScene::didChangeDirectionTo(HudLayer *hud, cocos2d::CCPoint direction){
	if(NULL == _mainHero) {
        return;
    }
	bool bNeedTurnBack = needTurnBack();
// 	if (bNeedTurnBack && _mainHero->isShooting()){
// 		direction.x = fabs(direction.x) * _mainHero->getScaleX() * (-1);
// 		_mainHero->stateChangeByAttack(false);
// 		_mainHero->stateChangeByMove(direction);
// 		_mainHero->stateChangeByAttack(true);
// 	}else{
// 		_mainHero->stateChangeByMove(direction);
// 	}
	if (bNeedTurnBack && _mainHero->isShooting()){
		_mainHero->turnBackShooting(direction);
	}else{
		_mainHero->stateChangeByMove(direction);
	}
	
	
}

void CBaseFightScene::isHoldingDirection(HudLayer *hud, cocos2d::CCPoint direction){
	bool bNeedTurnBack = needTurnBack();
// 	if (bNeedTurnBack && _mainHero->isShooting()){
// 			direction.x = fabs(direction.x) * _mainHero->getScaleX() * (-1);
// 			_mainHero->stateChangeByAttack(false);
// 			_mainHero->stateChangeByMove(direction);
// 			_mainHero->stateChangeByAttack(true);
// 		}else{
// 			_mainHero->stateChangeByMove(direction);
// 		}
//	_mainHero->stateChangeByMove(direction);

	if (bNeedTurnBack  && _mainHero->isShooting()){
		_mainHero->turnBackShooting(direction);
	}else{
		_mainHero->stateChangeByMove(direction);
	}
}

void CBaseFightScene::simpleDPadTouchEnded(HudLayer *hud){
	_mainHero->stateChangeByReleaseHud();
}

void CBaseFightScene::autoBtnEvent(CCObject *pSender, TouchEventType type){
	switch (type)
	{
	case TOUCH_EVENT_ENDED:
		if(pSender->m_uID == m_pAutoBtn->m_uID){
			if(P_USER_OBJ->getUserInfo()->vipLv <= 0){
				POP_TIP_LAYER_MASTER->ShowErrorStr(TextStr(TEXT_VIP_AUTO_FIGHT));
				return;
			}

			_mainHero->ForceIdle();
			_mainHero->setAIType(AI_TYPE_ATTACK_BOSS_CLOSEST);
			_mainHero->SetIsControlledByAI(true);
			m_pAutoBtn->setVisible(false);
			m_pAutoBtn->setTouchEnabled(false);

			addAutoFightLayer();

			autoFightHandle();
		}
		break;
	}
}

void CBaseFightScene::autoFightHandle(){

}

void CBaseFightScene::addAutoFightLayer(){
	if(m_pAutoLayer && m_pAutoLayer->getParent()){
		m_pAutoLayer->RemoveEventListener(this, CAutoFightLayer::CANCEL_AUTO_EVENT, eventListner_selector(CBaseFightScene::handleCancelAutoEvent));
		m_pAutoLayer->removeFromParentAndCleanup(true);
		m_pAutoLayer = NULL;
	}

	m_pAutoLayer = CAutoFightLayer::create();
	UILayer *pTopLayer = dynamic_cast<UILayer*>(getLayer(LAYER_ID_TOP));
	m_pAutoLayer->setZOrder(10);
	m_pAutoLayer->AddEventListener(this, CAutoFightLayer::CANCEL_AUTO_EVENT, eventListner_selector(CBaseFightScene::handleCancelAutoEvent));
	pTopLayer->addWidget(m_pAutoLayer);
}

void CBaseFightScene::handleCancelAutoEvent(const string& stEvent, CCObject* pstEventObj){
	if(m_pAutoLayer && m_pAutoLayer->getParent()){
		m_pAutoLayer->RemoveEventListener(this, CAutoFightLayer::CANCEL_AUTO_EVENT, eventListner_selector(CBaseFightScene::handleCancelAutoEvent));
		m_pAutoLayer->removeFromParentAndCleanup(true);
		m_pAutoLayer = NULL;
	}

	_mainHero->ForceIdle();
	_mainHero->SetIsControlledByAI(false);
	m_pAutoBtn->setVisible(true);
	m_pAutoBtn->setTouchEnabled(true);
}

void CBaseFightScene::removeAutoFightLayer(){
	if(m_pAutoLayer && m_pAutoLayer->getParent()){
		m_pAutoLayer->RemoveEventListener(this, CAutoFightLayer::CANCEL_AUTO_EVENT, eventListner_selector(CBaseFightScene::handleCancelAutoEvent));
		m_pAutoLayer->removeFromParentAndCleanup(true);
		m_pAutoLayer = NULL;
	}
}

void CBaseFightScene::btnTouchEvent(CCObject *pSender, TouchEventType type){
	UIWidget *pWidget = dynamic_cast<UIWidget*>(pSender);
	const char *pszWidgetName = pWidget->getName();
	//CCLOG("type %d, sender %p, m_uid %d, fire btn %p, uid %d",type,  pSender, pSender->m_uID, fireBtn, fireBtn->m_uID);
	if(_mainHero->IsDizzy()){  //眩晕时不能放技能
		return;
	}

	switch (type)
	{
	case TOUCH_EVENT_BEGAN:
		if (pSender->m_uID == fireBtn->m_uID){
			mainHeroShoot();
		}else if(pSender->m_uID == pauseBtn->m_uID){

		}

		break;
	case TOUCH_EVENT_MOVED:

		break;
	case TOUCH_EVENT_ENDED:
		if (pSender->m_uID == fireBtn->m_uID){
			mainHeroCeaseShoot();
		}else if(pSender->m_uID == pauseBtn->m_uID){
			P_LEVEL_CTL->getPausePanel()->open();
		}else if(pSender->m_uID == m_pChangeWeaponBtn->m_uID){
			autoChangeNextWeapon();
		}else{
			if(strcmp(pszWidgetName, "medical_btn") == 0) {  //医药箱按钮
				doMedicalBtnClick();
			}
		}
		break;
	case TOUCH_EVENT_CANCELED:
		if (pSender->m_uID == fireBtn->m_uID){
			mainHeroCeaseShoot();
		}else{

		}
		break;
		
	default:
		break;
	}
}

void CBaseFightScene::updateBtnState(){
	if (_mainHero->IsShootable()){
		m_bDisturbShoot = false;

		if(m_bShootAfterSkill){
			mainHeroShoot();
		}
	}else{
		m_bDisturbShoot = true;
	}

	P_FIGHT_OBJ->setEp(_mainHero->getEP());

	if(_mainHero->getEP() < SKILL_CFG_MASTER->GetSkillCfgByID(m_trickBtn->getSkillID())->epcost()){
		m_trickBtn->setIsUsable(false);
	}else{
		m_trickBtn->setIsUsable(true);
	}

	if(_mainHero->getEP() < SKILL_CFG_MASTER->GetSkillCfgByID(m_skillBtn->getSkillID())->epcost()){
		m_skillBtn->setIsUsable(false);
	}else{
		m_skillBtn->setIsUsable(true);
	}
}

CCPoint CBaseFightScene::checkXYInWalkable(CCPoint pst){
	CCPoint stGridPt = xyToGrid(pst);
	if (stGridPt.x > _gridWNum){
		stGridPt.x = _gridWNum - 1;
	}

	if(stGridPt.y > _gridHNum){
		stGridPt.y = _gridHNum - 1;
	}

	return gridToXY(stGridPt);
}

/************************************************************************/
/*	               地表buff				                                */
/************************************************************************/

CSceneBuff* CBaseFightScene::addBuffBySkill(CSkillEffLayer* pstSkill, CCPoint point){
	unsigned int buffID = pstSkill->m_stCfg.floorbuffid();

	return addFloorBuff(buffID, pstSkill->getOwnerParty(), point);
}

CSceneBuff* CBaseFightScene::addFloorBuff(unsigned int floorBuffID, unsigned int roleParty, CCPoint point, bool isDirectAdd/* = false*/){
	CSceneBuff* pstBuff = CSceneBuff::create();
	pstBuff->LoadRes(floorBuffID, isDirectAdd);

	//判断地表buff是否在可行的区域内，超出不添加
	if(point.x + pstBuff->getContentSize().width/2 <= walkableRect.getMinX()
		|| point.x - pstBuff->getContentSize().width/2 >= walkableRect.getMaxX()){
			return NULL;
	}

	int iNewZorder = CInstanceUtils::GetZOrderByType(FLOOR_BUFF_Z_ORDER, point.y);
	if(pstBuff->m_stCfg.modeltype() == 1){  //立体buff，使用人物的z轴算法
		iNewZorder = CInstanceUtils::GetZOrderByType(ROLE_Z_ORDER, point.y);
	}
	pstBuff->setPosition(point);
	pstBuff->setOwnerParty(roleParty);
	pstBuff->AddEventListener(this, CSceneBuff::SCENE_BUFF_DISAPPEAR, eventListner_selector(CBaseFightScene::handleSceneBuffDisappearEvent));
	_sceneBuffs->addObject(pstBuff);
	_sceneForegound->addChild(pstBuff, iNewZorder);

	return pstBuff;
}

CWeaponBuff* CBaseFightScene::addWeaponBuff(unsigned int floorBuffID, unsigned int roleParty, CCPoint point, bool isDirectAdd/* = false*/){
	CWeaponBuff* pstBuff = CWeaponBuff::create();
	pstBuff->LoadRes(floorBuffID, isDirectAdd);

	//判断地表buff是否在可行的区域内，超出不添加
	if(point.x + pstBuff->getContentSize().width/2 <= walkableRect.getMinX()
		|| point.x - pstBuff->getContentSize().width/2 >= walkableRect.getMaxX()){
			return NULL;
	}

	int iNewZorder = CInstanceUtils::GetZOrderByType(FLOOR_BUFF_Z_ORDER, point.y);
	if(pstBuff->m_stCfg.modeltype() == 1){  //立体buff，使用人物的z轴算法
		iNewZorder = CInstanceUtils::GetZOrderByType(ROLE_Z_ORDER, point.y);
	}
	pstBuff->setPosition(point);
	pstBuff->setOwnerParty(roleParty);
	pstBuff->AddEventListener(this, CSceneBuff::SCENE_BUFF_DISAPPEAR, eventListner_selector(CBaseFightScene::handleSceneBuffDisappearEvent));
	_sceneBuffs->addObject(pstBuff);
	_sceneForegound->addChild(pstBuff, iNewZorder);

	return pstBuff;
}

void CBaseFightScene::removeFloorBuff(CSceneBuff* pstBuff, bool isRemoveRoleBuff){
	CSceneBuff* stBuff;
	for(unsigned int i = 0; i < _sceneBuffs->count(); i++){
		stBuff = dynamic_cast<CSceneBuff*>(_sceneBuffs->objectAtIndex(i));
		if(pstBuff->m_uID == stBuff->m_uID){
			if(isRemoveRoleBuff){
				CCObject* pEntityObj;
				CCARRAY_FOREACH(_stageEntities, pEntityObj){
					CQStageEntity* pstTarget = dynamic_cast<CQStageEntity*>(pEntityObj);

					//buff消失，角色身上有buff就去掉
					if(pstTarget->CheckHasBuff(pstBuff->getBuffID(), pstBuff->m_uID)){
						pstTarget->RemoveBuff(pstBuff->getBuffID(), pstBuff->m_uID);
					}
				}
			}
			_sceneBuffs->removeObjectAtIndex(i);
			pstBuff->removeFromParentAndCleanup(true);
			break;
		}
	}
}

/************************************************************************/
/*	               场景物件				                                */
/************************************************************************/

void CBaseFightScene::addSceneItem(CItemObj* pstObj){
	//添加物件, add后面记得updateMapBlocks
	CSceneItem* item = CSceneItem::create();
	item->setCfgID(pstObj->id);
	item->setItemObj(pstObj);
	item->loadRes();
	item->setPosition(ccp(pstObj->x, _sceneForegound->getContentSize().height - pstObj->y));
	item->AddEventListener(this, CSceneItem::SCENE_ITEM_FALLING_EVENT, eventListner_selector(CBaseFightScene::handleSceneItemFallEvent));
	item->AddEventListener(this, CSceneItem::SCENE_ITEM_DISAPPEAR, eventListner_selector(CBaseFightScene::handleSceneItemDisappearEvent));
	int iNewZorder = CInstanceUtils::GetZOrderByType(ROLE_Z_ORDER, item->getPositionY());
	_sceneForegound->addChild(item, iNewZorder);

	if(item->getItemType() == ITEM_TYPE_BLOCK_DESTROY || 	item->getItemType() == ITEM_TYPE_BLOCK_UNDESTROY){  //阻挡
		getBlocksInSize(item->getBlockSize(), (CCPoint)item->getPosition(), true);

		CCLOG("the scene item size is with === %f, height ==== %f", item->getBlockSize().width, item->getBlockSize().height);
		updateMapBlocks();
	}

	_sceneItems->addObject(item);
}

/************************************************************************/
/*	               地表陷阱				                                */
/************************************************************************/

void CBaseFightScene::addSceneTrap(CTrapObj* pstObj, bool isDirectAdd/* = false*/){
	CSceneTrap* pstTrap = CSceneTrap::create();
	pstTrap->LoadRes(pstObj->id, isDirectAdd);

	//判断地表陷阱是否在可行的区域内，超出不添加
	if(pstObj->x + pstTrap->getContentSize().width/2 <= walkableRect.getMinX()
		|| pstObj->x - pstTrap->getContentSize().width/2 >= walkableRect.getMaxX()){
			return;
	}

	if(pstTrap->m_stCfg.type() != TRAP_TYPE_PUNCTURE){
		pstTrap->setPosition(ccp(pstObj->x, _sceneForegound->getContentSize().height - pstObj->y));
	}

	pstTrap->setTouchID(pstObj->touchId);
	pstTrap->setOwnerParty(pstObj->party);
	pstTrap->AddEventListener(this, CSceneTrap::SCENE_TRAP_DISAPPEAR, eventListner_selector(CBaseFightScene::handleSceneTrapDisappearEvent));
	pstTrap->AddEventListener(this, CSkillEffLayer::HIT_KEY_FRAME_EVENT, eventListner_selector(CBaseFightScene::handleSceneTrapHitKeyEvent));
	_sceneTraps->addObject(pstTrap);
	int iNewZorder = CInstanceUtils::GetZOrderByType(FLOOR_BUFF_Z_ORDER, pstTrap->getPositionY());
	if(pstTrap->m_stCfg.modeltype() == 1){  //立体陷阱，使用人物的z轴算法
		iNewZorder = CInstanceUtils::GetZOrderByType(ROLE_Z_ORDER, pstTrap->getPositionY());
	}
	_sceneForegound->addChild(pstTrap, iNewZorder);
}

void CBaseFightScene::addPunctureTrap(CTrapObj* pstObj, bool isDirectAdd /* = false */){
	CPunctureTrap* pstTrap = CPunctureTrap::create();
	pstTrap->LoadRes(pstObj->id, isDirectAdd);

	//判断地表陷阱是否在可行的区域内，超出不添加
	if(pstObj->x + pstTrap->getContentSize().width/2 <= walkableRect.getMinX()
		|| pstObj->x - pstTrap->getContentSize().width/2 >= walkableRect.getMaxX()){
			return;
	}

	if(pstTrap->m_stCfg.type() == TRAP_TYPE_PUNCTURE){
		//屏幕右边缘
		CCSize stSize = CCDirector::sharedDirector()->getVisibleSize(); 
		pstTrap->setPosition(ccp(stSize.width + fabs(_sceneForegound->getPositionX()) - pstTrap->getContentSize().width/2, 
			_sceneForegound->getContentSize().height - pstObj->y));
	}

	pstTrap->setTouchID(pstObj->touchId);
	pstTrap->setOwnerParty(pstObj->party);
	pstTrap->AddEventListener(this, CSkillEffLayer::HIT_KEY_FRAME_EVENT, eventListner_selector(CBaseFightScene::handleSceneTrapHitKeyEvent));
	_sceneTraps->addObject(pstTrap);
	int iNewZorder = CInstanceUtils::GetZOrderByType(FLOOR_BUFF_Z_ORDER, pstTrap->getPositionY());
	if(pstTrap->m_stCfg.modeltype() == 1){  //立体陷阱，使用人物的z轴算法
		iNewZorder = CInstanceUtils::GetZOrderByType(ROLE_Z_ORDER, pstTrap->getPositionY());
	}
	_sceneForegound->addChild(pstTrap, iNewZorder);
}

void CBaseFightScene::removeTrap(CSceneTrap* pstTrap, bool isRemoveRoleBuff){
	CSceneTrap* stTrap;
	for(unsigned int i = 0; i < _sceneTraps->count(); i++){
		stTrap = dynamic_cast<CSceneTrap*>(_sceneTraps->objectAtIndex(i));
		if(pstTrap->m_uID == stTrap->m_uID){

			//if(isRemoveRoleBuff){
			//	CCObject* pEntityObj;
			//	CCARRAY_FOREACH(_stageEntities, pEntityObj){
			//		CQStageEntity* pstTarget = dynamic_cast<CQStageEntity*>(pEntityObj);
			//		//陷阱消失，角色身上有buff就去掉
			//		if(pstTarget->CheckHasBuff(pstTrap->m_stCfg.buffid())){
			//			pstTarget->RemoveBuff(pstTrap->m_stCfg.buffid());
			//		}
			//	}
			//}

			_sceneTraps->removeObjectAtIndex(i);
			pstTrap->removeFromParentAndCleanup(true);
			break;
		}
	}
}

void CBaseFightScene::changeWeapon(){

}

void CBaseFightScene::skillRandFlyToGroud(CQStageEntity* pstAtker, CQSkillCell* pstSkill){
	m_uCurSKillCnt = 0;
	m_pSkillCell = NULL;
	m_pSkillAtker = NULL;
	m_pSkillCell = pstSkill;
	m_pSkillAtker = pstAtker;
	m_uSkillEffCnt = pstSkill->m_stCfg.targetcnt();
	this->unschedule(schedule_selector(CBaseFightScene::skillStepUpdate));

	getSkillAttackPos(pstSkill);
	float inter = pstSkill->m_stCfg.effinter()*1.0/1000.0f;
	this->schedule(schedule_selector(CBaseFightScene::skillStepUpdate), inter);
}

void CBaseFightScene::skillStepUpdate(float dt){
	if(m_bIsPreOver){
		this->unschedule(schedule_selector(CBaseFightScene::skillStepUpdate));
		return;
	}

	if(m_uCurSKillCnt >= m_stSkillPoints.size()){
		return;
	}

	if(m_pSkillCell->m_stCfg.triggertype() == SKILL_TRIGGER_RAND_DIRECT_ON_GROUND){  //在地上随机出现固定数量特效，其中一个固定为主角位置
		CInstanceUtils::SkillWarningOnGround(m_pSkillAtker, m_pSkillCell, m_stSkillPoints[m_uCurSKillCnt], this);
	}else if(m_pSkillCell->m_stCfg.triggertype() == SKILL_TRIGGER_RAND_FLY_TO_GROUND){  //随机飞向几个固定点，其中一个固定为主角位置
		CInstanceUtils::SkillWarningOnGround(m_pSkillAtker, m_pSkillCell, m_stSkillPoints[m_uCurSKillCnt], this);
	}

	m_uCurSKillCnt++;
	if(m_uCurSKillCnt >= m_uSkillEffCnt){
		this->unschedule(schedule_selector(CBaseFightScene::skillStepUpdate));
	}
}

void CBaseFightScene::getSkillAttackPos(CQSkillCell* pstSkill){
	m_stSkillPoints.clear();

	//第一个点为主角位置
	m_stSkillPoints.push_back(_mainHero->GetFootPos());

	unsigned int num = pstSkill->m_stCfg.targetcnt() - 1;
	unsigned int wDis = pstSkill->m_stCfg.effectx();
	unsigned int hDiw = pstSkill->m_stCfg.effecty();
	float width = 150;
	float height = 100;
	float x;
	float y;
	for(unsigned int i = 0; i < num; i++){
		if(i < num/2){  //左边
			if(wDis > (_mainHero->GetFootPos().x - width/2)){
				x = _mainHero->GetFootPos().x - CRandom::GetRand(_mainHero->GetFootPos().x - width/2);
			}else{
				x = _mainHero->GetFootPos().x - CRandom::GetRand(wDis);
			}
		}else{
			if(_mainHero->GetFootPos().x + width/2 + wDis > walkableRect.getMaxX()){
				x = _mainHero->GetFootPos().x + CRandom::GetRand(walkableRect.getMaxX() - _mainHero->GetFootPos().x - width/2);
			}else{
				x = _mainHero->GetFootPos().x + CRandom::GetRand(wDis);
			}
		}

		y = height/4 + CRandom::GetRand(m_uWalkHight - height/2);

		m_stSkillPoints.push_back(ccp(x, y));
	}
}

void CBaseFightScene::openModuleEffect(UIWidget* pWidget){
	if(pWidget == NULL){
		return;
	}

	CCArmature* pGuideEff = CCArmature::create("ut00035");
	m_pUiLayer->addChild(pGuideEff, 2014);
	if(strcmp("fire_button", pWidget->getName()) != 0){
		pGuideEff->setPosition(m_pSkillLayout->getPosition() + pWidget->getPosition() + ccp(pWidget->getSize().width/2, pWidget->getSize().height/2));
	}else{
		pGuideEff->setPosition(m_pSkillLayout->getPosition() + pWidget->getPosition());
	}
	pGuideEff->getAnimation()->setUserObject(pWidget);
	pGuideEff->getAnimation()->setSpeedScale(0.5f);
	pGuideEff->getAnimation()->setMovementEventCallFunc(this, movementEvent_selector(CBaseFightScene::handleOpenModuleEffComplete));
	pGuideEff->getAnimation()->play("fly", -1, -1, 0);
}

void CBaseFightScene::handleOpenModuleEffComplete(CCArmature * armature, MovementEventType type, const char * name){
	if (type == COMPLETE){
		armature->removeFromParentAndCleanup(true);
		UIWidget* pWidge = dynamic_cast<UIWidget*>(armature->getAnimation()->getUserObject());
		if(pWidge != NULL){
			pWidge->setVisible(true);
			pWidge->setTouchEnabled(true);
		}

		if(strcmp("fire_button", pWidge->getName()) != 0){
			SkillButton* pBtn = dynamic_cast<SkillButton*>(pWidge);
			pBtn->setBtnEnable(true);
		}
	}	
}

void CBaseFightScene::brushOneMonster(CEnemyData* pstObj){
	string emergeStr = getEmergeByData(pstObj);
	if(emergeStr != "null"){
		addOneMonster(pstObj, true);
	}else{
		//通用出场特效
		CCArmature* pArm = CCArmature::create("ut00038");
		pArm->getAnimation()->play("fly", -1, -1, 0);
		pArm->setUserObject(pstObj);
		pArm->setPosition(ccp(pstObj->x, _sceneForegound->getContentSize().height - pstObj->y));
		int iNewZorder = CInstanceUtils::GetZOrderByType(ROLE_Z_ORDER, pArm->getPositionY());
		_sceneForegound->addChild(pArm, iNewZorder + 10);
		pArm->getAnimation()->setFrameEventCallFunc(this, frameEvent_selector(CBaseFightScene::handleComEmergeEvent));
		pArm->getAnimation()->setMovementEventCallFunc(this, movementEvent_selector(CBaseFightScene::handleComEmergeComplete));
	}
}

string CBaseFightScene::getEmergeByData(CEnemyData* pstObj){
	MonsterCfg* monsterCfg = MONSTER_CFG_MASTER->GetMonsterCfgByID(pstObj->id);
	return monsterCfg->actions().emerge();
}

void CBaseFightScene::handleComEmergeEvent(CCBone* bone, const char* stEvent, int originFrameIndex, int currentFrameIdx){
	string strEvt = stEvent;
	if(strEvt == "chuxian"){
		CEnemyData* pstObj = dynamic_cast<CEnemyData*>(bone->getArmature()->getUserObject());
		if(!m_bIsPreOver){
			addOneMonster(pstObj, false);
		}
	}
}

CEnemy* CBaseFightScene::addOneMonster(CEnemyData* pstObj, bool isEmerge){
	CEnemy* pstMonster = initOneMonster(pstObj);
	if(pstMonster == NULL){
		return NULL;
	}

	pstMonster->LoadActionRes(isEmerge);
	int iNewZorder = CInstanceUtils::GetZOrderByType(ROLE_Z_ORDER, pstMonster->getPositionY());
	_sceneForegound->addChild(pstMonster, iNewZorder);

	if(!pstMonster->getBlockSize().equals(CCSizeZero)){
		getBlocksInSize(pstMonster->getBlockSize(), (CCPoint)pstMonster->getPosition(), true);
		updateMapBlocks();
	}

	if(pstMonster->getRoleType() == ROLE_TYPE_SENIOR_BOSS || pstMonster->getRoleType() == ROLE_TYPE_ELITE){
		pstMonster->setScaleX(-1);
	}

	return pstMonster;
}

void CBaseFightScene::handleComEmergeComplete(CCArmature * armature, MovementEventType type, const char * name){
	if (type == COMPLETE){
		armature->removeFromParentAndCleanup(true);
	}	
}

void CBaseFightScene::brushOneMonsterWithWalkIn(CEnemyData* pstObj){
	CEnemy* pstMonster = addOneMonster(pstObj, false);
	pstMonster->setIsInvincible(true);  //无敌
	pstMonster->SetIsControlledByAI(false);

	CCRect stRect = getPlayerVisibleSize();
	float fStartX;
	float fEndX;
	float fStartY = pstMonster->getPositionY();
	if(pstObj->type == APPEAR_WALK_IN_LEFT){
		fStartX = stRect.getMinX() - pstMonster->getCenterToSides();
		if(stRect.getMinX() < getStageRect().getMinX()){
			fEndX = getStageRect().getMinX() + pstMonster->getCenterToSides();
		}else{
			fEndX = stRect.getMinX() + pstMonster->getCenterToSides();
		}
	}else if(pstObj->type == APPEAR_WALK_IN_RIGHT){
		fStartX = stRect.getMaxX() + pstMonster->getCenterToSides();
		fEndX = stRect.getMaxX() - pstMonster->getCenterToSides();
	}

	pstMonster->SetEnityPosition(ccp(fStartX, fStartY));
	pstMonster->setDesiredPosition(ccp(fStartX, fStartY));

	moveToDst(pstMonster, ccp(fEndX, fStartY));
}

void CBaseFightScene::moveToDst(CQStageEntity* pstEntity, CCPoint stPos){
	if(pstEntity->getPosition().getDistance(stPos) < 20.0f) {
		onMonsterArrived("walk_done", pstEntity);
	} else {
		//CCLOG("[TRACE]::the monster start start move is %u", pstEntity->m_uID);
		pstEntity->WalkTo(stPos);
		pstEntity->AddEventListener(this, "walk_done", eventListner_selector(CBaseFightScene::onMonsterArrived));
	}
}

void CBaseFightScene::onMonsterArrived(const string &evt, CCObject *pSender){
	CQStageEntity* pstEntity = dynamic_cast<CQStageEntity*>(pSender);
	//CCLOG("[TRACE]::the monster end  end move is %u", pstEntity->m_uID);
	pstEntity->setIsInvincible(false);
	pstEntity->SetIsControlledByAI(true);
}

void CBaseFightScene::showBossWarning(){
	CCSize stSize = CCDirector::sharedDirector()->getVisibleSize(); 
	CCPoint visibleOrigin = CCDirector::sharedDirector()->getVisibleOrigin();
	if(m_pBossWarning && m_pBossWarning->getParent()){
		m_pBossWarning->stopWarning();
		m_pBossWarning->removeFromParentAndCleanup(true);
		m_pBossWarning = NULL;
	}

	if(m_pBossWarning == NULL){
		m_pBossWarning = CBossWarning::create(); 
		m_pBossWarning->AddEventListener(this, CBossWarning::BOSS_WARNING_COMPLETE, eventListner_selector(CBaseFightScene::handleBossWarningCompleteEvent));
	}
	m_pBossWarning->setTouchEnabled(false);
	m_pBossWarning->setPosition(visibleOrigin + ccp(stSize.width, stSize.height));
	m_pUiLayer->addWidget(m_pBossWarning);

	m_pBossWarning->showWarning();
}

void CBaseFightScene::handleBossWarningCompleteEvent(const string& stEvent, CCObject* pstEventObj){
	m_pBossWarning->removeAllChildrenWithCleanup(true);
	m_pBossWarning = NULL;
}

void CBaseFightScene::playKillEff(){
	const FightKillNumCfg* stCfg = FIGHT_KILL_NUM_CFG->GeCfgByKillNum(m_uTotalEnemyKilled);
	if(stCfg == NULL){
		return;
	}

	CCSize stSize = CCDirector::sharedDirector()->getVisibleSize(); 
	CCPoint visibleOrigin = CCDirector::sharedDirector()->getVisibleOrigin();
	CCArmature* pArm = dynamic_cast<CCArmature*>(m_pUiLayer->getChildByTag(KILL_EFF_TAG));

	if(pArm == NULL){
		pArm = CCArmature::create("ut00077");
		m_pUiLayer->addChild(pArm, 0, KILL_EFF_TAG);
		pArm->setPosition(visibleOrigin + ccp(stSize.width/2, stSize.height/2));
		pArm->getAnimation()->setMovementEventCallFunc(this, movementEvent_selector(CBaseFightScene::handleMainHeroHurtComplete));
	}

	string actName = "fly_" + intToString(stCfg->lev() - 1);
	pArm->getAnimation()->play(actName.c_str(), -1, -1, 0);

	AudioManager::playEffect(stCfg->killsound().c_str());
}

void CBaseFightScene::addCoinEff(CQStageEntity* pEntity, unsigned int uNum){
	if(!m_bIsAddCoinEff || uNum <= 0){
		return;
	}
	
	CCArmature* pArm = CCArmature::create("ut00042");
	CCPoint stPos = pEntity->getPosition();
	CCPoint stWorldPos = pEntity->getParent()->convertToWorldSpace(stPos);
	CCPoint stTargetPos = m_pUiLayer->convertToNodeSpace(stWorldPos);
	CCString* strValue = CCString::createWithFormat("%f", _sceneForegound->getPositionX());
	pArm->setPosition(stTargetPos);
	pArm->setUserObject(strValue);
	pArm->getAnimation()->setMovementEventCallFunc(this, movementEvent_selector(CBaseFightScene::handleCoinActComplete));
	string actName = "emerge" + getCoinActName(uNum);
	CCString* strName = CCString::create(actName);
	pArm->getAnimation()->setUserObject(strName);
	m_stAddCoins->addObject(pArm);

	//m_pUiLayer->addChild(pArm, 10);
	//m_stCoinEffs->addObject(pArm);
	//pArm->getAnimation()->play(actName.c_str(), -1, -1, 0);
	//AudioManager::playEffect("goldcoin.ogg");		//音效
}

const string CBaseFightScene::getCoinActName(unsigned int uNum){
	string str = "";
	if(uNum <= 1){
		str = "_0";
	}else if(uNum > 1 && uNum < 100){
		str = "_1";
	}else{
		str = "_2";
	}

	return str;
}

void CBaseFightScene::handleCoinActComplete(CCArmature* armature, MovementEventType type, const char* name){
	if (type == COMPLETE){
		string str = name;
		string actName = "";
		if(str.find_first_of("emerge_") == 0){
			string sub = str.substr(str.find_first_not_of("emerge_"));
			actName = "fly_" + sub;
			armature->getAnimation()->play(actName.c_str(), -1, -1, -1);
		}else if(str.find_first_of("die_") == 0){
			armature->removeFromParentAndCleanup(true);
		}
	}
}

void CBaseFightScene::handleCoinEndEvent(){

}

void CBaseFightScene::updateCoinPos(float dt){
	static int iCnt = 0;

	bool isAddEff = false;
	if(iCnt < 2){
		isAddEff = false;
		iCnt++;
	}else{
		iCnt = 0;
		isAddEff = true;
	}

	if(isAddEff){
		unsigned int uNum = 0;
		for(unsigned int idx = 0; idx < m_stAddCoins->count(); idx++){
			CCArmature* pArm = dynamic_cast<CCArmature*>(m_stAddCoins->objectAtIndex(idx));
			CCString* strValue = dynamic_cast<CCString*>(pArm->getAnimation()->getUserObject());
			string actName = strValue->getCString();
			m_pUiLayer->addChild(pArm, 10);
			m_stCoinEffs->addObject(pArm);
			pArm->getAnimation()->play(actName.c_str(), -1, -1, 0);
			AudioManager::playEffect("goldcoin.ogg");		//音效

			if(m_stAddCoins->indexOfObject(pArm) != UINT_MAX){
				m_stAddCoins->removeObject(pArm);
			}
			idx --;
			uNum++;

			if(uNum > 3){
				break;
			}
		}
	}

	for(unsigned int iIdx = 0; iIdx < m_stCoinEffs->count(); iIdx++){
		CCArmature* pArm = dynamic_cast<CCArmature*>(m_stCoinEffs->objectAtIndex(iIdx));
		bool bIsFlying = false;
		string nowActName = pArm->getAnimation()->getCurrentMovementID();
		if(nowActName.find_first_of("fly_") == 0){
			bIsFlying = true;
		}

		if(bIsFlying){
			CCPoint stPos = pArm->getPosition();
			CCPoint stDesPos = m_pHeroHeadUI->getPosition() + m_pHeroHeadUI->getHeadPos();
			string sub = nowActName.substr(nowActName.find_first_not_of("fly_"));
			string actName = "die_" + sub;

			if(stPos.getDistance(stDesPos) < 20) {
				pArm->getAnimation()->play(actName.c_str(), -1, -1, 0);
				AudioManager::playEffect("goldcoin_die.ogg");		//音效
				handleCoinEndEvent();

				if(m_stCoinEffs->indexOfObject(pArm) != UINT_MAX){
					m_stCoinEffs->removeObject(pArm);
				}
				iIdx --;
			}else{
				CCPoint dv = ccpNormalize(stDesPos - stPos);
				CCPoint _velocity = ccp(dv.x * 1000, dv.y * 1000);
				pArm->setPosition(ccpAdd(pArm->getPosition(), ccpMult(_velocity, dt)));
			}
		}else{
			CCString* strValue = dynamic_cast<CCString*>(pArm->getUserObject());
			float preX = strValue->floatValue();
			float disX = _sceneForegound->getPositionX() - preX;
			float armX = pArm->getPositionX();
			pArm->setPositionX(armX + disX);
			pArm->setUserObject(CCString::createWithFormat("%f", _sceneForegound->getPositionX()));
		}
	}
}

/************************************************************************/
/*                         事件处理    START                            */
/************************************************************************/

void CBaseFightScene::handleSkillKeyFrameEvent(const string& stEvent, CCObject* pstEventObj){
	if (!pstEventObj){
		return;
	}
	CQStageEntity* pstEntity = dynamic_cast<CQStageEntity*>(pstEventObj);

	CInstanceSkillUtils::SkillEffCastLogic(pstEntity, this, m_stTargetPos);
}

void CBaseFightScene::handleSkillEffAtkFrame(const string& stEvent, CCObject* pstEventObj){
	//计算哪些人在被打击范围,拥有打击帧的技能，必然是范围攻击类的，直接跟踪撞击类的在跟踪的时候就搞定了
	CSkillEffLayer* pstSkill = dynamic_cast<CSkillEffLayer*>(pstEventObj);
	bool bCleanSkill = false;

	////技能是否会对地表添加buff
	//if(pstSkill->m_stCfg.effectbuffid() != 0 && strcmp(pstSkill->m_stCfg.effectbuffres().c_str(), "null")){
	//	addBuffBySkill(pstSkill);
	//}

	if(pstSkill->m_stCfg.triggertype() == SKILL_TRIGGER_ON_ME){
		CInstanceUtils::SkillMakeEffect(pstSkill, pstSkill->getOwnerEntity(), this);
	}else{
		CInstanceUtils::SkillAtkEffect(pstSkill, _stageEntities, bCleanSkill, this);
	}
	
	//计算在战场的所有友方战斗单位
	//特殊处理一下，如果当前是玩家放大招的僵持状态，把黑幕的zorder放在最后
	if (_mainHero->getIsTheOne() && reinterpret_cast<uint64>(pstSkill->getOwnerEntity()) == reinterpret_cast<uint64>(_mainHero)){
		m_pstBlackScreen->setZOrder(0);  
	}

}

void CBaseFightScene::handleSkillEffEvent(const string& stEvent, CCObject* pstEventObj){
	CSkillEffLayer* pstSkill = dynamic_cast<CSkillEffLayer*>(pstEventObj);
	if (!getStageEntities()->containsObject(pstSkill->getOwnerEntity())){
		return;
	}

	string stPureEventStr = CQEventDispatcher::GetEventString(stEvent);
	if (strcmp(stPureEventStr.c_str(), CSkillEffLayer::HIT_KEY_FRAME_EVENT.c_str()) == 0){
		handleSkillEffAtkFrame(stEvent, pstEventObj);		
	}else if (strcmp(stPureEventStr.c_str(), CSkillEffLayer::SPAWN_KEY_FRAME_EVENT.c_str()) == 0){
		handleSkillEffSpawnFrame(stEvent, pstEventObj);
	}
}

void CBaseFightScene::handleSkillEffSpawnFrame(const string& stEvent, CCObject* pstEventObj){
	if(m_bIsPreOver){
		return;
	}

	CSkillEffLayer* pstSkill = dynamic_cast<CSkillEffLayer*>(pstEventObj);
	if (pstSkill->HasFlyAnimation() 
		&& (pstSkill->m_stCfg.triggertype() == SKILL_TRIGGER_SPAWN_FOLLOW 
			|| pstSkill->m_stCfg.triggertype() == SKILL_TRIGGER_SPAWN_FOLLOW_ON_GROUND
			|| pstSkill->m_stCfg.triggertype() == SKILL_TRIGGER_SPAWN_DIRECT)
		&& pstSkill->m_stCfg.targetcnt()){

			//找到所有的目标
			CCArray* stTargets = CCArray::create();
			unsigned int uiTargetParty = ROLE_PARTY_FRIEND;
			if (pstSkill->getOwnerEntity()->getRoleParty() == ROLE_PARTY_FRIEND){
				uiTargetParty = ROLE_PARTY_HOSTILE;
			}

			CInstanceUtils::GetSomeTargets(pstSkill->getOwnerEntity()->GetCenterPos(), 
				uiTargetParty,
				_stageEntities,
				pstSkill->m_stCfg.targetcnt(), 
				pstSkill->m_stCfg.castdist(),
				stTargets);

			//开始追踪他们
			CCObject *enimy;
			CCARRAY_FOREACH(stTargets, enimy){
				CSkillEffLayer* pstSpawnedSkill = CSkillEffLayer::create();
				pstSpawnedSkill->m_stCfg = pstSkill->m_stCfg;	
				pstSpawnedSkill->LoadRes(pstSkill->m_stCfg.skillid());
				pstSpawnedSkill->setTarget(dynamic_cast<CQStageEntity*>(enimy));
				if (pstSkill->IsFollowType()){
					pstSpawnedSkill->setFlyState(SKILL_ANIMATION_STATE_CHASE); //开始追踪
					pstSpawnedSkill->FlyToPos(dynamic_cast<CQStageEntity*>(enimy)->GetCenterPos());
					pstSpawnedSkill->setPosition(pstSkill->getPosition());
				}else if (pstSkill->m_stCfg.triggertype() == SKILL_TRIGGER_SPAWN_DIRECT){
					pstSpawnedSkill->setPosition(dynamic_cast<CQStageEntity*>(enimy)->getPosition());
					pstSpawnedSkill->Fly(false);
				}
				
				pstSpawnedSkill->AddEventListener(this, CSkillEffLayer::HIT_KEY_FRAME_EVENT, eventListner_selector(CBaseFightScene::handleSkillEffAtkFrame));
				pstSpawnedSkill->setOwnerEntity(pstSkill->getOwnerEntity());
				pstSpawnedSkill->setOwnerLevel(pstSkill->getOwnerLevel());
				pstSpawnedSkill->setOwnerParty(pstSkill->getOwnerParty());
				pstSpawnedSkill->m_stOwnerBrief = pstSkill->m_stOwnerBrief;
				_skills->addObject(pstSpawnedSkill);

				int iNewZorder = CInstanceUtils::GetZOrderByType(SKILL_Z_ORDER, pstSpawnedSkill->getPositionY());
				_sceneForegound->addChild(pstSpawnedSkill, iNewZorder);
			}
	}
}

void CBaseFightScene::handleFireABulletEvent(const string& stEvent, CCObject* pstEventObj){
	if(m_bIsPreOver){
		return;
	}

	CQStageEntity* pstEntity = dynamic_cast<CQStageEntity*>(pstEventObj);
	CInstanceUtils::FireABulletEvent(pstEntity, this);
}

void CBaseFightScene::handleCloseAtkKeyFrameEvent(const string& stEvent, CCObject* pstEventObj){
	CQStageEntity* pstEntity = dynamic_cast<CQStageEntity*>(pstEventObj);
	CQSkillCell* pstSkill = pstEntity->GetCurSkillCell();
    if(NULL == pstSkill) {
        return;
    }
	if(pstSkill->m_stCfg.triggertype() == SKILL_TRIGGER_CLOSE_ATTACK || pstSkill->m_stCfg.triggertype() == SKILL_TRIGGER_JUMP_ATK){
		CInstanceUtils::CloseSkillAtkEffect(pstEntity, _stageEntities, this);
	}else if(pstSkill->m_stCfg.triggertype() == SKILL_TRIGGER_ACTION_ONLY_ON_ME){
		CInstanceUtils::SkillMakeEffect(pstSkill, pstEntity, pstEntity, this);
	}
}

void CBaseFightScene::handleDieEffKeyFrameEvent(const string& stEvent, CCObject* pstEventObj){
	if (pstEventObj){
		CQStageEntity* pstEntity = dynamic_cast<CQStageEntity*>(pstEventObj);
		string dieRes = pstEntity->getDieResPath();

		if(dieRes == "null"){
			return;
		}

		//todo
		//CCArmature* m_pstDieEff = CCArmature::create(dieRes.c_str());
		//pstEntity->addChild(m_pstDieEff, 1100);
		////m_pstDieEff->setPosition(pstEntity->getPosition());
		//m_pstDieEff->getAnimation()->setMovementEventCallFunc(this,movementEvent_selector(CBaseFightScene::handleDieEffActionEvent));
		//m_pstDieEff->getAnimation()->play("fly", -1, -1, 0);
	}
}

void CBaseFightScene::handleHeroSkillCastEvent(const string& stEvent, CCObject* pstEventObj){
	CCString* strValue = (CCString*)pstEventObj;
	int skillID = strValue->intValue();

	if(P_USER_OBJ->getUserInfo()->evolutionSkillID == skillID){
		m_uEvolutionCnt++;
	}
}

void CBaseFightScene::handleHeroEmergeCompleteEvent(const string& stEvent, CCObject* pstEventObj){
	startUpdate();
}

void CBaseFightScene::handleHeroTheOneBeginEvent(const string& stEvent, CCObject* pstEventObj){
	if (stEvent.find(CQStageEntity::EXCLUSIVE_ONE_BEGIN_FRAME) != string::npos){
		setMainHeroAsTheOne();
	}
}

void CBaseFightScene::handleHeroTheOneEndEvent(const string& stEvent, CCObject* pstEventObj){
	if (stEvent.find(CQStageEntity::EXCLUSIVE_ONE_END_FRAME) != string::npos){
		unSetMainHeroAsTheOne();
	}
}

void CBaseFightScene::handleSceenShakeEvent(const string& stEvent, CCObject* pstEventObj){
	CInstanceUtils::ShakeScene(this, 1, 10);
}

void CBaseFightScene::handleHeroHPChangeEvent(const string& stEvent, CCObject* pstEventObj){
	//血量改变
	m_pHeroHeadUI->setHPPercent(_mainHero->getHP(), _mainHero->getMaxHP());
}

void CBaseFightScene::handleHeroHurtValueEvent(const string& stEvent, CCObject* pstEventObj){
	if(m_bIsPreOver){
		return;
	}

	//计入损失血量总值 
	CCString* strValue = (CCString*)pstEventObj;
	int hurtValue = strValue->intValue();
	m_uTotalLoseValue += hurtValue;	

	CCSize stSize = CCDirector::sharedDirector()->getVisibleSize(); 
	CCPoint visibleOrigin = CCDirector::sharedDirector()->getVisibleOrigin();
	CCArmature* pHeroHurtArm = dynamic_cast<CCArmature*>(m_pUiLayer->getChildByTag(HURT_ARM_TAG));

	if(pHeroHurtArm != NULL){
		return;
	}

	pHeroHurtArm = CCArmature::create("ut00021");
	m_pUiLayer->addChild(pHeroHurtArm, 0, HURT_ARM_TAG);
	pHeroHurtArm->setPosition(visibleOrigin + ccp(stSize.width/2, stSize.height/2));
	pHeroHurtArm->getAnimation()->setMovementEventCallFunc(this, movementEvent_selector(CBaseFightScene::handleMainHeroHurtComplete));
	pHeroHurtArm->getAnimation()->play("fly", -1, -1, 0);
}

void CBaseFightScene::handleMainHeroHurtComplete(CCArmature * armature, MovementEventType type, const char * name){
	if (type == COMPLETE){
		armature->removeFromParentAndCleanup(true);
	}	
}

void CBaseFightScene::handleHeroKilledEvent(const string& stEvent, CCObject* pstEventObj){
	doHeroDieEvent();
}

void CBaseFightScene::handleHeroEPCostEvent(const string& stEvent, CCObject* pstEventObj){
	CCString* strValue = (CCString*)pstEventObj;
	int hurtValue = strValue->intValue();

	//能量条改变
	m_pHeroHeadUI->setEPPercent(_mainHero->getEP(), _mainHero->getMaxEP());
}

void CBaseFightScene::handleEnemyHurtEvent(const string& stEvent, CCObject* pstEventObj){
	if (pstEventObj){
		CEnemy* pstMonster = dynamic_cast<CEnemy*>(pstEventObj);

		//boss受伤，出现血条
		if(pstMonster->getRoleType() == ROLE_TYPE_JUNIOR_BOSS || pstMonster->getRoleType() == ROLE_TYPE_SENIOR_BOSS){
			if(m_pBossHPUI->isVisible() == false){
				m_pBossHPUI->setVisible(true);
			}

			m_pBossHPUI->setBossType(pstMonster->getRoleType(), pstMonster->getHeadRes());
			m_pBossHPUI->setHPPercent(pstMonster->getHP(), pstMonster->getMaxHP());
		}
	}
}

void CBaseFightScene::handleEnemyHarmValueEvent(const string& stEvent, CCObject* pstEventObj){
	//计入总伤害 
	CCString* strValue = (CCString*)pstEventObj;
	int harmValue = strValue->intValue();
	m_uTotalHarmValue += harmValue;
}

void CBaseFightScene::handleEnemyDieCompleteEvent(const string& stEvent, CCObject* pstEventObj){

}

void CBaseFightScene::handleHeroKillValueChange(const string& stEvent, CCObject* pstEventObj){
	//杀怪总数
	m_uTotalEnemyKilled++;

	//猪脚连杀数据变化
	unsigned int m_uNowTime;
	if(m_uCurKillValue == 0 && m_uMaxKillValue == 0){ //杀死第一个怪
		m_uMaxKillValue = m_uCurKillValue = 1;
		m_uKillTime = (unsigned int)m_fFightTime;
	}else if(m_uCurKillValue == 0 &&  m_uMaxKillValue != 0){  //重置了连杀
		m_uCurKillValue = 1;
		m_uKillTime = (unsigned int)m_fFightTime;

		if(m_uCurKillValue > m_uMaxKillValue){
			m_uMaxKillValue = m_uCurKillValue;
		}
	}else if(m_uCurKillValue != 0 &&  m_uMaxKillValue != 0){  //正在连杀
		m_uNowTime = (unsigned int)m_fFightTime;
		unsigned int inter = (m_uNowTime - m_uKillTime);
		if(inter <= KILL_INTER){  //连杀间隔中
			m_uCurKillValue++;

			if(m_uCurKillValue > m_uMaxKillValue){
				m_uMaxKillValue = m_uCurKillValue;
			}

			m_uKillTime = (unsigned int)m_fFightTime;
		}else{  //连杀被终结
			m_uCurKillValue = 0;
		}
	}

	//连杀特效
	if(m_bIsPlayKillEff){
		playKillEff();
	}
}

void CBaseFightScene::handleHeroEPAddEvent(const string& stEvent, CCObject* pstEventObj){
	updateHit();

	if(_mainHero->getEP() >= _mainHero->getMaxEP()){  //已经满了
		return;
	}

	unsigned int temp = _mainHero->getEP() + ConstHelper(FIGHT_EP_ADD_VALUE);
	if(temp >= _mainHero->getMaxEP()){
		_mainHero->setEP(_mainHero->getMaxEP());
	}else{
		_mainHero->setEP(temp);
	}

	//能量条改变
	m_pHeroHeadUI->setEPPercent(_mainHero->getEP(), _mainHero->getMaxEP());
}

void CBaseFightScene::activeEnemyEvent(const string& stEvent, CCObject* pstEventObj){
	CEnemy* pstMonster = dynamic_cast<CEnemy*>(pstEventObj);
	//加入舞台数组
	if(_stageEntities->indexOfObject(pstMonster) == UINT_MAX){
		_stageEntities->addObject((CCObject*)pstMonster);
	}
}

void CBaseFightScene::handleSceneBuffDisappearEvent(const string& stEvent, CCObject* pstEventObj){
	CSceneBuff* pstBuff = dynamic_cast<CSceneBuff*>(pstEventObj);

	bool isRemove = true;
	if(pstBuff->m_stCfg.bufftype() == BUFF_TYPE_ONCE || pstBuff->m_stCfg.bufftype() == BUFF_TYPE_WEAPON){
		isRemove = false;
	}
	removeFloorBuff(pstBuff, isRemove);
}

void CBaseFightScene::handleSceneTrapDisappearEvent(const string& stEvent, CCObject* pstEventObj){
	CSceneTrap* pstTrap = dynamic_cast<CSceneTrap*>(pstEventObj);

	//陷阱挂在人物身上的buff统一不移除
	removeTrap(pstTrap, false);
}

void CBaseFightScene::handleSceneTrapHitKeyEvent(const string& stEvent, CCObject* pstEventObj){
	CSceneTrap* pstTrap = dynamic_cast<CSceneTrap*>(pstEventObj);

	CCObject* pEntityObj;
	CCARRAY_FOREACH(_stageEntities, pEntityObj){
		bool isSelf = false;
		CQStageEntity* pstTarget = dynamic_cast<CQStageEntity*>(pEntityObj);
		if(pstTarget->m_uID == _mainHero->m_uID){
			isSelf = true;
		}

		if(BUFF_CFG_MASTER->GetBuffCfgByID(pstTrap->m_stCfg.buffid())->addordec() == BUFF_DEC_EFF && pstTarget->IsImmune()){
			continue;
		}

		if(CInstanceUtils::IsTargetType(pstTrap->getTargetType(), 
			pstTrap->getOwnerParty(),
			pstTarget->getRoleParty(), 
			isSelf)){
				if(CInstanceUtils::IsContoursCollide(pstTrap, pstTarget)){
					if(pstTrap->getDamageCnt() > 0 && !pstTarget->CheckHasBuff(pstTrap->m_stCfg.buffid(), pstTrap->m_uID)){
						pstTarget->AddBuff(pstTrap->m_stCfg.buffid(), pstTrap->m_uID);
						unsigned int uPreCnt = pstTrap->getDamageCnt();
						pstTrap->setDamageCnt(uPreCnt - 1);
					}
				}else{  //出了陷阱范围就重置陷阱的伤害次数
					pstTrap->resetDamageCnt();
				}
		}

	}
}

void CBaseFightScene::handleSkillClickEvent(const string& stEvent, CCObject* pstEventObj) {
	m_pUiLayer->removeAllFireBtn();

	//技能释放
	if (!_mainHero->IsAttackable()){
		//主角处于不可攻击别人的状态，或者正在攻击，或者中了某个buff
		return;
	}
	CCString* strValue = (CCString*)pstEventObj;
	int skillIdx = strValue->intValue();

	if(skillIdx == 2){
		_mainHero->BackDash();
	}else{
		const CQSkillCell* pstSkill = _mainHero->GetSkillByIdx(skillIdx);
		if (!pstSkill){
			return;
		}

		if (pstSkill->m_stCfg.triggertype() != SKILL_TRIGGER_ON_ME){
			if (needTurnBackForSkill()){
				_mainHero->setScaleX(-1 * _mainHero->getScaleX());
			}
		}
 		_mainHero->CastSkill(skillIdx);

		if(skillIdx == 1){  //进化技能
			//m_uEvolutionCnt++;
		}

		if (skillIdx == 0){
			static unsigned int uWeaponID = 1;
			//_mainHero->SwitchWeapon(uWeaponID % 20);

			uWeaponID++;
		}		
	}
}

void CBaseFightScene::handleSkillBtnStateEvent(const string& stEvent, CCObject* pstEventObj){
	CCString* strValue = (CCString*)pstEventObj;
	int isInSkill = strValue->intValue();

	if(isInSkill){  //正在释放技能
		m_trickBtn->setIsEnable(false);
		m_skillBtn->setIsEnable(false);
		m_dodgeBtn->setIsEnable(false);
	}else{
		m_trickBtn->setIsEnable(true);
		m_skillBtn->setIsEnable(true);
		m_dodgeBtn->setIsEnable(true);
	}
}

void CBaseFightScene::handleHeroDashCompleteEvent(const string& stEvent, CCObject* pstEventObj){
	_mainHero->AddBuff(ConstHelper(FIGHT_DASH_INVINCIBLE_BUFF_ID), _mainHero->m_uID, true);		//复活加上无敌buff
}

void CBaseFightScene::handleHeroWeaponChange(const string& stEvent, CCObject* pstEventObj){
	if(_mainHero == NULL || _mainHero->checkIsDied()){
		return;
	}

	if (!_mainHero->IsActionInterruptable()){
		//不能换枪，要恢复换枪前状态
		//m_pArrLayout->RestorePreIdx();
		return;
	}

	if(m_bIsGameOver || m_bIsPreOver){   //已经结束
		return;
	}

	//资源加载
	loadHeroResByShowID(P_USER_OBJ->getUserInfo()->showID, P_USER_OBJ->getUserInfo()->weaponCfgID, false);
	loadWeaponeResByCfgID(P_USER_OBJ->getUserInfo()->weaponCfgID, false);

	//CCLOG("change Weapon the cfg id is %d", P_USER_OBJ->getUserInfo()->weaponCfgID);

	_mainHero->SwitchWeapon(P_USER_OBJ->getUserInfo()->weaponCfgID);
	_mainHero->setBulletTime(P_FIGHT_OBJ->getCurBulletVal());

	int skillID = (int)WEAPON_CFG_MASTER->GetWeaponCfgByID(P_USER_OBJ->getUserInfo()->weaponCfgID)->weaponsuperskill();
	m_trickBtn->updateSkillIcon(skillID);

	POP_TIP_LAYER_MASTER->ClearAllTips();
	cancelshowBulletOutTip();

	if(m_bIsMainHeroShooting){
		mainHeroShoot();
	}
}

void CBaseFightScene::changeTempWeapon(unsigned int weaponBuffID){
	if(_mainHero == NULL){
		return;
	}

	if (!_mainHero->IsActionInterruptable()){
		//m_pArrLayout->setSelectByGUID(P_USER_OBJ->getUserInfo()->weapon.weaponguid());
		return;
	}


	const WeaponBuffCfg* stCfg = WEAPON_BUFF_CFG->GetWeaponBuffCfgByID(weaponBuffID);
	//CCLOG("change Weapon the cfg id is %d", weaponBuffID);

	vmsg::CSWeapon stTempWeapon;
	stTempWeapon.set_cfgid(stCfg->weaponcfgid());
	stTempWeapon.set_weaponguid(TEMP_WEAPON_GUID);
	stTempWeapon.set_level(stCfg->weaponlv());
	stTempWeapon.set_exp(0);
	stTempWeapon.set_totalexp(0);
	stTempWeapon.set_quality(stCfg->weaponcolor()); //TODO:修改

	P_USER_OBJ->updateWeaponInfo(stTempWeapon);

	_mainHero->SwitchWeapon(P_USER_OBJ->getUserInfo()->weaponCfgID);

	int skillID = (int)WEAPON_CFG_MASTER->GetWeaponCfgByID(P_USER_OBJ->getUserInfo()->weaponCfgID)->weaponsuperskill();
	m_trickBtn->updateSkillIcon(skillID);

	m_pArrLayout->showTempWeapon(stTempWeapon);
}

void CBaseFightScene::weaponUpStar(unsigned int buffType){
	if(_mainHero == NULL){
		return;
	}
	unsigned int preWeaponCfgID = P_USER_OBJ->getUserInfo()->weaponCfgID;

	//再次加载次资源
	loadWeaponStarUpRes(preWeaponCfgID, false);

	unsigned int uLv = P_USER_OBJ->getUserInfo()->weapon.level();
	unsigned int uQuality = P_USER_OBJ->getUserInfo()->weapon.quality();
	unsigned int uWeaponID = WEAPON_CFG_MASTER->GetWeaponCfgByID(preWeaponCfgID)->weaponid();
	unsigned int uStar = WEAPON_CFG_MASTER->GetWeaponCfgByID(preWeaponCfgID)->initstar();
	unsigned int uMaxStar = WEAPON_CFG_MASTER->GetWeaponCfgByID(preWeaponCfgID)->maxstar();
	if(buffType == BUFF_TYPE_WEAPON_UP){  //升一星
		if(uStar >= uMaxStar){  //已满星
			return;
		}

		uStar = uStar + 1;
	}else if(buffType == BUFF_TYPE_WEAPON_MAX){  //直接满星 
		if(uStar >= uMaxStar){  //已满星
			return;
		}

		uStar = uMaxStar;
	}else{
		return;
	}

	WeaponCfg stCfg;
	WEAPON_CFG_MASTER->GetWeaponCfgByStarAndStep(uWeaponID, uStar, 0, stCfg);
	vmsg::CSWeapon stTempWeapon;
	stTempWeapon.set_cfgid(stCfg.cfgid());
	stTempWeapon.set_weaponguid(TEMP_WEAPON_GUID);
	stTempWeapon.set_level(uLv);
	stTempWeapon.set_exp(0);
	stTempWeapon.set_totalexp(0);
	stTempWeapon.set_quality(uQuality); 

	P_USER_OBJ->updateWeaponInfo(stTempWeapon);

	_mainHero->SwitchWeapon(P_USER_OBJ->getUserInfo()->weaponCfgID);

	int skillID = (int)stCfg.weaponsuperskill();
	m_trickBtn->updateSkillIcon(skillID);

	m_pArrLayout->showTempWeapon(stTempWeapon);
}

void CBaseFightScene::HandleBulletKeyFrameEvent(const string& stEvent, CCObject* pstEventObj){
	string stPureEventStr = CQEventDispatcher::GetEventString(stEvent);
	if (stPureEventStr == CBullet::BULLET_HIT_KEY){
		//打击帧，计算哪些角色被撞击了
		CBullet* pstBullet = dynamic_cast<CBullet*>(pstEventObj);
		CCObject* pEnimyObj;
		unsigned int uRoleParty = ROLE_PARTY_HOSTILE;
		if(pstBullet->getParty() == ROLE_PARTY_FRIEND){
			uRoleParty = ROLE_PARTY_HOSTILE;
		}else if(pstBullet->getParty() == ROLE_PARTY_HOSTILE){
			uRoleParty = ROLE_PARTY_FRIEND;
		}
		CCARRAY_FOREACH(_stageEntities, pEnimyObj){
			CQStageEntity* pstTarget = dynamic_cast<CQStageEntity*>(pEnimyObj);		
			if(!checkPosInWalkable(pstTarget->getPosition())){  //当怪物在阶段外，不可被攻击
				continue;
			}

			if (CInstanceUtils::IsBulletHitEntity(pstBullet, uRoleParty,  pstTarget)){				
				//范围扩散型的，这个范围以内所有人都受伤，不会因为撞到人就退出
				CInstanceUtils::HandleBulletHitTargetEffect(pstBullet, pstTarget, this, 0);													
			}
		}

		CCObject* pstItemObj;
		CCARRAY_FOREACH(_sceneItems, pstItemObj){
			CSceneItem* pstItem = dynamic_cast<CSceneItem*>(pstItemObj);	
			if (!pstItem && !checkPosInWalkable(pstItem->getPosition())){
				continue;
			}
			if (CInstanceUtils::IsBulletHitItem(pstBullet, pstItem)){
				CInstanceUtils::HandleBulletHitItemEffect(pstBullet, pstItem, 0);
			}
		}
	}

}

void CBaseFightScene::handleAniEffCountEvent(const string& stEvent, CCObject* pstEventObj){
	m_uCurAniTotalNum++;

	//CCLOG("[handleAniEffCountEvent]::now the animation count is %u", m_uCurAniTotalNum);
}

void CBaseFightScene::handleEnemySkillActionEvent(const string& stEvent, CCObject* pstEventObj){
	m_stTargetPos = _mainHero->GetFootPos();
}

void CBaseFightScene::handleEnemyDieMovementEvent(const string& stEvent, CCObject* pstEventObj){

}

void CBaseFightScene::handleEnemyDashCompleteEvent(const string& stEvent, CCObject* pstEventObj){
	CQStageEntity* pstEntity = dynamic_cast<CQStageEntity*>(pstEventObj);

	//冲撞完成后从舞台移动角色数组中移除
	_moveEntities->removeObject(pstEntity);
}

void CBaseFightScene::handleEnemyDieEffectEvent(const string& stEvent, CCObject* pstEventObj){
	CQStageEntity* pstEntity = dynamic_cast<CQStageEntity*>(pstEventObj);

	if (pstEntity->getDieVoice().length() && strcmp(pstEntity->getDieVoice().c_str(), "null")){
		AudioManager::playEffect(pstEntity->getDieVoice().c_str());
	}
}

void CBaseFightScene::handleTryKickEvent(const string& stEvent, CCObject* pstEventObj){
	string stPureEventStr = CQEventDispatcher::GetEventString(stEvent);
	if (stPureEventStr == CMainHero::TRY_KICK_ENIMY_EVENT){
		CInstanceUtils::HandleTryKickEvent(this, dynamic_cast<CMainHero*>(pstEventObj));	
	}
}

void CBaseFightScene::handleKickEvent(const string& stEvent, CCObject* pstEventObj){
	string stPureEventStr = CQEventDispatcher::GetEventString(stEvent);
	if (stPureEventStr == CQStageEntity::KICK_EVENT){
		CInstanceUtils::HandleKickEvent(this, dynamic_cast<CMainHero*>(pstEventObj));	
	}
}

void CBaseFightScene::hanldeWinCompleteFun(const string& stEvent, CCObject* pstEventObj){
	
}

void CBaseFightScene::handleSceneItemFallEvent(const string& stEvent, CCObject* pstEventObj){

}

void CBaseFightScene::handleSceneItemDisappearEvent(const string& stEvent, CCObject* pstEventObj){

}

void CBaseFightScene::handleWarningCompleteEvent(const string& stEvent, CCObject* pstEventObj){
	if(m_bIsPreOver){
		return;
	}

	CSkillEffLayer* pstSkill = dynamic_cast<CSkillEffLayer*>(pstEventObj);
	if(pstSkill == NULL || pstSkill->getOwnerEntity() == NULL){
		return;
	}

	if(pstSkill->m_stCfg.triggertype() == SKILL_TRIGGER_RAND_DIRECT_ON_GROUND){  //在地上随机出现固定数量特效，其中一个固定为主角位置
		CInstanceUtils::SkillRandOnGround(pstSkill->getOwnerEntity(), pstSkill, pstSkill->getPosition(), this);
	}else if(pstSkill->m_stCfg.triggertype() == SKILL_TRIGGER_RAND_FLY_TO_GROUND){  //随机飞向几个固定点，其中一个固定为主角位置
		CInstanceUtils::SkillRandOnGround(pstSkill->getOwnerEntity(), pstSkill, pstSkill->getPosition(), this);
	}

	pstSkill->removeFromParentAndCleanup(true);
}

void CBaseFightScene::handleHitTimeOutEvent(const string& stEvent, CCObject* pstEventObj){
	if(m_pHitLayout->getParent()){
		m_pHitLayout->reset();
		m_pHitLayout->removeFromParentAndCleanup(true);
	}
}

/************************************************************************/
/*                         事件处理   END                               */
/************************************************************************/

bool CBaseFightScene::needTurnBack(){	
	bool bHasEnimyBehindme = false;
	CCObject* pEnimyObj;
	float fClosestForwardEnimyDist = 10000;
	float fClosestBackwardEnimyDist = 10000;
	CCARRAY_FOREACH(_stageEntities, pEnimyObj){
		CQStageEntity* pstTarget = dynamic_cast<CQStageEntity*>(pEnimyObj);			
		if (pstTarget->getRoleParty() == _mainHero->getRoleParty()
			|| pstTarget->IsDead()
			|| !pstTarget->getArmature()){				
				continue;												
		}

		if ((pstTarget->getPositionX() > _mainHero->getPositionX() && _mainHero->getScaleX() > 0)
			|| (pstTarget->getPositionX() < _mainHero->getPositionX() && _mainHero->getScaleX() < 0)){
				if ((fabs(_mainHero->getPositionY() - pstTarget->getPositionY()) < _mainHero->getBulletDamageRangeHight()
					&& fabs(_mainHero->getPositionX() - pstTarget->getPositionX()) < _mainHero->getBulletDamageRangeWidth() / 2)
					|| (fabs(_mainHero->getPositionY() - pstTarget->getPositionY()) < _mainHero->getBulletDamageRangeHight() * 2
						&& (pstTarget->getRoleType() == ROLE_TYPE_SENIOR_BOSS 
							|| pstTarget->getRoleType() == ROLE_TYPE_JUNIOR_BOSS
							|| pstTarget->getRoleType() == ROLE_TYPE_ELITE))){
						return false;
				}	
				if (fabs(_mainHero->getPositionX() - pstTarget->getPositionX()) < fClosestForwardEnimyDist){
					fClosestForwardEnimyDist = fabs(_mainHero->getPositionX() - pstTarget->getPositionX());
				}
		}else{
			if (fabs(_mainHero->getPositionX() - pstTarget->getPositionX()) < fClosestBackwardEnimyDist){
				fClosestBackwardEnimyDist = fabs(_mainHero->getPositionX() - pstTarget->getPositionX());
			}
			
		}
			
	}

	if (fClosestForwardEnimyDist <= fClosestBackwardEnimyDist){
		return false;
	}else{
		return true;
	}
	
}

bool CBaseFightScene::needTurnBackForSkill(){	
	bool bHasEnimyBehindme = false;
	CCObject* pEnimyObj;
	float fClosestForwardEnimyDist = 10000;
	float fClosestBackwardEnimyDist = 10000;
	CCARRAY_FOREACH(_stageEntities, pEnimyObj){
		CQStageEntity* pstTarget = dynamic_cast<CQStageEntity*>(pEnimyObj);			
		if (pstTarget->getRoleParty() == _mainHero->getRoleParty()
			|| pstTarget->IsDead()
			|| !pstTarget->getArmature()){				
				continue;												
		}

		if ((pstTarget->getPositionX() > _mainHero->getPositionX() && _mainHero->getScaleX() > 0)
			|| (pstTarget->getPositionX() < _mainHero->getPositionX() && _mainHero->getScaleX() < 0)){
				if ((fabs(_mainHero->getPositionY() - pstTarget->getPositionY()) < _mainHero->getBulletDamageRangeHight() && fabs(_mainHero->getPositionX() - pstTarget->getPositionX()) < _mainHero->getBulletDamageRangeWidth() / 2)
					|| (pstTarget->getRoleType() == ROLE_TYPE_SENIOR_BOSS 
					|| pstTarget->getRoleType() == ROLE_TYPE_JUNIOR_BOSS
					|| pstTarget->getRoleType() == ROLE_TYPE_ELITE)){
						return false;
				}	
				if (fabs(_mainHero->getPositionX() - pstTarget->getPositionX()) < fClosestForwardEnimyDist){
					fClosestForwardEnimyDist = fabs(_mainHero->getPositionX() - pstTarget->getPositionX());
				}
		}else{
			if (fabs(_mainHero->getPositionX() - pstTarget->getPositionX()) < fClosestBackwardEnimyDist){
				fClosestBackwardEnimyDist = fabs(_mainHero->getPositionX() - pstTarget->getPositionX());
			}

		}

	}

	if (fClosestForwardEnimyDist <= fClosestBackwardEnimyDist){
		return false;
	}else{
		return true;
	}

}

void CBaseFightScene::handleGameOver(){

}

void CBaseFightScene::clearAllSceneItems(){
	CCObject* pObject;
	CSceneItem* pstItem;
	CCARRAY_FOREACH(_sceneItems, pObject){
		pstItem = dynamic_cast<CSceneItem*>(pObject);
		if(pstItem->getItemType() == ITEM_TYPE_BLOCK_DESTROY || pstItem->getItemType() == ITEM_TYPE_BLOCK_UNDESTROY){  //阻挡
			removeBlocksInSize(pstItem->getBlockSize(), (CCPoint)pstItem->getPosition(), true);
		}
		pstItem->removeFromParentAndCleanup(true);
	}

	_sceneItems->removeAllObjects();
	updateMapBlocks();
}

void CBaseFightScene::clearAllBuffs(){
	CCObject* pObject;
	CSceneBuff* pstBuff;
	CCARRAY_FOREACH(_sceneBuffs, pObject){
		pstBuff = dynamic_cast<CSceneBuff*>(pObject);
		CCObject* pEntityObj;
		CCARRAY_FOREACH(_stageEntities, pEntityObj){
			CQStageEntity* pstTarget = dynamic_cast<CQStageEntity*>(pEntityObj);
			//去掉所有地表buff相关
			if(pstBuff->m_stCfg.bufftype() != BUFF_TYPE_WEAPON && pstTarget->CheckHasBuff(pstBuff->getBuffID(), pstBuff->m_uID)){
				pstTarget->RemoveBuff(pstBuff->getBuffID(), pstBuff->m_uID);
			}
		}
		pstBuff->removeFromParentAndCleanup(true);
	}

	_sceneBuffs->removeAllObjects();
}

void CBaseFightScene::clearAllTraps(){
	CCObject* pObject;
	CSceneTrap* pstTrap;
	CCARRAY_FOREACH(_sceneTraps, pObject){
		pstTrap = dynamic_cast<CSceneTrap*>(pObject);
		pstTrap->removeFromParentAndCleanup(true);
	}

	_sceneTraps->removeAllObjects();
}

void CBaseFightScene::clearAllSkills(){
	CCObject* pstObj;
	CCARRAY_FOREACH(_skills, pstObj){
		CCNode* pNode = dynamic_cast<CCNode*>(pstObj);
		pNode->removeFromParentAndCleanup(true);
	}

	_skills->removeAllObjects();
}


void CBaseFightScene::clearAllBullet(){
	CCObject* pstObj;
	CCARRAY_FOREACH(_hero_bullets, pstObj){
		CCNode* pNode = dynamic_cast<CCNode*>(pstObj);
		pNode->removeFromParentAndCleanup(true);
	}
	
	_hero_bullets->removeAllObjects();
}

void CBaseFightScene::clearAllArrows(){
	CCObject* pstObj;
	CCARRAY_FOREACH(m_stLeftArrowArr, pstObj){
		CCNode* pNode = dynamic_cast<CCNode*>(pstObj);
		pNode->removeFromParentAndCleanup(true);
	}

	CCARRAY_FOREACH(m_stRightArrowArr, pstObj){
		CCNode* pNode = dynamic_cast<CCNode*>(pstObj);
		pNode->removeFromParentAndCleanup(true);
	}

	m_stLeftArrowArr->removeAllObjects();
	m_stRightArrowArr->removeAllObjects();
}

void CBaseFightScene::clearAllMonsters(){
	CCObject* pstObj;
	CCARRAY_FOREACH(_stageEntities, pstObj){
		CQStageEntity* pEntity = dynamic_cast<CQStageEntity*>(pstObj);
		if(pEntity->getRoleParty() == ROLE_PARTY_HOSTILE){  //怪物
			pEntity->removeFromParentAndCleanup(true);
			_stageEntities->removeObject(pstObj);
		}
	}
}

void CBaseFightScene::forceOhterMonstersDie(){
	CCObject* pstObj;
	CCARRAY_FOREACH(_stageEntities, pstObj){
		CQStageEntity* pEntity = dynamic_cast<CQStageEntity*>(pstObj);
		if(pEntity->getRoleParty() == ROLE_PARTY_HOSTILE){  //怪物
			if(!pEntity->IsDead() && pEntity->getMyState() != kActionStateDying && pEntity->getMyState() != kActionStateDie){
				pEntity->setIsPlayDieVoice(false);		//不播放死亡音效
				pEntity->setHP(0);
				pEntity->Die();
			}
		}
	}
}

unsigned int CBaseFightScene::getMonsterNumOnStage(){
	int iNum = 0;
	CCObject* pstObj;
	CCARRAY_FOREACH(_stageEntities, pstObj){
		CQStageEntity* pstEntity = dynamic_cast<CQStageEntity*>(pstObj);
		if(typeid(*pstEntity) == typeid(CEnemy) && pstEntity->getRoleParty() == ROLE_PARTY_HOSTILE){  //敌方
			iNum++;
		}
	}

	return iNum;
}

const CCRect CBaseFightScene::getPlayerVisibleSize(){
	//float xx = pstEnity->GetFootPos().x - abs(_sceneForegound->getPositionX());
	float xx = _sceneForegound->getPositionX();
	CCSize stSize = CCDirector::sharedDirector()->getVisibleSize();
	
	return CCRectMake(-xx, 0, stSize.width, getWalkHight());
}


CCRect CBaseFightScene::getStageRect() const{
	return walkableRect;
}

void CBaseFightScene::setAttackOpBtnVisible(bool bIsVisible){
	m_pSkillLayout->setVisible(bIsVisible);
	fireBtn->setTouchEnabled(bIsVisible);
	m_trickBtn->setBtnEnable(bIsVisible);
	m_skillBtn->setBtnEnable(bIsVisible);
	m_dodgeBtn->setBtnEnable(bIsVisible);
	pauseBtn->setEnabled(bIsVisible);
	P_FIGHT_CTRL->getMedicalLayout()->setEnabled(bIsVisible);

	if(m_pChangeWeaponBtn->isEnabled()){
		m_pChangeWeaponBtn->setTouchEnabled(bIsVisible);
	}

	if(bIsVisible){
		_hudLayer->Active();
	}else{
		_hudLayer->Inactive();
	}
	_hudLayer->setVisible(bIsVisible);

	if(_mainHero->getIsBulletLimit()){
		m_pArrLayout->setEnabled(bIsVisible);
	}
}

bool CBaseFightScene::checkMonsterOnStage(CEnemyData* pstObj){
	unsigned int key = pstObj->key;

	CCObject* pObject;
	CQStageEntity* pstEntity;
	CCARRAY_FOREACH(_stageEntities, pObject){
		pstEntity = dynamic_cast<CQStageEntity*>(pObject);
		if(pstEntity->getRoleParty() == ROLE_PARTY_HOSTILE){//敌方
			CEnemy* pstEnemy = dynamic_cast<CEnemy*>(pObject);
			CEnemyData* pstEnemyData = pstEnemy->getMonsterObj();
			if(pstEnemyData->key == key){
				return true;
			}
		}
	}

	return false;
}

void CBaseFightScene::updateHit(){
	if(m_pHitLayout->getParent() == NULL){
		m_pUiLayer->addChild(m_pHitLayout);
	}

	unsigned int uNum = m_pHitLayout->getPreHitNum() + 1;
	m_pHitLayout->setHitNum(uNum);
}

void CBaseFightScene::removeEntityBlock(CQStageEntity* pstEntity){
	if(pstEntity && pstEntity->getMyState() == kActionStateDie){  
		if(pstEntity->getIsBlock()){  //去掉阻挡
			removeBlocksInSize(pstEntity->getBlockSize(), (CCPoint)pstEntity->getPosition(), true);
			updateMapBlocks();
		}
	}
}

void CBaseFightScene::brushFromCar(CCObject* pstObject){
	CEnemyData* pstMonsterObj = dynamic_cast<CEnemyData*>(pstObject);

	CCFiniteTimeAction* brushOneFun = CCCallFuncO::create(this, callfuncO_selector(CBaseFightScene::brushFromCarOne), pstMonsterObj);
	this->runAction(CCRepeatForever::create(CCSequence::create(CCDelayTime::create(0.2f), brushOneFun, NULL)));
}

void CBaseFightScene::brushFromCarOne(CCObject* pstObject){
	CEnemyData* pstMonsterObj = dynamic_cast<CEnemyData*>(pstObject);
	if(pstMonsterObj == NULL || pstMonsterObj->num <= 0){
		this->stopAllActions();
		return;
	}

	int nowNum = pstMonsterObj->num;
	CEnemy* pstMonster = initOneMonster(pstMonsterObj);
	if(pstMonster != NULL){
		pstMonster->LoadActionRes();
		int iNewZorder = CInstanceUtils::GetZOrderByType(ROLE_Z_ORDER, pstMonster->getPositionY());
		_sceneForegound->addChild(pstMonster, iNewZorder);
		pstMonster->Active();

		if(!pstMonster->getBlockSize().equals(CCSizeZero)){
			getBlocksInSize(pstMonster->getBlockSize(), (CCPoint)pstMonster->getPosition(), true);
			updateMapBlocks();
		}
	}

	pstMonsterObj->num = (--nowNum);
}

void CBaseFightScene::carMoveEnd(CCObject* pstObject){
	CCArmature* pCarArmature = dynamic_cast<CCArmature*>(pstObject);
	if(pCarArmature != NULL && pCarArmature->getParent() != NULL){
		pCarArmature->removeFromParentAndCleanup(true);
	}
}

bool CBaseFightScene::checkPosInWalkable(CCPoint stPos){
	//在可行走区域加上一排格子
	CCSize realSize = walkableRect.size + CCSize(_gridW*2, _gridH*2);
	CCRect realRect = CCRectMake(walkableRect.getMinX() - _gridW, walkableRect.getMinY() - _gridH, realSize.width, realSize.height);
	return realRect.containsPoint(stPos);
}

CQStageEntity* CBaseFightScene::getFriend(){
	CCObject* pObject;
	CQStageEntity* pstEntity;
	CCARRAY_FOREACH(_stageEntities, pObject){
		pstEntity = dynamic_cast<CQStageEntity*>(pObject);
		if(pstEntity->getRoleParty() == ROLE_PARTY_FRIEND && pstEntity->m_uID != _mainHero->m_uID){//助战好友
			return pstEntity;
		}
	}

	return NULL;
}

void CBaseFightScene::handleArmatureRelease(CCArmature* armature, MovementEventType type, const char* name){
	if (type == COMPLETE){
		armature->removeFromParentAndCleanup(true);
	}
}

void CBaseFightScene::doHeroDieEvent(){

	if(_mainHero->GetIsControlledByAI()){  //自动战斗中
		doCancelHeroAutoFight();
	}

	pauseFight();		//暂停战斗

	CFightHeroReliveLayer* pReliveLayout = P_FIGHT_CTRL->getReliveLayout();
	pReliveLayout->AddEventListener(this, CFightHeroReliveLayer::FIGHT_HERO_RELIVE_EVENT, eventListner_selector(CBaseFightScene::handleHeroReliveDoneEvent));
	pReliveLayout->AddEventListener(this, CFightHeroReliveLayer::FIGHT_HERO_CANCEL_RELIVE_EVENT, eventListner_selector(CBaseFightScene::handleHeroCancelReliveEvent));

	if(pReliveLayout->getParent() != NULL) {
		pReliveLayout->removeFromParent();
	}

	pReliveLayout->startCountDown();
	pReliveLayout->setZOrder(10);
	m_pUiLayer->addWidget(pReliveLayout);
}

void CBaseFightScene::handleHeroReliveDoneEvent(const string& stEvent, CCObject* pstEventObj){
	CFightHeroReliveLayer* pReliveLayout = P_FIGHT_CTRL->getReliveLayout();
	pReliveLayout->RemoveEventListener(this, CFightHeroReliveLayer::FIGHT_HERO_RELIVE_EVENT, eventListner_selector(CBaseFightScene::handleHeroReliveDoneEvent));
	pReliveLayout->RemoveEventListener(this, CFightHeroReliveLayer::FIGHT_HERO_CANCEL_RELIVE_EVENT, eventListner_selector(CBaseFightScene::handleHeroCancelReliveEvent));

	//主角复活
	_mainHero->setHP(_mainHero->getMaxHP());
	_mainHero->setEP(0);
	_mainHero->Idle();
	_mainHero->setScaleX(1.0f); //面向右
	_mainHero->setShadowVisiable(true);
	m_pHeroHeadUI->setHPPercent(_mainHero->getHP(), _mainHero->getMaxHP());
	m_pHeroHeadUI->setEPPercent(_mainHero->getEP(), _mainHero->getMaxEP());
	_mainHero->AddBuff(ConstHelper(FIGHT_RELIVE_INVINCIBLE_BUFF_ID), _mainHero->m_uID, true);		//补充弹药加上无敌buff

	m_uUseReliveCnt++;

	//弹药补满，同时加上无敌buff  
	fillAllWeaponBullet(false);

	resumeFight();
}

void CBaseFightScene::handleHeroCancelReliveEvent(const string& stEvent, CCObject* pstEventObj){
	CFightHeroReliveLayer* pReliveLayout = P_FIGHT_CTRL->getReliveLayout();
	pReliveLayout->RemoveEventListener(this, CFightHeroReliveLayer::FIGHT_HERO_RELIVE_EVENT, eventListner_selector(CBaseFightScene::handleHeroReliveDoneEvent));
	pReliveLayout->RemoveEventListener(this, CFightHeroReliveLayer::FIGHT_HERO_CANCEL_RELIVE_EVENT, eventListner_selector(CBaseFightScene::handleHeroCancelReliveEvent));

	doHeroDieFail();
}

void CBaseFightScene::doHeroDieFail(){

}

void CBaseFightScene::doCancelHeroAutoFight(){
	removeAutoFightLayer();
	_mainHero->SetIsControlledByAI(false);
	m_pAutoBtn->setVisible(true);
	m_pAutoBtn->setTouchEnabled(true);
}

void CBaseFightScene::doAllBulletOut(){
	if(_mainHero->checkIsDied() || m_bIsWalkNextStage){			//正在通过阶段时不检测
		return;
	}

	if(_mainHero->GetIsControlledByAI()){  //自动战斗中
		doCancelHeroAutoFight();
	}

	mainHeroCeaseShoot();
	_mainHero->ForceIdle();

	//判断是否有弹药箱，有就自动补充弹药，没有就弹出弹药补充面板
	unsigned int uFillBulletCfgID = vmsg::ITEM_ARMO_ID;		//弹药箱ID
	unsigned int uFillBulletNum = P_BAG_CTRL->getItemCnt(uFillBulletCfgID);

	if(uFillBulletNum > 0){  //有弹药箱
		unsigned int uBulletCfgID = vmsg::ITEM_ARMO_ID;		//弹药箱ID 
		unsigned int uBagIdx = P_BAG_CTRL->getIdxByItemCfgID(uBulletCfgID);
		P_FIGHT_CTRL->getFightProxy()->sendBuyBulletRqst(1, uBulletCfgID, uBagIdx);

		//前台先更新弹药箱数量
		uFillBulletNum--;
		P_BAG_CTRL->updateItemCnt(uFillBulletCfgID, uFillBulletNum);

		fillAllWeaponBullet();

		m_uUseBulletCnt++;

		//射击按钮状态判断
		if(fireBtn->isBright()){
			mainHeroShoot();
		}
	}else{
		pauseFight();		//暂停战斗

		CFillBulletPanel* pFillPanel = P_LEVEL_CTL->getFillBulletPanel();
		pFillPanel->AddEventListener(this, CFillBulletPanel::FILL_BULLET_SUCCESS_EVENT, eventListner_selector(CBaseFightScene::handleFillBulletEvent));
		pFillPanel->AddEventListener(this, CFillBulletPanel::CANCEL_FILL_BULLET_EVENT, eventListner_selector(CBaseFightScene::handleCancelFillBulletEvent));
		pFillPanel->openWithCallFun(this, callfunc_selector(CBaseFightScene::resumeFight));
	}
}

void CBaseFightScene::handleFillBulletEvent(const string& stEvent, CCObject* pstEventObj){
	CFillBulletPanel* pFillPanel = P_LEVEL_CTL->getFillBulletPanel();
	pFillPanel->RemoveEventListener(this, CFillBulletPanel::FILL_BULLET_SUCCESS_EVENT, eventListner_selector(CBaseFightScene::handleFillBulletEvent));
	pFillPanel->RemoveEventListener(this, CFillBulletPanel::CANCEL_FILL_BULLET_EVENT, eventListner_selector(CBaseFightScene::handleCancelFillBulletEvent));

	//弹药补满  
	fillAllWeaponBullet();

	m_uUseBulletCnt++;
}

void CBaseFightScene::handleCancelFillBulletEvent(const string& stEvent, CCObject* pstEventObj){
	CFillBulletPanel* pFillPanel = P_LEVEL_CTL->getFillBulletPanel();
	pFillPanel->RemoveEventListener(this, CFillBulletPanel::FILL_BULLET_SUCCESS_EVENT, eventListner_selector(CBaseFightScene::handleFillBulletEvent));
	pFillPanel->RemoveEventListener(this, CFillBulletPanel::CANCEL_FILL_BULLET_EVENT, eventListner_selector(CBaseFightScene::handleCancelFillBulletEvent));

	//失败
	doHeroDieFail();
}

void CBaseFightScene::fillAllWeaponBullet(bool bIsInvincible){
	if(m_pArrLayout->isEnabled() && _mainHero->getIsBulletLimit()){
		m_pArrLayout->fillAllWeaponBullet();

		const WeaponCfg* stCfg = WEAPON_CFG_MASTER->GetWeaponCfgByID(P_USER_OBJ->getUserInfo()->weaponCfgID);
		if(stCfg){
			_mainHero->setBulletTime(stCfg->bulletval()*1.0f/1000.0f);
		}

		if(bIsInvincible){
			_mainHero->AddBuff(ConstHelper(FIGHT_BULLET_INVINCIBLE_BUFF_ID), _mainHero->m_uID, true);		//补充弹药加上无敌buff
		}
	}
}

void CBaseFightScene::doMedicalBtnClick(){
	unsigned int uMedicalCfgID = ConstHelper(FIGHT_MEDICAL_CFG_DI);
	unsigned int uMedicalNum = P_BAG_CTRL->getItemCnt(uMedicalCfgID);
	unsigned int uBagIdx = P_BAG_CTRL->getIdxByItemCfgID(uMedicalCfgID);

	if(uMedicalNum <= 0){  //医药包数量不足
		doMedicalOut();
	}else{
		P_BAG_CTRL->sendConsumeItemRqst(vmsg::THING_TYPE_ITEM, uMedicalCfgID, uBagIdx, 1);

		//前台先更新医药箱数量
		uMedicalNum--;
		P_BAG_CTRL->updateItemCnt(uMedicalCfgID, uMedicalNum);
		P_FIGHT_CTRL->getMedicalLayout()->updateUI();
		doMedicalEff();
	}
}

void CBaseFightScene::doMedicalEff(){
	m_uUseMedicalCnt++;

	unsigned int uMaxHp = _mainHero->getMaxHP();
	unsigned int uAddHp = _mainHero->getMaxHP()*2/5;		//40%血量回复
	unsigned int uNowHp = _mainHero->getHP();
	unsigned int uHp = uNowHp + uAddHp;
	if(uHp > uMaxHp){
		uHp = uMaxHp;
	}
	_mainHero->setHP(uHp);
	m_pHeroHeadUI->setHPPercent(_mainHero->getHP(), _mainHero->getMaxHP());
}

void CBaseFightScene::doMedicalOut(){
	if(_mainHero->checkIsDied()){
		return;
	}

	pauseFight();		//暂停战斗

	CBuyMedicalPanel* pBuyMedicalPanel = P_LEVEL_CTL->getBuyMedicalPanel();
	pBuyMedicalPanel->AddEventListener(this, CBuyMedicalPanel::BUY_MEDICAL_SUCCESS_EVENT, eventListner_selector(CBaseFightScene::handleBuyMedicalEvent));
	pBuyMedicalPanel->AddEventListener(this, CBuyMedicalPanel::CANCEL_BUY_MEDICAL_EVENT, eventListner_selector(CBaseFightScene::handleCancelBuyMedicalEvent));
	pBuyMedicalPanel->openWithCallFun(this, callfunc_selector(CBaseFightScene::resumeFight));
}

void CBaseFightScene::handleBuyMedicalEvent(const string& stEvent, CCObject* pstEventObj){
	CBuyMedicalPanel* pBuyMedicalPanel = P_LEVEL_CTL->getBuyMedicalPanel();
	pBuyMedicalPanel->RemoveEventListener(this, CBuyMedicalPanel::BUY_MEDICAL_SUCCESS_EVENT, eventListner_selector(CBaseFightScene::handleBuyMedicalEvent));
	pBuyMedicalPanel->RemoveEventListener(this, CBuyMedicalPanel::CANCEL_BUY_MEDICAL_EVENT, eventListner_selector(CBaseFightScene::handleCancelBuyMedicalEvent));

	P_FIGHT_CTRL->getMedicalLayout()->updateUI();
}

void CBaseFightScene::handleCancelBuyMedicalEvent(const string& stEvent, CCObject* pstEventObj){
	CBuyMedicalPanel* pBuyMedicalPanel = P_LEVEL_CTL->getBuyMedicalPanel();
	pBuyMedicalPanel->RemoveEventListener(this, CBuyMedicalPanel::BUY_MEDICAL_SUCCESS_EVENT, eventListner_selector(CBaseFightScene::handleBuyMedicalEvent));
	pBuyMedicalPanel->RemoveEventListener(this, CBuyMedicalPanel::CANCEL_BUY_MEDICAL_EVENT, eventListner_selector(CBaseFightScene::handleCancelBuyMedicalEvent));
}

void CBaseFightScene::startShowBulletOutTip(){
	if(m_bIsShowBulletOut || m_pArrLayout->IsAllWeaponOutBullet()){
		return;
	}

	unschedule(schedule_selector(CBaseFightScene::showBulletOutTip));
	schedule(schedule_selector(CBaseFightScene::showBulletOutTip), 3.0f);

	POP_TIP_LAYER_MASTER->ShowTipStr(TextStr(TEXT_BULLET_OVER_TIP));

	m_bIsShowBulletOut = true;
}

void CBaseFightScene::cancelshowBulletOutTip(){
	unschedule(schedule_selector(CBaseFightScene::showBulletOutTip));

	m_bIsShowBulletOut = false;
}

void CBaseFightScene::showBulletOutTip(float dt){
	POP_TIP_LAYER_MASTER->ShowTipStr(TextStr(TEXT_BULLET_OVER_TIP));
}

