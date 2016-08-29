#ifndef WeaponBuff_h__
#define WeaponBuff_h__

#include "SceneBuff.h"

class CWeaponBuff : public CSceneBuff
{
public:
	static const string WEAPON_BUFF_BOTTOM_RES;
public:
	CWeaponBuff();
	virtual ~CWeaponBuff();

	CREATE_FUNC(CWeaponBuff);

	void onEnter();
	void onExit();

	void update(float delta);

	int LoadRes(unsigned int uiBuffID, bool isDirectAdd = false);

	void Idle();

	void Die();
private:
	void LoadArmature(const char* resPath, bool isDirectAdd = false);
	void emergeMovementCallBack(CCArmature * armature, MovementEventType type, const char * name);
	void dieMovementCallBack(CCArmature * armature, MovementEventType type, const char * name);
private:
	CCArmature* m_pWeaponArm;		//Ç¹ÐµÄ£ÐÍ
};

#endif // WeaponBuff_h__
