#ifndef GameLayer_h__
#define GameLayer_h__

#include "cocos2d.h"

USING_NS_CC;

typedef void (CCObject::*SEL_HitEvent)(CCObject*, CCPoint);
#define hiteventselector(_SELECTOR) (SEL_HitEvent)(&_SELECTOR)

class GameLayer: public CCLayer
{
public:
	GameLayer();
	virtual ~GameLayer();

	static GameLayer* create();

	// override start

	virtual bool init();
	virtual void onEnter();
	virtual void onExit();

	virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);

	// override end

	void addHitEventListener(CCObject *pListener, SEL_HitEvent pfnSelector);

private:
	CCObject *m_pHitEventListener;

	SEL_HitEvent m_pfnHitEventSelector;

};

#endif // GameLayer_h__