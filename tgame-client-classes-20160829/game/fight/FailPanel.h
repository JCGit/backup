#ifndef FailPanel_h__
#define FailPanel_h__

#include "BasePanel.h"

class ImageNumber;

class CFailPanel : public BasePanel
{
public:
	CFailPanel();
	virtual ~CFailPanel();

	static CFailPanel* create();

	virtual bool init();

	virtual void open();

	void updateUI();

private:
	void initUI();

	void touchEventHandler(CCObject *pSender, TouchEventType type);

	void listViewEventhandler(CCObject *pSender, ListViewEventType type);

	void exit();

private:
	UIButton *m_pExitBtn;

	UIListView *m_pEntrysListView;

	UIButton *m_pChargeBtn;

	UILabel *m_pBecomeTxt;

	ImageNumber *m_pVIPImgTxt;

	UILabel *m_pOwnTxt;

	UILabel *m_pPrivilegeTxt;

	UIImageView *m_pBgPackageTitleImg;

	UILabel *m_pPackageTitleTxt;

	UIListView *m_pElmtsListView;

};

#endif // FailPanel_h__
