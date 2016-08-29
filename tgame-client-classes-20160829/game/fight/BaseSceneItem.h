#ifndef BaseSceneItem_h__
#define BaseSceneItem_h__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "vlib/QEventDispatcher.h"

USING_NS_CC;
USING_NS_CC_EXT;

class CBaseSceneItem : public CCNode, public CQEventDispatcher
{
public:
	enum BASE_SCENE_ITEM_TYPE{
		TYPE_FLOOR_BUFF,
		TYPE_SCENE_TRAP
	};
public:
	CBaseSceneItem();
	virtual ~CBaseSceneItem();

	CREATE_FUNC(CBaseSceneItem);

	virtual void onEnter();
	virtual void onExit();
protected:
	CC_SYNTHESIZE(CCArmature*, _armature, Armature);	
	CC_SYNTHESIZE(int, _currentFrameIdx, CurrentFrameIdx);		//当前播放的帧

	unsigned int m_uType;			//类型
private:
};


#endif // BaseSceneItem_h__
