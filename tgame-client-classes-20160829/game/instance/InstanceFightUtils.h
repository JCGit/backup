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
	//ˢ���߼�
	static bool BrushMonsterLogic(CBurshObj stBrushObj);
	static bool BrushNormalLogic(CBurshObj stBrushObj);				//��ͨˢ��
	static bool BrushAfterDieLogic(CBurshObj stBrushObj);			//ˢ�ֵ�ˢ���Ĺ����������ٰ�CDʱ��ˢ��һֻ��
	static bool BrushSleepLogic(CBurshObj stBrushObj);				//���ߵĹ����Ѿ�����̨��
	static bool BrushCarLogic(CBurshObj stBrushObj);				//�Ȼ���������ʽ
	static bool BrushWalkLogic(CBurshObj stBrushObj);				//����Ļ������

	static int MakeKey(int id, int x, int y);

};

#endif // InstanceFightUtils_h__
