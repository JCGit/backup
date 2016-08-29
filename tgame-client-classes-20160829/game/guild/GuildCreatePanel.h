#ifndef GuildCreatePanel_h__
#define GuildCreatePanel_h__

#include "basic/BasePanel.h"

class GuildCreatePanel: public BasePanel
{
public:
	GuildCreatePanel();
	virtual ~GuildCreatePanel();

	static GuildCreatePanel* create();

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

	UILabel *m_pWordsTxt;

	UITextField *m_pGuildNameInputTxt;

	UIButton *m_pCreateBtn;

};

#endif // GuildCreatePanel_h__
