#ifndef __MAIL_CONTROL_h__
#define __MAIL_CONTROL_h__

#include "MailProxy.h"
#include "MailObj.h"
#include "MailUIPanel.h"
#include "MailInfoPanel.h"

class MailControl
{
public:
    MailControl();
    ~MailControl();

    MailProxy* getProxy();

    MailUIPanel* getMailUIPanel();
    MailInfoPanel* getMailInfoPanel();

    MailObj* getMailObj() {
        return &m_stMailObj;
    }
    
	void doQueryMailRsp();

	void doUnReadNotify();

    void showGetAttachTip(const UNIT64_t uMailID );

    void finalizePanel();

	void notifyMailUnread();

private:
    MailProxy m_stMailProxy;
    
    MailUIPanel* m_pMailUIPanel;
    MailInfoPanel* m_pMailInfoPanel;

    MailObj m_stMailObj;
	
};

#define P_MAIL_CTRL CVLib::CVSingleton<MailControl>::Instance()

#endif // __MAIL_CONTROL_h__
