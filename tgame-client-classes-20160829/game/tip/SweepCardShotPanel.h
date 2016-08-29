#ifndef SweepCardShotPanel_h__
#define SweepCardShotPanel_h__

#include "BasePanel.h"
#include "CSGoldMsg.pb.h"

class CSweepCardShotPanel : public BasePanel
{
public:
	CSweepCardShotPanel();
	virtual ~CSweepCardShotPanel();

	CREATE_FUNC(CSweepCardShotPanel);

	bool init();

	void updateUI(const vmsg::CSQueryRMBStoreItemRsp& stRsp);

	void open();
	void close();

protected:
private:
	void initUI();
	void touchEventHandler(CCObject *pSender, TouchEventType type);

	void doBuy();

	Button* m_pCloseBtn;
	Button* m_pChallengeBtn;
	Button* m_pChargeBtn;
	Button* m_pBuyBtn;

	vmsg::CSQueryRMBStoreItemRsp m_stInfo;
};

#endif // SweepCardShotPanel_h__
