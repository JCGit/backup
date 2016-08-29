#ifndef PauseLayer_h__
#define PauseLayer_h__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "vlib/QEventDispatcher.h"

using namespace CVLib;
USING_NS_CC;
USING_NS_CC_EXT;

using namespace gui;

const std::string RESUME_GAME = "RESUME_GAME"; 

class CPauseLayer : public CCLayer, public CQEventDispatcher
{
public:
	CPauseLayer();
	virtual ~CPauseLayer();

	CREATE_FUNC(CPauseLayer);

	bool init();

	void startCountdown();

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
	ImageView* m_pNumImg;

	int timeFlag;
};

#endif // PauseLayer_h__
