#ifndef WarSweepInfoPanel_h__
#define WarSweepInfoPanel_h__

#include "basic/BasePanel.h"

class WarSweepInfoPanel: public BasePanel
{
public:
	WarSweepInfoPanel();
	virtual ~WarSweepInfoPanel();

	static WarSweepInfoPanel* create();

	// override start

	virtual bool init();

	virtual void close();

	// override end

	void updateUI();

private:
	void initUI();

	void touchEventHandler(CCObject *pSender, TouchEventType type);

	void sweepUpdate(float dt);

private:
	UIButton *m_pCloseBtn;

	UIListView *m_pSweepInfoListView;

	UILabel *m_pSweepStopTipTxt;

	int m_nSweepIdx;

};

#endif // WarSweepInfoPanel_h__