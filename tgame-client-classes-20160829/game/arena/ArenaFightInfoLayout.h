#ifndef ArenaFightInfoLayout_h__
#define ArenaFightInfoLayout_h__

#include "BaseLayout.h"

class CArenaFightInfoLayout : public BaseLayout
{
public:
	CArenaFightInfoLayout();
	virtual ~CArenaFightInfoLayout();

	CREATE_FUNC(CArenaFightInfoLayout);

	bool init();

	void setDefNum(unsigned int uNum, unsigned int uTotal);

	void setTime(unsigned int uTime);

	void updateTime(float dt);
protected:
private:
	ImageView* m_pInfoImg;
	ImageView* m_pOpImg;
	Label* m_pTimeTxt;
	Label* m_pDefNumTxt;

	unsigned int m_uTime;
};

#endif // ArenaFightInfoLayout_h__
