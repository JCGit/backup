#ifndef BagObj_h__
#define BagObj_h__

#include "CSBagMsg.pb.h"

class BagObj
{
public:
	BagObj();
	~BagObj();

	const vmsg::CSBagInfo& getBagInfo();

	vmsg::CSBagInfo& mutableBagInfo();

	void setBagInfo(const vmsg::CSBagInfo &stBagInfo);

	const vmsg::CSBagCellInfo& getSelectedBagCellInfo();

	void setSelectedBagCellInfo(const vmsg::CSBagCellInfo &stBagCellInfo);

private:
	vmsg::CSBagInfo m_stBagInfo;

	vmsg::CSBagCellInfo m_stSelectedBagCellInfo;

};

#endif // BagObj_h__
