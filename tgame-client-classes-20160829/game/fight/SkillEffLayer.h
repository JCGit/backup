#ifndef __SKILL_EFF_LAYER_H__
#define __SKILL_EFF_LAYER_H__
#include <cocos2d.h>
#include "cocos-ext.h"
#include "role/QStageEntity.h"
#include "SkillCfg.pb.h"
#include "EntityFightInfoBrief.h"

using namespace com::cfg::vo;

class CSkillEffLayer:public CCNode, public CQEventDispatcher
{
public:
	CREATE_FUNC(CSkillEffLayer);
	CSkillEffLayer();
	~CSkillEffLayer();

	int LoadRes(unsigned int uiSkillID);
	void LoadArmRes();
	void LoadArmature(const char* resPath);
	void FlyToDir(CCPoint stVec);
	void Fly(bool isLoop);
	void FlyToPos(CCPoint stDst);
	void FlyToPosWithDir(CCPoint stDst);		//飞向具体的一个点，到达后触发下个特效，一般有飞行最大宽度和最短距离
	void Cast(bool isLoop);
	void CastToPos(CCPoint stDst);
	void Boom();   //主动触发爆炸动画
	void update(float delta);
	void onEnter();
	void onExit();
	bool IsFollowType() const;
	bool IsCollideType() const; //是否为撞击型技能
	bool HasCastAnimation();
	bool HasFlyAnimation();
	bool HasHurtAnimation();
	bool IsAreaType();
	bool IsFlying();
	bool IsHiting();
	void PlayHitAnimation(); //播放打击动画，这个跟击中动画不一样的哈，不能混淆
	bool IsChasing() const; //跟踪类型的是否正在追击对象
	void CalcAcceleration();  //计算加速度
	bool IsShouldClear()const{
		return _bShouldClear;
	}

	void Warning(unsigned int skillID);

	void pauseSkill();
	void resumeSkill();
	
	void setScaleX(float fScaleX);
	float getScaleX();

	void setScaleY(float fScaleY);
	float getScaleY();

protected:
	void HandleFrameEvent(CCBone *bone, const char * stEvent, int originFrameIndex, int currentFrameIdx);
	void MovementCallback(CCArmature * armature, MovementEventType type, const char * name);
	void HandlePlaySoundEvent(const string& stEvent, CCObject* pstEventObj);
	void ThrownTrail(float dt);
	void UpdateThrownTrapType(float dt);
	void UpdateDirectFlyType(float dt);
	void UpdateThrownFlyType(float dt);		//抛物线飞行
public:
	static const string HIT_KEY_FRAME_EVENT;
	static const string SPAWN_KEY_FRAME_EVENT;
	static const string PLAY_SOUND_KEY_FRAME_EVENT;
	static const string WARNING_COMPLETE_EVENT;
	static const string SHAKE_SCREEN_KEY_FRAME;		//震屏帧事件

	CC_SYNTHESIZE(CQStageEntity*, _ownerEntity, OwnerEntity);
	CC_SYNTHESIZE(CCPoint, _dstPos, DstPos);
	CC_SYNTHESIZE(CCArmature*, _armature, Armature);	
	CC_SYNTHESIZE(CCPoint, _origPos, OrigPos);
	CC_SYNTHESIZE(CCPoint, _velocity, Velocity);
	CC_SYNTHESIZE(CQStageEntity*, _target, Target);		//跟踪类的目标
	CC_SYNTHESIZE(unsigned int, _flyState, FlyState);	//飞行状态	
	CC_SYNTHESIZE(float, _zalias, Zalias);				//该技能的z轴映射
	CC_SYNTHESIZE(int, _ownerLevel, OwnerLevel);		//该技能的z轴映射
	CC_SYNTHESIZE(int, _ownerParty, OwnerParty);		//技能所属者的阵营信息
	CC_SYNTHESIZE(bool, m_bIsArrive, IsArrive);			//是否飞到目的点

	CC_SYNTHESIZE(int, _currentFrameIdx, CurrentFrameIdx);		//当前播放的帧

	CEntityFightInfoBrief m_stOwnerBrief;
	SkillCfg m_stCfg;
	float _validDeadline;		//该技能最多能有效到啥时候
	bool _bShouldClear;			//表示是否应该清除这哥技能
	CCPoint _acclereration;		//加速度

	map<unsigned int, int> m_stIDToDamgeCnt;		//怪物uID对应伤害次数

	bool IsCanDamge(unsigned int uID);			//判断角色是否受伤
	void DecDamgeCnt(unsigned int uID);			//受伤次数减一

	CC_SYNTHESIZE(float, m_fBasicScale, BaseScale);
};

#endif