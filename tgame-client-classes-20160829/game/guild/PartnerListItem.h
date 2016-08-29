#ifndef PartnerListItem_h__
#define PartnerListItem_h__

#include "basic/BaseLayout.h"

class PartnerListItem: public BaseLayout
{
public:
	PartnerListItem();
	virtual ~PartnerListItem();
	
	static PartnerListItem* create();

	// override start

	virtual bool init();

	// override end

	void updateUI();

	bool isSelected();

	void selected(bool bSelected);

private:
	UIImageView *m_pBgSelectedImg;

	UILabel *m_pNameTxt;

	UILabel *m_pLevelTxt;

	bool m_bSelected;
};

#endif // PartnerListItem_h__