#include "MailControl.h"
#include "city/CityControl.h"
#include "PopTipLayer.h"

#include "game/notification/NotificationControl.h"

MailControl::MailControl()
{
    m_pMailUIPanel = NULL;
    m_pMailInfoPanel = NULL;
}

MailControl::~MailControl()
{
    finalizePanel();
}

void MailControl::finalizePanel() {
    CC_SAFE_RELEASE_NULL(m_pMailUIPanel);
    CC_SAFE_RELEASE_NULL(m_pMailInfoPanel);
}

void MailControl::notifyMailUnread() {
	if(m_stMailObj.getUnReadMailNum() > 0) {
		PUSH_NOTIFICATION(ENTRY_MAIL, NOTIFY_TAG_MAIL_UNREAD, 1);
	} else {
		PUSH_NOTIFICATION(ENTRY_MAIL, NOTIFY_TAG_MAIL_UNREAD, 0);
	}

}

MailProxy *MailControl::getProxy() {
    return &m_stMailProxy;
}

MailUIPanel* MailControl::getMailUIPanel() {
    if(m_pMailUIPanel == NULL) {
        m_pMailUIPanel = MailUIPanel::create();
        m_pMailUIPanel->retain(); // release at FragControl::finalize()
    }

    return m_pMailUIPanel;
}


MailInfoPanel* MailControl::getMailInfoPanel() {
    if(m_pMailInfoPanel == NULL) {
        m_pMailInfoPanel = MailInfoPanel::create();
        m_pMailInfoPanel->retain(); // release at FragControl::finalize()
    }

    return m_pMailInfoPanel;
}

void MailControl::doQueryMailRsp(){
	if(m_pMailUIPanel && m_pMailUIPanel->isOpen()){
        m_pMailUIPanel->updateUI();
    }

    if(m_pMailInfoPanel && m_pMailInfoPanel->isOpen()){
        m_pMailInfoPanel->updateMailContent();
    }

	doUnReadNotify();
}


void MailControl::doUnReadNotify(){
	notifyMailUnread();
}

void MailControl::showGetAttachTip(const UNIT64_t uMailID ) {
    const CSReadMailRsp* pMailContent = P_MAIL_CTRL->getMailObj()->getMailContent(uMailID);
    if(NULL == pMailContent) {
        return;
    }

    const int iAttachCnt = pMailContent->attach_size();
    for(int i = 0; i < iAttachCnt; ++i) {
        const CSMailAttach& stAttach = pMailContent->attach(i);
        POP_TIP_LAYER_MASTER->ShowAwardTip(stAttach.attachtype(), stAttach.attachcfgid(), stAttach.attachcnt());
    }
    
}
