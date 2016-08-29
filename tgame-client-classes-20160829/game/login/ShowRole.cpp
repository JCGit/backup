#include "ShowRole.h"

#include "Resources.h"

#include "vlib/CVRand.h"

#include "CfgMaster/PlayerInitCfgMaster.h"
#include "CfgMaster/HeroCfgMaster.h"

#include "game/role/MainHero.h"

#define ROLE_ZOOM_FACTOR 1.6f // 角色缩放因子

ShowRole::ShowRole()
:m_uPlayerID(0)
,m_pRoleArmature(NULL)
,m_pRole(NULL)
,m_bIsPlaying(false)
{
}

ShowRole::~ShowRole()
{
}

ShowRole* ShowRole::create() {
	ShowRole *pRet = new ShowRole();

	if(pRet != NULL && pRet->init()) {
		pRet->autorelease();
	} else {
		CC_SAFE_DELETE(pRet);
	}

	return pRet;
}

bool ShowRole::init() {
	bool bRet = false;

	do {
		CC_BREAK_IF(!CCNode::init());

		setAnchorPoint(ccp(0.5, 0));

		bRet = true;
	} while(0);

	return bRet;
}

void ShowRole::onEnter() {
	CCNode::onEnter();

	/*if(m_pRoleArmature != NULL) {
		m_pRoleArmature->getAnimation()->resume();
	}
	
	play();*/

	if(m_pRole == NULL) {
		setPlayerID(m_uPlayerID);
	}
}

void ShowRole::onExit() {
	CCNode::onExit();

	if(m_pRole && m_pRole->getParent()){
		m_pRole->removeFromParent();
		m_pRole = NULL;
	}
}

unsigned int ShowRole::getPlayerID() {
	return m_uPlayerID;
}

void ShowRole::setPlayerID(unsigned int uPlayerID) {
	m_uPlayerID = uPlayerID;

		const PlayerInitCfg *pPlayerInitCfg = PLAYER_INIT_CFG->GetPlayerCfgByID(m_uPlayerID);
		const HeroCfg *pHeroCfg = HERO_CFG_MASTER->GetHeroCfgByID(pPlayerInitCfg->showid());
		std::string name = pHeroCfg->resname() + "_denglu";
		
		/*if(m_pRoleArmature == NULL) {
			string url = HERO_RES_PATH + pHeroCfg->resname() + "_denglu/" + pHeroCfg->resname() + "_denglu.ExportJson";
			CCArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo(url.c_str());
			m_pRoleArmature = CCArmature::create(name.c_str());
			m_pRoleArmature->setScale(ROLE_ZOOM_FACTOR);
			m_pRoleArmature->getAnimation()->play("stand");
			addChild(m_pRoleArmature);
		}*/
		
		// 获取资源路径
		unsigned int uShowID = pHeroCfg->showid();
		unsigned int uWeaponCfgID = 58; // 登录指定一把枪
		string bodyName;
		int iRet = CMainHero::GetResourceNames(uWeaponCfgID, uShowID, bodyName);

		if(iRet != 0) {
			CCLOG("Fail to get hero res! ShowID=%u WeaponCfgID=%u", uShowID, uWeaponCfgID);
			return;
		}

		if(m_pRole != NULL) {
			m_pRole->removeFromParent();
			m_pRole = NULL;
		} 

		m_pRole = CMainHero::create();
		m_pRole->setShowID(uShowID);
		m_pRole->setResPath(bodyName);
		m_pRole->LoadActionRes();
		m_pRole->setHP(1);
		m_pRole->getBlood()->setVisible(false);
		m_pRole->setScale(ROLE_ZOOM_FACTOR);
		m_pRole->Idle();
		m_pRole->setShadowVisiable(false);
		addChild(m_pRole);
		m_pRole->SwitchWeapon(uWeaponCfgID, false);
		m_pRoleArmature = m_pRole->getArmature();

		if(m_stShowIdxs.size() == 0) {
			const vector<string> &actionNames = m_pRoleArmature->getAnimation()->getAnimationData()->movementNames;

			for(size_t i = 0; i < actionNames.size(); i++) {
				string name = actionNames.at(i);

				if(name == "stand" || name == "run" || name == "attack" || name == "win" || name == "walk_attack" || name == "back_attack") {
					m_stShowIdxs.push_back(i);
				}
			}
		}
}

void ShowRole::play() {
	if(!m_bIsPlaying && m_pRoleArmature != NULL) {
		m_bIsPlaying = true;
		m_pRoleArmature->getAnimation()->playWithIndex(0);
		scheduleOnce(schedule_selector(ShowRole::updateAnim), 2.0f);
	}
}

void ShowRole::stop() {
	if(m_bIsPlaying && m_pRoleArmature != NULL) {
		m_bIsPlaying = false;
		m_pRoleArmature->getAnimation()->play("stand");
		unschedule(schedule_selector(ShowRole::updateAnim));
	}
}

void ShowRole::updateAnim(float dt) {
	int idx = CVLib::CRandom::GetRand(m_stShowIdxs.size() - 1);
	vector<int> stIdxs;
	stIdxs.push_back(m_stShowIdxs.at(idx + 1));
	stIdxs.push_back(0);
	
	m_pRoleArmature->getAnimation()->playWithIndexes(stIdxs, -1, false);
	m_pRoleArmature->getAnimation()->setMovementEventCallFunc(this, movementEvent_selector(ShowRole::onAnimPlayEvent));
}

void ShowRole::onAnimPlayEvent(CCArmature* pArmature, MovementEventType type, const char *pszActionName) {
	switch(type) {
		case COMPLETE:
			m_pRoleArmature->getAnimation()->playWithIndex(0);
			scheduleOnce(schedule_selector(ShowRole::updateAnim), 2.0f);
			break;
		default:;
	}
}
