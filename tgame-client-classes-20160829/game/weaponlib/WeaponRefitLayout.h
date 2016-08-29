#ifndef WeaponRefitLayout_h__
#define WeaponRefitLayout_h__

#include "BasePanel.h"
#include "BasicTypes.h"

class IconCell;


class WeaponRefitLayout : public CCObject {
public:
    enum {
        MAX_REFIT_ITEM_NUM = 6,
    };

    WeaponRefitLayout(){
        m_bIsMaterialEnough = false;
    }
    virtual ~WeaponRefitLayout() {
        CC_SAFE_RELEASE_NULL(m_pRefLayout);
    }

    void initWithLayout(UILayout *pRefLayout);
    UILayout* getRefLayout() {
        return m_pRefLayout;
    }


    void updateWeaponInfo(const UINT64_t weaponGUID);

    void resetUI();
private:
    void touchEventHandler(CCObject* pSender, TouchEventType type);
    void setMaterialNumTxt(const int iCfgID, const int iCnt, UILabel* pTxt);
    void doRefit();
private:
    UILayout* m_pRefLayout; //
    
    //from equip info
    UILabel* m_pFromColorValTxt; //color_txt_0
    UILabel* m_pFromAtkValTxt; //atk_val_0
    UILabel* m_pFromAtkAttrTxt; //atk_attr_val_0


    //to equip info
    UILabel* m_pToColorValTxt; //color_txt_1
    UILabel* m_pToAtkValTxt; //atk_val_1
    UILabel* m_pToAtkAttrTxt; //atk_attr_val_1


    UIButton* m_pDoRefitBtn; //do_refit_btn

    IconCell* m_pItemCellLst[MAX_REFIT_ITEM_NUM]; //材料数组
    UIImageView* m_pCellBgLst[MAX_REFIT_ITEM_NUM]; //材料bg数组

    UINT64_t m_uWeaponGuid;
    bool m_bIsMaterialEnough;

};


#endif // WeaponRefitLayout_h__
