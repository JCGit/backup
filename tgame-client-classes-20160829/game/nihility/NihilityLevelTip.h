#ifndef NihilityLevelTip_h__
#define NihilityLevelTip_h__

#include "BaseLayout.h"
#include "ImageNumber.h"

class CNihilityLevelTip : public BaseLayout
{
public:
	CNihilityLevelTip();
	virtual ~CNihilityLevelTip();

	CREATE_FUNC(CNihilityLevelTip);

	bool init();

	void showLevelTip(unsigned int level);
protected:
private:
	ImageNumber* m_pLevelTxt;
	ImageView* m_pMissImg;

	void showEnd();

	unsigned int m_uLevel;
};

#endif // NihilityLevelTip_h__
