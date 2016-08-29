
#pragma once
#include "Constant.h"
#include "Message.h"
#include "game/BattleInfo.h"


class BaseStar : public MessageHandler
{
public:

	BaseStar() : mEndSuccess(-1)
	{
	}

	virtual ~BaseStar();

	virtual bool init(std::stringstream& ssparams);
	virtual void update(){}
	virtual eStarTag getStarTag() = 0;
	virtual std::string getStarText() = 0;

	virtual int		getEndTag() { return mEndSuccess; }
	virtual void	setEndTag(int statu);
	virtual void	startGameTime(){};

	virtual std::string getIconString(){ return "ui/LevelDetail/LevelDetail_blei.png"; };
protected:

	void	registerMessageType();
	std::string getStarTextByTag(eTextTagStar);

	int		param1;
	std::string params;
	// ��ʼ��-1  δ��ɣ�0  �Ѵ�ɣ�1
	int		mEndSuccess;
};

class StarFactory
{
public:
	virtual BaseStar* create(std::stringstream& ssparams) = 0;
};


#define DEFINE_STAR_FACTORY(_NAME_)	\
class _NAME_##Factory : public StarFactory	\
{\
public:	\
virtual BaseStar* create(std::stringstream& ssparams){\
_NAME_* pret = new _NAME_();\
if (pret && pret->init(ssparams)){\
return pret;\
}\
return NULL;\
}\
};



// ��N����ȡ��ʤ��
class StarSuccessInN : public BaseStar
{
public:
	~StarSuccessInN();
	virtual bool init(std::stringstream& ssparams);
	virtual void update();
	virtual void	startGameTime();
	virtual eStarTag getStarTag() { return STAR_TAG_SUCCESS_IN_N; }
	virtual void onReceiveMassage(GameMessage * message);
	virtual std::string getStarText();
	virtual std::string getIconString(){ return "ui/LevelDetail/LevelDetail_sl.png"; };

};
DEFINE_STAR_FACTORY(StarSuccessInN);

// �������ռ��Ľ�������������N
class EnemyBuildsLN : public BaseStar
{
public: 
	virtual std::string getIconString(){ return "ui/LevelDetail/LevelDetail_blei.png"; };
	virtual eStarTag getStarTag() { return STAR_TAG_ENE_BUILD_L_N; }
	virtual void onReceiveMassage(GameMessage * message);
	virtual std::string getStarText();
};
DEFINE_STAR_FACTORY(EnemyBuildsLN);

// �з���Ӫ��δռ��ָ���Ľ���
class StarBuildNoDestroy : public BaseStar
{
public:
	virtual bool init(std::stringstream& ssparams);
	virtual eStarTag getStarTag() { return STAR_TAG_BUILD_NODESTROY; }
	virtual void onReceiveMassage(GameMessage * message);
	virtual std::string getStarText();

protected:
	int param[5];
};
DEFINE_STAR_FACTORY(StarBuildNoDestroy);


// ʣ��N��������Ӫ�Ľ�������
class StarLastNeutralBuilds : public BaseStar
{
public:
	virtual eStarTag getStarTag() { return STAR_TAG_NEUTRAL_N; }
	virtual void onReceiveMassage(GameMessage * message);
	virtual std::string getStarText();
};

DEFINE_STAR_FACTORY(StarLastNeutralBuilds);

// �κ�һ���������޲�����N
class StarNoPeopleLN : public BaseStar
{
public:
	virtual std::string getIconString(){ return "ui/LevelDetail/LevelDetail_bl.png"; };
	virtual eStarTag getStarTag() { return STAR_TAG_NO_PEOP_MN; }
	virtual void onReceiveMassage(GameMessage * message);
	virtual std::string getStarText();
};
DEFINE_STAR_FACTORY(StarNoPeopleLN);

// ʤ��ʱ��ռ��ָ���Ľ���
class StarGetTarBuilds : public BaseStar
{
public:
	virtual bool init(std::stringstream& ssparams);
	virtual eStarTag getStarTag() { return STAR_TAG_TAR_BUILD; }
	virtual void onReceiveMassage(GameMessage * message);
	virtual std::string getStarText();

	int param[5];
};

DEFINE_STAR_FACTORY(StarGetTarBuilds);


// �������������з�����N
class StarOwnPeopleMEneN : public BaseStar
{
public:
	virtual std::string getIconString(){ return "ui/LevelDetail/LevelDetail_bl.png"; };
	virtual eStarTag getStarTag() { return STAR_TAG_MORE_ENE_N; }
	virtual void onReceiveMassage(GameMessage * message);
	virtual std::string getStarText();

};
DEFINE_STAR_FACTORY(StarOwnPeopleMEneN);

// ������������N
class StarOwnPeopleLN : public BaseStar
{
public:
	virtual std::string getIconString(){ return "ui/LevelDetail/LevelDetail_bl.png"; };
	virtual eStarTag getStarTag() { return STAR_TAG_PEOPLE_L_N; }
	virtual void onReceiveMassage(GameMessage * message);
	virtual std::string getStarText();

};
DEFINE_STAR_FACTORY(StarOwnPeopleLN);

// ����������N
class StarOwnPeopleMN : public BaseStar
{
public:
	virtual std::string getIconString(){ return "ui/LevelDetail/LevelDetail_bl.png"; };
	virtual eStarTag getStarTag() { return STAR_TAG_PEOPLE_M_N; }
	virtual void onReceiveMassage(GameMessage * message);
	virtual std::string getStarText();
};
DEFINE_STAR_FACTORY(StarOwnPeopleMN);


// �з���������N
class StarEnePeopleLN : public BaseStar
{
public:
	virtual std::string getIconString(){ return "ui/LevelDetail/LevelDetail_bl.png"; };
	virtual eStarTag getStarTag() { return STAR_ENE_PEOPLE_L_N; }
	virtual void onReceiveMassage(GameMessage * message);
	virtual std::string getStarText();

};
DEFINE_STAR_FACTORY(StarEnePeopleLN);

// �з�������N
class StarEnePeopleMN : public BaseStar
{
public:
	virtual std::string getIconString(){ return "ui/LevelDetail/LevelDetail_bl.png"; };
	virtual eStarTag getStarTag() { return STAR_ENE_PEOPLE_M_N; }
	virtual void onReceiveMassage(GameMessage * message);
	virtual std::string getStarText();

};
DEFINE_STAR_FACTORY(StarEnePeopleMN);


// ������ʼ������ʧ������N
class StarIniBuildLostLN : public BaseStar
{
public:
	StarIniBuildLostLN()
	{
		mInitBuildLost = 0;
	}
	bool init(std::stringstream& ssparams);
	virtual eStarTag getStarTag() { return STAR_TAG_BUILD_LOST; }
	virtual void onReceiveMassage(GameMessage * message);
	virtual std::string getStarText();

protected:

	int		mInitBuildLost;
	MapBulidList mMineBuilds;
};
DEFINE_STAR_FACTORY(StarIniBuildLostLN);

// ����ӵ�еĽ�������
class StarOwnBuildMN : public BaseStar
{
public:
	virtual eStarTag getStarTag() { return STAR_TAG_MY_BUILD_M_N; }
	virtual void onReceiveMassage(GameMessage * message);
	virtual std::string getStarText();
};
DEFINE_STAR_FACTORY(StarOwnBuildMN);

// ʤ��ʱ����ʣ��ı���
class StarSuccLastPeopleMN : public BaseStar
{
public:
	virtual std::string getIconString(){ return "ui/LevelDetail/LevelDetail_bl.png"; };
	virtual eStarTag getStarTag() { return STAR_TAG_SUCC_LAST_MN; }
	virtual void onReceiveMassage(GameMessage * message);
	virtual std::string getStarText();
};
DEFINE_STAR_FACTORY(StarSuccLastPeopleMN);

// ��Ұ����
class StarMonsterMN : public BaseStar
{
public:
	bool init(std::stringstream& ssparams);
	virtual eStarTag getStarTag() { return STAR_TAG_MONSTER_TIMES; }
	virtual void onReceiveMassage(GameMessage * message);
	virtual std::string getStarText();

	CC_SYNTHESIZE(int, mMonsterTimes, MonsterTimes);
};
DEFINE_STAR_FACTORY(StarMonsterMN);


// ��������ʼ����
class StarNoBuildUpgrade : public BaseStar
{
public:
	virtual bool init(std::stringstream& ssparams);
	virtual eStarTag getStarTag() { return STAR_TAG_BUILD_UPGRADE; }
	virtual void onReceiveMassage(GameMessage * message);
	virtual std::string getStarText();

protected:

	MapBulidList mMineBuilds;
};
DEFINE_STAR_FACTORY(StarNoBuildUpgrade);

// ���ǽ������ı���Ӫʱ��ռ����Ľ���
class StarNoDestroyGetTar : public BaseStar
{
public:
	virtual bool init(std::stringstream& ssparams);
	virtual eStarTag getStarTag() { return STAR_TAG_OCampNoDestroy_GetTar; }
	virtual void onReceiveMassage(GameMessage * message);
	virtual std::string getStarText();

	int param[5];
	int param2[5];
};
DEFINE_STAR_FACTORY(StarNoDestroyGetTar);

// ʿ���ﵽָ��ֵ
class StarMoraleMN : public BaseStar
{
public:
	virtual eStarTag getStarTag() { return STAR_TAG_MORALE_MN; }
	virtual void onReceiveMassage(GameMessage * message);
	virtual std::string getStarText();
};
DEFINE_STAR_FACTORY(StarMoraleMN);