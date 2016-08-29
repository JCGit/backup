#ifndef SvrNotifyMaster_h__
#define SvrNotifyMaster_h__

#include "CfgMasterBase.h"
#include "ServerNotifyCfg.pb.h"
#include <map>

using namespace com::cfg::vo;
using namespace std;

class SvrNotifyMaster: public CCfgMasterBase<ServerNotifyCfgSet>
{
public:
	SvrNotifyMaster() {

    }
	~SvrNotifyMaster() {

    }

    const ServerNotifyCfg* getNotify(const int iNotifyID);

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

#define SVR_NOTIFY_CFG_MASTER CVLib::CVSingleton<SvrNotifyMaster>::Instance()

#endif // SvrNotifyMaster_h__
