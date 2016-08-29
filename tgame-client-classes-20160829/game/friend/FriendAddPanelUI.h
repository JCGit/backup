#ifndef FriendAddPanelUI_h__
#define FriendAddPanelUI_h__

#include "basic/BasePanel.h"

class FriendAddPanelUI: public BasePanel
{
public:
    FriendAddPanelUI();
    virtual ~FriendAddPanelUI();

    // override
    virtual bool init();
    virtual void open();
    virtual void close();
private:
    void touchEventHandler(CCObject *pSender, TouchEventType type);
    
private:
    UIButton* m_pCloseBtn;
    UIButton* m_pDoAddBtn;
    UITextField* m_pInputTxt;
};

#endif // FriendAddPanelUI_h__
