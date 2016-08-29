#ifndef __CONSTANT_H__
#define __CONSTANT_H__

#define MIN_INNER_CMD  5000
#define MAX_INNER_CMD  10000

#define HERO_INIT_LEVEL 1
#define MAX_EQUIP_POS 4 	//����װ����������?
#define MAX_WEAR_ARMOR_NUM 2 //����2������
#define MAX_ROLE_NUM 3 //��ཨ3����ɫ
#define MAX_NAME_HASH_KEY_LEN 27		//����hash key��󳤶�,��ʽΪ������.sXXX
#define MAX_HEROSKILL_CNT 5 	//�佫���ܸ���������
#define HERO_DEFAULT_SKILL_NUM 2 //�佫��Ĭ�ϼ��������������ʹ���
#define MAX_NAME_LEN 22		//������󳤶�
#define MAX_ACHIEVE_CNT	10	//�ɾ͵��������
#define MAX_PLAYERSKILL_CNT 30	//�������ܵ��������
#define MAX_SUBHERO_CNT 9	        //  ���������Я������
#define MAX_HEROONDUTY_CNT 8//�����佫���������
#define MAX_EQUIP_EXTATTR_CNT 5 //װ���������Ե��������
#define MAX_PRO_AI_PER_UNIT 11
#define MAX_PAS_AI_PER_UNIT 11
#define MAX_PAS_AI_ENTRIES 10000
#define MAX_PRO_AI_ENTRIES 10000
#define MAX_CELL_CNT 144 //���������ޣ���ҿ���������ֻ��(MAX_CELL_CNT-SYS_BAG_CELL_CNT)��
#define SYS_BAG_CELL_CNT 10 //����ϵͳʹ�õı���������,��ҿ������ø�������DEFAULT_CELL_CNT������SYS_BAG_CELL_CNT���ռ��������
#define DEFAULT_CELL_CNT 72 //Ĭ�ϱ����Ŀ�����,��ҿ������ǣ�DEFAULT_CELL_CNT - SYS_BAG_CELL_CNT����
#define MAX_WARPOINT_CNT 50//ÿ�Źؿ�ͼ�еĹؿ��������
#define MAX_WAR_MAP_CNT 500//�ؿ�ͼ���������
#define MAX_WARMAP_DROP_CNT 10
#define MAX_REFINE_LV 150 //���ǿ���ȼ�
#define MAX_PLAYER_LV 150 //��ɫ�ȼ�����
#define MAX_LOT_ITEM_NUM 10 //Ѱ�����⽱����Ʒ����
//����
#define MAX_MANUAL_VAL 100 //�������ֵ
#define MANUAL_REC_TIME (60*5) //�ָ�ʱ��
#define MANUAL_REC_POINT 1  //�ָ���
#define MAX_BUY_MANUAL_CNT 10

#define DEC_EQUIP_REFINE_LV 5 //���������ǵ���ǿ���ȼ��Ľ���ֵ
#define MAX_HOLE_CNT 5 //������
#define MAX_BUFF_COUNT 12
#define MAX_DEBUFFEFF_COUNT 12
#define MAX_DROP_ELEMENT 3//������Ʒ������

#define MAX_DAILYTASK_CNT 10

#define SECOND_PER_DAY (24*60*60)
#define MAX_GOLD 2000000000
#define MAX_OPEN_ID_LEN 64
#define MAX_SOURCE_LEN 64   //�ⲿ��Դ����
#define MAX_OPEN_KEY_LEN 512
#define MAX_PF_LEN 64

//////////////////////////////////////////////////////////////////////////ǹе
#define MAX_WEAPON_CNT 100	//���ǹе����

#define MAX_WARMAP_STAR 3

#define MAX_PACKAGE_AWARD_CNT 10

/////////////����/////////////
#define MAX_FRIEND_CNT 100 //���Ѹ�������
#define MAX_FRIEND_ADD_REQ_CNT 10 //��Ӻ�����������
#define MAX_SEND_MANUAL_CNT 10 //ÿ�������10������
#define MAX_USE_RECV_MANUAL_CNT 10 //ÿ�������ȡ10�κ����͵�����
#define MAX_PROPOSE_FRIEND_NUM 6 //�����Ƽ����һ���Ƽ�6��

enum enumSystemMaxData
{
    EMIN_VALID_UIN  =   10000,  ///< ��С��Чuin
};

enum enumUnitType
{
    EUNIT_TYPE_MIN = 0,
    EUNIT_SUBHERO   = EUNIT_TYPE_MIN + 0,          ///< �佫
    EUNIT_MONSTER       = EUNIT_TYPE_MIN + 1,          ///< ����
    EUNIT_EQUIPMENT = EUNIT_TYPE_MIN + 2, ///< װ��
    EUNIT_WEAPON = EUNIT_TYPE_MIN + 3, ///< ǹе
    EUNIT_MAIL = EUNIT_TYPE_MIN + 4,    ///< �ʼ�
    EUNIT_GUILD = EUNIT_TYPE_MIN + 5,   ///< ����
    EUNIT_TEAM = EUNIT_TYPE_MIN + 6,
    EUNIT_TYPE_MAX = EUNIT_TYPE_MIN + 7,
};

////////////////////////////////////////////////// ����
#define MAX_SUBHERO_SPIRIT_CELL_NUM 8
#define MAX_SPIRIT_CELL_BAG_NUM 20
#define MAX_SPIRIT_CELL_IN_SHOP 30
#define MAX_SPIRIT_HOLE_NUM 5
#define MAX_ATTR_TYPE_PER_SPIRIT 2

////////////////////////////////////////////////// ��������
#define MAX_PLAYER_SECRET_STORE_CELL_NUM 6

////////////////////////////////////////////////// Ѻ��
#define MAX_PLAYER_CARGO_ROB_TIME 2

////////////////////////////////////////////////// ���ϵͳ
#define MAX_TEAM_MATE_NUM 4

////////////////////////////////////////////////// ���˸���
#define MAX_MULTIINSTANCE_NUM 20
//////////////////////////////////////////////////  �û���״̬
#define MAX_PLAYER_LOCK_TYPE 10

////////////////////////////////////////////////// �û�ͨ�ؼ�¼
#define MAX_INSTANCE_RECORD_UIN_NUM 10

#define MAX_PLANT_FIELD_NUM 9
#define MAX_PLAYER_CREATION_FLAG 16

#define MAX_PLAYER_ONLINE (1000+100)

#define MAX_PACKAGE_ITEM 10

#define MAX_REG_PLAYER_CNT 25000

/////////////////////////////////////////////////////// ǩ���
#define MAX_DAILY_CHECK_IN_SLOT 32
#define MAX_DAILY_CHECK_IN_AWARD_SLOT 8

/////////////////////////////////////////////////////// ���ֻƽ����
#define MAX_SIGNIN_PKG_NUM 3

////////////////////////////////////////////////// �����
#define MAX_ACTIVITY_FETCH_NUM_PER_TURN 60
#define MAX_ACTIVITY_INDIANA_JONES_RECORD_NUM  3

///////////////////////////////////////////////////////////  �svr
#define MAX_ACTPLAYER_INFO_CELL 20
#define MAX_PLATVIP_INFO_ARR_LEN 2


#define MAX_TOKEN_LEN 256

#define MAX_CLAIM_ONLY_ACT_NUM 16

#define MAX_CONTINOUSDAY_LEN 8

#define MAX_DAILY_INVITE_ITEMS 4

///////////////////////////////////////////////////////////  ������
#define MAX_ARENA_FREE_NUM 10				//ÿ�վ�������Ѵ���
#define ARENA_WIN_ADD_SCORE 5				//��ʤ�ӵķ���
#define ARENA_LOSE_DEC_SCORE 2				//ʧ�ܿ۵ķ���
#define ARENA_STORE_MAX_ITEMS 6				//�������һ��̵����6����Ʒ
#define ARENA_STORE_REFRESH_GOLD 100		//ˢ�¶һ��̵�����Gold
#define MAX_ARENA_RECORD 20					//ս����¼���20��
#endif
