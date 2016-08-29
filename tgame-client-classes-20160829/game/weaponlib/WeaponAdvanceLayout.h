#ifndef WeaponAdvanceLayout_h__
#define WeaponAdvanceLayout_h__

#include "BasePanel.h"
#include "BasicTypes.h"

class IconCell;

class WeaponAdvanceLayout : public CCObject {
public:
    enum {
        MAX_ITEM_NUM = 5, //最多4个材料
        LV_UP_EFF_NODE_TAG = 127,
    };
    WeaponAdvanceLayout(){
        m_uWeaponGuid = 0;
        m_pAdvanceArmature = NULL;
        m_bIsMatEnough = false;
    }
    virtual ~WeaponAdvanceLayout() {
        CC_SAFE_RELEASE_NULL(m_pRefLayout);
    }

    void initWithLayout(UILayout *pRefLayout);
    UILayout* getRefLayout() {
        return m_pRefLayout;
    }

    void updateWeaponInfo(const UINT64_t weaponGUID, const bool bIsPlayEff = false);

	UIButton* getAdvanceBtn();
    void resetUI();

	UIImageView* getAdvanceCell(){
		if (m_pCellBgLst)
		{
			return m_pCellBgLst[1];
		}
		return NULL;
	}
private:
    void setInfo(const UINT64_t weaponGUID);
    void showEff(); //材料消失特效
    void onEffDone(CCArmature * armature, MovementEventType type, const char * name);
private:
    void touchEventHandler(CCObject* pSender, TouchEventType type);
    void setMaterialNumTxt(const int iType, const int iCnt, const int iCfgID, UILabel* pTxt);
    void doAdvance();

    void movementCallbackToRelease(CCArmature * armature, MovementEventType type, const char * name);
    void removeAdvanceArmature();

	
private:
    UILayout* m_pRefLayout; //
    //from equip info
    UILabel* m_pFromAtkValTxt; //atk_val_0
    UILabel* m_pFromAtkAttrTxt; //atk_attr_val_0
    UILabel* m_pFromAtkGrowValTxt; //atk_growup_val_0
    UILabel* m_pFromAtkAttrGrowValTxt; //attr_growup_val_0
    UILabel* m_pFromMaxLvTxt; //max_lv_val_0

    //to equip info
    UILabel* m_pToAtkValTxt; //atk_val_1
    UILabel* m_pToAtkAttrTxt; //atk_attr_val_1
    UILabel* m_pToAtkGrowValTxt; //atk_growup_val_1
    UILabel* m_pToAtkAttrGrowValTxt; //attr_growup_val_1
    UILabel* m_pToMaxLvTxt; //max_lv_val_1

    UILabel* m_pAdvTipTxt; //adv_tip_txt 等级达到X阶可进阶
    UILabel* m_pCoinTxt; //coin_txt 消耗铜钱

    UIButton* m_pDoAdvBtn; //do_adv_btn

    IconCell* m_pItemCellLst[MAX_ITEM_NUM]; //材料数组
    UIImageView* m_pCellBgLst[MAX_ITEM_NUM]; //材料bg数组

    UINT64_t m_uWeaponGuid;
    int m_iNowWeaponCfgID;
    int m_iToWeaponCfgID;

    CCArmature* m_pAdvanceArmature;
    int m_iEffCellNum;

    bool m_bIsMatEnough;
};


#endif // WeaponAdvanceLayout_h__
