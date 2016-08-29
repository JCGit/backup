#ifndef SceneEffect_h__
#define SceneEffect_h__

#include "cocos2d.h"
#include "cocos-ext.h"

#include "SceneEffCfgMaster.h"
#include "vlib/QEventDispatcher.h"
using namespace CVLib;

USING_NS_CC;
USING_NS_CC_EXT;

using namespace std;

//场景特效类型 
enum SCENE_EFFECT_TYPE{
	EFFECT_TYPE_YUNSHI = 1,		//陨石
	EFFECT_TYPE_DRIFT = 2,		//神马从天空飘过,只在当前屏
	EFFECT_TYPE_DUST = 3,		//灰尘
};

class CSceneEffect : public CCNode, public CQEventDispatcher
{
public:
	static const string SHAKE_SCREEN_KEY_FRAME;		//震屏帧事件
	static const string DUST_PLAY_KEY_FRAME;		//灰尘帧事件
	static const string SCENE_EFFECT_SOUND_FRAME;	//场景特效音效帧事件
public:
	CSceneEffect();
	~CSceneEffect();

	CREATE_FUNC(CSceneEffect);

	void onEnter();
	void onExit();

	void update(float delta);

	int LoadRes(unsigned int uiEffectID);
	void LoadArmature(const char* resPath);

	void play(bool isLoop);

	unsigned int getEffType();
	unsigned int getNextEffID();

protected:
private:
	void HandleFrameEvent(CCBone *bone, const char * stEvent, int originFrameIndex, int currentFrameIdx);
	void MovementCallback(CCArmature * armature, MovementEventType type, const char * name);
public:
	CC_SYNTHESIZE(CCArmature*, _armature, Armature);	

	SceneEffCfg m_stCfg;
};

#endif // SceneEffect_h__
