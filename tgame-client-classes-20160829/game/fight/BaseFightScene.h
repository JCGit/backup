#ifndef BaseFightScene_h__
#define BaseFightScene_h__

#include "cocos-ext.h"
#include "BaseMapScene.h"
#include "role/Enemy.h"
#include "role/MainHero.h"
#include "FightBossHeadUI.h"
#include "FightHeroHeadUI.h"
#include "MonsterCfgMaster.h"
#include "SceneBuff.h"
#include "SkillEffLayer.h"
#include "MonsterShowCfgMaster.h"
#include "SkillButton.h"
#include "WeaponBuff.h"
#include "SceneTrap.h"
#include "FireButton.h"
#include "FightHitLayout.h"

USING_NS_CC_EXT;

using namespace gui;

class CBossWarning;
class CAutoFightLayer;
class CFightWeaponArrLayout;

const int NEED_UPDATE_NUM = 1;
const unsigned int KILL_INTER = 3;	//��ɱ���
const float BG_RATIO = 0.1f;		//�������ƶ�������
const float FG_RATIO = 1.5f;		//ǰ�����ƶ�������
const float MID_RATIO = 0.6f;		//�о����ƶ�������
const float MID2_RATIO = 0.25f;		//�о����ƶ�������
const float START_BRUSH_DIS = 500;	//ˢ�ֵĴ�����
const UINT64_t TEMP_WEAPON_GUID = 9999;	//��ʱǹеGUID

//ʤ������
enum WIN_CONDITIONS{
	WIN_NONE = 0,				//δͨ��
	WIN_CLEAN_ALL = 1,			//��������
	WIN_DESTROY_ONE = 2,		//����ĳ��
	WIN_ALIVE_TIME = 3,			//һ��ʱ�䲻��
	WIN_DEFEND_TIME = 4,		//�ػ�һ��ʱ��
	WIN_DESTROY_NUM = 5 		//����һ������
};

enum FIGHT_MODEL{
	NORMAL_FIGHT = 1,			//��ͨս��
	ENDLESS_FIGHT = 2			//�޾�ģʽ
};

class CBaseFightScene : public CBaseMapScene, public HudLayerDelegate
{
public:
	static const unsigned int HURT_ARM_TAG;		//������Ч��tag��
	static const unsigned int KILL_EFF_TAG;		//��ɱ��Ч��tag��

public:
	CBaseFightScene();
	virtual ~CBaseFightScene();

	virtual void didChangeDirectionTo(HudLayer *hud, CCPoint direction);
	virtual void isHoldingDirection(HudLayer *hud, CCPoint direction);
	virtual void simpleDPadTouchEnded(HudLayer *hud);

	virtual bool init();

	virtual void onEnterTransitionDidFinish();

	virtual void update(float delta);

	virtual void initMap();
	virtual void buildMap();

	virtual void pauseFight();			//��ͣս��
	virtual void resumeFight();			//�ָ�ս��

	virtual void setMainHeroAsTheOne();
	virtual void unSetMainHeroAsTheOne();
	virtual void pauseAllMonsters();
	virtual void resumeAllMonsters();
	virtual void stopAllMonsters();
	virtual void checkTheOneState();

	//������
	virtual void mainHeroShoot();
	virtual void mainHeroCeaseShoot();

	virtual CEnemy* initOneMonster(CEnemyData* monsterObj);							//��ʼ��һ������
	virtual CEnemy* initOneMonster(CEnemyData* monsterObj, MonsterCfg* monsterCfg);	//��ʼ��һ������

	virtual CCPoint checkXYInWalkable(CCPoint pst);

	//�����ͷź󣬵ر�buff�Ĺ���
	virtual CSceneBuff* addBuffBySkill(CSkillEffLayer* pstSkill, CCPoint point);

	virtual CSceneBuff* addFloorBuff(unsigned int floorBuffID, unsigned int roleParty, CCPoint point, bool isDirectAdd = false);
	virtual CWeaponBuff* addWeaponBuff(unsigned int floorBuffID, unsigned int roleParty, CCPoint point, bool isDirectAdd = false);
	virtual void removeFloorBuff(CSceneBuff* pstBuff, bool isRemoveRoleBuff = true);		//isRemoveRoleBuff�Ƿ��Ƴ���ɫ���ϵ�buff

	//scene item
	virtual void addSceneItem(CItemObj* pstObj);

	//����
	virtual void addSceneTrap(CTrapObj* pstObj, bool isDirectAdd = false);
	virtual void addPunctureTrap(CTrapObj* pstObj, bool isDirectAdd = false);
	virtual void removeTrap(CSceneTrap* pstTrap, bool isRemoveRoleBuff = true);

	//��ǹ
	virtual void changeWeapon();
	//ĳЩ�ض��ӵ���֡�¼�
	virtual void HandleBulletKeyFrameEvent(const string& stEvent, CCObject* pstEventObj);

	const CCRect getPlayerVisibleSize(); //�ҵ����ǿɼ�����������

	virtual CCRect getStageRect() const;		//��ȡ��ǰ�׶εĿ���������

	virtual void setAttackOpBtnVisible(bool bIsVisible);				//���ù��������ܰ�ť�Ƿ�ɼ�

	virtual UILayout* getSkillLayout(){
		return m_pSkillLayout;
	}

	virtual void openModuleEffect(UIWidget* pLayout);
	virtual void handleOpenModuleEffComplete(CCArmature * armature, MovementEventType type, const char * name);

	virtual void brushOneMonster(CEnemyData* pstObj);
	virtual string getEmergeByData(CEnemyData* pstObj);
	virtual CEnemy* addOneMonster(CEnemyData* pstObj, bool isEmerge);
	virtual void handleComEmergeEvent(CCBone *bone, const char* stEvent, int originFrameIndex, int currentFrameIdx);
	virtual void handleComEmergeComplete(CCArmature * armature, MovementEventType type, const char * name);

	virtual void brushOneMonsterWithWalkIn(CEnemyData* pstObj);			//����Ļ������
	virtual void moveToDst(CQStageEntity* pstEntity, CCPoint stPos);	//�ƶ���Ŀ�ĵ�
	virtual void onMonsterArrived(const string &evt, CCObject *pSender);//����Ŀ�ĵ�

	virtual void showBossWarning();										//boss���־���
	virtual void handleBossWarningCompleteEvent(const string& stEvent, CCObject* pstEventObj);	//boss���沥�����

	virtual bool checkMonsterOnStage(CEnemyData* pstObj);				//���ĳˢ�ֵ��Ƿ��й�����̨��

	virtual void brushFromCar(CCObject* pstObject);						//�ӾȻ�����ˢ������
	virtual void brushFromCarOne(CCObject* pstObject);					//һ�����ӾȻ�����ˢ������
	virtual void carMoveEnd(CCObject* pstObject);						//�Ȼ����Ƴ���Ļ

	virtual bool checkPosInWalkable(CCPoint stPos);						//�����Ƿ��ڿ���������

	virtual CQStageEntity* getFriend();									//��ȡ��ս����

	virtual unsigned int getMonsterNumOnStage();									//�����̨�ϵĹ�������	

	virtual void handleArmatureRelease(CCArmature* armature, MovementEventType type, const char* name);

	virtual void startShowBulletOutTip();
	virtual void cancelshowBulletOutTip();
	virtual void showBulletOutTip(float dt);

	virtual void autoChangeNextWeapon();								//��ǰ��ҩ�þ����Զ��л�����һ��λ��ǹе
	virtual bool isAllWeaponOutBullet();								//��ҩ�Ƿ�ȫ������
	virtual void checkAllWeaponOutBullet();
	//virtual void autoFillAllBullets();									//�ж��Ƿ��е�ҩ�䣬�о��Զ����䵯ҩ��û�о͵�����ҩ�������
	virtual void fillAllWeaponBullet(bool bIsInvincible = true);			//���䵯ҩ�������ǰǹеΪ��ʱǹе��ͬ������
protected:
	CC_SYNTHESIZE(CMainHero*, _mainHero, MainHero);						//���
	CC_SYNTHESIZE(CCArray*, _stageEntities, StageEntities);				//��ǰ��̨������Ŀ��
	CC_SYNTHESIZE(CCArray*, _sceneItems, SceneItems)					//��ǰ��̨�ϵ�item
	CC_SYNTHESIZE(CCArray*, _skills, Skills);							//��ǰ��̨�ϵļ���     
	CC_SYNTHESIZE(CCArray*, _hero_bullets, HeroBullets);				//�ѷ��ӵ�
	CC_SYNTHESIZE(CCSize, _winSize, WinSize);
	CC_SYNTHESIZE(CCArray*, _sceneBuffs, SceneBuffs);					//�ر�buff������,ע��ÿ���׶���Ҫ���
	CC_SYNTHESIZE(CCArray*, _sceneTraps, SceneTraps);					//�ر����������,ע��ÿ���׶���Ҫ���
	
	CC_SYNTHESIZE(CCArray*, _sleepMonsters, SleepMonsters);				//��ǰ�׶������еĹ���
	CC_SYNTHESIZE(CCArray*, _moveEntities, MoveEntities);				//��̨��λ�ƵĽ�ɫ

	CC_SYNTHESIZE(bool, m_bIsAniEffOn, bIsAniEffOn);					//�Ƿ�����ڹ������ϼ���Ч����
	CC_SYNTHESIZE(int, m_uCurAniTotalNum, CurAniTotalNum);				//���������й���������Ч(���ˡ�������papa)����
	CC_SYNTHESIZE(int, m_uBulletSkillBuffNum, BulletSkillBuffNum);		//�������ӵ����������buff����

	CC_SYNTHESIZE(bool, m_bIsShowBulletOut, IsShowBulletOut);			//�Ƿ�����ʾ��ҩ�þ�����ʾ

	float m_fLastAniTime;												//�ϴ��ڹ������ϼ���Ч��ʱ��
	unsigned int m_uAniMaxNum;											//ս�������ˡ�������Ч��������
	float m_fAniInter;													//ս�������ˡ�������Ч���ʱ��

	float m_fMidPos;
	bool m_bIsPreOver;													//�Ƿ��Ѿ�׼������,���ڲ���ʤ����������
	bool m_bIsGameOver;													//game over

	bool m_bIsInTheOneState;											//�ͷż���״̬
	unsigned int m_uTheOneBeginSec;										//�ͷż��ܿ�ʼʱ��

	bool m_bDisturbShoot;												//�Ƿ������
	bool m_bShootAfterSkill;											//�ڼ����ͷź��Ƿ�������� 

	bool m_bIsMainHeroShooting;											//�����Ƿ������

	CCArray* m_stRightArrowArr;											//�����ұ�ָ����ͷ����
	CCArray* m_stLeftArrowArr;											//�������ָ����ͷ����

	CCPoint m_stTargetPos;												//�����ͷ�ʱĿ��λ��
protected:
	CCLayer* m_pGameLayer;
	UILayer* m_pWidgeLayer;
	UILayer *m_pUiLayer;
	HudLayer* _hudLayer;
	CFightHeroHeadUI* m_pHeroHeadUI;		//���Ѫ��
	CFightBossHeadUI* m_pBossHPUI;			//bossѪ��
	CFightHitLayout* m_pHitLayout;			//�������
	CFightWeaponArrLayout* m_pArrLayout;	//ǹе��λ
	UILayout *m_pSkillLayout;				//����layout
	UILayout* m_pPauseLayout;
	FireButton* fireBtn;
	UIButton* m_pAutoBtn;					//�йܰ�ť
	UIButton* pauseBtn;						//��ͣ��ť
	UIButton* m_pChangeWeaponBtn;			//��ǹ��ť

	SkillButton* m_trickBtn;				//ǹе���ܰ�ť
	SkillButton* m_skillBtn;				//�������ܰ�ť
	SkillButton* m_dodgeBtn;				//���ܼ��ܰ�ť

	CCSprite* m_pstBlackScreen;				//����
	CCArmature* m_pstSanguang;				//������Ч

	CBossWarning* m_pBossWarning;			//boss���־���

	CAutoFightLayer* m_pAutoLayer;			//�й�״̬
protected:
	//У������
	float m_fFightTime;						//�ؿ�ʱ��

	unsigned int m_uTotalMonsterHp;			//������Ѫ��
	unsigned int m_uTotalHarmValue;			//�˺���ֵ
	unsigned int m_uTotalEnemyKilled;		//ɱ������
	unsigned int m_uTotalLoseValue;			//��ʧ��ֵ
	unsigned int m_uMaxKillValue;			//�����ɱ
	unsigned int m_uCurKillValue;			//��ǰ��ɱ
	unsigned int m_uKillTime;				//ɱ��һ���ֵ�ʱ�� 
	unsigned int m_uScorePercent;			//���ֱ�(��ֱ�)
	unsigned int m_uMark;					//����
	unsigned int m_uScore;					//�Ǽ� (ʧ��Ϊ0)
	unsigned int m_uBossID;					//bossID (δɱ��Ϊ0)
	unsigned int m_uEvolutionCnt;			//������ʹ�ô���
	unsigned int m_uLimiteTime;				//����ʱ��
	unsigned int m_uWarLevel;				//�ؿ��ȼ�
	unsigned int m_uDropCoin;				//������
	unsigned int m_uUseMedicalCnt;			//ʹ��ҽҩ������
	unsigned int m_uUseReliveCnt;			//ʹ�ø��������
	unsigned int m_uUseBulletCnt;			//ʹ�õ�ҩ������

	//ʤ����ֵ
	unsigned int m_uWinType;
	unsigned int m_uWinValue1;
	unsigned int m_uWinValue2;		

protected:
	virtual void setWalkHight();										//���ÿ���������

	virtual void loadHeroRes();											//��ɫ��Դ�ļ��أ���̬�ģ��Լ�����Դ�Ѽ��أ��ѷ�����Դ��Ҫÿ�������Լ���д

	virtual void loadItemRes();

	virtual void loadFloorBuffRes();

	virtual void loadTrapRes();

	virtual void loadHeroResByShowID(unsigned int showID, unsigned int weaponCfgID, bool isPreLoad = true);		//ͨ��showid���ؽ�ɫ��Դ, isPreLoad�Ƿ�ΪԤ����

	virtual void loadWeaponeResByCfgID(unsigned int cfgID, bool isPreLoad = true);		//ͨ������cfgid�������������Դ�� isPreLoad�Ƿ�ΪԤ����

	//Ԥ���ؼ�����Դ
	virtual void loadSkillRes(unsigned int skillID, bool isPreLoad = true);	

	virtual void loadItemResByID(unsigned int itemID, unsigned int dropType, unsigned int dropID, bool isPreLoad = true);	//���ص����ĳ�����������Դ

	virtual void loadFloorBuffByID(unsigned int id, bool isPreLoad = true);		//���صر�buff�����Դ

	virtual void loadWeaponStarUpRes(unsigned int weaponCfgID, bool isPreLoad = true);					//����ǹе������ص���Դ

	//Ԥ����������Դ
	virtual void loadTrapResByID(unsigned int cfgId, bool isPreLoad = true);

	//Ԥ����buff��Դ
	virtual void loadBuffRes(unsigned int uBuffID, bool isPreLoad = true);

	//Ԥ����buff��Դ
	virtual void loadBuffRes(const string& stBuffRes, bool isPreLoad = true);

	//Ԥ���ع�����Դ, num:����������isPreLoad:�Ƿ�ΪԤ����
	virtual void loadMonsterResByCfg(MonsterCfg* stCfg, int num, bool isPreLoad = true);

	virtual void loadMonsterResByCfg(const MonsterShowCfg* stCfg, int num, bool isPreLoad = true);

	//init���
	virtual void initSceneItem();										//�ؿ���ͼitem
	virtual void initFloorBuff();										//�ر�buff
	virtual void initTrap();											//����
	virtual void initMapUI();											//�ؿ�UI
	virtual void initMainHero();										//���
	virtual void initFriend();											//�Ѿ�
	virtual void initMonster();											//��ʼ����ǰ�׶εĹ��Ԥ������һ�׶εĹ�����Դ
	virtual void initFightData();										//��ʼ��ս����ص�����

	virtual void startFight();											//��ʼս��
	virtual void startUpdate();											//��ʼ����

	//update���
	virtual void brushMonsterLogic(float dt);							//ˢ���߼�
	virtual void handlePassLogic();										//�����߼�

	virtual void updateMainHeroPosition();								//���λ�ø���

	//����buff����
	virtual void updateSceneBuffs(float dt);							//�ر�buff����

	//�ر��������
	virtual void updateSceneTraps(float dt);

	//��ť����
	virtual void updateBtnState();										//��ť����

	virtual void updateAniEffNum(float dt);								//���µ�ǰ����������Ч����

	//����AI
	virtual void updateInstnaceByAI();

	//����ս��
	virtual void handleGiveUpLogic(CCObject* pstObj);					//��;�����˳��ؿ�

	virtual bool needTurnBack();										//����Ƿ�ת����ֻ�������״̬�£�����û�е����ˣ�������е��˵�����£�����Ҫ��ת
	virtual bool needTurnBackForSkill();
	//����ָ����ͷ����
	virtual void updateMonsterArrow();									//������̨�Ϲ���ָ����ͷ
	virtual bool checkArrowCondition(CQStageEntity* pstEntity);			//����ָ����ͷ������

	//������̨���ƶ��������˺�
	virtual void updateMoveEntities();									//������̨���ƶ��������˺�

	virtual void handleGameOver();										//���ͽ�������

	virtual void clearAllSceneItems();									//���item
	virtual void clearAllBuffs();										//���buff���
	virtual void clearAllTraps();										//�������

	virtual void clearAllSkills();										//������������м���
	virtual void clearAllBullet();										//��������������ӵ�
	virtual void clearAllArrows();										//�ؿ�����ʱ��������м�ͷ

	virtual void clearAllMonsters();									//�ؿ�ʤ��ʱ��������ŵĹ���
	virtual void forceOhterMonstersDie();								//�ؿ�ʤ��ʱ��ǿ��������������

	virtual void updateHit();

	virtual void playKillEff();

	virtual void removeEntityBlock(CQStageEntity* pstEntity);

	virtual void updateMainHeroBulletTime(float dt);

protected:
	//�����¼�
	virtual void autoBtnEvent(CCObject *pSender, TouchEventType type);
	virtual void btnTouchEvent(CCObject *pSender, TouchEventType type);

	//���ǻ��Ѿ��¼�
	virtual void handleHeroHurtValueEvent(const string& stEvent, CCObject* pstEventObj); //��Ѫ
	virtual void handleHeroKilledEvent(const string& stEvent, CCObject* pstEventObj);//���ǻ��Ѿ�����
	virtual void handleHeroEPCostEvent(const string& stEvent, CCObject* pstEventObj);
	virtual void handleHeroHPChangeEvent(const string& stEvent, CCObject* pstEventObj);	//����Ѫ���仯
	virtual void handleHeroEmergeCompleteEvent(const string& stEvent, CCObject* pstEventObj);	//���ǳ����������
	virtual void handleHeroSkillCastEvent(const string& stEvent, CCObject* pstEventObj);		//�����ͷż����¼�

	//�����¼�
	virtual void handleEnemyHurtEvent(const string& stEvent, CCObject* pstEventObj);
	virtual void handleEnemyHarmValueEvent(const string& stEvent, CCObject* pstEventObj);
	virtual void handleHeroKillValueChange(const string& stEvent, CCObject* pstEventObj);
	virtual void handleHeroEPAddEvent(const string& stEvent, CCObject* pstEventObj);
	virtual void activeEnemyEvent(const string& stEvent, CCObject* pstEventObj);		//����������м�������¼�
	virtual void handleEnemySkillActionEvent(const string& stEvent, CCObject* pstEventObj);	//���＼���ͷ�ʱ����¼һЩ����
	virtual void handleEnemyDieMovementEvent(const string& stEvent, CCObject* pstEventObj);	//��������˲������ݼ�¼
	virtual void handleEnemyDieCompleteEvent(const string& stEvent, CCObject* pstEventObj);	//����������������¼�
	virtual void handleEnemyDashCompleteEvent(const string& stEvent, CCObject* pstEventObj);	//�����ײ����ʱ��
	virtual void handleEnemyDieEffectEvent(const string& stEvent, CCObject* pstEventObj);	//boss������������Ч����Ч�¼�
	virtual void handleAniEffCountEvent(const string& stEvent, CCObject* pstEventObj);		//��¼����������Ч����

	//�ؼ�֡�¼�
	virtual void handleHeroTheOneBeginEvent(const string& stEvent, CCObject* pstEventObj); //���ǷŴ��У�������ΪΨһ��ʾ
	virtual void handleHeroTheOneEndEvent(const string& stEvent, CCObject* pstEventObj);
    virtual void handleSkillKeyFrameEvent(const string& stEvent, CCObject* pstEventObj); //�ż��ܹؼ�֡�¼��������˺�����Ϣ
    virtual void handleFireABulletEvent(const string& stEvent, CCObject* pstEventObj);
	virtual void handleCloseAtkKeyFrameEvent(const string& stEvent, CCObject* pstEventObj);		
	virtual void handleDieEffKeyFrameEvent(const string& stEvent, CCObject* pstEventObj);		//������Ч֡�¼�
	virtual void handleSkillBtnStateEvent(const string& stEvent, CCObject* pstEventObj);		//���ܰ�ť״̬�ı䣬�ͷ���������ť���ɵ�
	virtual void handleHeroDashCompleteEvent(const string& stEvent, CCObject* pstEventObj);		//������������¼��������޵�buff

	//��������¼�
	virtual void handleSceneItemFallEvent(const string& stEvent, CCObject* pstEventObj);
	virtual void handleSceneItemDisappearEvent(const string& stEvent, CCObject* pstEventObj);	//�����ʧ

	//�ر�buff�¼�
	virtual void handleSceneBuffDisappearEvent(const string& stEvent, CCObject* pstEventObj);

	//�ر�����
	virtual void handleSceneTrapDisappearEvent(const string& stEvent, CCObject* pstEventObj);
	virtual void handleSceneTrapHitKeyEvent(const string& stEvent, CCObject* pstEventObj);

	//���ܰ�ť�¼�
	virtual void handleSkillClickEvent(const string& stEvent, CCObject* pstEventObj);

	//��ǹ
	virtual void handleHeroWeaponChange(const string& stEvent, CCObject* pstEventObj);

	//����Ƿ���Ҫ�߷��Է�
	virtual void handleTryKickEvent(const string& stEvent, CCObject* pstEventObj);
	
	//���������¼�
	virtual void handleKickEvent(const string& stEvent, CCObject* pstEventObj);

	//ʤ�������������
	virtual void hanldeWinCompleteFun(const string& stEvent, CCObject* pstEventObj);

	//������ʱ�¼�
	virtual void handleHitTimeOutEvent(const string& stEvent, CCObject* pstEventObj);

	//��ҵ���
	virtual void addCoinEff(CQStageEntity* pEntity, unsigned int uNum);
	virtual void updateCoinPos(float dt);								//�����ҵ�λ�ø���
	const string getCoinActName(unsigned int uNum);
	virtual void handleCoinActComplete(CCArmature* armature, MovementEventType type, const char* name);
	virtual void handleCoinEndEvent();

	//�Զ�ս��
	virtual void addAutoFightLayer();
	virtual void handleCancelAutoEvent(const string& stEvent, CCObject* pstEventObj);
	virtual void removeAutoFightLayer();
	virtual void autoFightHandle();

	//�������
	virtual void doHeroDieEvent();	
	virtual void doHeroDieFail();				//���δ���ս��ʧ��
	virtual void handleHeroReliveDoneEvent(const string& stEvent, CCObject* pstEventObj);
	virtual void handleHeroCancelReliveEvent(const string& stEvent, CCObject* pstEventObj);

	//��ҩ�þ�
	virtual void doAllBulletOut();
	virtual void handleFillBulletEvent(const string& stEvent, CCObject* pstEventObj);
	virtual void handleCancelFillBulletEvent(const string& stEvent, CCObject* pstEventObj);
	virtual void doCancelHeroAutoFight();

	//ҽҩ���þ�
	virtual void doMedicalBtnClick();
	virtual void doMedicalEff();				//ҽҩ�����ú���,�̶���Ѫ50%(��������)
	virtual void doMedicalOut();
	virtual void handleBuyMedicalEvent(const string& stEvent, CCObject* pstEventObj);
	virtual void handleCancelBuyMedicalEvent(const string& stEvent, CCObject* pstEventObj);

private:
	void updateSkillEffPos();
	void updateBullets(float dt);

	void changeTempWeapon(unsigned int weaponBuffID);					//������ʱǹе
	void weaponUpStar(unsigned int buffType);							//ǹе����buff

	void handleSanguangActionEvent(CCArmature * armature, MovementEventType type, const char * name);
	void handleDieEffActionEvent(CCArmature * armature, MovementEventType type, const char * name);
	void handleMainHeroHurtComplete(CCArmature * armature, MovementEventType type, const char * name);

/************************************************************************/
/*                       boss����                                       */
/************************************************************************/
public:
	//Ԥ����������¼�
	virtual void handleWarningCompleteEvent(const string& stEvent, CCObject* pstEventObj);

	//�����¼�
	virtual void handleSkillEffEvent(const string& stEvent, CCObject* pstEventObj);
	virtual void handleSkillEffAtkFrame(const string& stEvent, CCObject* pstEventObj);
	virtual void handleSkillEffSpawnFrame(const string& stEvent, CCObject* pstEventObj);
	virtual void handleSceenShakeEvent(const string& stEvent, CCObject* pstEventObj);		//����֡�¼�
private:
	unsigned int m_uSkillEffCnt;				//boss��һ����������Ч�ظ�����
	unsigned int m_uCurSKillCnt;				//boss��ǰ������Ч�Ѿ��ظ�����
	CQStageEntity* m_pSkillAtker;				//�����ͷ���
	CQSkillCell* m_pSkillCell;					//��������
	vector<CCPoint> m_stSkillPoints;			//bossһ�����ܵĶ����Ч��λ��

protected:
	CCArray* m_stAddCoins;						//��Ҫ��ӵĽ��
	CCArray* m_stCoinEffs;						//�����Ч����

	bool m_bIsPlayKillEff;						//�Ƿ񲥷���ɱ��Ч
	bool m_bIsAddCoinEff;						//�Ƿ񲥷Ž�ҵ���

	bool m_bIsGamePause;						//ս���Ƿ���ͣ
	bool m_bIsWalkNextStage;					//�Ƿ�����ͨ���׶�
public:
	//boss��ؼ���
	void skillRandFlyToGroud(CQStageEntity* pstAtker, CQSkillCell* pstSkill);
	void skillStepUpdate(float dt);
	void getSkillAttackPos(CQSkillCell* pstSkill);				//��ȡ���ܴ����λ�ã�����һ��Ϊ����λ�ã�������������Χ���
};

#endif // BaseFightScene_h__
