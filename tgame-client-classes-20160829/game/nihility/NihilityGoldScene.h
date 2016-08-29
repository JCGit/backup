#ifndef NihilityGoldScene_h__
#define NihilityGoldScene_h__

#include "fight/BaseFightScene.h"
#include "fightendless/FightEndlessScoreUI.h"

class CNextStageTipLayer;

class CNihilityGoldScene : public CBaseFightScene
{
public:
	CNihilityGoldScene();
	virtual ~CNihilityGoldScene();

	CREATE_FUNC(CNihilityGoldScene);

	virtual bool init();

	virtual void onEnterTransitionDidFinish();

	void update(float delta);

	void loadHeroRes();											//��ɫ��Դ�ļ��أ���̬��
	void loadMonsterRes();										//������Դ�ļ��أ���̬

	void initMap();

	CEnemy* initOneMonster(CEnemyData* monsterObj);		

	void updateCoin();
	void updateScore();
	void updatePackage();

	void setAtkBtnEnable(bool isEnable);
protected:
	void startUpdate();											//��ʼ����

	//init���
	void initMapUI();											//�ؿ�UI
	void initMainHero();										//���
	void initMonster();											//��ʼ����ǰ�׶εĹ��Ԥ������һ�׶εĹ�����Դ
	void initFightData();										//��ʼ��ս����ص�����

	void brushMonsterLogic(float dt);							//ˢ���߼�

	//����ս��
	void handleGiveUpLogic(CCObject* pstObj);					//��;�����˳��ؿ�

	//update���
	void handlePassLogic();										//�����߼�

	void checkMonsterOutStage();								//�������Ƿ񳬳���Ļ���������Ƴ�

	void handleGameOver();										//���ͽ�������

	void handleCoinEndEvent();

	void autoFightHandle();
protected:
	void handleCancelAutoEvent(const string& stEvent, CCObject* pstEventObj);

	//����������δ����ս��ʧ��
	void doHeroDieFail();

	//�����¼�
	void handleEnemyDieMovementEvent(const string& stEvent, CCObject* pstEventObj);	//��������˲������ݼ�¼
	void handleEnemyDieCompleteEvent(const string& stEvent, CCObject* pstEventObj);
	void handleHeroKillValueChange(const string& stEvent, CCObject* pstEventObj);

private:
	void initMonsterAddToStage(CEnemyData* pstObj);

	void addNextStageTip();										//������һ�׶���ʾ
	void removeStageTip();										//�Ƴ����͵�
	void checkArrivedEdge();									//����Ƿ���һ�׶�

private:
	//����
	float m_fObjectTime;					//��¼ʱ��,��ʱ��
	float m_fStartWalkTime;					//���׶�ʱ���ǿ�ʼ����ʱ��

	unsigned int m_uCurMonsterLev;			//����ȼ�
	unsigned int m_uCurMonsterNum;			//��ǰ�׶ι������� 
	unsigned int m_uCurLeftMonsterNum;		//��ǰ�׶λ�ʣ����ֻ��Ҫˢ

	bool m_bIsArrivedEdge;					//�Ƿ��ڵ������ұ�
	bool m_bIsAutoWalking;					//�Ƿ��Զ�������һ�׶�

	map<unsigned int, int> m_stMonKeyToTime;//ÿ���׶�ÿ�������Ӧ����ʼˢ��ʱ��(key��id��x��y�ϳɣ�valueΪʱ��)

	CNextStageTipLayer* m_pTipLayer;		//��һ�׶���ʾ
	CFightEndlessScoreUI* m_pScoreUI;
};

#endif // NihilityGoldScene_h__
