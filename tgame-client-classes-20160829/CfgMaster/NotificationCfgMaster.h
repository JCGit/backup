#ifndef NotificationCfgMaster_h__
#define NotificationCfgMaster_h__

#include "CfgMasterBase.h"
#include "NotificationCfg.pb.h"

#include <map>

using namespace com::cfg::vo;
using namespace std;

class NotificationCfgMaster: public CCfgMasterBase<NotificationCfgSet>
{
public:
	NotificationCfgMaster() {};
	virtual ~NotificationCfgMaster() {};

	const NotificationCfg* getNotificationCfg(unsigned int uEntryID, unsigned int uTag);
	
protected:
	virtual int ValidateCfg() const;

	virtual int SetCfg();

	virtual void Clear() {
		m_stKey2Idx.clear();
	}

private:
	// key到下标的映射
	map<pair<unsigned int, unsigned int>, int> m_stKey2Idx;

};

#define NOTIFICATION_CFG_MASTER CVLib::CVSingleton<NotificationCfgMaster>::Instance()

#endif // NotificationCfgMaster_h__