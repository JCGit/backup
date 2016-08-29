#ifndef EquipCell_h__
#define EquipCell_h__

#include "basic/BaseLayout.h"
#include "CSEquipItemMsg.pb.h"

class EquipCell: public BaseLayout
{
public:
	EquipCell();
	virtual ~EquipCell();
	
	static EquipCell* create(UILayout *pRefLayout);

	bool initWithLayout(UILayout *pRefLayout);

	const vmsg::CSEquipInfo& getInfo();

	void setWeapon(const vmsg::CSWeapon &stWeapon);

	void setInfo(const vmsg::CSEquipInfo &stEquipInfo);

	void clearInfo();

private:
	UIImageView *m_pBgItemImg;

	UIImageView *m_pItemImg;

	vmsg::CSEquipInfo m_stEquipInfo;

	vmsg::CSWeapon m_stWeapon;
};

#endif // EquipCell_h__