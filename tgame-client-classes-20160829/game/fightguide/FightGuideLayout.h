#ifndef FightGuideLayout_h__
#define FightGuideLayout_h__

#include "cocos2d.h"
#include "cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;

using namespace gui;
using namespace std;

class CFightGuideLayout : public UILayout
{
public:
	CFightGuideLayout();
	virtual ~CFightGuideLayout();

	CREATE_FUNC(CFightGuideLayout);

	virtual bool init();

	virtual void onEnter();
	virtual void onExit();

	virtual bool hitTest(const CCPoint &pt);

	void initUI();

	void show(const CCRect &rect);

	void clearAllStencil();

	void showShootTip(CCPoint stPos);
	void showEff(CCPoint stPos);

	void showFireEff(CCPoint stPos);
protected:
private:
	CCClippingNode *m_pClipNode;
	CCLayerColor *m_pDarkLayer;
	CCNode *m_pLightStencil;
	ImageView* m_pTipImg;

	CCArray* m_stLightNodes;
	CCArray* m_stBorders;
};

#endif // FightGuideLayout_h__
