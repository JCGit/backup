#include "MailInfoPanel.h"
#include "IconCell.h"
#include "MailControl.h"
#include "ToolUtils.h"
#include "bag/BagControl.h"
#include "bag/ItemInfoPanel.h"
#include "NounHelper.h"

MailInfoPanel::MailInfoPanel()
{
    m_iMailID = 0;
}

MailInfoPanel::~MailInfoPanel(){

}

bool MailInfoPanel::init(){
	bool bRet = false;

	do 
	{
		CC_BREAK_IF(!BasePanel::init());
        
        setClickOutToClose(true);

        initLayoutFromJsonFile("ui_v2/mail_info_panel.ExportJson");
        addChild(m_pRootLayout);

        m_pCloseBtn = dynamic_cast<UIButton*>(m_pRootLayout->getChildByName("close_btn"));
        m_pCloseBtn->addTouchEventListener(this, toucheventselector(MailInfoPanel::touchEventHandler));

        m_pGetBtn = dynamic_cast<UIButton*>(m_pRootLayout->getChildByName("get_btn"));
        m_pGetBtn->addTouchEventListener(this, toucheventselector(MailInfoPanel::touchEventHandler));

        m_pTitleTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("mail_title_txt"));
        m_pContentTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("mail_content_txt"));

        m_pAttachTipTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("attach_tip_txt"));

        m_pCellOne = dynamic_cast<UIImageView*>(m_pRootLayout->getChildByName("cell_0"));
        m_pCellTwo = dynamic_cast<UIImageView*>(m_pRootLayout->getChildByName("cell_1"));

        m_pItemOne = IconCell::create();
        m_pItemOne->setSize(CCSizeMake(60, 60));
        m_pItemOne->setZOrder(5);
        m_pItemOne->showNumTxt(true);
        m_pCellOne->addChild(m_pItemOne);

        m_pItemTwo = IconCell::create();
        m_pItemTwo->setSize(CCSizeMake(60, 60));
        m_pItemTwo->setZOrder(5);
        m_pItemOne->showNumTxt(true);
        m_pCellTwo->addChild(m_pItemTwo);

        m_pCellOne->setTouchEnabled(true);
        m_pCellTwo->setTouchEnabled(true);
        m_pCellOne->addTouchEventListener(this, toucheventselector(MailInfoPanel::touchEventHandler));
        m_pCellTwo->addTouchEventListener(this, toucheventselector(MailInfoPanel::touchEventHandler));

		bRet = true;
	} while (0);

	return bRet;
}


void MailInfoPanel::touchEventHandler(CCObject* pSender, TouchEventType type){
	UIWidget *pWidget = dynamic_cast<UIWidget*>(pSender);
	const char *pszWidgetName = pWidget->getName();

	switch (type) {
	case TOUCH_EVENT_ENDED:
		if(strcmp(pszWidgetName, "close_btn") == 0) {
			close();			
        } else if(strcmp(pszWidgetName, "get_btn") == 0) {
            
            const CSMail* pstMail = P_MAIL_CTRL->getMailObj()->getMail(m_iMailID);
            if(NULL == pstMail) {
                break;
            }
            if(!(pstMail->hadgetattach()) && pstMail->hasattach() ) {
                P_MAIL_CTRL->getProxy()->sendGetAttachOne(m_iMailID);
            }

        } else if(std::strcmp(pszWidgetName, "cell_0") == 0) {
            if(m_pCellOne->isVisible() && m_pItemOne->getItemCfgid() != 0 && m_pItemOne->getItemType() != 0) {
                P_BAG_CTRL->getItemInfoPanel()->open();
                P_BAG_CTRL->getItemInfoPanel()->updateInfo(m_pItemOne->getItemCfgid(), m_pItemOne->getItemType(), false);
            }

        } else if(std::strcmp(pszWidgetName, "cell_1") == 0) {
            if(m_pCellTwo->isVisible() && m_pItemTwo->getItemCfgid() != 0 && m_pItemTwo->getItemType() != 0) {
                P_BAG_CTRL->getItemInfoPanel()->open();
                P_BAG_CTRL->getItemInfoPanel()->updateInfo(m_pItemTwo->getItemCfgid(), m_pItemTwo->getItemType(), false);
            }

        }
		break;
	default:

		break;
	}
}


void MailInfoPanel::open()
{
    BasePanel::open();
    
}


void MailInfoPanel::close()
{
    BasePanel::close();
    //reset
    m_iMailID = 0;
}


void MailInfoPanel::showMail(const vmsg::CSMail* pstMail) {
    if(NULL == pstMail) {
        return;
    }
    m_iMailID = pstMail->mailid();

    //如果本地有数据，直接显示
    const CSReadMailRsp* pMailContent = P_MAIL_CTRL->getMailObj()->getMailContent(m_iMailID);
    if(pMailContent) {
        updateMailContent();
    } else {
        showLoading();
        P_MAIL_CTRL->getProxy()->sendReadMail(m_iMailID);
    }

    this->open();
}


void MailInfoPanel::updateMailContent() {
    closeLoading();

    const CSMail* pstMail = P_MAIL_CTRL->getMailObj()->getMail(m_iMailID);
    if(NULL == pstMail) {
        return;
    }
    const CSReadMailRsp* pMailContent = P_MAIL_CTRL->getMailObj()->getMailContent(m_iMailID);
    if(NULL == pMailContent) {
        return;
    }

    //update ui,pMailContent
    m_pTitleTxt->setText(pstMail->mailhead());
    m_pContentTxt->setText(CToolUtils::splitUTFStr(pMailContent->mailbody(), 15));

    //update btn stat
    bool bCanGetAttach = (pMailContent->attach_size() > 0 && !pstMail->hadgetattach());

    m_pGetBtn->setBright(bCanGetAttach);
    m_pGetBtn->setTouchEnabled(bCanGetAttach);

    if(bCanGetAttach) {
        m_pGetBtn->setTitleText(NounStr(NOUN_GET));
    } else {
        m_pGetBtn->setTitleText(NounStr(NOUN_GOT));
        /*
        //已经领取过的不显示附件
        m_pCellOne->setVisible(false);
        m_pCellTwo->setVisible(false);
        return;
        */
    }
    //update cell info
    const int iAttachCnt = pMailContent->attach_size();
    if(0 == iAttachCnt) {
        m_pCellOne->setVisible(false);
        m_pCellTwo->setVisible(false);
    } else if(1 == iAttachCnt) {
        m_pCellOne->setVisible(true);
        m_pCellTwo->setVisible(false);
        const CSMailAttach& stAttach = pMailContent->attach(0);
        m_pItemOne->setInfo(stAttach.attachtype(), stAttach.attachcnt(), stAttach.attachcfgid());
        CToolUtils::adjustIconBgSize(stAttach.attachtype(), stAttach.attachcfgid(), m_pCellOne );
        CToolUtils::adjustFragIcon(stAttach.attachtype(), m_pCellOne);
    } else if(2 == iAttachCnt) {
        m_pCellOne->setVisible(true);
        m_pCellTwo->setVisible(true);
        const CSMailAttach& stAttach = pMailContent->attach(0);
        m_pItemOne->setInfo(stAttach.attachtype(), stAttach.attachcnt(), stAttach.attachcfgid());

        CToolUtils::adjustIconBgSize(stAttach.attachtype(), stAttach.attachcfgid(), m_pCellOne );
        CToolUtils::adjustFragIcon(stAttach.attachtype(), m_pCellOne);

        const CSMailAttach& stAttach2 = pMailContent->attach(1);
        m_pItemTwo->setInfo(stAttach2.attachtype(), stAttach2.attachcnt(), stAttach2.attachcfgid());

        CToolUtils::adjustIconBgSize(stAttach2.attachtype(), stAttach2.attachcfgid(), m_pCellTwo );
        CToolUtils::adjustFragIcon(stAttach2.attachtype(), m_pCellTwo);
    }
}


