#ifndef __DAILY_ACT_PANEL_H__
#define __DAILY_ACT_PANEL_H__

#include "BasePanel.h"
#include "CSActMsg.pb.h"

class DailyActPanel: public BasePanel
{
public:
	DailyActPanel();
	~DailyActPanel();

	virtual bool init();

    virtual void open();
	virtual void close();

    void updateUI();

	void updateOneItem(const vmsg::CSActStatChange& stInfo);

	void updateWorldBossActItem(unsigned int uMark);

private:
    void touchEventHandler(CCObject *pSender, TouchEventType type);

private:
	UIButton *m_pCloseBtn;

	ListView* m_pActList;
	UILayout* m_pItemDemo;

	CCArray* m_stDailyActItems;
};

#endif // __DAILY_ACT_PANEL_H__
