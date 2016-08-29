#ifndef LoadingLayer_h__
#define LoadingLayer_h__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "vlib/QEventDispatcher.h"
using namespace CVLib;
USING_NS_CC;
USING_NS_CC_EXT;

using namespace gui;

const std::string TIME_UP = "TIME_UP"; 
/*
转菊花
*/
class CLoadingLayer : public CCLayer, public CQEventDispatcher
{
public:
	CLoadingLayer();
	~CLoadingLayer();

	CREATE_FUNC(CLoadingLayer);

	bool init();


    //////////////////////////////
    void showLoading();
	void showLoading(CCObject* target, SEL_CallFunc callbackFun);
    void doLoadingDone();
    //////////////////////////////

	bool getIsInLoading() const{
		return m_bIsInLoading;
	}

	//为了屏蔽下层的触摸事件
	virtual void registerWithTouchDispatcher(); 
	virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent); 
	virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent); 
	virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent); 
protected:
    void startCountdown();
    void continueGame();
    void updateTime(float dt);
private:
	bool m_bIsInLoading;
	int timeFlag;

    CCAnimate *m_stAnimate;
	CCSprite* m_stBgSprite;
    CCSprite* m_stSpAnimate;

	CCObject*    m_pTarget;
	SEL_CallFunc m_pEventSelector;
};

#endif // LoadingLayer_h__
