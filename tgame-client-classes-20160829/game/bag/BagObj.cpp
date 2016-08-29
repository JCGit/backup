#include "BagObj.h"

BagObj::BagObj() 
{
}

BagObj::~BagObj()
{
}

const vmsg::CSBagInfo& BagObj::getBagInfo() {
	return m_stBagInfo;
}

vmsg::CSBagInfo& BagObj::mutableBagInfo() {
	return m_stBagInfo;
}

void BagObj::setBagInfo(const vmsg::CSBagInfo &stBagInfo) {
	m_stBagInfo = stBagInfo;
}

const vmsg::CSBagCellInfo& BagObj::getSelectedBagCellInfo() {
	return m_stSelectedBagCellInfo;
}

void BagObj::setSelectedBagCellInfo(const vmsg::CSBagCellInfo &stBagCellInfo) {
	m_stSelectedBagCellInfo = stBagCellInfo;
}
