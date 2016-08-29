#ifndef WeaponArrRulePanel_h__
#define WeaponArrRulePanel_h__

#include "BasePanel.h"

class CWeaponArrRulePanel : public BasePanel
{
public:
	CWeaponArrRulePanel();
	virtual ~CWeaponArrRulePanel();

	CREATE_FUNC(CWeaponArrRulePanel);

	bool init();

	void updateUI();

	void open();
	void close();

protected:
private:
	void initUI();
	void touchEventHandler(CCObject *pSender, TouchEventType type);
	void limitWidth(string &text, unsigned int width);

	Button* m_pCloseBtn;
	ListView* m_pRuleList;
};

#endif // WeaponArrRulePanel_h__
