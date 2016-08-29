#ifndef __MAIL_INFO_PANEL_H__
#define __MAIL_INFO_PANEL_H__

#include "BasePanel.h"
#include "BasicTypes.h"

class IconCell;
namespace vmsg {
    class CSMail;
};

class MailInfoPanel : public BasePanel
{
public:
	MailInfoPanel();
	virtual ~MailInfoPanel();

    CREATE_FUNC(MailInfoPanel);

    bool init();

    virtual void open();
    virtual void close();
    
    void showMail(const vmsg::CSMail* pstMail);

    void updateMailContent();
private:
	void touchEventHandler(CCObject* pSender, TouchEventType type);

	UIButton* m_pCloseBtn;

    UILabel *m_pTitleTxt;
    UILabel *m_pContentTxt; 

    UILabel *m_pAttachTipTxt;

    UIImageView *m_pCellOne;
    IconCell *m_pItemOne;
    UIImageView *m_pCellTwo;
    IconCell *m_pItemTwo;

    UIButton* m_pGetBtn;

    UNIT64_t m_iMailID;
    
};

#endif // __MAIL_INFO_PANEL_H__
