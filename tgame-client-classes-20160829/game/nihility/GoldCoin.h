#ifndef GlodCoin_h__
#define GlodCoin_h__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "vlib/QEventDispatcher.h"

USING_NS_CC;
USING_NS_CC_EXT;

using namespace std;

class CGoldCoin : public CCNode, public CQEventDispatcher
{
public:
	static const string GLOD_END_EVENT;		//飞行结束
public:
	CGoldCoin();
	virtual ~CGoldCoin();

	CREATE_FUNC(CGoldCoin);

	void onEnter();
	void onExit();

	bool init();

	void loadRes(string resStr);

	void setTarge(CCNode* pNode);

	void update(float delta);

	void startAct();
protected:
	void handleGlodActComplete(CCArmature * armature, MovementEventType type, const char * name);
private:
	CCArmature* m_pArm;
	CCNode* m_pTarget;

	bool m_bIsFlying; // 是否已开始飞行
};

#endif // GlodCoin_h__
