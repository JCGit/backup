#include "NotificationCfgMaster.h"

#include <set>

const NotificationCfg* NotificationCfgMaster::getNotificationCfg(unsigned int uEntryID, unsigned int uTag) {
	pair<unsigned int, unsigned int> key = make_pair(uEntryID, uTag);
	map<pair<unsigned int, unsigned int>, int>::const_iterator iter = m_stKey2Idx.find(key);

	if(iter == m_stKey2Idx.end())
	{
		CV_ERROR(("%s. Not found the cfg! EntryID=%u Tag=%u", __PRETTY_FUNCTION__, uEntryID, uTag));

		return NULL; // 找不到相应配置
	}

	return m_stCfg.mutable_notificationcfg(iter->second);
}

int NotificationCfgMaster::ValidateCfg() const {
	int iRet = 0;

	set<pair<unsigned int, unsigned int> > stCfgIDSet;

	for(int i = 0; i < m_stCfg.notificationcfg_size(); i++)
	{
		const NotificationCfg &stNotificationCfg = m_stCfg.notificationcfg(i);
		pair<unsigned int, unsigned int> key = make_pair(stNotificationCfg.entryid(), stNotificationCfg.tag());

		if(stCfgIDSet.insert(key).second == false)
		{
			CV_ERROR(("%s, The key is already exist! EntryID=%u Tag=%u", __PRETTY_FUNCTION__, stNotificationCfg.entryid(), stNotificationCfg.tag()));

			return -1; // id重复
		}
	}

	return iRet;
}

int NotificationCfgMaster::SetCfg() {
	int iRet = 0;

	for(int i = 0; i < m_stCfg.notificationcfg_size(); i++)
	{
		const NotificationCfg &stNotificationCfg = m_stCfg.notificationcfg(i);
		pair<unsigned int, unsigned int> key = make_pair(stNotificationCfg.entryid(), stNotificationCfg.tag());

		if(m_stKey2Idx.insert(make_pair(key, i)).second == false)
		{
			CV_ERROR(("%s, The key is already exist! EntryID=%u Tag=%u", __PRETTY_FUNCTION__, stNotificationCfg.entryid(), stNotificationCfg.tag()));

			return -1; // id重复
		}
	}

	return iRet;
}
