#ifndef DropItem_h__
#define DropItem_h__

#include "basic/BaseLayout.h"

class DropItem: public BaseLayout
{
public:
	DropItem();
	virtual ~DropItem();

	static DropItem* create();

	// override start

	virtual bool init();

	// override end

	void setInfo(unsigned int uThingType, unsigned int uThingID, unsigned int uWarType, unsigned int uWarID);

	unsigned int getWarType();

	unsigned int getWarID();

private:
	UIImageView *m_pItemImg;

	UILabel *m_pDescTxt;

	unsigned int m_uWarType;

	unsigned int m_uWarID;

};

#endif // DropItem_h__