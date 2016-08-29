#include "AutoPopControl.h"

#include "game/signin/SignInControl.h"
#include "game/shop/ShopControl.h"
#include "game/warmap/WarMapControl.h"

#include "CityControl.h"

AutoPopControl::AutoPopControl()
:m_pCurPopPanel(NULL)
{
	// 第一次进入
	//addPopPanel(P_SIGN_IN_CTRL->getSignInPanel());
	addPopPanel(P_SHOP_CTRL->getBuyMonthCardPanel());
	addPopPanel(P_SHOP_CTRL->getFirstChargePanel());
	addPopPanel(P_CITY_CTRL->getGetAwardPanel());
}

AutoPopControl::~AutoPopControl()
{
}

void AutoPopControl::addPopPanel(BasePanel *pPanel) {
	if(pPanel != NULL) {
		m_stPanels.push_back(pPanel);
	}
}

void AutoPopControl::popPanel() {
	while(m_stPanels.size() > 0) {
		BasePanel *pPanel = m_stPanels.front();
		m_stPanels.pop_front();

		if(pPanel->isMeetAutoOpen()) {
			pPanel->open();
			m_pCurPopPanel = pPanel;
			break;
		}
	}

	if(m_pCurPopPanel == NULL) { // 没有要自动打开的面板了
		if(P_GUIDE_CTRL->isNewcomer()) {
			P_GUIDE_CTRL->setNewcomer(false);
			P_GUIDE_CTRL->startGuide(GUIDE_ID_FIRST_TASK);
			return;
		}

		if(P_GUIDE_CTRL->isGuideDuring()
		&& P_GUIDE_CTRL->getGuideID() == GUIDE_ID_FIRST_TASK
		&& P_GUIDE_CTRL->getStepID() == 5) {
			P_GUIDE_CTRL->execStep();
			return;
		}

		vector<unsigned int>& stOpenModules = P_CITY_CTRL->getOpenModules();

		if(stOpenModules.size() > 0) {
			unsigned int uModuleID = stOpenModules[0];
			stOpenModules.clear();

			if(P_CITY_CTRL->getCityScreenView() != NULL) {
				P_CITY_CTRL->getCityScreenView()->moduleOpen(uModuleID);
				P_CITY_CTRL->getCityScreenView()->updateEntry();
			}
		}
	}
}

void AutoPopControl::onClosePanel(BasePanel* pPanel) {
	if(m_pCurPopPanel == pPanel) {
		m_pCurPopPanel = NULL;
		popPanel();
	}
}
