#ifndef GuideView_h__
#define GuideView_h__

#include "cocos2d.h"
#include "cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace cocos2d::ui;

class GuideView: public UILayout
{
public:
	enum MinViewSize {
		MIN_WIDTH = 20, // 最小宽度
		MIN_HEIGHT = 20, // 最小高度
	};
	
public:
	GuideView();
	virtual ~GuideView();
	
	static GuideView* create();

	// override start

	virtual bool init();

	virtual void onEnter();
	virtual void onExit();

	virtual bool hitTest(const CCPoint &pt);

	// override end

	void initUI();

	bool isScreenEnabled();

	void setScreenEnabled(bool enabled);

	bool isForced();

	void forced(bool bForced);

	bool isHandVisible();

	void setHandVisible(bool bHandVisible);

	bool isBorderVisible();

	void setBorderVisible(bool bBorderVisible);

	void show(const CCRect &rect, const std::string &shape);

	void hide();

private:
	CCClippingNode *m_pClipNode;

	CCLayerColor *m_pDarkLayer;

	CCDrawNode *m_pLightNode;

	CCScale9Sprite *m_pBorder;

	CCArmature *m_pHitArmture;

	CCAction *m_pBorderAction;

	CCLabelTTF *m_pWordsTxt;

	bool m_bForced;

	bool m_bHandVisible;

	bool m_bBorderVisible;

	std::string m_stWords;

	bool m_bRectShape;

};

#endif // GuideView_h__