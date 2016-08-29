/*
* jacey
* ��Ϣ
*/
#pragma once

#include "cocos2d.h"
USING_NS_CC;

#include "Constant.h"

enum eMessageType
{
	EMT_INVALID = -1,
	EMT_ATTACK,			// ����A   ����  ����B

	EMT_MOVE,			//�ƶ�
	EMT_TRANSFORM,		//ͬ��ӪǶ��
	EMT_FIGHT,			//ʿ��ս����һ�ţ�
	EMT_PRODUCT,		//����
	EMT_UPGRADE,		//����
	EMT_TOWER_ATTACK,	//��������
	EMT_BUILD_DESTORY,	// ������ռ��
	EMT_SUCC_CHECKSTAR,	// ���ʤ��ʱ���ǽ��� 
	EMT_REFRESH_STAR,	// ���ǽ�����ɣ�ˢ��UI
	EMT_PAGE_GO,			//����
	EMT_STAR_STATU,			//�������
	EMT_CHECKPOINT_START,	// �ؿ���ʼ
	EMT_CHECKPOINT_END,		// �ؿ�����
	EMP_POWER_CHANGE,		// ����ֵ�ı�
	EMT_ENERGY_CHANGE,   //����ֵ�ı�
	EMT_GUIDE_STEP_OVER,	// ָ������
	EMT_OPEN_UI,			// ��UI 
	//�������
	EMT_PROP_CHANGE,	//��������ת��
	EMT_PROP_CHANGEOK,	//��������ת���ɹ�
	EMT_PROP_ADDTIME,	//�Ʊ�ת��
	EMT_PROP_FULLPEOP,	//��������
	EMT_PROP_FREEZE,	//����
	EMT_PROP_COOKING,	//�ٻ���ʦ
	EMT_PROP_INVISIBILITY,	//���ζ���
	EMT_PROP_Oil,	//�ع���
	EMT_PROP_USER,
	EMT_CHANNEL_LOGIN = 1000,		// ��¼���
	EMT_CHANNEL_LOGOUT,				// �ǳ����
	EMT_GUIDE,			//��������
	EMT_GUIDE_TIPS,		//���������ʾ
	EMT_GUIDE_PAUSE,	//��Ϸ��ͣ  
	EMT_JUNGLE,			//��Ұ��Ϣ���
	EMT_MORALE,			//ʿ����Ϣ���
	EMT_BUFF,			//buff��Ϣ���
	EMT_REDUCEPEOPLE,	//
	EMT_MONEY_CHANGE,//��Ҹı�
	EMT_RESMODE_SCORE, // ����ģʽ����һ��ֱ仯
	EMT_CLOSEPAGE, //�رս���
	EMT_STOP_MATCH, // ֹͣƥ��
	EMT_CLOSE_PVP, // �ر�pvp
	EMT_OPEN_STAGE, //PVE�������½�
	EMT_ENABLE_OPTIONBUTTON, //�������ð�ť

	EMT_CHANGE_NAME,//������
	EMT_CHANGE_PLAYER_ICON,//��ͷ�� add in 2015-10-08

	EMT_SIGNAL_CHANGE,//��̖���Q
	EMT_BUFFSHOW, //Buff��ʾ���
	EMT_MORALESHOW, //ʿ����ʾ���

	EMT_MENU_BTN, //gamepage Ͷ����ť����

	EMT_ROW_DEPART, //ʿ���뿪��һ�ţ�
	EMT_FORMATION_OVER, // ���ͽ���

	EMT_ACHIEVE_GOTOLEVEL,  //�ɾ�ǰ���ؿ�
	EMT_HONGDIAN_TIP = 1025,//��δ��ȡ����ʾ��ȡ

	EMT_SHOP_PAYCOMPLENT,  //֧����ɺ�ص�
	EMT_SHOP_SHOW_BUYITEM,//��ʾ������ʯ��
	EMT_SHOP_AFTER_BUYDIAMOND, //������ʯ������Ӧ����Ʒ
	//ս����ʾ��Ϣ
	EMT_BATTLE_TIP,					//ս����Ϣ
	EMT_MISSION_STATE,              //ÿ������

	EMT_BTN_CONTROL,			//���ť����/����
	EMT_ACTIVATION_TIP,		//��������ȡʧ��ԭ����ʾ
	EMT_CHAT_MSG,		//����֪ͨ
	EMT_FRIEND_UPDATE,			//����ϵͳ�б����
	EMT_BUFFER_TIP,		//ս��buff��ʾ
	EMT_GAMESTART,		//��Ϸ��ʽ��ʼ

	EMT_ITEM_OPERATION, // ��Ʒ����
	EMT_PROP_USE_ALL, // ��������
	EMT_PVP_INFO,		// pvp��Ϣ

	EMT_LITTLETIPS,//С��ʾ
	EMT_GETSAVEDATAOVER, //��ȡ�浵���
	EMT_RQST_RESULT, //������ 
	EMT_SDKLOGIN_SUCCESS,//SDK��½�ɹ�
	EMT_GAMESERVERLOGIN_SUCCESS,//��Ϸ����½�ɹ�
	EMT_SIMPLIFY_UI,    //pvp ��սģʽ�� ui��
	EMT_UPDATE_PLAYER_BADGES,   //ѫ�»���б����
	EMT_CHANGE_PLAYER_BADGE,   //��ǰѫ�¸���
	EMT_CONTROL_OUT_PERCENT,	//ս����¼����,�����ٷֱȰ�ť״̬����
	EMT_CONTROL_PROP_USE,	//ս����¼����,ʹ�õ��ߣ����Ŀ��
	EMT_CONTROL_CLICK_PROP,	//ս����¼����,�������
	EMT_CONTROL_CLICK_PROP_SECEND,//ս����¼����,������߶���ѡ�񣬽���ת����
	EMT_CONTROL_CANCEL_PROP,	//ս����¼����,ȡ��ʹ�õ���

	EMT_BUTTON_OUT_PERCENT,	//�����ٷֱȰ�ť״̬����

	EMT_BUTTON_LOGINSCENE,//��¼���濪ʼ��Ϸ��ť
	EMT_BUTTON_LOGINSCENE_UNVISIBLE,

	EMT_CRAZY_WEEK,// ���ܻ
	EMT_OPEN_LEVEL_DIALOG,	//�򿪹ؿ���Ϣ����
	EMT_UPDATE_EXCHANGE,	//�����һ�
	EMT_UPDATE_SHOPLIST,//��ֵ���
	EMT_CLOSESHOP,//�ر��̳�
	EMT_GW,	//ʳ��������(GlobalWars)
	EMT_GOTO_ACTLAB,	//��ת���ǩ
	EMT_BUY_VIP,		//vip
	EMT_PLAYER_UPGRADE,//�������

	EMT_GAME_LOGIN_FLOW, // ��¼����
	EMT_RED_PACKET_CAN_REWARD = 1065,//�������
	EMT_SELECTWORD_SCHEDULE = 1066,//�����涨ʱ������
	EMT_CHANGE_SERVER,  //���µ�½������

	EMT_CHANGE_SIGN,//����ǩ��
	EMT_CHANGE_EXTRA_VISIBLE,//���Ķ�����Ϣ�Ŀɼ���
	EMT_GET_EXTRA,//��ȡ������Ϣ
	EMT_POINT_CHANGE,   //���pvp���ָı�

	EMT_RECORD_CREEPS_BORN,	//��¼Ұ�ֳ���
	EMT_RECORD_OUT_PERCENT,	//��¼��������
	EMT_RECORD_CLICK_PROP,	//��¼�������
	EMT_RECORD_CLICK_PROP_SECEND,	//��¼���ε������
	EMT_RECORD_CANCEL_PROP,	//��¼ȡ��ѡ�����

	EMT_PVPRESULT_OPENBOX,       //pvp���㱦���

	EMT_OPEN_DAILY_MISSION = 1078,//��ÿ���������
	EMT_DALIY_MISSION_TIPS = 1079,//ÿ��������ʾ
	EMT_DAILY_MISSION_PROGRESS_BACK = 1080, //ÿ�������ϱ��������ݵĻذ�
	EMT_SHOW_DAILY_MISSION_PROGRESS = 1081, //��ʾÿ��������Ƚ���
	EMT_SHARE_SUCCESS, //����ɹ�
	EMT_SKIP_SUCCESS = 1083,//���سɹ�
	EMT_INVITE_BAT_SWITCH = 1084, //��ս��ʾ��ʾ�����л�

	EMT_RECORD_TURRET_UPDATE, //��¼��̨תaoe��ice
	EMT_RECORD_EXECUTE_TURRET_UPDATE, //�طţ�ִ��������̨ת���Ĳ���

	EMT_HONOR_CHANGE, // ����ֵ�ı�
	
	EMT_GUIDE_FIRST = 1088, //��һ�ν�����Ϸ��ز���
	
	EMT_RANK_CHANGE,   //��������ı�
};


class GameMessage
{
public:
	virtual int getMessageType() const { return -1; }

	virtual void	formatMessage(std::stringstream& ) = 0;
};

class MessageHandler
{
public:
	virtual void onReceiveMassage(GameMessage * message) = 0;
};

class GameMessageFactory
{
public:
	virtual GameMessage* create(std::string params) = 0;
};

#define DEFINE_FACTORY(message)	\
class message##Factory : public GameMessageFactory	\
{	\
public:	\
	virtual GameMessage* create(std::string params)	\
{\
	message* pret = new message(); \
if (pret)\
{\
std::stringstream ssParams(params);\
pret->formatMessage(ssParams); \
return pret; \
}\
	return NULL; \
}\
};


class MessageMove : public GameMessage
{
public:

	virtual void formatMessage(std::stringstream& sstream)
	{
		mIsNet = false;
		mTime = 0;
		sstream >> mStarId >> mEndId >> mFormationId >> mNumType >> mNumValue >> mTime;
		mIsNet = (mTime != 0); //flag = 1 Ϊtrue

	}

	virtual int getMessageType() const { return EMT_MOVE; }

protected:

	CC_SYNTHESIZE(int, mEndId, ToBuild);	// build key
	CC_SYNTHESIZE(int, mStarId, FromBuild);
	CC_SYNTHESIZE(int, mFormationId, FormationId);
	CC_SYNTHESIZE(float, mNumValue, NumValue);
	CC_SYNTHESIZE(int, mNumType, NumType);	// �������� 0-percent 1-num
	CC_SYNTHESIZE(bool, mIsNet, IsNet);	// �Ƿ�Ϊ���緢�� 
	CC_SYNTHESIZE(float, mTime, Time);	// �����ӳ�ʱ��
};

DEFINE_FACTORY(MessageMove);


class MessageAttack : public GameMessage
{
public:

	virtual void formatMessage(std::stringstream& sstream)
	{
		sstream >> mFromBuild
			>> mToBuild;
	}

	virtual int getMessageType() const { return EMT_ATTACK; }

protected:

	CC_SYNTHESIZE(int, mFromBuild, FromBuild);	// build key
	CC_SYNTHESIZE(int, mToBuild, ToBuild);
};

DEFINE_FACTORY(MessageAttack);
class MessageFight : public GameMessage
{
public:

	virtual void formatMessage(std::stringstream& sstream)
	{
		int attact;
		int defense;
		sstream >> attact >>defense >> mAttactNum >> mBuild >> mBuild2 >> mIsLastRow >> mFormationID;
		mAttactCamp = (eCampType)attact;
		mDefenseCamp = (eCampType)defense;

	}

	virtual int getMessageType() const { return EMT_FIGHT; }

protected:

	CC_SYNTHESIZE(eCampType, mAttactCamp, AttactCamp);
	CC_SYNTHESIZE(eCampType, mDefenseCamp, DefenseCamp);
	CC_SYNTHESIZE(int, mAttactNum, AttactNum);	
	CC_SYNTHESIZE(int, mBuild, Build);
	CC_SYNTHESIZE(int, mBuild2, Build2);
	CC_SYNTHESIZE(int, mIsLastRow, IsLastRow);
	CC_SYNTHESIZE(int, mFormationID, FormationID);
};

DEFINE_FACTORY(MessageFight);


class MessageProduct : public GameMessage
{
public:

	virtual void formatMessage(std::stringstream& sstream)
	{
		sstream >> mBuildKey;
	}

	int getMessageType() const { return EMT_PRODUCT; }

	CC_SYNTHESIZE(int, mBuildKey, BuildKey);
};
DEFINE_FACTORY(MessageProduct);


class MessageTransform : public GameMessage
{
public:

	virtual void formatMessage(std::stringstream& sstream)
	{
		sstream >> mInput >> mBuild >> mBuild2 >> mIsLastRow >> mFormationID; 
	}

	virtual int getMessageType() const { return EMT_TRANSFORM; }

protected:

	CC_SYNTHESIZE(int, mInput, TransformNum);
	CC_SYNTHESIZE(int, mBuild, Build);
	CC_SYNTHESIZE(int, mBuild2, Build2);
	CC_SYNTHESIZE(int, mIsLastRow, IsLastRow);
	CC_SYNTHESIZE(int, mFormationID, FormationID);
};

DEFINE_FACTORY(MessageTransform);

class MessageUpgrade : public GameMessage
{
public:

	virtual void formatMessage(std::stringstream& sstream)
	{
		isNet = false;
		int flag = 0;
		sstream >> mBuild >> flag;
		isNet = (flag == 1);
	}

	virtual int getMessageType() const { return EMT_UPGRADE; }

protected:
	CC_SYNTHESIZE(int, mBuild, Build);
	CC_SYNTHESIZE(bool, isNet, IsNet);

};

DEFINE_FACTORY(MessageUpgrade);

class MessageBuildChange : public GameMessage
{
public:

	virtual void formatMessage(std::stringstream& sstream)
	{
		mCamp = 10;
		int iNew = 0;
		sstream >> mBuild >> mCamp >> iNew >> mPropId;

		isNew = iNew == 1;
	}

	virtual int getMessageType() const { return EMT_PROP_CHANGE; }

protected:

	CC_SYNTHESIZE(int, mBuild, Build);
	CC_SYNTHESIZE(int, mCamp, Camp);
	CC_SYNTHESIZE(bool,isNew , New);//�Ƿ��� ����һ ת����.
	CC_SYNTHESIZE(int, mPropId, PropId);
};

DEFINE_FACTORY(MessageBuildChange);

class MessageBuildChangeOk : public GameMessage
{
public:

	virtual void formatMessage(std::stringstream& sstream)
	{
		int type;
		int iNew = 0;
		sstream >> mBuild >> type >> iNew;
		mChangeType = (eBulidType)type;
		isNew = iNew == 1;
	}

	virtual int getMessageType() const { return EMT_PROP_CHANGEOK; }

protected:

	CC_SYNTHESIZE(int, mBuild, Build);
	CC_SYNTHESIZE(eBulidType, mChangeType, ChangeType);
	CC_SYNTHESIZE(bool, isNew, isNew);//�Ƿ��� ����һ ת����.
};

DEFINE_FACTORY(MessageBuildChangeOk);


class MessageFullPeopel : public GameMessage
{
public:

	virtual void formatMessage(std::stringstream& sstream)
	{
		mCamp = 10;
		sstream >> mBuild >> mCamp;
	}

	virtual int getMessageType() const { return EMT_PROP_FULLPEOP; }

protected:

	CC_SYNTHESIZE(int, mBuild, BuildId);
	CC_SYNTHESIZE(int, mCamp, Camp);
};

DEFINE_FACTORY(MessageFullPeopel);

class MessageAddTime : public GameMessage
{
public:

	virtual void formatMessage(std::stringstream& sstream)
	{
		sstream >> time;
	}

	virtual int getMessageType() const { return EMT_PROP_ADDTIME; }

public:
	
	float time;
};

DEFINE_FACTORY(MessageAddTime);

class MessageFreeze : public GameMessage
{
public:

	virtual void formatMessage(std::stringstream& sstream)
	{
		sstream >> camp;
	}

	virtual int getMessageType() const { return EMT_PROP_FREEZE; }

public:

	float time;
	int camp;
};

DEFINE_FACTORY(MessageFreeze);

class MessageCooking : public GameMessage
{
public:

	virtual void formatMessage(std::stringstream& sstream)
	{
		sstream >> x >> y >> camp ;

	}

	virtual int getMessageType() const { return EMT_PROP_COOKING; }

public:

	float x;
	float y;
	int camp;
};

DEFINE_FACTORY(MessageCooking);

class MessageOil : public GameMessage
{
public:

	virtual void formatMessage(std::stringstream& sstream)
	{
		sstream >> x >> y >> camp;

	}

	virtual int getMessageType() const { return EMT_PROP_Oil; }

public:

	float x;
	float y;
	int camp;
};

DEFINE_FACTORY(MessageOil);

class MessageInvisibility : public GameMessage
{
public:

	virtual void formatMessage(std::stringstream& sstream)
	{
		sstream >> buildId >> camp;
	}

	virtual int getMessageType() const { return EMT_PROP_INVISIBILITY; }

public:

	int buildId;
	int camp;
};

DEFINE_FACTORY(MessageInvisibility);

class MessageBuildAttact : public GameMessage
{
public:

	virtual void formatMessage(std::stringstream& sstream)
	{
		sstream >> mBuild;
	}

	virtual int getMessageType() const { return EMT_TOWER_ATTACK; }

protected:

	CC_SYNTHESIZE(int, mBuild, Build);
};

DEFINE_FACTORY(MessageBuildAttact);

class MessageBuildDestroy : public GameMessage
{
public:

	virtual void formatMessage(std::stringstream& sstream)
	{
		sstream >> mKeyID >> mInitCamp >> mToCamp;
	}

	virtual int getMessageType() const { return EMT_BUILD_DESTORY; }

	CC_SYNTHESIZE(int, mKeyID, KeyID);
	CC_SYNTHESIZE(int, mInitCamp, InitCamp);
	CC_SYNTHESIZE(int, mToCamp, ToCamp);
};

DEFINE_FACTORY(MessageBuildDestroy);

// ���ǽ���
class MessageCheckStar : public GameMessage
{
public:
	virtual void formatMessage(std::stringstream& sstream)
	{
		sstream >> mPlayerSuccess;
	}

	virtual int getMessageType() const { return EMT_SUCC_CHECKSTAR; }

	int mPlayerSuccess;
};
DEFINE_FACTORY(MessageCheckStar);

// ˢ������UI
class MessageRefreshStar : public GameMessage
{
public:
	virtual void formatMessage(std::stringstream& sstream)
	{
		sstream >> mFlashAll;
	}

	virtual int getMessageType() const { return EMT_REFRESH_STAR; }
	CC_SYNTHESIZE(int, mFlashAll, FlashAll);
};
DEFINE_FACTORY(MessageRefreshStar);

class MessagePage : public GameMessage
{
public:

	MessagePage() : mLuaType(0), mName(""), mIsCacheRes(false){}
	virtual void formatMessage(std::stringstream& sstream)
	{
		int page;
		sstream >> page >> mName >> mLuaType >> mIsCacheRes;
		mPage = (ePageType)page;
	}

	virtual int getMessageType() const { return EMT_PAGE_GO; }

protected:

	CC_SYNTHESIZE(ePageType, mPage, PageType);
	CC_SYNTHESIZE(int, mLuaType, LuaType);
	CC_SYNTHESIZE(std::string, mName, PageName);
	CC_SYNTHESIZE(bool, mIsCacheRes, IsCacheRes);
};

DEFINE_FACTORY(MessagePage);

// �ؿ���ʼ
class MessageCheckPointStart : public GameMessage
{
public:
	virtual void formatMessage(std::stringstream& sstream)
	{
		sstream >> mCpIndex;
	}

	virtual int getMessageType() const { return EMT_CHECKPOINT_START; }

	int mCpIndex;
};

DEFINE_FACTORY(MessageCheckPointStart);


// �ؿ�����
class MessageCheckPointEnd : public GameMessage
{
public:
	virtual void formatMessage(std::stringstream& sstream)
	{
		sstream >> mCpIndex >> mSuccess >> mReason;
	}

	virtual int getMessageType() const { return EMT_CHECKPOINT_END; }

	int mCpIndex;
	int mSuccess;
	std::string mReason;
};

DEFINE_FACTORY(MessageCheckPointEnd);


// �ؿ�����
class MessageStarStatu : public GameMessage
{
public:
	virtual void formatMessage(std::stringstream& sstream)
	{
		sstream >> mStatu >> mTag;
	}

	virtual int getMessageType() const { return EMT_STAR_STATU; }

	int mStatu;
	int mTag;
};

DEFINE_FACTORY(MessageStarStatu);

// Channel��¼
class MessageChannelLogin : public GameMessage
{
public:
	virtual void formatMessage(std::stringstream& sstream)
	{
		sstream >> mResult >> mToken >> channelId >> XGToken;
	}

	virtual int getMessageType() const { return EMT_CHANNEL_LOGIN; }

	int mResult;
	std::string mUid;
	std::string mToken;
	int  channelId;
	std::string XGToken;
	//std::string version;
};

DEFINE_FACTORY(MessageChannelLogin);

// Channel�ǳ�
class MessageChannelLogout : public GameMessage
{
public:
	virtual void formatMessage(std::stringstream& sstream)
	{
		sstream >> mResult >> mParam1;
	}

	virtual int getMessageType() const { return EMT_CHANNEL_LOGOUT; }

	int mResult;
	std::string mParam1;
};

DEFINE_FACTORY(MessageChannelLogout);


class MessageGuideStepOver : public GameMessage
{
public:
	virtual void formatMessage(std::stringstream& sstream)
	{
		sstream >> mStep;
	}

	virtual int getMessageType() const { return EMT_GUIDE_STEP_OVER; }

	int mStep;

};

DEFINE_FACTORY(MessageGuideStepOver);


// ����ֵ�ı�
class MessagePowerChanged : public GameMessage
{
public:
	virtual void formatMessage(std::stringstream& sstream)
	{		
	}

	virtual int getMessageType() const { return EMP_POWER_CHANGE; }
};

DEFINE_FACTORY(MessagePowerChanged);


class MessageOpenUI : public GameMessage
{
public:
	virtual void formatMessage(std::stringstream& sstream)
	{
		sstream >> mPageType;
	}

	virtual int getMessageType() const { return EMT_OPEN_UI; }

	int mPageType;

};

DEFINE_FACTORY(MessageOpenUI);


//����������Ϣ
class MessageGuideData: public GameMessage
{
public: 
	MessageGuideData(): m_GateID(0), m_GuideLocation(0), m_State(0){};
	~MessageGuideData(){};
	virtual void formatMessage(std::stringstream& sstream)
	{
		sstream >> m_GateID
			>> m_GuideLocation
			>> m_State;
	}

	virtual int getMessageType() const { return EMT_GUIDE; }
private:  
	CC_SYNTHESIZE(int, m_GateID, GateID);
	CC_SYNTHESIZE(int, m_GuideLocation, GuideLocation);
	CC_SYNTHESIZE(int, m_State, State); 
};
DEFINE_FACTORY(MessageGuideData);

class MessageGuideTips: public GameMessage
{
public:
	MessageGuideTips() : m_GuideID(0), m_FunctionID(0), m_FunctionParam(0){};
	~MessageGuideTips(){};
	virtual void formatMessage(std::stringstream& sstream)
	{
		sstream >> m_GuideID
			>> m_FunctionID
			>> m_FunctionParam;
	}

	virtual int getMessageType() const { return EMT_GUIDE_TIPS; }
private:
	CC_SYNTHESIZE(int, m_GuideID, GuideID); 
	CC_SYNTHESIZE(int, m_FunctionID, FunctionID);
	CC_SYNTHESIZE(int, m_FunctionParam, FunctionParam);
};
DEFINE_FACTORY(MessageGuideTips);
 


class MessagePropUserData : public GameMessage
{
public:
	MessagePropUserData() {};
	~MessagePropUserData(){};
	virtual void formatMessage(std::stringstream& sstream)
	{
		sstream >> mPropID;
	}

	virtual int getMessageType() const { return EMT_PROP_USER; }
private:
	CC_SYNTHESIZE(int, mPropID, PropID);
};
DEFINE_FACTORY(MessagePropUserData);

class MessageGuidePause: public GameMessage
{
public:
	MessageGuidePause(){};
	~MessageGuidePause(){};
	virtual void formatMessage(std::stringstream& sstream)
	{
		sstream 
			>> m_GuideID
			>> m_StopGame;
	}

	virtual int getMessageType() const { return EMT_GUIDE_PAUSE; }

private:
	CC_SYNTHESIZE(int, m_GuideID, GuideID);
	CC_SYNTHESIZE(bool, m_StopGame, StopGame);

}; 
DEFINE_FACTORY(MessageGuidePause);


enum eJungleOccupy
{
	E_JUNGLE_CREATE = 0,	//������Ұ����
	E_JUNGLE_ATTACK,		//������Ұ����
	E_JUNGLE_SUCCESS,		//ռ��ɹ�
	E_JUNGLE_FAILED,		//ռ��ʧ��
	E_JUNGLE_RETURN,		//ռ���߻ر�
	E_JUNGLE_MOVE,			//�ϲ�֪ͨ�²�ʿ���ƶ�
	E_JUNGLE_CLOSE,			//��Ұ����ʧЧ
	E_JUNGLE_REFRESH,		//��Ұ����ˢ��
	E_JUNGLE_REMOVEATTACK,  //�Ƴ�������
	E_JUNGLE_CLEAR,	//�Ƴ�����
};
class MessageJungle: public GameMessage
{
public:
	MessageJungle():m_SrcID(0), m_DestID(0), m_SrcCamp(0), m_DestCamp(0), m_State(0), m_FormationID(0), m_JungleBuildID(0), m_JungleLocationID(0){};
	~MessageJungle(){};
	virtual void formatMessage(std::stringstream& sstream)
	{
		sstream 
			>> m_SrcID
			>> m_DestID
			>> m_SrcCamp
			>> m_DestCamp
			>> m_FormationID
			>> m_JungleBuildID
			>> m_JungleLocationID
			>> m_State;
	}


	virtual int getMessageType() const { return EMT_JUNGLE; }

private:
	CC_SYNTHESIZE(int, m_SrcID, SrcID);
	CC_SYNTHESIZE(int, m_DestID, DestID);
	CC_SYNTHESIZE(int, m_SrcCamp, SrcCamp);
	CC_SYNTHESIZE(int, m_DestCamp, DestCamp);
	CC_SYNTHESIZE(int, m_FormationID, FormationID);
	CC_SYNTHESIZE(int, m_JungleBuildID, JungleBuildID);
	CC_SYNTHESIZE(int, m_JungleLocationID, JungleLocationID);
	CC_SYNTHESIZE(int, m_State, State);
	

}; 
DEFINE_FACTORY(MessageJungle);

///////////////ʿ�������Ϣ
enum eMoraleComputeType
{
	E_MORALECOMPUTE_NORMAL = 0, //��ͨ���㷽ʽ 
	E_MORALECOMPUTE_GLYPH, //���ļ��㷽ʽ 
	E_MORALECOMPUTE_DEFENCE //��������㷽ʽ,
}; 
class MessageMorale: public GameMessage
{
public:
	MessageMorale(): m_CampType(0), m_MoraleValue(0), m_ComputeType(0){};
	~MessageMorale(){};
	virtual void formatMessage(std::stringstream& sstream)
	{
		sstream
			>> m_CampType
			>> m_MoraleValue
			>> m_ComputeType; 
	} 
	virtual int getMessageType() const { return EMT_MORALE; }
private:
	CC_SYNTHESIZE(int, m_CampType, CampType);
	CC_SYNTHESIZE(float, m_MoraleValue, MoraleValue); 
	CC_SYNTHESIZE(int, m_ComputeType, ComputeType);  
};
DEFINE_FACTORY(MessageMorale);

////////////ʿ����ʾ���
class MessageMoraleShow: public GameMessage
{
public:
	MessageMoraleShow(): m_CampType(0), m_MoraleValue(0), m_SrcID(0){};
	virtual void formatMessage(std::stringstream& sstream)
	{
		sstream
			>> m_CampType
			>> m_MoraleValue
			>> m_SrcID;
	} 
	virtual int getMessageType() const { return EMT_MORALESHOW; }
private:
	CC_SYNTHESIZE(int, m_CampType, CampType);
	CC_SYNTHESIZE(float, m_MoraleValue, MoraleValue);
	CC_SYNTHESIZE(int, m_SrcID, SrcID);
}; 
DEFINE_FACTORY(MessageMoraleShow);


////////////buff�����Ϣ
class MessageBuff: public GameMessage
{
public:
	MessageBuff(): m_ID(0),m_CampType(0), m_BuffSrc(0){};
	~MessageBuff(){};
	virtual void formatMessage(std::stringstream& sstream)
	{
		sstream
			>> m_ID
			>> m_CampType
			>> m_BuffSrc;
	} 
	virtual int getMessageType() const { return EMT_BUFF; }
private: 
	CC_SYNTHESIZE(int, m_ID, ID); //buffID  
	CC_SYNTHESIZE(int, m_CampType, CampType);  //Ŀ������ 
	CC_SYNTHESIZE(int, m_BuffSrc, BuffSrc); //buff��Դ 0--ʿ������  1---Ұ�ִ���
};
DEFINE_FACTORY(MessageBuff);

//////////buff��ʾ
class MessageBuffShow: public GameMessage
{
public:
	MessageBuffShow(): m_ID(0),m_CampType(0), m_CDTime(0), m_BuffSrc(0){};
	~MessageBuffShow(){};
	virtual void formatMessage(std::stringstream& sstream)
	{
		sstream
			>> m_CampType
			>> m_ID
			>> m_CDTime
			>> m_BuffSrc;
	} 
	virtual int getMessageType() const { return EMT_BUFFSHOW; }
private: 
	CC_SYNTHESIZE(int, m_ID, ID); //buffType 
	CC_SYNTHESIZE(int, m_CampType, CampType);  //��Ӫ
	CC_SYNTHESIZE(float, m_CDTime, CDTime); //CDʱ��
	CC_SYNTHESIZE(int, m_BuffSrc, BuffSrc);
};
DEFINE_FACTORY(MessageBuffShow);
 

/////////////����������ʾ
class MessageReducePeople: public GameMessage
{
public:
	MessageReducePeople(): m_BuildID(0), m_Num(0), m_Camp(0){};
	~MessageReducePeople();
	virtual void formatMessage(std::stringstream& sstream)
	{
		sstream
			>> m_BuildID
			>> m_Camp
			>> m_Num; 
	} 
	virtual int getMessageType() const { return EMT_REDUCEPEOPLE; }
	int m_BuildID;
	int m_Num;
	int m_Camp;
private:

}; 
DEFINE_FACTORY(MessageReducePeople);

 
// ��ʯ�ı�
class MessageMoneyChange : public GameMessage
{
public:
	virtual void formatMessage(std::stringstream& sstream)
	{
		sstream >> mRecordData
			>> mChangeOperation
			>> mVCType
			>> mChangeCnt
			>> mCurrentCnt
			>> mReason;
	}

	MessageMoneyChange() : mRecordData(0), mChangeOperation(0), mVCType(0), mChangeCnt(0), mCurrentCnt(0)
	{

	}

	virtual int getMessageType() const { return EMT_MONEY_CHANGE; }

	CC_SYNTHESIZE(int, mRecordData, RecordData); // OSS record, 0 for not, 1 for yes
	CC_SYNTHESIZE(int, mChangeOperation, ChangeOperation); // 1 for add or remove, 2 for set directly
	CC_SYNTHESIZE(int, mVCType, VCType); // 0 for diamond
	CC_SYNTHESIZE(int, mChangeCnt, ChangeCnt);
	CC_SYNTHESIZE(int, mCurrentCnt, CurrentCnt);
	CC_SYNTHESIZE(std::string, mReason, Reason);
};

DEFINE_FACTORY(MessageMoneyChange);

// ����ģʽ����һ��ֱ仯
class MsgResModeScore : public GameMessage
{
public:
	virtual void formatMessage(std::stringstream& sstream){}

	virtual int getMessageType() const { return EMT_RESMODE_SCORE; }
};

DEFINE_FACTORY(MsgResModeScore);

//�����ı�
class MessageEnergyChange : public GameMessage
{
public:
	virtual void formatMessage(std::stringstream& sstream)
	{		
	}

	virtual int getMessageType() const { return EMT_ENERGY_CHANGE; }
};
DEFINE_FACTORY(MessageEnergyChange);

// ������ı�
class MessageHonorChange : public GameMessage
{
public:
	virtual void formatMessage(std::stringstream &sstream)
	{
	
	}
	virtual int getMessageType() const { return EMT_HONOR_CHANGE; }
};
DEFINE_FACTORY(MessageHonorChange);

//���ָı�
class MessagePvpPointChange : public GameMessage
{
public:
	virtual void formatMessage(std::stringstream& sstream)
	{
	}

	virtual int getMessageType() const { return EMT_POINT_CHANGE; }
};
DEFINE_FACTORY(MessagePvpPointChange);

//�ر�UI����
class MessageClosePage: public GameMessage
{
public:
	virtual void formatMessage(std::stringstream& sstream)
	{
		 
	}

	virtual int getMessageType() const { return EMT_CLOSEPAGE;}
	 
};
DEFINE_FACTORY(MessageClosePage);


//ȡ��ƥ��/�޵��߳�����
class MessagePvpClose : public GameMessage
{
public:
	virtual void formatMessage(std::stringstream& sstream)
	{
		sstream >> mResult;
	}

	virtual int getMessageType() const { return EMT_STOP_MATCH; }
	int mResult;//0 �������� 1 �޵��߳������ 2 �޵��߳���ر�
};

DEFINE_FACTORY(MessagePvpClose);

//�ر�pvp����
class MessagePvpDilogClose : public GameMessage
{
public:
	virtual void formatMessage(std::stringstream& sstream)
	{
	}

	virtual int getMessageType() const { return EMT_CLOSE_PVP; }
};

DEFINE_FACTORY(MessagePvpDilogClose);

// �ı�����
class MessageChangeName : public GameMessage
{
public:
	virtual void formatMessage(std::stringstream& sstream)
	{		
		sstream >> mResult;
	}

	virtual int getMessageType() const { return EMT_CHANGE_NAME; }

	int mResult;
};

DEFINE_FACTORY(MessageChangeName);

// ��̖�ı�
class MessageSignalChange : public GameMessage
{
public:
	virtual void formatMessage(std::stringstream& sstream)
	{		
		sstream >> mResult;
	}

	virtual int getMessageType() const { return EMT_SIGNAL_CHANGE; }

	int mResult;
};

DEFINE_FACTORY(MessageSignalChange);

//PVE�������½�
class MessageOpenStage : public GameMessage
{
public:
	virtual void formatMessage(std::stringstream& sstream)
	{
		sstream >> mStage;
	}

	virtual int getMessageType() const { return EMT_OPEN_STAGE; }
	int mStage;
};

DEFINE_FACTORY(MessageOpenStage);


//PVP
class MessageMenuBtn : public GameMessage
{
public:
	virtual void formatMessage(std::stringstream& sstream)
	{
	}

	virtual int getMessageType() const { return EMT_MENU_BTN; }
};

DEFINE_FACTORY(MessageMenuBtn);


class MessageRowDepart: public GameMessage
{
public:
	virtual void formatMessage(std::stringstream& sstream)
	{
		sstream >> mFormationId
			>> mAttackCamp
			>> mDefendCamp
			>> mScrBuild
			>> mDstBuild
			>> mCount;
	}

	virtual int getMessageType() const { return EMT_ROW_DEPART; }

	CC_SYNTHESIZE(int, mFormationId, FormationId);
	CC_SYNTHESIZE(int, mAttackCamp, AttackCamp);
	CC_SYNTHESIZE(int, mDefendCamp, DefendCamp);
	CC_SYNTHESIZE(int, mScrBuild, ScrBuild);
	CC_SYNTHESIZE(int, mDstBuild, DstBuild);
	CC_SYNTHESIZE(int, mCount, Count);
};

DEFINE_FACTORY(MessageRowDepart);

class MessageAchieveGoto: public GameMessage
{
public:
	virtual void formatMessage(std::stringstream& sstream)
	{
		sstream >> mGotoLevel;
	}

	virtual int getMessageType() const { return EMT_ACHIEVE_GOTOLEVEL; }

	CC_SYNTHESIZE(int, mGotoLevel, GotoLevel);
};

DEFINE_FACTORY(MessageAchieveGoto);

class MessageAchieveTip: public GameMessage
{
public:
	virtual void formatMessage(std::stringstream& sstream)
	{
		sstream >> mBtnType >> mIsShow;
	}

	virtual int getMessageType() const { return EMT_HONGDIAN_TIP; }
	CC_SYNTHESIZE(int, mBtnType, BtnType);//��ť���� --->����id
	CC_SYNTHESIZE(bool, mIsShow, IsShow);
};

DEFINE_FACTORY(MessageAchieveTip);

class MessageShopPayComplent: public GameMessage
{
public:
	virtual void formatMessage(std::stringstream& sstream)
	{
		sstream >> mProductId >> mOrderId;
	}

	virtual int getMessageType() const { return EMT_SHOP_PAYCOMPLENT; }

	CC_SYNTHESIZE(int, mProductId, ProductId);
	CC_SYNTHESIZE(std::string, mOrderId, OrderId);
};

DEFINE_FACTORY(MessageShopPayComplent);

class MessageBattleTips : public GameMessage
{
public:
	virtual void formatMessage(std::stringstream& sstream)
	{
		sstream >> mBattleTipMsg >> mPositionX >> mPositionY >> mGlyphMsg;
	}

	virtual int getMessageType() const { return EMT_BATTLE_TIP; }

	CC_SYNTHESIZE(int, mBattleTipMsg, BattleTipMsg);
	CC_SYNTHESIZE(float, mPositionX, PositionX);
	CC_SYNTHESIZE(float, mPositionY, PositionY);
	CC_SYNTHESIZE(std::string, mGlyphMsg, GlyphMsg);
};
DEFINE_FACTORY(MessageBattleTips);

class MessageEnableOption : public GameMessage
{
public:
	virtual void formatMessage(std::stringstream& sstream)
	{
		sstream >> mEnable;
	}

	virtual int getMessageType() const { return EMT_ENABLE_OPTIONBUTTON; }

	CC_SYNTHESIZE(int, mEnable, Enable);
};
DEFINE_FACTORY(MessageEnableOption);

class MessageBtnControl: public GameMessage
{
public:
	virtual void formatMessage(std::stringstream& sstream)
	{
		sstream >> mBtnType >> mIsShow;
	}

	virtual int getMessageType() const { return EMT_BTN_CONTROL; }
	CC_SYNTHESIZE(int, mBtnType, BtnType);//��ť���� --->����id
	CC_SYNTHESIZE(bool, mIsShow, IsShow);
};
DEFINE_FACTORY(MessageBtnControl);


class MessageFormationOver : public GameMessage
{
public:
	virtual void formatMessage(std::stringstream& sstream)
	{
		sstream >> mFormationId
			>> mAttackCamp
			>> mDefendCamp
			>> mScrBuild
			>> mDstBuild
			>> mIsArrived;
	}

	virtual int getMessageType() const { return EMT_FORMATION_OVER; }

	CC_SYNTHESIZE(int, mFormationId, FormationId);
	CC_SYNTHESIZE(int, mAttackCamp, AttackCamp);
	CC_SYNTHESIZE(int, mDefendCamp, DefendCamp);
	CC_SYNTHESIZE(int, mScrBuild, ScrBuild);
	CC_SYNTHESIZE(int, mDstBuild, DstBuild);
	CC_SYNTHESIZE(int, mIsArrived, IsArrived);

};
DEFINE_FACTORY(MessageFormationOver);

class MessageDayMission : public GameMessage
{
public:
	virtual void formatMessage(std::stringstream& sstream)
	{
		sstream >> mMissionId;
	}

	virtual int getMessageType() const { return EMT_MISSION_STATE; }

	CC_SYNTHESIZE(int, mMissionId, MissionId);
};
DEFINE_FACTORY(MessageDayMission);

class MessageActivationTip : public GameMessage
{
public:
	virtual void formatMessage(std::stringstream& sstream)
	{
		sstream >> mStatusId;
	}

	virtual int getMessageType() const { return EMT_ACTIVATION_TIP; }

	CC_SYNTHESIZE(int, mStatusId, StatusId);
};

DEFINE_FACTORY(MessageActivationTip);

class MessageChatMsg : public GameMessage
{
public:
	virtual void formatMessage(std::stringstream& sstream)
	{
		sstream >> mChatType;
	}

	virtual int getMessageType() const { return EMT_CHAT_MSG; }
	CC_SYNTHESIZE(int, mChatType, ChatType);
};

DEFINE_FACTORY(MessageChatMsg);

class MessageFriend : public GameMessage
{
public:
	virtual void formatMessage(std::stringstream& sstream)
	{
		sstream >> mListId;
	}

	virtual int getMessageType() const { return EMT_FRIEND_UPDATE; }

	CC_SYNTHESIZE(int, mListId, ListId);
};
DEFINE_FACTORY(MessageFriend);

class MessageShopBuyItem : public GameMessage
{
public:
	virtual void formatMessage(std::stringstream& sstream)
	{
		sstream >> mItemType;
	}

	virtual int getMessageType() const { return EMT_SHOP_SHOW_BUYITEM; }

	CC_SYNTHESIZE(int, mItemType, ItemType);
};

DEFINE_FACTORY(MessageShopBuyItem);

class MessageAfterBuyDiamond : public GameMessage
{
public:
	virtual void formatMessage(std::stringstream& sstream)
	{
	}

	virtual int getMessageType() const { return EMT_SHOP_AFTER_BUYDIAMOND; }
};

DEFINE_FACTORY(MessageAfterBuyDiamond);

class MessageChangePlayerIcon : public GameMessage
{
public:
	virtual void formatMessage(std::stringstream& sstream)
	{
		sstream >> mIconId;
	}

	virtual int getMessageType() const { return EMT_CHANGE_PLAYER_ICON; }

	CC_SYNTHESIZE(std::string, mIconId, IconId);
};
DEFINE_FACTORY(MessageChangePlayerIcon);

class MessageBufferTips : public GameMessage
{
public:
	virtual void formatMessage(std::stringstream& sstream)
	{
		sstream >> mBufferTipMsg >> mPositionX >> mPositionY;
	}

	virtual int getMessageType() const { return EMT_BUFFER_TIP; }

	CC_SYNTHESIZE(int, mBufferTipMsg, BufferTipMsg);
	CC_SYNTHESIZE(float, mPositionX, PositionX);
	CC_SYNTHESIZE(float, mPositionY, PositionY);
};
DEFINE_FACTORY(MessageBufferTips);


class MessageGameStart: public GameMessage
{
public:
	virtual void formatMessage(std::stringstream& sstream)
	{
		sstream >> mLocationID;
	}
	virtual int getMessageType() const { return EMT_GAMESTART; }
private:
	CC_SYNTHESIZE(int, mLocationID, LocationID);
};
DEFINE_FACTORY(MessageGameStart);


class MessageItemOperation : public GameMessage
{
public:
	virtual void formatMessage(std::stringstream& sstream)
	{
		sstream >> mOperationType
			>> mItemId
			>> mItemType
			>> mItemCnt
			>> mVCCnt
			>> mVCType
			>> mParam;
	}
	virtual int getMessageType() const { return EMT_ITEM_OPERATION; }


	CC_SYNTHESIZE(int, mOperationType, OperationType);	// 0 for buy, 1 for get, 2 for use
	CC_SYNTHESIZE(int, mItemId, ItemId);
	CC_SYNTHESIZE(int, mItemType, ItemType);
	CC_SYNTHESIZE(int, mItemCnt, ItemCnt);
	CC_SYNTHESIZE(int, mVCType, VCType); // 0 for diamond
	CC_SYNTHESIZE(int, mVCCnt, VCCnt);
	CC_SYNTHESIZE(std::string, mParam, Param); // ConsumePoint for buy type, other for reason
};

DEFINE_FACTORY(MessageItemOperation);


class MessagePVPInfo : public GameMessage
{
public:
	virtual void formatMessage(std::stringstream& sstream)
	{
		sstream >> mPVPType
			>> mPVPMode
			>> mOperation;
	}
	virtual int getMessageType() const { return EMT_PVP_INFO; }


	CC_SYNTHESIZE(int, mPVPType, PVPType); // 
	CC_SYNTHESIZE(int, mPVPMode, PVPMode); // 1 for match mode, 2 for room mode
	CC_SYNTHESIZE(int, mOperation, Operation); // 1 for start, 2 for end

};

DEFINE_FACTORY(MessagePVPInfo);

class MessagePropUseAll : public GameMessage
{
public:
	virtual void formatMessage(std::stringstream& sstream)
	{
		sstream >> mPropId;
	}
	virtual int getMessageType() const { return EMT_PROP_USE_ALL; }


	CC_SYNTHESIZE(int, mPropId, PropId);

};

DEFINE_FACTORY(MessagePropUseAll);


class LittleTipsMessage : public GameMessage
{
public:
	virtual void formatMessage(std::stringstream& sstream)
	{
		sstream >> mIsPause;
	}
	virtual int getMessageType() const { return EMT_LITTLETIPS; }
	CC_SYNTHESIZE(bool, mIsPause, IsPause);

};
DEFINE_FACTORY(LittleTipsMessage);


class GetSaveDataMessage : public GameMessage
{
public:
	virtual void formatMessage(std::stringstream& sstream)
	{
		sstream >> mResult;
	}
	virtual int getMessageType() const { return EMT_GETSAVEDATAOVER; }
	CC_SYNTHESIZE(int, mResult, Result);

};
DEFINE_FACTORY(GetSaveDataMessage);

class RqstResultMessage : public GameMessage
{
public:
	virtual void formatMessage(std::stringstream& sstream)
	{
		sstream >> mID >> mResult;
	}
	virtual int getMessageType() const { return EMT_RQST_RESULT; }
	CC_SYNTHESIZE(int, mID, ID); //����ID
	CC_SYNTHESIZE(int, mResult, Result); //��ȷ����0������Ϊ������
};
DEFINE_FACTORY(RqstResultMessage);

class SDKLOGINMessage : public GameMessage
{
public:
	virtual void formatMessage(std::stringstream& sstream)
	{
	}
	virtual int getMessageType() const { return EMT_SDKLOGIN_SUCCESS; }
};
DEFINE_FACTORY(SDKLOGINMessage);

class GAMESERVERLOGINMessage : public GameMessage
{
public:
	virtual void formatMessage(std::stringstream& sstream)
	{
	}
	virtual int getMessageType() const { return EMT_GAMESERVERLOGIN_SUCCESS; }
};
DEFINE_FACTORY(GAMESERVERLOGINMessage);

class SimplifyUIMessage : public GameMessage
{
public:
	virtual void formatMessage(std::stringstream& sstream)
	{
	}
	virtual int getMessageType() const { return EMT_SIMPLIFY_UI; }

};
DEFINE_FACTORY(SimplifyUIMessage);

class MessageUpdatePlayerBadges : public GameMessage
{
public:
	virtual void formatMessage(std::stringstream& sstream)
	{
	}

	virtual int getMessageType() const { return EMT_UPDATE_PLAYER_BADGES; }

};
DEFINE_FACTORY(MessageUpdatePlayerBadges);

class MessageChangePlayerBadge : public GameMessage
{
public:
	virtual void formatMessage(std::stringstream& sstream)
	{
		sstream >> mBadgeId;
	}

	virtual int getMessageType() const { return EMT_CHANGE_PLAYER_BADGE; }
	CC_SYNTHESIZE(int, mBadgeId, BadgeId); 

};
DEFINE_FACTORY(MessageChangePlayerBadge);

class MessageOutPercentBtn : public GameMessage
{
public:
	virtual void formatMessage(std::stringstream& sstream)
	{
		sstream >> mPercent;
	}

	CC_SYNTHESIZE(int, mPercent, Percent); //����ID
	virtual int getMessageType() const { return EMT_BUTTON_OUT_PERCENT; }
};

DEFINE_FACTORY(MessageOutPercentBtn);
//PVP
class MessageOutPercentControl : public GameMessage
{
public:
	virtual void formatMessage(std::stringstream& sstream)
	{
		sstream >> mPercent;
	}

	CC_SYNTHESIZE(int, mPercent, Percent); //����ID
	virtual int getMessageType() const { return EMT_CONTROL_OUT_PERCENT; }
};

DEFINE_FACTORY(MessageOutPercentControl);

class MessagePropUseControl : public GameMessage
{
public:
	virtual void formatMessage(std::stringstream& sstream)
	{
		sstream >> mX >> mY;
	}

	CC_SYNTHESIZE(float, mX, X);	//����x
	CC_SYNTHESIZE(float, mY, Y);	//����y

	virtual int getMessageType() const { return EMT_CONTROL_PROP_USE; }
};

DEFINE_FACTORY(MessagePropUseControl);

class MessageClickPropControl : public GameMessage
{
public:
	virtual void formatMessage(std::stringstream& sstream)
	{
		sstream >> mPropId;
	}

	CC_SYNTHESIZE(int, mPropId, PropId);

	virtual int getMessageType() const { return EMT_CONTROL_CLICK_PROP; }
};

DEFINE_FACTORY(MessageClickPropControl);

class MessageClickPropSecendControl : public GameMessage
{
public:
	virtual void formatMessage(std::stringstream& sstream)
	{
		sstream >> mPropId;
	}

	CC_SYNTHESIZE(int, mPropId, PropId);

	virtual int getMessageType() const { return EMT_CONTROL_CLICK_PROP_SECEND; }
};

DEFINE_FACTORY(MessageClickPropSecendControl);

class MessageCanelPropControl : public GameMessage
{
public:
	virtual void formatMessage(std::stringstream& sstream)
	{
		sstream >> mClickType;
	}

	CC_SYNTHESIZE(int, mClickType, ClickType);

	virtual int getMessageType() const { return EMT_CONTROL_CANCEL_PROP; }
};

DEFINE_FACTORY(MessageCanelPropControl);

class MessageClickStartGameBtn : public GameMessage
{
public:
	virtual void formatMessage(std::stringstream& sstream)
	{
	}

	virtual int getMessageType() const { return EMT_BUTTON_LOGINSCENE; }

};
DEFINE_FACTORY(MessageClickStartGameBtn);

class MessageClickStartGameBtnUnvisible : public GameMessage
{
public:
	virtual void formatMessage(std::stringstream& sstream)
	{
	}

	virtual int getMessageType() const { return EMT_BUTTON_LOGINSCENE_UNVISIBLE; }

};
DEFINE_FACTORY(MessageClickStartGameBtnUnvisible);

class MessageCrazyWeek : public GameMessage
{
public:
	virtual void formatMessage(std::stringstream& sstream)
	{
		sstream >> mCmd;
	}

	virtual int getMessageType() const { return EMT_CRAZY_WEEK; }

	CC_SYNTHESIZE(int, mCmd, Cmd); // 
};
DEFINE_FACTORY(MessageCrazyWeek);

class MessageOpenLevelDialog : public GameMessage
{
public:
	virtual void formatMessage(std::stringstream& sstream)
	{
	}

	virtual int getMessageType() const { return EMT_OPEN_LEVEL_DIALOG; }

};
DEFINE_FACTORY(MessageOpenLevelDialog);

class MessageShopList : public GameMessage
{
public:
	virtual void formatMessage(std::stringstream& sstream)
	{
	}

	virtual int getMessageType() const { return EMT_UPDATE_SHOPLIST; }

};
DEFINE_FACTORY(MessageShopList);

class MessageGW : public GameMessage
{
public:
	virtual void formatMessage(std::stringstream& sstream)
	{
		sstream >> m_iType;
	}

	virtual int getMessageType() const { return EMT_GW; }

	CC_SYNTHESIZE(int, m_iType, Type);

};
DEFINE_FACTORY(MessageGW);

class MessageUpdateExchange : public GameMessage
{
public:
	virtual void formatMessage(std::stringstream &sstream)
	{
		sstream >> mItemId;
	}

	virtual int getMessageType() const { return EMT_UPDATE_EXCHANGE;  }

	CC_SYNTHESIZE(int, mItemId, ItemId);
};
DEFINE_FACTORY(MessageUpdateExchange);

class MessageCloseShop : public GameMessage
{
public:
	virtual void formatMessage(std::stringstream& sstream)
	{
	}

	virtual int getMessageType() const { return EMT_CLOSESHOP; }

};
DEFINE_FACTORY(MessageCloseShop);


class MessageGotoActTab : public GameMessage
{

public:
	virtual void formatMessage(std::stringstream &sstream)
	{
		sstream >> mPage >> mLab;
	}

	virtual int getMessageType() const { return EMT_GOTO_ACTLAB; }

	CC_SYNTHESIZE(int, mPage, Page);
	CC_SYNTHESIZE(int, mLab, Lab);

};

DEFINE_FACTORY(MessageGotoActTab);


class MessageBuyVip : public GameMessage
{
public:
	virtual void formatMessage(std::stringstream& sstream)
	{
		sstream >> mID;
	}

	virtual int getMessageType() const { return EMT_BUY_VIP; }
	CC_SYNTHESIZE(int, mID, ID);
};
DEFINE_FACTORY(MessageBuyVip);

class MessagePlayerUpgrade : public GameMessage
{
public:
	virtual void formatMessage(std::stringstream& sstream)
	{
	}

	virtual int getMessageType() const { return EMT_PLAYER_UPGRADE; }
	
};
DEFINE_FACTORY(MessagePlayerUpgrade);


class MessageGameLoginFlow : public GameMessage
{
public:
	enum FlowType
	{
		ChannelLogin = 1,
		EnterCenterSvr,
		EnterLoginSvr,
		EnterGameSvr,
	};

public:
	virtual void formatMessage(std::stringstream& sstream)
	{
		sstream >> mFlowType >> mResult >> mParam1;
	}

	virtual int getMessageType() const { return EMT_GAME_LOGIN_FLOW; }

	int mFlowType;
	int mResult;
	std::string mParam1;
};

DEFINE_FACTORY(MessageGameLoginFlow);

class MessageRedReward : public GameMessage
{
public:
	virtual void formatMessage(std::stringstream& sstream)
	{
		sstream >> mIsReward;
	}

	virtual int getMessageType() const { return EMT_RED_PACKET_CAN_REWARD; }
	CC_SYNTHESIZE(bool, mIsReward, IsReward);
};
DEFINE_FACTORY(MessageRedReward);

class MessageChangeServer : public GameMessage
{
public:
	virtual void formatMessage(std::stringstream& sstream)
	{
		sstream >> mServerNum;
	}

	virtual int getMessageType() const { return EMT_CHANGE_SERVER; }
	CC_SYNTHESIZE(int, mServerNum, ServerNum);

};
DEFINE_FACTORY(MessageChangeServer);


class MessageSchedule : public GameMessage
{
public:
	virtual void formatMessage(std::stringstream& sstream)
	{
	}
	virtual int getMessageType() const { return EMT_SELECTWORD_SCHEDULE; }

};
DEFINE_FACTORY(MessageSchedule);

class MessageChangeSign : public GameMessage
{
public:
	virtual void formatMessage(std::stringstream& sstream)
	{
	}
	virtual int getMessageType() const { return EMT_CHANGE_SIGN; }

};
DEFINE_FACTORY(MessageChangeSign);

class MessageChangeExtraVisible : public GameMessage
{
public:
	virtual void formatMessage(std::stringstream& sstream)
	{
		sstream >> mIsVisible;
	}
	virtual int getMessageType() const { return EMT_CHANGE_EXTRA_VISIBLE; }
	CC_SYNTHESIZE(int, mIsVisible, IsVisible);

};
DEFINE_FACTORY(MessageChangeExtraVisible);

class MessageGetExtra : public GameMessage
{
public:
	virtual void formatMessage(std::stringstream& sstream)
	{
		sstream >> mTotalPvpCount >> mTotalPveCount >> mTotalPvpUseItem >> mTotalPveUseItem >> mMaxPvpPoint >> mMaxPvpWinCount;
	}
	virtual int getMessageType() const { return EMT_GET_EXTRA; }

	CC_SYNTHESIZE(int, mTotalPvpCount, TotalPvpCount);
	CC_SYNTHESIZE(int, mTotalPveCount, TotalPveCount);
	CC_SYNTHESIZE(int, mTotalPvpUseItem, TotalPvpUseItem);
	CC_SYNTHESIZE(int, mTotalPveUseItem, TotalPveUseItem);
	CC_SYNTHESIZE(int, mMaxPvpPoint, MaxPvpPoint);
	CC_SYNTHESIZE(int, mMaxPvpWinCount, MaxPvpWinCount);
};
DEFINE_FACTORY(MessageGetExtra);

class MessageRecordCreepsBorn : public GameMessage
{
public:
	virtual void formatMessage(std::stringstream& sstream)
	{
		sstream >> mCreepsId >> mHittedCreepsID >> mHitIndex;
	}
	virtual int getMessageType() const { return EMT_RECORD_CREEPS_BORN; }

	CC_SYNTHESIZE(int, mCreepsId, CreepsId);
	CC_SYNTHESIZE(int, mHittedCreepsID, HittedCreepsID);
	CC_SYNTHESIZE(unsigned int, mHitIndex, HitIndex);
};
DEFINE_FACTORY(MessageRecordCreepsBorn);

class MessageRecordOutPercent : public GameMessage
{
public:
	virtual void formatMessage(std::stringstream& sstream)
	{
		sstream >> mOutPercent;
	}
	virtual int getMessageType() const { return EMT_RECORD_OUT_PERCENT; }

	CC_SYNTHESIZE(int, mOutPercent, OutPercent);

};
DEFINE_FACTORY(MessageRecordOutPercent);

class MessageRecordClickProp : public GameMessage
{
public:
	virtual void formatMessage(std::stringstream& sstream)
	{
		sstream >> mPropId;
	}
	virtual int getMessageType() const { return EMT_RECORD_CLICK_PROP; }

	CC_SYNTHESIZE(int, mPropId, PropId);

};
DEFINE_FACTORY(MessageRecordClickProp);

class MessageRecordClickPropSecend : public GameMessage
{
public:
	virtual void formatMessage(std::stringstream& sstream)
	{
		sstream >> mPropId;
	}
	virtual int getMessageType() const { return EMT_RECORD_CLICK_PROP_SECEND; }

	CC_SYNTHESIZE(int, mPropId, PropId);

};
DEFINE_FACTORY(MessageRecordClickPropSecend);

class MessageRecordCancelProp : public GameMessage
{
public:
	virtual void formatMessage(std::stringstream& sstream)
	{
		sstream >> mCancelType;
	}
	virtual int getMessageType() const { return EMT_RECORD_CANCEL_PROP; }

	CC_SYNTHESIZE(int, mCancelType, CancelType);

};
DEFINE_FACTORY(MessageRecordCancelProp);

class MessagePvpResultOpenBox : public GameMessage
{
public:
	virtual void formatMessage(std::stringstream& sstream)
	{
		
	}
	virtual int getMessageType() const { return EMT_PVPRESULT_OPENBOX; }


};
DEFINE_FACTORY(MessagePvpResultOpenBox);

class MessageOpenDailyMission : public GameMessage
{
public:
	virtual void formatMessage(std::stringstream& sstream)
	{
		
	}
	virtual int getMessageType() const { return EMT_OPEN_DAILY_MISSION; }

};
DEFINE_FACTORY(MessageOpenDailyMission);

class MessageDailyMissionTips : public GameMessage
{
public:
	virtual void formatMessage(std::stringstream& sstream)
	{
		sstream >> mTipType >> mIsShow >> mX >> mY;
	}
	virtual int getMessageType() const { return EMT_DALIY_MISSION_TIPS; }
	CC_SYNTHESIZE(int, mTipType, TipType);
	CC_SYNTHESIZE(int, mIsShow, IsShow);
	CC_SYNTHESIZE(float, mX, X);	
	CC_SYNTHESIZE(float, mY, Y);
};
DEFINE_FACTORY(MessageDailyMissionTips);

class MessageDailyMissionProgressBack : public GameMessage
{
public:
	virtual void formatMessage(std::stringstream& sstream)
	{

	}
	virtual int getMessageType() const { return EMT_DAILY_MISSION_PROGRESS_BACK; }

};
DEFINE_FACTORY(MessageDailyMissionProgressBack);

class MessageShowDailyMissionProgress : public GameMessage
{
public:
	virtual void formatMessage(std::stringstream& sstream)
	{

	}
	virtual int getMessageType() const { return EMT_SHOW_DAILY_MISSION_PROGRESS; }

};
DEFINE_FACTORY(MessageShowDailyMissionProgress);

class MessageShareSuccess: public GameMessage
{
public:
	virtual void formatMessage(std::stringstream& sstream)
	{

	}
	virtual int getMessageType() const { return EMT_SHARE_SUCCESS; }

};
DEFINE_FACTORY(MessageShareSuccess);


class MessageSkipSuccess : public GameMessage
{
public:
	virtual void formatMessage(std::stringstream& sstream)
	{

	}
	virtual int getMessageType() const { return EMT_SKIP_SUCCESS; }

};
DEFINE_FACTORY(MessageSkipSuccess);

class MessageInviteBatSwitch : public GameMessage
{
public:
	virtual void formatMessage(std::stringstream& sstream)
	{

	}
	virtual int getMessageType() const { return EMT_INVITE_BAT_SWITCH; }
};
DEFINE_FACTORY(MessageInviteBatSwitch);

class MessageRecordTurretUpdate : public GameMessage
{
public:
	virtual void formatMessage(std::stringstream& sstream)
	{
		sstream >> mBuildId >> mStep;
	}

	CC_SYNTHESIZE(int, mBuildId, BuildId);
	CC_SYNTHESIZE(int, mStep, Step);

	virtual int getMessageType() const { return EMT_RECORD_TURRET_UPDATE; }
};
DEFINE_FACTORY(MessageRecordTurretUpdate);

class MessageRecordExecuteTurretUpdate : public GameMessage
{
public:
	virtual void formatMessage(std::stringstream& sstream)
	{
		sstream >> mBuildId >> mStep;
	}

	CC_SYNTHESIZE(int, mBuildId, BuildId);
	CC_SYNTHESIZE(int, mStep, Step);

	virtual int getMessageType() const { return EMT_RECORD_EXECUTE_TURRET_UPDATE; }
};
DEFINE_FACTORY(MessageRecordExecuteTurretUpdate);


//�ر�UI����
class MessageGuideFirst : public GameMessage
{
public:
	MessageGuideFirst() : m_State(-1){};
	virtual void formatMessage(std::stringstream& sstream)
	{
		sstream >> m_State;
	}

	virtual int getMessageType() const { return EMT_GUIDE_FIRST; }
private:
	CC_SYNTHESIZE(int, m_State, State);
};
DEFINE_FACTORY(MessageGuideFirst);


// �����ı�
class MessageRankChange : public GameMessage
{
public:
	virtual void formatMessage(std::stringstream &sstream)
	{

	}
	virtual int getMessageType() const { return EMT_RANK_CHANGE; }
};
DEFINE_FACTORY(MessageRankChange);
