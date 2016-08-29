/* game constant value
* jacey
* 20150506
*/
#pragma once

#include "cocostudio/ActionTimeline/CSLoader.h"
#include "cocostudio/WidgetCallBackHandlerProtocol.h"
#include "cocostudio/WidgetReader/NodeReader/NodeReader.h"
#include "cocostudio/CCObjectExtensionData.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "EnvSet.h"
#include "Singleton.h"

typedef unsigned long long uint64;

const int g_TimeNodeInterval = 5 * 6;

#define USER_SERVER_NAME "USER_SERVER_NAME"
#define USER_SERVER_IP "USER_SERVER_IP"
#define SRV_LIST_URL "http://10.10.1.134/address.php"

#define REQUEST_NOTICE "Notice.csv"
#define REQUEST_ACTINFORM "ActInform.csv"
#define TILED_SIZE 20
#define TILED_FEET_SIZE 29
#define FORMATION_ROW_SPACE 1
#define NORMAL_SOLDIER_SPEED 50.0f 
#define POWER_RECOVERY_TIMES 5 * 60		// �����ָ����

#define POWER_RECOVERY_TIMES_PVP 60*5
#define  PVPPOWER_INIT_VALUE 50
#define  PVEPOWER_INIT_VALUE 50
#define  PVPPOWER_RESUMEVALUE 1
#define  PVEPOWER_RESUMEVALUE 1

#define PVP_ENERGY_VALUE  5			//pvp���ľ���
#define POWER_BUY_MAX 150            //���������ɹ���ʱ�����ֵ
#define PVP_ENERGY_TYPE 1001
#define PVE_POWER_TYPE 1002
#define ITEM_BUY_TYPE 10001 //������ʼ

#define MGAME_FILE_DECODE_KEY		"HappyMGame"
#define MGAME_FILE_SIGNATURE		"61Games-MGame"
#define TAG_GUIDELAYER				321654
#define TAG_GUIDELAYERCHILD1		1001
#define TAG_GUIDELAYERCHILD2		1002
#define TAG_GUIDELAYERCHILD3		1003
#define TAG_GUIDELAYERCHILD4		1004
const int DIS_SOLDIER_SOLDIER = TILED_FEET_SIZE;
const int DIS_OTHER_BUILD = TILED_FEET_SIZE * 2;
const int DIS_SRCTAR_BUILD = TILED_FEET_SIZE * 5;

#define PVP_SCORE_VALUE  200			//pvp�޵��߳���Ҫ����
#define AREA_NAME_COUNT (6)		//��������

#define DEFAULT_FONT     "Helvetica"

#define RICH_TEXT_ALL		 "<txt face='%s' size='%d' color='%s'>%s</txt>"		    //�������壬�����С��������ɫ�ĸ��ı�
//#define RICH_TEXT_FONT_COLOR "<txt face='%s' color='%s'>%s</txt>"					//�������壬������ɫ�ĸ��ı�
//#define RICH_TEXT_FONT_SIZE  "<txt face='%s' size='%d'>%s</txt>"					//�������壬�����С�ĸ��ı�
//#define RICH_TEXT_SIZE_COLOR "<txt size='%d' color='%s'>%s</txt>"					//���������С��������ɫ�ĸ��ı�
//#define RICH_TEXT_FONT		 "<txt face='%s'>%s</txt>"								//��������ĸ��ı�
//#define RICH_TEXT_SIZE		 "<txt size='%d'>%s</txt>"								//���������С�ĸ��ı�
//#define RICH_TEXT_COLOR		 "<txt color='%s'>%s</txt>"							    //����������ɫ�ĸ��ı�

namespace RichHelp
{
	struct pTextDefnition
	{
		std::string text;		//����
		std::string fntName;	//�������� eg: fonts/font.ttf
		std::string fntColor;	//������ɫ eg: #5a4eae
		int fntSize;			//�����С eg: 26

		pTextDefnition() :text(""), fntName("fonts/font.TTF"), fntColor("000000"), fntSize(26){}
	};
}

//
typedef void (CCObject::*SEL_CallFuncPage)(int pageIndex, int id, int index);
#define callfuncPage_selector(_SELECTOR) (SEL_CallFuncPage)(&_SELECTOR)

typedef void (CCObject::*SEL_CallFuncID)(int id);
#define callfuncID_selector(_SELECTOR) (SEL_CallFuncID)(&_SELECTOR)

//������Ϣ��pvp��Ϣ���鿴��ϸ��Ϣ��ģ���õ������ô˵�
namespace FriendSystem
{
	struct PlayerData
	{
		unsigned int m_uId;
		std::string m_sName;
		std::string m_iIcon;
		int m_iGroup;
		int m_iOrder;
		int m_iScore;
		float m_fWinRate;
		int m_iTime;
		int m_iLogoutTime;
		int m_iRoom;
		unsigned int m_uInviterUin;
		int m_iIndex;
		int m_iTitle;
		int m_iLevel;//�ɾ͵ȼ�
		int m_iVipLevel;
		bool m_bTPFriend;
		int m_iPlayerLevel;//��ҵȼ�
		int m_iExp;
		std::string m_sOpenId;
		std::string m_sThirdPartyIcon;	
		PlayerData() : m_uId(0), m_sName(""), m_iIcon("0"), m_iGroup(0),
			m_iOrder(0), m_iScore(0), m_fWinRate(0), m_iTime(0),
			m_iLogoutTime(0), m_iRoom(0), m_uInviterUin(0),
			m_iIndex(0), m_iTitle(0), m_iLevel(0), m_iVipLevel(0), 
			m_bTPFriend(false), m_sOpenId(""), m_sThirdPartyIcon(""), m_iPlayerLevel(1), m_iExp(0){}
	};
};

enum eChargeMode
{
	CHARGE_TYPE_INVALID = 0,
	CHARGE_TYPE_DAY,//30����ÿ�����500��ʯ,��ι����ӳ�ʱ��
	CHARGE_TYPE_POWER, //ǰ5�ι���ÿ������10����������
	CHARGE_TYPE_FLAG, //�״ι������ͱ��λ
	CHARGE_TYPE_P_POWER, //�״ι������;���
	CHARGE_TYPE_COIN, //�״ι�����
	CHARGE_TYPE_MAX //���
};

enum ERRORCODE
{
	OK                          = 0,   //-- ����

	//--��½���
	ACCOUNT_LOGIN_EXPECTION     = 1001, //-- ��½�쳣
	ACCOUNT_AUTH_FAIL           = 1002, //-- ��������֤ʧ��
	ACCOUNT_NO_LOGIN            = 1003, //-- �޵�½����

	//--Player���
	POWER_NOT_ENOUGH            = 2001, //-- ��������
	TIME_COUNT_ERROR            = 2002, //-- dao
	NAME_EXIST_ERROR			= 2003, //-- �����Ѵ���
	NAME_NOT_CHANGE				= 2004, //-- �ǳ�û�б仯

	//cdk ���
	GIFT_TYPE_IS_USED			= 2008,	//-- ��������Ѿ�ʹ�ù���
	GIFT_CODE_NOE_EXIST			= 2009, //-- �����벻����
	GIFT_TYPE_NOE_EXIST			= 2010, //-- ���������Ͳ�����
	CODE_IS_USED				= 2011,	//-- �������Ѿ���ʹ����
	PARAM_ERROR					= 2012,	//-- ��������
	NAME_ERROR					= 2014,	//-- ���ִ��ڷǷ��ַ�

	//--PVP���
	PVP_NOT_MATCH               = 3001, //-- pvpƥ��ʧ��
	ENERGY_NOT_ENOUGH           = 3002, //-- ��������	
	PVP_NOT_TIME				= 3003, //-- //����ʱ�����
	PVP_NOT_NET					= 3005, //-- //����Ͽ�
	NOT_MATCH_STATE				= 3006, //ֹͣƥ������룬������ƥ��״̬
	NOT_PVP_SCORE				= 3007, //�޵��߳� ���ֲ���


	//--��Ʒ���
	GIFT_NOT_EXIST 		        = 4001, //-- ����벻����
	ACHIEVEMENT_REWARD_NOT_EXIST = 4003, //-- �ɾͽ���������
	LEVEL_OR_POINT_ERROR         = 4004, //-- �ɾ͵������ߵȾõȼ�����
	RECEIVE_ERROR        		 = 4005, //-- �ɾͽ����Ѿ���ȡ���˲����ظ���ȡ
	ACHIEVEMENTS_NOT_COMPLEMENT = 4006,  //-- �ɾ�û�����
	//--�ؿ����
	STAGE_STAR_COUNT_ERROR =	5001, //-- �ؿ�������������

	//-- �������
	ITEM_COUNT_ERROR    	= 6001, //--������������
	ITEM_DIAMAON_NOT_ENOUGH_ERROR    	= 6002, //--��ʯ����

	//����
	ACTIVITY_RECEIVE_ERROR  = 6101, // ���������ȡ
	ACTIVITY_NOT_RECEIVE_ROCODE_ERROR  = 6102, //  û�п���ȡ��¼��
	ACTIVITY_NOT_START_ERROR  = 6103 ,	//-- ���δ��ʼ
	ACTIVITY_END_ERROR  = 6104, //-- ��Ѿ�������
};

enum eCampType
 {
	ECAMP_TYPE_Neutral = 0,		//����	����
	ECAMP_TYPE_Player,			//���ƶ�  ����
	ECAMP_TYPE_AI,				//AI	����
	ECAMP_TYPE_AI2,				//AI	����
	ECAMP_TYPE_AI3,				//AI	����
	ECAMP_TYPE_Player1,			//PVP	����
	ECAMP_TYPE_Player2,			//PVP	����
	ECAMP_TYPE_Player3,			//PVP	����
	ECAMP_TYPE_Player4,			//PVP	���� 
	ECAMP_TYPE_Creeps,			//��Ұ����
	ECAMP_TYPE_Invalid			//��Ч
};

enum eExpressType
{
	EXP_TYPE_Neutral = 0,		//����	����
	EXP_TYPE_Player,			//���ƶ�  ����
	EXP_TYPE_AI,				//AI	����
	EXP_TYPE_AI2,				//AI	����
	EXP_TYPE_AI3,				//AI	����
	EXP_TYPE_Player1,			//PVP	����
	EXP_TYPE_Player2,			//PVP	����
	EXP_TYPE_Player3,			//PVP	����
	EXP_TYPE_Player4,			//PVP	���� 
	EXP_TYPE_LaJiao,			//��Ұ����-���� 9 
	EXP_TYPE_DaCong,			//��Ұ����-��� 10
	EXP_TYPE_JiDan,				//��Ұ����-���� 11
	EXP_TYPE_NaiLao,			//��Ұ����-����
	EXP_TYPE_SuanTou,			//��Ұ����-��ͷ
	EXP_TYPE_XiaMi1,			//��Ұ����-Ϻ�� 
	EXP_TYPE_Invalid			//��Ч
};

//��������
enum eBulidType
{
	EBULID_TYPE_BARRACK = 1,	//��Ӫ
	EBULID_TYPE_TURRET,			//������
	EBULID_TYPE_INSISTUTE,		//�о���
	EBULID_TYPE_CREEPS,			// ��Ұ����
	EBUILD_TYPE_STABLES,		//���
};

//��������
enum ePropType
{
	PROP_TYPE_EXCHANGE_B = 1,//ת��Ϊ��Ӫ
	PROP_TYPE_EXCHANGE_I,	// ת��Ϊ�о���
	PROP_TYPE_EXCHANGE_T,	// ת��Ϊ����
	PROP_TYPE_ADDPEOPEL,//������
	PROP_TYPE_TIME,//�Ʊ�
	PROP_TYPE_COOKING,//�ٻ���ʦ
	PROP_TYPE_FREEZE,//����
	PROP_TYPE_EXCHANGE,//����ת����
	PROP_TYPE_INVISIBILITY_CLOAK,//���ζ���
	PROP_TYPE_CHAIN_OIL,//�ع���
	PROP_TYPE_NULL,
};

//������������
enum eBuildAni
{
	BUILD_ANI_STAND = 0,			//վ��
	BUILD_ANI_BEHURT,				//�ܻ�
	BUILD_ANI_OUTPUT_STAR,			//������ʼ_��
	BUILD_ANI_OUTPUTING,			//����������_��
	BUILD_ANI_OUTPUT_END,			//��������_��
	BUILD_ANI_OUTPUT_STAR_RIGHT,	//������ʼ_��
	BUILD_ANI_OUTPUTING_RIGHT,		//����������_��
	BUILD_ANI_OUTPUT_END_RIGHT,		//��������_��
	//BUILD_ANI_INSISTUTE_STAND,		//�о�������
	//BUILD_ANI_TURRET_STAND,			//��̨����
	BUILD_ANI_TURRET_ATTACK,		//��̨����
	BUILD_ANI_UPGRADE,
	BUILD_ANI_RANGE_BORN,
	BUILD_ANI_RANGEING,
	BUILD_ANI_EXCHANGE,
	BUILD_ANI_EXCHANGE1,
	BUILD_ANI_EXCHANGE2,
	BUILD_ANI_EXCHANGE3,
	BUILD_ANI_EXCHANGE_END,
	BUILD_ANI_SMALL_CRY,
	BUILD_ANI_BIG_CRY,
	BUILD_ANI_WIN,					//ʤ��
	BUILD_ANI_NONE
};

// ͨ�ع���
enum eBalanceTag
{
	BALANCE_TAG_INVALID = 0,
	BALANCE_TAG_DESTORY,	// ����ģʽ
	BALANCE_TAG_OCCUPY,		// ռ��ģʽ
	BALANCE_TAG_RESOURCE,	// ��Դģʽ
	BALANCE_TAG_SURVIVAL,	// ����ģʽ
	BALANCE_TAG_GUARD,		// �ػ�ģʽ
};

enum eBalanceScore
{
	SCORE_TIME = 1,
	SCORE_PEOPLE,
	SCORE_BOMB,
	SCORE_FREEZE,
	SCORE_EXCHANGE,
	SCORE_INVISIBILITY,
	SCORE_SPEED,
};

// ���ǹ���
enum eStarTag
{
	STAR_TAG_INVALID = 0,
	STAR_TAG_SUCCESS_IN_N,
	STAR_TAG_ENE_BUILD_L_N,	// �з���Ӫռ�ݵĽ�������
	STAR_TAG_BUILD_NODESTROY,// �з���Ӫ��δռ��ָ���Ľ���
	STAR_TAG_NEUTRAL_N,		// ʣ��N��������Ӫ������
	STAR_TAG_NO_PEOP_MN,	// �κ�һ���������޲�����N
	STAR_TAG_TAR_BUILD,		// ʤ��ʱ��ռ��ָ���Ľ���
	STAR_TAG_MORE_ENE_N,	// �������������з�����N
	STAR_TAG_PEOPLE_L_N,	// ������������N
	STAR_TAG_PEOPLE_M_N,	// ����������N
	STAR_ENE_PEOPLE_L_N,	// �з���������N
	STAR_ENE_PEOPLE_M_N,	// �з�������N
	STAR_TAG_BUILD_LOST,	// ������ʼ�Ľ�����ʧ����
	STAR_TAG_MY_BUILD_M_N,	// ����ӵ�еĽ�������
	STAR_TAG_SUCC_LAST_MN,	// ʤ��ʱ����ʣ��ı���
	STAR_TAG_MONSTER_TIMES,	// ��Ұ����
	STAR_TAG_BUILD_UPGRADE,	// ��������ʼ����
	STAR_TAG_OCampNoDestroy_GetTar,		// ���ǽ������ı���Ӫʱ��ռ����Ľ���
	STAR_TAG_MORALE_MN,		// ʿ������N
};

// ��Ϸ���ı�
enum eTextTagBase
{
	TEXT_BALANCE_MODE = 100,	// ͨ��ģʽ
	TEXT_BALANCE_STAR = 200,	// ����
	TEXT_GUARD = 300,			// ����ָ��
	TEXT_COMM_TIPS = 400,		// ͨ����ʾ�ı�
	TEXT_ACHIEVE = 600,       //�ɾ�
	TEXT_ACT        = 700,      //�
	TEXT_LOGIN		= 800,     //��¼
	TEXT_RENAME		= 820,     //�޸�����
	TEXT_BUY_ITEM = 900,     //����
	TEXT_PVP	= 1000,		//pvp����ı�
	TEXT_BATTLE = 2000,		//ս������ı�
	TEXT_CHAT = 7000, //����
	TEXT_FRIEND = 8000,		//��������ı�
	TEXT_POWER_GIFT = 9000,		//���������ı�
	TEXT_BUFF	= 10000, //BUFF��ʾ
	TEXT_SAVEDATA = 12000, //�浵��ʾ
	TEXT_RANKING = 13000, // ���а�
	TEXT_VIP     = 15000,//VIP
	TEXT_BADGE = 20000, // ����
	TEXT_SHOP = 21100,//shop
	TEXT_BTH =	22100,//ѡ�ؽ���
	TEXT_GET_HONOR = 22104, // ��ȡ������ʾ
};

enum eTextTagStar
{
	TEXT_STAR_SUCCESS = 250,
	TEXT_STAR_FAILED = TEXT_STAR_SUCCESS + 1,
	TEXT_STAR_SuccessN = TEXT_BALANCE_STAR + 1,		// ��N����ȡ��ʤ��
	TEXT_STAR_EnemyBuildsLN,						// �������ռ��Ľ�������������N
	TEXT_STAR_Build_NoDestroy,					
	TEXT_STAR_EneBuild_LastLN,						// ʣ�಻����%d����˾����ȡ��ʤ��
	TEXT_STAR_OwnPeople_LN,
	TEXT_STAR_GetTar_Builds,					// ռ��ȫ����ǵĽ���
	TEXT_STAR_People_M_ENE_N,							// �����������ڵз�%d
	TEXT_STAR_People_LN,							// ��������������%d
	TEXT_STAR_People_MN,							// ������������%d
	TEXT_STAR_EnePeo_LN,							// �з���������N
	TEXT_STAR_EnePeo_MN,							// �з�������N
	TEXT_STAR_BuildLost_LN,							// ������ʼ�Ľ�����ʧ����
	TEXT_STAR_OwnBuild_MN,							// ����ӵ�еĽ�������
	TEXT_STAR_SuccOwnPeop_MN,						// ʤ��ʱ����ʣ��ı���
	TEXT_STAR_KillMonster_MN,						// ��Ұ����
	TEXT_STAR_NoInitBuild_Upgrade,					// ��������ʼ����
	TEXT_STAR_NoDestroyGetTar,						// ָ������Ӫ����ʧָ���Ľ���ͬʱָ���Ľ��������ռ��
	TEXT_STAR_HAS_N_BUILD,							// ͬʱӵ��N������
	TEXT_STAR_MORALE_MN,							// ʿ������N
	TEXT_STAR_NO,									// û��������������һ����ʾ��

};

enum eVipText
{
	TEXT_VIP1 = TEXT_VIP +1,
	TEXT_VIP2 ,
	TEXT_VIP3,
	TEXT_VIP4,
	TEXT_VIP5,
	TEXT_VIP6,
	TEXT_VIP7,
	TEXT_VIP8,
	TEXT_VIP9,
	TEXT_VIP10,
	TEXT_VIP11,
	TEXT_VIP12,
	TEXT_VIP13,
	TEXT_VIP14,
	TEXT_VIP15,
	TEXT_VIP_TIME1,
	TEXT_VIP_TIME2 ,
	TEXT_VIP_HONOR,//����
	TEXT_VIP1_TIPS,  //vip1��ʾ
	TEXT_VIP2_TIPS,
	TEXT_VIP3_TIPS,
	TEXT_VIP22,
	TEXT_VIP23,
	TEXT_VIP24,
	TEXT_VIP25,
};

enum eCommTips
{
	TEXT_COMMTIPS_TIPS = TEXT_COMM_TIPS,
	TEXT_COMMTIPS_WORLD,	
	TEXT_COMMTIPS_WORLD2,
	TEXT_COMMTIPS_CHECKPT,			// ��һ�ؿ�δ����
	TEXT_COMMTIPS_CP_STARS,			// ��һ�ؿ�����������
	TEXT_COMMTIPS_NO_CHARGE,		// ��ֵ����δ����
	TEXT_COMMTIPS_NO_TOOLS,			// ���߹���δ����
	TEXT_COMMTIPS_NO_POWERS,		// ��������
	TEXT_COMMTIPS_STATE2_1,
	TEXT_COMMTIPS_STATE2_2,
	TEXT_COMMTIPS_PROP_TEXT1,			//������ʾ
	TEXT_COMMTIPS_PROP_HEXT,
	TEXT_COMMTIPS_PROP_CONTENT,
	TEXT_COMMTIPS_SHOP_TEXT1,
	TEXT_COMMTIPS_PROP_AIM_TEXT,			//����û��Ŀ��
	TEXT_COMMTIPS_END,						//�����쳣��ʾ
	TEXT_COMMTIPS_PROP_DATA,					//��������ʹ�õ�����ʾ
	TEXT_COMMTIPS_AWARD,				//��ϲ�����
	TEXT_COMMITPS_CHARGMAX,         //�̴���ֵ������
	TEXT_COMMITPS_ReLoginOk,           //����OK
	TEXT_COMMTIPS_GLYPGNOOPEN,			//������Ҫ��ҵȼ�15��
	TEXT_COMMTIPS_GUIDENODEERROR,      //���ֽڵ������
	TEXT_SERVER_WILL_STOP,			//�����������ر�
	TEXT_COMMTIPS_GLYPH_TIP,			//ս������ķ�����ʾ��
	TEXT_GUIDE_LEVEL_TIP = 425,			//��ѧ�ؿ�������ʾ
};

enum eAchieveTips
{
	TEXT_ACHIEVE_GETFAILUES = TEXT_ACHIEVE,  //��ȡ�ɾ�ʧ��
	TEXT_ACHIEVE_GETPOINT,								//���%d��ɾ͵���
	TEXT_ACHIEVE_CANGET,
	TEXT_ACHIEVE_GETED,
	TEXT_ACHIEVE_CANNOTGET,
	TEXT_ACHIEVE_POINT_1,
	TEXT_ACHIEVE_POINT_2,
	TEXT_ACHIEVE_STAGE_NOOPEN,
	TEXT_ACHIEVE_LEVEL_NOOPEN,
	TEXT_ACHIEVE_COMPLENT_PLAYER,
};

enum eActText
{
	TEXT_ACT_GOLD = TEXT_ACT,		//���
	TEXT_ACT_TILI,								//����
	TEXT_ACT_JINGLI,							//����
	TEXT_ACT_NOMAIL,                      //��ǰû���յ��ʼ�!
	TEXT_ACT_OPEN_TIME,                      //�����ʱ��
	TEXT_ACT_ROOM_TICKET,					//  roomticket
	TEXT_ACT_OPEN_FOREVER,                      //���ÿ���
};

enum eLoginTips
{
	TEXT_LOGIN_BEGIN = TEXT_LOGIN,
	TEXT_LOGIN_AUTO,	//�Զ���¼��ʾ
	TEXT_SERVER_FORCE,//������ǿ���˳�
	TEXT_LOGIN_QUEUE,//��¼�Ŷ�
	TEXT_NETWORK_ERROR,//�����쳣
	TEXT_FORCE_UPDATE,//ǿ��
	TEXT_LOGIN_WAIT,
	TEXT_NET_ERROR_TIP,//���������쳣���������������
	TEXT_CUR_VER,
	TEXT_LOGIN_BY_WX,
	TEXT_LOGIN_BY_QQ,
	TEXT_LOGIN_BY_YK,
	TEXT_SRV_CONN_FAIL,	//����������ʧ��
};

enum ePvpTips
{
	TEXT_PVP_2V2_LOCK = TEXT_PVP,
	TEXT_PVP_NO_CONN,
	TEXT_PVP_NO_RANK,//δ�ϰ�
	TEXT_PVP_MACTH,//ƥ����
	TEXT_PVP_SCORE,//���ֲ���
	TEXT_SCORE_NEED,//��λ����
	TEXT_RANK_GROUP_1 = 1011,
	TEXT_RANK_GROUP_2,
	TEXT_RANK_GROUP_3,
	TEXT_RANK_GROUP_4,
	TEXT_RANK_GROUP_5,
	TEXT_PVP_ROOM_1,
	TEXT_PVP_ROOM_2,
	TEXT_PVP_ROOM_3,
	TEXT_PVP_ROOM_4,
	TEXT_PVP_ROOM_5,
	TEXT_PVP_ROOM_ACCEPT_CONFIRM,	//����ȷ�϶�ս����
	TEXT_PVP_ROOM_REJECT_CONFIRM,	//����ȷ�϶�ս�ܾ�
	TEXT_PVP_ROOM_6,
	TEXT_PVP_ROOM_7,
	TEXT_PVP_ROOM_8,
	TEXT_PVP_ROOM_9,
	TEXT_PVP_ROOM_10,
	TEXT_PVP_ROOM_11,
	TEXT_PVP_ROOM_12,
	TEXT_PVP_NOITEM_TIME,//�޵���ʱ��δ��
	TEXT_PVP_BTN_TEXT_1, //ƥ�䰴ť����
	TEXT_PVP_BTN_TEXT_2,//���䰴ť����
	TEXT_PVP_BTN_TEXT_3,//�޵��߰�ť����
	TEXT_PVP_LIMIT_ROUND10,
	TEXT_PVP_PROTECT_ROUND10,
	TEXT_PVP_ROOM_13 = 252,
	TEXT_PVP_TIPS_1 = 1051,
	TEXT_PVP_TIPS_2,
	TEXT_PVP_TIPS_3,
	TEXT_PVP_TIPS_4,
	TEXT_PVP_TIPS_5,
	TEXT_PVP_TIPS_6 = 3002,
};

enum eRenameTips
{
	TEXT_RENAME_BEGIN = TEXT_RENAME,
	TEXT_RENAME_SPEND,//�����ֻ���
	TEXT_RENAME_FAILED,//��ʯ����
	TEXT_RENAME_EXIST,//����
	TEXT_NOT_CHANGE,//����û�ı�
	TEXT_RENAME_LENGHT,//���ֳ�����ʾ
	TEXT_RENAME_FREE,//���������
	TEXT_RENAME_ERROR,//���ִ��ڷǷ��ַ�
	TEXT_NAMEING_TIP1,
	TEXT_NAMEING_TIP2,
};

enum eBTHTips
{
	TEXT_BTH_BEGIN = TEXT_BTH,
	TEXT_BTH_MISSION,//����ͨ����ʾ
	TEXT_BTH_STAR,//���ǲ�������ؿ���ʾ
	TEXT_BTH_TITLE_BASE = 22104,	//ͨ�ؽ�������
	TEXT_BTH_TITLE_PERFECT = 22105,	//���ǽ�������
	TEXT_BTH_BEYOUD = 22108,		//��Խȫ��%d%%���
	TEXT_BTH_BEYOUD_FIFTH,			//������Խ��%d��
	TEXT_BTH_TOP,					//ȫ����߼�¼
	TEXT_BTH_TOP_CONTENT,			//Ӣ�������%s����ʳð�յ�%d�µ�%d��ˢ������߷��������Ϊ������������
	TEXT_BTH_TITLE_SKIP = 22112,    //���ش�������
	TEXT_BTH_SKIP_SUCCESS = 22113   //���سɹ�
};

enum eSpecialTips
{
	TEXT_WINNER_SUCCESS = 841,//��ȡ��ʤ�����ɹ�
	TEXT_WINNER_FAIL,//��ȡ��ʤʧ��
	TEXT_WINNER_CONFIRM,
	TEXT_WINNER_ACTTIPS = 846 ,//�δ����

	TEXT_ACTTIPS_HOPE = 847,  //���δ���ţ������ڴ���
	TEXT_WINNER_CONTENT_1 = 848,  //��ʤ30����������1
	TEXT_WINNER_CONTENT_2 = 849,  //��ʤ30����������
	TEXT_WINNER_COMPLENT = 850,//�����������
	TEXT_WINNER_NOCOMPLENT = 851,//��������δ���

	TEXT_POWER_FULL_TIPS = 881,//��������
	TEXT_POWER_NOT_FULL = 882,//��������
	TEXT_ENERGY_FULL_TIPS = 883,//��������
	TEXT_ENERGY_NOT_FULL = 884, //��������

	TEXT_CHAT_NULL = 7002,
	TEXT_CHAT_LEN,
	TEXT_CHAT_INVITE,
	TEXT_CHAT_LIMIT,
	TEXT_CHAT_EXPIRED,
	TEXT_CHAT_LEVEL,
	TEXT_ROOM_LEVEL,//���뷿������
	TEXT_ROOM_LEVEL1,//���뷿������
	TEXT_CHAT_FREE,
	TEXT_CHAT_COST,
	TEXT_CHAT_FAIL,
};

enum eBuyItemTips
{
	TEXT_BUY_ITEM_TILI = TEXT_BUY_ITEM,
	TEXT_BUY_ITEM_TILI_SURE,
	TEXT_BUY_ITEM__JINGLI,
	TEXT_BUY_ITEM_JINGLI_SURE,
	TEXT_BUY_ITEM_NO_EN,
	TEXT_BUY_ITEM_CLOSE_PVP,
	TEXT_BUY_ITEM_SUCCESS,
	TEXT_BUY_PVP_DEAD,
	TEXT_BUY_POWER_MAX,
	TEXT_BUY_DIAMOND_COUNT,  //%d��ʯ
	TEXT_BUY_ITEM_TITLE,//��ܰ��ʾ
	TEXT_BUY_ITEM_USE,//ʹ��˵��
	TEXT_BUY_NEWERGIFT_TITLE,	//���������������
	TEXT_BUY_NEWERGIFT_SURE,	//���������������
	TEXT_BUY_ITEM_QIUCK_CLOSE_PVP,	//���������뿪pvp
	TEXT_EXIT_PLAYBACK,			//ȷ���˳��ۿ�ս���ط���
};

//ս����ʾ
enum eBattleTips
{
	TEXT_BATTLE_POPULATION = TEXT_BATTLE,	//������������
	TEXT_BATTLE_SPEED_PRODUCE,				//�����ٶ�����
	TEXT_BATTLE_DEFENSE,					//��������
	TEXT_BATTLE_SPEED_ATTACK,				//�����ٶ�����
	TEXT_BATTLE_ATTACK_ALL,					//ȫ�幥������
	TEXT_BATTLE_DEFENSE_ALL,				//ȫ���������
	TEXT_BATTLE_SPEED_MOVE,					//�ƶ��ٶ�����
	TEXT_BATTLE_PEOPLE_FILL,				//����ȫ��
};
 
enum eBuffTips
{
	TEXT_BUFF_DESC = TEXT_BUFF,
	TEXT_BUFF_LAJIAO,
	TEXT_BUFF_DACONT,
	TEXT_BUFF_JIDAN,
	TEXT_BUFF_NAILAO,
	TEXT_BUFF_SUANTOU,
	TEXT_BUFF_XIAMI, 
	TEXT_BUFF_ADDATTACK,
	TEXT_BUFF_ADDDEFENCE,
	TEXT_BUFF_FILLPEOPLE,
	TEXT_BUFF_ATTACKSPEED,
	TEXT_BUFF_PRODUCTSPEED,
	TEXT_BUFF_MOVESPEED, 
};

enum eGetSaveDataTips
{
	TEXT_GettingSaveData = TEXT_SAVEDATA,
	TEXT_GetSaveDateOver,
	TEXT_AskGetSaveData,
	TEXT_CancelGetDesc,
	TEXT_EnterGameDesc,
};
//�������
enum eFriendText
{
	TEXT_FRIEND_MAX = TEXT_FRIEND,			//������������
	TEXT_FRIEND_IS_FRIEND,					//�����Ѿ��Ǻû�����
	TEXT_FRIEND_FIND_NONE,					//δ�ҵ����
	TEXT_FRIEND_DELETE,						//ɾ��ȷ��
	TEXT_FRIEND_RECENTLY,					//���
	TEXT_FRIEND_HOUR,						//%dСʱǰ
	TEXT_FRIEND_DAY,						//%d��ǰ
	TEXT_FRIEND_MONTH,						//%d����ǰ
	TEXT_FRIEND_LONGTIME,					//�ܾúܾ���ǰ
	TEXT_FRIEND_CHANGE_HEAD,				//����ͷ��
	TEXT_FRIEND_ONLINE,						//����
};


// ���а�
enum eRankingText
{
	TEXT_RANKING_START = TEXT_RANKING,
	TEXT_RANKING_GROUP_1,					// ѧͽ
	TEXT_RANKING_GROUP_2,					// ����
	TEXT_RANKING_GROUP_3,					// ����
	TEXT_RANKING_GROUP_4,					// ����
	TEXT_RANKING_RANKING,					// ����
	TEXT_RANKING_PLAYER,					// ���
	TEXT_RANKING_WINRATE,					// ʤ��
	TEXT_RANKING_POINT,						// ����
	TEXT_RANKING_ROUNDCOUNT,				// ����
	TEXT_RANKING_RESULT_WIN,				// ����ں�%s�ı����л�ʤ
	TEXT_RANKING_RESULT_LOST,				// ����ں�%s�ı�����ʧ��
	TEXT_RANKING_GAP,
	TEXT_RANKING_RESULT_GAINPOINT,			// �ջ�%d����
	TEXT_RANKING_RESULT_LOSTPOINT,			// �۳�%d����
	TEXT_RANKING_RESULT_GAINSCORE,			// ���%d����
	TEXT_RANKING_SCORE,						// ����
};


// ����
enum eBadgeText
{
	TEXT_BADGE_START = TEXT_BADGE,
	TEXT_BADGE_OBTAINTIME,					// XX��XX��XX�ջ��
	TEXT_BADGE_NOTOBTAIN,					// δ���
	TEXT_BADGE_NOTACTIVE,					// δ����
	TEXT_BADGE_DRESSSUCCESS,				// ����ɹ�
};

//������
enum eActivationText
{
	TEXT_ACTIVATION_NOTEXIT = 2009,				//�����벻����,
	TEXT_ACTIVATION_TYPE_NOTEXIT,				//���������Ͳ�����,
	TEXT_ACTIVATION_USED,						//�������Ѿ���ʹ����,
	TEXT_ACTIVATION_PARAM_ERROR,				//��������,
	TEXT_ACTIVATION_GIFTTYPE_USED,				//��������Ѿ�ʹ�ù���,
	TEXT_ACTIVATION_UNINPUT,					// δ���뼤����,
	TEXT_ACTIVATION_INPUT_NOTENOUGH,			//���뼤���볤�Ȳ���,
	TEXT_ACTIVATION_EMAIL_SUCCESS,				//�һ��ɹ�������ʼ���ȡ����,
};

//ҳ������
enum ePageType
{
	MSG_FUC_BEGIN = 0,

	MSG_FUC_SCENCE_BEGIN,
	MSG_FUC_LOGO,
	MSG_FUC_UPDATE,		//����
	MSG_FUC_LOGIN,
	MSG_FUC_MAIN_LEVEL,		//��ؿ�ѡ��
	MSG_FUC_MINI_LEVEL,		//С�ؿ�ѡ��
	MSG_FUC_STORE,			//�̵�
	MSG_FUC_PLAYING,		//ս����
	MSG_FUC_SCENCE_END,
	//
	MSG_FUC_WIN_FAIL = 110,		//ʧ�ܵ���
	MSG_FUC_WIN_RESULT = 111,		//����ѡ��
	MSG_FUC_PVP_RESULT = 112, //pvp����
	MSG_FUC_WIN_LEVEL = 113,		//�ؿ�ѡ��
	MSG_FUC_WIN_PROPNOSET = 114,		//δѡ�������ʾ

	MSG_FUC_WIN_PAUSE = 115,		//��ͣ
	MSG_FUC_WIN_GENERL = 116,		//ͨ��
	MSG_FUC_WIN_SHOP = 117,		//�̵�
	MSG_FUC_WIN_SET = 118,		//����
	MSG_FUC_WIN_PVP = 119,		//pvp
	MSG_FUC_WIN_PVP_WATCH = 120,		//pvp watch
	MSG_FUC_WIN_PVP_ROOM_BACK =121,		//pvp ROOM_BACK
	MSG_FUC_ACHIEVE =122,    //�ɾ�
	MSG_FUC_WINNER = 123,    //����֮·	
	MSG_FUC_WIN_PRO = 124,	//��������	
	MSG_FUC_ACT = 125,           //�
	MSG_FUC_ACT_LOGIN = 126,   //ǩ����ʾ
	MSG_FUC_RANKING = 127,	// ���а�
	MSG_FUC_GIFTDIALOG = 128, //�������
	MSG_FUC_FAIL_PROMPT = 129, // pvpʧ����ʾ��

	MSG_FUC_SET_NAME = 130,//��������
	MSG_FUC_CONNECTING = 131, // �����нڵ�
	MSG_FUC_BACKPACK = 132, //���˸���
	MSG_FUC_DAYTASK = 133, // ÿ������
	MSG_FUC_POWER_GIFT = 134, //���������
	MSG_FUC_NEWER_GIFT = 135, //������� 

	MSG_FUC_CHATMSG = 136,//������Ϣ���
	MSG_FUC_FRIEND = 137,		//���ѽ���
	MSG_FUC_SET_HEAD = 138,	//����ͷ��
	MSG_FUC_PVPROOM = 139,	//PVP����
	MSG_FUC_SAVEDATA = 140,   //��ȡ�浵

	MSG_FUC_NEWER_BIBLE = 141,//���ֱ���
	MSG_FUC_PVP_MATCH_LOADING = 142, // PVPƥ����

	MSG_FUC_KICKPLAYER = 143,
	MSG_FUC_CRAZYWEEK = 144, //��ĩ��
	MSG_FUC_HONOREXCHANGE = 145, // ������һ�
	MSG_FUC_FIVE_STAR = 146,//���Ǻ���
	MSG_FUC_CUSTOMER_SERVER = 147,//�ͷ�����
	MSG_FUC_INVITECODE = 148,//������

	MSG_FUC_MAIL = 149, //�ʼ�

	MSG_FUC_GW_SELECTAREA = 150,	//������ѡ��
	MSG_FUC_GW_AUDITION = 151,	//��������ѡ
	MSG_FUC_GW_FINAL = 152,		//������ʳ��

	MSG_FUC_ACTINFORM = 153, //�����

	MSG_FUC_VIP = 154,	//VIP
	MSG_FUC_VIP_TIME = 155,	//VIP����
	MSG_FUC_PLAYER_UPGRADE = 156,//�������
	MSG_FUC_GLYPH = 157, //����
	MSG_FUC_RANKSESSION_UPGRADE = 158, //PVP�����λ����


	MSG_FUC_FREEDIAMOND = 159,//�^��ӰƬ

	MSG_FUC_REDPACKET = 160,//����������ť
	MSG_FUC_CHRISTMAS = 161,// ʥ���
	MSG_FUC_RELOGIN = 162,//����
	MSG_FUC_CHAGE_SERVER = 164, //�������б�
	MSG_FUC_FIRST_CHARGE = 165,  // �׳����
	MSG_FUC_BADGE = 166,  // ����
	MSG_FUC_TOTAL_CHARGE = 167,  //�ۼƳ�

	MSG_FUC_INFO = 168,//������Ϣ���
	MSG_FUC_PVP_HELP = 169,

	MSG_FUC_LUCKY_MONEY = 170, // �����
	MSG_FUC_VALENTINE =171,//���������˽ڰ�ť	
	MSG_FUC_WEIXINSHARE =172,//΢�ŷ���
	MSG_FUC_SRVDOWN_NOTICE = 173,
	MSG_FUC_RESULT_BOX = 174, //����򿪺���ʾ��Ʒ
	MSG_FUC_NAMEING_DIALOG = 175, //ȡ��

	MSG_FUC_DAILY_MISSION = 176,//������ÿ������
	MSG_FUC_INSTITUTE_FREE = 178, // �о�Ժ��ʾ
	MSG_FUC_END,
	
	//add by weihua luawindow message
	MSG_FUC_LUA_WIN  = 1000,
	MSG_FUC_TOP_RANK = 10000,

};
//���������������key
enum eTextTagGuide
{
	TEXT_GUIDE_0_0 = 300, //��һ�����ֱ�ʾ�ڼ������񣬵ڶ������ֱ�ʾ����ĵڼ�����
	TEXT_GUIDE_0_1,
	TEXT_GUIDE_1_0,
};


enum eMusicType
{
	SOUND_SOLDIER_OUT = -200,		//��������ָ��
	SOUND_BUILD_LEVELUP = -199,		//��������
	SOUND_BUILD_CHANGE_CAMP = -198,	//����������Ӫ
	SOUND_BUILD_ATTACK = -197,		//��������
	SOUND_BUILD_SOLDIER_DIE = -196,	//��������С������
	SOUND_SOLDIER_ATTACK = -195,	//С��Χ������
	SOUND_BUILD_CHANGE_TYPE = -194,	//������������
	SOUND_CLICK_BACK = -193,		//���ذ�ť
	//SOUND_CLICK_BIG_GATE = -194,	//������
	//SOUND_LICK_SMALL_GATE = -193,	//���С��
	SOUND_CLICK_BATTLE = -192,		//�������(ս��)
	SOUND_CLICK_COMMON = -191,		//��ť����, ͨ��
	SOUND_UI_WIN = -190,			//�ؿ�ʤ��
	SOUND_UI_FAIL = -189,			//�ؿ�ʧ��
	SOUND_SOLDIER_WALK_1 = -188,	//С������1
	SOUND_SOLDIER_WALK_2 = -187,	//С������2
	SOUND_SOLDIER_WALK_3 = -186,	//С������3
	SOUND_READY = -185,				//read go!
	SOUND_ITEM_GAIN = -184,		//��õ���
	SOUND_SOLDIER_STABLES_WALK_1 = -183,	//���С������ 1
	SOUND_SOLDIER_STABLES_WALK_2 = -182,	//���С������ 2
	SOUND_SOLDIER_STABLES_WALK_3 = -181,	//���С������ 3
	SOUND_PROP_FULLPEOP = -180,	//������
	SOUND_PROP_ADDTIME = -179,		//ʱ��
	SOUND_PROP_INVISIBILITY = -178,	//������
	SOUND_PROP_POISON = -177,	//��Ģ��
	SOUND_PROP_COOKING = -176,		//����/�ܲ�ը��
	SOUND_PROP_OIL = -175,		//�ع���
	SOUND_PROP_FREEZE = -174,		//����
	MUSIC_UI_MAIN = -100,			//������
	MUSIC_UI_BIG_GATE = -99,		//��ؿ�����
	MUSIC_UI_SMALL_GATE = -98,		//С�ؿ�����
	MUSIC_UI_START = -97,			//Ƭͷ����
	MUSIC_UI_UPDATE = -96,			//���½���
	MUSIC_UI_MALL = -95,			//�̳ǽ���
	MUSIC_UI_PVPDIALOG = -94,		//pvp������
	MUSIC_UI_TEACH_DIALOG = -93,		//��ѧUI
	MUSIC_UI_PVPROOM = -92,		//����UI
	MUSIC_GATE_1010 = 1010,			//�ؿ�1010
	MUSIC_GATE_1020 = 1020,
	MUSIC_GATE_1030 = 1030,
	MUSIC_GATE_1040 = 1040,
	MUSIC_GATE_1050 = 1050,
	MUSIC_GATE_1051 = 1051,
	MUSIC_GATE_1060 = 1060,
	MUSIC_GATE_1070 = 1070,
	MUSIC_GATE_1080 = 1080,
	MUSIC_GATE_1090 = 1090,
	MUSIC_GATE_1100 = 1100,
	MUSIC_GATE_1101 = 1101,
	MUSIC_GATE_1110 = 1110,
	MUSIC_GATE_1120 = 1120,
	MUSIC_GATE_1130 = 1130,
	MUSIC_GATE_1140 = 1140,
	MUSIC_GATE_1150 = 1150,
	MUSIC_GATE_1151 = 1151,
	MUSIC_GATE_1152 = 1152,
	MUSIC_GATE_2010 = 2010,			//�ؿ�2010
	MUSIC_GATE_2020 = 2020,
	MUSIC_GATE_2030 = 2030,
	MUSIC_GATE_2040 = 2040,
	MUSIC_GATE_2050 = 2050,
	MUSIC_GATE_2051 = 2051,
	MUSIC_GATE_2060 = 2060,
	MUSIC_GATE_2070 = 2070,
	MUSIC_GATE_2080 = 2080,
	MUSIC_GATE_2090 = 2090,
	MUSIC_GATE_2100 = 2100,
	MUSIC_GATE_2101 = 2101,
	MUSIC_GATE_2102 = 2102,
	MUSIC_GATE_2110 = 2110,
	MUSIC_GATE_2120 = 2120,
	MUSIC_GATE_2130 = 2130,
	MUSIC_GATE_2140 = 2140,
	MUSIC_GATE_2150 = 2150,
	MUSIC_GATE_2151 = 2151,
	MUSIC_GATE_2152 = 2152,
	MUSIC_PVPGATE_10201 = 10201,
	MUSIC_PVPGATE_10202 = 10202,
	MUSIC_PVPGATE_10203 = 10203,
	MUSIC_PVPGATE_10204 = 10204,
	MUSIC_PVPGATE_10301 = 10301,
	MUSIC_PVPGATE_10303 = 10303,
	MUSIC_PVPGATE_10401 = 10401,
	MUSIC_PVPGATE_10402 = 10402,
	MUSIC_PVPGATE_10403 = 10403,
	MUSIC_PVPGATE_10404 = 10404,
	MUSIC_PVPGATE_10406 = 10406,

};

enum ePVPReultText
{
	TEXT_PVPRESULT_BEGIN = 14000,
	TEXT_PVPRESULT_ADDGLORY,		 //PVP����������
	TEXT_PVPRESULT_ADDSCORE,		 //PVP�����û���
	TEXT_PVPRESULT_SUBSCORE,		 //PVP�����ø�����
	TEXT_PVPRESULT_OPNER_EXTRAAWARD, //PVP���㷿��ʤ����ö������
	TEXT_PVPRESULT_OPNER_AWARD,		 //PVP���㷿��ʧ�ܽ�������
	TEXT_PVPRESULT_NORANK,			 //PVP����δ�ϰ�
	TEXT_PVPRESULT_ADDHONOR = 14009, //pvp����������������
	TEXT_PVPRESULT_VIPADD = 14012,	 //VIP�ӳɽ���,
	TEXT_PVPRESULT_STREAKADD = 14013,  //��ʤ,
};

enum eCrazyWeekText
{
	TEXT_CRAZYWEEK_DONE = 14008,     // �����
};

enum eHonorShopText
{
	TEXT_HONORSHOP_LIMIT = 14010,   // ����
	TEXT_HONORSHOP_FRESH,			// n��ˢ��
	TEXT_HONORSHOP_NOTENOUGH = 15008, // ��������
	TEXT_HONORSHOP_DAYS = 22202,   // %d��
	TEXT_HONORSHOP_HOURS = 22203,  // %dСʱ
	TEXT_HONORSHOP_MINUTE = 22204, //%02d:%02d
};

enum eActNoticeText
{
	TEXT_FIRST_CHARGE = 19004,    // �׳����
	TEXT_TOTAL_CHARGE,
};

enum eMPlusHelpText
{
	TEXT_MPLUSHELP_BEGIN = 18000,
	TEXT_MPLUSHELP_SHARING,
	TEXT_MPLUSHELP_INPUT5INVITCODE,
	TEXT_MPLUSHELP_INVITED,
	TEXT_MPLUSHELP_INPUTINVITCODE_ERROR,
	TEXT_MPLUSHELP_INPUTINVITCODE_SELF,
	TEXT_MPLUSHELP_INPUTINVITCODE_NOTEXSIT,
	TEXT_MPLUSHELP_INPUTINVITCODE_AGAIN,
	TEXT_MPLUSHELP_FIVESTAR_AGAIN,
	TEXT_MPLUSHELP_WATCH_MOVIE,
	TEXT_MPLUSHELP_AWARD_MAIL,
	TEXT_MPLUSHELP_PLEASE_BINDFB,
	TEXT_MPLUSHELP_FREE_DIAMONDS,
	TEXT_MPLUSHELP_FREE_MOIVE,
};

enum eShopText
{
	TEXT_SHOP_LEFTCOUNT = TEXT_SHOP + 1,
	TEXT_SHOP_LEFTTIME_DAY,
	TEXT_SHOP_LEFTTIME_HOUR,
	TEXT_SHOP_BUY_CONFIRM,
};

// �����ָ�ʱ��ڵ�
enum eRecoveryTime
{
	ERT_Zero = 0,
	ERT_Twelve,
	ERT_Max,
};

// pvpƥ������
enum ePvpMatchType
{
	EPT_Match_Two = 0,		// 2�˶�ս
	EPT_Match_Three,		// 3�˶�ս
	EPT_Match_Four,			// 4�˶�ս
};

// ���а�����
enum eRankingGroupType
{
	RANKINGGROUP_DEFAULT = 0,
	RANKINGGROUP_BRONZE = 1,
	RANKINGGROUP_SILVER = 2,
	RANKINGGROUP_GOLD = 3,
	RANKINGGROUP_DIAMOND = 4,
	RANKINGGROUP_KING = 5,
};

//Buff����
enum eBuffType
{
	E_BUFF_NONE = 0,
	E_Buff_MoveSpeed = 1,	//�ƶ��ٶ�buff
	E_Buff_AttackSpeed,		//�����ٶ�buff
	E_Buff_Defence,			//������buff
	E_Buff_FillPeople,		//����buff
	E_Buff_ProduceSpeed,	//�����ٶ�buff
	E_Buff_Attack,
}; 


enum eCheckPointState
{
	E_CHECKPOINTSTATE_CLOSE = 0,
	E_CHECKPOINTSTATE_OPEN = 1,
	E_CHECKPOINTSTATE_PASS = 2,
};

enum eBuyDiamondAwardType
{
	E_AWARD_MONTH = 1,
	E_AWARD_PVEPOWER_LIMIT,
	E_AWARD_FLAG,
	E_AWARD_PVPPOWER_LIMIT,
	E_AWARD_DIAMOND,
};
enum eCheckPointType
{
	E_CHECKPOINT_ALL = 0,
	E_CHECKPOINT_MAIN,
	E_CHECKPOINT_BRANCH,
};

enum eLevelType
{
	E_Level_Main = 0,		//����
	E_Level_Branch = 1,		//ֱ��
	E_Level_Boss =2,		//BOSS
	E_Level_Trans = 3,		//ѵ��Ӫ�ؿ�
};

enum eWorldType
{
	E_WORLD_1 =1,
	E_WORLD_2,
	E_WORLD_3
};

// 
enum ePaySdkType
{
	E_PAY_JIDI = 0,
	E_PAY_DIANXING,
	E_PAY_ONLING,
};

enum ePropPos//����λ��
{
	E_POS_LEFT = 1,
	E_POS_MID, 
	E_POS_RIGHT,
	E_POS_NONE
};
//////////////////////////////////////////////////////////
enum eGuideMessageType
{
	E_GUIDEMESSAGE_START,
	E_GUIDEMESSAGE_OVER,
};

enum eGuideOperatorType
{
	E_GuideOperator_None = -1,
	E_GuideOperator_Click = 0,
	E_GuideOperator_Drag = 1,
	E_GuideOperator_DouClick = 2, 

};
 

enum eGuideLocationType
{
	E_GUIDELOC_BEGIN = 0,
	E_GUIDELOC_GATE = 1,					//�ؿ�
	E_GUIDELOC_GAMEPAGE = 2,				//ս��UI
	E_GUIDELOC_SHOP = 3,					//�̵�
	E_GUIDELOC_JUNGLE = 4,					//Ұ�ֳ���֪ͨbattleView
	E_GUIDELOC_MAINUI = 5,					//������
	E_GUIDELOC_SELECTGATE = 6,				//ѡ�ؽ���
	E_GUIDELOC_BATTLESUCCESS = 7,			//ս��ʤ���Ի������
	E_GUIDELOC_SIGNIN = 8,					//ǩ��
	E_GUIDELOC_PVPUI = 9,					//pvp����
	E_GUIDELOC_LEVELDIALOG = 10,			//ѡ��ս�������
	E_GUIDELOC_OCU1 = 11,					//ռ�콨��֪ͨgamePage����
	E_GUIDELOC_LEVELUP = 12,				//������Ϊ
	E_GUIDELOC_GLYPH = 13,					//����
	E_GUIDELOC_SOLDIERNUM = 14,				//��������һ��ֵ����
	E_GUIDELOC_E_ATK_P1 = 15,				//battleView��ҽ��յ��˽���
	E_GUIDELOC_E_ATK_P2 = 16,				//gamepage��ҽ��յ��˽���
	E_GUIDELOC_LEVELUP2 = 17,				//������ʱ����
	E_GUIDELOC_E_ATKED_N = 18,				//���������ܵ����� (ǿ������֮��)
	E_GUIDELOC_P_ATK_N = 19,				//���������յ���ҹ�����Ϊ Player attack Neutral (ǿ������ǰ)
	E_GUIDELOC_E_ATK_N = 20,				//���������յ����˹�����Ϊ Enemy attack Neutral (ǿ������ǰ) 
	E_GUIDELOC_P_ASS_P = 21,				//��ҷ���֧Ԯ�Լ����� Player assist Player (ǿ������ǰ) 
	E_GUIDELOC_P_ATK_E = 22,				//���˽����յ���ҹ�����Ϊ Player attack Enemy  (ǿ������ǰ)  
	E_GUIDELOC_OCU2 = 23,					//ռ�콨��֪ͨbattleView����
	E_GUIDELOC_JUNGLE2 = 24,				//Ұ�ֳ���֪ͨgamePage
	E_GUIDELOC_OCU_JUNGLE = 25,				//Ұ�ֱ�ռ��
	E_GUIDELOC_USEPROP = 26,				//ʹ�õ���
	E_GUIDELOC_RENAMEFINISH = 27,           //������ɴ���
	E_GUIDELOC_INSTITUTE_ENTER= 28,			//�о�Ժ�������
	E_GUIDELOC_INSTITUTE_GETCARD = 29,	    //�о�Ժ�鿨���
	E_GUIDELOC_GENERAL_ENTER = 30,          //��ʳ��������ҳ��
	E_GUIDELOC_GENERAL_EQUIP = 31,			//��ʳ����װ������
	E_GUIDELOC_END = 999,					//��������
};

enum eGuideFunctionType
{
	E_GuideFuc_Tips = 1,				//ս����ʾ��ť
	E_GuideFuc_Drag = 2,				//ս����ҷ����
	E_GuideFuc_AddMoney = 3,			//����ʯ����
	E_GuideFuc_TabShop = 4,				//�л��̳Ǳ�ǩ
	E_GuideFuc_CloseShopDl = 5,			//�رչ���ҳ��detail
	E_GuideFuc_EnterShopMn = 6,			//���빺��ҳ��main
	E_GuideFuc_AddMoneyAni = 7,			//������ʯ�Ķ���
	E_GuideFuc_AddGlyphStuff = 8,		//���Ӳ��ϵĶ��� 
	E_GuideFuc_UseProp      = 9,		//ʹ�õ��� 
	E_GuideFuc_BV2GP = 10,				//battleView֪ͨgamepage
	E_GuideFuc_GP2BV = 11,				//gamepage֪ͨbattleView
	E_GuideFuc_AddGift = 12,			//�������
	E_GuideFuc_GotoMain = 13,			//������ҳ��
	E_GuideFuc_OpenUI = 14,				//��UI����
	E_GuideFuc_OpenLuaUI = 15,			//��lua��UI����
};

enum eGuideNodeTag
{
	E_GuideTag_ShopBuyBtn = 832,		//�̳ǹ���ť
	E_GuideTag_LevelStargBtn = 81,		//ѡ�ؽ��濪ʼս����ť
	E_GuideTag_LevelPanel = 675,		//����ѡ������
	E_GuideTag_LevelPropOneBtn = 481,	//����ѡ��ť1
	E_GuideTag_LevelPropOKBtn = 482,	//����ȷ�ϰ�ť2
	E_GuideTag_ShopPanelBuyBtn = 144,   //�̳ǹ���ť
	E_GuideTag_ShopPanelBackBtn = 293,  //�̳Ƿ��ذ�ť
}; 

//////////////////////////////////////////////////////////


enum eChattingMsgType
{
	MSG_TYPE_NULL,
	MSG_TYPE_WORLD = 1,
	MSG_TYPE_PVP = 2,
	MSG_TYPE_SYSTEM = 3,
};



//ս����Ϣ��ʾ����
enum eBattleTip
{
	E_COMPLETE_UPGRADE_BARRACK,		//��Ӫ�������
	E_COMPLETE_UPGRADE_TURRET,		//�����������
	E_OCCUPY_BARRACK,				//ռ���Ӫ���
	E_OCCUPY_TURRET,				//ռ���������
	E_OCCUPY_INSTITUTE,				//ռ���о������
	E_GETBUFF_ATTACKSPEED,			//�����ٶ�buff
	E_GETBUFF_PRODUCTSPEED,
	E_GETBUFF_ATTACK,
	E_GETBUFF_DEFENCE,
	E_GETBUFF_FILLPEOPLE,
	E_GETBUFF_MOVESPEED,

};

//�����б�����
enum eFriendListType
{
	E_FRIEND_MINE_GET = 1,		//�ҵĺ���
	E_FRIEND_MINE_ADD,			//�ҵĺ�������
	E_FRIEND_MINE_REMOVE,		//�ҵĺ���ɾ��
	E_FRIEND_APPLIACTION,		//�����б�
	E_FRIEND_APPLIACTION_NONE,	//֪ͨ�������������
	E_FRIEND_APPLIACTION_HAVE,	//֪ͨ�������������
	E_FRIEND_BLACK,				//������
	E_FRIEND_PK,				//Լս�б�
	E_FRIEND_PK_NONE,			//֪ͨ������Լս����
	E_FRIEND_PK_HAVE,			//֪ͨ������Լս����
	E_FRIEND_SEARCH,			//�����б�
	E_FRIEND_ADD_REQUEST_DONE,	//���뷢��
	E_FRIEND_FIND_NONE,			//����Ϊ��
	E_FRIEND_AGREE_PK,			//ͬ��Լս
	E_FRIEND_INFO_PANLE,		//����ϸ���
};

enum ePowerGiftType
{
	TEXT_POWER_NULL = TEXT_POWER_GIFT,
	TEXT_POWER_OFF,
	TEXT_ENERGY_OFF,
	TEXT_POWER_TITLE,
	TEXT_ENERGY_TITLE,
	TEXT_POWER_TIPS,
	TEXT_ENERGY_TIPS,
	TEXT_POWER_GET,
};


enum eCurrencyType
{
	CNY = 0,
	HKD = 1,
	TWD = 2,
	USD = 3,
	EUR = 4,
	GBP = 5,
	JPY = 6,
};

enum eVituralCurrencyType
{
	Diamond = 0,
};

enum eItemOperation
{
	Buy = 0,
	Get = 1,
	Use = 2,
};

enum eItemType
{
	Defualt = 0,
};

enum AWARDTYPE
{
	A_GLYPHSTUFF	= 100000,//���Ĳ���
	A_DIAMOND		= 20001, //��ʯ
	A_PVEPOWER,				 //����
	A_PVPPOWER,				 //����
	A_Honor,				 //����
	A_ROOMTICKET,			 //����ȯ
};

enum ePveBattleRecord	//pveʱ��ҵĲ�����		��Message.h�е�EMT_XX����
{
	PVE_RECORD_MOVE = 1,
	PVE_RECORD_UPGRADE = 5,
	PVE_RECORD_PROP_CHANGE = 18,
	PVE_RECORD_PROP_CHANGEOK = 19,
	PVE_RECORD_PROP_ADDTIME = 20,
	PVE_RECORD_PROP_FULLPEOP = 21,
	PVE_RECORD_PROP_FREEZE = 22,
	PVE_RECORD_PROP_COOKING = 23,
	PVE_RECORD_CREEPS_BORN = 24,
	PVE_RECORD_OUT_PERCENT = 25,
	PVE_RECORD_TURRET_UPDATE = 26,
}; 


enum eSoldierType
{
	Soldier_Normal, //��ͨС��
	Soldier_Speed,  //����С��
};


enum TitleType {
	INVALID = 0,
	WINNER = 1,
	BRAVE = 2,
	FIGHTCRAZY = 3,
	MAX,
};

enum eCsvLoadState
{
	LS_LOADING,
	LS_COMPLETE,	// ���ļ��������
	LS_OVER,		// �������
};

//�����淨ָ������
enum eIntelligentType
{
	EIT_NONE = -1,//�к�㵫��û������ָ��
	EIT_GIFT = 1,
	//
	EIT_WINNER = 3,
	EIT_POWER = 4,
	EIT_ACHIEVE = 5,
	EIT_FRIEND = 6,
	EIT_GLYPH = 7,
	EIT_DAYTASK = 8,
	EIT_NEWYEAR = 9,
	EIT_CRAZYWEEK = 10,
};

//������Ϣ���
enum ePersonalInfo
{
	EPI_RANK = 22000,
	EPI_VIP_START = 22000,
	EPI_VIP_ONE = 22001,
	EPI_VIP_TWO = 22002,
	EPI_VIP_THREE = 22003,
	EPI_CHANG = 22004,
	EPI_CI = 22005,
	EPI_UNVISIBLE = 22006,
	EPI_EXTRA_UNLOCK = 22007,
	EPI_EXTRA_LOCK = 22008,
	EPI_SIGN_ERROR_ONE = 22009,
	EPI_SIGN_ERROR_TWO = 22010,
};

//ʱ���ʽ�ı�
enum eTextTimeFormat
{
	TEXT_TIME_FORMAT1 = 23000,
	TEXT_TIME_FORMAT2 = 23001,
	TEXT_TIME_FORMAT3 = 23002,
	TEXT_TIME_FORMAT4 = 23003,
};

//���а����ı�
enum eTextRankAward
{
	TEXT_RANK_PVP_COUNT  = 23100,
	TEXT_RANK_NEED_SCORE = 23101,
	TEXT_RANK_BOX		 = 23102,
	TEXT_NO_RANK		 = 23103,
};

//����Ŀ��ϵͳ�ı�
enum eTextSevenGoal
{
	TEXT_SEVEN_GOAL_NOW_MISSION = 23201,
	TEXT_SEVEN_GOAL_NEXT_MISSION = 23202,
	TEXT_SEVEN_GOAL_NOW_GET = 23203,
	TEXT_SEVEN_GOAL_NEXT_GET = 23204,
};

enum eTextInstitue
{
	TEXT_INSTITUTE_1 = 30040,
	TEXT_INSTITUTE_2 = 30041,
	TEXT_INSTITUTE_DES11 = 30042,
	TEXT_INSTITUTE_DES12 = 30043,
	TEXT_INSTITUTE_DES21 = 30044,
	TEXT_INSTITUTE_DES22 = 30045,
};

//�۷������ı�
enum eTextTopRank
{
	TEXT_TOP_RANK_RULE1 = 30100,
	TEXT_TOP_RANK_RULE2 = 30101,
	TEXT_TOP_RANK_RULE3 = 30102,
	TEXT_TOP_RANK_ACTIVITY_TIME = 30103,
	TEXT_TOP_RANK_UPDATE_TIME = 30104,
	TEXT_TOP_RANK_GET_STAR = 30105,
	TEXT_TOP_RANK_GET_WIN = 30106,
	TEXT_TOP_RANK_GET_WINSTREAK = 30107,
	TEXT_TOP_RANK_TEXT_STAR = 30108,
	TEXT_TOP_RANK_TEXT_WIN = 30109,
	TEXT_TOP_RANK_TEXT_WINSTREAK = 30110,
	TEXT_TOP_RANK_ASSISTANT_CARD = 30111,
};

enum eNewCampLevel
{
	E_NEWCAMP_0 = 99000,
	E_NEWCAMP_1 = 99010,
	E_NEWCAMP_2 = 99020,
	E_NEWCAMP_3 = 99030,
	E_NEWCAMP_4 = 99040,
	E_NEWCAMP_5 = 99050,
	E_NEWCAMP_6 = 99060,
};

//lua�е�GetTimeRemaining����ʹ��
enum eTimeRemainingMode
{
	E_NORMAL = 1, //����
	E_WITH_NEXT = 2, //���ǳ���Ŀ��ʱ��㣬���ڱ����ڣ�����㵽���ܵ�Ŀ��ʱ���ʱ��
};

/* �ı�����ʱ��ͬʱ��������
* @state : �����Զ�Ӧ������λ
*/
#define CC_SYNTHESIZE_DIRTY(varType, varName, funName, state)\
protected: varType varName; \
public: virtual varType get##funName(void) const { return varName; }\
public: virtual void set##funName(varType var){ varName = var; mDirtyTag.set(state); }

/* 3.0 uireader
*/
#define DEFINE_PAGE_READER(_page_name)\
class _page_name##Reader : public cocostudio::NodeReader \
{ \
public:\
	static cocostudio::NodeReader* getInstance(){ return Singleton<_page_name##Reader>::Get(); }\
	static void purge(){ Singleton<_page_name##Reader>::Free(); }\
	cocos2d::Node* createNodeWithFlatBuffers(const flatbuffers::Table* nodeOptions)\
{\
	_page_name* node = _page_name::Get(); \
if (node)\
{\
	setPropsWithFlatBuffers(node, nodeOptions); \
	dynamic_cast<PageBase*>(node)->initUi(); \
	return node; \
	}\
	return NULL; \
	}\
	};

#define REGIST_PAGE_READER(_page_name)\
	Singleton<_page_name##Reader>::Get()->registReaderObject(#_page_name, (ObjectFactory::Instance)_page_name::getInstance);
