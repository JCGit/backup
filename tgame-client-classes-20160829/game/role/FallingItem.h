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

	bool checkInScope(const cocos2d::CCPoint &itemPos, const cocos2d::CCPoint &heroPos); // ����Ƿ��ʰȡ

	void onPickup(cocos2d::CCObject* pListener, cocos2d::SEL_CallFuncO pHandler); // ����ʰȡ��Ʒ��Ļص�

private:
	void initUI(unsigned int itemID, CCPoint &pos);

	bool checkInScopeByRect(const cocos2d::CCPoint &itemPos, const cocos2d::CCPoint &heroPos); // ���η�Χ���

	bool checkInScopeByCircle(const cocos2d::CCPoint &itemPos, const cocos2d::CCPoint &heroPos); // ʸ�����ȼ��

	void flyByFrame(const cocos2d::CCPoint &destPos);

private:

	const char* m_strName;

	// pickup
	float m_fScopeYScale; // ʰȡ��Χ�������ű���
	float m_fScopeWidth; // ʰȡ��Χ����ֱ��
	float m_fSpeed; // ��Ʒ�ƶ��ٶȣ�����ÿ֡
	bool m_bIsFlying; // �Ƿ��ѿ�ʼ����

	cocos2d::CCObject* m_pListener;
	cocos2d::SEL_CallFuncO m_pfnHandler;
	
	cocos2d::CCNode* m_pHero;
};

#endif // FallingItem_h__
