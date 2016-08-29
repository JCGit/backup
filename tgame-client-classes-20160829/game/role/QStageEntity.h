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

#define SKILL_GUN_EFF_TAG 3000				//技能释放的枪口特效

struct CHurtDamageObj{
	string stHurtAnimation;
	float fDamage;
	int uBulletParty; 
	int iBulletOwnerLevel; 
	int iBulletRoleType;
	CCPoint stHitBackPos;   //击退距离
	CCPoint stBulletPos;	//子弹位置
	const SkillCfg* pstSkillCfg;
	CCPoint stSkillPos;
	string stPapaAnimation;
	int iCurAniEffNum;		//场景中所有怪物身上特效(受伤、死亡、papa)总数
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
	float m_uLastCastTime;							//上次使用技能时间，当初始值不为0，一开始就需要走cd
	map<unsigned int, int> m_stIDToDamgeCnt;		//怪物uID对应伤害次数
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

	bool IsCanDamge(unsigned int uID){   //判断角色是否受伤
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

	virtual void SleepEntity();			//休眠状态

	virtual void Active();			//激活动作

	void Jump();

	void JumpAtkWithAction(CCPoint stPos, const char* stAct);			//跳跃攻击

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

	void BackDash();   //向后删冲
	virtual void OtherBackDash();

	void ForwardDash(float distance);		//向前冲撞
	void CheckRollBack(const CCRect& stWalkRect);					//冲撞检测出屏后回退

	void Immobilize(); //不能移动，但是可以攻击
	virtual void OtherImmobilize();


	void Blind(); //致盲
	virtual void OtherBlind();

	void Kick();
	virtual void OtherKick();

	//播放出生特效
	virtual void Emerge();
	virtual void EmergeComplete();

	virtual void HurtWithDamage(float fDamage);
	//计算被击退距离
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


	bool checkIsDied();				//检测血量是否为0，做相应的操作
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
	virtual void SwitchRes(const char* pstActionName); //根据动作名称切换动作资源

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

	//不可攻击
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
	static const string ATK_ACTION_KEY_FRAME;		//近战普通攻击
	static const string EXCLUSIVE_ONE_BEGIN_FRAME;	//播放技能黑屏，只有猪脚动
	static const string EXCLUSIVE_ONE_END_FRAME;	//播放完技能取消黑屏
	static const string SKILL_CAST_SOUND_FRAME;		//技能音效
	static const string SKILL_CAST_FRAME;		   //技能特效
	static const string FIRE_EVENT;				   //发射子弹
	static const string DIE_EFF_FRAME;			   //角色死亡播放死亡特效的帧
	static const string ENLARGE_FRAME;			   //角色变大
	static const string SHRINK_FRAME;			   //缩小到正常大小
	static const string SKILL_CAST_IN_ACTION;	   //技能释放动作播放事件
	static const string KICK_EVENT;                //踢人事件
	static const string BEGIN_SLIDE_FRAME;         //开始后移
	static const string CEASE_SLIDE_FRAME;		   //停止后移
	static const string GUN_SMOKE_FRAME;		   //枪口烟尘帧
	static const string GUN_BULLET_SHADE_FRAME;	   //枪口弹幕帧
	static const string GUN_HIT_GROUND_FRAME;		//子弹打到地面的效果
	static const string ACTIVE_COMPLETE_EVENT;		//从休眠中激活完成事件
	static const string WIN_COMPLETE_EVENT;			//胜利动作
	static const string DIE_MOMENT_EVENT;			//死亡瞬间的数据记录
	static const string DIE_COMPLETE_EVENT;			//死亡动作完成事件 
	static const string DASH_COMPLETE_EVENT;		//冲撞技能完成事件
	static const string DIE_EFF_SOUND_EVENT;		//boss死亡动作中特效的音效事件
	static const string JUMP_ATK_END_KET_FRAME;		//跳跃攻击结束帧
	static const string GUN_POINT_EFF_KEY_FRAME;    //枪口特效帧
	static const string ADD_ANI_EFF_EVENT;			//角色身上特效增加事件
	static const string HERO_DASH_COMPLETE_EVENT;	//角色闪避完成事件

	void HandleFrameEvent(CCBone *bone, const char * stEvent, int originFrameIndex, int currentFrameIdx);
	
	virtual void WalkToDstDoneCallback(); // 角色移动到目的地后的回调，子类实现
	void checkWalk(); // 同屏用
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
	void ShowNumberCallBack(CCObject* pstObj);  //飘字回调
	void FloatTextCallBack(); 

	void resetColor();	
	int randDamage(int iRealNum);				//展示飘血伤害时，伤害浮动【-5,5】

	void HandleSingleEvent(const string& stSingleEvent);
	void CastSkillGunAnimation(const CQSkillCell* pstSkill);

	bool checkData(UINT64_t uPreData, UINT64_t uMd5Data);
	void updateData(UINT64_t uPreData, UINT64_t& uMd5Data);
	void resumeData(UINT64_t& uPreData, UINT64_t uMd5Data);
private:
	CC_SYNTHESIZE( cocos2d::extension::CCArmature*, mArmature, Armature);
	CC_SYNTHESIZE(cocos2d::CCPoint, _velocity, Velocity);
	CC_SYNTHESIZE(cocos2d::CCPoint, _desiredPosition, DesiredPosition); //neil add comment,设置角色位置

	CC_PROPERTY(CQStageEntityObj*, _obj, Obj);
	
	//measurements
	CC_SYNTHESIZE(float, _centerToSides, CenterToSides);
	CC_SYNTHESIZE(float, _centerToBottom, CenterToBottom); 
	CC_SYNTHESIZE(float, _attakRange, AttakRange);
	CC_SYNTHESIZE(float, _damageRange, DamageRange);
	//还有各种数值属性相关的，都只是在战斗中用到的。
	CC_SYNTHESIZE(std::string, _resPath, ResPath);
	CC_SYNTHESIZE(unsigned int, _curSkill, CurSkill); //当前正在使用的skill ID，涉及到如何使用特效
	CC_SYNTHESIZE(cocos2d::extension::CCArmature*, _skillArmature, SkillArmature); //技能的特效,这里只算发起？飞行过程和放哪儿？
	CC_SYNTHESIZE(std::string, _dieResPath, DieResPath);	//死亡特效资源
	CC_SYNTHESIZE(float, _patrolArea, PatrolArea);  //怪物巡逻范围
	
	CC_SYNTHESIZE(CCArray*, _skillArray, SkillArray);
//	CC_SYNTHESIZE(CCPoint, _dstPoint, DstPoint);
	CC_SYNTHESIZE(CCPoint, m_stDstDirection, DstDirection); // 角色停止时的方向（向量表示）
    vector<CCPoint> m_stPathPtLst; //路径点信息列表[反向存储的]

	//角色基本属性
	CC_SYNTHESIZE(unsigned int, _atkVal, AtkVal);           //基本攻击， 没有任何加成
	CC_SYNTHESIZE(unsigned int, _defVal, DefVal);			//基本防御， 没有任何加成
	unsigned int _attackType;                               //攻击类型
	unsigned int _armorType;								//护甲类型
	unsigned int _attackSpeed;								//攻速
	unsigned int _attackMagicType;							//攻击的属性类型
	CC_SYNTHESIZE(unsigned int, _hit, Hit);					//命中值
	CC_SYNTHESIZE(bool, _isBlock, IsBlock);					//是否阻挡
	CC_SYNTHESIZE(CCSize, _blockSize, BlockSize);			//阻挡大小
	

	CC_SYNTHESIZE(float, _walkSpeed, WalkSpeed);
	CC_SYNTHESIZE(float, _dashSpeed, DashSpeed);
	CC_SYNTHESIZE(float, _backSpeed, BackSpeed);

	unsigned int m_uRandData;
	UINT64_t m_uHpMd5;				//校验当前血量

	CC_PROPERTY(UINT64_t, _hp, HP);					//当前血量
	CC_SYNTHESIZE(UINT64_t, _maxHp, MaxHP);		//最大血量
	CC_SYNTHESIZE(unsigned int, _ep, EP);			//当前能量
	CC_SYNTHESIZE(unsigned int, _maxEp, MaxEP);		//最大能量
	CC_SYNTHESIZE(unsigned int, _roleAtkSpeed, RoleAtkSpeed);	//人物本身攻击速度
	CC_SYNTHESIZE(unsigned int, _toatlAtkSpeed, ToatlAtkSpeed);	//总的攻击速度
	CC_SYNTHESIZE(std::string, _nameStr, NameStr);	//名称	
	CC_SYNTHESIZE(string, _stEmergeVoice, EmergeVoice); //出现在场景中的音效
	CC_SYNTHESIZE(string, _stHurtVoice, HurtVoice); //受击音效
	CC_SYNTHESIZE(string, _stDieVoice, DieVoice);  //死亡音效

	CC_SYNTHESIZE(int, _hurtColor, HurtColor);		//受伤时变化的颜色
	CC_SYNTHESIZE(string, _stEmergeAct, EmergeAct);	//出生动作
	CC_SYNTHESIZE(bool, _isEmerge, IsEmerge);		//是否出生了 


	//有效的技能，包括闪避、自己的大招、枪械的绝招等		
	CQSkillCell m_stSkills[ROLE_SKILL_CNT];
	
	//当前有效的各种抗性
	int m_stMagicAtkRatio[MAX_MAGIC_CNT];	//所有属性加成,结算顺序应该是：(自身+装备),然后是系统buff，然后是战斗buff等
	int m_stMagicDefRatio[MAX_MAGIC_CNT];    //所有属性抗性


	int m_uDefAdjustRatio;					//防御调整系数
	int m_uAtkAdjustRatio;					//攻击力调整系数
	
	CC_SYNTHESIZE(uint64, m_uEffAtk, EffAtk);                         //目前的有效攻击，已经算了自己的基础数值、加上枪械的以及其他绝对值加成的 
	CC_SYNTHESIZE(uint64, m_uEffDef, EffDef);							//目前的有效防御，已经算了自己的基础数值和枪械的绝对值加成项

	
	CC_SYNTHESIZE(unsigned int, m_uBulletDepthRatio, BulletDepth);			//弹道宽度调整系数，默认为0
	CC_SYNTHESIZE(unsigned int, m_uBulletLightRatio, BulletLight);			//弹道亮度调整系数,默认为0

	//角色身上的buff--这里只维护中了哪些buff，什么时候该被清除，同时做一些需要按事件结算的
	CBuff* m_stBuffs[ROLE_MAX_BUFF_CNT];

	//该角色属于哪种类型，玩家角色、怪物、大boss、友好的npc？
	CC_SYNTHESIZE(unsigned int, _roleType, RoleType);
	CC_SYNTHESIZE(unsigned int, _roleParty, RoleParty);

	CC_SYNTHESIZE(int, _vision, Vision); //视野宽度， -1 为无限
	unsigned int _preState;
	CCPoint m_stPreVocity;
	CQSkillCell* _pstCurSkillCell;
	CC_SYNTHESIZE(bool, _isTheOne, IsTheOne);
	CC_SYNTHESIZE(bool, _hurtSuspendAction, HurtSuspendAction);

	CC_SYNTHESIZE(unsigned int, _weaponCfgID, WeaponCfgID);

	// osd start

	CC_SYNTHESIZE(bool, m_bIsFollowActivate, IsFollowActivate);		// 是否激活追踪，用于同屏角色，激活后，角色的位置滞后于其真实位置，并不断修改当前位置以接近真实位置，直到相同
	CC_SYNTHESIZE(CCPoint, m_stFollowDstPos, FollowDstPos);			// 虚拟对象的点坐标，真实的位置
	CC_SYNTHESIZE(CCPoint, m_stFollowDirection, FollowDirection);	// 虚拟对象的方向（向量表示），真实的方向
	CC_SYNTHESIZE(CCRect, m_stWalkableRect, WalkableRect);					// 可行走矩形区域

	// osd end

	CC_SYNTHESIZE(CBloodBar*, _blood, Blood);	//血条;
	bool m_bIsControlledByAI;
	CC_SYNTHESIZE(bool, m_bIsNeedPath, IsNeedPath);	// 是地上走的还是天上飞的
	CC_SYNTHESIZE(float, m_fAITime, FAiTime);
	CC_SYNTHESIZE(int, m_iNextHurtActionTime, NextHurtActionTime);   //下一次播放受击动作的时间还差多久
	CC_SYNTHESIZE(int, m_iEffectHurtActionCD, HurtActionCD);   //下一次播放受击动作的时间，来至配置里
	CC_SYNTHESIZE(int, m_iLevel, Level); //该角色的等级
	//int m_iEffectHurtActionCD; //实际有效的僵直时间
	CCPoint m_stHitBackDest; //被击退是目的地

	WeaponCfg* m_pstWeaponCfg;   //武器的配置
	unsigned int m_uiBullteTrailType;   //弹道类型
	CC_SYNTHESIZE(unsigned int, m_uiBulletBurstCnt, BulletBurstCnt);    //子弹burst个数
	CC_SYNTHESIZE(unsigned int, m_uiBulletCurCnt, BulletCurCnt);      //当前子弹发射个数

	CCPoint m_stPrePos; //上一个地址，用于位置校验的

	CC_SYNTHESIZE(int, m_iBackDashDist, BackDashDist);		//闪避的距离，闪避时间自己计算
	CC_SYNTHESIZE(int, m_iBackDashCD, BackDashCD);			//闪避的cd
	float m_fNextDashableTime;								//可以闪避的下一次时间

	CCPoint m_stDashDstPos; //目标地址
	CCPoint m_stJumpAtkDstPos;		//跳跃攻击的目标地址

	CCPoint m_stForDashDstPos;								//冲撞的目标地址
	CC_SYNTHESIZE(bool, m_bIsCheckOutSize, IsCheckOutSize);	//是否检测出屏

	float m_fTransformScale;		//变大变小的目标scale
	float m_fOrigScale;             //开始变化前的scale
	CC_SYNTHESIZE(bool, m_bIsPlayingMovement, IsPlayingMovement);
	CC_SYNTHESIZE(int, m_AIType, AIType);  //该角色的AI计算方案
	CC_SYNTHESIZE(bool, m_bIsInvincible, IsInvincible);  //是否无敌的
	CC_SYNTHESIZE(int, m_iKnockCount, KnockCount);  //是否无敌的

	CC_SYNTHESIZE(bool, _isActivate, IsActivate);		//是否激活
	int m_iBulletDamageAdj;               //子弹伤害加成

	double m_fPreTime;					//上一次飘雪时间

	float m_fHurtTime;					//受伤变红的时间
	bool m_bIsHurtColor;				//是否已经变红
	int m_iCurAniEffNum;				//场景中所有怪物身上特效(受伤、死亡、papa)总数

	CC_SYNTHESIZE(unsigned int, m_uInitScale, InitScale);	//初始x轴,0向右,1向左

	CC_SYNTHESIZE(bool, m_bIsPlayDieVoice, IsPlayDieVoice);	//是否播放死亡音效

	CC_SYNTHESIZE(bool, m_bIsBulletLimit, IsBulletLimit);	//是否有子弹限制
	CC_SYNTHESIZE(float, m_fBulletTime, BulletTime);		//子弹剩余时间
	CC_SYNTHESIZE(float, m_fBaseScale, BaseScale);
protected:
	double m_dObjectLife;	//记录当前是对象进入显示舞台的时间
	double m_dLastCheckTime;
	double m_dStartWalkTime;

	CMovements m_stMovements;			//角色的动作名
public:
	unsigned int GetMagicDefRatio(unsigned int uMagicType) const;
	unsigned int GetMagicAtkRatio(unsigned int uMagicType) const;
	void SetMagicDefRatio(const uint32 def[]);
	void SetMagicAtkRatio(const uint32 atk[]);
	virtual unsigned int GetAtkType() const;			//主角和怪物获得这个数值的方式不同
	virtual unsigned int GetAtkMagicType() const;		//主角和怪物获得这个数值的方式不同
	virtual unsigned int GetArmorType() const;			//主角和怪物获得这个数值的方式不同
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


	void ReCalAtkDef();//每次更新装备信息，脱装备、加装备什么的时候，就必须要完全重新计算一次角色的属性
	void RefreshBuffs();
	void AddBuff(unsigned int uiBuffID, unsigned int uOwnerUID, bool bIsAddArm = true);
	void RemoveBuff(unsigned int uiBuffID, unsigned int uOwnerUID);
	bool CheckHasBuff(unsigned int uiBuffID, unsigned int uOwnerUID);
	void AddHP(UINT64_t uiHP, bool bIsAdd);
	bool IsDead(){
		return _hp <= 0 ;
	}
	double GetShootInterval() const;
	virtual unsigned int GetBulletID();		//子弹ID
	virtual unsigned int GetShootSpeed();		//子弹射速
	virtual unsigned int GetBulletShootDistance(); //子弹射程
	virtual CCPoint GetBulletPosition();  //子弹起始位置
	virtual double GetZAlias();   //获得子弹的Z轴
	virtual bool IsInThrash();    //当前角色是否在暴走状态
	virtual int GetBulletPenatrate(); //子弹穿透个数
	virtual int GetBulletAttenuation(); //子弹穿透衰减系数

    //获取Entity脚下的坐标
    CCPoint GetFootPos() { 
        //return ccp(this->getPositionX(), this->getPositionY() - this->getCenterToBottom());
		return this->getPosition(); // 先保留此接口
    }

	CCPoint GetCenterPos(){
		return ccp(this->getPositionX(), this->getPositionY() + this->getCenterToBottom());
	}

	CCPoint GetTopPos(){
		return ccp(this->getPositionX(), this->getPositionY() + this->getCenterToBottom() * 2);
	}

	virtual void HPChange();		//血量变化 
	void RollBackPosition(bool bIsIdle = true);		//走到不可行地址上了，回去
	void SetIsControlledByAI(bool bval)	// 虚拟对象的方向（向量表示）
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