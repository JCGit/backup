#ifndef AttrCell_h__
#define AttrCell_h__

#include "basic/BaseLayout.h"

class AttrCell: public BaseLayout
{
public:
	AttrCell();
	virtual ~AttrCell();
	
	static AttrCell* create(UILayout *pRefLayout);

	virtual bool initWithLayout(UILayout *pRefLayout);

	void setType(unsigned int uAttrType, unsigned int uMagicType);

	unsigned int getAttrType();

	unsigned int getMagicType();

	unsigned int getLevel();

	void updateUI(unsigned int uLevel);

	bool isSelected();

	void selected(bool bSelected);

private:
	UIImageView *m_pAttrIconImg;

	unsigned int m_uAttrType;

	unsigned int m_uMagicType;

	unsigned int m_uLevel;

	bool m_bSelected;

};

#endif // AttrCell_h__