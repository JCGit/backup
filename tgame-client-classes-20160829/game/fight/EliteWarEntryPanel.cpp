#include "EliteWarEntryPanel.h"
#include "defines.h"

#include "CfgMaster/EliteWarMapCfgMaster.h"
#include "CfgMaster/PackageCfgMaster.h"
#include "CfgMaster/EliteAwardLibCfgMaster.h"

#include "basic/TextHelper.h"
#include "basic/NounHelper.h"
#include "basic/CommonDialog.h"

#include "game/IconCell.h"
#include "game/bag/BagControl.h"

#include "LevelControl.h"
#include "FightControl.h"
#include "FightObj.h"
#include "LevelObj.h"
#include "game/warmap/WarMapControl.h"

#define MAX_AWARD_SHOW_NUM 5
#define CELL_PREFIX_NAME "cell_"

EliteWarEntryPanel::EliteWarEntryPanel()
:m_pCloseBtn(NULL)
,m_pBgInteriorImg(NULL)
,m_pTextChapterImg(NULL)
,m_pWarMapTxt(NULL)
,m_pBetterLevelValTxt(NULL)
,m_pWinCondValTxt(NULL)
,m_pEnterBtn(NULL)
,m_pAwardCells(NULL)
,m_uEliteWarMapID(0)
,m_bOpenWithQuery(false)
{
}

EliteWarEntryPanel::~EliteWarEntryPanel()
{
	m_pAwardCells->removeAllObjects();
	CC_SAFE_RELEASE_NULL(m_pAwardCells);
}

EliteWarEntryPanel* EliteWarEntryPanel::create() {
	EliteWarEntryPanel *pRet = new EliteWarEntryPanel();

	if(pRet != NULL && pRet->init()) {
		pRet->autorelease();
	} else {
		CC_SAFE_DELETE(pRet);
	}

	return pRet;
}

bool EliteWarEntryPanel::init() {
	bool bRet = false;

	do {
		CC_BREAK_IF(!BasePanel::init());

		m_pAwardCells = CCArray::create();
		CC_SAFE_RETAIN(m_pAwardCells);

		initUI();

		bRet = true;
	} while(0);

	return bRet;
}

void EliteWarEntryPanel::open() {
	BasePanel::open();

	/*showLoading();

	if(!m_bUIInited) {
		initUI();
	}*/

	const EliteWarMapCfg *pEliteWarMapCfg = ELITE_WAR_MAP_CFG_MASTER->getEliteWarMapCfg(m_uEliteWarMapID);

	if(pEliteWarMapCfg != NULL) {
		/*string url = "level_panel_ui/text_chapter_" + intToString(pEliteWarMapCfg->warid()) + ".png";
		m_pTextChapterImg->loadTexture(url.c_str(), UI_TEX_TYPE_PLIST);*/

		m_pWarMapTxt->setText(pEliteWarMapCfg->name());
		m_pBetterLevelValTxt->setText(intToString(pEliteWarMapCfg->level()));
		m_pWinCondValTxt->setText(P_WAR_MAP_CTRL->getWinCondText(pEliteWarMapCfg->wintype(), pEliteWarMapCfg->winvalue1(), pEliteWarMapCfg->winvalue2()));

		const vector<EliteAwardLibCfg*> &stChapterAwards = ELITE_AWARD_LIB_CFG_MASTER->getChapterAwards(pEliteWarMapCfg->warid());
		vector<EliteAwardLibCfg*> stFilterChapterAwards;

		for(unsigned int i = 0; i < stChapterAwards.size(); i++) {
			if(m_uEliteWarMapID % 5 == 0) {
				if(stChapterAwards[i]->isboss() > 0) {
					stFilterChapterAwards.push_back(stChapterAwards[i]);
				}
			} else {
				if(stChapterAwards[i]->isboss() == 0) {
					stFilterChapterAwards.push_back(stChapterAwards[i]);
				}
			}
		}

		unsigned int uPackageID = pEliteWarMapCfg->packageid();
		const PackageCfg* pPackageCfg = PACKAGE_CFG_MASTER->GetPackageCfgByID(uPackageID);
		unsigned int elmtNum = pPackageCfg->elmtlst_size() < MAX_AWARD_SHOW_NUM ? pPackageCfg->elmtlst_size() : MAX_AWARD_SHOW_NUM;
		IconCell *pCell = NULL;

		for(unsigned int i = 0; i < MAX_AWARD_SHOW_NUM; i++){
			pCell = dynamic_cast<IconCell*>(m_pAwardCells->objectAtIndex(i));
			
			if(i < elmtNum) { // 优先显示礼包
				const PackageElmt &stElmt = pPackageCfg->elmtlst(i);
				pCell->setInfo(stElmt.elementtype(), stElmt.elementcntmin(), stElmt.elementid());
			} else {
				if((i - elmtNum) < stFilterChapterAwards.size()) { // 显示库里面的奖励
					const EliteAwardLibCfg *pEliteAwardLibCfg = stFilterChapterAwards[i-elmtNum];
					pCell->setInfo(pEliteAwardLibCfg->awardtype(), pEliteAwardLibCfg->awardcnt(), pEliteAwardLibCfg->awardcfgid());
				} else {
					pCell->clearCell();
				}
			}
		}

		const vmsg::CSEliteWarInfo &stEliteWarInfo = P_LEVEL_OBJ->getEliteWarInfo();
		unsigned int uFightMapSize = stEliteWarInfo.fightmapids_size();
		const vector<EliteWarMapCfg*> &stChapter = ELITE_WAR_MAP_CFG_MASTER->getEliteWarChapter(pEliteWarMapCfg->warid());

		m_pEnterBtn->setEnabled(uFightMapSize < stChapter.size() && stChapter[uFightMapSize]->id() == m_uEliteWarMapID);
	}

	GUIDE_SHOW(GUIDE_UI_ELITE_WAR_ENTER);

	m_bOpenWithQuery = false;
}

void EliteWarEntryPanel::close(){
	BasePanel::close();

	if(this->getIsInLoading()){
		this->closeLoading();
	}
}

CCNode* EliteWarEntryPanel::getGuideNodeByName(const std::string &name) {
	CCNode *pRet = NULL;

	if(name == "enter_btn") {
		if(m_pEnterBtn != NULL && m_pEnterBtn->isRunning() && m_pEnterBtn->isTouchEnabled()) {
			pRet = m_pEnterBtn;
		}
	}

	return pRet;
}

void EliteWarEntryPanel::openWithEliteWarMapID(unsigned int uEliteWarMapID) {
	m_uEliteWarMapID = uEliteWarMapID;

	open();

	/*if(!m_bUIInited) {
		showLoading();
		m_bUIDelayed = true;
		initUI();
	} else {
		open();
	}*/
}

void EliteWarEntryPanel::openWithQuery(unsigned int uEliteWarMapID) {
	m_uEliteWarMapID = uEliteWarMapID;

	const EliteWarMapCfg *pEliteWarMapCfg = ELITE_WAR_MAP_CFG_MASTER->getEliteWarMapCfg(uEliteWarMapID);

	if(pEliteWarMapCfg != NULL) {
		m_bOpenWithQuery = true;
		P_FIGHT_CTRL->getFightProxy()->sendQueryEliteWarMapRqst(pEliteWarMapCfg->warid());
	}
}

bool EliteWarEntryPanel::isOpenWithQuery() {
	return m_bOpenWithQuery;
}

void EliteWarEntryPanel::initUI() {
	initLayoutFromJsonFile("ui_v2/EliteWarInfoPanelUI.ExportJson");
	addChild(m_pRootLayout);

	m_pCloseBtn = dynamic_cast<UIButton*>(m_pRootLayout->getChildByName("close_btn"));
	m_pCloseBtn->addTouchEventListener(this, toucheventselector(EliteWarEntryPanel::touchEventHandler));

	/*m_pBgInteriorImg = dynamic_cast<UIImageView*>(m_pRootLayout->getChildByName("bg_interior_img"));
	m_pBgInteriorImg->loadTexture("ui_bg/bg_level_elite_entry.png");*/

	//m_pTextChapterImg = dynamic_cast<UIImageView*>(m_pRootLayout->getChildByName("text_chapter_txt"));

	m_pWarMapTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("war_map_txt"));

	m_pBetterLevelValTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("better_level_val_txt"));

	m_pWinCondValTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("win_cond_val_txt"));

	m_pEnterBtn = dynamic_cast<UIButton*>(m_pRootLayout->getChildByName("enter_btn"));
	m_pEnterBtn->addTouchEventListener(this, toucheventselector(EliteWarEntryPanel::touchEventHandler));

	UIImageView *pImg;
	IconCell *pCell;

	for(int i = 0; i < MAX_AWARD_SHOW_NUM; i++) {
		string imgName = string("item_") + intToString(i + 1) + "_img";
		pImg = dynamic_cast<UIImageView*>(m_pRootLayout->getChildByName(imgName.c_str()));
		pCell = IconCell::create();
		string cellName = string(CELL_PREFIX_NAME) + intToString(i);
		pCell->setName(cellName.c_str());
		pImg->setTouchEnabled(true);
		pImg->addChild(pCell);
		pImg->addTouchEventListener(this, toucheventselector(EliteWarEntryPanel::touchEventHandler));

		m_pAwardCells->addObject(pCell);
	}

	/*m_bUIInited = true;

	if(m_bUIDelayed) {
		m_bUIDelayed = false;
		open();
		closeLoading();
	}*/
}

void EliteWarEntryPanel::touchEventHandler(CCObject *pSender, TouchEventType type) {
	UIWidget *pWidget = dynamic_cast<UIWidget*>(pSender);
	const char *pszWidgetName = pWidget->getName();

	switch(type) {
		case TOUCH_EVENT_ENDED:
			if(strcmp(pszWidgetName, m_pCloseBtn->getName()) == 0) {
				close();
			} else if(strcmp(pszWidgetName, m_pEnterBtn->getName()) == 0) {
				GUIDE_TOUCH(pWidget);

				if(P_BAG_CTRL->getEmptyCellCnt() < MAX_AWARD_SHOW_NUM) {
					COMMON_DIALOG->show(TextStr(TEXT_BAG_NOT_ENOUGH), NULL, NounStr(OK));
					return;
				}

				P_FIGHT_OBJ->setFightType(ELITE_FIGHT_TYPE);
				P_LEVEL_OBJ->setEnterLevelID(m_uEliteWarMapID);
				P_FIGHT_CTRL->getFightProxy()->sendQueryPlayerFightInfo();
				showLoading();
			} else if(string(pszWidgetName).substr(0, string("item_").size()) == "item_") {
                unsigned int num = -1;
                sscanf(pszWidgetName, "item_%u_img", &num);
                if(num >= 1 && num <= m_pAwardCells->count() ) {
                    IconCell *pCell = dynamic_cast<IconCell*>(m_pAwardCells->objectAtIndex(num - 1));
                    if(pCell) {
                        P_BAG_CTRL->getItemInfoPanel()->open();
                        P_BAG_CTRL->getItemInfoPanel()->updateInfo(pCell->getItemCfgid(), pCell->getItemType(), false);
                    }                
                }

            }
				
			break;
		default:;
	}
}
