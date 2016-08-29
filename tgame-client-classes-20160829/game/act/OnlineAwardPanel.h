#ifndef OnlineAwardPanel_h__
#define OnlineAwardPanel_h__

#include "BasePanel.h"

class RewardStateBtn;
class OnlineAwardPanel : public BasePanel,public CCTargetedTouchDelegate
{
public:
	OnlineAwardPanel();
	virtual ~OnlineAwardPanel();

	//CREATE_FUNC(OnlineAwardPanel);

	bool init();

	void open();

	void close();

	void refresh();

	void updateUI();

	virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
private:
	void initUI();

	//void touchEventHandler(CCObject *pSender, TouchEventType type);

	void scheduleTime(float dt);
	void onTouch(const CCPoint& stGlobalPt);
private:
	Label* m_descLab;
	Label* m_getTxt;
	Label* m_getTxtTime;

	Button* m_closeBtn;

	ImageView* m_Item0;
	ImageView* m_Item1;

	UILayout*	m_RewardBtnLayout;
	RewardStateBtn* m_getBtn;

	int m_leftTime;
};

#endif // OnlineAwardPanel_h__
