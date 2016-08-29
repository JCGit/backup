#include "SignInListItem.h"
#include "defines.h"
#include "login/LoginControl.h"
#include "DailyRegAwardCfgMaster.h"
#include "IconCell.h"
#include "ToolUtils.h"
#include "TextHelper.h"
#include "weaponlib/WeaponLibControl.h"
#include "tip/TipControl.h"
#include "game/guide/GuideControl.h"

SignInListItem::SignInListItem()
{
}

SignInListItem::~SignInListItem()
{
	CCLOG("destroy SignInListItem");
}

SignInListItem* SignInListItem::create() {
	SignInListItem *pSignInListItem = new SignInListItem();

	if(pSignInListItem != NULL && pSignInListItem->init()) {
		pSignInListItem->autorelease();
	} else {
		CC_SAFE_DELETE(pSignInListItem);
	}

	return pSignInListItem;
}

SignInListItem* SignInListItem::create(UILayout *pRefLayout) {
	SignInListItem *pRet = new SignInListItem();

	if(pRet != NULL && pRet->initWithLayout(pRefLayout)) {
		pRet->autorelease();
	} else {
		CC_SAFE_DELETE(pRet);
	}

	return pRet;
}

bool SignInListItem::initWithLayout(UILayout *pRefLayout) {
	bool bRet = false;

	do {
		CC_BREAK_IF(!BaseLayout::init());

		initLayoutWithUICopy(pRefLayout);
		addChild(m_pRootLayout);

		initUI();

		bRet = true;
	} while (0);

	return bRet;
}

bool SignInListItem::init() {
	bool bRet = false;

	do {
		CC_BREAK_IF(!BaseLayout::init());

		initLayoutFromJsonFile("ui_v2/sign_in_list_item_ui.ExportJson");
		addChild(m_pRootLayout);

		initUI();

		bRet = true;
	} while(0);

	return bRet;
}

void SignInListItem::initUI(){


    m_pDayTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("day_txt"));
    m_pCell0Bg = dynamic_cast<UIImageView*>(m_pRootLayout->getChildByName("cell_0_bg"));
	m_pCell1Bg = dynamic_cast<UIImageView*>(m_pRootLayout->getChildByName("cell_1_bg"));
    m_pSignBtn = dynamic_cast<UIButton*>(m_pRootLayout->getChildByName("sign_btn"));

    m_pIconCell = IconCell::create();
    m_pIconCell->showNumTxt();
    m_pCell0Bg->addChild(m_pIconCell);

	m_pSecondIconCell = IconCell::create();
	m_pSecondIconCell->showNumTxt();
	m_pCell1Bg->addChild(m_pSecondIconCell);
}

void SignInListItem::onTouch(const CCPoint& stGlobalPt) {
    if(m_pSignBtn->hitTest(stGlobalPt)) {
        
        if(m_pSignBtn->isVisible() && m_pSignBtn->isTouchEnabled() ) {
			GUIDE_TOUCH(m_pSignBtn);
            P_LOGIN_CTRL->getProxy()->sendDailySignInRqst();
        }
    } else if( m_pCell0Bg->hitTest(stGlobalPt) ) {
        //
		P_TIP_CTRL->showItemTipPanel(m_pIconCell->getItemType(), m_pIconCell->getItemCfgid(), m_pIconCell->getItemNum());
    } else if(m_pCell1Bg->hitTest(stGlobalPt)) {
		P_TIP_CTRL->showItemTipPanel(m_pSecondIconCell->getItemType(), m_pSecondIconCell->getItemCfgid(), m_pSecondIconCell->getItemNum());
	}
}


void SignInListItem::setInfo(const int iIdx, const int iStat) {
    char str[100] = {0};
    snprintf(str, 100, TextStr(TEXT_SIGN_IN_DAY), iIdx+1);

    m_pDayTxt->setText( str );
    switch(iStat) {
    case SIGN_ITEM_STAT_DONE:
        m_pSignBtn->setVisible(true);
        m_pSignBtn->setBright(false);
        m_pSignBtn->setTouchEnabled(false);
        break;
    case SIGN_ITEM_STAT_IDLE:
        m_pSignBtn->setVisible(false);
        break;
    }

    const int iDay = iIdx + 1;
    const DailyRegAwardCfg* pCfg = DAILYREGAWARD_CFG_MASTER->GetCfg(iDay);
    if(NULL == pCfg) {
        return;
    }

    for(int i = 0; i < pCfg->awardlst_size(); ++i) {
        const DailyRegAwardElement& stElmt = pCfg->awardlst(i);

        if(i >= MAX_AWARD_NUM) {
            break;
        }

		if(i == 0) {
			m_pIconCell->setInfo(stElmt.elementtype(), stElmt.elementcnt(), stElmt.elementid() );
			CToolUtils::adjustIconBgSize(stElmt.elementtype(), stElmt.elementid(), m_pCell0Bg);
			CToolUtils::adjustFragIcon(stElmt.elementtype(),  m_pCell0Bg);
		} else if(i == 1) {
			m_pSecondIconCell->setInfo(stElmt.elementtype(), stElmt.elementcnt(), stElmt.elementid() );
			CToolUtils::adjustIconBgSize(stElmt.elementtype(), stElmt.elementid(), m_pCell1Bg);
			CToolUtils::adjustFragIcon(stElmt.elementtype(),  m_pCell1Bg);
		}
    }
}

void SignInListItem::setTodayCanSign(bool bCanSign) {
    //m_pSignBtn->setEnabled(bCanSign);
    m_pSignBtn->setVisible(true);
    m_pSignBtn->setBright(bCanSign);
    m_pSignBtn->setTouchEnabled(bCanSign);
}

UIButton* SignInListItem::getSignBtn()
{
	return m_pSignBtn;
}