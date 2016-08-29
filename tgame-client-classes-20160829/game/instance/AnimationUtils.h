#ifndef AnimationUtils_h__
#define AnimationUtils_h__

#include "role/QStageEntity.h"

class CAnimationUtils
{
public:
	//获取动画中某个动作的骨骼中的帧数据
	static CCFrameData* getFrameDataByFrameIdx(CCArmature* pArm, const string stActName, const string stBoneName, const int frameIdx);

	static const CCRect getColliderRect(CCContourData* contour, CCPoint pos, float scale_x = 1.0f, float scale_y = 1.0f);

	//获取动画中的阻挡区域，勾兑好了的
	static const CCRect getBlockRect(CCArmature* pArm, const string stBoneName = "block");

	//获取动画中事件帧数据(注：事件为唯一的帧事件)
	static CCFrameData* getFrameDataByEvent(CCArmature* pArm, const string stActName, const string stEventName);
};

#endif // AnimationUtils_h__
