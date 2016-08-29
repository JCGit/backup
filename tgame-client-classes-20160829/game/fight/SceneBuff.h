#ifndef SceneBuff_h__
#define SceneBuff_h__

#include "BaseSceneItem.h"
#include "FloorBuffCfgMaster.h"

class CFloorBuffObj : public CCObject
{
public:
	CREATE_FUNC(CFloorBuffObj);
	int x;
	int y;
	int id;
	int touchId;
	int party;
	int stage;
	bool init(){return true;}
public:
	CFloorBuffObj(){
		x = 0;
		y = 0;
		id = 0;
		touchId = 0;
		party = 0;
		stage = 0;
	}
};


class CSceneBuff : public CBaseSceneItem
{
public:
	static const string SCENE_BUFF_DISAPPEAR;	//buff消失
public:
	CSceneBuff();
	virtual ~CSceneBuff();

	CREATE_FUNC(CSceneBuff);

	virtual void onEnter();
	virtual void onExit();

	virtual void update(float delta);

	virtual int LoadRes(unsigned int uiBuffID, bool isDirectAdd = false);

	virtual void Idle();

	virtual void Die();
private:
	void LoadArmature(const char* resPath, bool isDirectAdd = false);
	void emergeMovementCallBack(CCArmature * armature, MovementEventType type, const char * name);
	void dieMovementCallBack(CCArmature * armature, MovementEventType type, const char * name);
public:
	FloorBuffCfg m_stCfg;
private:
	CC_SYNTHESIZE(unsigned int, _buffID, BuffID);  //地表buff导致的buff id
	CC_SYNTHESIZE(unsigned int, _ownerParty, OwnerParty);
	CC_SYNTHESIZE(unsigned int, _targetType, TargetType);
	CC_SYNTHESIZE(bool, _isDie, IsDie);

	float m_fStartTime;
	float m_fBaseScle;
};

#endif // SceneBuff_h__
