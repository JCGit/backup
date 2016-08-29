#include "FriendAddPanelUI.h"
#include "FriendControl.h"

FriendAddPanelUI::FriendAddPanelUI()
{

}

FriendAddPanelUI::~FriendAddPanelUI()
{
}

bool FriendAddPanelUI::init() 
{
	bool bRet = false;

	do 
    {
		CC_BREAK_IF(!BasePanel::init());

		setClickOutToClose(true);

		initLayoutFromJsonFile("ui_v2/friend_add_panel_ui.ExportJson");
		addChild(m_pRootLayout);

        m_pInputTxt = dynamic_cast<UITextField*>(m_pRootLayout->getChildByName("input_txt"));

        m_pCloseBtn = dynamic_cast<UIButton*>(m_pRootLayout->getChildByName("close_btn"));    
        m_pCloseBtn->addTouchEventListener(this, toucheventselector(FriendAddPanelUI::touchEventHandler));

        m_pDoAddBtn = dynamic_cast<UIButton*>(m_pRootLayout->getChildByName("do_add_btn"));    
        m_pDoAddBtn->addTouchEventListener(this, toucheventselector(FriendAddPanelUI::touchEventHandler));
        
		bRet = true;
	} 
    while(0);

	return bRet;
}

void FriendAddPanelUI::open()
{
    BasePanel::open();
}


void FriendAddPanelUI::close()
{
    BasePanel::close();
    m_pInputTxt->setText("");
}

void FriendAddPanelUI::touchEventHandler(CCObject *pSender, TouchEventType type) 
{
	UIWidget *pWidget = dynamic_cast<UIWidget*>(pSender);
    const string strWidgetName(pWidget->getName());
    
	switch(type) {
		case TOUCH_EVENT_ENDED:
			if(strWidgetName == "close_btn") {
				close();
			} else if(strWidgetName == "do_add_btn") {
                std::string str(m_pInputTxt->getStringValue());
                if(str.length() <= 0) {
                    CCLOG("%s,friend name can not be null", strWidgetName.c_str());
                    return;
                }
                P_FRIEND_CTRL->getProxy()->sendAddReqRqst(str);
                
                close();
            } 
            
		default:
			break;
	}
}

