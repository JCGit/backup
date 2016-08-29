#ifndef BeStrongerPanel_h__
#define BeStrongerPanel_h__

#include "basic/BasePanel.h"

class ImageNumber;

class BeStrongerPanel: public BasePanel
{
public:
	BeStrongerPanel();
	virtual ~BeStrongerPanel();

	static BeStrongerPanel* create();

	// override start

	virtual bool init();

	virtual void open();

	virtual void close();

	virtual bool isMeetAutoOpen();

	// override end

	void updateUI();

private:
	void initUI();

	void touchEventHandler(CCObject *pSender, TouchEventType type);

	void listViewEventHandler(CCObject *pSender, ListViewEventType type);

private:
	UIButton *m_pCloseBtn;

	UILayout *m_pVIPIntroLayout;

	UILabel *m_pBecomeTxt;

	ImageNumber *m_pNextVIPImgTxt;

	UIButton *m_pPrivilegeBtn;

	UIButton *m_pChargeBtn;

	UILabel *m_pVIPPackageTitleTxt;

	UILayout *m_pAwardsLayout;

	UILayout *m_pTopVIPLayout;

	UIButton *m_pTopChargeBtn;

	UIListView *m_pEntrysListView;

};

#endif // BeStrongerPanel_h__