#ifndef __NOUN_HELPER_H__
#define __NOUN_HELPER_H__

#include "NounCfgMaster.h"


enum enumNounConsts
{
    EQUIP   = 1, ///< װ��
    ITEM    = 2, ///< ����
    EXP     = 3, ///< ����
    LEVEL   = 4, ///< �ȼ�
    ATTR    = 5, ///< ����
    COIN    = 6, ///< ͭǮ
    GOLD    = 7, ///< �ƽ�
    COST    = 8, ///< ����
    UPVAL   = 9, ///< ����
    MAXVAL  = 10, ///< ���ֵ
    WEAPON  = 11, ///< ����
    RUNSPEED = 12, ///< �����ٶ�
    HP      = 13, ///< ����
    DEF     = 14, ///< ����
    MOVESPEED = 15, ///< �ƶ��ٶ�
    MP      = 16, ///< ����
    ATK     = 17, ///< ����
    RING    = 18, ///< ����
    ARMOR   = 19, ///< �׾�
    CLOTH   = 20, ///< ����
    SHOE    = 21, ///< Ь��
    OPEN    = 22, ///< ����
    OPENING = 23, ///< ������
    CLOSE   = 24, ///< �ر�
    CLOSING = 25, ///< �ر���
    SEMICOLON = 26, ///< ��
    MULTIPLY = 27, ///< ��
    NOT_ENOUGH = 28, ///< ����
    OK      = 29, ///< ȷ��
    CANCEL  = 30, ///< ȡ��
    HEARY_ARMOR_DEF_STR = 31, ///< �ؼ�
    LIGHT_ARMOR_DEF_STR = 32, ///< ���
	NONE_ARMOR_DEF_STR = 33, ///< �޼�
    NONE = 34, ///< ��
    STR_ICE     = 35, ///< ��
    STR_FLAME   = 36, ///< ��
    STR_POISON  = 37, ///< ��
    STR_BIOTECH = 38, ///< ����
    STR_LIGHTING= 39, ///< ����
    TXT_THING_TYPE_GEN = 40, ///< ����
    TXT_THING_TYPE_MONTHCARD = 41, ///< �¿�
    TXT_THING_TYPE_DEBRIS = 42, ///< ��Ƭ
    TXT_THING_TYPE_MANUAL = 43, ///< ����
    TXT_TGAME_NAME = 44, ///< ����ս�� 
    TXT_TGAME_DESC_MONTHCARD = 45, ///< �����30����ÿ�տɻ��130��ʯ��
    TXT_NOW_VERSION = 46, ///< ��ǰ�汾��

}; //end enumNounConsts


//neil[1000-1999]
enum enumNounConstsNeil {
    MONTH       = 1000, ///< �� 
    DAY         = 1001, ///< ��
    HOUR        = 1002, ///< Сʱ
    MINUTE      = 1003, ///< ����
    SECOND      = 1004, ///< ��
    LEFT_TIME   = 1005, ///< ʣ��ʱ��
    SYSTEM      = 1006, ///< ϵͳ
    NOUN_FREE   = 1007, ///< ���
    NOUN_FREE2  = 1008, ///< �����
    NOUN_ADD    = 1009, ///< ���
    NOUN_REFUSE = 1010, ///< �ܾ�
    NOUN_DEL    = 1011, ///< ɾ��
    NOUN_CHAT1  = 1012, ///< ����
    NOUN_SEND_MANUAL    = 1013, ///< ��������
    NOUN_CHECK_VER      = 1014, ///< ������
    NOUN_DOWNLOAD       = 1015, ///< ������Դ
    NOUN_ENTER_GAME     = 1016, ///< ����
    NOUN_REBOOT_GAME    = 1017, ///< ����
    NOUN_DONE           = 1018, ///< ���
    NOUN_EXTRACE_RES    = 1019, ///< ������Դ
    NOUN_BASE           = 1020, ///< ����
    NOUN_REFINE         = 1021, ///< ǿ��
    NOUN_FRIEND_NUM     = 1022, ///< ��������
    NOUN_ACTIVATE       = 1023, ///< �Ѽ���
    NOUN_RECV_MANUAL    = 1024, ///< ��������
    NOUN_OUTTIME        = 1025, ///< �����
};

//beck[2000-2999]
enum enumNounConstsBeck {
	WEAPON_TAB = 2000,					//ǹе
	MATERIAL_TAB = 2001,				//��Դ
	CLICK_TIP = 2002,					//������
	PLAYER_NAME_TITLE = 2003,			//�����
	POWER_TITLE = 2004,					//ս��
	WARMAP_TITLE = 2005,				//�ؿ�
	ENDLESS_TITLE = 2006,				//����
	NO_RANK_TIP = 2007,					//��������
	TIME_COUNT_TXT = 2008,				//����ʱ
	RANK_AWARD_TXT = 2009,				//���а����
	PLAYER_POWER_TXT = 2010,			//����ս��
	PLAYER_ENDLESS_TXT = 2011,			//���˻���
	PLAYER_WARMAP_TXT = 2012,			//���˹ؿ�
	LOW_MANUAL_TXT = 2013,				//��������
	COIN_NOT_ENOUGH_TXT = 2014,			//����Ҳ���
	ENERGY_NOT_ENOUGH_TXT = 2015,		//��������
	DESC_STRING_TXT = 2016,				//����
	WIN_STRING_TXT = 2017,				//ʤ
	FAIL_STRING_TXT = 2018,				//��
	YEAR_STRING_TXT = 2019,				//��
	DAY_STRING_TXT = 2020,				//��
	HOUR_STRING_TXT = 2021,				//ʱ
	MINUTE_STRING_TXT = 2022,			//��
	ARENA_COIN_TXT = 2023,				//���������ң���ѫ
	NOUN_STAGE_TXT = 2024,				//�׶�
	NOUN_WEAPON_TXT = 2025,				//ǹе
	NOUN_COLOR_WHITE = 2026,			//��ͨ
	NOUN_COLOR_GREEN = 2027,			//��ɫ
	NOUN_COLOR_BLUE = 2028,				//��ɫ
	NOUN_COLOR_PURPLE = 2029,			//��ɫ
	NOUN_COLOR_GOLDEN = 2030,			//��ɫ
	NOUN_GOLD_NOT_ENOUGH = 2031,		//���겻�㣬�Ƿ��ֵ��
	NOUN_GOLD_CHARGE = 2032,			//�� ֵ
	NOUN_NAME_NOT_EMPTY = 2033,			//���ֲ���Ϊ�� 
	NOUN_NAME_NO_SPACE = 2034,			//�����в����пո�
	NOUN_NAME_TOO_LONG = 2035,			//���ֳ��ȳ�������
	NOUN_STOCK_TXT = 2036,				//���:
};

//taddy[3000-3999]
enum enumNounConstsTaddy {
	NOUN_ALL_GUILD = 3001, // ���в���
	NOUN_APPLY, // ����
	NOUN_APPLIED, // ������
	NOUN_UNDO, // ����
	NOUN_SEARCH, // ����
	NOUN_CREATE_GUILD, // ��������
	NOUN_ENTER_GUILD, // ���벿��
	NOUN_INVITE, // ����
	NOUN_AGREE, // ͬ��
	NOUN_APPLY_LIST, // �����б�
	NOUN_EXIT_GUILD, // �˳�����
	NOUN_DISMISS_GUILD, // ��ɢ����
	NOUN_IMPEACH_CHAIRMAN, // ����������
	NOUN_TO_DEPUTY, // ��ְ
	NOUN_TO_MEMBER, // ��ְ
	NOUN_BE_FRIEND, // ��Ϊ����
	NOUN_CHAT, // ˽��
	NOUN_IGNORE_LIST, // ������
	NOUN_KICKOUT_FROM_GUILD, // �߳�����
	NOUN_CHAIRMAN, // ������
	NOUN_DEPUTY, // ����
	NOUN_MEMBER, // ����
	NOUN_ONLINE, // ����
	NOUN_OFFLINE, // ������
	NOUN_HP, // ����
	NOUN_MP, // ����
	NOUN_ATTACK, // ����
	NOUN_DEFENSE, // ����
	NOUN_MOVE_SPEED, // �ƶ��ٶ�
	NOUN_RUN_SPEED, // �����ٶ�
	NOUN_DODGE_DISTANCE, // ���ܾ���
	NOUN_DODGE_COOLING, // ������ȴ
	NOUN_ATTR_DPS, // ����
	NOUN_RECV_TASK, // ��ȡ����
	NOUN_COMMIT_TASK, // �ύ����
	NOUN_ATK_ICE, // �����Թ���
	NOUN_ATK_FIRE, // �����Թ���
	NOUN_ATK_THUNDER, // �����Թ���
	NOUN_ATK_POISON, // �����Թ���
	NOUN_ATK_BIOCHEMISTRY, // �������Թ���
	NOUN_DEF_ICE, // �����Է���
	NOUN_DEF_FIRE, // �����Է���
	NOUN_DEF_THUNDER, // �����Է���
	NOUN_DEF_POISON, // �����Է���
	NOUN_DEF_BIOCHEMISTRY, // �������Է���
	NOUN_DRESSED_EQUIP, // ��ǰװ��
	NOUN_SELECTED_EQUIP, // ѡ��װ��
	NOUN_TAKE_ON, // ����
	NOUN_TAKE_OFF, // ж��
	NOUN_REPLACE, // �滻
	NOUN_ITEM, // ���
	NOUN_MATEARIAL, // ����
	NOUN_GIFT, // ���
	NOUN_COIN, // �����
	NOUN_GOLD, // ����
	NOUN_CLOSE_PANITRATE, // ���̴̻�
	NOUN_CLOSE_BLUNT, // ���̶ۻ�
	NOUN_CLOSE_MAGIC, // �������Թ���
	NOUN_CLOSE_ORDINARY, // ������ͨ����
	NOUN_REMOTE_MAGIC, // Զ�����Թ���
	NOUN_REMOTE_PANITRATE, // Զ�̴��̹���
	NOUN_REMOTE_ORDINARY, // Զ����ͨ����
	NOUN_GET, // ��ȡ
	NOUN_GOT, // ����ȡ
	NOUN_USE, // ʹ��
	NOUN_BUY, // ����
	NOUN_EXP, // ����
	NOUN_EQUIP, // װ��
	NOUN_WEAPON, // ����
	NOUN_ARENA_COIN, // ��ѫ
	NOUN_WEAPON_LEVELUP_PROP, // ǹеǿ������
	NOUN_WEAPON_STEPSTAR_PROP, // ������������
	NOUN_ATTR_STRENGTHEN_PROP, // ���Ե���
	NOUN_EVOLUTION_PROP, // ��������
	NOUN_HOT_PROP, // ��������
	NOUN_GOTO, // ǰ��
	NOUN_COMPLETED, // �����
	NOUN_BAG, // ����
    NOUN_DETAIL_PROP, // ��ϸ����
    NOUN_ATTR_HIT, // ����
	NOUN_DEL_FRIEND, //ɾ������
	NOUN_CULTURE, //����
};


inline std::string NounStr(const int iKey)
{
    return NOUN_CFG_MASTER->GetStr(iKey);
}

inline std::string AttrTypeStr(const int iAttrType)
{
    //�������Ͷ�����ClientSvrMsg�����Ͳ������˰�
    switch(iAttrType) {
    case 1:
        return NOUN_CFG_MASTER->GetStr(RUNSPEED);
    case 2:
        return NOUN_CFG_MASTER->GetStr(HP);
    case 3:
        return NOUN_CFG_MASTER->GetStr(DEF);
    case 4:
        return NOUN_CFG_MASTER->GetStr(MOVESPEED);
    case 5:
        return NOUN_CFG_MASTER->GetStr(MP);
    case 6:
        return NOUN_CFG_MASTER->GetStr(ATK);
    case 7:
        return NOUN_CFG_MASTER->GetStr(NOUN_ATTR_HIT);
    default:
        return "";
    }
    
}


//�������� ARMOR_TYPE
inline std::string ArmorTypeStr(const int iArmorType)
{
    switch(iArmorType) {
    case 0:
        return NOUN_CFG_MASTER->GetStr(NONE);
    case 1:
        return NOUN_CFG_MASTER->GetStr(HEARY_ARMOR_DEF_STR);
    case 2:
        return NOUN_CFG_MASTER->GetStr(LIGHT_ARMOR_DEF_STR);
    default:
        return "";
    }

}


//WeaponCfg AtkAttrType, 
inline std::string AtkAttrTypeStr(const int iAtkAttrType)
{
    //������Macros.proto��MAGIC_TYPE
    switch(iAtkAttrType) {
    case 1:
        return NOUN_CFG_MASTER->GetStr(STR_ICE); ///< ��
    case 2:
        return NOUN_CFG_MASTER->GetStr(STR_FLAME); ///< ��
    case 3:
        return NOUN_CFG_MASTER->GetStr(STR_POISON);  ///< ��
    case 4:
        return NOUN_CFG_MASTER->GetStr(STR_BIOTECH);  ///< ����
    case 5:
        return NOUN_CFG_MASTER->GetStr(STR_LIGHTING); ///< ����
    default:
        return "";
    }

}



#endif //__NOUN_HELPER_H__


