#ifndef PackageInfoPanel_h__
#define PackageInfoPanel_h__

#include "basic/BasePanel.h"

class PackageInfoPanel: public BasePanel
{
public:
	PackageInfoPanel();
	virtual ~PackageInfoPanel();

	static PackageInfoPanel* create();

	// override start

	virtual bool init();

	// override end

	void showPackage(unsigned int uPackageID);

private:
	void initUI();

	void touchEventHandler(CCObject *pSender, TouchEventType type);

	void listViewEventHandler(CCObject *pSender, ListViewEventType type);

private:
	UIButton *m_pCloseBtn;

	UILabel *m_pCongratulationsTxt;

	UIListView *m_pElmtsListView;

	UIButton *m_pOpenBtn;

	unsigned int m_uPackageID;

};

#endif // PackageInfoPanel_h__