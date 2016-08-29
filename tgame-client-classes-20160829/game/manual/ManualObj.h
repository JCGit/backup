#ifndef ManualObj_h__
#define ManualObj_h__

#include "CSPlayerMsg.pb.h"

class ManualObj
{
public:
	ManualObj();
	~ManualObj();

	const vmsg::CSBuyManualInfo& getBuyManualInfo();

	void setBuyManualInfo(const vmsg::CSBuyManualInfo &stBuyManualInfo);

private:
	vmsg::CSBuyManualInfo m_stBuyManualInfo;

};

#endif // ManualObj_h__
