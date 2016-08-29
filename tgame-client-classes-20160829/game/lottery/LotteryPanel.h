#ifndef LotteryPanel_h__
#define LotteryPanel_h__

#include "basic/BasePanel.h"
#include "../guide/GuideContainer.h"

class LotteryPanel: public BasePanel, public GuideContainer
{
public:
	LotteryPanel();
	virtual ~LotteryPanel();

	static LotteryPanel* create();

	// override start

	virtual bool init();

	virtual void open();

	virtual void close();

	// override end

	void updateUI();

	CCNode* getGuideNodeByName(const std::string &name);
private:
	void initUI();

	void touchEventHandler(CCObject *pSender, TouchEventType type);

	void updateTimeTxt();

	void updateTime(float dt);

private:
	UIButton *m_pCloseBtn;

	UILayout *m_pCoinLotteryLayout; // ½ð±Ò³é¿¨

	UIImageView *m_pCoinFreeImg;

	UILabel *m_pCoinFreeTimeTxt;

	UILabel *m_pCoinFreeTxt;

	UIImageView *m_pIconCoinImg;

	UILabel *m_pCoinValTxt;

	UILayout *m_pGoldLotteryLayout; // ×êÊ¯³é¿¨

	UIImageView *m_pGoldFreeImg;

	UILabel *m_pGoldFreeTimeTxt;

	UILabel *m_pGoldFreeTxt;

	UIImageView *m_pIconGoldImg;

	UILabel *m_pGoldValTxt;

	UILayout *m_pVIPLotteryLayout; // VIP³é¿¨

	UILabel *m_pTodayTimesValTxt;

	UILabel *m_pVIPLevelValTxt;

	unsigned int m_uCoinLotFreeTime;

	unsigned int m_uGoldLotFreeTime;

};

#endif // LotteryPanel_h__