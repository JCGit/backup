#ifndef HappyNewYearPanel_h__
#define HappyNewYearPanel_h__

#include "basic/BasePanel.h"

class IconFrameCell;

class HappyNewYearPanel: public BasePanel
{
public:
	HappyNewYearPanel();
	virtual ~HappyNewYearPanel();

	static HappyNewYearPanel* create();

	// override start

	virtual bool init();

	virtual void close();

	// override end

	void openWithPkgID(unsigned int uPkgID);

private:
	void initUI();

	void touchEventHandler(CCObject *pSender, TouchEventType type);

private:
	UIButton *m_pCloseBtn;

	UIButton *m_pOkBtn;

	UILayout *m_pAwardsLayout;

	vector<IconFrameCell*> m_stAwardCells;

	vector<UILabel*> m_stLabelNames;

	CCArmature *m_pFlyStars;

};

#endif // HappyNewYearPanel_h__