#ifndef LotteryVIPTipPanel_h__
#define LotteryVIPTipPanel_h__

#include "basic/BasePanel.h"

class ImageNumber;

class LotteryVIPTipPanel: public BasePanel
{
public:
	LotteryVIPTipPanel();
	virtual ~LotteryVIPTipPanel();

	static LotteryVIPTipPanel* create();

	// override start

	virtual bool init();

	virtual void open();

	// override end

	void setGoldLotteryTip(bool bGoldLotteryTip);

	void updateUI();

private:
	void initUI();

	void touchEventHandler(CCObject *pSender, TouchEventType type);

private:
	UIButton *m_pCloseBtn;

	UIImageView *m_pTitleImg;

	UILayout *m_pVIPIntroLayout;

	UILabel *m_pCurVipTxt;

	ImageNumber *m_pCurVIPImgTxt;

	UILabel *m_pChargeTxt;

	UILabel *m_pGoldValTxt;

	UILabel *m_pNextVIPTxt;

	ImageNumber *m_pNextVIPImgTxt;

	UILabel *m_pVIPLotteryTxt;

	UILabel *m_pVIPPackageTitleTxt;

	UILayout *m_pAwardsLayout;

	UIButton *m_pChargeBtn;

	UILayout *m_pVIPTopLayout;

	ImageNumber *m_pTopVIPTxt;

	bool m_bIsGoldLotteryTip;

};

#endif // LotteryVIPTipPanel_h__