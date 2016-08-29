#ifndef ArenaRulePanel_h__
#define ArenaRulePanel_h__

#include "BasePanel.h"

class CArenaRulePanel : public BasePanel
{
public:
	CArenaRulePanel();
	virtual ~CArenaRulePanel();

	CREATE_FUNC(CArenaRulePanel);

	bool init();

	void updateUI();

	void open();
protected:
private:
	void touchEventHandler(CCObject *pSender, TouchEventType type);
private:
	UIButton* m_pCloseBtn;
	ListView* m_pRuleList;
};

#endif // ArenaRulePanel_h__
