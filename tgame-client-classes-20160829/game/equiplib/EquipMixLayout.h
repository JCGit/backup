#ifndef EquipMixLayout_h__
#define EquipMixLayout_h__

#include "BasePanel.h"
#include "BasicTypes.h"
#include "EquipLibBaseTabLayout.h"

class IconCell;

class EquipMixLayout : public CCObject, public EquipLibBaseTabLayout {
public:
    enum 
    {
        MAX_EQUIP_MIX_ITEM_NUM = 5,
    }; 
    EquipMixLayout(){
        m_pRefLayout = NULL;
        m_iEquipBagIdx = -1;
        m_uEquipGuid = 0;
        m_iEquipCfgID = 0;
        m_bIsMatEnough = false;
    }
    virtual ~EquipMixLayout() {
        CC_SAFE_RELEASE_NULL(m_pRefLayout);
    }

    void initWithLayout(UILayout *pRefLayout);
    UILayout* getRefLayout() {
        return m_pRefLayout;
    }
    virtual void resetUI();
    void doUpdateUI();
private:
    void initUI();
    void touchEventHandler(CCObject* pSender, TouchEventType type);
    void touchCellEventHandler(CCObject* pSender, TouchEventType type);
    void doMix();
    void setMaterialNumTxt(const int iType, const int iCnt, const int iCfgID, UILabel* pTxt);
    const char* getCellColorImg(const int iColor);
private:
    UILayout* m_pRefLayout; 

    UILabel* m_pAttrNameTxt;

    UILabel* m_pFromMixLvTxt;
    UILabel* m_pFromAttrValTxt;

    UILabel* m_pToAttrValTxt;
    UILabel* m_pToMixLvTxt;

    UILabel* m_pCoinTxt;

    IconCell* m_pItemCellLst[MAX_EQUIP_MIX_ITEM_NUM]; //材料数组
    UIImageView* m_pCellBgLst[MAX_EQUIP_MIX_ITEM_NUM]; //材料bg数组
    
    //data
    int m_iEquipBagIdx;
    UINT64_t m_uEquipGuid;
    int m_iEquipCfgID;
    int m_iEquipLv;
    bool m_bIsMatEnough;
};


#endif // EquipMixLayout_h__
