#include "ManualObj.h"
#include "UserObj.h"

ManualObj::ManualObj()
{
}

ManualObj::~ManualObj()
{
}

const vmsg::CSBuyManualInfo& ManualObj::getBuyManualInfo() {
	return m_stBuyManualInfo;
}

void ManualObj::setBuyManualInfo(const vmsg::CSBuyManualInfo &stBuyManualInfo) {
	m_stBuyManualInfo = stBuyManualInfo;

	P_USER_OBJ->getMutableUserInfo()->buyManualCnt = stBuyManualInfo.buycnt();
	P_USER_OBJ->getMutableUserInfo()->maxManualCnt = stBuyManualInfo.maxcnt();
}