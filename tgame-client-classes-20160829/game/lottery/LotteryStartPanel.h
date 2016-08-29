#ifndef LotteryStartPanel_h__
#define LotteryStartPanel_h__

#include "basic/BasePanel.h"

class LotteryStartPanel: public BasePanel
{
public:
	LotteryStartPanel();
	virtual ~LotteryStartPanel();

	static LotteryStartPanel* create();

	// override start

	virtual bool init();

	// override end

	void openWithLotteryType(unsigned int uLotteryType, bool isFree = false);

	void lottery(unsigned int uLotteryType);

private:
	void initUI();

	void touchEventHandler(CCObject *pSender, TouchEventType type);

	void onGoldLotteryDialog();

	void onGoldBatchLotteryDialog();

	void onVipLotteryDialog();

private:
	UIButton *m_pCloseBtn;

	UILayout *m_pCoinLotteryLayout; // ��ҳ鿨

	UIImageView *m_pCoinOnceLotteryImg;

	UILabel *m_pOnceCoinValTxt;

	UIImageView *m_pCoinBatchLotteryImg;

	UILabel *m_pBatchCoinValTxt;

	UILayout *m_pGoldLotteryLayout; // ��ʯ�鿨

	UIImageView *m_pGoldOnceLotteryImg;

	UILabel *m_pOnceGoldValTxt;

	UIImageView *m_pGoldBatchLotteryImg;

	UILabel *m_pBatchGoldValTxt;

	UILabelAtlas *m_pGoldLotCertainlyTxt;

	UILabelAtlas *m_pGoldWeaponAdvTxt;

	UILayout *m_pVIPLotteryLayout; // VIP�鿨

	UIImageView *m_pVIPLotteryImg;

	UILabel *m_pVIPGoldValTxt;

	UILabelAtlas *m_pVIPLotCertainlyTxt;

	UILabelAtlas *m_pVIPWeaponAdvTxt;

};

#endif // LotteryStartPanel_h__