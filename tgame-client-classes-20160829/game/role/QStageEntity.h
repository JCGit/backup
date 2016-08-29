#ifndef __QSTAGE_ENTITY_H__
#define __QSTAGE_ENTITY_H__
#include "cocos2d.h"
#include "cocos-ext.h"
#include "defines.h"
#include "base_nodes/CCNode.h"
#include "sprite_nodes/CCSprite.h"
#include <string>
#include "weapon/Weapon.h"
#include "SkillCfg.pb.h"
#include "Macros.pb.h"
#include "fight/Buff.h"
#include "role/BloodBar.h"
#include "fight/QStageEntityObj.h"
#include "WeaponCfg.pb.h"
#include "vlib/Common.h"
#include "vlib/QEventDispatcher.h"
USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;
using namespace com::cfg::vo;

#define SKILL_GUN_EFF_TAG 3000				//�����ͷŵ�ǹ����Ч

struct CHurtDamageObj{
	string stHurtAnimation;
	float fDamage;
	int uBulletParty; 
	int iBulletOwnerLevel; 
	int iBulletRoleType;
	CCPoint stHitBackPos;   //���˾���
	CCPoint stBulletPos;	//�ӵ�λ��
	const SkillCfg* pstSkillCfg;
	CCPoint stSkillPos;
	string stPapaAnimation;
	int iCurAniEffNum;		//���������й���������Ч(���ˡ�������papa)����
	CHurtDamageObj(){
		fDamage = 0;
		stHurtAnimation = "";
		uBulletParty = 0;
		iBulletOwnerLevel = 0;
		iBulletRoleType = 0;
		stHitBackPos = CCPointZero;
		stBulletPos = CCPointZero;
		pstSkillCfg = NULL;
		stSkillPos = CCPointZero;
		iCurAniEffNum = 0;
	}
};

class CQSkillCell:public CCObject{
public:
	CREATE_FUNC(CQSkillCell);
	unsigned int m_uSkillID;	
	float m_uLastCastTime;							//�ϴ�ʹ�ü���ʱ�䣬����ʼֵ��Ϊ0��һ��ʼ����Ҫ��cd
	map<unsigned int, int> m_stIDToDamgeCnt;		//����uID��Ӧ�˺�����
	SkillCfg m_stCfg;
	bool init(){return true;}
	int LoadCfg();
	bool IsFollowType(){
		return (m_stCfg.triggertype() == SKILL_TRIGGER_DIRECT_FOLLOW || m_stCfg.triggertype() == SKILL_TRIGGER_SPAWN_FOLLOW);
	}

	bool HasCastAnimiation(){
		return strcmp(m_stCfg.castanimation().c_str(), "null") != 0;
	}

	bool HasFlyAnimation(){

		return strcmp(m_stCfg.flyanimation().c_str(), "null") != 0;
	}
	
	bool HasHurtAnimation(){
		return strcmp(m_stCfg.hurtanimation().c_str(), "null") != 0;
	}

	bool IsCloseAtk(){
		return m_stCfg.castdist() <= 2;
	}

	bool IsCanDamge(unsigned int uID){   //�жϽ�ɫ�Ƿ�����
		map<unsigned int, int>::iterator stIter = m_stIDToDamgeCnt.find(uID);
		if(stIter == m_stIDToDamgeCnt.end()){
			m_stIDToDamgeCnt[uID] = m_stCfg.damagecnt();
			return true;
		}else{
			if(stIter->second > 0){
				return true;
			}else{
				return false;
			}
		}
	}

	void DecDamgeCnt(unsigned int uID){
		map<unsigned int, int>::iterator stIter = m_stIDToDamgeCnt.find(uID);
		if(stIter == m_stIDToDamgeCnt.end()){
			m_stIDToDamgeCnt[uID] = m_stCfg.damagecnt() - 1;
		}else{
			unsigned int cnt = stIter->second;
			m_stIDToDamgeCnt[uID] = cnt - 1;
		}
	}
public:
	CQSkillCell(){
		
		m_uSkillID = 0;
		m_uLastCastTime = 0;
		m_stIDToDamgeCnt.clear();
	}
};

class CQStageEntity:public CCNode , public CQEventDispatcher
{
public:
	CQStageEntity();
	virtual ~CQStageEntity();
	CREATE_FUNC(CQStageEntity);
	virtual bool init();
	virtual void setScaleX(float fScaleX);
	virtual void setScale(float fScale);
	virtual void setScaleY(float fScale);

	virtual float getScale();
	virtual float getScaleX();
	virtual float getScaleY();

	void WalkTo(const CCPoint& stDstPoint);
	virtual void OtherWalk();

	void WalkWithDirection(const CCPoint& stDir);

	void AttackWalkWithDirection(const CCPoint& stDir);
	virtual void OtherAttackWalk();

	void DashWithDirection(const CCPoint& stDir);
	virtual void OtherDash();

	void AttackDashWithDirection(const CCPoint& stDir);
	virtual void OtherAttackDash();

	void CounterMarchWithDirection(const CCPoint& stDir);
	virtual void OtherCounterMarch();

	void AttackCounterMarchWithDirection(const CCPoint& stDir);
	virtual void OtherAttackCounterMarch();

	void WanderWithDirection(const CCPoint& stDir);


	void WanderTo(const CCPoint& stPoint);

	virtual void SleepEntity();			//����״̬

	virtual void Active();			//�����

	void Jump();

	void JumpAtkWithAction(CCPoint stPos, const char* stAct);			//��Ծ����

	virtual void AttackWithSkill(unsigned int uSkillID);
	virtual void OtherAttackWithSkill(const CQSkillCell* pstSkill);
	virtual void AttackWithAction(const string& strName, const string& strSoundName);

	void AttackWithSkillAtIdx(int iIdx);

	const CQSkillCell* GetSkillByIdx(int iIdx);

	void Attack();
	virtual void OtherAttack();

	virtual void Idle();
	virtual void OtherIdle();

	virtual void ForceIdle();
	virtual void ForceWalk();
	

	virtual void Hurt();

	void Die();
	void DieWithAction(const char* stDieAction);
	virtual void OtherDie();

	void Dizzy();
	virtual void OtherDizzy();

	void BackDash();   //���ɾ��
	virtual void OtherBackDash();

	void ForwardDash(float distance);		//��ǰ��ײ
	void CheckRollBack(const CCRect& stWalkRect);					//��ײ�����������

	void Immobilize(); //�����ƶ������ǿ��Թ���
	virtual void OtherImmobilize();


	void Blind(); //��ä
	virtual void OtherBlind();

	void Kick();
	virtual void OtherKick();

	//���ų�����Ч
	virtual void Emerge();
	virtual void EmergeComplete();

	virtual void HurtWithDamage(float fDamage);
	//���㱻���˾���
	void HurtWithDamage(float fDamage, const CCPoint& stHitBackPos);
	virtual void OtherHurt();

	virtual void HurtWithDamage(CHurtDamageObj stObj);

	virtual void KnockDown();

	void ShowBeatenAnimation(const string& stBeatenAni);
	

	virtual void ShowHurt(int fDamage);
	void ActionReleaseCb(CCObject* pstNode);
	virtual void update(float dt); //update? update what? 
	void updatePosition(float dt);
	void SetEnityPosition(const CCPoint& stPos);

	virtual void setPosition(const CCPoint &position);

	void ShowBloodNum(UINT64_t num, bool isAdd);
	void FloatText(const char *str);
    void WalkByPath(const vector<CCPoint>& stPathLst);

	void ShowMiss();
	
	CCRect getBoundingBox()
	{
		if (!mArmature)
		{
			return CCRect();
		}
		return mArmature->boundingBox();
	}

	virtual void LoadActionRes(bool isEmerge = true);
	virtual void LoadOtherRes();
	virtual void onExit();
	virtual void onEnter();
	CQSkillCell* GetCurSkillCell();

	int GetAReadySkill();


	bool checkIsDied();				//���Ѫ���Ƿ�Ϊ0������Ӧ�Ĳ���
	void HandleBeatenEffActionEvent(CCArmature * armature, MovementEventType type, const char * name);
	void HandleEnlargeEvent(const string& stEvent, CCObject* pstEventObj);
	
	virtual void Pause();
	virtual void Resume();
	virtual void Stop();
	int InitSkills();

	bool IsFriend(unsigned int uMyParty);
	bool IsEnimy(unsigned int uMyParty);
	bool IsAttacking(){
		return getMyState() == kActionStateAttack;
	}
	virtual void SwitchRes(const char* pstActionName); //���ݶ��������л�������Դ

	virtual CCPoint GetFirePosition();

	virtual bool IsWalkable() const{
		if (getMyState() == kActionStateDizzy
			//|| getMyState() == kActionStateHurt
			|| getMyState() == kActionStateDying
			|| getMyState() == kActionStateBackDash
			|| getMyState() == kActionStateDie
			|| getMyState() == kActionStateImmobilize
			|| getMyState() == kActionStateForwardDash
			|| getMyState() == kActionStateEmerge
			|| getMyState() == kActionStateJumpAtk
			|| _pstCurSkillCell
			|| !mArmature){
				return false;
		}
		return true;
	}


	virtual bool IsAttackable() const{
		
		if (getMyState() == kActionStateDizzy
			//|| getMyState() == kActionStateHurt
			|| getMyState() == kActionStateDying
			|| getMyState() == kActionStateBackDash
			|| getMyState() == kActionStateDie
			|| getMyState() == kActionStateBlind
			|| getMyState() == kActionStateWin
			|| getMyState() == kActionStateForwardDash
			|| getMyState() == kActionStateJumpAtk
			|| getMyState() == kActionStateEmerge
			|| _pstCurSkillCell
			|| !mArmature){
				return false;
		}
		return true;
	}

	bool IsDead() const{
		if (_hp <= 0){
			return true;
		}

		return false;
	}

	//���ɹ���
	virtual bool IsImmune() const{
		if (getMyState() == kActionStateBackDash
			|| getMyState() == kActionStateDying
			|| getMyState() == kActionStateWin
			|| m_bIsInvincible){
				return true;
		}

		return false;
	}

	virtual bool IsBlindable() const {
		if (getMyState() == kActionStateBackDash
			|| getMyState() == kActionStateDying
			|| getMyState() == kActionStateWin){
				return false;
		}

		return true;
	}

	virtual bool IsImmobilizible() const{
		if (getMyState() == kActionStateBackDash
			|| getMyState() == kActionStateDying
			|| getMyState() == kActionStateWin){
				return false;
		}

		return true;
	}

	virtual bool IsNeedUpdateSpeed() const{
		if (getMyState() == kActionStateBackDash
			|| getMyState() == kActionStateDying){
				return false;
		}

		return true;
	}

	virtual bool IsWalking() const{
		if (getMyState() == kActionStateWalk
			|| getMyState() == kActionStateWalkToDst
			|| getMyState() == kActionStateAtkCounterMarch){
				return true;
		}
		return false;
	}

	virtual bool IsForwardDashing() const{
		if (getMyState() == kActionStateForwardDash){
				return true;
		}
		return false;
	}

	virtual void resetScale();
public:
	static const string ATK_ACTION_KEY_FRAME;		//��ս��ͨ����
	static const string EXCLUSIVE_ONE_BEGIN_FRAME;	//���ż��ܺ�����ֻ����Ŷ�
	static const string EXCLUSIVE_ONE_END_FRAME;	//�����꼼��ȡ������
	static const string SKILL_CAST_SOUND_FRAME;		//������Ч
	static const string SKILL_CAST_FRAME;		   //������Ч
	static const string FIRE_EVENT;				   //�����ӵ�
	static const string DIE_EFF_FRAME;			   //��ɫ��������������Ч��֡
	static const string ENLARGE_FRAME;			   //��ɫ���
	static const string SHRINK_FRAME;			   //��С��������С
	static const string SKILL_CAST_IN_ACTION;	   //�����ͷŶ��������¼�
	static const string KICK_EVENT;                //�����¼�
	static const string BEGIN_SLIDE_FRAME;         //��ʼ����
	static const string CEASE_SLIDE_FRAME;		   //ֹͣ����
	static const string GUN_SMOKE_FRAME;		   //ǹ���̳�֡
	static const string GUN_BULLET_SHADE_FRAME;	   //ǹ�ڵ�Ļ֡
	static const string GUN_HIT_GROUND_FRAME;		//�ӵ��򵽵����Ч��
	static const string ACTIVE_COMPLETE_EVENT;		//�������м�������¼�
	static const string WIN_COMPLETE_EVENT;			//ʤ������
	static const string DIE_MOMENT_EVENT;			//����˲������ݼ�¼
	static const string DIE_COMPLETE_EVENT;			//������������¼� 
	static const string DASH_COMPLETE_EVENT;		//��ײ��������¼�
	static const string DIE_EFF_SOUND_EVENT;		//boss������������Ч����Ч�¼�
	static const string JUMP_ATK_END_KET_FRAME;		//��Ծ��������֡
	static const string GUN_POINT_EFF_KEY_FRAME;    //ǹ����Ч֡
	static const string ADD_ANI_EFF_EVENT;			//��ɫ������Ч�����¼�
	static const string HERO_DASH_COMPLETE_EVENT;	//��ɫ��������¼�

	void HandleFrameEvent(CCBone *bone, const char * stEvent, int originFrameIndex, int currentFrameIdx);
	
	virtual void WalkToDstDoneCallback(); // ��ɫ�ƶ���Ŀ�ĵغ�Ļص�������ʵ��
	void checkWalk(); // ͬ����
	virtual bool isShooting() const;
	virtual bool isBackDashable() const;
	virtual void HandleEntityDeath();

	virtual bool IsActionInterruptable();
protected:
    CC_SYNTHESIZE(int, _state, MyState); //neilwu mod
public:
	virtual void MovementCallback(CCArmature * armature, MovementEventType type, const char * name);
	void MovementCallbackToRelease(CCArmature * armature, MovementEventType type, const char * name);

	virtual void HandleMovementEvent(CCArmature * armature, MovementEventType type, const char * name);
	virtual CCPoint GetSkillGunPoint(const string& stSkillAction);
private:
	void ShowNumberCallBack(CCObject* pstObj);  //Ʈ�ֻص�
	void FloatTextCallBack(); 

	void resetColor();	
	int randDamage(int iRealNum);				//չʾƮѪ�˺�ʱ���˺�������-5,5��

	void HandleSingleEvent(const string& stSingleEvent);
	void CastSkillGunAnimation(const CQSkillCell* pstSkill);

	bool checkData(UINT64_t uPreData, UINT64_t uMd5Data);
	void updateData(UINT64_t uPreData, UINT64_t& uMd5Data);
	void resumeData(UINT64_t& uPreData, UINT64_t uMd5Data);
private:
	CC_SYNTHESIZE( cocos2d::extension::CCArmature*, mArmature, Armature);
	CC_SYNTHESIZE(cocos2d::CCPoint, _velocity, Velocity);
	CC_SYNTHESIZE(cocos2d::CCPoint, _desiredPosition, DesiredPosition); //neil add comment,���ý�ɫλ��

	CC_PROPERTY(CQStageEntityObj*, _obj, Obj);
	
	//measurements
	CC_SYNTHESIZE(float, _centerToSides, CenterToSides);
	CC_SYNTHESIZE(float, _centerToBottom, CenterToBottom); 
	CC_SYNTHESIZE(float, _attakRange, AttakRange);
	CC_SYNTHESIZE(float, _damageRange, DamageRange);
	//���и�����ֵ������صģ���ֻ����ս�����õ��ġ�
	CC_SYNTHESIZE(std::string, _resPath, ResPath);
	CC_SYNTHESIZE(unsigned int, _curSkill, CurSkill); //��ǰ����ʹ�õ�skill ID���漰�����ʹ����Ч
	CC_SYNTHESIZE(cocos2d::extension::CCArmature*, _skillArmature, SkillArmature); //���ܵ���Ч,����ֻ�㷢�𣿷��й��̺ͷ��Ķ���
	CC_SYNTHESIZE(std::string, _dieResPath, DieResPath);	//������Ч��Դ
	CC_SYNTHESIZE(float, _patrolArea, PatrolArea);  //����Ѳ�߷�Χ
	
	CC_SYNTHESIZE(CCArray*, _skillArray, SkillArray);
//	CC_SYNTHESIZE(CCPoint, _dstPoint, DstPoint);
	CC_SYNTHESIZE(CCPoint, m_stDstDirection, DstDirection); // ��ɫֹͣʱ�ķ���������ʾ��
    vector<CCPoint> m_stPathPtLst; //·������Ϣ�б�[����洢��]

	//��ɫ��������
	CC_SYNTHESIZE(unsigned int, _atkVal, AtkVal);           //���������� û���κμӳ�
	CC_SYNTHESIZE(unsigned int, _defVal, DefVal);			//���������� û���κμӳ�
	unsigned int _attackType;                               //��������
	unsigned int _armorType;								//��������
	unsigned int _attackSpeed;								//����
	unsigned int _attackMagicType;							//��������������
	CC_SYNTHESIZE(unsigned int, _hit, Hit);					//����ֵ
	CC_SYNTHESIZE(bool, _isBlock, IsBlock);					//�Ƿ��赲
	CC_SYNTHESIZE(CCSize, _blockSize, BlockSize);			//�赲��С
	

	CC_SYNTHESIZE(float, _walkSpeed, WalkSpeed);
	CC_SYNTHESIZE(float, _dashSpeed, DashSpeed);
	CC_SYNTHESIZE(float, _backSpeed, BackSpeed);

	unsigned int m_uRandData;
	UINT64_t m_uHpMd5;				//У�鵱ǰѪ��

	CC_PROPERTY(UINT64_t, _hp, HP);					//��ǰѪ��
	CC_SYNTHESIZE(UINT64_t, _maxHp, MaxHP);		//���Ѫ��
	CC_SYNTHESIZE(unsigned int, _ep, EP);			//��ǰ����
	CC_SYNTHESIZE(unsigned int, _maxEp, MaxEP);		//�������
	CC_SYNTHESIZE(unsigned int, _roleAtkSpeed, RoleAtkSpeed);	//���ﱾ�����ٶ�
	CC_SYNTHESIZE(unsigned int, _toatlAtkSpeed, ToatlAtkSpeed);	//�ܵĹ����ٶ�
	CC_SYNTHESIZE(std::string, _nameStr, NameStr);	//����	
	CC_SYNTHESIZE(string, _stEmergeVoice, EmergeVoice); //�����ڳ����е���Ч
	CC_SYNTHESIZE(string, _stHurtVoice, HurtVoice); //�ܻ���Ч
	CC_SYNTHESIZE(string, _stDieVoice, DieVoice);  //������Ч

	CC_SYNTHESIZE(int, _hurtColor, HurtColor);		//����ʱ�仯����ɫ
	CC_SYNTHESIZE(string, _stEmergeAct, EmergeAct);	//��������
	CC_SYNTHESIZE(bool, _isEmerge, IsEmerge);		//�Ƿ������ 


	//��Ч�ļ��ܣ��������ܡ��Լ��Ĵ��С�ǹе�ľ��е�		
	CQSkillCell m_stSkills[ROLE_SKILL_CNT];
	
	//��ǰ��Ч�ĸ��ֿ���
	int m_stMagicAtkRatio[MAX_MAGIC_CNT];	//�������Լӳ�,����˳��Ӧ���ǣ�(����+װ��),Ȼ����ϵͳbuff��Ȼ����ս��buff��
	int m_stMagicDefRatio[MAX_MAGIC_CNT];    //�������Կ���


	int m_uDefAdjustRatio;					//��������ϵ��
	int m_uAtkAdjustRatio;					//����������ϵ��
	
	CC_SYNTHESIZE(uint64, m_uEffAtk, EffAtk);                         //Ŀǰ����Ч�������Ѿ������Լ��Ļ�����ֵ������ǹе���Լ���������ֵ�ӳɵ� 
	CC_SYNTHESIZE(uint64, m_uEffDef, EffDef);							//Ŀǰ����Ч�������Ѿ������Լ��Ļ�����ֵ��ǹе�ľ���ֵ�ӳ���

	
	CC_SYNTHESIZE(unsigned int, m_uBulletDepthRatio, BulletDepth);			//������ȵ���ϵ����Ĭ��Ϊ0
	CC_SYNTHESIZE(unsigned int, m_uBulletLightRatio, BulletLight);			//�������ȵ���ϵ��,Ĭ��Ϊ0

	//��ɫ���ϵ�buff--����ֻά��������Щbuff��ʲôʱ��ñ������ͬʱ��һЩ��Ҫ���¼������
	CBuff* m_stBuffs[ROLE_MAX_BUFF_CNT];

	//�ý�ɫ�����������ͣ���ҽ�ɫ�������boss���Ѻõ�npc��
	CC_SYNTHESIZE(unsigned int, _roleType, RoleType);
	CC_SYNTHESIZE(unsigned int, _roleParty, RoleParty);

	CC_SYNTHESIZE(int, _vision, Vision); //��Ұ��ȣ� -1 Ϊ����
	unsigned int _preState;
	CCPoint m_stPreVocity;
	CQSkillCell* _pstCurSkillCell;
	CC_SYNTHESIZE(bool, _isTheOne, IsTheOne);
	CC_SYNTHESIZE(bool, _hurtSuspendAction, HurtSuspendAction);

	CC_SYNTHESIZE(unsigned int, _weaponCfgID, WeaponCfgID);

	// osd start

	CC_SYNTHESIZE(bool, m_bIsFollowActivate, IsFollowActivate);		// �Ƿ񼤻�׷�٣�����ͬ����ɫ������󣬽�ɫ��λ���ͺ�������ʵλ�ã��������޸ĵ�ǰλ���Խӽ���ʵλ�ã�ֱ����ͬ
	CC_SYNTHESIZE(CCPoint, m_stFollowDstPos, FollowDstPos);			// �������ĵ����꣬��ʵ��λ��
	CC_SYNTHESIZE(CCPoint, m_stFollowDirection, FollowDirection);	// �������ķ���������ʾ������ʵ�ķ���
	CC_SYNTHESIZE(CCRect, m_stWalkableRect, WalkableRect);					// �����߾�������

	// osd end

	CC_SYNTHESIZE(CBloodBar*, _blood, Blood);	//Ѫ��;
	bool m_bIsControlledByAI;
	CC_SYNTHESIZE(bool, m_bIsNeedPath, IsNeedPath);	// �ǵ����ߵĻ������Ϸɵ�
	CC_SYNTHESIZE(float, m_fAITime, FAiTime);
	CC_SYNTHESIZE(int, m_iNextHurtActionTime, NextHurtActionTime);   //��һ�β����ܻ�������ʱ�仹����
	CC_SYNTHESIZE(int, m_iEffectHurtActionCD, HurtActionCD);   //��һ�β����ܻ�������ʱ�䣬����������
	CC_SYNTHESIZE(int, m_iLevel, Level); //�ý�ɫ�ĵȼ�
	//int m_iEffectHurtActionCD; //ʵ����Ч�Ľ�ֱʱ��
	CCPoint m_stHitBackDest; //��������Ŀ�ĵ�

	WeaponCfg* m_pstWeaponCfg;   //����������
	unsigned int m_uiBullteTrailType;   //��������
	CC_SYNTHESIZE(unsigned int, m_uiBulletBurstCnt, BulletBurstCnt);    //�ӵ�burst����
	CC_SYNTHESIZE(unsigned int, m_uiBulletCurCnt, BulletCurCnt);      //��ǰ�ӵ��������

	CCPoint m_stPrePos; //��һ����ַ������λ��У���

	CC_SYNTHESIZE(int, m_iBackDashDist, BackDashDist);		//���ܵľ��룬����ʱ���Լ�����
	CC_SYNTHESIZE(int, m_iBackDashCD, BackDashCD);			//���ܵ�cd
	float m_fNextDashableTime;								//�������ܵ���һ��ʱ��

	CCPoint m_stDashDstPos; //Ŀ���ַ
	CCPoint m_stJumpAtkDstPos;		//��Ծ������Ŀ���ַ

	CCPoint m_stForDashDstPos;								//��ײ��Ŀ���ַ
	CC_SYNTHESIZE(bool, m_bIsCheckOutSize, IsCheckOutSize);	//�Ƿ������

	float m_fTransformScale;		//����С��Ŀ��scale
	float m_fOrigScale;             //��ʼ�仯ǰ��scale
	CC_SYNTHESIZE(bool, m_bIsPlayingMovement, IsPlayingMovement);
	CC_SYNTHESIZE(int, m_AIType, AIType);  //�ý�ɫ��AI���㷽��
	CC_SYNTHESIZE(bool, m_bIsInvincible, IsInvincible);  //�Ƿ��޵е�
	CC_SYNTHESIZE(int, m_iKnockCount, KnockCount);  //�Ƿ��޵е�

	CC_SYNTHESIZE(bool, _isActivate, IsActivate);		//�Ƿ񼤻�
	int m_iBulletDamageAdj;               //�ӵ��˺��ӳ�

	double m_fPreTime;					//��һ��Ʈѩʱ��

	float m_fHurtTime;					//���˱���ʱ��
	bool m_bIsHurtColor;				//�Ƿ��Ѿ����
	int m_iCurAniEffNum;				//���������й���������Ч(���ˡ�������papa)����

	CC_SYNTHESIZE(unsigned int, m_uInitScale, InitScale);	//��ʼx��,0����,1����

	CC_SYNTHESIZE(bool, m_bIsPlayDieVoice, IsPlayDieVoice);	//�Ƿ񲥷�������Ч

	CC_SYNTHESIZE(bool, m_bIsBulletLimit, IsBulletLimit);	//�Ƿ����ӵ�����
	CC_SYNTHESIZE(float, m_fBulletTime, BulletTime);		//�ӵ�ʣ��ʱ��
	CC_SYNTHESIZE(float, m_fBaseScale, BaseScale);
protected:
	double m_dObjectLife;	//��¼��ǰ�Ƕ��������ʾ��̨��ʱ��
	double m_dLastCheckTime;
	double m_dStartWalkTime;

	CMovements m_stMovements;			//��ɫ�Ķ�����
public:
	unsigned int GetMagicDefRatio(unsigned int uMagicType) const;
	unsigned int GetMagicAtkRatio(unsigned int uMagicType) const;
	void SetMagicDefRatio(const uint32 def[]);
	void SetMagicAtkRatio(const uint32 atk[]);
	virtual unsigned int GetAtkType() const;			//���Ǻ͹����������ֵ�ķ�ʽ��ͬ
	virtual unsigned int GetAtkMagicType() const;		//���Ǻ͹����������ֵ�ķ�ʽ��ͬ
	virtual unsigned int GetArmorType() const;			//���Ǻ͹����������ֵ�ķ�ʽ��ͬ
	void AddMagicAtkRatio(unsigned int uMagic, unsigned int uVal);
	void DecMagicAtkRatio(unsigned int uMagic, unsigned int uVal);
	void AddMagicDefRatio(unsigned int uMagic, unsigned int uVal);
	void DecMagicDefRatio(unsigned int uMagic, unsigned int uVal);
	void AddAtkRatio(unsigned int uVal);
	void DecAtkRatio(unsigned int uVal);
	void AddDefRatio(unsigned int uVal);
	void DecDefRatio(unsigned int uVal);
	void AddHpMaxByRatio(unsigned int uVal);
	void DecHpMaxByRatio(unsigned int uVal);
	void AddDashSpeedByRatio(unsigned int uVal);
	void DecDashSpeedByRatio(unsigned int uVal);
	void AddWalkSpeedByRatio(unsigned int uVal);
	void DecWalkSpeedByRatio(unsigned int uVal);
	void AddBulletDepth(unsigned int uVal);
	void DecBulletDepth(unsigned int uVal);
	void AddBulletLight(unsigned int uVal);
	void DecBulletLight(unsigned int uVal);	
	void SetBulletDamageAdj(unsigned int uVal);
	void UnSetBulleteDamageAdj();


	void ReCalAtkDef();//ÿ�θ���װ����Ϣ����װ������װ��ʲô��ʱ�򣬾ͱ���Ҫ��ȫ���¼���һ�ν�ɫ������
	void RefreshBuffs();
	void AddBuff(unsigned int uiBuffID, unsigned int uOwnerUID, bool bIsAddArm = true);
	void RemoveBuff(unsigned int uiBuffID, unsigned int uOwnerUID);
	bool CheckHasBuff(unsigned int uiBuffID, unsigned int uOwnerUID);
	void AddHP(UINT64_t uiHP, bool bIsAdd);
	bool IsDead(){
		return _hp <= 0 ;
	}
	double GetShootInterval() const;
	virtual unsigned int GetBulletID();		//�ӵ�ID
	virtual unsigned int GetShootSpeed();		//�ӵ�����
	virtual unsigned int GetBulletShootDistance(); //�ӵ����
	virtual CCPoint GetBulletPosition();  //�ӵ���ʼλ��
	virtual double GetZAlias();   //����ӵ���Z��
	virtual bool IsInThrash();    //��ǰ��ɫ�Ƿ��ڱ���״̬
	virtual int GetBulletPenatrate(); //�ӵ���͸����
	virtual int GetBulletAttenuation(); //�ӵ���͸˥��ϵ��

    //��ȡEntity���µ�����
    CCPoint GetFootPos() { 
        //return ccp(this->getPositionX(), this->getPositionY() - this->getCenterToBottom());
		return this->getPosition(); // �ȱ����˽ӿ�
    }

	CCPoint GetCenterPos(){
		return ccp(this->getPositionX(), this->getPositionY() + this->getCenterToBottom());
	}

	CCPoint GetTopPos(){
		return ccp(this->getPositionX(), this->getPositionY() + this->getCenterToBottom() * 2);
	}

	virtual void HPChange();		//Ѫ���仯 
	void RollBackPosition(bool bIsIdle = true);		//�ߵ������е�ַ���ˣ���ȥ
	void SetIsControlledByAI(bool bval)	// �������ķ���������ʾ��
	{
		m_bIsControlledByAI = bval;
	}

	bool GetIsControlledByAI() const{
		return (m_bIsControlledByAI == true && getMyState() != kActionStateDizzy);
	}

	void setShadowVisiable(bool var);

	int GetBulletDamageAdj() const{
		return m_iBulletDamageAdj;
	}

	virtual float GetBloodPosY();

	float GetEntityTime(){
		return (float)m_dObjectLife;
	}
};

#endif