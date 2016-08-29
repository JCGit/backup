#pragma once

#include "AIState.h"
#include "battle/Build.h"


class BaseState:public Ref
{
public:

	BaseState() :mTime(0), mLimitTime(0), misLimit(false), mCanDo(false){}
	virtual ~BaseState(){}
	
	virtual bool init(std::stringstream& ssparams);
	virtual eStateTag getStateTag() = 0;
	virtual bool checkState() = 0;

	void	setTargetBuild(BaseBuild* pBuild);
	BaseBuild * getTargetBuild();
	eCampType getOwnCamp();

	bool checkCamps(const int srcCamp, const int dstCamp, int relation);

	void setOutputBuildings(const std::vector<int>& builds) {
		m_outputBuildings.resize(builds.size());
		m_outputBuildings.assign(builds.begin(), builds.end());
		for (const auto& iter : m_outputBuildings)
		{
			CCLOG("output buildId:%d", iter);
		}
	}
	const std::vector<int>& getOutputBuildings(){ return m_outputBuildings; }

protected:
	int mTargetBuildId;

	int param1;
	int param2;
	int param3;

	std::vector<int> m_outputBuildings;
	CC_SYNTHESIZE(int, mTime, Time);          //��ǰʱ��
	CC_SYNTHESIZE(int, mLimitTime, LimitTime);//����ʱ��
	CC_SYNTHESIZE(bool, misLimit, IsLimitTime);//�Ƿ�����
	CC_SYNTHESIZE(bool, mCanDo, Cando);			//�Ƿ�ִ��
};

class StateFactory
{
public:
	virtual BaseState* create(std::stringstream& szparams) = 0;
};

#define DEFINE_STATE_FACTORY(_NAME_)	\
class _NAME_##Factory : public StateFactory	\
{\
public:	\
	virtual BaseState* create(std::stringstream& szparams){\
	_NAME_* pret = new _NAME_();	\
	if (pret && pret->init(szparams)){\
		return pret;	\
	}	\
		return NULL;\
	}	\
};


// �����Ƿ������
class StateUpgradeBuild : public BaseState
{
public:

	virtual eStateTag getStateTag(){ return EST_Build_CanUpgrade; }
	virtual bool checkState();
};

DEFINE_STATE_FACTORY(StateUpgradeBuild);

// ���������ﵽ�������޵İٷֱ�
class StateProductRate : public BaseState
{
public:

	virtual eStateTag getStateTag(){ return EST_Build_ProductRate; }
	virtual bool checkState();
};
DEFINE_STATE_FACTORY(StateProductRate);

// �����ܱ����ﵽ
class CampPeopleMoreN : public BaseState
{
public:

	virtual eStateTag getStateTag(){ return EST_Camp_MorePeople; }
	virtual bool checkState();
};
DEFINE_STATE_FACTORY(CampPeopleMoreN);

// ���������ﵽ�������޵İٷֱ�
class HoldPeopleMoreN : public BaseState
{
public:

	virtual eStateTag getStateTag(){ return EST_Build_HoldRate; }
	virtual bool checkState();
};
DEFINE_STATE_FACTORY(HoldPeopleMoreN);

// ����N�񼺷������ܵ�����ʱ
class BuildInNBeAttack : public BaseState
{
public:

	virtual eStateTag getStateTag(){ return EST_Builds_BeAttN; }
	virtual bool checkState();
};
DEFINE_STATE_FACTORY(BuildInNBeAttack);

// ����N�������������Ӫʱ
class NeutralBuildInN : public BaseState
{
public:

	virtual eStateTag getStateTag(){ return EST_N_NeutralBuild; }
	virtual bool checkState(); 
	
};
DEFINE_STATE_FACTORY(NeutralBuildInN);

// �������������з�����N, �෽��ս�򳬹�����ΪN
class PeopleMoreEnemyN : public BaseState
{
public:

	virtual eStateTag getStateTag(){ return EST_N_MorePeople; }
	virtual bool checkState();
};
DEFINE_STATE_FACTORY(PeopleMoreEnemyN);


// ����ռ��Ľ�����������N
class HaveBuildMoreN : public BaseState
{
public:
	virtual bool init(std::stringstream& ssparams);

	virtual eStateTag getStateTag(){ return EST_N_Builds; }
	virtual bool checkState();
};
DEFINE_STATE_FACTORY(HaveBuildMoreN);

// �����ѷ��������������ʱ
class OAttON : public BaseState
{
public:

	virtual eStateTag getStateTag(){ return EST_Battle_Other; }
	virtual bool checkState();
};
DEFINE_STATE_FACTORY(OAttON);

// �������ɴ�Ұ��
class OccuMonster : public BaseState
{
public:

	virtual eStateTag getStateTag(){ return EST_D_Monster; }
	virtual bool checkState();
};
DEFINE_STATE_FACTORY(OccuMonster);


// �������ɸ���ڵĵз���������N
class EneBuildsMoreN : public BaseState
{
public:
	virtual bool init(std::stringstream& ssparams);
	virtual eStateTag getStateTag(){ return EST_D_Builds; }
	virtual bool checkState();
};
DEFINE_STATE_FACTORY(EneBuildsMoreN);


// �������ɸ���ڵĵз�������������N
class EnePeopleMoreN : public BaseState
{
public:
	virtual bool init(std::stringstream& ssparams);
	virtual eStateTag getStateTag(){ return EST_D_MorePeople; }
	virtual bool checkState();
};
DEFINE_STATE_FACTORY(EnePeopleMoreN);

// �������ɸ���ڵĵз���������С��N
class EnePeopleLessN : public BaseState
{
public:
	virtual bool init(std::stringstream& ssparams);
	virtual eStateTag getStateTag(){ return EST_D_LessPeople; }
	virtual bool checkState();
};
DEFINE_STATE_FACTORY(EnePeopleLessN);

// ���м�������û���ܵ��κι���
class BuildsNoAttack : public BaseState
{
public:

	virtual eStateTag getStateTag(){ return EST_NO_BeAttack; }
	virtual bool checkState();
};
DEFINE_STATE_FACTORY(BuildsNoAttack);


// ��ǰ�����ڵı����ﵽָ��ֵ
class BuildPeopleNumN : public BaseState
{
public:

	virtual eStateTag getStateTag(){ return EST_Build_PeopleNum; }
	virtual bool checkState();
};
DEFINE_STATE_FACTORY(BuildPeopleNumN);

// N��Χ�ڴ��ڱ�������M�Ľ���
class PeopleCountInN : public BaseState
{
public:
	bool init(std::stringstream& ssparams);
	virtual eStateTag getStateTag(){ return EST_PeopleCountInN; }
	virtual bool checkState();

private:
	int param4;
};
DEFINE_STATE_FACTORY(PeopleCountInN);

//����ʿ������M
class HaveXXbuffer : public BaseState
{
public:
	virtual eStateTag getStateTag(){ return EST_HaveXXbuffer; }
	virtual bool checkState();
};
DEFINE_STATE_FACTORY(HaveXXbuffer);

//����ʿ������M
class MoraleGtN : public BaseState
{
public:
	bool init(std::stringstream& ssparams);
	virtual eStateTag getStateTag(){ return EST_MoraleGtN; }
	virtual bool checkState();
};
DEFINE_STATE_FACTORY(MoraleGtN);

//������û���յ��˺�	
class OwnerNotDamage : public BaseState
{
public:
	virtual eStateTag getStateTag(){ return EST_OwnerNotDamage; }
	virtual bool checkState();
};
DEFINE_STATE_FACTORY(OwnerNotDamage);

//�����ܱ�������M
class OwnerCampPeople : public BaseState
{
public:
	bool init(std::stringstream& ssparams);
	virtual eStateTag getStateTag(){ return EST_OwnerCampPeople; }
	virtual bool checkState();
};
DEFINE_STATE_FACTORY(OwnerCampPeople);



//ָ������������Ԯָ��
class RunTransformCommand : public BaseState
{
public:
	bool init(std::stringstream& ssparams);
	virtual eStateTag getStateTag(){ return EST_RunTransformCommand; }
	virtual bool checkState();
};
DEFINE_STATE_FACTORY(RunTransformCommand);

//ʣ��ʱ������M��
class CountDownSurplus : public BaseState
{
public:
	bool init(std::stringstream& ssparams);
	virtual eStateTag getStateTag(){ return EST_CountDownSurplus; }
	virtual bool checkState();
};
DEFINE_STATE_FACTORY(CountDownSurplus);


//EST_BuildSoldierNum,	//ָ����������
//EST_EnemySubDstSoldierNum,	//�з��򱾽����ɱ�����פ�����Ĳ��
//EST_EnemySubFriendSoldierNum, //�з����ѷ������ɱ�����פ�����Ĳ��
//EST_EnemySubOtherSoldierNum, //�з���������ɱ�����פ�����Ĳ��
//EST_EnemyBuildAfterSendNum, //�з��ɱ�֮�������
//EST_EnemyBuildAfterReceiveNum, //�з��ܵ�֮�������
//
//ָ����������
class BuildSoldierNum : public BaseState
{
public:
	bool init(std::stringstream& ssparams);
	virtual eStateTag getStateTag(){ return EST_BuildSoldierNum; }
	virtual bool checkState();
private:
	int param4;
};
DEFINE_STATE_FACTORY(BuildSoldierNum);


class EnemySubDstSoldierNum : public BaseState
{
public:
	bool init(std::stringstream& ssparams);
	virtual eStateTag getStateTag(){ return EST_EnemySubDstSoldierNum; }
	virtual bool checkState();
};
DEFINE_STATE_FACTORY(EnemySubDstSoldierNum);


class EnemySubFriendSoldierNum : public BaseState
{
public:
	bool init(std::stringstream& ssparams);
	virtual eStateTag getStateTag(){ return EST_EnemySubFriendSoldierNum; }
	virtual bool checkState();
};
DEFINE_STATE_FACTORY(EnemySubFriendSoldierNum);

class EnemySubOtherSoldierNum : public BaseState
{
public:
	bool init(std::stringstream& ssparams);
	virtual eStateTag getStateTag(){ return EST_EnemySubOtherSoldierNum; }
	virtual bool checkState();
};
DEFINE_STATE_FACTORY(EnemySubOtherSoldierNum);

class EnemyBuildAfterSendNum : public BaseState
{
public:
	bool init(std::stringstream& ssparams);
	virtual eStateTag getStateTag(){ return EST_EnemyBuildAfterSendNum; }
	virtual bool checkState();
};
DEFINE_STATE_FACTORY(EnemyBuildAfterSendNum);

class EnemyBuildAfterReceiveNum : public BaseState
{
public:
	bool init(std::stringstream& ssparams);
	virtual eStateTag getStateTag(){ return EST_EnemyBuildAfterReceiveNum; }
	virtual bool checkState();
};
DEFINE_STATE_FACTORY(EnemyBuildAfterReceiveNum);



//�������ȼ�
class SelfBuildLevel : public BaseState
{
public:
	bool init(std::stringstream& ssparams);
	virtual eStateTag getStateTag(){ return EST_SelfBuildLevel; }
	virtual bool checkState();
};
DEFINE_STATE_FACTORY(SelfBuildLevel);



//����ռ���M����Ӫ���������о�����������R��С�ڡ����ڵ��ڣ�N
class BuildTypeNum : public BaseState
{
public:
	bool init(std::stringstream& ssparams);
	virtual eStateTag getStateTag(){ return EST_BuildTypeNum; }
	virtual bool checkState();
};
DEFINE_STATE_FACTORY(BuildTypeNum);


class TimeCompare :public BaseState
{
public:
	bool init(std::stringstream & ssparams);
	virtual eStateTag getStateTag(){ return EST_TimeCompareLeft; }
	virtual bool checkState();
};
DEFINE_STATE_FACTORY(TimeCompare);

class SoldierNumCompareALL :public BaseState
{
public:
	bool init(std::stringstream & ssparams);
	virtual eStateTag getStateTag(){ return EST_SoldierNumCompareALL; }
	virtual bool checkState();
};
DEFINE_STATE_FACTORY(SoldierNumCompareALL);


class SildierNumOrBuildNum :public BaseState
{
public:
	bool init(std::stringstream & ssparams);
	virtual eStateTag getStateTag(){ return EST_SildierNumOrBuildNum; }
	virtual bool checkState();
};
DEFINE_STATE_FACTORY(SildierNumOrBuildNum);

class PlayerPropCount :public BaseState
{
public:
	bool init(std::stringstream & ssparams);
	virtual eStateTag getStateTag(){ return EST_PlayerPropCount; }
	virtual bool checkState();
};
DEFINE_STATE_FACTORY(PlayerPropCount);