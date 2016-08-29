#ifndef Bullet_h__
#define Bullet_h__
#include "cocos2d.h"
#include "cocos-ext.h"
#include "Macros.pb.h"
#include "game/role/QStageEntity.h"
#include "CfgMaster/BulletCfgMaster.h"
#include "vlib/QEventDispatcher.h"

USING_NS_CC;
USING_NS_CC_EXT;

enum BULLET_PARTY {
	BULLET_PARTY_HERO = 0,		//�Լ���
	BULLET_PARTY_FRIEND = 1,	//���ѵ�
	BULLET_PARTY_ENEMY = 2		//���˵�
};

class CBullet : public CCNode, public CQEventDispatcher
{
public:
	CBullet();
	virtual ~CBullet();

	CREATE_FUNC(CBullet);

	virtual void onExit();
	virtual void onEnter();
	void update(float dt);

	void LoadRes();

	void Fly();
	bool IsContinuous() const{
		return (_trail == TRA_CON_HORI_LINE);
	}

	bool IsPenatrating() const{
// 		if ( _trail == TRA_CON_HORI_LINE || _trail == TRA_CON_SPREAD){
// 			return true;
// 		}

		return ((_trail == TRA_ISO_FULL_TRACE || _trail == TRA_ISO_FORWARD_TRACE || _trail == TRA_ISO_HORI_LINE) && (_maxPenatrate - getPenatrate()) > 0);
		
				
	}

	float GetLeftPenaRatio() const{
		if (_trail == TRA_CON_HORI_LINE || _trail == TRA_CON_SPREAD || _trail == TRA_JTL_LINE){
			return 1;
		}

		float fRatio = (_maxPenatrate -_penatrate) * _attenuation * 1.0 / (_maxPenatrate * RANDOM_DOMINATOR);
		if (fRatio < 0){
			return 0;
		}else{
			return fRatio;
		}		
	}


	static const string BULLET_HIT_KEY;

	bool canTargetBeAdded(unsigned int uID);

	void AddTargetToPenatrate(unsigned int uID);
protected:
	void updatePostion(float dt);
	void traceTarget(float dt);
	void traceForwardTarget(float dt);
	void straightLineFly(float dt);
	void spread(float dt);
	void straightContinous(float dt, bool isSpread);
	void HandleMovementEvent(CCArmature * armature, MovementEventType type, const char * name);
	void HandleFrameEvent(CCBone *bone, const char * stEvent, int originFrameIndex, int currentFrameIdx);
private:
	CC_SYNTHESIZE(std::string, _res, Res);
	CC_SYNTHESIZE(std::string, _hurtRes, HurtRes);
	CC_SYNTHESIZE(CCArmature*, _bullet, Bullet);
	
	CC_SYNTHESIZE(int, _atk, AtkVal);				//����ֵ
	CC_SYNTHESIZE(int, _speed, Speed);				//�ӵ������ٶ�
	CC_SYNTHESIZE(CCPoint, _destination, Destination); //��Զ��Ŀ�����
	CC_SYNTHESIZE(int, _depth, Depth);             //ֱ����ɢ�ͣ���ʾһ���м����ӵ���ֱ�������ԣ���ʾ�м���ԭʼ��ȣ����ڵ����ӵ���˵��������岻��
	CC_SYNTHESIZE(int, _light, Light);			   //���ȣ�һ������£������Աȶȵ�������������£��滻��ʾ��Դ
	CC_SYNTHESIZE(int, _trail, Trail);             //�����켣�����ڿ��Ƶ����ķ�������
	CC_SYNTHESIZE(int, _zalias, Zalias);		   //�ӵ���z��Ͷ�䣬���ڼ�����ЩĿ����Ա�������
	CC_SYNTHESIZE(unsigned int, _atkMagic, AtkMagic);       //���ӵ�������������
	CC_SYNTHESIZE(unsigned int, _atkRatio, AtkRatio);       //���ӵ�Я���Ĺ����ӳ�
	CC_SYNTHESIZE(unsigned int, _atkType, AtkType);         //���ӵ��Ĺ�������
	CC_SYNTHESIZE(unsigned int, _bulletID, BulletID);		//�ӵ�������ID
	CC_SYNTHESIZE(unsigned int, _hit, Hit);					//�ӵ�����
	
	CC_SYNTHESIZE(CCObject*, _target, Target);				//Ŀ�����

	CC_SYNTHESIZE(CCPoint, _velocity, Velocity);			//�ӵ������ٶ�
	CC_SYNTHESIZE(unsigned int, _damageZone, DamageZone);			//�ӵ������ٶ�
	CC_SYNTHESIZE(bool, m_bShouldClear, ShouldClear);									//��ĳЩ״̬�£�Ҫ�Ѹ��ӵ�ɾ�����������Ѿ����жԷ����������Ѿ��ɳ��磬��������Ŀ���Ѿ���ʧ
	CC_SYNTHESIZE(unsigned int, _shootDistance, ShootDistance);
	CC_SYNTHESIZE(CQStageEntity*, _ownerEntity, Owner);			//����ӵ�������˭��
	CC_SYNTHESIZE(int, _penatrate, Penatrate);			//��͸�ĸ���
	CC_SYNTHESIZE(float, _hitDist, HitDist);					//ʵ�ʴ�����ľ���	
	CC_SYNTHESIZE(float, _attenuation, Attenuation);			//˥��ϵ��
	CC_SYNTHESIZE(float, _maxPenatrate, MaxPenatrate);			//��������
	CC_SYNTHESIZE(unsigned int, _party, Party);				//�ӵ�������һ��
	CC_SYNTHESIZE(float, _undealedTime, UndealedTime);
	CC_SYNTHESIZE(int, _hitBackDist, HitBackDist);			//�����˵ľ���
    CC_SYNTHESIZE(int, _ownerLevel, OwnerLevel);			//���˵ĵȼ�
    CC_SYNTHESIZE(int, _roleType, RoleType);			//�ӵ������ĸ�RoleType
	CC_SYNTHESIZE(const BulletCfg*, _cfg, Cfg);			//�ӵ���cfg�ṹ
	CC_SYNTHESIZE(int, _evtCnt, EvtCnt);				//�ӵ��˺�֡�¼�����

	unsigned int m_apHitTargets[5];


};

#endif // Bullet_h__
