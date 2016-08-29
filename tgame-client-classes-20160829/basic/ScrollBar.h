#ifndef ScrollBar_h__
#define ScrollBar_h__

#include "cocos2d.h"
#include "cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;

enum CSclBarDirection{
	DIR_NODIR = 0,
	DIR_VERTICAL,
	DIR_HORIZENTAL,
};

class CScrollBar : public CCSprite, public CCTouchDelegate
{
public:
	CScrollBar();
	virtual ~CScrollBar();

	void onEnter();
	void onExit();

	/**
	* 因为九宫图不能缩小到比实际图片要小，所以传入的图片的实际大小要足够小，否则slider的大小会有问题
	*/
	static CScrollBar * create(CCScale9Sprite* bar_bg, CCScale9Sprite* bar_slider, CCTableView* tableView, CSclBarDirection dir);
	static CScrollBar * create(const char* bar_bgFile, const char* bar_sliderFile, CCTableView* tableView, CSclBarDirection dir);
	bool myInit(CCScale9Sprite* bar_bg, CCScale9Sprite* bar_slider, CCTableView* tableView, CSclBarDirection dir);

	virtual void registerWithTouchDispatcher(); 

protected:
	virtual bool onTouchBegan(CCTouch* touch, CCEvent* pEvent);
	virtual void onTouchMoved(CCTouch* pTouch, CCEvent* pEvent);
	virtual void onTouchEnded(CCTouch* pTouch, CCEvent* pEvent);

	virtual void update(float dt);
	/**
	* 动态改变slider的大小
	*/
	void updateSlider();

private:
	CCTableView* m_pTarget;
	CCScale9Sprite * m_pBg;
	CCScale9Sprite * m_pSlider;
	CSclBarDirection m_direction;
	CCSize m_preContentSize;
	CCSize m_viewSize;
	bool m_sliderTouched;
	CCPoint m_firstTouch;
	CCPoint m_sliderCurPos;
	CCPoint m_targetCurPos;

};


#endif // ScrollBar_h__
