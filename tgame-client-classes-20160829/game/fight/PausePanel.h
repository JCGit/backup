#ifndef PausePanel_h__
#define PausePanel_h__

#include "BasePanel.h"

class CPausePanel : public BasePanel
{
public:
	CPausePanel();
	virtual ~CPausePanel();

	CREATE_FUNC(CPausePanel);

	bool init();

	void open();
	void close();
protected:
private:
	void touchEventHandler(CCObject *pSender, TouchEventType type);

	UIButton* m_pContinueBtn;
	UIButton* m_pQuitBtn;

	CCSet* m_pBeforeTargetSets;
};

#endif // PausePanel_h__
