#ifndef VIPDetailLayout_h__
#define VIPDetailLayout_h__

#include "basic/BaseLayout.h"

class CStarProgressBar;
class ImageNumber;

class VIPDetailLayout: public BaseLayout
{
public:
	enum AwardStatus {
		AWARD_NONE, // 不可领取
		AWARD_GET, // 领取
		AWARD_GOT, // 已领取
	};
public:
	VIPDetailLayout();
	virtual ~VIPDetailLayout();
	
	static VIPDetailLayout* create(UILayout *pRefLayout);

	bool initWithLayout(UILayout *pRefLayout);

	void updateUI(unsigned int uVIPLv);

	unsigned int getVIPLv();

	void updateAwardCell();

private:
	void touchEventHandler(CCObject *pSender, TouchEventType type);

	void limitWith(std::string &text, unsigned int width);

	void updatePrivilege(unsigned int uLv, UIScrollView* pScroll, UILabel* pLabel);

private:
	UIScrollView *m_pPermissionScrollView1;
	UIScrollView *m_pPermissionScrollView2;

	UILabel *m_pPermissionDescTxt1;
	UILabel *m_pPermissionDescTxt2;

	UIImageView* m_pNextVipGiftImg;
	UIImageView* m_pNextVipPrivilegeImg;
	UIImageView* m_pFullVipTipImg;
	
    std::vector<UIImageView*> m_stCellBgLst;

    UIImageView* m_pWeaponCellBg;

    CStarProgressBar* m_pWeaponLvStar;

	ImageNumber *m_pVIPLvImgNum;

	unsigned int m_uVIPLv;

};

#endif // VIPDetailLayout_h__