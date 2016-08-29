#ifndef ReviveLayer_h__
#define ReviveLayer_h__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "vlib/QEventDispatcher.h"
using namespace CVLib;

USING_NS_CC;
USING_NS_CC_EXT;

using namespace gui;

const std::string REVIVE_DONE = "REVIVE_DONE"; 
/*
复活倒计时
*/
class CReviveLayer : public CCLayer, public CQEventDispatcher
{
public:
	CReviveLayer();
	~CReviveLayer();

	CREATE_FUNC(CReviveLayer);

	bool init();

    void startCountdown();
    
    void showWaiting();
    void doWaitingDone();

	void continueGame();

	void updateTime(float dt);

	//为了屏蔽下层的触摸事件
	virtual void registerWithTouchDispatcher(); 

	virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent); 
	virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent); 
	virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent); 
protected:
private:
	CC_SYNTHESIZE(CCSet*, m_pBeforeTargetSets, BeforeTargetSets);

	UILayout* m_pLayer;
	UILabel* m_pTxt;

	int timeFlag;
};

#endif // ReviveLayer_h__
