#ifndef RefitDonePanel_h__
#define RefitDonePanel_h__

#include "basic/BasePanel.h"

class RefitDonePanel: public BasePanel
{
public:
	RefitDonePanel();
	virtual ~RefitDonePanel();

	static RefitDonePanel* create();

	// override start

	virtual bool init();

	virtual void open();

	// override end

	void refitPart(unsigned int uPart);

	void updateUI();

private:
	void initUI();

	void touchEventHandler(CCObject *pSender, TouchEventType type);

private:
	UIButton *m_pCloseBtn;

	UIImageView *m_pItemImg;

	UILabel *m_pAtkValTxt;

	UILabel *m_pDefValTxt;

	UILabel *m_pHpValTxt;

	UILabel *m_pHitValTxt;

	UILabel *m_pAtkCurValTxt;

	UILabel *m_pDefCurValTxt;

	UILabel *m_pHpCurValTxt;

	UILabel *m_pHitCurValTxt;

	unsigned int m_uPart;

	unsigned int m_uAtkVal;

	unsigned int m_uDefVal;

	unsigned int m_uHpVal;

	unsigned int m_uHitVal;

};

#endif // RefitDonePanel_h__