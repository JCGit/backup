#ifndef __INSTANCE_UTILS_H__
#define __INSTANCE_UTILS_H__
#include "game/role/QStageEntity.h"
#include "Macros.pb.h"

#include "fight/SkillEffLayer.h"
#include "game/weapon/Bullet.h"
#include "fight/BaseFightScene.h"
#include "fight/EntityFightInfoBrief.h"
#include "fight/BaseSceneItem.h"


struct CBuffEffObj{
	CQStageEntity* pstEntity;
	unsigned int uiVal;
	unsigned int uiEffType;
	bool isAddVal;
	bool isAddBuff;

	CBuffEffObj(){
		pstEntity = NULL;
		uiVal = 0;
		uiEffType = 0;
		isAddVal = true;
		isAddBuff = true;
	}
};
typedef int(*pfEffCalculator_t)(CBuffEffObj stEffObj);

enum Z_ORDERS{
	FLOOR_BUFF_Z_ORDER = 1000,		//地表buff，陷阱的z
	FLOOR_OTHER_Z_ORDER = 2000,		//地表上其他，如技能前的警告
	ROLE_Z_ORDER = 3000,			//人物、怪物
	SKILL_Z_ORDER = 3010,			//技能特效
};

class CInstanceUtils{
public:
	static bool IsEntityInSkillEffRange(CSkillEffLayer* pstSkill, CQStageEntity* pstEntity);
	static void SkillAssitEffect(CSkillEffLayer* pstSkill, CQStageEntity* pstEntity, bool& isCleanSkillEff);
	static void SkillAtkEffect(CSkillEffLayer* pstSkill, CCArray* pstEnimies, bool& isCleanSkillEff, CBaseFightScene* pstFightScene);
	static void CloseSkillAtkEffect(CQStageEntity* pstAtker, CCArray* pstEntities, CBaseFightScene* pstFightScene); //近程攻击
	static void ActionTriggeredNoAniSkill(CQStageEntity* pstAtker, CCArray* pstEntities, CBaseFightScene* pstFightScene);   //由动作直接触发的没有任何技能特效的技能

	static int FindAtkSkill(CQStageEntity* pstAtker, float distX, float distY);
	static bool DoesContoursCollide(CCContourData* contour1, 
		CCContourData* contour2, 
		CCPoint pos1, 
		CCPoint pos2,
		float scale1_x = 1,
		float scale1_y = 1,
		float scale2_x = 1,
		float scale2_Y = 1);
	static bool DoTwoEntityCollide(CQStageEntity* pstAtk, CQStageEntity* pstDef, float fDist);
	static bool IsEntityInStraightSkillRange(CSkillEffLayer* pstSkill, CQStageEntity* pstEntity);
	static bool IsEntityInVerticalSkillRange(CSkillEffLayer* pstSkill, CQStageEntity* pstEntity);
	static void GetSomeTargets(const CCPoint& stOrig, unsigned int uiParity, CCArray* stObjects, int iNum, float fDist, CCArray* stTargets);

	static void SkillCollides(CSkillEffLayer* pstSkill, CCArray* pstEntities, CBaseFightScene* pstFightScene);
	static void SkillMakeEffect(CSkillEffLayer* pstSkill, CQStageEntity* pstEntity, CBaseFightScene* pstFightScene);
	static void SkillMakeEffect(CQSkillCell* pstSkill, CQStageEntity* pstAtker, CQStageEntity* pstEntity, CBaseFightScene* pstFightScene);
	static void SkillNormalAtkEffect(CSkillEffLayer* pstSkill, int ratio, CQStageEntity* pstTarget, int iCurAniEffNum);
	static void SkillHPAbsHurtEffect(CSkillEffLayer* pstSkill, int value, CQStageEntity* pstTarget, int iCurAniEffNum);

	static void SkillNormalAtkEffect(const SkillCfg* pstSkill, 
		const CEntityFightInfoBrief& stAtker, 
		int ratio,
		CQStageEntity* pstTarget, 
		int iBackScale,
		const CCPoint& stSkillPos,
		int iCurAniEffNum);
	static void SkillHPAbsHurtEffect(const SkillCfg* pstSkill, 
		int value, 
		CQStageEntity* pstTarget, 
		int iBackScale ,
		const CCPoint& stSkillPos,
		int iCurAniEffNum);

	static unsigned int GetGunDamage(const CQStageEntity* pstAtker, const CQStageEntity* pstDefer);
	static unsigned int GetBasicDamage(const CEntityFightInfoBrief& pstAtker, const CQStageEntity* pstDefer);
	static unsigned int BuildArmorRatioCfgID(unsigned int uAtkType, unsigned int uArmorType);
	static bool IsEntityInCircle(CQStageEntity* pstEntity, const CCPoint& stPos, unsigned int uRadius);
	static bool IsEntityInRect(CQStageEntity* pstEntity, const CCPoint& stPos, unsigned int x, unsigned int y);
	static bool IsEntityInCross(CQStageEntity* pstEntity, const CCPoint& stPos, unsigned int x, unsigned int y, unsigned int uDamageZone);
	static bool IsEntityInT(CQStageEntity* pstEntity, const CCPoint& stPos, unsigned int x, unsigned int y, unsigned int uDamageZone, float iXScale);

	static int AddEffect(CQStageEntity* pstEntity, unsigned int uiEffID, unsigned int uiEffVal, unsigned int uiEffType, bool isAdd);
	static int RemoveEffect(CQStageEntity* pstEntity, unsigned int uiEffID, unsigned int uiEffVal, unsigned int uiEffType, bool isAdd);

	static int AddContHpByCurRatio(CBuffEffObj stEffObj);
	static int AddContHpByMaxRatio(CBuffEffObj stEffObj);
	static int AddMaxHpByRatio(CBuffEffObj stEffObj);
	static int AddDefByRatio(CBuffEffObj stEffObj);
	static int AddAtkByRatio(CBuffEffObj stEffObj);
	static int AddIceDefByRatio(CBuffEffObj stEffObj);
	static int AddIceAtkByRatio(CBuffEffObj stEffObj);
	static int AddFireDefByRatio(CBuffEffObj stEffObj);
	static int AddFireAtkByRatio(CBuffEffObj stEffObj);
	static int AddPoisonDefByRatio(CBuffEffObj stEffObj);
	static int AddPoisonAtkByRatio(CBuffEffObj stEffObj);
	static int AddBioticDefByRatio(CBuffEffObj stEffObj);
	static int AddBioticAtkByRatio(CBuffEffObj stEffObj);
	static int AddDashSpeedByRatio(CBuffEffObj stEffObj);
	static int AddWalkSpeedByRatio(CBuffEffObj stEffObj);
	static int AddBulletDepthByRatio(CBuffEffObj stEffObj);
	static int AddBulletLightAtkByRatio(CBuffEffObj stEffObj);
	static int AddContHPByAbs(CBuffEffObj stEffObj);
	static int AddDizzyBuff(CBuffEffObj stEffObj);
	static int AddImmobilizeBuff(CBuffEffObj stEffObj);
	static int AddBlindBuff(CBuffEffObj stEffObj);
	static int AddInvincibleBuff(CBuffEffObj stEffObj);
	static int AddBulletDamageAdjBuff(CBuffEffObj stEffObj);
	static int AddMaxHpByFormula(CBuffEffObj stEffObj);

	static bool IsBulletHitEntity(CBullet* pstBullet, unsigned int uiParty, CQStageEntity* pstTarget);
	static bool IsBulletHitItem(CBullet* pstBullet, CSceneItem* pstItem);

	static bool IsCanHit(unsigned int hit, unsigned int atkLev, unsigned int defLev);
	static int CalBulletDamage(CBullet* pstBullet, CQStageEntity* pstTarget);
	static int CalBulletDamage(CBullet* pstBullet, CQStageEntity* pstTarget, double dt);
	static int CalBulletDamage(CBullet* pstBullet, double dt);
	static int CalBulletDamage(CBullet* pstBullet);

	static void HandleBulletHitTargetEffect(CBullet* pstBullet, CQStageEntity* pstTarget, CBaseFightScene* pstFightScene, double dt);
	static void HandleBulletHitItemEffect(CBullet* pstBullet, CSceneItem* pstItem, double dt);

	static void HandleTryKickEvent(CBaseFightScene* pstFightScene, CMainHero* pstHero);

	static void HandleKickEvent(CBaseFightScene* pstFightScene, CQStageEntity* pstEntity);

	static void FireABulletEvent(CQStageEntity* pstEntity, CBaseFightScene* pstLayer);
	static void RemoveLineBullets(CQStageEntity* pstEntity, CBaseFightScene* pstLayer);
	static bool IsTargetType(unsigned int uiEffectTargetType, 
		unsigned int uiOwnerParty,
		unsigned int uiTargetType,
		bool isSelf);

	//屏幕震动
	static void ShakeScene(BaseScene* pstScene, float d, float strength, ELayerID layerID = LAYER_ID_GAME);

	static void BuildEntityFightInfoBrief(CEntityFightInfoBrief& stBrief, CQStageEntity* pstEntity);

	static bool IsContoursCollide(CBaseSceneItem* pstItem, CQStageEntity* pstEntity);

	static void DashSkillAtk(CQStageEntity* pstAtker, CCArray* pstEntities, CBaseFightScene* pstLayer);   //冲撞攻击

	//预警点放置
	static void SkillWarningOnGround(CQStageEntity* pstAtker, CQSkillCell* pstSkill, CCPoint stPos, CBaseFightScene* pstLayer);		

	//在地上随机出现固定数量特效，其中一个固定为主角位置
	static void SkillRandOnGround(CQStageEntity* pstAtker, CSkillEffLayer* pstSkill, CCPoint stPos, CBaseFightScene* pstLayer);	

	//随机飞向几个固定点，其中一个固定为主角位置
	static void SkillRandFlyToGround(CQStageEntity* pstAtker, CSkillEffLayer* pstSkill, CCPoint stPos, CBaseFightScene* pstLayer);	

	//触发下一个动作
	static void SkillNextAction(CQStageEntity* pstAtker, CQSkillCell* pstSkill, CBaseFightScene* pstLayer);

	static int GetZOrderByType(unsigned int uType, float y);
private:
	static pfEffCalculator_t m_stBuffCalculators[MAX_BUFF_TYPE];
	static bool m_isInited;
};

#endif