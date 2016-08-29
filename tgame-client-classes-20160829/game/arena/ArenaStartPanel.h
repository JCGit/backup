#ifndef ArenaStartLayer_h__
#define ArenaStartLayer_h__

#include "BasePanel.h"
#include "vlib/QEventDispatcher.h"

class CArenaStartPanel : public BasePanel, public CQEventDispatcher
{
public:
	static const string START_PVP; 
public:
	CArenaStartPanel();
	virtual ~CArenaStartPanel();

	CREATE_FUNC(CArenaStartPanel);

	bool init();
protected:
private:
	void touchEventHandler(CCObject *pSender, TouchEventType type);

	ImageView* m_pTipImg;

};

#endif // ArenaStartPanel_h__
