#ifndef FirstChargePanel_h__
#define FirstChargePanel_h__

#include "basic/BasePanel.h"

class FirstChargePanel: public BasePanel
{
public:
	FirstChargePanel();
	virtual ~FirstChargePanel();

	static FirstChargePanel* create();

	// override start

	virtual bool init();

	virtual void open();

	virtual void close();

	virtual bool isMeetAutoOpen();

	// override end

private:
	void initUI();

	void touchEventHandler(CCObject *pSender, TouchEventType type);

private:
	UIButton *m_pCloseBtn;

	CCArray *m_pUICellArray;

	CCArray *m_pCellArray;

	Button* m_pGetBtn;

	ImageView* m_pBtnImg;

	unsigned int m_uPkgType;

	unsigned int m_uState;

};

#endif // FirstChargePanel_h__