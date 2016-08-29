#ifndef FightGuideHud_h__
#define FightGuideHud_h__

#include "BaseLayout.h"
#include "fight/FireButton.h"

enum GUIDE_HUD_TYPE{
	UP_TYPE,			//上方
	DOWN_TYPE,			//下方
};

class CFightGuideHud : public BaseLayout
{
public:
	CFightGuideHud();
	virtual ~CFightGuideHud();

	CREATE_FUNC(CFightGuideHud);

	bool init();

	//bool hitTest(const CCPoint &pt);

	void updatePos(float delta);

	void initUI();

	void setActive(bool isActive);

	bool getActive(){
		return m_bIsActive;
	}

	void setDirect(int direct);
	int getDirect(){
		return m_iDirection;
	}

	void endTouch();
	void resetPos();

	void startMove();

	bool checkInRect(CCPoint stPos);

	bool getIsMove(){
		return m_bIsMove;
	}
protected:
private:
	void touchEventHandler(CCObject *pSender, TouchEventType type);

	FireButton* m_pClickImg;
	//ImageView* m_pClickImg;
	ImageView* m_pBgImg;
	ImageView* m_pHudImg;

	CCPoint m_stPos;

	bool m_bIsActive;
	bool m_bIsMove;

	int m_iDirection;
};

#endif // FightGuideHud_h__
