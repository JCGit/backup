#ifndef NotificationControl_h__
#define NotificationControl_h__

#include "vlib/base/CVSingleton.h"

#include "CfgMaster/NotificationCfgMaster.h"

#include "game/city/EntryControl.h"

#include "NotificationPanel.h"
#include "NotificationLayout.h"

typedef enum RemindWay {
	REMIND_BY_BADGE = 0x1, // ���ϽǱ��
	REMIND_BY_ALERT = 0x2, // ����������
} REMIND_WAY;

enum NotifyTag {
	// ǹе��
	NOTIFY_TAG_WEAPON_LVUP = 1,
	NOTIFY_TAG_WEAPON_ADVANCE = 2,
	// װ������
	NOTIFY_TAG_EQUIP_LVUP = 1,
	NOTIFY_TAG_EQUIP_ADVANCE = 2,
	// �ɾ�
	NOTIFY_TAG_MILESTONE_FINISHED = 1,
	// ����
	NOTIFY_TAG_EVOLUTION = 1,
	// ����ǿ��
	NOTIFY_TAG_ATTR_LVUP = 1,
	// �ȼ�����
	NOTIFY_TAG_LEVEL_UNREACHED = 1,
	// ��������
	NOTIFY_TAG_MANUAL_NOT_ENOUGH = 1,
	// ����װ��
	NOTIFY_TAG_ADVANCED_EQUIP_IN_BAG = 1,
	// ������
	NOTIFY_TAG_ARENA_NEW_RECORD = 1,
	NOTIFY_TAG_ARENA_FREE_CHALLENGE = 2,
	// ����
	NOTIFY_TAG_FRIEND_ADD_OR_RECV_MANUAL = 1,
	// �ʼ�
	NOTIFY_TAG_MAIL_UNREAD = 1,
	// ��װ
	NOTIFY_TAG_REFIT_PART = 1,
	// �ᱦ���
	NOTIFY_TAG_FREE_LOTTERY = 1,
	// �ճ��
	NOTIFY_TAG_GET_MANUAL = 1,
	NOTIFY_TAG_DAILY_ACT = 2,
	// ǹ��
	NOTIFY_TAG_WEAPON_BOOK = 1,
	// ��ȡ����
	NOTIFY_TAG_GET_AWARD = 1,
	// �׳����
	NOTIFY_TAG_FIRST_CHARGE_PKG = 1,
	//�
	NOTIFY_TAG_ACT = 1,
	//ǩ��
	NOTIFY_TAG_SIGNIN = 1,
	//�������
	NOTIFY_TAG_ONLINE_AWARD = 1,
	//�������
	NOTIFY_TAG_NEXTDAY_AWARD =1,
};

class NotificationControl
{
public:
	NotificationControl();
	~NotificationControl();

	void init();

	void finalize();

	void secTimer(unsigned int seconds); // ��ʱ

	void check(); // ���

	const Notification* getFrontNotification(REMIND_WAY way); // ��֪ͨ��ʽ��ȡλ����ǰ���֪ͨ

	void pushNotification(unsigned int uEntryID, unsigned int uTag, unsigned int uBadgeNum);

	void pushNotification(const Notification &note); // ����֪ͨ

	void lockNotification(unsigned int uEntryID, unsigned int uTag); // ����һ��������������Ͳ���Ч

	void removeNotification(unsigned int uEntryID, unsigned int uTag); // �Ƴ�֪ͨ

	void showNotification(); // չʾ�����������ѵ�֪ͨ

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