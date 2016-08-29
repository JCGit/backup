#ifndef SceneTrap_h__
#define SceneTrap_h__

#include "BaseSceneItem.h"
#include "TrapCfgMaster.h"

class CTrapObj : public CCObject
{
public:
	CREATE_FUNC(CTrapObj);
	int x;
	int y;
	int id;
	int touchId;
	int party;
	int stage;
	bool init(){return true;}
public:
	CTrapObj(){
		x = 0;
		y = 0;
		id = 0;
		touchId = 0;
		party = 0;
		stage = 0;
	}
};


class CSceneTrap : public CBaseSceneItem
{
public:
	static const string SCENE_TRAP_DISAPPEAR;	//陷阱消失

	enum TRAP_STATE{
		TRAP_STATE_IDLE,
		TRAP_STATE_ATTACKING,  //攻击中
		TRAP_STATE_DIE
	};
public:
	CSceneTrap();
	virtual ~CSceneTrap();

	CREATE_FUNC(CSceneTrap);

	virtual void onEnter();
	virtual void onExit();

	virtual void update(float delta);

	virtual int LoadRes(unsigned int uiCfgID, bool isDirectAdd = false);

	virtual void Idle();

	virtual void Attack(bool isDie);

	virtual void resetDamageCnt();
protected:
	virtual void LoadArmature(const char* resPath, bool isDirectAdd = false);
	void handleFrameEvent(CCBone *bone, const char * stEvent, int originFrameIndex, int currentFrameIdx);
	void emergeMovementCallBack(CCArmature * armature, MovementEventType type, const char * name);
	void dieMovementCallBack(CCArmature * armature, MovementEventType type, const char * name);
public:
	TrapCfg m_stCfg;
private:
	CC_SYNTHESIZE(unsigned int, _trapID, TrapID);  //地表buff导致的buff id
	CC_SYNTHESIZE(unsigned int, _ownerParty, OwnerParty);
	CC_SYNTHESIZE(unsigned int, _targetType, TargetType);
	CC_SYNTHESIZE(unsigned int, _state, State);
	CC_SYNTHESIZE(unsigned int, _touchID, TouchID);  //陷阱的触发id

	CC_SYNTHESIZE(unsigned int, _DamageCnt, DamageCnt);			//计算伤害次数;

	float m_fStartTime;
	float m_fBaseScle;
};

#endif // SceneTrap_h__
