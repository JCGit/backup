#ifndef NihilityResultItem_h__
#define NihilityResultItem_h__

#include "BaseLayout.h"

class IconCell;

class CNihilityResultItem : public BaseLayout
{
public:
	CNihilityResultItem();
	virtual ~CNihilityResultItem();

	CREATE_FUNC(CNihilityResultItem);

	static CNihilityResultItem* create(UILayout *pRefLayout);

	bool init();

	bool initWithLayout(UILayout *pRefLayout);

	void setInfo(const int iType, const int iNum, const int iCfgID);
private:
	void initUI();
	void touchEventHandler(CCObject *pSender, TouchEventType type);

	ImageView* m_pBgImg;
	IconCell* m_pCell;
};

#endif // NihilityResultItem_h__
