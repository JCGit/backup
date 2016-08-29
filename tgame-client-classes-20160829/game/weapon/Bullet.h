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
	BULLET_PARTY_HERO = 0,		//自己的
	BULLET_PARTY_FRIEND = 1,	//好友的
	BULLET_PARTY_ENEMY = 2		//敌人的
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
	
	CC_SYNTHESIZE(int, _atk, AtkVal);				//攻击值
	CC_SYNTHESIZE(int, _speed, Speed);				//子弹飞行速度
	CC_SYNTHESIZE(CCPoint, _destination, Destination); //最远的目标距离
	CC_SYNTHESIZE(int, _depth, Depth);             //直线离散型，表示一共有几排子弹；直线连续性，表示有几倍原始宽度（对于单个子弹来说，这个意义不大）
	CC_SYNTHESIZE(int, _light, Light);			   //亮度，一般情况下，制作对比度调整，特殊情况下，替换显示资源
	CC_SYNTHESIZE(int, _trail, Trail);             //弹道轨迹，用于控制弹道的分析策略
	CC_SYNTHESIZE(int, _zalias, Zalias);		   //子弹的z轴投射，用于计算哪些目标可以被他击中
	CC_SYNTHESIZE(unsigned int, _atkMagic, AtkMagic);       //该子弹带出来的属性
	CC_SYNTHESIZE(unsigned int, _atkRatio, AtkRatio);       //该子弹携带的攻击加成
	CC_SYNTHESIZE(unsigned int, _atkType, AtkType);         //该子弹的攻击类型
	CC_SYNTHESIZE(unsigned int, _bulletID, BulletID);		//子弹的配置ID
	CC_SYNTHESIZE(unsigned int, _hit, Hit);					//子弹命中
	
	CC_SYNTHESIZE(CCObject*, _target, Target);				//目标对象

	CC_SYNTHESIZE(CCPoint, _velocity, Velocity);			//子弹飞行速度
	CC_SYNTHESIZE(unsigned int, _damageZone, DamageZone);			//子弹飞行速度
	CC_SYNTHESIZE(bool, m_bShouldClear, ShouldClear);									//在某些状态下，要把该子弹删掉，比如他已经击中对方，比如他已经飞出界，比如他的目标已经消失
	CC_SYNTHESIZE(unsigned int, _shootDistance, ShootDistance);
	CC_SYNTHESIZE(CQStageEntity*, _ownerEntity, Owner);			//这个子弹是属于谁的
	CC_SYNTHESIZE(int, _penatrate, Penatrate);			//穿透的个数
	CC_SYNTHESIZE(float, _hitDist, HitDist);					//实际打击到的距离	
	CC_SYNTHESIZE(float, _attenuation, Attenuation);			//衰减系数
	CC_SYNTHESIZE(float, _maxPenatrate, MaxPenatrate);			//最大击穿数
	CC_SYNTHESIZE(unsigned int, _party, Party);				//子弹属于哪一方
	CC_SYNTHESIZE(float, _undealedTime, UndealedTime);
	CC_SYNTHESIZE(int, _hitBackDist, HitBackDist);			//被击退的距离
    CC_SYNTHESIZE(int, _ownerLevel, OwnerLevel);			//主人的等级
    CC_SYNTHESIZE(int, _roleType, RoleType);			//子弹属于哪个RoleType
	CC_SYNTHESIZE(const BulletCfg*, _cfg, Cfg);			//子弹的cfg结构
	CC_SYNTHESIZE(int, _evtCnt, EvtCnt);				//子弹伤害帧事件数量

	unsigned int m_apHitTargets[5];


};

#endif // Bullet_h__
