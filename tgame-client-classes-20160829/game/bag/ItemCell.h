#ifndef ItemCell_h__
#define ItemCell_h__

#include "basic/BaseLayout.h"
#include "ItemInfo.h"

class ItemCell: public BaseLayout
{
public:
	ItemCell();
	virtual ~ItemCell();

	static ItemCell* create();

	static ItemCell* create(UILayout *pRefLayout);

	virtual bool initWithLayout(UILayout *pRefLayout);

	// override start

	virtual bool init();

	virtual std::string getDescription() const;

	// override end

	UIImageView* getDefaultImg();

	UIImageView* getItemImg();

	const vmsg::CSItemInfo& getItemInfo();

	void updateItemInfo(const vmsg::CSItemInfo& stItemInfo);

	const vmsg::CSEquipInfo& getEquipInfo();

	void updateEquipInfo(const vmsg::CSEquipInfo& stEquipInfo);

	void clearItem();

protected:
	UIImageView *m_pDefaultImg;

	UIImageView *m_pItemImg;

	vmsg::CSItemInfo m_stItemInfo;

	vmsg::CSEquipInfo m_stEquipInfo;
};

#endif // ItemCell_h__
