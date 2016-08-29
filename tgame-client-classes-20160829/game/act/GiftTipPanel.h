#ifndef GiftTipPanel_h__
#define GiftTipPanel_h__

#include "BasePanel.h"

class IconCell;

class GiftTipPanel : public BasePanel
{
public:
    enum {
        MAX_REFIT_ITEM_NUM = 6,
    };
	GiftTipPanel();
	virtual ~GiftTipPanel();
	CREATE_FUNC(GiftTipPanel);
    bool init();

    void setItemInfo(const int iType, const int iCfgID) {
        m_iType = iType;
        m_iCfgID = iCfgID;
    }

    virtual void open();
    virtual void close();
    
    void updateUI();
private:
	void touchEventHandler(CCObject* pSender, TouchEventType type);

	void listViewEventHandler(CCObject *pSender, ListViewEventType type);
	UIButton* m_pCloseBtn;

    UILabel *m_pDescTxt; 
    UIButton *m_pDropTipBtn;
	ListView* m_pItemList;

    int m_iType;
    int m_iCfgID;

};

#endif // GiftTipPanel_h__
