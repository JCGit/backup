#ifndef WeaponLibListItem_h__
#define WeaponLibListItem_h__

#include "CSWeaponMsg.pb.h"
#include "BasicTypes.h"
#include "BaseLayout.h"

class CStarProgressBar;

class CWeaponLibListItemOne {
public:
    CWeaponLibListItemOne();
    ~CWeaponLibListItemOne() {}

    void initWithLayout(UILayout *pRefLayout);

    void setWeaponInfo(const unsigned int cfgID, const UINT64_t weaponGUID, const int iLv);
    void hide(); //不显示
    //bool hitTest(const CCPoint& stGlobalTouchPt);
    UILayout* getRefLayout() {
        return m_pRefLayout;
    }
    void onTouch(); //点击
private:
    void setWeaponLv(const unsigned int lev);
private:
    UILayout *m_pRefLayout;

    UILabel* m_pNameTxt;
    UILabel* m_pLvTxt;
    
    UIImageView* m_pLvImg;
    UIImageView* m_pWeaponImg;


    CStarProgressBar* m_pStarBar;	//星级

    CC_SYNTHESIZE(int, _lv, Lv);
    CC_SYNTHESIZE(unsigned int, _cfgID, CfgID);
    CC_SYNTHESIZE(UINT64_t, _weaponGUID, WeaponGUID);
};

class CWeaponLibListItem : public BaseLayout
{
public:
	CWeaponLibListItem();
	virtual ~CWeaponLibListItem();

	CREATE_FUNC(CWeaponLibListItem);

	static CWeaponLibListItem* create(UILayout *pRefLayout);

	virtual bool init();

	bool initWithLayout(UILayout *pRefLayout);

	void initUI();

	void setWeaponInfo(const int iItemIdx, const unsigned int cfgID, const UINT64_t weaponGUID, const int iLv);
    void hideSecondItem();
    void setShowNowOwnTip(const bool bIsShow);

    void onItemTouched(const CCPoint& stGlobalTouchPt);

    CCNode* getFirstItemNode();

   
private:
    CWeaponLibListItemOne m_stOne;
    CWeaponLibListItemOne m_stTwo;

};

#endif // WeaponLibListItem_h__
