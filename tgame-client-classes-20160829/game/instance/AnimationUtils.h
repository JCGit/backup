#ifndef AnimationUtils_h__
#define AnimationUtils_h__

#include "role/QStageEntity.h"

class CAnimationUtils
{
public:
	//��ȡ������ĳ�������Ĺ����е�֡����
	static CCFrameData* getFrameDataByFrameIdx(CCArmature* pArm, const string stActName, const string stBoneName, const int frameIdx);

	static const CCRect getColliderRect(CCContourData* contour, CCPoint pos, float scale_x = 1.0f, float scale_y = 1.0f);

	//��ȡ�����е��赲���򣬹��Һ��˵�
	static const CCRect getBlockRect(CCArmature* pArm, const string stBoneName = "block");

	//��ȡ�������¼�֡����(ע���¼�ΪΨһ��֡�¼�)
	static CCFrameData* getFrameDataByEvent(CCArmature* pArm, const string stActName, const string stEventName);
};

#endif // AnimationUtils_h__
