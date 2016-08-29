#ifndef GuildFindPanel_h__
#define GuildFindPanel_h__

#include "basic/BasePanel.h"

class GuildFindPanel: public BasePanel
{
public:
	GuildFindPanel();
	virtual ~GuildFindPanel();

	static GuildFindPanel* create();

	// override start

	virtual bool init();

	virtual void open();

	// override end

private:
	void initUI();

	void touchEventHandler(CCObject *pSender, TouchEventType type);

	void onDialog();

private:
	UIButton *m_pCloseBtn;

	UITextField *m_pGuildNameInputTxt;

	UIButton *m_pFindBtn;

};

#endif // GuildFindPanel_h__
