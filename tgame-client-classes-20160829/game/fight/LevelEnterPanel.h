#ifndef LevelEnterPanel_h__
#define LevelEnterPanel_h__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "basic/BasePanel.h"

#include "game/guide/GuideControl.h"

USING_NS_CC;
USING_NS_CC_EXT;

using namespace gui;
using namespace std;

class CLevelEnterPanel : public BasePanel, public GuideContainer
{
public:
    enum {
        MAX_STAR_NUM = 3,
    };
	CLevelEnterPanel();
	virtual ~CLevelEnterPanel();

	CREATE_FUNC(CLevelEnterPanel);

	bool init();

	virtual CCNode* getGuideNodeByName(const std::string &name);

	void updateUI();

	void open();
	void close();

	void updateInfoCallBack();

	void reset();
protected:

private:
	void initUI();

	void touchEventHandler(CCObject *pSender, TouchEventType type);

	void listViewEventHandler(CCObject *pSender, ListViewEventType type);

    void setScoreStar(unsigned int num);

	void buyManualTip();
	void buyManual();
	void doSweep(unsigned int uNum);
	void doBuyCnt();
	void buyCnt();

	CC_PROPERTY(unsigned int, _levelID, LevelID);		//关卡id
	unsigned int m_uRemainTimes;

    UIButton* m_pCloseBtn;

	UILabel* m_pManualCostTxt; // 体力消耗

	UIImageView *m_pStar1Img;

	UIImageView *m_pStar2Img;

	UIImageView *m_pStar3Img;

	UIButton* m_pFightTogetherBtn; // 好友助战

	UILabel* m_pWinCondTxt; // 胜利条件

	UIListView *m_pDropListView;

	UIListView *m_pSpecDropListView;

	UIButton* m_pSweepBtn; // 扫荡

	UIButton* m_pBatchSweepBtn; // 多次扫荡

	UIButton* m_pEnterBtn; // 进入按钮

	UILabel *m_pNameTxt;

	UIImageView *m_pSweepCardImg;

	UILabel *m_pSweepCardCntTxt;

	UILabel* m_pFullStarConditionTxt;
};

#endif // LevelEnterPanel_h__
