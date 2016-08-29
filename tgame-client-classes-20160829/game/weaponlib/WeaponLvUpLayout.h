#ifndef WeaponLvUpLayout_h__
#define WeaponLvUpLayout_h__

#include "BasePanel.h"
#include "BasicTypes.h"

class IconCell;


class WeaponLvUpLayout : public CCObject {
public:
    enum {
        MAX_ITEM_NUM = 6,
        LV_UP_EFF_NODE_TAG = 109,
    };

    WeaponLvUpLayout(){
        m_uCostCoin = 0;
		m_bHasWeaponMaterial = false;
    }
    virtual ~WeaponLvUpLayout() {
        CC_SAFE_RELEASE_NULL(m_pRefLayout);
    }

    void initWithLayout(UILayout *pRefLayout);
    UILayout* getRefLayout() {
        return m_pRefLayout;
    }


    void updateWeaponInfo(const UINT64_t weaponGUID, const bool bIsRsp = false);
    void doSelMatResult();

	UIImageView* getMaterialCell();

	UIButton* getLevelUpBtn();
    void resetUI();
	UIButton* getAutoSelBtn();

private:
    void setInfo(const UINT64_t weaponGUID);
    void showLvUpDoneEff(); //材料消失特效
    void onEffDone(CCArmature * armature, MovementEventType type, const char * name);
private:
    void touchEventHandler(CCObject* pSender, TouchEventType type);
    void touchCellEventHandler(CCObject* pSender, TouchEventType type);
    void setMaterialNumTxt(const int iCfgID, const int iCnt, UILabel* pTxt);
    void doLvUp();
    void doAutoSel(); //自动选择材料
    void updateCells();
    void resetCells();
    bool isLvFull();
	void onClickDialogGotoBtn();
private:
    UILayout* m_pRefLayout; //
    
    UILabel* m_pExpTipTxt; //exp_tip_txt
    UILabel* m_pCoinTxt; //coin_txt
    UILabel* m_pTotalItemExpTxt; //total_item_exp_tip_txt
    
    UIButton* m_pAutoSelBtn; //auto_sel_btn
    UIButton* m_pDoLvUpBtn; //do_lv_up_btn

    IconCell* m_pItemCellLst[MAX_ITEM_NUM]; //材料数组
    UIImageView* m_pCellBgLst[MAX_ITEM_NUM]; //材料bg数组

    UINT64_t m_uWeaponGuid;
    int m_uCostCoin;
    int m_iEffCellNum;

	bool m_bHasWeaponMaterial; // 升级材料是否选了枪械

};


#endif // WeaponLvUpLayout_h__
