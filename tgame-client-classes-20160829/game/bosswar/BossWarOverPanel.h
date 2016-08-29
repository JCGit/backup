#ifndef BossWarOverPanel_h__
#define BossWarOverPanel_h__

#include "BasePanel.h"

class CBossWarOverPanel : public BasePanel
{
public:
	CBossWarOverPanel();
	virtual ~CBossWarOverPanel();

	CREATE_FUNC(CBossWarOverPanel);

	bool init();

	void updateUI();

	void open();
	void close();

protected:
private:
	void initUI();
	void touchEventHandler(CCObject *pSender, TouchEventType type);

	void doOver();

	ImageView* m_pWinImg;
	ImageView* m_pFailImg;

	Label* m_pNameTxt;

	Button* m_pCloseBtn;
	Button* m_pQuitBtn;

};

#endif // BossWarOverPanel_h__
