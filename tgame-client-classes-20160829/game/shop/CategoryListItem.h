#ifndef CategoryListItem_h__
#define CategoryListItem_h__

#include "basic/BaseLayout.h"

class CategoryListItem: public BaseLayout
{
public:
	CategoryListItem();
	virtual ~CategoryListItem();
	
	static CategoryListItem* create(UILayout *pRefLayout);

	bool initWithLayout(UILayout *pRefLayout);

	bool isSelected();

	void selected(bool bSelected);

	void setCategory(const std::string &category);

	bool getHotFlag();

	void setHotFlag(bool bFlag);

	unsigned int getType();

	void setType(unsigned int uType);

	unsigned int getDetailType();

	void setDetailType(unsigned int uDetailType);

private:
	UIImageView *m_pBgSelectedImg;
    UIImageView* m_pBgNormalImg;

	UILabel *m_pCategoryTxt;

	bool m_bSelected;

	bool m_bHotFlag;

	unsigned int m_uType;

	unsigned int m_uDetailType;

};

#endif // CategoryListItem_h__