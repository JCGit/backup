#ifndef WeaponRefitTipPanel_h__
#define WeaponRefitTipPanel_h__

#include "BasePanel.h"

class IconCell;

class CWeaponRefitTipPanel : public BasePanel
{
public:
    enum {
        MAX_REFIT_ITEM_NUM = 6,
    };
	CWeaponRefitTipPanel();
	virtual ~CWeaponRefitTipPanel();

    bool init();

    void setItemInfo(const int iType, const int iNum, const int iCfgID, const bool bCanOpenDropTip = true) {
        m_iType = iType;
        m_iNum = iNum;
        m_iCfgID = iCfgID;
        m_bCanOpenDropTip = bCanOpenDropTip;
    }

    virtual void open();
    virtual void close();
    
    void updateUI();
private:
	void touchEventHandler(CCObject* pSender, TouchEventType type);

	UIButton* m_pCloseBtn;

    UILabel *m_pNameTxt;
    UILabel *m_pDescTxt; 
    UIButton *m_pDropTipBtn;

    UIImageView *m_pImgCell; // cell
    IconCell *m_pCell;


    int m_iType;
    int m_iNum;
    int m_iCfgID;

    bool m_bCanOpenDropTip;
};

#endif // WeaponRefitTipPanel_h__
