#include "EliteWarPage.h"

#include "defines.h"

#include "CfgMaster/EliteWarMapCfgMaster.h"
#include "CfgMaster/GlobalCfgMaster.h"

#include "basic/CommonDialog.h"
#include "basic/TextHelper.h"
#include "basic/NounHelper.h"

#include "game/guide/GuideControl.h"

#include "LevelObj.h"
#include "LevelControl.h"
#include "FightControl.h"
#include "game/warmap/WarMapCell.h"

#define ELITE_WAR_CELL_NUM 5
#define CELL_PREFIX_NAME "elite_war_cell_"
#define POINT_PREFIX_NAME "cell_"

EliteWarPage::EliteWarPage()
:m_pChapterMapImg(NULL)
,m_pEliteWarCells(NULL)
,m_uEliteWarChapterID(0)
{
}

EliteWarPage::~EliteWarPage()
{
	m_pEliteWarCells->removeAllObjects();
	CC_SAFE_RELEASE_NULL(m_pEliteWarCells);
}

EliteWarPage* EliteWarPage::create() {
	EliteWarPage *pRet = new EliteWarPage();

	if(pRet != NULL && pRet->init()) {
		pRet->autorelease();
	} else {
		CC_SAFE_DELETE(pRet);
	}

	return pRet;
}

bool EliteWarPage::init() {
	bool bRet = false;

	do {
		CC_BREAK_IF(!BaseLayout::init());

		m_pEliteWarCells = CCArray::create();
		CC_SAFE_RETAIN(m_pEliteWarCells);

		initLayoutFromJsonFile("ui_v2/EliteWarPageUI.ExportJson");
		addChild(m_pRootLayout);

		m_pChapterMapImg = dynamic_cast<UIImageView*>(m_pRootLayout->getChildByName("map_img"));
		m_pChapterMapImg->setEnabled(false);

		for(int i = 0; i < ELITE_WAR_CELL_NUM; i++) {
			EliteWarMapCell *pCell = EliteWarMapCell::create();
			pCell->addTouchEventListener(this, toucheventselector(EliteWarPage::touchEventHandler));
			string cellName = string(CELL_PREFIX_NAME) + intToString(i);
			pCell->setName(cellName.c_str());
			string ptName = string(POINT_PREFIX_NAME) + intToString(i + 1);
			UIImageView *pPathPoint = dynamic_cast<UIImageView*>(m_pRootLayout->getChildByName(ptName.c_str()));
			pPathPoint->setVisible(false);
			pPathPoint->setEnabled(false);
			pCell->setPosition(pPathPoint->getPosition());
			m_pRootLayout->addChild(pCell, pPathPoint->getZOrder());
		}

		bRet = true;
	} while(0);

	return bRet;
}

unsigned int EliteWarPage::getEliteWarChapterID() {
	return m_uEliteWarChapterID;
}

void EliteWarPage::setEliteWarChapterID(unsigned int uEliteWarChapterID) {
	m_uEliteWarChapterID = uEliteWarChapterID;

	//string url = string("chapter_map/chapter") + intToString(m_uEliteWarChapterID) + "_map_bg.png";
	/*string url = "chapter_map/war_page_bg.png";
	m_pChapterMapImg->loadTexture(url.c_str());*/
}

void EliteWarPage::updateUI() {
	const vmsg::CSEliteWarInfo &stEliteWarInfo = P_LEVEL_OBJ->getEliteWarInfo();
	unsigned int uFightMapSize = stEliteWarInfo.fightmapids_size();
	const vector<EliteWarMapCfg*> &stChapter = ELITE_WAR_MAP_CFG_MASTER->getEliteWarChapter(m_uEliteWarChapterID);
	
	for(unsigned int i = 0; i < stChapter.size(); i++) {
		string cellName = string(CELL_PREFIX_NAME) + intToString(i);
		EliteWarMapCell *pCell = dynamic_cast<EliteWarMapCell*>(m_pRootLayout->getChildByName(cellName.c_str()));
		pCell->setWarMapID(stChapter[i]->id());

		if(i < uFightMapSize) {
			pCell->setWarStat(WAR_STAT_PASSED);
		} else if(i == uFightMapSize) {
			pCell->setWarStat(WAR_STAT_OPENED);
		} else {
			pCell->setWarStat(WAR_STAT_LOCKED);
		}
	}
}

UIWidget* EliteWarPage::getCurWarMapCell() {
	UIWidget *pRet = NULL;

	const vmsg::CSEliteWarInfo &stEliteWarInfo = P_LEVEL_OBJ->getEliteWarInfo();
	unsigned int uFightMapSize = stEliteWarInfo.fightmapids_size();
	const vector<EliteWarMapCfg*> &stChapter = ELITE_WAR_MAP_CFG_MASTER->getEliteWarChapter(m_uEliteWarChapterID);

	if(uFightMapSize < stChapter.size()) {
		string cellName = string(CELL_PREFIX_NAME) + intToString(uFightMapSize);
		EliteWarMapCell *pCell = dynamic_cast<EliteWarMapCell*>(m_pRootLayout->getChildByName(cellName.c_str()));
		pRet = pCell;
	}
	
	return pRet;
}

void EliteWarPage::touchEventHandler(CCObject *pSender, TouchEventType type) {
	UIWidget *pWidget = dynamic_cast<UIWidget*>(pSender);
	const char *pszWidgetName = pWidget->getName();

	switch(type) {
		case TOUCH_EVENT_ENDED:
			if(string(pszWidgetName).substr(0, string(CELL_PREFIX_NAME).size()) == CELL_PREFIX_NAME) {
				EliteWarMapCell *pCell = dynamic_cast<EliteWarMapCell*>(pWidget);

				if(pCell != NULL) {
					GUIDE_TOUCH(pCell);
					P_LEVEL_CTL->getEliteWarEntryPanel()->openWithEliteWarMapID(pCell->getWarMapID());
				}
			}
			break;
		default:;
	}
}

void EliteWarPage::onDialog() {
	P_FIGHT_CTRL->getFightProxy()->sendResetEliteWarMapRqst(m_uEliteWarChapterID);
}
