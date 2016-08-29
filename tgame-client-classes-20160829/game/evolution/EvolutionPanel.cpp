#include "EvolutionPanel.h"

#include "defines.h"
#include "Resources.h"

#include "CfgMaster/GlobalCfgMaster.h"
#include "CfgMaster/EvolutionSkillCfgMaster.h"
#include "CfgMaster/SkillCfgMaster.h"
#include "CfgMaster/EvolutionSkillExpCfgMaster.h"
#include "CfgMaster/EvolutionSkillGrowCfgMaster.h"

#include "basic/NounHelper.h"
#include "basic/TextHelper.h"
#include "basic/CommonDialog.h"
#include "basic/ToolUtils.h"

#include "game/UserObj.h"
#include "game/bag/BagControl.h"
#include "tip/TipControl.h"

#include "EvolutionControl.h"
#include "EvolutionPage.h"
#include "EvolutionCell.h"
#include "EvolutionSkillItem.h"

#define MAX_CELLS_PER_PAGE 16

EvolutionPanel::EvolutionPanel()
:m_pBackBtn(NULL)
,m_pTabListView(NULL)
,m_pEvolutionLayout(NULL)
,m_pStarPage(NULL)
,m_pPageNumTxt(NULL)
,m_pNeedLevelTxt(NULL)
,m_pNeedCoinValTxt(NULL)
,m_pNeedPowerStoneValTxt(NULL)
,m_pEvolutionBtn(NULL)
,m_pSetSkillLayout(NULL)
,m_pSkillIconImg(NULL)
,m_pSkillDescValTxt(NULL)
,m_pSkillExpProgress(NULL)
,m_pSkillExpProgressTxt(NULL)
,m_pSetSkillBtn(NULL)
,m_pAttrAddLayout(NULL)
,m_pHpValTxt(NULL)
,m_pPowerValTxt(NULL)
,m_pAttackValTxt(NULL)
,m_pDefenseValTxt(NULL)
,m_pDodgeDistanceValTxt(NULL)
,m_pSkillsLayout(NULL)
,m_pSkillListView(NULL)
,m_pAttrTipLayout(NULL)
,m_pAttrTxt(NULL)
,m_pAttrValTxt(NULL)
,m_uNextEvolutionID(0)
,m_uNeedLevel(0)
,m_uNeedCoin(0)
,m_uNeedStone(0)
,m_uTabType(TAB_EVOLUTION)
,m_nTabSelectedIdx(0)
,m_nSkillSelectedIdx(0)
,m_uStoneCnt(0)
{
}

EvolutionPanel::~EvolutionPanel()
{
	//CToolUtils::removeArmatureFile("effect/ut00019/ut00019.ExportJson");
}

EvolutionPanel* EvolutionPanel::create() {
	EvolutionPanel *pRet = new EvolutionPanel();

	if(pRet != NULL && pRet->init()) {
		pRet->autorelease();
	} else {
		CC_SAFE_DELETE(pRet);
	}

	return pRet;
}

bool EvolutionPanel::init() {
	bool bRet = false;

	do {
		CC_BREAK_IF(!BasePanel::init());
		
		setShowMoneyTopBar(true);

		initUI();

		bRet = true;
	} while(0);

	return bRet;
}

void EvolutionPanel::open() {
	BasePanel::open();

	P_EVOLUTION_CTRL->sendQueryEvolutionInfoRqst();
	P_EVOLUTION_CTRL->sendQueryEvolutionSkillInfoRqst();
	P_BAG_CTRL->sendQueryBagInfoRqst();

	showLoading();

	if(!m_bUIInited) {
		initUI();
	}
}

void EvolutionPanel::onExit() {

	BasePanel::onExit();
}

CCNode* EvolutionPanel::getGuideNodeByName(const std::string &name) {
	CCNode* pRet = NULL;
	if(name == "evolution_btn") {
		pRet = m_pEvolutionBtn;
	}

	return pRet;
}

void EvolutionPanel::initUI() {
	initLayoutFromJsonFile("ui_v2/EvolutionPanelUI.ExportJson");
	addChild(m_pRootLayout);

	m_pBackBtn = dynamic_cast<UIButton*>(m_pRootLayout->getChildByName("back_btn"));
	m_pBackBtn->addTouchEventListener(this, toucheventselector(EvolutionPanel::touchEventHandler));

	m_pTabListView = dynamic_cast<UIListView*>(m_pRootLayout->getChildByName("tab_list"));
	m_pTabListView->addEventListenerListView(this, listvieweventselector(EvolutionPanel::listViewEventHandler));

	m_pEvolutionLayout = dynamic_cast<UILayout*>(m_pRootLayout->getChildByName("evolution_layout"));

	UIImageView *pBgStarsImg = dynamic_cast<UIImageView*>(m_pEvolutionLayout->getChildByName("bg_stars_img"));
	pBgStarsImg->loadTexture("ui_bg/bg_stars.png");

	m_pStarPage = dynamic_cast<UIPageView*>(m_pEvolutionLayout->getChildByName("star_page"));
	m_pStarPage->addEventListenerPageView(this, pagevieweventselector(EvolutionPanel::pageViewEventHandler));

	m_pPageNumTxt = dynamic_cast<UILabel*>(m_pEvolutionLayout->getChildByName("page_num_txt"));

	m_pNeedLevelTxt = dynamic_cast<UILabel*>(m_pEvolutionLayout->getChildByName("need_level_txt"));
	m_pNeedLevelTxt->setVisible(false);

	m_pNeedCoinValTxt = dynamic_cast<UILabel*>(m_pEvolutionLayout->getChildByName("need_coin_val_txt"));

	m_pNeedPowerStoneValTxt = dynamic_cast<UILabel*>(m_pEvolutionLayout->getChildByName("need_power_stone_val_txt"));

	m_pEvolutionBtn = dynamic_cast<UIButton*>(m_pEvolutionLayout->getChildByName("evolution_btn"));
	m_pEvolutionBtn->addTouchEventListener(this, toucheventselector(EvolutionPanel::touchEventHandler));

	m_pSetSkillLayout = dynamic_cast<UILayout*>(m_pRootLayout->getChildByName("set_skill_layout"));

	m_pSkillIconImg = dynamic_cast<UIImageView*>(m_pSetSkillLayout->getChildByName("skill_icon_img"));

	m_pSkillDescValTxt = dynamic_cast<UILabel*>(m_pSetSkillLayout->getChildByName("skill_desc_val_txt"));

	m_pSkillExpProgress = dynamic_cast<UILoadingBar*>(m_pSetSkillLayout->getChildByName("skill_exp_progress"));

	m_pSkillExpProgressTxt = dynamic_cast<UILabel*>(m_pSetSkillLayout->getChildByName("skill_exp_progress_txt"));

	m_pSetSkillBtn = dynamic_cast<UIButton*>(m_pSetSkillLayout->getChildByName("set_skill_btn"));
	m_pSetSkillBtn->addTouchEventListener(this, toucheventselector(EvolutionPanel::touchEventHandler));

	m_pAttrAddLayout = dynamic_cast<UILayout*>(m_pRootLayout->getChildByName("attr_add_layout"));

	m_pHpValTxt = dynamic_cast<UILabel*>(m_pAttrAddLayout->getChildByName("hp_val_txt"));

	m_pPowerValTxt = dynamic_cast<UILabel*>(m_pAttrAddLayout->getChildByName("power_val_txt"));

	m_pAttackValTxt = dynamic_cast<UILabel*>(m_pAttrAddLayout->getChildByName("attack_val_txt"));

	m_pDefenseValTxt = dynamic_cast<UILabel*>(m_pAttrAddLayout->getChildByName("defense_val_txt"));

	m_pDodgeDistanceValTxt = dynamic_cast<UILabel*>(m_pAttrAddLayout->getChildByName("dodge_distance_val_txt"));

	m_pSkillsLayout = dynamic_cast<UILayout*>(m_pRootLayout->getChildByName("skills_layout"));

	m_pSkillListView = dynamic_cast<UIListView*>(m_pSkillsLayout->getChildByName("skill_list"));
	m_pSkillListView->addEventListenerListView(this, listvieweventselector(EvolutionPanel::listViewEventHandler));

	m_pAttrTipLayout = dynamic_cast<UILayout*>(m_pRootLayout->getChildByName("attr_tip_layout"));

	m_pAttrTxt = dynamic_cast<UILabel*>(m_pAttrTipLayout->getChildByName("attr_txt"));

	m_pAttrValTxt = dynamic_cast<UILabel*>(m_pAttrTipLayout->getChildByName("attr_val_txt"));

	//CCTexture2D::PVRImagesHavePremultipliedAlpha(true);
	//CCArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo("effect/ut00019/ut00019.ExportJson");

	switchTab(m_uTabType);
	m_bUIInited = true;

	if(m_bUIDelayed) {
		m_bUIDelayed = false;
		updateUI();
	}
}

void EvolutionPanel::switchTab(unsigned int uTabType) {
	if(uTabType >= TAB_MIN && uTabType <= TAB_MAX) {
		m_uTabType = uTabType;
		UIButton *pTabBtn = NULL;

		if(m_nTabSelectedIdx >= 0) {
			pTabBtn = dynamic_cast<UIButton*>(m_pTabListView->getItem(m_nTabSelectedIdx));
			pTabBtn->setBrightStyle(BRIGHT_NORMAL);
			pTabBtn->setTouchEnabled(true);
			pTabBtn->getTextVirtualRenderer()->setScale(1.0f);
			pTabBtn->setTitleColor(ccc3(0x7b, 0xbc, 0xe1));
		}

		pTabBtn = dynamic_cast<UIButton*>(m_pTabListView->getItem(m_uTabType));
		pTabBtn->setBrightStyle(BRIGHT_HIGHLIGHT);
		pTabBtn->setTouchEnabled(false);
		pTabBtn->getTextVirtualRenderer()->setScale(1.2f);
		pTabBtn->setTitleColor(ccc3(0xff, 0xfa, 0xae));
		m_nTabSelectedIdx = m_uTabType;

		m_pEvolutionLayout->setVisible(m_uTabType == TAB_EVOLUTION);
		m_pSetSkillLayout->setVisible(m_uTabType == TAB_SET_SKILL);
		m_pAttrAddLayout->setVisible(m_uTabType == TAB_EVOLUTION);
		m_pSkillsLayout->setVisible(m_uTabType == TAB_SET_SKILL);
		m_pEvolutionLayout->setEnabled(m_uTabType == TAB_EVOLUTION);
		m_pSetSkillLayout->setEnabled(m_uTabType == TAB_SET_SKILL);
		m_pAttrAddLayout->setEnabled(m_uTabType == TAB_EVOLUTION);
		m_pSkillsLayout->setEnabled(m_uTabType == TAB_SET_SKILL);
	}
}

void EvolutionPanel::updateUI() {
	if(m_bUIInited) {
		if(m_uTabType == TAB_EVOLUTION) {
			updateEvolution();
		} else if(m_uTabType == TAB_SET_SKILL) {
			updateSkillList();
		}
	} else {
		m_bUIDelayed = true;
	}
	GUIDE_SHOW(GUIDE_UI_EVOLUTION);
}

void EvolutionPanel::updateEvolution() {
	if(!m_bUIInited) {
		m_bUIDelayed = true;
		return;
	}

	unsigned int uMaxEvolutionID = EVOLUTION_SKILL_CFG_MASTER->GetCfg().evolutionskillcfg_size();
	unsigned int uEvolutionID = P_EVOLUTION_CTRL->getEvolutionID(); // EvolutionID代表已达成的进化，初始为0
	unsigned int uTotalPageNum = uEvolutionID < uMaxEvolutionID ? uEvolutionID / MAX_CELLS_PER_PAGE + 1 : uEvolutionID / MAX_CELLS_PER_PAGE;
	unsigned int uStartIdx = m_pStarPage->getPages()->count() * MAX_CELLS_PER_PAGE;
	int nPageIdx = -1;
	EvolutionPage *pEvolutionPage = NULL;
	string newSkillName = "";
	string newSkillDesc = "";

	for(unsigned int i = 0; i < uTotalPageNum * MAX_CELLS_PER_PAGE; i++) {
		if(i % MAX_CELLS_PER_PAGE == 0) {
			nPageIdx++;

			if(m_pStarPage->getPage(nPageIdx) == NULL) {
				std::string pageName = std::string("star_page_") + intToString(nPageIdx);
				//pEvolutionPage = EvolutionPage::create(dynamic_cast<UILayout*>(m_pRootLayout->getChildByName(pageName.c_str())));
				pEvolutionPage = EvolutionPage::create();
				pEvolutionPage->setName(pageName.c_str());
				m_pStarPage->addPage(pEvolutionPage);
			}
		}

		if(i / MAX_CELLS_PER_PAGE == nPageIdx) {
			pEvolutionPage = dynamic_cast<EvolutionPage*>(m_pStarPage->getPage(nPageIdx));
			string cellName = std::string("star_cell_") + intToString(i % MAX_CELLS_PER_PAGE);
			EvolutionCell *pCell = NULL;
			EvolutionSkillCfg *pEvolutionSkillCfg = EVOLUTION_SKILL_CFG_MASTER->getEvolutionSkillCfgByID(i+1);
			
			if((i + 1) % MAX_CELLS_PER_PAGE == 0) {
				pCell = dynamic_cast<EvolutionSkillCell*>(pEvolutionPage->getRootLayout()->getChildByName(cellName.c_str()));
				
				dynamic_cast<EvolutionSkillCell*>(pCell)->setSkill(pEvolutionSkillCfg->skillid());
				const SkillCfg *pSkillCfg = SKILL_CFG_MASTER->GetSkillCfgByID(pEvolutionSkillCfg->skillid());

				if(pSkillCfg != NULL) {
					newSkillName = pSkillCfg->name();
					newSkillDesc = pSkillCfg->desc();
				}
			} else {
				pCell = dynamic_cast<EvolutionStarCell*>(pEvolutionPage->getRootLayout()->getChildByName(cellName.c_str()));
			}

			// calc attr addition
			string attrName = "";
			unsigned int attrAddVal = 0;

			if((i + 1) % MAX_CELLS_PER_PAGE != 0) { // 每页最后一个为技能，不包含属性加成
				if(i == 0) {
					if(pEvolutionSkillCfg->hp() > 0) {
						attrName = NounStr(NOUN_HP);
						attrAddVal = pEvolutionSkillCfg->hp();
					} else if(pEvolutionSkillCfg->mp() > 0) {
						attrName = NounStr(NOUN_MP);
						attrAddVal = pEvolutionSkillCfg->mp();
					} else if(pEvolutionSkillCfg->atk() > 0) {
						attrName = NounStr(NOUN_ATTACK);
						attrAddVal = pEvolutionSkillCfg->atk();
					} else if(pEvolutionSkillCfg->def() > 0) {
						attrName = NounStr(NOUN_DEFENSE);
						attrAddVal = pEvolutionSkillCfg->def();
					} else if(pEvolutionSkillCfg->dodgedist() > 0) {
						attrName = NounStr(NOUN_DODGE_DISTANCE);
						attrAddVal = pEvolutionSkillCfg->dodgedist();
					} else {
						attrName = "";
						attrAddVal = 0;
					}
				} else {
					EvolutionSkillCfg *pPrevEvolutionSkillCfg = EVOLUTION_SKILL_CFG_MASTER->getEvolutionSkillCfgByID(i);

					if(pEvolutionSkillCfg->hp() > pPrevEvolutionSkillCfg->hp()) {
						attrName = NounStr(NOUN_HP);
						attrAddVal = pEvolutionSkillCfg->hp() - pPrevEvolutionSkillCfg->hp();
					} else if(pEvolutionSkillCfg->mp() > pPrevEvolutionSkillCfg->mp()) {
						attrName = NounStr(NOUN_MP);
						attrAddVal = pEvolutionSkillCfg->mp() - pPrevEvolutionSkillCfg->mp();
					} else if(pEvolutionSkillCfg->atk() > pPrevEvolutionSkillCfg->atk()) {
						attrName = NounStr(NOUN_ATTACK);
						attrAddVal = pEvolutionSkillCfg->atk() - pPrevEvolutionSkillCfg->atk();
					} else if(pEvolutionSkillCfg->def() > pPrevEvolutionSkillCfg->def()) {
						attrName = NounStr(NOUN_DEFENSE);
						attrAddVal = pEvolutionSkillCfg->def() - pPrevEvolutionSkillCfg->def();
					} else if(pEvolutionSkillCfg->dodgedist() > pPrevEvolutionSkillCfg->dodgedist()) {
						attrName = NounStr(NOUN_DODGE_DISTANCE);
						attrAddVal = pEvolutionSkillCfg->dodgedist() - pPrevEvolutionSkillCfg->dodgedist();
					} else {
						attrName = "";
						attrAddVal = 0;
					}
				}

				// set color
				EvolutionStarCell::StarColor color = EvolutionStarCell::STAR_GREEN;

				if(attrName == NounStr(NOUN_HP)) {
					color = EvolutionStarCell::STAR_GREEN;
				} else if(attrName == NounStr(NOUN_MP)) {
					color = EvolutionStarCell::STAR_BLUE;
				} else if(attrName == NounStr(NOUN_ATTACK)) {
					color = EvolutionStarCell::STAR_YELLOW;
				} else if(attrName == NounStr(NOUN_DEFENSE)) {
					color = EvolutionStarCell::STAR_PURPLE;
				} else if(attrName == NounStr(NOUN_DODGE_DISTANCE)) {
					color = EvolutionStarCell::STAR_RED;
				}

				dynamic_cast<EvolutionStarCell*>(pCell)->setColor(color);
			}

			if(i < uEvolutionID) {
				pCell->setStat(EvolutionCell::CELL_LIGHT);
			} else if(i == uEvolutionID) {
				pCell->setStat(EvolutionCell::CELL_NEXT);

				// show attr addition
				m_pAttrTxt->setText(attrName);
				m_pAttrValTxt->setText(string("+") + intToString(attrAddVal));
				m_pAttrValTxt->setPositionX(m_pAttrTxt->getPositionX() + m_pAttrTxt->getSize().width + 12);
				m_pAttrTipLayout->setSize(CCSizeMake(m_pAttrTxt->getSize().width + m_pAttrValTxt->getSize().width + 44, m_pAttrTipLayout->getSize().height));
				CC_SAFE_RETAIN(m_pAttrTipLayout);
				m_pAttrTipLayout->removeFromParent();
				float offsetX = pCell->getPositionX() > m_pStarPage->getSize().width / 2 ? 8 - m_pAttrTipLayout->getSize().width : -8;
				float posX = pCell->getPositionX() + offsetX;
				float posY = pCell->getPositionY() - m_pAttrTipLayout->getSize().height / 2;
				m_pAttrTipLayout->setPosition(ccp(posX, posY));
				pCell->getParent()->addChild(m_pAttrTipLayout);
				CC_SAFE_RELEASE(m_pAttrTipLayout);
				m_pAttrTipLayout->setVisible(uEvolutionID == 0 || (uEvolutionID + 1) % MAX_CELLS_PER_PAGE != 0);

				// evolution condition
				m_uNeedLevel = pEvolutionSkillCfg->needlevel();
				m_uNeedCoin = pEvolutionSkillCfg->needcoin();
				m_uNeedStone = pEvolutionSkillCfg->needpowerstone();
				unsigned int uStoneID = GLOBAL_CFG_MASTER->GetCfg().powerstoneid();
				const UserInfo *pUserInfo = P_USER_OBJ->getUserInfo();
				unsigned int uLevel = pUserInfo->level;
				UINT64_t uCoin = pUserInfo->coin;
				unsigned int uStoneCnt = P_BAG_CTRL->getItemCnt(uStoneID);
				m_uStoneCnt = uStoneCnt;

				char text[100];
				snprintf(text, 100, TextStr(TEXT_EVOLUTION_LEVLE_LMITED_DESC), m_uNeedLevel);
				m_pNeedLevelTxt->setText(text);
				m_pNeedLevelTxt->setVisible(m_uNeedLevel > uLevel);
				m_pNeedCoinValTxt->setText(intToString(m_uNeedCoin));
				m_pNeedPowerStoneValTxt->setText(intToString(uStoneCnt) + "/" + intToString(m_uNeedStone));
				m_pNeedCoinValTxt->setColor(m_uNeedCoin <= uCoin ? ccc3(0x00, 0xff, 0x00) : ccc3(0xff, 0x00, 0x00));
				m_pNeedPowerStoneValTxt->setColor(m_uNeedStone <= uStoneCnt ? ccc3(0x00, 0xff, 0x00) : ccc3(0xff, 0x00, 0x00));

				m_pEvolutionBtn->setTouchEnabled(m_uNeedLevel <= uLevel && m_uNeedCoin <= uCoin && m_uNeedStone <= uStoneCnt);
				m_pEvolutionBtn->setBright(m_pEvolutionBtn->isTouchEnabled());
			} else {
				pCell->setStat(EvolutionCell::CELL_DARK);
			}
		}
	}

	// total added
	EvolutionSkillCfg *pCurEvolutionSkillCfg = EVOLUTION_SKILL_CFG_MASTER->getEvolutionSkillCfgByID(uEvolutionID);

	if(pCurEvolutionSkillCfg != NULL) {
		m_pHpValTxt->setText(getAddText(pCurEvolutionSkillCfg->hp()));
		m_pPowerValTxt->setText(getAddText(pCurEvolutionSkillCfg->mp()));
		m_pAttackValTxt->setText(getAddText(pCurEvolutionSkillCfg->atk()));
		m_pDefenseValTxt->setText(getAddText(pCurEvolutionSkillCfg->def()));
		m_pDodgeDistanceValTxt->setText(getAddText(pCurEvolutionSkillCfg->dodgedist()));
	} else {
		m_pHpValTxt->setText(getAddText(0));
		m_pPowerValTxt->setText(getAddText(0));
		m_pAttackValTxt->setText(getAddText(0));
		m_pDefenseValTxt->setText(getAddText(0));
		m_pDodgeDistanceValTxt->setText(getAddText(0));
	}

	m_pStarPage->scrollToPage(uTotalPageNum - 1);

	closeLoading();
}

void EvolutionPanel::updateSkillList() {
	if(!m_bUIInited) {
		m_bUIDelayed = true;
		return;
	}

	m_pSkillListView->removeAllItems();

	const std::vector<vmsg::CSEvolutionSkill> &stSkills = P_EVOLUTION_CTRL->getSkills();
	unsigned int uUsedSkillIdx = P_EVOLUTION_CTRL->getUsedSkillIdx();

	for(unsigned int i = 0; i < stSkills.size(); i++) {
		EvolutionSkillItem *pItem = EvolutionSkillItem::create();
		pItem->updateUI(stSkills[i]);
		m_pSkillListView->pushBackCustomItem(pItem);

		if(i == uUsedSkillIdx) {
			m_nSkillSelectedIdx = i;
			pItem->selected(true);
		}
	}

	showSkill(uUsedSkillIdx);

	closeLoading();
}

void EvolutionPanel::showSkill(unsigned int uSkillIdx) {
	const std::vector<vmsg::CSEvolutionSkill> &stSkills = P_EVOLUTION_CTRL->getSkills();
	unsigned int uTotalSkills = stSkills.size();
	
	if(uTotalSkills > uSkillIdx) {
		const vmsg::CSEvolutionSkill &stSkill = stSkills[uSkillIdx];

		EvolutionSkillGrowCfg *psteSkillGrowCfg = EVOLUTION_SKILL_GROW_CFG_MASTER->getEvolutionSkillGrowCfgByID(stSkill.startskillid());
		EvolutionSkillExpCfg *pstSkillExpCfg = EVOLUTION_SKILL_EXP_CFG_MASTER->getEvolutionSkillExpCfgByID(stSkill.level());
		unsigned int uSkillID = psteSkillGrowCfg->skillids(stSkill.level() - 1);

		const SkillCfg *pstSkillCfg = SKILL_CFG_MASTER->GetSkillCfgByID(uSkillID);
		const SkillCfg *pstNextSkillCfg = NULL;

		if(stSkill.level() < (unsigned int)psteSkillGrowCfg->skillids_size()) {
			unsigned int uNextSkillID = psteSkillGrowCfg->skillids(stSkill.level());
			pstNextSkillCfg = SKILL_CFG_MASTER->GetSkillCfgByID(uNextSkillID);
		}
		std::string path = SKILLICON_RES_PATH + pstSkillCfg->skillicon() + ".png";
		m_pSkillIconImg->setVisible(true);
		m_pSkillIconImg->loadTexture(path.c_str());
		m_pSkillDescValTxt->setText(pstSkillCfg->desc());
		m_pSkillExpProgress->setPercent(stSkill.exp() * 100 / pstSkillExpCfg->exp());
		m_pSkillExpProgressTxt->setText(intToString(stSkill.exp()) + "/" + intToString(pstSkillExpCfg->exp()));
		unsigned int uUsedSkillIdx = P_EVOLUTION_CTRL->getUsedSkillIdx();
		m_pSetSkillBtn->setBright(uSkillIdx != uUsedSkillIdx);
		m_pSetSkillBtn->setTouchEnabled(uSkillIdx != uUsedSkillIdx);
	} else {
		m_pSkillIconImg->setVisible(false);
		m_pSkillDescValTxt->setText("");
		m_pSkillExpProgress->setPercent(0);
		m_pSkillExpProgressTxt->setText(intToString(0) + "/" + intToString(100));
		m_pSetSkillBtn->setBright(false);
		m_pSetSkillBtn->setTouchEnabled(false);
	}
}

void EvolutionPanel::updateStone() {
	unsigned int uStoneID = GLOBAL_CFG_MASTER->GetCfg().powerstoneid();
	unsigned int uCurStoneCnt = P_BAG_CTRL->getItemCnt(uStoneID);
	if(m_uStoneCnt == uCurStoneCnt){
		return;
	}


	updateUI();
}

void EvolutionPanel::evolve() {
	// check
	unsigned int uStoneID = GLOBAL_CFG_MASTER->GetCfg().powerstoneid();

	if(m_uNeedLevel > P_USER_OBJ->getUserInfo()->level) {
		COMMON_DIALOG->show(TextStr(TEXT_LEVEL_UNREACHED), this, NounStr(OK), NounStr(CANCEL), NULL);
	} else if(!P_TIP_CTRL->CoinCostTip(m_uNeedCoin, this)) {
		
	}  else if(m_uNeedStone > P_BAG_CTRL->getItemCnt(uStoneID)) {
		COMMON_DIALOG->show(TextStr(TEXT_EVOLUTION_STONE_NOT_ENOUGH), this, NounStr(OK), NounStr(CANCEL), NULL);
	} else {
		P_EVOLUTION_CTRL->sendEvolutionSkillRqst();

	}
}
 
void EvolutionPanel::touchEventHandler(CCObject *pSender, TouchEventType type) {
	UIWidget *pWidget = dynamic_cast<UIWidget*>(pSender);
	const char *pszWidgetName = pWidget->getName();

	switch(type) {
		case TOUCH_EVENT_ENDED:
			if(strcmp(pszWidgetName, m_pBackBtn->getName()) == 0) {
				close();
			} else if(strcmp(pszWidgetName, "left_flip_btn") == 0) {
				int nPageIdx = m_pStarPage->getCurPageIndex();

				if(nPageIdx > 0) {
					m_pStarPage->scrollToPage(nPageIdx - 1);
				}
			} else if(strcmp(pszWidgetName, "right_flip_btn") == 0) {
				int nPageIdx = m_pStarPage->getCurPageIndex();
				unsigned int uTotalPageNum = m_pStarPage->getPages()->count();

				if(nPageIdx < (int)(uTotalPageNum - 1)) {
					m_pStarPage->scrollToPage(nPageIdx + 1);
				}
			} else if(strcmp(pszWidgetName, m_pEvolutionBtn->getName()) == 0) {
				GUIDE_TOUCH(pWidget);
				//m_pEvolutionBtn->setTouchEnabled(false);
				P_EVOLUTION_CTRL->sendEvolutionSkillRqst();
				P_BAG_CTRL->sendQueryBagInfoRqst();
			} else if(strcmp(pszWidgetName, m_pSetSkillBtn->getName()) == 0) { // 设置进化技
				if(m_pSkillListView->getCurSelectedIndex() >= 0) {
					P_EVOLUTION_CTRL->sendChangeEvolutionSkillRqst(m_nSkillSelectedIdx);
				}
			}
			break;
		default:;
	}
}

void EvolutionPanel::pageViewEventHandler(CCObject *pSender, PageViewEventType type) {
	UIPageView *pPageView = dynamic_cast<UIPageView*>(pSender);
	const char *pszPageName = pPageView->getName();
	
	switch(type) {
		case PAGEVIEW_EVENT_TURNING:
			if(std::strcmp(pszPageName, m_pStarPage->getName()) == 0) {
				int nPageIdx = m_pStarPage->getCurPageIndex();
				unsigned int uTotalPageNum = m_pStarPage->getPages()->count();
				m_pPageNumTxt->setText(intToString(nPageIdx + 1) + "/" + intToString(uTotalPageNum));
				/*m_pLeftFlipBtn->setBright(nPageIdx != 0);
				m_pLeftFlipBtn->setTouchEnabled(nPageIdx != 0);
				m_pRightFlipBtn->setBright(nPageIdx != (uTotalPageNum - 1));
				m_pRightFlipBtn->setTouchEnabled(nPageIdx != (uTotalPageNum - 1));*/
			}
			break;
		default:;
	}
}

void EvolutionPanel::listViewEventHandler(CCObject *pSender, ListViewEventType type) {
	UIListView *pListView = dynamic_cast<UIListView*>(pSender);
	const char *pszListViewName = pListView->getName();

	switch(type) {
		case LISTVIEW_ONSELECTEDITEM_END:
			if(strcmp(pszListViewName, m_pTabListView->getName()) == 0) {
				int nSelectedIdx = m_pTabListView->getCurSelectedIndex();
				switchTab(nSelectedIdx);
				updateUI();
			} else if(strcmp(pszListViewName, m_pSkillListView->getName()) == 0) {
				int nSelectedIdx = m_pSkillListView->getCurSelectedIndex();
				EvolutionSkillItem *pItem = NULL;

				if(m_nSkillSelectedIdx >= 0) {
					pItem = dynamic_cast<EvolutionSkillItem*>(m_pSkillListView->getItem(m_nSkillSelectedIdx));
					pItem->selected(false);
				}
				
				pItem = dynamic_cast<EvolutionSkillItem*>(m_pSkillListView->getItem(nSelectedIdx));
				pItem->selected(true);
				m_nSkillSelectedIdx = nSelectedIdx;

				showSkill(m_nSkillSelectedIdx);
			}
			break;
		default:;
	}
}

std::string EvolutionPanel::getAddText(unsigned int uAddVal) {
	std::string text = "";

	if(uAddVal > 0) {
		text += "+";
	}

	text += intToString(uAddVal);

	return text;
}

std::string EvolutionPanel::getDodgeCDText(unsigned int uDodgeVal) {
	std::string text = "";

	if(uDodgeVal > 0) {
		text += "-";

		text += intToString(uDodgeVal / 1000);

		if(uDodgeVal % 1000 > 0) {
			text += ".";

			unsigned int n = 0;

			n = uDodgeVal % 1000 / 100;
			if(n > 0) {
				text += intToString(n);
			}

			n = uDodgeVal % 100 / 10;
			if(n > 0) {
				text += intToString(n);
			}

			n = uDodgeVal % 10;
			if(n > 0) {
				text += intToString(n);
			}
		}

		text += "s";

	} else {
		text = "0s";
	}

	return text;
}
