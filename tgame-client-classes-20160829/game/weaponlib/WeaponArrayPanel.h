#ifndef WeaponArrayPanel_h__
#define WeaponArrayPanel_h__

#include "BasePanel.h"
#include "WeaponArrCell.h"
#include "game/guide/GuideContainer.h"

class CWeaponArrayPanel : public BasePanel, public GuideContainer
{
public:
	enum {
		MAX_ARR_CELL_NUM = 5,
	};

public:
	CWeaponArrayPanel();
	virtual ~CWeaponArrayPanel();

	CREATE_FUNC(CWeaponArrayPanel);

	bool init();

	virtual CCNode* getGuideNodeByName(const std::string &name);

	void updateUI();

	void doUpdateUI();

	void open();
	void close();

protected:
private:
	void initUI();
	void touchEventHandler(CCObject *pSender, TouchEventType type);

	Button* m_pCloseBtn;
	Button* m_pRuleBtn;

	Label* m_pAddTxt;

	ImageView* m_pEquipWeaponBg;
	ImageView* m_pEquipWeaponImg;

	CCArray* m_stCellLst;
};

#endif // WeaponArrayPanel_h__
