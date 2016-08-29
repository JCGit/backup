#ifndef LevelUpEntryPanel_h__
#define LevelUpEntryPanel_h__

#include "basic/BasePanel.h"

class LevelUpEntryPanel: public BasePanel
{
public:
	LevelUpEntryPanel();
	virtual ~LevelUpEntryPanel();

	static LevelUpEntryPanel* create();

	// override start

	virtual bool init();

	virtual void open();

	// override end

private:
	void initUI();

	void touchEventHandler(CCObject *pSender, TouchEventType type);

	void listViewEventHandler(CCObject *pSender, ListViewEventType type);

private:
	UIButton *m_pCloseBtn;

	UIListView *m_pEntrysListView;

};

#endif // LevelUpEntryPanel_h__