#ifndef HeroReviveLayer_h__
#define HeroReviveLayer_h__

#include "BaseLayout.h"
#include "vlib/QEventDispatcher.h"

class CHeroReviveLayer : public BaseLayout, public CQEventDispatcher
{
public:
	static const string HERO_REVIVE_EVENT;
public:
	CHeroReviveLayer();
	virtual ~CHeroReviveLayer();

	CREATE_FUNC(CHeroReviveLayer);

	bool init();

	void startCountdown();
	void continueGame();

	void updateTime(float dt);

protected:
private:
	void onBuyReliveBack();
	void touchEventHandler(CCObject *pSender, TouchEventType type);

	UILayout* m_pUILayout;
	UILayout* m_pLayer;

	ImageView* m_pLabelImg;
	ImageView* m_pIconImg;

	Button* m_pReviveBtn;

	Label* m_pNumTxt;
	Label* m_pTimeTxt;

	unsigned int m_uType;

	unsigned int m_uReliveTime;
	unsigned int m_uReliveCost;

	int timeFlag;
};


#endif // HeroReviveLayer_h__
