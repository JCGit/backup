#ifndef AwardBoxTipPanel_h__
#define AwardBoxTipPanel_h__

#include "basic/BasePanel.h"

class IconFrameCell;

class AwardBoxTipPanel: public BasePanel
{
public:
	AwardBoxTipPanel();
	virtual ~AwardBoxTipPanel();

	static AwardBoxTipPanel* create();

	// override start

	virtual bool init();

	virtual void open();

	// override end

	void show(unsigned int uType, unsigned int uCond, unsigned int uAwardID);

private:
	void initUI();

	void touchEventHandler(CCObject *pSender, TouchEventType type);

private:
	UILabel *m_pAwardTxt;

	IconFrameCell *m_pAwardIconFrameCell;

	UILabel *m_pItemCntTxt;

	UILabel *m_pCondDescTxt;

};

#endif // AwardBoxTipPanel_h__