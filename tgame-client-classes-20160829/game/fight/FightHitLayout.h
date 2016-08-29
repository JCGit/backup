#ifndef FightHitLayout_h__
#define FightHitLayout_h__

#include "BaseLayout.h"
#include "ImageNumber.h"
#include "vlib/QEventDispatcher.h"

class CFightHitLayout : public BaseLayout, public CQEventDispatcher
{
public:
	static const unsigned int HIT_LAST_TIME;
	static const string HIT_OUT_TIME_EVENT;
public:
	CFightHitLayout();
	virtual ~CFightHitLayout();

	CREATE_FUNC(CFightHitLayout);

	bool init();

	void onEnter();
	void onExit();

	void update(float delta);

	void reset();

	void setHitNum(unsigned int uNum);

	unsigned int getPreHitNum(){
		return m_uPreHitNum;
	}
protected:

private:
	ImageNumber* m_pHitNumImg;

	float m_fObjectTime;

	bool m_bIsOutTime;

	unsigned int m_uPreHitNum;
	unsigned int m_uHitNum;
};

#endif // FightHitLayout_h__
