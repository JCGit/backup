#ifndef BossWarWaitLayer_h__
#define BossWarWaitLayer_h__

#include "BaseLayout.h"
#include "vlib/QEventDispatcher.h"

class CBossWarWaitLayer : public BaseLayout, public CQEventDispatcher
{
public:
	static const string WAIT_TIME_DONE_EVENT;
public:
	CBossWarWaitLayer();
	virtual ~CBossWarWaitLayer();

	CREATE_FUNC(CBossWarWaitLayer);

	bool init();

	void startCountdown();
	void continueGame();

	void updateTime(float dt);
protected:
private:
	UILayout* m_pUILayout;
	UILayout* m_pLayer;

	Label* m_pTimeTxt;

	int timeFlag;
};

#endif // BossWarWaitLayer_h__
