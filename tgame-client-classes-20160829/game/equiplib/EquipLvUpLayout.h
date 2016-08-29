#ifndef EquipLvUpLayout_h__
#define EquipLvUpLayout_h__

#include "BasePanel.h"
#include "CSEquipItemMsg.pb.h"
#include "EquipLibBaseTabLayout.h"
#include "EquipLibObj.h"

using namespace vmsg;
class IconCell;
class CStarProgressBar;

class EquipLvUpLayout : public CCObject, public EquipLibBaseTabLayout {
public:
    enum 
    {
        LV_UP_EFF_NODE_TAG = 11,
    };
    EquipLvUpLayout(){
        m_pRefLayout = NULL;
        m_iEquipBagIdx = -1;
        m_uCostCoin = 0;
    }
    virtual ~EquipLvUpLayout() {
        CC_SAFE_RELEASE_NULL(m_pRefLayout);
    }

    void initWithLayout(UILayout *pRefLayout);
    UILayout* getRefLayout() {
        return m_pRefLayout;
    }

    void updateSelMat();
    void doUpdateUI(const bool bIsRsp);

    virtual void resetUI();


	UIButton* getAutoSelBtn();
	UIButton* getLvUpBtn();

private:
    void setInfo(const CSEquipInfo& stEquip, const int iBagIdx);
    void initUI();
    void touchCellEventHandler(CCObject* pSender, TouchEventType type);
    void doAutoSel();
    void doLvUp();
    void resetMatCell();
    void onConfiremLvUp(); //二次确认

    void showLvUpDoneEff(const bool bIsLvUp);
    void onEffDone(CCArmature * armature, MovementEventType type, const char * name);


    void playLvUpEffCallback(CCArmature* armature, MovementEventType type, const char * name);
	void onClickDialogGotoBtn();
private:
    UILayout* m_pRefLayout; //

    UILabel* m_pFullRemainExp; 
    UILabel* m_pTotalGetExp; 
    UILabel* m_pCoinTxt;


    UILoadingBar* m_pExpBar;

    IconCell* m_pItemCellLst[EQUIP_SEL_MAT_MAX_NUM]; //材料数组
    UIImageView* m_pCellBgLst[EQUIP_SEL_MAT_MAX_NUM]; //材料bg数组

    UIButton* m_pAutoSelBtn;
    UIButton* m_pDoLvUpBtn;


    CSEquipInfo m_stEquip;
    int m_iEquipBagIdx;
    int m_uCostCoin;

    //特效
    int m_iEffCellNum;
};


#endif // EquipLvUpLayout_h__
