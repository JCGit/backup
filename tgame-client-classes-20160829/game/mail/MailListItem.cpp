#include "MailListItem.h"
#include "defines.h"
#include "NounHelper.h"
#include "MailControl.h"

MailListItem::MailListItem()
{
}

MailListItem::~MailListItem()
{
	CCLOG("destroy MailListItem");
}

MailListItem* MailListItem::create() {
	MailListItem *pMailListItem = new MailListItem();

	if(pMailListItem != NULL && pMailListItem->init()) {
		pMailListItem->autorelease();
	} else {
		CC_SAFE_DELETE(pMailListItem);
	}

	return pMailListItem;
}

bool MailListItem::init() {
	bool bRet = false;

	do {
		CC_BREAK_IF(!BaseLayout::init());

		initLayoutFromJsonFile("ui_v2/mail_item_ui.ExportJson");
        
		m_pTitleTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("title_txt"));
        m_pSenderTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("sender_txt"));
        m_pTimeTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("time_txt"));


        m_pMailOpenPic = dynamic_cast<UIImageView*>(m_pRootLayout->getChildByName("stat_icon"));
        m_pAttachIconPic = dynamic_cast<UIImageView*>(m_pRootLayout->getChildByName("attach_icon"));
        
        //m_pSelStatBg->setVisible(false);

		addChild(m_pRootLayout);

		bRet = true;
	} while(0);

	return bRet;
}

std::string MailListItem::getDescription() const {
	return "MailListItem";
}

#include "TimeEx.h"
void MailListItem::updateMailInfo(const vmsg::CSMail& stMail){
    m_stMail = stMail;

    m_pTitleTxt->setText(stMail.mailhead());
    m_pSenderTxt->setText(NounStr(SYSTEM));


    int iLeftTime = stMail.remaintime();
    string strDesc = TimeEx::getTimeStr(iLeftTime, "dhms");
    m_pTimeTxt->setText(strDesc + NounStr(NOUN_OUTTIME));

    //neil,mod,已经领取附件的也标记为已读吧
    if(stMail.hadread() || (stMail.hasattach() && stMail.hadgetattach()) ) {
        m_pMailOpenPic->loadTexture("other/icon_read.png", UI_TEX_TYPE_PLIST);
    } else {
        m_pMailOpenPic->loadTexture("other/icon_unread.png", UI_TEX_TYPE_PLIST);
    }
    m_pAttachIconPic->setVisible( (stMail.hasattach() && !stMail.hadgetattach()) );
    
}

void MailListItem::setSelect(bool isSel)
{
    //m_pSelStatBg->setVisible(isSel);
    //m_pDescTxt->setText("aa：aaa");
}



void MailListItem::onTouched() {
    P_MAIL_CTRL->getMailInfoPanel()->showMail(&m_stMail);
}

