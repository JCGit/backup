#ifndef NotificationControl_h__
#define NotificationControl_h__

#include "vlib/base/CVSingleton.h"

#include "CfgMaster/NotificationCfgMaster.h"

#include "game/city/EntryControl.h"

#include "NotificationPanel.h"
#include "NotificationLayout.h"

typedef enum RemindWay {
	REMIND_BY_BADGE = 0x1, // 左上角标记
	REMIND_BY_ALERT = 0x2, // 主界面提醒
} REMIND_WAY;

enum NotifyTag {
	// 枪械库
	NOTIFY_TAG_WEAPON_LVUP = 1,
	NOTIFY_TAG_WEAPON_ADVANCE = 2,
	// 装备养成
	NOTIFY_TAG_EQUIP_LVUP = 1,
	NOTIFY_TAG_EQUIP_ADVANCE = 2,
	// 成就
	NOTIFY_TAG_MILESTONE_FINISHED = 1,
	// 进化
	NOTIFY_TAG_EVOLUTION = 1,
	// 机体强化
	NOTIFY_TAG_ATTR_LVUP = 1,
	// 等级不够
	NOTIFY_TAG_LEVEL_UNREACHED = 1,
	// 体力不足
	NOTIFY_TAG_MANUAL_NOT_ENOUGH = 1,
	// 背包装备
	NOTIFY_TAG_ADVANCED_EQUIP_IN_BAG = 1,
	// 竞技场
	NOTIFY_TAG_ARENA_NEW_RECORD = 1,
	NOTIFY_TAG_ARENA_FREE_CHALLENGE = 2,
	// 好友
	NOTIFY_TAG_FRIEND_ADD_OR_RECV_MANUAL = 1,
	// 邮件
	NOTIFY_TAG_MAIL_UNREAD = 1,
	// 改装
	NOTIFY_TAG_REFIT_PART = 1,
	// 夺宝奇兵
	NOTIFY_TAG_FREE_LOTTERY = 1,
	// 日常活动
	NOTIFY_TAG_GET_MANUAL = 1,
	NOTIFY_TAG_DAILY_ACT = 2,
	// 枪魂
	NOTIFY_TAG_WEAPON_BOOK = 1,
	// 领取奖励
	NOTIFY_TAG_GET_AWARD = 1,
	// 首充礼包
	NOTIFY_TAG_FIRST_CHARGE_PKG = 1,
	//活动
	NOTIFY_TAG_ACT = 1,
	//签到
	NOTIFY_TAG_SIGNIN = 1,
	//在线礼包
	NOTIFY_TAG_ONLINE_AWARD = 1,
	//次日礼包
	NOTIFY_TAG_NEXTDAY_AWARD =1,
};

class NotificationControl
{
public:
	NotificationControl();
	~NotificationControl();

	void init();

	void finalize();

	void secTimer(unsigned int seconds); // 计时

	void check(); // 检查

	const Notification* getFrontNotification(REMIND_WAY way); // 按通知方式读取位于最前面的通知

	void pushNotification(unsigned int uEntryID, unsigned int uTag, unsigned int uBadgeNum);

	void pushNotification(const Notification &note); // 推送通知

	void lockNotification(unsigned int uEntryID, unsigned int uTag); // 安静一会儿，解锁后推送才有效

	void removeNotification(unsigned int uEntryID, unsigned int uTag); // 移除通知

	void showNotification(); // 展示在主界面提醒的通知

	NotificationLayout* getNotificationLayout();

private:
	bool m_bInitialized;

	NotificationPanel *m_pNotificationPanel;

	NotificationLayout *m_pNotificationLayout;

	unsigned int m_uSecTime; // seconds

	list<Notification> m_stPushLst; // Notification

	map<pair<unsigned int, unsigned int>, unsigned int> m_stLockedMap; // <pair<EntryID, Tag>, Time>

};

#define P_NOTIFICATION_CTRL CVLib::CVSingleton<NotificationControl>::Instance()

#define PUSH_NOTIFICATION(uEntryID, uTag, uBadgeNum) \
do { \
	P_NOTIFICATION_CTRL->pushNotification((uEntryID), (uTag), (uBadgeNum)); \
} while(0)

#endif // NotificationControl_h__