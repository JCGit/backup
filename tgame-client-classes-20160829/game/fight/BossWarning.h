#ifndef BossWarning_h__
#define BossWarning_h__

#include "BaseLayout.h"
#include "vlib/QEventDispatcher.h"

class CBossWarning : public BaseLayout, public CQEventDispatcher
{
public:
	static const string BOSS_WARNING_COMPLETE;
public:
	CBossWarning();
	virtual ~CBossWarning();

	CREATE_FUNC(CBossWarning);

	bool init();

	void showWarning();

	void stopWarning();
protected:
private:
	void playSound();
	void endFun();

	ImageView* m_pWarningImg;

	unsigned int m_uEffectID;
};

#endif // BossWarning_h__
