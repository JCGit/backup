#ifndef WeaponArrCell_h__
#define WeaponArrCell_h__

#include "BaseLayout.h"
#include "CSWeaponMsg.pb.h"

using namespace vmsg;

class CWeaponArrCell : public BaseLayout
{
public:
	CWeaponArrCell();
	virtual ~CWeaponArrCell();

	static CWeaponArrCell* create(UILayout *pRefLayout);

	bool initWithLayout(UILayout *pRefLayout);

	void updateUI();

	void setInfo(const CSInbattleWeaponElmt& stElmt);
	const CSInbattleWeaponElmt& getInfo(){
		return m_stInfo;
	}

	ImageView* getBgImg(){
		return m_pWeaponBg;
	}

protected:
private:
	void initUI();
	void touchEventHandler(CCObject *pSender, TouchEventType type);

	ImageView* m_pWeaponBg;
	ImageView* m_pFrameBg;
	ImageView* m_pIconBg;
	ImageView* m_pWeaponImg;

private:
	CSInbattleWeaponElmt m_stInfo;

};

#endif // WeaponArrCell_h__
