#include "SubEntryPanel.h"

#include "ModuleID.pb.h"

#include "defines.h"

#include "basic/ToolUtils.h"

#include "game/city/CityControl.h"
#include "game/evolution/EvolutionControl.h"
#include "game/attrstrengthen/AttrStrengthenControl.h"
#include "game/friend/FriendControl.h"
#include "game/guild/GuildControl.h"
#include "game/arena/ArenaControl.h"
#include "game/fightendless/FightEndlessControl.h"
#include "game/nihility/NihilityControl.h"

#include "frag/FragControl.h"
#include "shop/ShopControl.h"

#include "EntryControl.h"

#define BTN_TIP_CHILD_TAG 101

SubEntryPanel::SubEntryPanel()
:m_pSubEntryLayout(NULL)
,m_pBgImg(NULL)
,m_pTitleWeaponImg(NULL)
,m_pTitleGrowthImg(NULL)
,m_pTitleSocialImg(NULL)
,m_pTitleChallengeImg(NULL)
,m_pTitleBusinessImg(NULL)
{
}

SubEntryPanel::~SubEntryPanel()
{
	std::map<unsigned int, UIButton*>::const_iterator iter;

	for(iter = m_stBtnMap.begin(); iter != m_stBtnMap.end(); iter++) {
		UIButton *pBtn = iter->second;
		pBtn->release();
	}
}

SubEntryPanel* SubEntryPanel::create() {
	SubEntryPanel *pRet = new SubEntryPanel();

	if(pRet != NULL && pRet->init()) {
		pRet->autorelease();
	} else {
		CC_SAFE_DELETE(pRet);
	}

	return pRet;
}

bool SubEntryPanel::init() {
	bool bRet = false;

	do {
		CC_BREAK_IF(!BasePanel::init());

		unsigned int subWeaponLst[] = {
			ENTRY_WEAPON_CULTURE,
			ENTRY_EQUIP_CULTURE,
			ENTRY_WEAPON_ARRAY,
			ENTRY_FRAGMENT,
		};

		m_stWeaponEntryGroup = std::vector<unsigned int>(subWeaponLst, subWeaponLst + sizeof(subWeaponLst) / sizeof(unsigned int));

		unsigned int subGrowthLst[] = {
			ENTRY_EVOLUTION,
			ENTRY_ATTR_STRENGTHEN,
			ENTRY_GENE_STRENGTHEN,
		};

		m_stGrowthEntryGroup = std::vector<unsigned int>(subGrowthLst, subGrowthLst + sizeof(subGrowthLst) / sizeof(unsigned int));

		unsigned int subSocialLst[] = {
			ENTRY_FRIEND,
			ENTRY_GUILD,
		};

		m_stSocialEntryGroup = std::vector<unsigned int>(subSocialLst, subSocialLst + sizeof(subSocialLst) / sizeof(unsigned int));

		unsigned int subChallengeLst[] = {
			ENTRY_ELITE_WAR,
			ENTRY_ARENA,
			ENTRY_NOTHING_LAND,
		};

		m_stChallengeEntryGroup = std::vector<unsigned int>(subChallengeLst, subChallengeLst + sizeof(subChallengeLst) / sizeof(unsigned int));
		
		unsigned int subBusinessLst[] = {
			ENTRY_BLACK_MARKET,
			ENTRY_LIMITED_SHOP,
			//ENTRY_FRAGMENT,
		};

		m_stBusinessEntryGroup = std::vector<unsigned int>(subBusinessLst, subBusinessLst + sizeof(subBusinessLst) / sizeof(unsigned int));


		initUI();

		bRet = true;
	} while(0);

	return bRet;
}

CCNode* SubEntryPanel::getGuideNodeByName(const std::string &name) {
	UIButton *pRet = NULL;
	UIButton *pBtn = NULL;

	if(name == "friend_entry") {
		pBtn = getEntryBtn(ENTRY_FRIEND);
	} else if(name == "arena_entry") {
		pBtn = getEntryBtn(ENTRY_ARENA);
	} else if(name == "nothing_land_entry") {
		pBtn = getEntryBtn(ENTRY_NOTHING_LAND);
	} else if(name == "elite_war_entry") {
		pBtn = getEntryBtn(ENTRY_ELITE_WAR);
	} else if(name == "weapon_culture_entry") {
		pBtn = getEntryBtn(ENTRY_WEAPON_CULTURE);
	} else if(name == "weapon_array_entry") {
		pBtn = getEntryBtn(ENTRY_WEAPON_ARRAY);
	} else if(name == "gene_strengthen_entry") {
		pBtn = getEntryBtn(ENTRY_GENE_STRENGTHEN);
	}else if(name == "equip_culture_entry"){
		pBtn = getEntryBtn(ENTRY_EQUIP_CULTURE);
	}else if (name == "evolution_entry")
	{
		pBtn = getEntryBtn(ENTRY_EVOLUTION);
	}

	if(pBtn != NULL && pBtn->isRunning() && pBtn->isVisible() && pBtn->isTouchEnabled()) {
		pRet = pBtn;
	}

	return pRet;
}

UIButton* SubEntryPanel::getEntryBtn(unsigned int uEntryID) {
	UIButton *pRet = NULL;
	std::map<unsigned int, UIButton*>::const_iterator iter = m_stBtnMap.find(uEntryID);

	if(iter != m_stBtnMap.end()) {
		pRet = iter->second;
	}

	return pRet;
}

void SubEntryPanel::addEntryBtn(unsigned int uEntryID) {
	UIButton *pBtn = getEntryBtn(uEntryID);

	if(pBtn == NULL) {
		pBtn = P_ENTRY_CTRL->createEntryBtn(uEntryID);
		pBtn->setScale(1.2f);
		m_stBtnMap[uEntryID] = pBtn;
		pBtn->retain(); // release at ModuleEntryLayout::removeEntryBtn(unsigned int uEntryID)
	}

	if(pBtn->getParent() == NULL) {
		//pBtn->setVisible(false);
		//pBtn->setTouchEnabled(false);
		pBtn->addTouchEventListener(this, toucheventselector(SubEntryPanel::touchEventHandler));

		for(unsigned int i = 0; i < m_stSubEntryLst.size(); i++) {
			if(m_stSubEntryLst[i] == uEntryID) {
				m_pSubEntryLayout->addChild(pBtn);
				break;
			}
		}

	}
}

void SubEntryPanel::removeEntryBtn(unsigned int uEntryID) {
	UIButton *pBtn = getEntryBtn(uEntryID);

	if(pBtn != NULL && pBtn->getParent() != NULL) {
		pBtn->removeFromParent();
		m_stBtnMap.erase(uEntryID);
		pBtn->release(); // retain at ModuleEntryLayout::addEntryBtn(unsigned int uEntryID)
	}
}

void SubEntryPanel::openWithType(SubEntryType type) {
	open();

	for(unsigned int i = 0; i < m_stSubEntryLst.size(); i++) {
		UIButton *pBtn = getEntryBtn(m_stSubEntryLst[i]);

		if(pBtn != NULL && pBtn->getParent() != NULL) {
			pBtn->removeFromParent();
		}
	}

	m_stSubEntryLst.clear();

	std::vector<unsigned int> stEntryGroup;

	m_pTitleWeaponImg->setVisible(type == SUB_ENTRY_WEAPON);
	m_pTitleGrowthImg->setVisible(type == SUB_ENTRY_GROWTH);
	m_pTitleSocialImg->setVisible(type == SUB_ENTRY_SOCIAL);
	m_pTitleChallengeImg->setVisible(type == SUB_ENTRY_CHALLENGE);
	m_pTitleBusinessImg->setVisible(type == SUB_ENTRY_BUSINESS);

	if(type == SUB_ENTRY_WEAPON) {
		stEntryGroup = m_stWeaponEntryGroup;
	} else if(type == SUB_ENTRY_GROWTH) {
		stEntryGroup = m_stGrowthEntryGroup;
	} else if(type == SUB_ENTRY_SOCIAL) {
		stEntryGroup = m_stSocialEntryGroup;
	} else if(type == SUB_ENTRY_CHALLENGE) {
		stEntryGroup = m_stChallengeEntryGroup;
	} else if(type == SUB_ENTRY_BUSINESS) {
		stEntryGroup = m_stBusinessEntryGroup;
	}

	for(unsigned int i = 0; i < stEntryGroup.size(); i++) {
		unsigned int uEntryID = stEntryGroup[i];
		
		if(P_ENTRY_CTRL->isEntryOpen(uEntryID)) {
			m_stSubEntryLst.push_back(uEntryID);
			addEntryBtn(uEntryID);
			map<unsigned int, unsigned int>::const_iterator iter = m_stBadgeMap.find(uEntryID);
			
			if(iter != m_stBadgeMap.end()) {
				badgeEntryBtn(uEntryID, iter->second);
			}
		} else {
			removeEntryBtn(uEntryID);
		}
	}

	float originX = m_pSubEntryLayout->getSize().width / 2 - (m_stSubEntryLst.size() * (120 + 8) - 8) / 2 + 120 / 2;
	float originY = m_pSubEntryLayout->getSize().height / 2 - 20;

	for(unsigned int i = 0; i < m_stSubEntryLst.size(); i++) {
		UIButton *pBtn = getEntryBtn(m_stSubEntryLst[i]);
		pBtn->setPosition(ccp(originX + 128 * i, originY));
	}

	GUIDE_SHOW(GUIDE_UI_SUB_ENTRY);
}

void SubEntryPanel::badgeEntryBtn(unsigned int uEntryID, unsigned int uBadgeNum) {
	unsigned int uParentEntryID = uEntryID >> 8;

	if(uParentEntryID > 0 && uParentEntryID >> 8 == 0) { // 二级入口
		map<unsigned int, unsigned int>::iterator iter = m_stBadgeMap.find(uEntryID);

		if(iter != m_stBadgeMap.end()) {
			iter->second = uBadgeNum;
		} else {
			m_stBadgeMap.insert(make_pair(uEntryID, uBadgeNum));
		}

		if(m_bIsOpen) {
			UIButton *pBtn = getEntryBtn(uEntryID);

			if(pBtn != NULL) {
				float offsetX = pBtn->getSize().width * 0.32f;
				float offsetY = pBtn->getSize().height * 0.32f;
				//CToolUtils::badgeWidgetWithPoint(pBtn, uBadgeNum, offsetX, offsetY);
				CToolUtils::effectEntryIcon(pBtn, uBadgeNum > 0);
			}
		}
	}
}

void SubEntryPanel::initUI() {
	CCDirector *pDirector = CCDirector::sharedDirector();
	CCSize screenSize = pDirector->getVisibleSize();
	CCPoint screenOrigin = pDirector->getVisibleOrigin();

	initLayoutFromJsonFile("ui_v2/SubEntryPanelUI.ExportJson");
	m_pRootLayout->setSize(screenSize);
	m_pRootLayout->setPosition(screenOrigin);
	m_pRootLayout->setBackGroundColorType(LAYOUT_COLOR_SOLID);
	m_pRootLayout->setBackGroundColor(ccc3(0x00, 0x00, 0x00));
	m_pRootLayout->setBackGroundColorOpacity(0xA0);
	addChild(m_pRootLayout);
	setName("sub_entry_panel");
	setPopType(POP_CENTER);
	m_pSubEntryLayout = dynamic_cast<UILayout*>(m_pRootLayout->getChildByName("sub_entry_layout"));
	m_pSubEntryLayout->setPosition(CCPoint((m_pRootLayout->getSize() - m_pSubEntryLayout->getSize()) / 2));
	//m_pSubEntryLayout->setPosition(CCPoint(screenSize - m_pSubEntryLayout->getSize()));

	m_pBgImg = dynamic_cast<UIImageView*>(m_pSubEntryLayout->getChildByName("bg_img"));
	m_pBgImg->loadTexture("ui_bg/bg_sub_entry.png");

	m_pTitleWeaponImg = dynamic_cast<UIImageView*>(m_pSubEntryLayout->getChildByName("title_weapon_img"));

	m_pTitleGrowthImg = dynamic_cast<UIImageView*>(m_pSubEntryLayout->getChildByName("title_growth_img"));

	m_pTitleSocialImg = dynamic_cast<UIImageView*>(m_pSubEntryLayout->getChildByName("title_social_img"));

	m_pTitleChallengeImg = dynamic_cast<UIImageView*>(m_pSubEntryLayout->getChildByName("title_challenge_img"));

	m_pTitleBusinessImg = dynamic_cast<UIImageView*>(m_pSubEntryLayout->getChildByName("title_business_img"));

	addTouchEventListener(this, toucheventselector(SubEntryPanel::touchEventHandler));
	
}

void SubEntryPanel::touchEventHandler(CCObject *pSender, TouchEventType type) {
	UIWidget *pWidget = dynamic_cast<UIWidget*>(pSender);
	const char *pszWidgetName = pWidget->getName();

	switch(type) {
		case TOUCH_EVENT_ENDED:
			if(strcmp(pszWidgetName, getName()) == 0) {
				close();
			} else if(string(pszWidgetName).substr(0, string(ENTRY_PREFIX_NAME).size()) == ENTRY_PREFIX_NAME) {
				string btnName = pszWidgetName;
				unsigned int uPrefixSize = string(ENTRY_PREFIX_NAME).size();
				string strEntryID = btnName.substr(uPrefixSize, btnName.size());
				unsigned int uEntryID = atol(strEntryID.c_str());
				GUIDE_TOUCH(pWidget);

				P_ENTRY_CTRL->enter(uEntryID);

				close();
			}
			break;
		default:;
	}
}
