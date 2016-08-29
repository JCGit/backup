#ifndef InstanceFightUtils_h__
#define InstanceFightUtils_h__

#include "role/Enemy.h"
#include "fight/BaseFightScene.h"

struct CBurshObj{
	CBaseFightScene* pLayer;
	CEnemyData* pstEnemyObj;
	float iBrushTime;

	CBurshObj(){
		pLayer = NULL;
		pstEnemyObj = NULL;
		iBrushTime = 0;
	}
};

class CInstanceFightUtils
{
public:
	//刷怪逻辑
	static bool BrushMonsterLogic(CBurshObj stBrushObj);
	static bool BrushNormalLogic(CBurshObj stBrushObj);				//普通刷怪
	static bool BrushAfterDieLogic(CBurshObj stBrushObj);			//刷怪点刷出的怪物死亡后再按CD时间刷下一只怪
	static bool BrushSleepLogic(CBurshObj stBrushObj);				//休眠的怪物已经在舞台上
	static bool BrushCarLogic(CBurshObj stBrushObj);				//救护车出场方式
	static bool BrushWalkLogic(CBurshObj stBrushObj);				//从屏幕外走入

	static int MakeKey(int id, int x, int y);

};

#endif // InstanceFightUtils_h__
