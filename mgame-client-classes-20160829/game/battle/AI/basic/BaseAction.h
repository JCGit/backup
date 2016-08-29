#pragma once

#include <vector>
#include "AIState.h"

class BaseBuild;
class BaseAction
{
public:
	BaseAction(){}
	virtual ~BaseAction(){}

	virtual bool init(std::stringstream& ssparams){ return true; }
	virtual bool doAction() = 0;
	virtual eActionTag getActionTag() = 0;

	//enemy 1.�ѷ� 2.�з� 3.���� 
	bool checkCamps(const int srcCamp, const int dstCamp, int relation);
	void getNearBuildings(int keyId, int relation,  std::vector<BaseBuild*>& vecBuild);

	void	setTargetBuild(BaseBuild* pBuild){ mTargetBuild = pBuild; }

	void	setInputBuildings(const std::vector<int >& builds) {
		m_inBuildingIds.resize(builds.size());
		m_inBuildingIds.assign(builds.begin(), builds.end());
	}
	const std::vector<int>& getInputBuildings() { return m_inBuildingIds; }

protected:
	BaseBuild*	mTargetBuild;

	std::vector<int> m_inBuildingIds;

};

class ActionFactory
{
public:
	virtual BaseAction* create(std::stringstream& ssparams) = 0;
};

// ����ai action����
#define DEFINE_ACTION_FACTORY(_NAME_)	\
class _NAME_##Factory : public ActionFactory	\
{\
public:	\
	virtual BaseAction* create(std::stringstream& ssparams){\
_NAME_* pret = new _NAME_();	\
if (pret && pret->init(ssparams)){\
return pret;}\
return NULL; \
}\
};

// ��������
class ActionDoNothing : public BaseAction
{
public:

	virtual eActionTag getActionTag(){ return EAT_DO_NOTHING; }
	virtual bool doAction(){ return  true; }


};

DEFINE_ACTION_FACTORY(ActionDoNothing);


// ȫ������
class ActionAllAttack : public BaseAction
{
public:

	virtual eActionTag getActionTag(){ return EAT_ALL_ATTACK; }
	virtual bool doAction();
};

DEFINE_ACTION_FACTORY(ActionAllAttack);

// ��������
class ActionBuildUpgrade : public BaseAction
{
public:

	virtual eActionTag getActionTag(){ return EAT_BUILD_UPGRADE; }
	virtual bool doAction();
};

DEFINE_ACTION_FACTORY(ActionBuildUpgrade);

// �������ͱ�
class ActionShiftNum : public BaseAction
{
public:
	virtual bool init(std::stringstream& ssparams);
	virtual eActionTag getActionTag(){ return EAT_SHIFT1; }
	virtual bool doAction();


protected:

	int getRuleBuildIns();
	// ���ܵĹ�������id
	std::vector<int> vParam;
	// �ͱ�����������
	int	mShiftParam;
};

DEFINE_ACTION_FACTORY(ActionShiftNum);

// ���ٷֱ��ͱ�
class ActionShiftRate : public ActionShiftNum
{
public:
	virtual eActionTag getActionTag(){ return EAT_SHIFT2; }
	virtual bool doAction();
};

DEFINE_ACTION_FACTORY(ActionShiftRate);

// ����������
class ActionAttackNum : public BaseAction
{
public:
	virtual bool init(std::stringstream& ssparams);
	virtual eActionTag getActionTag(){ return EAT_ATTACK1; }
	virtual bool doAction();

protected:

	int getRuleBuildIns();

	int param[6];
};

DEFINE_ACTION_FACTORY(ActionAttackNum);

// ���ٷֱȽ���
class ActionAttackRate : public ActionAttackNum
{
public:
	virtual bool init(std::stringstream& ssparams);
	virtual eActionTag getActionTag(){ return EAT_ATTACK2; }
	virtual bool doAction();

};

DEFINE_ACTION_FACTORY(ActionAttackRate);

// ��������Ԯ
class ActionHelpNum : public BaseAction
{
public:
	virtual bool init(std::stringstream& ssparams);
	virtual eActionTag getActionTag(){ return EAT_HELP1; }
	virtual bool doAction();

protected:
	// ����ܹ����Ľ���
	// @type 0-���� 1-����
	// @numLimit ��Ԯ�����ı����ﵽnumLimit��ִ��
	int getRuleBuildKey(int type, int limit);

	int param1;
	int param2;		// ��Ԯ�����ı�������������Ϊ�����������
};

DEFINE_ACTION_FACTORY(ActionHelpNum);

// ���ٷֱȾ�Ԯ
class ActionHelpRate : public ActionHelpNum
{
public:
	virtual eActionTag getActionTag(){ return EAT_HELP2; }
	virtual bool doAction();
};

DEFINE_ACTION_FACTORY(ActionHelpRate);

// ��Ұ �������ɱ�
class ActionMonsterNum : public ActionAttackNum
{
public:
	virtual bool init(std::stringstream& ssparams);
	virtual eActionTag getActionTag(){ return EAT_MONSTER_ATTACK1; }
	virtual bool doAction();

protected:

	int param1;
};

DEFINE_ACTION_FACTORY(ActionMonsterNum);


class ActionMonsterRate: public ActionAttackNum
{
public:
	virtual bool init(std::stringstream& ssparams);
	virtual eActionTag getActionTag(){ return EAT_MONSTER_ATTACK2; }
	virtual bool doAction();
};
DEFINE_ACTION_FACTORY(ActionMonsterRate);

//
class ActionAttackRange : public BaseAction
{
public:
	virtual bool init(std::stringstream& ssparams);
	virtual eActionTag getActionTag(){ return EAT_ATTACK_RANGE; }
	virtual bool doAction();

protected:
	virtual BaseBuild* getMinimalBuild();
	int param1;
	int param2;	
};
DEFINE_ACTION_FACTORY(ActionAttackRange);

class ActionAttackNearest : public BaseAction
{
public:
	virtual bool init(std::stringstream& ssparams);
	virtual eActionTag getActionTag(){ return EAT_ATTACK_NEAREST; }
	virtual bool doAction();

protected:
	virtual BaseBuild* getNearestBuild();
	int param1;
};
DEFINE_ACTION_FACTORY(ActionAttackNearest);


class ActionAttackRateRange : public ActionAttackRange
{
public:
	virtual bool init(std::stringstream& ssparams);
	virtual eActionTag getActionTag(){ return EAT_ATTACK_RATE_RANGE; }
	virtual bool doAction();

protected:
	int param1;
	int param2;
};
DEFINE_ACTION_FACTORY(ActionAttackRateRange);

class ActionAttackRateNearest : public ActionAttackNearest
{
public:
	virtual bool init(std::stringstream& ssparams);
	virtual eActionTag getActionTag(){ return EAT_ATTACK_RATE_NEAREST; }
	virtual bool doAction();

protected:
	int param1;
};
DEFINE_ACTION_FACTORY(ActionAttackRateNearest);



class ActionPropSildier : public BaseAction
{
public:
	bool init(std::stringstream& ssparams);
	virtual eActionTag getActionTag(){ return EAT_PROP_SILDIER; }
	virtual bool doAction();
	virtual BaseBuild* getBuildId();
protected:
	int param1;
};
DEFINE_ACTION_FACTORY(ActionPropSildier);

class ActionPropSildierType : public ActionPropSildier
{
public:
	virtual eActionTag getActionTag(){ return EAT_PROP_SILDIER_TYPE; }
	BaseBuild* getBuildId();
};
DEFINE_ACTION_FACTORY(ActionPropSildierType);

class ActionProBoom :public BaseAction
{
public:
	virtual eActionTag getActionTag(){ return EAT_PROP_BOOM; }
	virtual bool doAction();
};
DEFINE_ACTION_FACTORY(ActionProBoom);

class ActionProIce :public BaseAction
{
public:
	virtual eActionTag getActionTag(){ return EAT_PROP_ICE; }
	virtual bool doAction();
};
DEFINE_ACTION_FACTORY(ActionProIce);

class ActionProOil :public BaseAction
{
public:
	virtual eActionTag getActionTag(){ return EAT_PROP_OIL; }
	virtual bool doAction();
};
DEFINE_ACTION_FACTORY(ActionProOil);