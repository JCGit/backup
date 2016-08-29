#ifndef __MAIN_HERO_H__
#define __MAIN_HERO_H__
#include "QStageEntity.h"
#include "game/control/SimpleDPad.h"
#include "cocos2d.h"
#include "basic/HudLayer.h"
#include "game/weapon/Weapon.h"

using namespace cocos2d::ui;

class CMainHero:public CQStageEntity
{
public:
	enum{
		E_STATE_NULL = 0,
		E_STATE_IDLE,
		E_STATE_SHOOTING,
		E_STATE_WALK_SHOOTING,
		E_STATE_BACK_SHOOTING,
		E_STATE_WALK,
		E_STATE_DIE,
	};

	enum{
		BODY_ZORDER = 0,
		LEFT_HAND_ZORDER = -8,
		RIGHT_HAND_ZORDER = 1,
	};

	CMainHero();

	CREATE_FUNC(CMainHero);
	
	static const string TRY_KICK_ENIMY_EVENT;
	static const string HERO_HP_CHANGE_EVENT;
	static const string HERO_EMERGE_COMPLETE_EVENT;
	static const string HERO_CAST_SKILL_EVENT;

	void onEnter();
	void onExit();
	//状态改变
	void stateChangeByAttack(bool attack);
	void stateChangeByMove(const CCPoint& stDir);
	void stateChangeByReleaseHud(); //释放摇杆
	void stateChangeByKeyTouch(const HudTouchEvent& stTouch);


	//需要转向，敌人在身后，这个只用于移动射击中的转身问题
	void turnBackShooting(const CCPoint& stDir);
	void turnBackShooting();


	void CastSkill(int iSkillIdx);
	void CastSkillWithSkillID(int iSkillID);
	unsigned int GetSkillNextActSec(int iIdx);
	void pickupItem(CCObject *pItem); // 捡到掉落物品的回调
	bool init(){
        bool bRet = false;

        do {
            CC_BREAK_IF(!CQStageEntity::init());

            m_uHeroState = E_STATE_IDLE;
            m_bAttacking = false;
            
            bRet = true;
        } while(0);

        return bRet;
	}

	virtual void EmergeComplete();

	virtual void Idle();
	virtual void ForceIdle();

	virtual void Dizzy();

	virtual void FirstRand();	//开始站立都播放第一套动作
	virtual void RandStand();   //随机播放一套待机动作

	virtual void OtherAttack();
	virtual void OtherAttackCounterMarch();
	virtual void OtherAttackDash();
	virtual void OtherAttackWalk();

	virtual void OtherCounterMarch();
	virtual void OtherDash();
	virtual void OtherDie();
	virtual void OtherHurt();
	virtual void OtherIdle();
	virtual void OtherWalk();
	virtual void OtherDizzy();
	virtual void OtherAttackWithSkill(const CQSkillCell* pstSkill);
	virtual void OtherBackDash();
	virtual void OtherKick();
	virtual void LoadOtherRes();
	virtual void KnockDown();
	void ShowHands(){
		if (m_pstLeftHand){
			m_pstLeftHand->setVisible(true);
		}
		if (m_pstRightHand){
			m_pstRightHand->setVisible(true);
		}
	}

	virtual bool isShooting() const{
		if (m_uHeroState == E_STATE_SHOOTING 
				|| m_uHeroState == E_STATE_WALK_SHOOTING
				|| m_uHeroState == E_STATE_BACK_SHOOTING){
					return true;
		}else if (getArmature() && getArmature()->getAnimation()){
			if (getArmature()->getAnimation()->getCurrentMovementID() == "attack"
				|| getArmature()->getAnimation()->getCurrentMovementID() == "walk_attack"
				|| getArmature()->getAnimation()->getCurrentMovementID() == "back_attack"){
					return true;
			}
		}

		return false;
	}

	virtual void WalkToDstDoneCallback();
	virtual unsigned int GetAtkType() const;			//主角从武器上获得这个
	virtual unsigned int GetAtkMagicType() const;		//主角从武器上获得这个
	virtual unsigned int GetArmorType() const;			//主角从护甲上获得这个
	
	virtual void update(float dt);

	virtual void ShowHurt(int fDamage);
	virtual void HPChange();
	void ReloadWeaponRes();
	void SwitchRes(const char* pstActionName);
	virtual CCPoint GetFirePosition();
	virtual CCPoint GetSkillGunPoint(const string& stSkillAction);
	void PauseMovement();   //只是停止动作，但是scale什么的继续
	void ResumeMovement();	//继续放动作
	bool IsShootable() const;
	int SwitchWeapon(unsigned int uiWeaponID, bool isNeedupdateData = true);     //换枪，此时要由外部来确保这个角色是拥有这种枪的
	virtual bool IsAttackable() const{
		if (getMyState() == kActionStateDizzy			
			|| getMyState() == kActionStateDying
			|| getMyState() == kActionStateBackDash
			|| getMyState() == kActionStateDie
			|| _pstCurSkillCell){
				return false;
		}
		return true;
	}

	//是否眩晕
	virtual bool IsDizzy() const{
		if(getMyState() == kActionStateDizzy){
			return true;
		}
		return false;
	}

	//static int GetResourceNames(unsigned int uiWeaponID, string& stBodyName, string& stLeftHandName, string& stRightHandName);
	static int GetResourceNames(unsigned int uiWeaponID, unsigned int uShowID, string& stBodyName);

	void ChangeWeapon();		//换枪的数据变化,在战斗使用，同屏中主需要改变外观

	//设置显示名称
	void showName(string nick, bool isSelf = false);

	virtual void setScaleX(float fScaleX);
	virtual void setScale(float fScale);
	virtual void setScaleY(float fScale);
	virtual float getScaleX();
	virtual float getScaleY();

	virtual void walkActWithSpeed(unsigned int speed);		//移动时让动作匹配
	virtual void winAct(bool isLoop);			//胜利动作

	//virtual void reLive();						//复活

	void setVIPLv(unsigned int uVIPLv); // 等级大于0会显示vip头衔

private:
	void HandleMovementEvent(CCArmature * armature, MovementEventType type, const char * name);
	void CheckShooting(double dt);
	void ShowGunFire();
	void HideGunFire();
	CCPoint GetGunRelativePos();
	
private:
	unsigned int m_uHeroState;
	bool m_bAttacking;
	//当前的武器
	CC_SYNTHESIZE(CWeapon, m_stCurWeapon, Weapon);					  //当前的武器（数据结构不大的情况下，可以用对象，否则就指针）
	//CC_SYNTHESIZE(string, m_stLeftHandRes, LeftHandRes);
	//CC_SYNTHESIZE(string, m_stRightHandRes, RightHandRes);
// 	CC_SYNTHESIZE(string, m_stBodyRes, BodyRes);

	CC_SYNTHESIZE(unsigned int, m_uShowID, ShowID);
	double m_dLastFireTime;    //上一发子弹发射的时间
// 	CCArmature* m_pstBody_attack;			//身体的骨骼,攻击动作面
// 	CCArmature* m_pstBody_stand;			//身体的骨骼,站立动作面
// 	CCArmature* m_pstBody_skill;			//身体的骨骼,技能攻击动作面
// 
// 	CCArmature* m_pstLeftHand_attack;			//身体的骨骼,左手攻击动作面
// 	CCArmature* m_pstLeftHand_stand;			//身体的骨骼,左手站立动作面
// 	CCArmature* m_pstLeftHand_skill;			//身体的骨骼,左手技能攻击动作面
// 
// 	CCArmature* m_pstRightHand_attack;			//身体的骨骼,右手攻击动作面
// 	CCArmature* m_pstRightHand_stand;			//身体的骨骼,右手站立动作面
// 	CCArmature* m_pstRightHand_skill;			//身体的骨骼,右手技能攻击动作面

	CCArmature* m_pstLeftHand;		//左手骨骼
	CCArmature* m_pstRightHand;		//右手骨骼
	CCArmature* m_pstGunFireAni;    //枪口射击特效
	unsigned int m_uWeaponMovementID;  //枪械的动作索引
	unsigned int m_uWeaponGuesture; //持枪动作
	int m_iGunfireSoundID;			//开枪的音效
	string m_stFirePointStr;		//缓存一下枪口位置的骨骼名称
	CCPoint m_stFirePointPos;       //缓存枪口骨骼的位置，以免每帧计算一次
	int m_iNextState;               //下一个状态，主要是在战斗中改为idle时候用于缓冲
	CC_SYNTHESIZE(double, m_dFireStateChgTime, FireStateChgTime);
	CC_SYNTHESIZE(int, m_iBulletDamageRangeHight, BulletDamageRangeHight);
	CC_SYNTHESIZE(int, m_iBulletDamageRangeWidth, BulletDamageRangeWidth);
	UIImageView *m_pTitleImg; // 头衔

protected:
	virtual void HandleEntityDeath();
};

#endif