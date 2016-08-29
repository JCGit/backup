#ifndef GameExitPanel_h__
#define GameExitPanel_h__

#include "BasePanel.h"

class GameExitPanel : public BasePanel
{
public:
	GameExitPanel();
	virtual ~GameExitPanel();

	CREATE_FUNC(GameExitPanel);

	bool init();

	void show(string strDesc);

	void open();

	void close();
protected:
private:
	void touchEventHandler(CCObject *pSender, TouchEventType type);

	UIButton    *m_pYesBtn;
	UIButton    *m_pNoBtn;
	UIButton	*m_pCloseBtn;
	UILabel     *m_pDescTxt;
};

#endif // GameExitPanel_h__
