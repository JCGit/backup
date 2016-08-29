#ifndef EquipAdvLayout_h__
#define EquipAdvLayout_h__

#include "BasePanel.h"
#include "BasicTypes.h"
#include "EquipLibBaseTabLayout.h"

class IconCell;
class CStarProgressBar;

class EquipAdvLayout : public CCObject, public EquipLibBaseTabLayout {
public:
    enum 
    {
        MAX_EQUIP_ADV_ITEM_NUM = 5,
    };    
    EquipAdvLayout(){
        m_pRefLayout = NULL;
        m_iEquipBagIdx = -1;
        m_uEquipGuid = 0;
        m_iEquipCfgID = 0;
        m_bIsMatEnough = false;
    }
    virtual ~EquipAdvLayout() {
        CC_SAFE_RELEASE_NULL(m_pRefLayout);
    }

    void initWithLayout(UILayout *pRefLayout);
    UILayout* getRefLayout() {
        return m_pRefLayout;
    }

    void doUpdateUI();
    virtual void resetUI();
private:
    void initUI();
    void touchEventHandler(CCObject* pSender, TouchEventType type);
    void touchCellEventHandler(CCObject* pSender, TouchEventType type);

    void setMaterialNumTxt(const int iType, const int iCnt, const int iCfgID, UILabel* pTxt);
    void doAdv();
private:
    UILayout* m_pRefLayout; //

    UILabel* m_pFromMaxLvTxt;
    UILabel* m_pFromGrowupAttrTxt;
    UILabel* m_pFromAttrTxt;

    UILabel* m_pAttrNameTxt;

    UILabel* m_pToMaxLvTxt;
    UILabel* m_pToGrowupAttrTxt;
    UILabel* m_pToAttrTxt;

    UILabel* m_pCoinTxt;

    IconCell* m_pItemCellLst[MAX_EQUIP_ADV_ITEM_NUM]; //材料数组
    UIImageView* m_pCellBgLst[MAX_EQUIP_ADV_ITEM_NUM]; //材料bg数组

    int m_iEquipBagIdx;
    UINT64_t m_uEquipGuid;
    int m_iEquipCfgID;
    int m_iEquipLv;
    bool m_bIsMatEnough;
};


#endif // EquipAdvLayout_h__
