#ifndef PunctureTrap_h__
#define PunctureTrap_h__

#include "SceneTrap.h"

class CPunctureTrap : public CSceneTrap
{
public:
	CPunctureTrap();
	virtual ~CPunctureTrap();

	CREATE_FUNC(CPunctureTrap);

	void onEnter();
	void onExit();

	void update(float delta);

	int LoadRes(unsigned int uiCfgID, bool isDirectAdd = false);

	void Attack(bool isDie);
protected:
	void Warning();
	void AttackAfterWarn();
	void LoadArmature(const char* resPath, bool isDirectAdd = false);
	void handleFrameEvent(CCBone *bone, const char * stEvent, int originFrameIndex, int currentFrameIdx);
	void atkMovementCallBack(CCArmature * armature, MovementEventType type, const char * name);
private:
	float m_fStartTime;
};

#endif // PunctureTrap_h__
