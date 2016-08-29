#ifndef LoadingScreen_h__
#define LoadingScreen_h__

#include "cocos2d.h"
#include "cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;

using namespace gui;
using namespace std;

class LoadingScreen: public CCLayer
{
public:
	static LoadingScreen* getInstance();

	static void destroyInstance();

	static LoadingScreen* create();

	// override start
	virtual bool init();

	virtual void onEnter();

	virtual void onExit();

	bool isOpen();

	void open();

	void close();

	// override end

	void setPercentage(float fPercentage);

	void setNoticeWords(const char *pszNoticeWords);

	//为了屏蔽下层的触摸事件
	virtual void registerWithTouchDispatcher(); 

	virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent); 

private:
	LoadingScreen();
	virtual ~LoadingScreen();

	bool m_bIsAllReady;

	float m_fPercentage;

	UILayout* m_pUILayout;

	UIImageView *m_pBgImg;

	ImageView* m_pLoadingBg;
	LoadingBar* m_pLoadBar;
	UILabel* m_pLoadTxt;

    CCSprite* m_pAniSp;
    CCAnimate* m_pAni;

	CCArmature *m_pLoadingArmature;

	UILabel *m_pTipTxt;

	bool m_bIsOpen;
};

#endif // LoadingScreen_h__
