#ifndef EntryID_h__
#define EntryID_h__

typedef enum EntryID {
	ENTRY_UNKNOWN, // δ֪
	ENTRY_ROLE, // ��ɫ
	ENTRY_EQUIP_CULTURE, // װ������
	ENTRY_WEAPON_LIB, // ǹе��
	ENTRY_GROWTH, // �ɳ�
	ENTRY_SOCIAL, // �罻
	ENTRY_CHALLENGE, // ��ս
	ENTRY_BUSINESS, // ����
	ENTRY_RANK, // ���а�
	ENTRY_SHOP, // �̳�
	ENTRY_LOTTERY, // �ᱦ���
	ENTRY_WEAPON_BOOK, // ǹ��
	ENTRY_CHAT, // ����
	ENTRY_MAIL, // �ʼ�
	ENTRY_DAILY_TASK, // �ճ�����
	ENTRY_SIGN_IN, // ǩ��
	ENTRY_BUY_COIN, // �һ���
	ENTRY_DAILY_ACT, // �ճ��
	ENTRY_SETTINGS, // ����
	ENTRY_REFIT, // ��װ
	ENTRY_LIMITED_SHOP, // �޹��̳�
	ENTRY_ACTIVITIES, // �
	ENTRY_SECRET_CHAT, // ˽��
	ENTRY_DAILY_WAR, // �ճ�����
	ENTRY_LEVEL_UP, // �����ȼ�
	ENTRY_WAR, // ��ͨ����
	ENTRY_BOSS_WAR, // �������
	ENTRY_GET_AWARD, // ��ȡ����
	ENTRY_WAR_CHALLENGE, // ��ս����
	ENTRY_FIRST_CHARGE_PKG, // �׳����
	ENTRY_VIP,	// vip
	ENTRY_ONLINE_AWARD, //�������
	ENTRY_MORROW_AWARD, //�������

	// ǹе��
	ENTRY_WEAPON_CULTURE = (ENTRY_WEAPON_LIB << 8) + 1, // ǹе����
	ENTRY_WEAPON_ARRAY, // ս��

	// �ɳ�
	ENTRY_EVOLUTION = (ENTRY_GROWTH << 8) + 1, // ����
	ENTRY_ATTR_STRENGTHEN, // ����ǿ��
	ENTRY_GENE_STRENGTHEN, // ����ǿ��

	// �罻
	ENTRY_FRIEND = (ENTRY_SOCIAL << 8) + 1, // ����
	ENTRY_GUILD, // ����
	// ��ս
	ENTRY_ARENA = (ENTRY_CHALLENGE << 8) + 1, // ������
	ENTRY_NOTHING_LAND, // ����֮��
	ENTRY_ELITE_WAR, // ��Ӣ����
	// �̻�
	ENTRY_BLACK_MARKET = (ENTRY_BUSINESS << 8) + 1, // ����
	ENTRY_FRAGMENT, // ��Ƭ

} ENTRY_ID;

#endif // EntryID_h__