#include "DetailInfoLayout.h"

#include "defines.h"

#include "CfgMaster/ExpCfgMaster.h"

#include "Macros.pb.h"
#include "WeaponUtils.h"

#include "basic/Multiresolution.h"
#include "basic/NounHelper.h"
#include "basic/BasicTypes.h"
#include "basic/ImageNumber.h"

#include "game/Constant.h"
#include "game/UserObj.h"

DetailInfoLayout::DetailInfoLayout()
:m_pRoleNameTxt(NULL)
,m_pVIPTxt(NULL)
,m_pLevelValTxt(NULL)
,m_pHpValTxt(NULL)
,m_pExpProgress(NULL)
,m_pExpProgressTxt(NULL)
,m_pFightPowerValTxt(NULL)
,m_pUINValTxt(NULL)
,m_pAtkValTxt(NULL)
,m_pDefValTxt(NULL)
,m_pHitValTxt(NULL)
,m_pAtkTypeValTxt(NULL)
,m_pDefTypeValTxt(NULL)
,m_pThunderAtkValTxt(NULL)
,m_pPoisonAtkValTxt(NULL)
,m_pFireAtkValTxt(NULL)
,m_pIceAtkValTxt(NULL)
,m_pBiochemistryAtkValTxt(NULL)
,m_pThunderDefValTxt(NULL)
,m_pPoisonDefValTxt(NULL)
,m_pFireDefValTxt(NULL)
,m_pIceDefValTxt(NULL)
,m_pBiochemistryDefValTxt(NULL)
{
}

DetailInfoLayout::~DetailInfoLayout()
{
}

DetailInfoLayout* DetailInfoLayout::create(UILayout *pRefLayout) {
	DetailInfoLayout *pRet = new DetailInfoLayout();

	if(pRet != NULL && pRet->initWithLayout(pRefLayout)) {
		pRet->autorelease();
	} else {
		CC_SAFE_DELETE(pRet);
	}

	return pRet;
}

bool DetailInfoLayout::initWithLayout(UILayout *pRefLayout) {
	bool bRet = false;

	do {
		CC_BREAK_IF(!BaseLayout::init());

		initLayoutWithUI(pRefLayout);
		initUI();

		bRet = true;
	} while(0);

	return bRet;
}

void DetailInfoLayout::initUI() {
	m_pRoleNameTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("role_name_txt"));

	m_pVIPTxt = ImageNumber::create(25);
	m_pVIPTxt->setAnchorPoint(ccp(0.0f, 0.0f));
	m_pVIPTxt->setPosition(240, 538);
	m_pRootLayout->addNode(m_pVIPTxt);
	m_pVIPTxt->setNum("0", 0.0f, "vip");

	m_pLevelValTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("level_val_txt"));

	m_pHpValTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("hp_val_txt"));

	m_pExpProgress = dynamic_cast<UILoadingBar*>(m_pRootLayout->getChildByName("exp_progress"));
	
	m_pExpProgressTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("exp_progress_txt"));

	m_pFightPowerValTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("fight_power_val_txt"));
	CCLabelTTF *pLabel = dynamic_cast<CCLabelTTF*>(m_pFightPowerValTxt->getVirtualRenderer());
	pLabel->enableStroke(ccc3(0xff, 0x60, 0x00), 0.8f);
	//pLabel->enableShadow(CCSizeMake(2,-2), 1.0f, 1.0f);

	m_pUINValTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("uid_val_txt"));

	m_pDefValTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("def_val_txt"));

	m_pDefTypeValTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("def_type_val_txt"));

	m_pAtkValTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("atk_val_txt"));

	m_pAtkTypeValTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("atk_type_val_txt"));

	m_pHitValTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("hit_val_txt"));

	m_pThunderAtkValTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("thunder_atk_val_txt"));

	m_pThunderDefValTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("thunder_def_val_txt"));

	m_pPoisonAtkValTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("poison_atk_val_txt"));

	m_pPoisonDefValTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("poison_def_val_txt"));

	m_pFireAtkValTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("fire_atk_val_txt"));

	m_pFireDefValTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("fire_def_val_txt"));

	m_pIceAtkValTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("ice_atk_val_txt"));

	m_pIceDefValTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("ice_def_val_txt"));

	m_pBiochemistryAtkValTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("biochemistry_atk_val_txt"));

	m_pBiochemistryDefValTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("biochemistry_def_val_txt"));

	//setClickOutToClose(true);
}

void DetailInfoLayout::updateUI() {
	const UserInfo *pUserInfo = P_USER_OBJ->getUserInfo();

	vmsg::CSPlayerInfo stPlayerInfo;
	stPlayerInfo.set_uin(pUserInfo->uin);
	stPlayerInfo.set_nick(pUserInfo->nick);
	stPlayerInfo.set_level(pUserInfo->level);
	stPlayerInfo.set_exp(pUserInfo->exp);
	stPlayerInfo.set_weaponcfgid(pUserInfo->weaponCfgID);
	stPlayerInfo.set_coin(pUserInfo->coin);
	stPlayerInfo.set_remainmanual(pUserInfo->remainManual);
	stPlayerInfo.set_updateremaintime(pUserInfo->updateRemainTime);
	stPlayerInfo.set_showid(pUserInfo->showID);
	stPlayerInfo.set_sceneid(pUserInfo->sceneID);
	stPlayerInfo.set_newestwarmapid(pUserInfo->newestWarMapID);
	stPlayerInfo.set_viplv(pUserInfo->vipLv);
	stPlayerInfo.set_fightpower(pUserInfo->fightPower);

	vmsg::CSPlayerDetailInfo stPlayerDetailInfo;
	stPlayerDetailInfo.set_hp(pUserInfo->hp);
	stPlayerDetailInfo.set_mp(pUserInfo->mp);
	stPlayerDetailInfo.set_def(pUserInfo->def);
	stPlayerDetailInfo.set_atk(pUserInfo->atk);
	stPlayerDetailInfo.set_deftype(pUserInfo->defType);
	stPlayerDetailInfo.set_atktype(pUserInfo->atkType);
	stPlayerDetailInfo.set_hit(pUserInfo->hit);
	stPlayerDetailInfo.set_hitwithoutinitval(pUserInfo->hitWithOutInitVal);
	*stPlayerDetailInfo.mutable_weapon() = pUserInfo->weapon;

	for(unsigned int i = 0; i < pUserInfo->stAtkAttrs.size(); i++) {
		vmsg::CSPlayerAttr *pAttr = stPlayerDetailInfo.add_atkattrs();
		pAttr->set_attrtype(pUserInfo->stAtkAttrs[i].attrtype());
		pAttr->set_attrval(pUserInfo->stAtkAttrs[i].attrval());
	}

	for(unsigned int i = 0; i < pUserInfo->stDefAttrs.size(); i++) {
		vmsg::CSPlayerAttr *pAttr = stPlayerDetailInfo.add_defattrs();
		pAttr->set_attrtype(pUserInfo->stDefAttrs[i].attrtype());
		pAttr->set_attrval(pUserInfo->stDefAttrs[i].attrval());
	}

	stPlayerDetailInfo.set_movespeed(pUserInfo->moveSpeed);
	stPlayerDetailInfo.set_runspeed(pUserInfo->runSpeed);
	stPlayerDetailInfo.set_backspeed(pUserInfo->backSpeed);
	stPlayerDetailInfo.set_fightpower(pUserInfo->fightPower);

	stPlayerDetailInfo.set_showid(pUserInfo->showID);
	stPlayerDetailInfo.set_dodgedist(pUserInfo->dodgeDist);
	stPlayerDetailInfo.set_dodgecd(pUserInfo->dodgeCD);
	stPlayerDetailInfo.set_atkspeed(pUserInfo->atkSpeed);
	stPlayerDetailInfo.set_evolutionskillid(pUserInfo->evolutionSkillID);

	updateUI(stPlayerInfo, stPlayerDetailInfo);
}

void DetailInfoLayout::updateUI(const vmsg::CSPlayerInfo &stPlayerInfo, const vmsg::CSPlayerDetailInfo &stPlayerDetailInfo) {
	if(stPlayerInfo.has_uin()) {
		m_pRoleNameTxt->setText(stPlayerInfo.nick());
		m_pVIPTxt->setNum(intToString(stPlayerInfo.viplv()), 0.0f, "vip");
		m_pLevelValTxt->setText(intToString(stPlayerInfo.level()));

		/*m_pManualValTxt->setText(intToString(stPlayerDetailInfo.mp()) + "/" + intToString(stPlayerDetailInfo.mp()));
		m_pManualProgress->setPercent(100 * stPlayerDetailInfo.mp() / stPlayerDetailInfo.mp());*/

		unsigned int uLevel = stPlayerInfo.level();
		UINT64_t uExp = stPlayerInfo.exp();
		UINT64_t uMaxExp = EXP_CFG_MASTER->getExpCfgByID(uLevel)->exp();
		m_pExpProgress->setPercent(100 * uExp / uMaxExp);
		m_pExpProgressTxt->setText(intToString(100 * uExp / uMaxExp) + "%");

		m_pUINValTxt->setText(intToString((unsigned int)stPlayerInfo.uin()));
	}

	if(stPlayerDetailInfo.has_hp()) {
		m_pHpValTxt->setText(intToString(stPlayerDetailInfo.hp()));
		m_pFightPowerValTxt->setText(intToString(stPlayerDetailInfo.fightpower()));

		unsigned int weaponAddCoe = WeaponUtils::getWeaponAddCoe(stPlayerDetailInfo.weapon().cfgid(), stPlayerDetailInfo.weapon().level(), stPlayerDetailInfo.weapon().quality());
		m_pDefValTxt->setText(intToString(stPlayerDetailInfo.def()));
		m_pAtkValTxt->setText(intToString(stPlayerDetailInfo.atk() + weaponAddCoe));
		m_pHitValTxt->setText(intToString((unsigned int)(floor(stPlayerDetailInfo.hitwithoutinitval() * 2.3f + 0.5f))));

		std::string atkTypeName = "";

		if(stPlayerDetailInfo.atktype() == CLOSE_PANITRATE) {
			atkTypeName = NounStr(NOUN_CLOSE_PANITRATE);
		} else if(stPlayerDetailInfo.atktype() == CLOSE_BLUNT) {
			atkTypeName = NounStr(NOUN_CLOSE_BLUNT);
		} else if(stPlayerDetailInfo.atktype() == CLOSE_MAGIC) {
			atkTypeName = NounStr(NOUN_CLOSE_MAGIC);
		} else if(stPlayerDetailInfo.atktype() == CLOSE_ORDINARY) {
			atkTypeName = NounStr(NOUN_CLOSE_ORDINARY);
		} else if(stPlayerDetailInfo.atktype() == REMOGE_MAGIC) {
			atkTypeName = NounStr(NOUN_REMOTE_MAGIC);
		} else if(stPlayerDetailInfo.atktype() == REMOTE_PANITRATE) {
			atkTypeName = NounStr(NOUN_REMOTE_PANITRATE);
		} else if(stPlayerDetailInfo.atktype() == REMOTE_ORDINARY) {
			atkTypeName = NounStr(NOUN_REMOTE_ORDINARY);
		}  

		m_pAtkTypeValTxt->setText(atkTypeName);
		m_pAtkTypeValTxt->setVisible(atkTypeName != "");

		std::string defTypeName = "";

		if(stPlayerDetailInfo.deftype() == HEARY_ARMOR_DEF) {
			defTypeName = NounStr(HEARY_ARMOR_DEF_STR);
		} else if(stPlayerDetailInfo.deftype() == LIGHT_ARMOR_DEF) {
			defTypeName = NounStr(LIGHT_ARMOR_DEF_STR);
		} else if(stPlayerDetailInfo.deftype() == NO_ARMOR_DEF) {
			defTypeName = NounStr(NONE_ARMOR_DEF_STR);
		}

		m_pDefTypeValTxt->setText(defTypeName);
		m_pDefTypeValTxt->setVisible(defTypeName != "");

		for(int i = 0; i < stPlayerDetailInfo.atkattrs_size(); i++) {
			const vmsg::CSPlayerAttr &attr = stPlayerDetailInfo.atkattrs(i);

			switch(attr.attrtype()) {
				case ICE_MAGIC:
					m_pIceAtkValTxt->setText(intToString(attr.attrval()));
					break;
				case FLAME_MAGIC:
					m_pFireAtkValTxt->setText(intToString(attr.attrval()));
					break;
				case POISON_MAGIC:
					m_pPoisonAtkValTxt->setText(intToString(attr.attrval()));
					break;
				case BIOTECH_MAGIC:
					m_pBiochemistryAtkValTxt->setText(intToString(attr.attrval()));
					break;
				case LIGHTNING_MAGIC:
					m_pThunderAtkValTxt->setText(intToString(attr.attrval()));
					break;
				default:;
			}
		}

		for(int i = 0; i < stPlayerDetailInfo.defattrs_size(); i++) {
			const vmsg::CSPlayerAttr &attr = stPlayerDetailInfo.defattrs(i);

			switch(attr.attrtype()) {
				case ICE_MAGIC:
					m_pIceDefValTxt->setText(intToString(attr.attrval()));
					break;
				case FLAME_MAGIC:
					m_pFireDefValTxt->setText(intToString(attr.attrval()));
					break;
				case POISON_MAGIC:
					m_pPoisonDefValTxt->setText(intToString(attr.attrval()));
					break;
				case BIOTECH_MAGIC:
					m_pBiochemistryDefValTxt->setText(intToString(attr.attrval()));
					break;
				case LIGHTNING_MAGIC:
					m_pThunderDefValTxt->setText(intToString(attr.attrval()));
					break;
				default:;
			}
		}

	}
}
