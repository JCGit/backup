#include "WarPage.h"

#include "defines.h"

#include "CfgMaster/WarMapCfgMaster.h"

#include "basic/CommonDialog.h"
#include "basic/TextHelper.h"
#include "basic/NounHelper.h"

#include "game/fight/FightControl.h"
#include "game/fight/LevelObj.h"
#include "game/fight/LevelControl.h"
#include "game/task/TaskControl.h"

#include "WarMapCell.h"

#define LEVEL_ARMATURE_TAG 101
#define CELL_PREFIX_NAME "war_map_cell_"

WarPage::WarPage()
:m_uChapterID(0)
,m_pChapterMapImg(NULL)
,m_pWarCells(NULL)
,m_bIsTaskTrace(false)
,m_pCurLevelWidget(NULL)
{
}

WarPage::~WarPage()
{
	m_pWarCells->removeAllObjects();
	CC_SAFE_RELEASE_NULL(m_pWarCells);
}

WarPage* WarPage::create(unsigned int uChapterID) {
	WarPage *pRet = new WarPage();

	if(pRet != NULL && pRet->initWithChapterID(uChapterID)) {
		pRet->autorelease();
	} else {
		CC_SAFE_DELETE(pRet);
	}

	return pRet;
}

bool WarPage::initWithChapterID(unsigned int uChapterID) {
	bool bRet = false;

	do {
		CC_BREAK_IF(!BaseLayout::init());

		m_pWarCells = CCArray::create();
		CC_SAFE_RETAIN(m_pWarCells);

		//string jsonURL = "ui_v2/WarPage" + intToString(uChapterID == 1 ? 1 : 2) + "UI.ExportJson";
		string jsonURL = "ui_v2/WarPageUI.ExportJson";
		initLayoutFromJsonFile(jsonURL.c_str());
		addChild(m_pRootLayout);

		m_uChapterID = uChapterID;
		m_pChapterMapImg = dynamic_cast<UIImageView*>(m_pRootLayout->getChildByName("map_img"));
		m_pChapterMapImg->setEnabled(false);
		//string url = string("chapter_map/chapter") + intToString(m_uChapterID) + "_map_bg.png";
		/*string url = "chapter_map/war_page_bg.png";
		m_pChapterMapImg->loadTexture(url.c_str());*/

		const vector<WarMapCfg*>* pChapter = WAR_MAP_CFG->getWarChapter(m_uChapterID);

		if(pChapter != NULL) {
			for(unsigned int i = 0; i < pChapter->size(); i++){
				string name = "war_cell_" + to_string(i + 1);
				UIImageView* pCellPoint = dynamic_cast<UIImageView*>(m_pRootLayout->getChildByName(name.c_str()));
                if(NULL == pCellPoint) {
                    CCLOG("[warn]WarPage::initWithChapterID, fail to get child! name %s", name.c_str() );
                    continue;
                }
				pCellPoint->setEnabled(false);
				pCellPoint->setVisible(false);
				m_pWarCells->addObject(pCellPoint);
			}
		}

		bRet = true;
	} while(0);

	return bRet;
}

unsigned int WarPage::getChapterID() {
	return m_uChapterID;
}

void WarPage::setChapterID(unsigned int uChapterID) {
	m_uChapterID = uChapterID;
}

void WarPage::updateUI(){
	const vector<WarMapCfg*>* pChapter = WAR_MAP_CFG->getWarChapter(m_uChapterID);

	if(pChapter == NULL) {
		return;
	}

	//unsigned int uChapterSize = pChapter->size();
	unsigned int uChapterSize = 15;

	unsigned int maxLevelNum = P_LEVEL_OBJ->getMapInfo().toid() - P_LEVEL_OBJ->getMapInfo().fromid() + 1;
	unsigned int maxPage = ceil(maxLevelNum/(float)(uChapterSize));
	unsigned int maxLevel = P_LEVEL_OBJ->getMapInfo().newestwarmapid();  //最新通关的ID
	unsigned int fromId = pChapter->front()->id();

	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("ui_v2/war_map.plist");
	CCPoint stPos = CCPointZero;
	int iZorder = 0;
	bool bCurLevInPage = false;		//最新关卡是否在当前页面

	for(unsigned int i = 0; i < uChapterSize; i++) {
		UIImageView *pCellPoint = dynamic_cast<UIImageView*>(m_pWarCells->objectAtIndex(i));
        if(NULL == pCellPoint) {
            CCLOG("[warn]WarPage::updateUI,cell is null! index %d", i);
            continue;
        }
		WarMapCfg* pWarMapCfg = WAR_MAP_CFG->GetWarMapCfgByID(fromId + i);

		string cellName = string(CELL_PREFIX_NAME) + intToString(i);
		UIWidget *pWidget = m_pRootLayout->getChildByName(cellName.c_str());
		unsigned int cnt = m_pRootLayout->getChildrenCount();
		WarMapCell *pCell = dynamic_cast<WarMapCell*>(m_pRootLayout->getChildByName(cellName.c_str()));

		if(pCell == NULL) {
			if(/*pWarMapCfg->bossid() != 0*/(i+1) % 3 == 0) {
				pCell = StageWarMapCell::create();
			} else {
				pCell = WarMapCell::create();
			}

			pCellPoint->setEnabled(false);
			pCell->setName(cellName.c_str());
			pCell->setWarMapID(fromId + i);
			pCell->setPosition(pCellPoint->getPosition());
			m_pRootLayout->addChild(pCell, pCellPoint->getZOrder());
			pCell->addTouchEventListener(this, toucheventselector(WarPage::touchEventHandler));
		}

		if(fromId + i <= maxLevel) {
			pCell->setWarStat(WAR_STAT_PASSED);
			const vmsg::CSWarMapInfo &stWarMapInfo = P_LEVEL_OBJ->getMapInfo().infos().Get(fromId + i - 1);
			pCell->setStarNum(stWarMapInfo.score());
		} else if(fromId + i == maxLevel + 1) {
			pCell->setWarStat(WAR_STAT_OPENED);
			//m_bIsTaskTrace = true;
			m_pCurLevelWidget = pCell;
			bCurLevInPage = true;
			stPos = pCell->getPosition();
			iZorder = pCell->getZOrder();
			pCell->setStarNum(0);
		} else {
			pCell->setWarStat(WAR_STAT_LOCKED);
			pCell->setStarNum(0);
		}
	}

	if(m_pRootLayout->getNodeByTag(LEVEL_ARMATURE_TAG) != NULL){
		m_pRootLayout->removeNodeByTag(LEVEL_ARMATURE_TAG);
	}

	if(bCurLevInPage == false){
		return;
	}

	WarMapCfg* stCfg = WAR_MAP_CFG->GetWarMapCfgByID(maxLevel + 1);
	string armStr = "";
	CCPoint armPos;
	if(stCfg->bossid() != 0){
		armStr = "ut00075";			//boss 关卡特效
		armPos = stPos - ccp(0, 30);
		iZorder = iZorder - 2;
	}else{
		armStr = "ut00004";
		armPos = stPos;
	}


	//neilwu,改成，仅当任务自动寻路打开时才显示动画特效
	if(m_bIsTaskTrace && armStr.size() > 0) {
		CCArmature* pLevArmatrue = CCArmature::create(armStr.c_str());
		m_pRootLayout->addNode(pLevArmatrue, iZorder, LEVEL_ARMATURE_TAG);
		pLevArmatrue->setPosition(armPos);
		pLevArmatrue->getAnimation()->play("fly", -1, -1, 1);
	}

}

Widget* WarPage::getCurLevelCell() {
	Widget *pRet = NULL;

	if(m_pCurLevelWidget != NULL && m_pCurLevelWidget->isRunning() && m_pCurLevelWidget->isTouchEnabled()) {
		pRet = m_pCurLevelWidget;
	}

	return pRet;
}


void WarPage::touchEventHandler(CCObject *pSender, TouchEventType type) {
	UIWidget *pWidget = dynamic_cast<UIWidget*>(pSender);
	const char *pszWidgetName = pWidget->getName();

	switch(type) {
		case TOUCH_EVENT_ENDED:
			if(string(pszWidgetName).substr(0, string(CELL_PREFIX_NAME).size()) == CELL_PREFIX_NAME) {
				WarMapCell *pCell = dynamic_cast<WarMapCell*>(pWidget);

				if(pCell != NULL) {
					GUIDE_TOUCH(pWidget);
					unsigned int uMaxOpenWarID = P_TASK_CTRL->getMaxOpenWarID();
					/*P_LEVEL_CTL->getEnterPanel()->open();
					P_LEVEL_CTL->getEnterPanel()->setLevelID(pCell->getLevelID());*/
					if(pCell->getWarMapID() <= uMaxOpenWarID) {
						P_LEVEL_CTL->getEnterPanel()->open();
						P_LEVEL_CTL->getEnterPanel()->setLevelID(pCell->getWarMapID());
					} else {
						COMMON_DIALOG->show(TextStr(TEXT_NOT_RECV_THE_TASK), this, NounStr(OK), NounStr(CANCEL), NULL);
					}
				}
			}
			break;
		default:;
	}
}
