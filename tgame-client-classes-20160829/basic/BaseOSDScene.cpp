#include "BaseOSDScene.h"
#include "basic/HudLayer.h"
#include "CfgMaster/OSDCfgMaster.h"
#include "basic/SceneManager.h"
#include "game/fight/BgTextureNode.h"
#include "instance/InstanceEventUtils.h"
#include "HeroCfgMaster.h"
#include "ToolUtils.h"
#include "UserObj.h"
#include "CfgMaster/BulletCfgMaster.h"
#include "CfgMaster/WeaponCfgMaster.h"
#include "instance/InstanceUtils.h"


BaseOSDScene::BaseOSDScene()
:m_pRolesDict(NULL)
,m_stMainHero(NULL)
{
	
}

BaseOSDScene::~BaseOSDScene() {
	CCNotificationCenter::sharedNotificationCenter()->removeAllObservers(this);

	m_stRolesQueue.clear();
	m_pRolesDict->removeAllObjects();
	CC_SAFE_RELEASE_NULL(m_pRolesDict);

	CC_SAFE_RELEASE_NULL(m_stHeroRes);
}

bool BaseOSDScene::init() {
	bool bRet = false;

	do {
		CC_BREAK_IF(!CBaseMapScene::init());

		addLayer(LAYER_ID_GAME);
		addLayer(LAYER_ID_WIDGET);
		addLayer(LAYER_ID_HUD);
		addLayer(LAYER_ID_POP);
		addLayer(LAYER_ID_UI);
		addLayer(LAYER_ID_TOP);

		m_pRolesDict = CCDictionary::create();
		m_pRolesDict->retain();

		m_stHeroRes = CCArray::create();
		m_stHeroRes->retain();

		bRet = true;
		//setScale(720 / 480);
	} while(0);

	return bRet;
}

CMainHero* BaseOSDScene::createRole(UIN_t uin, string nick, unsigned int showID, unsigned int weaponCfgID, unsigned int uVIPLv) {
	//showID = showID == 0 ? 1 : showID;
	if(weaponCfgID == 0){
		weaponCfgID = 1;
	}

	std::string str = HERO_CFG_MASTER->GetHeroCfgByID(showID)->resname();
	string stBodyName;
	//棍哥修改一下，把weaponid加进来
	int iRet = CMainHero::GetResourceNames(weaponCfgID, showID, stBodyName);
	if (iRet){
		CCLog("[error]BaseOSDScene::createRole,no resource name is found for hero %d", showID);
		return NULL;
	}

	string bodyStr = HERO_RES_PATH + stBodyName + "/" + stBodyName + JSON_TYPE_NAME;
	
	addResToList(bodyStr);
	
	string footResStr = EFFECT_RES_PATH + "ut00091/ut00091" + JSON_TYPE_NAME;
	addResToList(footResStr);


	CMainHero *role = CMainHero::create();
	//role->retain(); //neil comment, 看外面立马就addChild了,先注释掉
	role->setShowID(showID);
	role->setResPath(stBodyName);
	
 	role->setDesiredPosition(role->getPosition());
	role->setDashSpeed(300);
	role->setWalkSpeed(330);
	role->setBackSpeed(300);
	role->setMaxHP(100000);	//设置最大血量
	role->setHP(100000);		//设置最大血量

	role->setCenterToBottom(110);
	role->setCenterToSides(80);
	role->setDefVal(500);
	role->setAtkVal(400);
	
	
	role->setWeaponCfgID(weaponCfgID);
	role->LoadActionRes(false);
	role->Idle();

	bool bIsSelf = false;
	if(P_USER_OBJ->getUserInfo()->uin == uin){  //玩家自己,添加换枪事件
		CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(BaseOSDScene::handleHeroWeaponChange), 
																HERO_WEAPON_CHANGE_EVENT, NULL);
		bIsSelf = true;

		CCArmature* pArm = CCArmature::create("ut00091");
		role->addChild(pArm, -1);
		pArm->getAnimation()->play("fly", -1, -1, 1);
	}

	role->showName(nick, bIsSelf);
	role->setVIPLv(uVIPLv);

	return role;
}

void BaseOSDScene::handleHeroWeaponChange(CCObject* pstObj){
	if(m_stMainHero == NULL){
		return;
	}
	
	//资源加载
	string stBodyName;
	int iRet = CMainHero::GetResourceNames(P_USER_OBJ->getUserInfo()->weaponCfgID, P_USER_OBJ->getUserInfo()->showID, stBodyName);
	string bodyStr = HERO_RES_PATH + stBodyName + "/" + stBodyName + JSON_TYPE_NAME;
	if (iRet){
		CCLog("no resource name is found for weapon %d", P_USER_OBJ->getUserInfo()->weaponCfgID);
		return;
	}
	addResToList(bodyStr);

	m_stMainHero->SwitchWeapon(P_USER_OBJ->getUserInfo()->weaponCfgID, false);
}

CMainHero* BaseOSDScene::getRole(UIN_t uin) {
	return dynamic_cast<CMainHero*>(m_pRolesDict->objectForKey(uin));
}

void BaseOSDScene::addRoleQueue(const vmsg::CSPlayerPosInScene& stInfo){
	UIN_t stUin = stInfo.comm().uin();
	map<UIN_t, vmsg::CSPlayerPosInScene>::iterator stIter; 
	stIter = m_stRolesQueue.find(stUin);

	if(stIter == m_stRolesQueue.end()){
		m_stRolesQueue.insert(make_pair(stUin, stInfo));
		//m_stRolesQueue[stUin] = stInfo;
	}
}

void BaseOSDScene::addRole(const vmsg::CSPlayerCommInScene& playerInfo, const CCPoint &pos, const CCPoint &direction) {
	CMainHero *pRole = NULL;
	UIN_t uin = playerInfo.uin();

	if(m_pRolesDict->objectForKey(uin) == NULL) { // 找不到
		pRole = createRole(uin, playerInfo.nick(), playerInfo.showid(), playerInfo.weaponcfgid(), playerInfo.viplvl());
		m_pRolesDict->setObject(pRole, uin);
		//getLayer(LAYER_ID_GAME)->addChild(pRole);
		pRole->setWalkableRect(walkableRect);
		int iNewZorder = CInstanceUtils::GetZOrderByType(ROLE_Z_ORDER, pRole->getPositionY());
		_sceneForegound->addChild(pRole, iNewZorder);

		if(uin == P_USER_OBJ->getUserInfo()->uin){
			pRole->setVIPLv(P_USER_OBJ->getUserInfo()->vipLv);
			m_stMainHero = pRole;
		}

		if(pos.equals(CCPointZero)) {
			pRole->setPosition(DEFAULT_ROLE_POS);
		} else {
			pRole->setPosition(pos);
		}

		if(direction.x >= 0) {
			pRole->setScaleX(1.0f);
		} else {
			pRole->setScaleX(-1.0f);
		}
	}
}

void BaseOSDScene::removeRole(UIN_t uin) {
	CMainHero *pRole = dynamic_cast<CMainHero*>(m_pRolesDict->objectForKey(uin));

	if(pRole != NULL) {
		m_pRolesDict->removeObjectForKey(uin);
		//getLayer(LAYER_ID_GAME)->removeChild(pRole);
		_sceneForegound->removeChild(pRole);
		//pRole->release(); //neil comment,createrole时把retain去掉了
	}
}

void BaseOSDScene::setRolePos(UIN_t uin, const CCPoint &pos, const CCPoint &direction) {
	CMainHero *pRole = dynamic_cast<CMainHero*>(m_pRolesDict->objectForKey(uin));
	
	if(pRole != NULL) {
		pRole->WalkTo(pos);
		pRole->setDstDirection(direction);
	}
}

void BaseOSDScene::moveRoleByDirection(UIN_t uin, const CCPoint &startPos, const CCPoint &direction) {
	CMainHero *pRole = dynamic_cast<CMainHero*>(m_pRolesDict->objectForKey(uin));
	
	if(pRole != NULL) {
		//pRole->setPosition(startPos); // 强制拉回
		pRole->setIsFollowActivate(true);
		pRole->setFollowDstPos(startPos); // 模拟强制拉回
		pRole->stateChangeByMove(direction);
	}
}

void BaseOSDScene::moveRoleToDestination(UIN_t uin, const CCPoint &endPos, const CCPoint &direction) {
	CMainHero *pRole = dynamic_cast<CMainHero*>(m_pRolesDict->objectForKey(uin));

	if(pRole != NULL) {
		pRole->WalkTo(endPos);
		//moveEntity(pRole, endPos);
	}
}

//void BaseOSDScene::cleanup(){
//	cleanRes();
//}
//
//void BaseOSDScene::cleanRes(){
//}
