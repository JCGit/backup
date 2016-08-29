#ifndef FightGuideMapScene_h__
#define FightGuideMapScene_h__

#include "fight/BaseFightScene.h"

class CFightTipLayout;
class CBossWarning;

class CFightGuideMapScene : public CBaseFightScene
{
public:
	CFightGuideMapScene();
	virtual ~CFightGuideMapScene();

	CREATE_FUNC(CFightGuideMapScene);

	bool init();

	void onEnterTransitionDidFinish();

	void update(float delta);

	void initMap();

	void loadMapBgRes();
	void loadHeroRes();											//��ɫ��Դ�ļ��أ���̬��
	void loadMonsterRes();										//������Դ�ļ��أ���̬
	void loadItemRes();											//����item��Դ�ļ��أ���̬
	void loadFloorBuffRes();									//����buff��Դ����
	void loadTrapRes();											//������Դ����

protected:
	void setMapRes();											//ͨ����ͼ��������ÿ�����Դ��
	void setWalkHight();

	//init���
	void initSceneItem();										//�ؿ���ͼitem
	void initFloorBuff();										//�ر�buff
	void initTrap();											//����
	void initMapUI();											//�ؿ�UI
	void initMainHero();										//���
	void initMonster();											//��ʼ����ǰ�׶εĹ��Ԥ������һ�׶εĹ�����Դ
	virtual void initFightData();								//��ʼ��ս����ص�����

	void initSleepMonsters();									//��ʼ�����ߵĹ��������̨��

	void startFight();											//��ʼս��

	//�����¼�
	void btnTouchEvent(CCObject *pSender, TouchEventType type);

	//update���
	void updateInstnaceByAI();									//����AI
	void brushMonsterLogic(float dt);							//ˢ���߼�
	void handlePassLogic();										//�����߼�
	void handleGiveUpLogic(CCObject* pstObj);

	void updateSceneBuffs(float dt);							//�ر�buff����

	//�ӽ�update
	void setViewpointCenter(const CCPoint& position);			//�ӽǾ���
	void updateBgViewCenter(float foreX);						//���µ�ͼǰ����Զ�����ӽ�(���ݵ�ͼ��ͬ����д)

	void handleGameOver();										//���ͽ�������

protected:
	//���ǻ��Ѿ��¼�
	void handleHeroKilledEvent(const string& stEvent, CCObject* pstEventObj);
	void handleFriendHurtValueEvent(const string& stEvent, CCObject* pstEventObj);	//�Ѿ���Ѫ
	void handleHeroEmergeCompleteEvent(const string& stEvent, CCObject* pstEventObj);	//���ǳ����������

	//�����¼�
	void handleEnemyHarmValueEvent(const string& stEvent, CCObject* pstEventObj);
	void handleEnemyDieCompleteEvent(const string& stEvent, CCObject* pstEventObj);
	void handleHeroKillValueChange(const string& stEvent, CCObject* pstEventObj);
	void handleEnemyDieMovementEvent(const string& stEvent, CCObject* pstEventObj);	//��������˲������ݼ�¼

	//��������¼�
	void handleSceneItemFallEvent(const string& stEvent, CCObject* pstEventObj);
	void handleSceneItemDisappearEvent(const string& stEvent, CCObject* pstEventObj);	//�����ʧ

	//ʤ����Ч�������
	void handleWinEffComplete(CCArmature * armature, MovementEventType type, const char * name);

	//boss���沥�����
	void handleBossWarningCompleteEvent(const string& stEvent, CCObject* pstEventObj);

private:
	void setAttackOpBtnVisible(bool bIsVisible);

	void checkShowGoTip();										//����Ƿ���ʾ��ǰ�ߣ�����̨���޹�������׶�δ���أ���ʾ��ǰ
	void nextStageTip();										//��һ�׶���ʾ
	void cancelTip();											//ȡ����ʾ

	void checkBrushSceneItem();									//����Ƿ񰴴�����ˢ��� OR BUFF
	void brushSceneItemByTouch(unsigned int touchID);			//���ݴ�����ˢ���
	void brushFloorBuffByTouch(unsigned int touchID);			//���ݴ�����ˢfloor buff
	void brushTrapByTouch(unsigned int touchID);				//���ݴ�����ˢ����

	void checkShowStage();										//��ʾ��ǰ�׶�

	void brushMonster();										//ˢ�־����߼�
	void brushMonsterByTouch(CEnemyData* monsterObj);			//���ݴ�����ˢ��
	void activateMonsterByTouch(CEnemyData* monsterObj);		//���ݴ����߼������
	void activateCarByTouch(CEnemyData* monsterObj);			//���ݴ����ߴ����Ȼ������Ź������			
	void brushFromCar(CCObject* pstObject);						//�ӾȻ�����ˢ������
	void brushFromCarOne(CCObject* pstObject);					//һ�����ӾȻ�����ˢ������
	void carMoveEnd(CCObject* pstObject);						//�Ȼ����Ƴ���Ļ

	void brushAfterDie(CEnemyData* pstObj);						//��ǰ�׶ΰ���������cdˢ��
	void updateTimeByKey(unsigned int key);						//����key����ʱ��

	void showBossWarning();										//boss���־���

	void handleFailLogic();										//ʧ���߼�

	void nextStage();											//��һ�׶�
	void delPreStage();											//ɾ��ǰһ�׶�
	void passStageViewChange(float dt);

	//�����߼�
	void handleSuccessLogic();									//�ɹ��߼�
	bool checkWinLogic(unsigned int cfgID);						//���ͨ������

	CCRect getStageRect() const;								//��ȡ��ǰ�׶εĿ���������

	void moveToDst(CQStageEntity* pstEntity, CCPoint stPos);	//�ƶ���Ŀ�ĵ�
	void onArrived(const string &evt, CCObject *pSender);	    //����Ŀ�ĵ�

	void checkFightTipShow();
protected:
	CFightTipLayout* m_pnextStageTip;							//��һ�׶���ʾ
	CBossWarning* m_pBossWarning;								//boss���־���
	ImageView* m_pTipImg;
protected:
	unsigned int m_uTotalStage;									//�ܽ���
	unsigned int m_uCurStageIdx;								//��ǰ�׶Σ���1��ʼ(�ӵ��Ĳ߻�˵��)

	unsigned int m_uCurStageMonNum;								//��ǰ�׶ι�������
			
	float m_fStartBrushTime;									//���׶ο�ʼˢ��ʱ��
	bool m_bStartedBrush;										//���׶��Ƿ�ʼˢ��
	bool m_bPassViewChange;										//ͨ���׶ε��ӽǱ仯
	bool m_bHasShowStage;										//�Ƿ���ʾ����ǰ�׶�

	int m_uPassType;											//ͨ������,(0��δͨ��)

	bool m_bIsIntroduced;										//boss�Ƿ��Ѿ����ܹ�

	unsigned int m_uLastKillID;									//ʤ����ʱ��ɱ�������һ������id
	bool m_bIsPreSuccess;										//�ȴ�ʤ��
	bool m_bIsWinComplete;										//ʤ����Ч�������Ƿ񲥷����

	map<int, vector<CEnemyData*> > m_stIDXToMonsters;			//��¼ÿ���׶ε�ˢ�ֵ㣬<int, CCArray*>�׶�������Ӧ����
	map<int, int> m_stIDXToMonsterNum;							//��¼ÿ���׶εĹ�������
	map<int, vector<CItemObj*> > m_stIDXToItems;				//��¼ÿ���׶ε�item��<int, CCArray*>�׶�������Ӧitem
	map<int, vector<CFloorBuffObj*> > m_stIDXToFloorBuffs;		//��¼ÿ���׶ε�floorbuff��<int, CCArray*>�׶�������Ӧfloorbuff
	map<int, vector<CTrapObj*> > m_stIDXToTraps;				//��¼ÿ���׶ε����壬<int, CCArray*>�׶�������Ӧ����

	vector<CEnemyData*> m_stCurStageMonsters;					//��ǰ�׶�ˢ�ֵ����Ϣ
	map<unsigned int, int> m_stMonKeyToTime;					//ÿ���׶�ÿ�������Ӧ����ʼˢ��ʱ��(key��id��x��y�ϳɣ�valueΪʱ��)

	vector<CEnemyData*> m_stBrushAfterDieArr;					//��ǰ�׶ΰ���������cdˢ�ֵ�����
	vector<unsigned int> m_stDieMonsterData;					//�������������

	CC_SYNTHESIZE(CCArray*, _sleepMonsters, SleepMonsters);		//��ǰ�׶������еĹ���
	vector<CEnemyData*> m_stCurCarMonsters;						//��ǰ�׶ξȻ�����ˢ�ֵ�

	CCPoint m_stBuffPos;
};

#endif // FightGuideMapScene_h__
