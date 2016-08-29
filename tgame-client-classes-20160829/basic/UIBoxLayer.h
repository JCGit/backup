#ifndef UIBoxLayer_h__
#define UIBoxLayer_h__

#include "cocos2d.h"
#include "cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace ui;

class BasePanel; // box
class MoneyTopBar;

typedef BasePanel Box;

class UIBoxLayer: public UILayer
{
public:
	UIBoxLayer();
	virtual ~UIBoxLayer();

	static UIBoxLayer* create(UIButton *pBackBtn);

	bool init(UIButton *pBackBtn);

	void setBackBtn(UIButton *pBackBtn);

	void addBox(Box *pBox);

	void removeBox(Box *pBox);

	void removeAllBoxes();

	bool isBgImgVisible();

	void setBgImgVisible(bool visible);

	bool isBgEnabled();

	void setBgEnabled(bool enabled);

	bool isBackBtnEnabled();

	void setBackBtnEnabled(bool enabled);

	UIButton* getBackBtn();

    //pBox是否处于最上层打开的面板
    bool isBoxOnTop(Box *pBox);

    void setMoneyBarVisible(bool bIsShow);
    void updateMoneyBar();
private:
	void touchEventHandler(CCObject *pSender, TouchEventType type);

private:
	UIImageView *m_pBgImg;

	UILayout *m_pBgLayout;

	UILayout *m_pContainerLayout;

    MoneyTopBar *m_pMoneyTopBar;

	UIButton *m_pBackBtn;

	std::vector<Box*> m_stBoxes;

	bool m_bInitialized;

};

#endif //UIBoxLayer_h__
