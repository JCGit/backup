#ifndef WeaponAdvDonePanel_h__
#define WeaponAdvDonePanel_h__

#include "basic/BasePanel.h"
#include "game/guide/GuideControl.h"

class WeaponAdvDonePanel: public BasePanel, public GuideContainer
{
public:
    WeaponAdvDonePanel();
	virtual ~WeaponAdvDonePanel();

    // override
    virtual bool init();
    virtual void open();
    virtual void close();

	// guide
	virtual CCNode* getGuideNodeByName(const std::string &name);

    void setOneWeaponInfo(const bool bIsFromEquip, const int iCfgID, const int iLv, const int iColor);
    
private:
    void touchEventHandler(CCObject *pSender, TouchEventType type);

    void doUpdateUI();
private:
    UIImageView *m_pCloseBtn;
    UIButton *m_pDoCloseBtn;//do_close

    UIImageView* m_pFromWeaponBg;
    UIImageView* m_pFromWeaponIcon; //icon_0
    UILabel* m_pFromAtkValTxt; //atk_val_txt_0
    UILabel* m_pFromAtkAttrValTxt; //atk_attr_val_txt_0
    UILabel* m_pFromMaxLvTxt; //max_lv_txt_0

    UIImageView* m_pToWeaponBg;
    UIImageView* m_pToWeaponIcon; //icon_1
    UILabel* m_pToAtkValTxt; //atk_val_txt_1
    UILabel* m_pToAtkAttrValTxt; //atk_attr_val_txt_1
    UILabel* m_pToMaxLvTxt; //max_lv_txt_1

    int m_iFromWeaponCfgID;
    int m_iFromWeaponLv;
    int m_iFromWeaponColor;

    int m_iToWeaponCfgID;
    int m_iToWeaponLv;
    int m_iToWeaponColor;

};

#endif // WeaponAdvDonePanel_h__
