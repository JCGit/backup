#include "AttrStrengthenInfoPanel.h"

#include "defines.h"

#include "Macros.pb.h"

#include "CfgMaster/AttrStrengthenCfgMaster.h"

#include "basic/NounHelper.h"

#include "game/UserObj.h"

#include "AttrStrengthenControl.h"
#include "AttrCell.h"

#define MIN_SCROLL_DISTANCE 16.0f

AttrStrengthenInfoPanel::AttrStrengthenInfoPanel()
:m_pBgImg(NULL)
,m_pCurAttrNameTxt(NULL)
,m_pCurAttrLevelImg(NULL)
,m_pCurAttrLevelTxt(NULL)
,m_pCurAttrLimitLevelTxt(NULL)
,m_pCurAttrLimitLevelValTxt(NULL)
,m_pStrengthenBtn(NULL)
,m_pLeftFlipBtn(NULL)
,m_pRightFlipBtn(NULL)
,m_pAttrScrollView(NULL)
,m_pAttrPage(NULL)
,m_pAttrLayout(NULL)
,m_pAttrListView(NULL)
,m_pFireAtkAddValTxt(NULL)
,m_pIceAtkAddValTxt(NULL)
,m_pPoisonAtkAddValTxt(NULL)
,m_pThunderAtkAddValTxt(NULL)
,m_pBiochemistryAtkAddValTxt(NULL)
,m_pFireDefAddValTxt(NULL)
,m_pIceDefAddValTxt(NULL)
,m_pPoisonDefAddValTxt(NULL)
,m_pThunderDefAddValTxt(NULL)
,m_pBiochemistryDefAddValTxt(NULL)
,m_nCellSelectedIdx(-1)
{
}

AttrStrengthenInfoPanel::~AttrStrengthenInfoPanel()
{
}

AttrStrengthenInfoPanel* AttrStrengthenInfoPanel::create() {
	AttrStrengthenInfoPanel *pRet = new AttrStrengthenInfoPanel();

	if(pRet != NULL && pRet->init()) {
		pRet->autorelease();
	} else {
		CC_SAFE_DELETE(pRet);
	}

	return pRet;
}

bool AttrStrengthenInfoPanel::init() {
	bool bRet = false;

	do {
		CC_BREAK_IF(!BasePanel::init());

		setOpenWithBgImg(true);
		setUseBackForClose(true);

		initLayoutFromJsonFile("ui_v2/AttrStrengthenInfoPanelUI.ExportJson");
		addChild(m_pRootLayout);

		m_pBgImg = dynamic_cast<UIImageView*>(m_pRootLayout->getChildByName("bg_img"));

		m_pCurAttrNameTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("attr_name_txt"));

		m_pCurAttrLevelImg = dynamic_cast<UIImageView*>(m_pRootLayout->getChildByName("attr_level_img"));

		m_pCurAttrLevelTxt = dynamic_cast<UILabelAtlas*>(m_pRootLayout->getChildByName("attr_level_txt"));

		m_pCurAttrLimitLevelTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("limit_level_txt"));

		m_pCurAttrLimitLevelValTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("limit_level_val_txt"));

		m_pStrengthenBtn = dynamic_cast<UIButton*>(m_pRootLayout->getChildByName("strengthen_btn"));
		m_pStrengthenBtn->addTouchEventListener(this, toucheventselector(AttrStrengthenInfoPanel::touchEventHandler));

		m_pLeftFlipBtn = dynamic_cast<UIButton*>(m_pRootLayout->getChildByName("left_flip_btn"));
		m_pLeftFlipBtn->addTouchEventListener(this, toucheventselector(AttrStrengthenInfoPanel::touchEventHandler));

		m_pRightFlipBtn = dynamic_cast<UIButton*>(m_pRootLayout->getChildByName("right_flip_btn"));
		m_pRightFlipBtn->addTouchEventListener(this, toucheventselector(AttrStrengthenInfoPanel::touchEventHandler));

		m_pAttrScrollView = dynamic_cast<UIScrollView*>(m_pRootLayout->getChildByName("attr_scroll"));
		m_pAttrScrollView->setBounceEnabled(false);
		m_pAttrScrollView->setEnabled(false);
		m_pAttrScrollView->addEventListenerScrollView(this, scrollvieweventselector(AttrStrengthenInfoPanel::scrollViewEventHandler));

		m_pAttrPage = NPageView::create();
		m_pAttrPage->setDirection(SCROLLVIEW_DIR_VERTICAL);
		m_pAttrPage->setSize(CCSizeMake(1000, 240));
		m_pAttrPage->setInnerContainerSize(CCSizeMake(2764, 240));
		m_pAttrPage->setPosition(ccp(140, 320));
		m_pAttrPage->setTouchEnabled(true);
		m_pRootLayout->addChild(m_pAttrPage);
		m_pAttrPage->addTouchEventListener(this, toucheventselector(AttrStrengthenInfoPanel::touchEventHandler));
		m_pAttrPage->addEventListenerScrollView(this, scrollvieweventselector(AttrStrengthenInfoPanel::scrollViewEventHandler));

		/*m_pAttrScrollView = ScrollView::create();
		m_pAttrScrollView->setDirection(SCROLLVIEW_DIR_HORIZONTAL);
		m_pAttrScrollView->setSize(CCSizeMake(1000, 200));
		m_pAttrScrollView->setInnerContainerSize(CCSizeMake(2764, 200));
		m_pAttrScrollView->setPosition(ccp(140, 360));
		m_pAttrScrollView->setTouchEnabled(true);
		m_pRootLayout->addChild(m_pAttrScrollView);*/

		m_pAttrLayout = UILayout::create();
		m_pAttrLayout->setLayoutType(LAYOUT_LINEAR_HORIZONTAL);
		m_pAttrLayout->setSize(CCSizeMake(1980, 240));
		m_pAttrLayout->setPosition(ccp(392, 0));
		m_pAttrLayout->setTouchEnabled(false);
		m_pAttrPage->addChild(m_pAttrLayout);

		/*m_pAttrLayout = dynamic_cast<UILayout*>(m_pAttrScrollView->getChildByName("attr_list"));
		m_pAttrLayout->setLayoutType(LAYOUT_LINEAR_HORIZONTAL);*/

		m_pAttrListView = dynamic_cast<UIListView*>(m_pRootLayout->getChildByName("attr_list"));
		//m_pAttrListView->addEventListenerListView(this, listvieweventselector(AttrStrengthenInfoPanel::listViewEventHandler));

		m_pFireAtkAddValTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("fire_atk_add_val_txt"));

		m_pIceAtkAddValTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("ice_atk_add_val_txt"));

		m_pPoisonAtkAddValTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("poison_atk_add_val_txt"));

		m_pThunderAtkAddValTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("thunder_atk_add_val_txt"));

		m_pBiochemistryAtkAddValTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("boichemistry_atk_add_val_txt"));

		m_pFireDefAddValTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("fire_def_add_val_txt"));

		m_pIceDefAddValTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("ice_def_add_val_txt"));

		m_pPoisonDefAddValTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("poison_def_add_val_txt"));

		m_pThunderDefAddValTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("thunder_def_add_val_txt"));

		m_pBiochemistryDefAddValTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("biochemistry_def_add_val_txt"));

		bRet = true;
	} while(0);

	return bRet;
}

void AttrStrengthenInfoPanel::updateUI() {
    //const vmsg::CSPlayerInfo &stPlayerInfo = P_USER_OBJ->getPlayerInfo();
    const UserInfo *pUserInfo = P_USER_OBJ->getUserInfo();

	const vmsg::CSAttrStrengthenInfo &stAttrStrengthenInfo = P_ATTR_STRENGTHEN_CTRL->getAttrStrengthenInfo();

	LinearLayoutParameter *pLayoutParameter = LinearLayoutParameter::create();
	pLayoutParameter->setGravity(LINEAR_GRAVITY_CENTER_VERTICAL);
	pLayoutParameter->setMargin(Margin(0, 0, 16, 0));

	for(unsigned int i = 0; i < 10; i++) {
		unsigned int uAttrType = i < 5 ? vmsg::MAGIC_ATK : vmsg::MAGIC_DEF;
		unsigned int uMagicType = i % 5 + 1;

		string cellName = string("cell_") + intToString(i);

		if(m_pAttrLayout->getChildrenCount() < i + 1) {
			AttrCell *pCell = AttrCell::create(dynamic_cast<UILayout*>(m_pRootLayout->getChildByName("attr_cell")));
			pCell->setType(uAttrType, uMagicType);
			m_pAttrLayout->addChild(pCell);
			pCell->setLayoutParameter(pLayoutParameter);
			pCell->setName(cellName.c_str());
			pCell->addTouchEventListener(this, toucheventselector(AttrStrengthenInfoPanel::touchEventHandler));
		}

		AttrCell *pCell = dynamic_cast<AttrCell*>(m_pAttrLayout->getChildByName(cellName.c_str()));
		unsigned int uLevel = uAttrType == vmsg::MAGIC_ATK ? stAttrStrengthenInfo.atkmagics(uMagicType) : stAttrStrengthenInfo.defmagics(uMagicType);
		pCell->updateUI(uLevel);

		if(uLevel < 1) {
			continue;
		}

		AttrStrengthenCfg *pstAttrStrengthenCfg = ATTR_STRENGTHEN_CFG_MASTER->getAttrStrengthenCfgByLevel(uAttrType, uLevel);

		if(pstAttrStrengthenCfg != NULL) {
			switch(uMagicType) {
				case ICE_MAGIC:
					if(uAttrType == vmsg::MAGIC_ATK) {
						m_pIceAtkAddValTxt->setText(intToString(pstAttrStrengthenCfg->attrval()));
					} else {
						m_pIceDefAddValTxt->setText(intToString(pstAttrStrengthenCfg->attrval()));
					}
					break;
				case FLAME_MAGIC:
					if(uAttrType == vmsg::MAGIC_ATK) {
						m_pFireAtkAddValTxt->setText(intToString(pstAttrStrengthenCfg->attrval()));
					} else {
						m_pFireDefAddValTxt->setText(intToString(pstAttrStrengthenCfg->attrval()));
					}
					break;
				case POISON_MAGIC:
					if(uAttrType == vmsg::MAGIC_ATK) {
						m_pPoisonAtkAddValTxt->setText(intToString(pstAttrStrengthenCfg->attrval()));
					} else {
						m_pPoisonDefAddValTxt->setText(intToString(pstAttrStrengthenCfg->attrval()));
					}
					break;
				case BIOTECH_MAGIC:
					if(uAttrType == vmsg::MAGIC_ATK) {
						m_pBiochemistryAtkAddValTxt->setText(intToString(pstAttrStrengthenCfg->attrval()));
					} else {
						m_pBiochemistryDefAddValTxt->setText(intToString(pstAttrStrengthenCfg->attrval()));
					}
					break;
				case LIGHTNING_MAGIC:
					if(uAttrType == vmsg::MAGIC_ATK) {
						m_pThunderAtkAddValTxt->setText(intToString(pstAttrStrengthenCfg->attrval()));
					} else {
						m_pThunderDefAddValTxt->setText(intToString(pstAttrStrengthenCfg->attrval()));
					}
					break;
				default:;
			}
			
		}
	}

	m_nCellSelectedIdx = m_nCellSelectedIdx == -1 ? 0 : m_nCellSelectedIdx;

	string cellName = string("cell_") + intToString(m_nCellSelectedIdx);
	AttrCell *pCell = dynamic_cast<AttrCell*>(m_pAttrLayout->getChildByName(cellName.c_str()));
	pCell->selected(true);
	updateAttrInfo();
	m_pAttrPage->getInnerContainer()->setPosition(calcPoint(m_nCellSelectedIdx));
	m_pAttrLayout->requestDoLayout();
	checkFlipBtnStatus();
	
}

void AttrStrengthenInfoPanel::prevAttr() {
	if(m_nCellSelectedIdx > 0) {
		AttrCell *pCell = NULL;

		if(m_nCellSelectedIdx >= 0) {
			string cellName = string("cell_") + intToString(m_nCellSelectedIdx);
			pCell = dynamic_cast<AttrCell*>(m_pAttrLayout->getChildByName(cellName.c_str()));
			pCell->selected(false);
		}

		m_nCellSelectedIdx--;
		string cellName = string("cell_") + intToString(m_nCellSelectedIdx);
		pCell = dynamic_cast<AttrCell*>(m_pAttrLayout->getChildByName(cellName.c_str()));
		pCell->selected(true);
		updateAttrInfo();

		m_pAttrPage->getInnerContainer()->setPosition(calcPoint(m_nCellSelectedIdx));
		m_pAttrLayout->requestDoLayout();

		checkFlipBtnStatus();
	}
}

void AttrStrengthenInfoPanel::nextAttr() {
	if(m_nCellSelectedIdx < 9) {
		AttrCell *pCell = NULL;

		if(m_nCellSelectedIdx >= 0) {
			string cellName = string("cell_") + intToString(m_nCellSelectedIdx);
			pCell = dynamic_cast<AttrCell*>(m_pAttrLayout->getChildByName(cellName.c_str()));
			pCell->selected(false);
		}

		m_nCellSelectedIdx++;
		string cellName = string("cell_") + intToString(m_nCellSelectedIdx);
		pCell = dynamic_cast<AttrCell*>(m_pAttrLayout->getChildByName(cellName.c_str()));
		pCell->selected(true);
		updateAttrInfo();

		m_pAttrPage->getInnerContainer()->setPosition(calcPoint(m_nCellSelectedIdx));
		m_pAttrLayout->requestDoLayout();

		checkFlipBtnStatus();
	}
}

CCPoint AttrStrengthenInfoPanel::calcPoint(int nSelectedIdx) {
	UILayout *pInnerContainer = m_pAttrScrollView->getInnerContainer();

	float startX = 0.0f;
	float endX = m_pAttrScrollView->getSize().width - pInnerContainer->getSize().width;
	float dx = startX - endX;
	float pw = dx / 9;

	return ccp(-nSelectedIdx * pw, pInnerContainer->getPositionY());
}

void AttrStrengthenInfoPanel::touchEventHandler(CCObject *pSender, TouchEventType type) {
	UIWidget *pWidget = dynamic_cast<UIWidget*>(pSender);
	const char *pszWidgetName = pWidget->getName();
	string widgetName = pWidget->getName();

	switch(type) {
		case TOUCH_EVENT_ENDED:
			if(strcmp(pszWidgetName, m_pStrengthenBtn->getName()) == 0) {
				strengthen();
			} else if(strcmp(pszWidgetName, m_pLeftFlipBtn->getName()) == 0) {
				prevAttr();
			} else if(strcmp(pszWidgetName, m_pRightFlipBtn->getName()) == 0) {
				nextAttr();
			} else if(strcmp(pszWidgetName, m_pAttrPage->getName()) == 0) {
				const CCPoint &startPos = m_pAttrPage->getTouchStartPos();
				const CCPoint &endPos = m_pAttrPage->getTouchEndPos();

				if(startPos.getDistance(endPos) > MIN_SCROLL_DISTANCE) {
					if(startPos.x > endPos.x) {
						CCLOG("scroll left");
					} else {
						CCLOG("scroll right");
					}

					/*if(m_nScrollDirection == -1) {
						//prevRole();
						CCLOG("scroll left");
						m_nScrollDirection = 0;
					} else if(m_nScrollDirection == 1) {
						//nextRole();
						CCLOG("scroll right");
						m_nScrollDirection = 0;
					} else {
						CCLOG("not scroll");
					}*/
				}


			} else if(widgetName.substr(0, 4) == "cell") {
				int nSelectedIdx = atoi(widgetName.substr(5, widgetName.size() - 5).c_str());
				
				if(m_nCellSelectedIdx == nSelectedIdx) {
					strengthen();
				}

			}
			break;
		default:;
	}
}

void AttrStrengthenInfoPanel::listViewEventHandler(CCObject *pSender, ListViewEventType type) {
	UIWidget *pWidget = dynamic_cast<UIWidget*>(pSender);
	const char *pszWidgetName = pWidget->getName();

	switch(type) {
		case LISTVIEW_ONSELECTEDITEM_END:
			if(std::strcmp(pszWidgetName, m_pAttrListView->getName()) == 0) {
				AttrCell *pCell = dynamic_cast<AttrCell*>(m_pAttrListView->getItem(m_pAttrListView->getCurSelectedIndex()));
				
				if(pCell != NULL) {
					const vmsg::CSAttrStrengthenInfo &stAttrStrengthenInfo = P_ATTR_STRENGTHEN_CTRL->getAttrStrengthenInfo();
					unsigned int uAttrType = pCell->getAttrType();
					unsigned int uMagicType = pCell->getMagicType();
					unsigned int uLevel = uAttrType == vmsg::MAGIC_ATK ? stAttrStrengthenInfo.atkmagics(uMagicType) : stAttrStrengthenInfo.defmagics(uMagicType);
					AttrStrengthenCfg *pNextAttrStrengthenCfg = ATTR_STRENGTHEN_CFG_MASTER->getAttrStrengthenCfgByLevel(uAttrType, uLevel + 1);

					if(pNextAttrStrengthenCfg != NULL) {
						unsigned int uNeedLevel = pNextAttrStrengthenCfg->needlv();
						unsigned int uLevel = P_USER_OBJ->getUserInfo()->level;

						if(uLevel >= uNeedLevel) {
							P_ATTR_STRENGTHEN_CTRL->getAttrStrengthenPanel()->open();
							P_ATTR_STRENGTHEN_CTRL->getAttrStrengthenPanel()->setType(pCell->getAttrType(), pCell->getMagicType());
							P_ATTR_STRENGTHEN_CTRL->getAttrStrengthenPanel()->updateUI();
						}
					}

				}
			}
			break;
		default:;
	}
}

void AttrStrengthenInfoPanel::scrollViewEventHandler(CCObject *pSender, ScrollviewEventType type) {
	UIScrollView *pScrollView = dynamic_cast<UIScrollView*>(pSender);
	string senderName = pScrollView->getName();

	switch(type) {
		case SCROLLVIEW_EVENT_SCROLL_TO_LEFT:
			m_nScrollDirection = -1;
			break;
		case SCROLLVIEW_EVENT_SCROLL_TO_RIGHT:
			m_nScrollDirection = 1;
			break;
		default:
			m_nScrollDirection = 0;
	}
}

void AttrStrengthenInfoPanel::checkFlipBtnStatus() {
	m_pLeftFlipBtn->setBright(m_nCellSelectedIdx > 0);
	m_pLeftFlipBtn->setTouchEnabled(m_nCellSelectedIdx > 0);
	m_pRightFlipBtn->setBright(m_nCellSelectedIdx < 9);
	m_pRightFlipBtn->setTouchEnabled(m_nCellSelectedIdx < 9);
}

void AttrStrengthenInfoPanel::updateAttrInfo() {
	AttrCell *pCell = NULL;
	string cellName = string("cell_") + intToString(m_nCellSelectedIdx);
	pCell = dynamic_cast<AttrCell*>(m_pAttrLayout->getChildByName(cellName.c_str()));

	unsigned int uAttrType = pCell->getAttrType();
	unsigned int uMagicType = pCell->getMagicType();
	unsigned int uLevel = pCell->getLevel();
	std::string name = "";

	switch(uMagicType) {
		case ICE_MAGIC:
			name = uAttrType == vmsg::MAGIC_ATK ? NounStr(NOUN_ATK_ICE) : NounStr(NOUN_DEF_ICE);
			break;
		case FLAME_MAGIC:
			name = uAttrType == vmsg::MAGIC_ATK ? NounStr(NOUN_ATK_FIRE) : NounStr(NOUN_DEF_FIRE);
			break;
		case POISON_MAGIC:
			name = uAttrType == vmsg::MAGIC_ATK ? NounStr(NOUN_ATK_POISON) : NounStr(NOUN_DEF_POISON);
			break;
		case BIOTECH_MAGIC:
			name = uAttrType == vmsg::MAGIC_ATK ? NounStr(NOUN_ATK_BIOCHEMISTRY) : NounStr(NOUN_DEF_BIOCHEMISTRY);
			break;
		case LIGHTNING_MAGIC:
			name = uAttrType == vmsg::MAGIC_ATK ? NounStr(NOUN_ATK_THUNDER) : NounStr(NOUN_DEF_THUNDER);
			break;
		default:;
	}

	m_pCurAttrNameTxt->setText(name);
	m_pCurAttrLevelTxt->setStringValue(intToString(uLevel));
	m_pCurAttrLevelImg->setPositionX(m_pCurAttrNameTxt->getPositionX() + m_pCurAttrNameTxt->getSize().width / 2 + 16);
	m_pCurAttrLevelTxt->setPositionX(m_pCurAttrLevelImg->getPositionX() + m_pCurAttrLevelImg->getSize().width + 8);

	AttrStrengthenCfg *pNextAttrStrengthenCfg = ATTR_STRENGTHEN_CFG_MASTER->getAttrStrengthenCfgByLevel(uAttrType, uLevel + 1);

	if(pNextAttrStrengthenCfg != NULL) {
		m_pCurAttrLimitLevelValTxt->setText(intToString(pNextAttrStrengthenCfg->needlv()));

		if(P_USER_OBJ->getUserInfo()->level >= pNextAttrStrengthenCfg->needlv()) {
			m_pCurAttrLimitLevelTxt->setVisible(false);
			m_pCurAttrLimitLevelValTxt->setVisible(false);
			m_pStrengthenBtn->setVisible(true);
			m_pStrengthenBtn->setTouchEnabled(true);
		} else {
			m_pCurAttrLimitLevelTxt->setVisible(true);
			m_pCurAttrLimitLevelValTxt->setVisible(true);
			m_pStrengthenBtn->setVisible(false);
			m_pStrengthenBtn->setTouchEnabled(false);
		}
	} else {
		m_pCurAttrLimitLevelTxt->setVisible(false);
		m_pCurAttrLimitLevelValTxt->setVisible(false);
		m_pStrengthenBtn->setBright(false);
		m_pStrengthenBtn->setTouchEnabled(false);
	}
}

void AttrStrengthenInfoPanel::strengthen() {
	string cellName = string("cell_") + intToString(m_nCellSelectedIdx);
	AttrCell *pCell = dynamic_cast<AttrCell*>(m_pAttrLayout->getChildByName(cellName.c_str()));

	if(pCell != NULL) {
		const vmsg::CSAttrStrengthenInfo &stAttrStrengthenInfo = P_ATTR_STRENGTHEN_CTRL->getAttrStrengthenInfo();
		unsigned int uAttrType = pCell->getAttrType();
		unsigned int uMagicType = pCell->getMagicType();
		unsigned int uLevel = uAttrType == vmsg::MAGIC_ATK ? stAttrStrengthenInfo.atkmagics(uMagicType) : stAttrStrengthenInfo.defmagics(uMagicType);
		AttrStrengthenCfg *pNextAttrStrengthenCfg = ATTR_STRENGTHEN_CFG_MASTER->getAttrStrengthenCfgByLevel(uAttrType, uLevel + 1);

		if(pNextAttrStrengthenCfg != NULL) {
			unsigned int uNeedLevel = pNextAttrStrengthenCfg->needlv();
			unsigned int uLevel = P_USER_OBJ->getUserInfo()->level;

			if(uLevel >= uNeedLevel) {
				P_ATTR_STRENGTHEN_CTRL->getAttrStrengthenPanel()->open();
				P_ATTR_STRENGTHEN_CTRL->getAttrStrengthenPanel()->setType(pCell->getAttrType(), pCell->getMagicType());
				P_ATTR_STRENGTHEN_CTRL->getAttrStrengthenPanel()->updateUI();
			}
		}

	}
}
