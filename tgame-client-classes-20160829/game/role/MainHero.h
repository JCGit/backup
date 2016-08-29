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
	//״̬�ı�
	void stateChangeByAttack(bool attack);
	void stateChangeByMove(const CCPoint& stDir);
	void stateChangeByReleaseHud(); //�ͷ�ҡ��
	void stateChangeByKeyTouch(const HudTouchEvent& stTouch);


	//��Ҫת�򣬵�����������ֻ�����ƶ�����е�ת������
	void turnBackShooting(const CCPoint& stDir);
	void turnBackShooting();


	void CastSkill(int iSkillIdx);
	void CastSkillWithSkillID(int iSkillID);
	unsigned int GetSkillNextActSec(int iIdx);
	void pickupItem(CCObject *pItem); // �񵽵�����Ʒ�Ļص�
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

	virtual void FirstRand();	//��ʼվ�������ŵ�һ�׶���
	virtual void RandStand();   //�������һ�״�������

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
	virtual unsigned int GetAtkType() const;			//���Ǵ������ϻ�����
	virtual unsigned int GetAtkMagicType() const;		//���Ǵ������ϻ�����
	virtual unsigned int GetArmorType() const;			//���Ǵӻ����ϻ�����
	
	virtual void update(float dt);

	virtual void ShowHurt(int fDamage);
	virtual void HPChange();
	void ReloadWeaponRes();
	void SwitchRes(const char* pstActionName);
	virtual CCPoint GetFirePosition();
	virtual CCPoint GetSkillGunPoint(const string& stSkillAction);
	void PauseMovement();   //ֻ��ֹͣ����������scaleʲô�ļ���
	void ResumeMovement();	//�����Ŷ���
	bool IsShootable() const;
	int SwitchWeapon(unsigned int uiWeaponID, bool isNeedupdateData = true);     //��ǹ����ʱҪ���ⲿ��ȷ�������ɫ��ӵ������ǹ��
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

	//�Ƿ�ѣ��
	virtual bool IsDizzy() const{
		if(getMyState() == kActionStateDizzy){
			return true;
		}
		return false;
	}

	//static int GetResourceNames(unsigned int uiWeaponID, string& stBodyName, string& stLeftHandName, string& stRightHandName);
	static int GetResourceNames(unsigned int uiWeaponID, unsigned int uShowID, string& stBodyName);

	void ChangeWeapon();		//��ǹ�����ݱ仯,��ս��ʹ�ã�ͬ��������Ҫ�ı����

	//������ʾ����
	void showName(string nick, bool isSelf = false);

	virtual void setScaleX(float fScaleX);
	virtual void setScale(float fScale);
	virtual void setScaleY(float fScale);
	virtual float getScaleX();
	virtual float getScaleY();

	virtual void walkActWithSpeed(unsigned int speed);		//�ƶ�ʱ�ö���ƥ��
	virtual void winAct(bool isLoop);			//ʤ������

	//virtual void reLive();						//����

	void setVIPLv(unsigned int uVIPLv); // �ȼ�����0����ʾvipͷ��

private:
	void HandleMovementEvent(CCArmature * armature, MovementEventType type, const char * name);
	void CheckShooting(double dt);
	void ShowGunFire();
	void HideGunFire();
	CCPoint GetGunRelativePos();
	
private:
	unsigned int m_uHeroState;
	bool m_bAttacking;
	//��ǰ������
	CC_SYNTHESIZE(CWeapon, m_stCurWeapon, Weapon);					  //��ǰ�����������ݽṹ���������£������ö��󣬷����ָ�룩
	//CC_SYNTHESIZE(string, m_stLeftHandRes, LeftHandRes);
	//CC_SYNTHESIZE(string, m_stRightHandRes, RightHandRes);
// 	CC_SYNTHESIZE(string, m_stBodyRes, BodyRes);

	CC_SYNTHESIZE(unsigned int, m_uShowID, ShowID);
	double m_dLastFireTime;    //��һ���ӵ������ʱ��
// 	CCArmature* m_pstBody_attack;			//����Ĺ���,����������
// 	CCArmature* m_pstBody_stand;			//����Ĺ���,վ��������
// 	CCArmature* m_pstBody_skill;			//����Ĺ���,���ܹ���������
// 
// 	CCArmature* m_pstLeftHand_attack;			//����Ĺ���,���ֹ���������
// 	CCArmature* m_pstLeftHand_stand;			//����Ĺ���,����վ��������
// 	CCArmature* m_pstLeftHand_skill;			//����Ĺ���,���ּ��ܹ���������
// 
// 	CCArmature* m_pstRightHand_attack;			//����Ĺ���,���ֹ���������
// 	CCArmature* m_pstRightHand_stand;			//����Ĺ���,����վ��������
// 	CCArmature* m_pstRightHand_skill;			//����Ĺ���,���ּ��ܹ���������

	CCArmature* m_pstLeftHand;		//���ֹ���
	CCArmature* m_pstRightHand;		//���ֹ���
	CCArmature* m_pstGunFireAni;    //ǹ�������Ч
	unsigned int m_uWeaponMovementID;  //ǹе�Ķ�������
	unsigned int m_uWeaponGuesture; //��ǹ����
	int m_iGunfireSoundID;			//��ǹ����Ч
	string m_stFirePointStr;		//����һ��ǹ��λ�õĹ�������
	CCPoint m_stFirePointPos;       //����ǹ�ڹ�����λ�ã�����ÿ֡����һ��
	int m_iNextState;               //��һ��״̬����Ҫ����ս���и�Ϊidleʱ�����ڻ���
	CC_SYNTHESIZE(double, m_dFireStateChgTime, FireStateChgTime);
	CC_SYNTHESIZE(int, m_iBulletDamageRangeHight, BulletDamageRangeHight);
	CC_SYNTHESIZE(int, m_iBulletDamageRangeWidth, BulletDamageRangeWidth);
	UIImageView *m_pTitleImg; // ͷ��

protected:
	virtual void HandleEntityDeath();
};

#endif