#ifndef FallingItem_h__
#define FallingItem_h__

#include "cocos2d.h"

USING_NS_CC;

class FallingItem : public cocos2d::CCSprite
{
public:
	FallingItem();
	virtual ~FallingItem();

	static FallingItem* fall(unsigned int itemID, cocos2d::CCNode* pHero, cocos2d::CCPoint pos);

	static FallingItem* fall(unsigned int itemID, cocos2d::CCNode* pHero, cocos2d::CCPoint pos, cocos2d::SEL_CallFuncO selector);

	CREATE_FUNC(FallingItem);

	virtual bool init(void);

	virtual void update(float delta);

	void setHero(cocos2d::CCNode* pHero);

	const char* getName();
	void setName(const char* name);

	bool checkInScope(const cocos2d::CCPoint &itemPos, const cocos2d::CCPoint &heroPos); // 检查是否可拾取

	void onPickup(cocos2d::CCObject* pListener, cocos2d::SEL_CallFuncO pHandler); // 设置拾取物品后的回调

private:
	void initUI(unsigned int itemID, CCPoint &pos);

	bool checkInScopeByRect(const cocos2d::CCPoint &itemPos, const cocos2d::CCPoint &heroPos); // 矩形范围检测

	bool checkInScopeByCircle(const cocos2d::CCPoint &itemPos, const cocos2d::CCPoint &heroPos); // 矢量长度检测

	void flyByFrame(const cocos2d::CCPoint &destPos);

private:

	const char* m_strName;

	// pickup
	float m_fScopeYScale; // 拾取范围纵向缩放比例
	float m_fScopeWidth; // 拾取范围横向直径
	float m_fSpeed; // 物品移动速度，像素每帧
	bool m_bIsFlying; // 是否已开始飞行

	cocos2d::CCObject* m_pListener;
	cocos2d::SEL_CallFuncO m_pfnHandler;
	
	cocos2d::CCNode* m_pHero;
};

#endif // FallingItem_h__
