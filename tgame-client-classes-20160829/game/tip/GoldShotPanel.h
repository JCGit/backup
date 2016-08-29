#ifndef GoldShotPanel_h__
#define GoldShotPanel_h__

#include "BasePanel.h"

class CGoldShotPanel : public BasePanel
{
public:
	CGoldShotPanel();
	virtual ~CGoldShotPanel();

	bool init();

	virtual void close();

protected:
private:
	void touchEventHandler(CCObject *pSender, TouchEventType type);

	Button* m_pSureBtn;
	Button* m_pCancelBtn;
	Button* m_pCloseBtn;
};

#endif // GoldShotPanel_h__
