#include "EvolutionPage.h"

#include "defines.h"

#include "EvolutionCell.h"
#include "EvolutionControl.h"

#define TOTAL_STAR_NUM 16

EvolutionPage::EvolutionPage()
{
}

EvolutionPage::~EvolutionPage()
{
}

//EvolutionPage* EvolutionPage::create(UILayout *pRefLayout) {
//	EvolutionPage *pRet = new EvolutionPage();
//
//	if(pRet != NULL && pRet->initWithLayout(pRefLayout)) {
//		pRet->autorelease();
//	} else {
//		CC_SAFE_DELETE(pRet);
//	}
//
//	return pRet;
//}
//
//bool EvolutionPage::initWithLayout(UILayout *pRefLayout) {
//	bool bRet = false;
//
//	do {
//		CC_BREAK_IF(!BaseLayout::init());
//
//		initLayoutWithUICopy(pRefLayout);
//		addChild(m_pRootLayout);
//
//		for(unsigned int i = 0; i < m_pRootLayout->getChildrenCount(); i++) {
//			std::string name = std::string("star_cell_") + intToString(i);
//			EvolutionCell *pCell = NULL;
//
//			if((i + 1) % 6 == 0) {
//				pCell = EvolutionSkillCell::create(dynamic_cast<UILayout*>(m_pRootLayout->getChildByName(name.c_str())));
//			} else {
//				pCell = EvolutionStarCell::create(dynamic_cast<UILayout*>(m_pRootLayout->getChildByName(name.c_str())));
//			}
//			
//			pCell->addTouchEventListener(this, toucheventselector(EvolutionPage::touchEventHandler));
//			pCell->setName(name.c_str());
//		}
//
//		bRet = true;
//	} while(0);
//
//	return bRet;
//}

EvolutionPage* EvolutionPage::create() {
	EvolutionPage *pRet = new EvolutionPage();

	if(pRet != NULL && pRet->init()) {
		pRet->autorelease();
	} else {
		CC_SAFE_DELETE(pRet);
	}

	return pRet;
}

bool EvolutionPage::init() {
	bool bRet = false;

	do {
		CC_BREAK_IF(!BaseLayout::init());

		initLayoutFromJsonFile("ui_v2/StarPageUI.ExportJson");
		addChild(m_pRootLayout);

		for(unsigned int i = 0; i < m_pRootLayout->getChildrenCount(); i++) {
			std::string name = std::string("star_cell_") + intToString(i);
			EvolutionCell *pCell = NULL;

			if((i + 1) % TOTAL_STAR_NUM == 0) {
				//pCell = EvolutionSkillCell::create(dynamic_cast<UILayout*>(m_pRootLayout->getChildByName(name.c_str())));
				UIImageView *pImg = dynamic_cast<UIImageView*>(m_pRootLayout->getChildByName(name.c_str()));
				pCell = EvolutionSkillCell::create();
				pCell->setPosition(pImg->getPosition());
				m_pRootLayout->addChild(pCell);
				pImg->removeFromParent();
			} else {
				//pCell = EvolutionStarCell::create(dynamic_cast<UILayout*>(m_pRootLayout->getChildByName(name.c_str())));
				UIImageView *pImg = dynamic_cast<UIImageView*>(m_pRootLayout->getChildByName(name.c_str()));
				pCell = EvolutionStarCell::create();
				pCell->setPosition(pImg->getPosition());
				m_pRootLayout->addChild(pCell);
				pImg->removeFromParent();
			}

			pCell->addTouchEventListener(this, toucheventselector(EvolutionPage::touchEventHandler));
			pCell->setName(name.c_str());
		}

		bRet = true;
	} while(0);

	return bRet;
}

void EvolutionPage::touchEventHandler(CCObject *pSender, TouchEventType type) {
	switch(type) {
		case TOUCH_EVENT_ENDED:
			P_EVOLUTION_CTRL->evolve();
			break;
		default:;
	}
}
