
#ifndef _ASSISTANT_VIEW_H
#define _ASSISTANT_VIEW_H

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"

class AssistantView : public cocos2d::Node
{
public:
	enum AniEvent
	{
		kCastSkillStart,
		kCastSkill,
		kCastSkillEnd,
	};
	typedef std::function<void(AniEvent)> AniEventCallback;

public:
	static AssistantView* create(const std::string& aniName);

	void setAniEventCallback(const AniEventCallback& callback);

	void onIdle();
	void onCastSkill();

	bool isCastingSkill() const;
	void playMoveAciton(Vec2 pos, float scale);
	void stopAnimtion();

protected:
	AssistantView();
	virtual ~AssistantView();

	virtual bool init(const std::string& aniFilename);

	void sendAniEvent(AniEvent eventType);

	void onAniMovementEventCallback(cocostudio::Armature *pAr, cocostudio::MovementEventType type, const std::string& id);
	void onAniFrameEventCallback(cocostudio::Bone *bone, const std::string& evt, int originFrameIndex, int currentFrameIndex);

protected:
	enum LZO {
		kDefualt = 0,
		kAnimation = 1,
	};

	static const std::string ActionKeyWalk;
	static const std::string ActionKeyStand;
	static const std::string ActionKeyCastSkill;
	static const float ArcActionDeltaAngle;
	static const float ArcActionDuration;
	static const float ArcActionDelayTime;
	static const std::string FrameKeyCastSkill;

	std::string mAniFilename;
	cocostudio::Armature* mAnimation;

	AniEventCallback mAniEventCallback;
};


/*
Բ��������
*/
class CCArcBy : public cocos2d::CCActionInterval
{
public:
	//��ʼ��Բ��������
	//duration: ������ĳ���ʱ��
	//ptCenter: Բ�������ĵ�
	//deltaAngle: ���ȵı仯��������������ʾ��ʱ���˳ʱ�뷽��
	bool initWithDuration(float duration, const cocos2d::Vec2& ptCenter, float deltaAngle);

	//virtual CCObject* copyWithZone(cocos2d::CCZone* pZone);
	virtual void startWithTarget(cocos2d::CCNode *pTarget);
	virtual CCActionInterval* reverse(void)const override;
	virtual CCActionInterval* clone() const override;
	virtual void update(float time);

public:
	static CCArcBy* create(float duration, const cocos2d::Vec2& ptCenter, float deltaAngle);

protected:
	cocos2d::Vec2 m_startPosition;
	cocos2d::Vec2 m_previousPosition;

	cocos2d::Vec2 m_ptCenter;
	float            m_fAngleDelta;
};

#endif //_ASSISTANT_VIEW_H