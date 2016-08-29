#ifndef __TEXT_HELPER_H__
#define __TEXT_HELPER_H__

#include "TextCfgMaster.h"
#include "vlib/base/CVSingleton.h"

enum enumBaseTextConsts
{
    EQUIP_REFINE_PANEL   = 100, ///< װ��ǿ�����
    WEAPON_PANEL = 200,
    TEXT_GUILD_BASE = 300, // ����
	ENDLESS_PANEL = 400,		//�޾�ģʽ�����
	ATTR_STRENGTHEN_BASE = 500, // ��������
	SWEEP_INFO_BASE = 600,		//ɨ��
	ARENA_BASE = 700,			//������
	VIP_BASE = 800, // VIP
	QIANGHUN_BASE = 900,		//ǹ��
	COMMON_BASE = 1100, // ͨ��
	EVOLUTION_BASE = 1200, // ����
	MODULE_OPEN_BASE = 1300, // ���ܿ���
	FIGHT_BASE = 1400, // ս��
	BAG_BASE = 1500, // ����
	TASK_BASE = 1600, // ����
	DAILYT_ACT_BASE = 1700,	//�ճ��
	LOTTERY_BASE = 1800, // �鿨
	GOLD_TIP_BASE = 2000,	//������ʾ
	ACT_TIP_BASE = 2200,	//���ʾ
	WORLD_BOSS_BASE = 2400,	//����bossս��ʾ
	WEAPON_ARR_BASE = 2500,	//ս����ʾ
	GET_AWARD_BASE = 2600, // ��ȡ����
	GEN_TIP_BASE = 2700,	//����ǿ��
	WAR_MAP_BASE = 2800,//����
}; //end enumBaseTextConsts


//װ��ǿ�����
enum enumEquipRefineTextConsts
{
    //100
    EQUIP_REFINE_FULL_TIP   = EQUIP_REFINE_PANEL + 1, ///< ǿ��ȫ���ﵽ%d��\n�ɼ���\n%s+%d
    EQUIP_REFINE_MAX_REFINE_LV   = EQUIP_REFINE_PANEL + 2, ///< �Ѿ��ﵽ���ǿ���ȼ�
    TEXT_MAIL_DEL_TIP   = EQUIP_REFINE_PANEL + 3, ///< ����δ��ȡ�Ľ�����ȷ��ɾ����
    TEXT_REMAIN_MANUAL_TIMES  = EQUIP_REFINE_PANEL + 4, ///< ʣ�����ȡ����������%d
    TEXT_ACT_DONE  = EQUIP_REFINE_PANEL + 5, ///< �����
    TEXT_ACT_FAIL  = EQUIP_REFINE_PANEL + 6, ///< �ʧ��
    TEXT_ADD_FRIEND_TIP = EQUIP_REFINE_PANEL + 7, ///< ���ͼӺ�������ɹ����ȴ��Է�ȷ�ϡ�
    TEXT_SENDER = EQUIP_REFINE_PANEL + 8,   ///< ������
    TEXT_SUBJECT = EQUIP_REFINE_PANEL + 9,  ///< ����
    TEXT_NO_MAIL = EQUIP_REFINE_PANEL + 10, ///< ��ʱû��δ�����ʼ�
    TEXT_REFINE_LV_INVALID = EQUIP_REFINE_PANEL + 11, ///< ֻ�ܽ�ǿ���ȼ��ߵ�װ��ת�Ƹ��͵�װ��
    TEXT_MAIL_MANUAL = EQUIP_REFINE_PANEL + 12, ///< ���ĺ��ѡ�%s������������2������
    TEXT_REBOOT_TIP = EQUIP_REFINE_PANEL + 13, ///< ������ɣ������ť�󽫹ر���Ϸ�������½���
    TEXT_ENTER_TIP = EQUIP_REFINE_PANEL + 14, ///< �����ť������Ϸ
    TEXT_MAX_MANUAL_RECV = EQUIP_REFINE_PANEL + 15, ///< ÿ�տ���ȡ10������
    TEXT_NO_MANUAL_RECV = EQUIP_REFINE_PANEL + 16, ///< û�п�����ȡ������ 
    TEXT_REFINE_MAX_LV = EQUIP_REFINE_PANEL + 17, ///< %d��װ����߿�ǿ����+%d
    TEXT_WEAPON_ADV_LV_TIP = EQUIP_REFINE_PANEL + 18, ///< �ȼ��ﵽ%d���ɽ���
    TEXT_REFINE_SUCCESS = EQUIP_REFINE_PANEL + 19, ///< ǿ���ɹ�
    TEXT_REFINE_FAIL    = EQUIP_REFINE_PANEL + 20, ///< ǿ��ʧ��
    TEXT_SEL_EQUIP_TIP  = EQUIP_REFINE_PANEL + 21, ///< ��û��ѡ��װ��
    TEXT_EAT_EQUIP_TIP  = EQUIP_REFINE_PANEL + 22, ///< ���Ĳ����а���װ�����Ƿ����������
    TEXT_EQUIP_ADV_TIP  = EQUIP_REFINE_PANEL + 23, ///< ����������ſɽ���
    TEXT_EQUIP_MIX_FULL_TIP  = EQUIP_REFINE_PANEL + 24, ///< �Ѵﵽ����ںϵȼ�
    TEXT_EQUIP_MAT_NULL  = EQUIP_REFINE_PANEL + 25, ///< û�п���ʹ�õĲ���

}; //end enumEquipRefineTextConsts

//�������
enum enumWeaponTextConsts
{
	WEAPON_NO_ENOUGH = WEAPON_PANEL + 1,				//���ϲ���
	WEAPON_NO_LEVEL  = WEAPON_PANEL + 2,				//�ȼ�����
	WEAPON_GET_EXP  = WEAPON_PANEL + 3,					//��þ���
	WEAPON_OFFER_EXP  = WEAPON_PANEL + 4,				//�ṩ����
	WEAPON_CHOOSE_WEAPON  = WEAPON_PANEL + 5,			//ѡ��ǹе
	WEAPON_FULL_LEVEL  = WEAPON_PANEL + 6,				//�ȼ�����
	WEAPON_CHOOSE_MATERIAL  = WEAPON_PANEL + 7,			//ѡ�����

	WEAPON_ATTR_TYPE_NONE = WEAPON_PANEL + 8,			//������
	WEAPON_ATTR_TYPE_ICE = WEAPON_PANEL + 9,			//����
	WEAPON_ATTR_TYPE_FIRE = WEAPON_PANEL + 10,			//��
	WEAPON_ATTR_TYPE_DRUG = WEAPON_PANEL + 11,			//����
	WEAPON_ATTR_TYPE_BIO = WEAPON_PANEL + 12,			//������
	WEAPON_ATTR_TYPE_THUNDER  = WEAPON_PANEL + 13,		//�׹�

	WEAPON_STAR_UP_DESC = WEAPON_PANEL + 14,			//%d��ǹе������%d��ǹе����ԭ��

	WEAPON_REFIT_DESC = WEAPON_PANEL + 15,				//%s��װΪ%s�������
	WEAPON_REFIT_TITLE = WEAPON_PANEL + 16,				//��װ�ֲ�
	WEAPON_REFIT_ENOUGH = WEAPON_PANEL + 17,			//��װ����
    WEAPON_STAR_UP_ENOUGH = WEAPON_PANEL + 18,			//�Ǽ�����
    WEAPON_BAG_FULL = WEAPON_PANEL + 19,			    //ǹе������
	WEAPON_LEVEL_UP_SELECTED_WEAPON = WEAPON_PANEL + 20, // ��������Ϊ���ϵ�ǹе������ʧ���Ƿ������
	WEAPON_NO_ENOUGH_GOTOBUY = WEAPON_PANEL + 21, //���ϲ��㣬ǰ�����к��޹��̵깺��
}; //end enumEquipRefineTextConsts

enum enumSweepTextConsts
{
	SWEEP_ROUND_DESC = SWEEP_INFO_BASE + 1,				//�ڼ���
	SWEEP_WAR = SWEEP_INFO_BASE + 2,				//��%d��
};

enum enumArenaTextConsts
{
	ARENA_WIN_TXT = ARENA_BASE + 1,						//������ʤ������
	ARENA_LOSE_TXT = ARENA_BASE + 2,					//������ʧ������
	ARENA_MATCH_COST = ARENA_BASE + 3,					//����:%d�����
	ARENA_FREE_MATCH = ARENA_BASE + 4,					//���ƥ�����
	COIN_DESC_TXT = ARENA_BASE + 5,						//����ҵ�����
	GOLD_DESC_TXT = ARENA_BASE + 6,						//���������
	ARENA_COIN_DESC_TXT = ARENA_BASE + 7,				//��������ѫ������
	ARENA_MATCH_FAIL_TXT = ARENA_BASE + 8,				//ƥ��ʧ��
	ARENA_COIN_NOT_ENOUGH_TXT = ARENA_BASE + 9,			//��ѫ����
    ARENA_FIGHT_NUM_NOT_ENOUGH_TXT = ARENA_BASE + 10,	//��ս����Ϊ�㣬�빺����ս����
    ARENA_EXCHANGE_NUM_TIP = ARENA_BASE + 11,	        //��ǰӵ�У�%d
    ARENA_RD_WIN        = ARENA_BASE + 12,				//�����ܡ�%s��ȡ��ʤ������������%d������%s��
    ARENA_RD_LOSE       = ARENA_BASE + 13,				//����ս��%s��ʧ�ܣ���������%d������%s��
    ARENA_RD_LOSE_OTHER = ARENA_BASE + 14,				//������%s�����ܣ���ʧ�����%d������%s��
    ARENA_RD_LOSE_BACK  = ARENA_BASE + 15,				//���ܵ���%s���ķ�������ʧ�����%d������%s��
    ARENA_RD_WIN_BACK   = ARENA_BASE + 16,				//���ܵ���%s���ķ����������ʤ����
	ARENA_BUY_NUM_NOT_ENOUGH = ARENA_BASE + 17,			//�������������
	ARENA_REFRESH_NUM_NOT_ENOUGH = ARENA_BASE + 18,		//ˢ�´���������
	ARENA_FIGHT_CD_TXT = ARENA_BASE + 19,				//CD��ȴ��
};

// ����
enum enumGuildTextConsts {
	TEXT_GUILD_CREATE_COST = TEXT_GUILD_BASE + 1, // ����%d���Ǳң��ɴ������䣡
	TEXT_GUILD_NULL_GUILD_NAME, // �������Ʋ���Ϊ�գ�
	TEXT_GUILD_INVALID_GUILD_NAME, // ��������Ч�Ĳ������ƣ�
	TEXT_GUILD_IMPEACH_CHAIRMAN, // ȷ��Ҫ������������
	TEXT_GUILD_LEAVE_GUILD, // ȷ��Ҫ�뿪������
	TEXT_GUILD_DISMISS_GUILD, // ȷ��Ҫ��ɢ������
    TEXT_GUILD_HAS_IMPEACHED, // ���������ڱ�������
    TEXT_GUILD_NOTIFY_JOIN,     // ��ϲ�����Ѿ�����׼���벿�� 8
    TEXT_GUILD_NOTIFY_DISMISS,  // �����ڵĲ����ѱ���ɢ 9
    TEXT_GUILD_NOTIFY_KICK,     // ���ѱ��Ƴ����� 10
    TEXT_GUILD_IMPEACH_TIME,     // ���뵯���ɹ����� 11
    TEXT_GUILD_IMPEACH_INVALID_RULE,     // ����������ʱ�䳬��5��ɷ����� 12
	TEXT_GUILD_APPLIED_LIMITED, // �Ѿ������˿��������ޣ�
};

// ��������
enum enumAttrStrengthen {
	TEXT_ATTR_STEP_1 = ATTR_STRENGTHEN_BASE + 1, // һ��
	TEXT_ATTR_STEP_2, // ����
	TEXT_ATTR_STEP_3, // ����
	TEXT_ATTR_STEP_4, // �Ľ�
	TEXT_ATTR_STEP_5, // ���
	TEXT_ATTR_STEP_6, // ����
	TEXT_ATTR_STEP_7, // �߽�
	TEXT_ATTR_STEP_8, // �˽�
	TEXT_ATTR_STEP_9, // �Ž�
	TEXT_ATTR_STEP_10, // ʮ��
};

// VIP
enum enumVIPTextConsts {
	TEXT_VIP_PRIVILEGE = VIP_BASE + 1, // VIP%d��Ȩ
    TEXT_VIP_GIFT = VIP_BASE + 2, // VIP%d���
    TEXT_VIP_NO_BUY_COIN_NUM = VIP_BASE + 3, // ����Ĺ�������Ѿ�����
    TEXT_VIP_BUY_COIN_NUM_DESC = VIP_BASE + 4,	//�����տ���%s��
    TEXT_SIGN_IN_DAY = VIP_BASE + 5,	//ǩ��%d��
    TEXT_SIGN_IN_REFRESH_TIME = VIP_BASE + 6,	//ÿ��%d��ˢ��
	TEXT_VIP_AUTO_FIGHT = VIP_BASE + 7,			//��ΪVIP���Զ�ս��
	TEXT_VIP_EXTRA_AWARD = VIP_BASE + 8,		//����������
};

enum enumEndlessTextConsts {
	ENDLESS_RAND_GEN = ENDLESS_PANEL + 1,				//���ǿ��
	ENDLESS_ADD_ATK_GEN = ENDLESS_PANEL + 2,			//��߹���
	ENDLESS_ADD_HP_DEF_GEN = ENDLESS_PANEL + 3,			//��߷�����������
	ENDLESS_ADD_SCORE_GEN = ENDLESS_PANEL + 4,			//��������
	ENDLESS_ADD_COIN_GEN = ENDLESS_PANEL + 5,			//�Ǳ�����
	ENDLESS_RUSH_GEN = ENDLESS_PANEL + 6,			    //������Ӱս�����%d��
};

enum enumQianghunTextConsts {
	TEXT_QIANGHUN_LEVEL = QIANGHUN_BASE + 1,			//��%d��
	TEXT_QIANGHUN_FEW = QIANGHUN_BASE + 2,				//ϡ��
	TEXT_QIANGHUN_BIT = QIANGHUN_BASE + 3,				//����
	TEXT_QIANGHUN_MORE = QIANGHUN_BASE + 4,				//�϶�
	TEXT_QIANGHUN_MASS = QIANGHUN_BASE + 5,				//����
	TEXT_QIANGHUN_GET_LEVEL = QIANGHUN_BASE + 6,		//��%d���ã�
	TEXT_QIANGHUN_NEXT_LEV_TIP = QIANGHUN_BASE + 7,		//������δ��Ļ꿨��ȷ��Ҫ����ǰ����һ����
	TEXT_QIANGHUN_KEY_TIP = QIANGHUN_BASE + 8,			//���ѻ�á�ǹ����Կ����������һ�㰴ť�Ѿ���������
};

// ͨ��
enum enumCommonTextConsts {
	TEXT_NOTHING = COMMON_BASE + 1, // û���ˣ�
	TEXT_LEVEL_UNREACHED, // ���ĵȼ�������
	TEXT_COIN_NOT_ENOUGH, // ��������Ҳ��㣡
	TEXT_GOLD_NOT_ENOUGH, // �������겻�㣡
	TEXT_CONFIRM_GOLD_COST, // �Ƿ񻨷�%d���ꣿ
    TEXT_ITEM_NOT_ENOUGH, // ����[%s]���㣡
    TEXT_NET_CLOSE, // �������ӶϿ�
    TEXT_CONFIRM_COIN_COST, // ��Ҫ����%d����ң��Ƿ������
    TEXT_NET_RECONNECT, // ��������
    TEXT_NET_RECONNECT_DONE, // �����ɹ�
	TEXT_TODAY, // ����
	TEXT_A_FEW_DAYS_AGO, // ������ǰ
    TEXT_A_MONTH_AGO, // һ������ǰ
    TEXT_SDK_LOGIN_FAIL, // ��½ʧ��
    TEXT_SDK_BUY_SUCCESS, // ֧���ɹ�����ȴ�����
    TEXT_SDK_BUY_FAIL, // ֧��ʧ��
	TEXT_CONGRATULATIONS_TO_GET, // ��ϲ����� %s
	TEXT_SDK_BIND_SUCCESS_TXT,	//���˺ųɹ�
	TEXT_SDK_LOGOUT_SUCCESS_TXT,	//�ǳ��ɹ��������µ�½��
	TEXT_SDK_SHARE_SUCCESS,		//����ɹ�
	TEXT_SDK_SHARE_FAILED,		//����ʧ��
	TEXT_SVR_NOT_SERVICES,		//������δ����
	TEXT_SVR_STOP_SERVICES,		//ͣ��ά��
	TEXT_SVR_RECOMMEND,			//�·�
	TEXT_SVR_CROWED,			//ӵ��
	TEXT_SVR_FULL,				//����
};

// ����
enum enumEvolutionTextConsts {
	TEXT_EVOLUTION_STONE_NOT_ENOUGH = EVOLUTION_BASE + 1, // ���Ľ���ʯ���㣡
	TEXT_EVOLUTION_LEVLE_LMITED_DESC, // �ȼ��ﵽ%d����������!
};

// ���ܿ���
enum enumModuleOpenTextConsts {
	TEXT_MODULE_OPEN_EVOLUTION_SKILL = MODULE_OPEN_BASE + 1, // ��ϲ����������[��������]��
	TEXT_MODULE_OPEN_BAG, // ��ϲ����������[����]��
	TEXT_MODULE_OPEN_EQUIP_LEVELUP, // ��ϲ����������[װ��ǿ��]��
	TEXT_MODULE_OPEN_LOTTERY, // ��ϲ����������[̽��]��
	TEXT_MODULE_OPEN_WEAPON_LEVELUP, // ��ϲ����������[ǹеǿ��]��
	TEXT_MODULE_OPEN_WEAPON_STARSTEP, // ��ϲ����������[ǹе����]��
	TEXT_MODULE_OPEN_DAILY_TASK, // ��ϲ����������[�ճ�����]��
	TEXT_MODULE_OPEN_RANK, // ��ϲ����������[���а�]��
	TEXT_MODULE_OPEN_FRIENDS, // ��ϲ����������[����]��
	TEXT_MODULE_OPEN_GUILD, // ��ϲ����������[����]��
	TEXT_MODULE_OPEN_EVOLUTION, // ��ϲ����������[����]��
	TEXT_MODULE_OPEN_ATTR_STRENGTHEN, // ��ϲ����������[���Թ���]��
	TEXT_MODULE_OPEN_ARENA, // ��ϲ����������[������]��
	TEXT_MODULE_OPEN_INFINITE, // ��ϲ����������[�޾�ģʽ]��
	TEXT_MODULE_OPEN_MAIL, // ��ϲ����������[�ʼ�]��
	TEXT_MODULE_OPEN_REFIT, // ��ϲ����������[��װ]��
	TEXT_MODULE_NOOPEN, //�ù�����δ���ţ�
};

// ս��
enum enumFightTextConsts {
    TEXT_NOT_RECV_THE_TASK = FIGHT_BASE + 1,    // ��δ��ȡ������
    TEXT_BOSS_WAR_PASS_TIP = FIGHT_BASE + 2,    // ��ɱBOSS��%s��
    TEXT_NORMAL_WAR_PASS_TIP = FIGHT_BASE + 3,  // ������й���
    TEXT_WAR_PASS_TIP_KILL_NUM = FIGHT_BASE + 4,  // �����%d��������
    TEXT_WAR_PASS_TIP_KILL_NUM_SPEC = FIGHT_BASE + 5,  // �����%d������%s��
    TEXT_WAR_PASS_TIP_TIME = FIGHT_BASE + 6,  // ���ս����%s��
	TEXT_WAR_HAVE_NO_FIGHT_TIMES = FIGHT_BASE + 7,	// �ؿ�����������
	TEXT_NIHILITY_USE_ITEM = FIGHT_BASE + 8,		// ʹ���ؾ�
	TEXT_NIHILITY_FREE_TXT = FIGHT_BASE + 9,		// ���%s��
	TEXT_SURE_TO_REFRESH = FIGHT_BASE + 10,		// ȷ��Ҫˢ����
	TEXT_SWEEP_STOP = FIGHT_BASE + 11,		// ����������ֹͣɨ����
    TEXT_NIHILITY_USE_ITEM_TIP = FIGHT_BASE + 12,	//��ѡ���ؾ�
    TEXT_WAR_NOT_CLEAR_TIP = FIGHT_BASE + 13,	//�ùؿ���δͨ��
	TEXT_NIHILITY_STAGE_DESC = FIGHT_BASE + 14, //��ǰ��%d��
	TEXT_RELIVE_COST_ITEM_TIP = FIGHT_BASE + 15,	//��ʣ��%d�����
	TEXT_FILL_BULLET_COST_ITEM_TIP = FIGHT_BASE + 16,	//��ʣ��%d��������ҩ��
	TEXT_BULLET_OVER_TIP = FIGHT_BASE + 17,			//��ҩ�þ����뻻ǹ
	TEXT_AUTO_FILL_BULLET_TIP = FIGHT_BASE + 18,	//��ҩ��������ҩ��-1
	TEXT_RELIVE_BTN_GOLD_LABEL_TIP = FIGHT_BASE + 19,	//���긴��
	TEXT_RELIVE_BTN_LABEL_TIP = FIGHT_BASE + 20,		//��   ��
	TEXT_STAR_CNT_UNREACHED = FIGHT_BASE + 21,		// �ռ�%d����ȡ���䡣
	TEXT_FULL_STAR_CONDITION = FIGHT_BASE + 22,  //%d����ͨ��
};

// ����
enum enumBagTextConsts {
	TEXT_SALE_ITEM = BAG_BASE + 1, // ȷ��Ҫ������
    TEXT_BAG_FULL_OP_TIP = BAG_BASE + 2, // �������������������
    TEXT_SHOP_BUY_DONE = BAG_BASE + 3, // ����ɹ�
	TEXT_BAG_NOT_ENOUGH = BAG_BASE + 4, // ����ʣ��ռ䲻�㣬���������
};

// ����
enum enumTaskTextConsts {
	TEXT_TASK_LEVEL_UNREACHED = TASK_BASE + 1, // �ȼ��ﵽ%d���ɽ�ȡ������
	TEXT_MONTH_CARD, // �¿�
	TEXT_BUY_MONTH_CARD, // �����¿�
	TEXT_MONTH_CARD_AWARD_DESC, // �����¿�ÿ�տ��Ի��%d����
	TEXT_NONE_VALID_TASK, // ���޿ɽ�����
};

//�ճ��
enum enumDailyActTextConsts{
	TEXT_DAILY_ACT_PREPARE_STAT = DAILYT_ACT_BASE + 1,	//δ��ʼ
	TEXT_DAILY_ACT_START_STAT = DAILYT_ACT_BASE + 2,	//�ѿ�ʼ
	TEXT_DAILY_ACT_END_STAT = DAILYT_ACT_BASE + 3,		//�ѽ���
	TEXT_DAILY_ACT_JOINED_STAT = DAILYT_ACT_BASE + 4,	//�Ѳ���
	TEXT_DAILY_FORCE_QUIT = DAILYT_ACT_BASE + 5,		//�Ƿ��˳����
	TEXT_DAILY_ACT_ENTER = DAILYT_ACT_BASE + 6,			//��  ��
	TEXT_DAILY_MANUAL_CAN_GET = DAILYT_ACT_BASE + 7,	//����ȡ
	TEXT_DAILY_MANUAL_GET = DAILYT_ACT_BASE + 8,		//��  ȡ
	TEXT_DAILY_MANUAL_GETED = DAILYT_ACT_BASE + 9,		//����ȡ
};

//�鿨
enum enumLotteryTextConsts{
	TEXT_AFTER_TIMES_CERTAINLY_LOTTERY = LOTTERY_BASE + 1,	// ��Ѱ��%d�κ��´αص�����װ��
	TEXT_CERTAINLY_LOTTERY,	// �´αص�����װ��
	TEXT_NEED_GOLD, // %d����
	TEXT_VIP_LOTTERY_INTRO, // VIP%d�����ÿ�տ���%d�����������Ȩ
	TEXT_FIRST_CHARGE_PKG, // �׳����
};


enum enumGoldCostConsts{
	TEXT_COMMON_COST_TIP = GOLD_TIP_BASE + 1,		//ͨ��������ʾ:�Ƿ�����%d���깺��
	TEXT_MANUAL_COST_TIP = GOLD_TIP_BASE + 2,		//�����������Ƿ�����%d���깺��\n%d������ʣ��ɹ������%d��
	TEXT_REFRESH_COST_TIP = GOLD_TIP_BASE + 3,		//ˢ���̵꣬�Ƿ�����%d����ˢ��
    TEXT_BUY_ARENA_COST_TIP = GOLD_TIP_BASE + 4,	//����������Ƿ�����%d���깺����ս������ʣ��ɹ���%d��
    TEXT_RMB_FIRST_GIFT = GOLD_TIP_BASE + 5,		//�״�����%d����
    TEXT_RMB_EXTRA_GIFT = GOLD_TIP_BASE + 6,		//��������%d����
    TEXT_MONTH_CARD_TIP = GOLD_TIP_BASE + 7,		//%d����ÿ�����ȡ%d����
	TEXT_MANUAL_BUY_TIP = GOLD_TIP_BASE + 8,		//�������㣬�Ƿ���
	TEXT_MANUAL_LOW_TIP = GOLD_TIP_BASE + 9,		//�������㣬
	TEXT_LEVEL_LOW_TIMES_TIP = GOLD_TIP_BASE + 10,	//�ؿ���������
	TEXT_CLEAN_CD_TIP = GOLD_TIP_BASE + 11,			//�Ƿ�����%d�������CD
	TEXT_RESTRICT_BUY_TIP = GOLD_TIP_BASE + 12,		//�����ѹ����޷��ظ�����
	TEXT_RESTRICT_SELL_OUT_TIP = GOLD_TIP_BASE + 13,//�������۾������´�����
	TEXT_RELIVE_COST_GOLD_TIP = GOLD_TIP_BASE + 14, //�Ƿ�����%d���긴�
	TEXT_FILL_BULLET_COST_GOLD_TIP = GOLD_TIP_BASE + 15,	//�Ƿ�����%d���깺��ҩ��
	TEXT_DIFF_GOLD_PROMPT = GOLD_TIP_BASE + 16,		// �˴λ��ѻ���%d����
	TEXT_CHARGE_GOLD_NUM_TIP = GOLD_TIP_BASE + 17,	//ÿ��ֵ%d��ʯ����
	TEXT_BUY_WAR_FIGHT_CNT_TIP = GOLD_TIP_BASE + 18,//�Ƿ�����%d���깺��10����ս������
	TEXT_BUY_EXP_ITEM = GOLD_TIP_BASE + 19,// %s������%dԪ���Ƿ����������
	TEXT_RMB_BUY = GOLD_TIP_BASE + 20,// ��%d ����
	TEXT_BUY_ITEM_GOLD_TIP = GOLD_TIP_BASE + 21,	//�Ƿ�����%d���깺��%d����
	TEXT_SWEEPCARD_LOW_TIP = GOLD_TIP_BASE + 22,//ɨ��������
};

enum enumActTipTextConsts{
	TEXT_ACT_CDK_CLAIM_TIP = ACT_TIP_BASE + 1,		//�һ��ɹ�
	TEXT_ACT_FIRST_CHARGE_TIP,	//�׳������ȡ�ɹ�
	TEXT_ACT_SPRING_DAY_1,	// ��һ
	TEXT_ACT_SPRING_DAY_2,	// ����
	TEXT_ACT_SPRING_DAY_3,	// ����
	TEXT_ACT_SPRING_DAY_4,	// ����
	TEXT_ACT_SPRING_DAY_5,	// ����
	TEXT_ACT_SPRING_DAY_6,	// ����
	TEXT_ACT_SPRING_DAY_7,	// ����
	TEXT_ACT_SPRING_DAY_8,	// ����
	TEXT_ACT_SPRING_DAY_9,	// ����
	TEXT_ACT_SPRING_DAY_10,	// ��ʮ
	TEXT_ACT_SPRING_DAY_11,	// ʮһ
	TEXT_ACT_SPRING_DAY_12,	// ʮ��
	TEXT_ACT_TIME_LAST_FORMAT,	//��ֵ�ʱ��XXX-XXX
	TEXT_ACT_CHARGE_LEVEL,	//��ֵXX����
	TEXT_ACT_TOTAL_CHARGE,	// ��ǰ��ֵ
	TEXT_ACT_TOTAL_LOGINED,	// ��ǰ�ۼƵ�½����
	TEXT_ACT_TIME_FOREVER,	// �ʱ��-  ����
	TEXT_ACT_REWARD_BEST,	// �ռ����
	TEXT_ACT_LOGIN_DAYS,	// ��½�ڼ���
	TEXT_ACT_TOTAL_FIGHT,	// ��ǰս����
	TEXT_ACT_FIGHTING_LEVEL,// ս���ȼ�
	TEXT_ACT_CUR_LEVEL,		// ��ǰ�ȼ�
	TEXT_ACT_LEVEL,			// �ȼ�
	TEXT_ACT_REWARD_LAST,	// ���ʣ������
	TEXT_ACT_GET_FAILED,	// ��ȡ����ʧ�ܣ���������
	TEXT_ACT_VIP_GIFT,//VIP�ȼ�
	TEXT_ACT_VIP_NOW,//��ǰVIP�ȼ�
	TEXT_ACT_DAILY_CONSUME,//�����ۼ�����
	TEXT_ACT_SUM_CONSUME,//��ǰ���ѽ��
	TEXT_ACT_NEED_GOLD,//����
	TEXT_ONLINE_AWARD, //�������
	TEXT_NEXTDAY_AWARD, //���յ�¼���
};

enum enumWorldBossWarTextConsts{
	TEXT_WORLD_BOSS_ENTRY_TIP = WORLD_BOSS_BASE + 1,			//%s����ս��
	TEXT_WORLD_BOSS_DAMAGE_TIP = WORLD_BOSS_BASE + 2,			//%s��BOSS���%d�˺����ﱤ��
	TEXT_WORLD_BOSS_SKILL_DAMAGE_TIP = WORLD_BOSS_BASE + 3,		//%sʹ��%s��BOSS���%d�˺�����ק���ŵ�ը�죡
	TEXT_WORLD_BOSS_RELIVE_TIP = WORLD_BOSS_BASE + 4,			//�Ƿ�����%d���긴�
	TEXT_WORLD_BOSS_TISHEN_TIP = WORLD_BOSS_BASE + 5,			//�Ƿ�ʹ������BOSSս��ʼʱ�Զ��۳�%d���꣡
};

enum enumWeaponArrTextConsts{
	TEXT_WEAPON_ARR_ADD_TIP = WEAPON_ARR_BASE + 1,				//ս���ӳɣ���������%d%����������%d%������������%d% ��
};

enum enumGetAwardTextConsts {
	TEXT_MONTH_CARD_AWARD = GET_AWARD_BASE + 1, // �¿�ÿ�ս���
	TEXT_VIP_DAILY_AWARD, // VIPÿ�ս���
};

enum enumGenTextConsts {
	TEXT_GEN_UP_LIMIT = GEN_TIP_BASE + 1,						//(����)
	TEXT_GEN_DOWN_LIMIT = GEN_TIP_BASE + 2,						//(�Ѵ�����)
	TEXT_GEN_COST_GOLD_TIP = GEN_TIP_BASE + 3,					//�Ƿ�����%d�������ǿ����
};

enum enumWarMapTextConsts {
	TEXT_WAR_ELITE_CHAPTER = WAR_MAP_BASE + 1,      //��Ӣ�����½�
	TEXT_WAR_ELITE_NO_OPEN = WAR_MAP_BASE +2,    //���½���δ����

};

#define NEW_LINE "\n"
inline const char* TextStr(const int iKey)
{
    return TEXT_CFG_MASTER->GetCstr(iKey); 
}



#endif //__TEXT_HELPER_H__


