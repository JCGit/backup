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
	void FlyToPosWithDir(CCPoint stDst);		//��������һ���㣬����󴥷��¸���Ч��һ���з�������Ⱥ���̾���
	void Cast(bool isLoop);
	void CastToPos(CCPoint stDst);
	void Boom();   //����������ը����
	void update(float delta);
	void onEnter();
	void onExit();
	bool IsFollowType() const;
	bool IsCollideType() const; //�Ƿ�Ϊײ���ͼ���
	bool HasCastAnimation();
	bool HasFlyAnimation();
	bool HasHurtAnimation();
	bool IsAreaType();
	bool IsFlying();
	bool IsHiting();
	void PlayHitAnimation(); //���Ŵ����������������ж�����һ���Ĺ������ܻ���
	bool IsChasing() const; //�������͵��Ƿ�����׷������
	void CalcAcceleration();  //������ٶ�
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
	void UpdateThrownFlyType(float dt);		//�����߷���
public:
	static const string HIT_KEY_FRAME_EVENT;
	static const string SPAWN_KEY_FRAME_EVENT;
	static const string PLAY_SOUND_KEY_FRAME_EVENT;
	static const string WARNING_COMPLETE_EVENT;
	static const string SHAKE_SCREEN_KEY_FRAME;		//����֡�¼�

	CC_SYNTHESIZE(CQStageEntity*, _ownerEntity, OwnerEntity);
	CC_SYNTHESIZE(CCPoint, _dstPos, DstPos);
	CC_SYNTHESIZE(CCArmature*, _armature, Armature);	
	CC_SYNTHESIZE(CCPoint, _origPos, OrigPos);
	CC_SYNTHESIZE(CCPoint, _velocity, Velocity);
	CC_SYNTHESIZE(CQStageEntity*, _target, Target);		//�������Ŀ��
	CC_SYNTHESIZE(unsigned int, _flyState, FlyState);	//����״̬	
	CC_SYNTHESIZE(float, _zalias, Zalias);				//�ü��ܵ�z��ӳ��
	CC_SYNTHESIZE(int, _ownerLevel, OwnerLevel);		//�ü��ܵ�z��ӳ��
	CC_SYNTHESIZE(int, _ownerParty, OwnerParty);		//���������ߵ���Ӫ��Ϣ
	CC_SYNTHESIZE(bool, m_bIsArrive, IsArrive);			//�Ƿ�ɵ�Ŀ�ĵ�

	CC_SYNTHESIZE(int, _currentFrameIdx, CurrentFrameIdx);		//��ǰ���ŵ�֡

	CEntityFightInfoBrief m_stOwnerBrief;
	SkillCfg m_stCfg;
	float _validDeadline;		//�ü����������Ч��ɶʱ��
	bool _bShouldClear;			//��ʾ�Ƿ�Ӧ�������缼��
	CCPoint _acclereration;		//���ٶ�

	map<unsigned int, int> m_stIDToDamgeCnt;		//����uID��Ӧ�˺�����

	bool IsCanDamge(unsigned int uID);			//�жϽ�ɫ�Ƿ�����
	void DecDamgeCnt(unsigned int uID);			//���˴�����һ

	CC_SYNTHESIZE(float, m_fBasicScale, BaseScale);
};

#endif