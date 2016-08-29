#ifndef ArenaFightScene_h__
#define ArenaFightScene_h__

#include "fight/BaseFightScene.h"
#include "vlib/CVArray.h"

#include "ArenaMapLibCfgMaster.h"

class CArenaFightInfoLayout;

class CArenaFightScene : public CBaseFightScene
{
public:
	CArenaFightScene();
	virtual ~CArenaFightScene();

	CREATE_FUNC(CArenaFightScene);

	void onEnterTransitionDidFinish();

	void update(float delta);

	void loadHeroRes();
	void loadMonsterRes();
	void loadItemRes();

	void initMap();

	CEnemy* addOneMonster(CEnemyData* pstObj, bool isEmerge);
	CEnemy* initOneMonster(CEnemyData* monsterObj);

	void stopAllMonsters();
protected:
	//init���
	void initSceneItem();										//�ؿ���ͼitem
	void initMapUI();											//�ؿ�UI
	void initMainHero();										//���
	void initMonster();											//��ʼ����ǰ�׶εĹ��Ԥ������һ�׶εĹ�����Դ
	void initOpponent();										//��ʼ������
	void initFightData();										//��ʼ��ս����ص�����

	void setMapRes();											//ͨ����ͼ��������ÿ�����Դ��
	void setWalkHight();										//���ÿ���������

	void brushMonsterLogic(float dt);							//ˢ���߼�
	void brushMonsterSpecial(CEnemyData* pstObj);				//����������ˢ��

	void brushOneMonster(CEnemyData* pstObj);

	//�ӽ�update
	void setViewpointCenter(const CCPoint& position);

	//����ս��
	void handleGiveUpLogic(CCObject* pstObj);					//��;�����˳��ؿ�

	void handleGameOver();										//���ͽ�������

	bool checkMonsterOnStage(CEnemyData* pstObj);				//���ĳˢ�ֵ��Ƿ��й�����̨��

	bool checkArrowCondition(CQStageEntity* pstEntity);			//����ָ����ͷ������

protected:
	//����������δ����ս��ʧ��
	void doHeroDieFail();

	//�����¼�
	void handleEnemyHurtEvent(const string& stEvent, CCObject* pstEventObj);
	void handleEnemyDieMovementEvent(const string& stEvent, CCObject* pstEventObj);	//��������˲������ݼ�¼
	void handleEnemyDieCompleteEvent(const string& stEvent, CCObject* pstEventObj);
	void handleHeroKillValueChange(const string& stEvent, CCObject* pstEventObj);

	//�����¼�
	void handleOpponentEPAddEvent(const string& stEvent, CCObject* pstEventObj);
	void handleOpponentHurtValueEvent(const string& stEvent, CCObject* pstEventObj);	//���ֵ�Ѫ
	void handleOpponentKilledEvent(const string& stEvent, CCObject* pstEventObj);		//��������

	//ʤ����Ч�������
	void handleWinEffComplete(CCArmature * armature, MovementEventType type, const char * name);

	//��ɱ��Ч
	void handleKillEffComplete(CCArmature * armature, MovementEventType type, const char * name);
private:
	void startPvp(const string& stEvent, CCObject* pstEventObj);

	void setCurBossAI();

	void rectifyPos(CEnemyData* pstObj);						//����ˢ�ֵ��λ��
	void initStage();

	void checkLimitTime();										//����Ƿ�ʱ
	void setCurStageBoss();
	CCPoint getOpponentPos();									//���ֳ���λ��
	bool checkWinLogic(unsigned int cfgID);						//����Ƿ�ͨ��
	void doDieMovement(unsigned int cfgID, unsigned int uID);
	void doWinLogic(unsigned int uID);
	void doWin();

	void nextStage(unsigned int uID);							//��һ�׶�
	void delBrushByStage();										//ͨ���׶κ󣬽����׶ε�ˢ�ֵ����

	void checkOppInRevive();									//�ж϶����Ƿ񸴻�
	void doReviveOpp();
	float getReiviveTime();										//��ø���ʱ��

	void initPowerGrid();										//��ӵ���
	void addBossTipEff(CEnemy* pEnemy);							//�ڹؿ�boss������ͷ�ϼ�����Ч
	void addOpTipEff();											//�������������Ч

	void playKillHeroEff();

	void doHeroReplyHp();										//����Ѫ����ep�ظ�		
private:
	CArenaFightInfoLayout* m_pInfoLayout;
	CMainHero* m_pOpponentHero;									//����Ӣ��

private:
	unsigned int m_uTotalStage;									//�ܽ׶���
	unsigned int m_uStageIdx;									//�׶�
	unsigned int m_uArenaLv;									//����������
	unsigned int m_uMonsterLev;									//����ȼ�
	unsigned int m_uLastKillID;									//ʤ����ʱ��ɱ�������һ������id

	unsigned int m_uOppDieNum;									//���ָ������

	unsigned int m_uKillBossNum;								//�Ѿ�ɱ��boss����
	unsigned int m_uBossTotalNum;								//ͨ��boss����

	float m_fLimitTime;
	float m_fOppDieTime;										//��������ʱ��
	float m_fStageW;

	bool m_bIsWin;	
	bool m_bIsWinComplete;
	bool m_bIsOppDying;											//���ָ�����
	bool m_bIsBossHited;										//boss�����Ƿ񱻹���

	bool m_bIsFirstEnter;										//�Ƿ��һ�ν���

	ArenaMapLibCfg m_stMapCfg;

	vector<CCPoint> m_stPosVec;									//����3���׶ε�ˢ��λ��
	CCArray* m_stAllBossData;									//����boss����
	CVArray<unsigned int, 4> m_stBossIDs;						//��ǰ�׶�bossIDs
	CCArray* m_stPowerGrids;									//���������δ��

	unsigned int m_uOpRandWeaponID;								//���������ǹ
};

#endif // ArenaFightScene_h__
