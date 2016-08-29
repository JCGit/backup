#ifndef ErrorCodeCfgMaster_h__
#define ErrorCodeCfgMaster_h__

#include "CfgMasterBase.h"
#include "ErrorCode.pb.h"
#include <map>

using namespace com::cfg::vo;
using namespace std;

class ErrorCodeCfgMaster: public CCfgMasterBase<ErrorCodeSet>
{
public:
	ErrorCodeCfgMaster() {};
	~ErrorCodeCfgMaster() {};

	const char* getErrDesc(const int iError);
protected:
	// override start

	virtual int ValidateCfg() const;

	virtual int SetCfg();

	virtual void Clear() {
		m_stID2Idx.clear();
	}

	// override end

private:
	// id到下标的映射
	map<unsigned int, int> m_stID2Idx;
};

#define ERROR_CODE_CFG_MASTER CVLib::CVSingleton<ErrorCodeCfgMaster>::Instance()

#endif // ErrorCodeCfgMaster_h__
