#ifndef SweepInfoPanel_h__
#define SweepInfoPanel_h__

#include "BasePanel.h"

class CSweepInfoPanel : public BasePanel
{
public:
	CSweepInfoPanel();
	virtual ~CSweepInfoPanel();

	CREATE_FUNC(CSweepInfoPanel);

	bool init();

	void updateUI();

	void open();
	void close();
protected:
private:
	void touchEventHandler(CCObject *pSender, TouchEventType type);

	void stepUpdate(float dt);

	UIButton* m_pCloseBtn;
	ListView* m_pInfoList;
	UILabel* m_pTipTxt;

	UILabelAtlas *m_pMinutesTxt;
	UILabelAtlas *m_pSecondsTxt;

	bool m_bIsError;		//是否提示背包已满

	unsigned int m_uIDX;
};

#endif // SweepInfoPanel_h__
