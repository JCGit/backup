#ifndef FightHeroReliveLayer_h__
#define FightHeroReliveLayer_h__

#include "BaseLayout.h"
#include "vlib/QEventDispatcher.h"

class ImageNumber;

class CFightHeroReliveLayer : public BaseLayout, public CQEventDispatcher
{
public:
	static const int HERO_RELIVE_COUNT_DOWN;
	static const string FIGHT_HERO_RELIVE_EVENT;
	static const string FIGHT_HERO_CANCEL_RELIVE_EVENT;

public:
	CFightHeroReliveLayer();
	virtual ~CFightHeroReliveLayer();

	CREATE_FUNC(CFightHeroReliveLayer);

	bool init();

	void startCountDown();
	void gameFailed();

	void updateTime(float dt);

	void doReliveDone();			//复活成功
protected:
private:
	void touchEventHandler(CCObject *pSender, TouchEventType type);

	void doRelive();
	void reliveFun();

	UILayout* m_pUILayout;
	UILayout* m_pLayer;

	ImageView* m_pReliveIcon;
	ImageView* m_pGlodIcon;

	Button* m_pReviveBtn;
	Button* m_pCancelBtn;

	Label* m_pGoldNumTxt;
	Label* m_pReliveNumTxt;
	Label* m_pDesTxt;

	ImageNumber* m_pTimeTxt;

	bool m_bIsCostGold;				//是否消耗星钻
	unsigned int m_uReliveCost;

	int timeFlag;
};


#endif // FightHeroReliveLayer_h__
