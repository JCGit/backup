#ifndef WeaponDetailInfoLayout_h__
#define WeaponDetailInfoLayout_h__

#include "BasePanel.h"
#include "BasicTypes.h"
class WeaponDetailInfoLayout  {
public:
    WeaponDetailInfoLayout(){

    }
    virtual ~WeaponDetailInfoLayout() {
        CC_SAFE_RELEASE_NULL(m_pRefLayout);
    }

    void initWithLayout(UILayout *pRefLayout);
    UILayout* getRefLayout() {
        return m_pRefLayout;
    }


    void updateWeaponDetailInfo(const unsigned int cfgID, const UINT64_t weaponGUID);

private:
    UILayout* m_pRefLayout; //
    UILabel* m_DescTxt; //desc_txt
    UILabel* m_pAtkGrowValTxt; //atk_grow_val_txt
    UILabel* m_pAtkAttrGrowValTxt; //atk_attr_grow_val_txt
    UILabel* m_pAtkValTxt; //atk_val_txt
    UILabel* m_pAtkSpTxt; //atk_sp_txt

    UILabel* m_pAtkTypeTxt; //atk_type_txt 
    UILabel* m_pAtkAttrTxt; //atk_attr_txt
    UILabel* m_pChuantouliTxt; //chuantouli_txt

    UILabel* m_pWeightTxt; //weight_txt
    UILabel* m_pAtkDistTxt; //atk_dist_txt
    UILabel* m_pAddPerTxt; //add_per_txt

};


#endif // WeaponDetailInfoLayout_h__
