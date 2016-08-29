#ifndef CardDrawPanel_h__
#define CardDrawPanel_h__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "basic/BasePanel.h"
#include "CardCell.h"
#include "CardCfgMaster.h"

USING_NS_CC;
USING_NS_CC_EXT;

using namespace gui;
using namespace std;

const int DEFAULT_TIMES = 1;	
const int DOWN_TIME = 6;		//倒计时

class CCardDrawPanel : public BasePanel
{
public:
	CCardDrawPanel();
	virtual ~CCardDrawPanel();

	CREATE_FUNC(CCardDrawPanel);

	virtual bool init();
	void open();
	virtual void close();

	void updateUI();

	void updateCurCard();

protected:
    void lockCards();
    void unLockCards();
private:
	void initUI();
	void touchEventHandler(CCObject *pSender, TouchEventType type);
	void handleOpenOtherEvent(const string& stEvent, CCObject* pstEventObj);

	void openCard(CCardCell* pCell);
	void openCostCard();
	void openOtherFreeCards();		//翻开剩余的免费卡
	void openOtherCostCards();		//翻开剩余的花费卡

	vector<CardElement> getOtherCardsInfo(bool isFree = true);	//从配置中获取其他卡牌的信息

	void downTime(float dt);				//倒计时

	UILabel* timeTxt;
	UILabel* timeValueTxt;
	UIButton* sureBtn;
    UIImageView* m_pPanelBg;

	CCardCell* curCard;
	CCardCell* costCurCard;

	CCArray* pCards;		//卡牌

	bool b_isOpenFreeCard;	//是否打开免费卡

	int totalTime;			//总的倒计时
	int freeTimes;			//免费次数
	int costTimes;			//花费的次数
};

#endif // CardDrawPanel_h__
