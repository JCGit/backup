#include "CommonDialog.h"
#include "NounHelper.h"
#include "GameInf.h"

USING_NS_CC;
USING_NS_CC_EXT;

CommonDialog::CommonDialog()
:m_pYesBtn(NULL)
,m_pNoBtn(NULL)
,m_bIsInit(false)
,m_uBtnNum(0)
,m_fInitPosX(0)
, m_bRet2Login(false)
{
}

CommonDialog::~CommonDialog()
{
}

bool CommonDialog::init() {
	bool bRet = false;

	do {
		CC_BREAK_IF(!BasePanel::init());

		setClickOutToClose(true);
		initLayoutFromJsonFile("ui_v2/common_dialog_ui.ExportJson");
		addChild(m_pRootLayout);

		m_pYesBtn = dynamic_cast<UIButton*>(m_pRootLayout->getChildByName("yes_btn"));
		m_pYesBtn->addTouchEventListener(this, toucheventselector(CommonDialog::touchEventHandler));
		m_fInitPosX = m_pYesBtn->getPositionX();

		m_pNoBtn = dynamic_cast<UIButton*>(m_pRootLayout->getChildByName("no_btn"));
		m_pNoBtn->addTouchEventListener(this, toucheventselector(CommonDialog::touchEventHandler));

		m_pCloseBtn = dynamic_cast<UIButton*>(m_pRootLayout->getChildByName("close_btn"));
		m_pCloseBtn->addTouchEventListener(this, toucheventselector(CommonDialog::touchEventHandler));

        m_pDescTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("desc_txt"));
        m_pDescTxt->setText("");

		bRet = true;
	} while(0);

	return bRet;
}

void CommonDialog::close()
{
	BasePanel::close();

	if (m_bRet2Login)
		P_GAME_INF->doSDKLogout();
}

std::string CommonDialog::getDescription() const {
	return "CommonDialog";
}

CCNode* CommonDialog::getGuideNodeByName(const std::string &name) {
	CCNode *pRet = NULL;

	if(name == "ok_btn") {
		if(m_pYesBtn != NULL && m_pYesBtn->isVisible() && m_pYesBtn->isTouchEnabled()) {
			pRet = m_pYesBtn;
		}
	}

	return pRet;
}

void CommonDialog::touchEventHandler(CCObject *pSender, TouchEventType type) {

    if (TOUCH_EVENT_ENDED == type) {
        UIWidget *pWidget = dynamic_cast<UIWidget*>(pSender);
        const char *pszWidgetName = pWidget->getName();
        if(std::strcmp(pszWidgetName, "no_btn") == 0) {
            close();
        } else if(std::strcmp(pszWidgetName, "yes_btn") == 0) {
			GUIDE_TOUCH(m_pYesBtn);
            close(); //先关闭，然后回掉
            if (_btnEventListener && _btnYesEventSelector) {
                (_btnEventListener->*_btnYesEventSelector)();
            } 
		}else if(std::strcmp(pszWidgetName, "close_btn") == 0) {
			close();
		}
    }

}


void CommonDialog::show(const string& strDescTxt, CCObject *target, 
                        const string& strYesBtnTxt, const string& strNoBtnTxt,
						SEL_BtnEvent yesEvent, bool bRet2Login)
{
    if (!m_bIsInit)
    {
        init();
        m_bIsInit = true;
    }
    if (!isOpen())
    {
        open();
    }

	m_bRet2Login = bRet2Login;

    CCLOG("[trace]CommonDialog,text=%s", strDescTxt.c_str());
    m_pDescTxt->setText(strDescTxt);

	if(std::strcmp(strNoBtnTxt.c_str(), "") == 0){
		m_uBtnNum = 1;
		m_pNoBtn->setVisible(false);
		m_pNoBtn->setEnabled(false);
		m_pNoBtn->setTouchEnabled(false);
		m_pYesBtn->setPosition(ccp(m_pDescTxt->getPositionX(), m_pYesBtn->getPositionY()));
	}else{
		m_uBtnNum = 2;
		m_pNoBtn->setVisible(true);
		m_pNoBtn->setEnabled(true);
		m_pNoBtn->setTouchEnabled(true);
		m_pNoBtn->setTitleText(strNoBtnTxt);
		m_pYesBtn->setPosition(ccp(m_fInitPosX, m_pYesBtn->getPositionY()));
	}
    if(strYesBtnTxt.size() <= 0) {
        m_pYesBtn->setTitleText(NounStr(OK));
    } else {
        m_pYesBtn->setTitleText(strYesBtnTxt);
    }
    if(1 == m_uBtnNum && m_pYesBtn->getTitleText() == NounStr(OK)) {
        //setClickOutToClose(true); //仅有一个确定按钮时，设置点击任何位置可以关闭
    } else {
        //setClickOutToClose(false);
    }
    _btnEventListener = target;
    _btnYesEventSelector = yesEvent;

	GUIDE_SHOW(GUIDE_UI_COMMON_DIALOG);
}

void CommonDialog::show(const string& strDescTxt) {
    if (!m_bIsInit)
    {
        init();
        m_bIsInit = true;
    }
    if (!isOpen())
    {
        open();
    }
    setClickOutToClose(true); //仅有一个确定按钮时，设置点击任何位置可以关闭

    m_pDescTxt->setText(strDescTxt);

    m_uBtnNum = 1;
    m_pNoBtn->setVisible(false);
    m_pNoBtn->setEnabled(false);
    m_pNoBtn->setTouchEnabled(false);
    m_pYesBtn->setPosition(ccp(m_pDescTxt->getPositionX(), m_pYesBtn->getPositionY()));

    m_pYesBtn->setTitleText(NounStr(OK));
    _btnEventListener = NULL;
    _btnYesEventSelector = NULL;
}

void CommonDialog::reset()
{
	m_bRet2Login = false;
}

