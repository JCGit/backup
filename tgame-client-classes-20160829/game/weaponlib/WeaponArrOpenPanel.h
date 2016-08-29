#ifndef WeaponArrOpenPanel_h__
#define WeaponArrOpenPanel_h__

#include "BasePanel.h"

class CWeaponArrOpenPanel : public BasePanel
{
public:
	CWeaponArrOpenPanel();
	virtual ~CWeaponArrOpenPanel();

	CREATE_FUNC(CWeaponArrOpenPanel);

	bool init();

	void updateUI();

	void open();
	void close();

	void setArrIdx(int iIdx);

	int getArrIdx(){
		return m_uArrIdx;
	}

protected:
private:
	void initUI();
	void touchEventHandler(CCObject *pSender, TouchEventType type);

	Button* m_pCloseBtn;
	Button* m_pChargeBtn;
	Button* m_pCancelBtn;

	Label* m_pDescTxt;

	int m_uArrIdx;
};

#endif // WeaponArrOpenPanel_h__
