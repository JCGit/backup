#include "LevelUpPanel.h"

#include "defines.h"

#include "CfgMaster/PlayerGrowCfgMaster.h"
#include "CfgMaster/EntryCfgMaster.h"

#include "basic/ImageNumber.h"
#include "basic/WeaponUtils.h"

#include "game/UserObj.h"
#include "game/task/TaskControl.h"
#include "EntryControl.h"
#include "CfgMaster/ModuleOpenCfgMaster.h"
#include "AutoPopControl.h"
#include "task/TaskControl.h"

LevelUpPanel::LevelUpPanel()
:m_pCloseBtn(NULL)
,m_pUpToImg(NULL)
,m_pLvImgTxt(NULL)
,m_pLvImg(NULL)
,m_pLastHpTxt(NULL)
,m_pLastAtkTxt(NULL)
,m_pLastDefTxt(NULL)
,m_pHpTxt(NULL)
,m_pAtkTxt(NULL)
,m_pDefTxt(NULL)
,m_pNewModuleOpenImg(NULL)
,m_pEntrysLayout(NULL)
,m_bIsLevelUp(false)
{
}

LevelUpPanel::~LevelUpPanel()
{
}

LevelUpPanel* LevelUpPanel::create() {
	LevelUpPanel *pRet = new LevelUpPanel();

	if(pRet != NULL && pRet->init()) {
		pRet->autorelease();
	} else {
		CC_SAFE_DELETE(pRet);
	}

	return pRet;
}

bool LevelUpPanel::init() {
	bool bRet = false;

	do {
		CC_BREAK_IF(!BasePanel::init());

		bRet = true;
	} while(0);

	return bRet;
}

void LevelUpPanel::open() {
	if(!m_bUIInited) {
		m_bUIDelayed = true;
	} else {
		m_bIsLevelUp = false;
		BasePanel::open();
	}
}

void LevelUpPanel::close() {
	BasePanel::close();

	P_AUTO_POP_CTRL->onClosePanel(this);
}

bool LevelUpPanel::isMeetAutoOpen() {
	return m_bIsLevelUp;
}

void LevelUpPanel::levelup(unsigned int uPreLevel, unsigned int uLevel) {
	if(!m_bUIInited) {
		initUI();
	}

	m_bIsLevelUp = true;
	const UserInfo *pUserInfo = P_USER_OBJ->getUserInfo();
	const PlayerGrowCfg *pPlayerGrowCfg = PLAYER_GROW_CFG_MASTER->getPlayerGrowCfg(uLevel);
	m_pLvImgTxt->setNum(intToString(uLevel));
	m_pLvImgTxt->setPosition(m_pUpToImg->getPosition() + ccp(m_pUpToImg->getSize().width + 8, 0));
	m_pLvImg->setPosition(m_pLvImgTxt->getPosition() + ccp(m_pLvImgTxt->getContentSize().width + 8, 0));

	// 大坑说显示总值
	unsigned int weaponAddCoe = WeaponUtils::getWeaponAddCoe(pUserInfo->weapon.cfgid(), pUserInfo->weapon.level(), pUserInfo->weapon.quality());

	m_pLastHpTxt->setText(intToString(pUserInfo->hp));
	m_pLastAtkTxt->setText(intToString(pUserInfo->atk + weaponAddCoe));
	m_pLastDefTxt->setText(intToString(pUserInfo->def));

	const PlayerGrowCfg *pPrePlayerGrowCfg = PLAYER_GROW_CFG_MASTER->getPlayerGrowCfg(uPreLevel); // 初始1级，升级时总会有上一级
	m_pHpTxt->setText(intToString(pUserInfo->hp + pPlayerGrowCfg->hp() - pPrePlayerGrowCfg->hp()));
	m_pAtkTxt->setText(intToString(pUserInfo->atk + weaponAddCoe + pPlayerGrowCfg->atk() - pPrePlayerGrowCfg->atk()));
	m_pDefTxt->setText(intToString(pUserInfo->def + pPlayerGrowCfg->def() - pPrePlayerGrowCfg->def()));

	//pair<unsigned int, vector<unsigned int> > stLevelEntrysPair = ENTRY_CFG_MASTER->getLevelEntrys(uLevel);
	//m_pNewModuleOpenImg->setVisible(uLevel == stLevelEntrysPair.first);
	m_pEntrysLayout->removeAllChildren();
	/*
	if(uLevel == stLevelEntrysPair.first) {
		vector<unsigned int> stLevelEntrys = stLevelEntrysPair.second;
		float originX = m_pEntrysLayout->getSize().width / 2 - (stLevelEntrys.size() * (100 + 8) - 8) / 2 + 100 / 2;
		float originY = m_pEntrysLayout->getSize().height / 2;

		for(unsigned int i = 0; i < stLevelEntrys.size(); i++) {
			unsigned int uEntryID = stLevelEntrys[i];
			UIImageView *pEntryIconImg = UIImageView::create();
			const EntryCfg *pEntryCfg = ENTRY_CFG_MASTER->getEntryCfg(uEntryID);
			string iconURL = string("icon/entry/") + pEntryCfg->entryicon() + ".png";
			pEntryIconImg->loadTexture(iconURL.c_str());
			pEntryIconImg->setPosition(ccp(originX + 108 * i, originY));
			m_pEntrysLayout->addChild(pEntryIconImg);
		}
	}
	*/
	unsigned int pCurentTask = P_TASK_CTRL->getCurTask().taskid()+1;
	map<int ,unsigned int ,CmpByKey> pDisModuleMap; //差值，moduleId
	for (int i = 0; i< MODULE_OPEN_CFG_MASTER->getModuleOpenCfgNumb(); i++)
	{
		ModuleOpenCfg* pModule = MODULE_OPEN_CFG_MASTER->getModuleOpenCfgByIndex(i);
		if (pModule)
		{
			int disTaskId = pModule->taskid() - pCurentTask;
			if(disTaskId >=0)
			{
				pDisModuleMap.insert(make_pair(disTaskId,pModule->moduleid()));
			}
		}
	}
	if (pDisModuleMap.size()>0)
	{
		m_pEntrysLayout->setVisible(true);
		m_pNewModuleOpenImg->setVisible(true);
		
		unsigned int uEntryID = ENTRY_CFG_MASTER->getEntryIDByModuleID(pDisModuleMap.begin()->second);
		UIImageView *pEntryIconImg = UIImageView::create();
		const EntryCfg *pEntryCfg = ENTRY_CFG_MASTER->getEntryCfg(uEntryID);
		string iconURL = string("icon/entry/") + pEntryCfg->entryicon() + ".png";
		pEntryIconImg->loadTexture(iconURL.c_str());
		pEntryIconImg->setPosition(ccp(m_pEntrysLayout->getSize().width / 2 , m_pEntrysLayout->getSize().height / 2));
		m_pEntrysLayout->addChild(pEntryIconImg);

	}else
	{
		m_pEntrysLayout->setVisible(false);
		m_pNewModuleOpenImg->setVisible(false);
	}

	if(m_bUIDelayed) {
		m_bUIDelayed = false;
		open();
	}
}

vector<unsigned int> LevelUpPanel::getWillOpenEntry()
{
	//int pNowTaskId = P_TASK_CTRL->getCurTask().taskid;
	//MODULE_OPEN_CFG_MASTER->
	vector<unsigned int> pEntry;
	return pEntry;
}

void LevelUpPanel::initUI() {
	initLayoutFromJsonFile("ui_v2/LevelUpPanelUI.ExportJson");
	addChild(m_pRootLayout);

	m_pCloseBtn = dynamic_cast<UIButton*>(m_pRootLayout->getChildByName("close_btn"));
	m_pCloseBtn->addTouchEventListener(this, toucheventselector(LevelUpPanel::touchEventHandler));

	m_pUpToImg = dynamic_cast<UIImageView*>(m_pRootLayout->getChildByName("up_to_img"));

	m_pLvImgTxt = ImageNumber::create(29);
	m_pLvImgTxt->setAnchorPoint(ccp(0.0f, 0.0f));
	m_pLvImgTxt->setPosition(m_pUpToImg->getPosition() + ccp(m_pUpToImg->getSize().width + 8, 0));
	m_pRootLayout->addNode(m_pLvImgTxt);
	m_pLvImgTxt->setNum("0", 0.0f);

	m_pLvImg = dynamic_cast<UIImageView*>(m_pRootLayout->getChildByName("lv_img"));

	m_pLastHpTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("last_hp_val_txt"));

	m_pLastAtkTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("last_atk_val_txt"));

	m_pLastDefTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("last_def_val_txt"));

	m_pHpTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("hp_val_txt"));

	m_pAtkTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("atk_val_txt"));

	m_pDefTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("def_val_txt"));

	m_pNewModuleOpenImg = dynamic_cast<UIImageView*>(m_pRootLayout->getChildByName("new_module_open_img"));

	m_pEntrysLayout = dynamic_cast<UILayout*>(m_pRootLayout->getChildByName("entrys_layout"));
	
	m_bUIInited = true;

}

void LevelUpPanel::touchEventHandler(CCObject *pSender, TouchEventType type) {
	switch(type) {
		case TOUCH_EVENT_ENDED:
			if(pSender == m_pCloseBtn) {
				close();
			}
			break;
		default:;
	}
}
