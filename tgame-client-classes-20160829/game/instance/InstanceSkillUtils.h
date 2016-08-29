#ifndef InstanceSkillUtils_h__
#define InstanceSkillUtils_h__

#include "role/QStageEntity.h"
#include "fight/BaseFightScene.h"

class CInstanceSkillUtils
{
public:
	static void SkillEffCastLogic(CQStageEntity* pAtker, CBaseFightScene* pLayer, CCPoint stTargePos);

	static float GetSkillAtkDis(CQStageEntity* pAtker, CBaseFightScene* pLayer);

	static CCPoint GetSkillAtPos(CQStageEntity* pAtker, CCPoint stTargePos);
};

#endif // InstanceSkillUtils_h__
