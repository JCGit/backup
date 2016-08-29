#include "NotificationControl.h"

#include "Macros.pb.h"
#include "ModuleID.pb.h"

#include "CfgMaster/NotificationCfgMaster.h"
#include "CfgMaster/GlobalCfgMaster.h"
#include "CfgMaster/EquipmentCfgMaster.h"
#include "CfgMaster/TaskCfgMaster.h"
#include "CfgMaster/AttrStrengthenCfgMaster.h"
#include "CfgMaster/EvolutionSkillCfgMaster.h"
#include "CfgMaster/MilestoneCfgMaster.h"
#include "CfgMaster/WeaponExpItemCfgMaster.h"
#include "CfgMaster/EquipExpItemCfgMaster.h"
#include "CfgMaster/ItemCfgMaster.h"
#include "CfgMaster/WeaponCfgMaster.h"
#include "CfgMaster/WeaponStarStepCfgMaster.h"
#include "CfgMaster/WeaponRefitCfgMaster.h"
#include "CfgMaster/EquipStarStepCfgMaster.h"
#include "CfgMaster/EquipRefitCfgMaster.h"

#include "ClientSvrMsg.pb.h"

#include "basic/BaseScene.h"

#include "game/UserObj.h"
#include "game/bag/BagControl.h"
#include "game/task/TaskControl.h"
#include "game/attrstrengthen/AttrStrengthenControl.h"
#include "game/evolution/EvolutionControl.h"
#include "game/firearms/FireArmsObj.h"
#include "game/city/CityControl.h"
#include "game/weaponlib/WeaponLibControl.h"
#include "game/equiplib/EquipLibControl.h"
#include "game/nihility/NihilityControl.h"
#include "game/city/EntryControl.h"
#include "game/arena/ArenaControl.h"
#include "game/friend/FriendControl.h"
#include "game/mail/MailControl.h"

#define LOCK_INTERVAL 60 // seconds

NotificationControl::NotificationControl()
:m_bInitialized(false)
,m_pNotificationPanel(NULL)
,m_pNotificationLayout(NULL)
,m_uSecTime(0)
{
}

NotificationControl::~NotificationControl()
{
	finalize();
}

void NotificationControl::init() {
	if(!m_bInitialized) {
		m_pNotificationPanel = NotificationPanel::create();
		CC_SAFE_RETAIN(m_pNotificationPanel);

		m_pNotificationLayout = NotificationLayout::create();
		CC_SAFE_RETAIN(m_pNotificationLayout);

		m_bInitialized = true;
	}
}

void NotificationControl::finalize() {
	if(m_bInitialized) {
		CC_SAFE_RELEASE_NULL(m_pNotificationPanel);
		CC_SAFE_RELEASE_NULL(m_pNotificationLayout);

		m_bInitialized = false;
	}
}

void NotificationControl::secTimer(unsigned int seconds) {
	m_uSecTime = seconds;

	// 若已达到锁定时限，则解锁
	map<pair<unsigned int, unsigned int>, unsigned int>::iterator iter = m_stLockedMap.begin();
    map<pair<unsigned int, unsigned int>, unsigned int> stTmpMap;
	while(iter != m_stLockedMap.end()) {
		if((m_uSecTime - iter->second) <= LOCK_INTERVAL) {
			//iter = m_stLockedMap.erase(iter);
			stTmpMap[iter->first] = iter->second;
		}

		iter++;
	}
    m_stLockedMap = stTmpMap;
}

const Notification* NotificationControl::getFrontNotification(REMIND_WAY way) {
	const Notification *pRet = NULL;

	list<Notification>::const_iterator iter = m_stPushLst.begin();

	while(iter != m_stPushLst.end()) {
		pair<unsigned int, unsigned int> key = make_pair(iter->entryid(), iter->tag());

		if(iter->remindway() == way && m_stLockedMap.find(key) == m_stLockedMap.end()) {
			pRet = &(*iter);
			break;
		}

		iter++;
	}

	return pRet;
}

void NotificationControl::pushNotification(unsigned int uEntryID, unsigned int uTag, unsigned int uBadgeNum) {
	if(!P_ENTRY_CTRL->isEntryOpen(uEntryID)) { // 没开放
		return;
	}
	
	Notification note;
	note.set_entryid(uEntryID);
	note.set_tag(uTag);
	const NotificationCfg *pNotificationCfg = NOTIFICATION_CFG_MASTER->getNotificationCfg(note.entryid(), note.tag());

	if(pNotificationCfg != NULL) {
		note.set_remindway(pNotificationCfg->remindway());

		if(uBadgeNum > 0) {
			note.set_badgenum(uBadgeNum);
			note.set_words(pNotificationCfg->words());
		}

		pushNotification(note);
	}
}

void NotificationControl::pushNotification(const Notification &note) {
	bool bExist = false;
	list<Notification>::iterator iter = m_stPushLst.begin();

	while(iter != m_stPushLst.end()) {
		if(iter->entryid() == note.entryid() && iter->tag() == note.tag()) {
			*iter = note;
			bExist = true;
			break;
		}

		iter++;
	}

	if(!bExist) {
		m_stPushLst.push_back(note);
	}

	if(note.ByteSize() > 0) {
		unsigned int uRemindWay = note.remindway();
		vector<unsigned int> ways;
		int i = 0;

		while(uRemindWay >> i > 0) {
			unsigned int way = uRemindWay & (1 << i);

			if(way > 0) {
				ways.push_back(way);
			}

			i++;
		}

		for(unsigned int i = 0; i < ways.size(); i++) {
			unsigned int way = ways[i];

			if(way == REMIND_BY_BADGE) {
				if(!note.has_badgenum() || note.badgenum() == 0) {
					removeNotification(note.entryid(), note.tag());
				}

				CityScreenView *pCityScreenView = P_CITY_CTRL->getCityScreenView();

				if(pCityScreenView != NULL) {
					unsigned int uEntryID = note.entryid();
					unsigned int uBadgeNum = note.has_badgenum() ? note.badgenum() : 0;
					pCityScreenView->badgeEntryBtn(uEntryID, uBadgeNum);
					unsigned int uParentEntryID = uEntryID >> 8;

					if(uParentEntryID > 0) {
						pCityScreenView->badgeEntryBtn(uParentEntryID, uBadgeNum);
					}
				}
			} else if(way == REMIND_BY_ALERT) {
				if(!note.has_words() || note.words().empty()) {
					removeNotification(note.entryid(), note.tag());
				}

				if(m_pNotificationLayout != NULL) {
					const Notification *pNotification = getFrontNotification(REMIND_BY_ALERT);
					m_pNotificationLayout->setEnabled(pNotification != NULL);
				}
			}
		}
	}
}

void NotificationControl::lockNotification(unsigned int uEntryID, unsigned int uTag) {
	pair<unsigned int, unsigned int> key = make_pair(uEntryID, uTag);

	if(m_stLockedMap.find(key) == m_stLockedMap.end()) {
		m_stLockedMap.insert(make_pair(key, m_uSecTime));
	}

	removeNotification(uEntryID, uTag);
}

void NotificationControl::removeNotification(unsigned int uEntryID, unsigned int uTag) {
	list<Notification>::iterator iter = m_stPushLst.begin();

	while(iter != m_stPushLst.end()) {
		if(iter->entryid() == uEntryID && iter->tag() == uTag) {
			iter = m_stPushLst.erase(iter);
			break;
		} else {
			iter++;
		}
	}

	if(m_pNotificationLayout != NULL) {
		const Notification *pNotification = getFrontNotification(REMIND_BY_ALERT);
		m_pNotificationLayout->setEnabled(pNotification != NULL);
	}
}

void NotificationControl::showNotification() {
	if(m_pNotificationPanel != NULL) {
		m_pNotificationPanel->open();
	}
}

NotificationLayout* NotificationControl::getNotificationLayout() {
	return m_pNotificationLayout;
}
