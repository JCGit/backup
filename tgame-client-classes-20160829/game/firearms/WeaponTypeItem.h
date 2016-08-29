#ifndef WeaponTypeItem_h__
#define WeaponTypeItem_h__

#include "BaseItem.h"
#include "vlib/QEventDispatcher.h"
using namespace CVLib;
using namespace std;

class CWeaponTypeItem : public CBaseItem, public CQEventDispatcher
{
public:	 
	const static string WEAPON_TAB_BTN_CLICK;
public:
	CWeaponTypeItem();
	virtual ~CWeaponTypeItem();

	CREATE_FUNC(CWeaponTypeItem);

	bool init();

	void setSelected(bool var);
	bool getSelected();

	void setTxt(const std::string& text);
	void setNum(unsigned int num);
    ImageView* getImgView() {
        return m_pTabBg;
    }
	bool isNumShow(){return m_pNum->isVisible();}
protected:
private:
	//void touchEventHandler(CCObject* pSender, TouchEventType type);

	ImageView* m_pTabBg;
	UILabel* m_pNum;
    UILabel* m_pTabTxt;
    UIImageView* m_pBgNum;
};


#endif // WeaponTypeItem_h__
