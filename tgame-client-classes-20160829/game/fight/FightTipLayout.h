#ifndef FightTipLayout_h__
#define FightTipLayout_h__

#include "BaseLayout.h"

class CFightTipLayout : public BaseLayout
{
public:
	CFightTipLayout();
	virtual ~CFightTipLayout();

	CREATE_FUNC(CFightTipLayout);

	bool init();

	void startAct();
	void stopAct();
protected:
private:
	ImageView* m_pGoImg;
	ImageView* m_pArrowImg;

	CC_SYNTHESIZE(bool, _isActing, IsActing);

	void scaleFun();
	void moveFun();
};

#endif // FightTipLayout_h__
