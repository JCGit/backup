#ifndef FightMapScene_h__
#define FightMapScene_h__

#include "BaseFightScene.h"
#include "role/Enemy.h"
#include "FightTipLayout.h"

class CBossWarning;
class CNextStageTipLayer;

class CFightMapScene : public CBaseFightScene
{
public:
    virtual void release(void);
    virtual void retain(void);
    virtual CCObject* autorelease(void);

	CFightMapScene();
	virtual ~CFightMapScene();

	CREATE_FUNC(CFightMapScene);

    bool init();
    void onExit();

	virtual void onEnterTransitionDidFinish();

	void update(float delta);

	void initMap();

	void loadMapBgRes();
	void loadHeroRes();											//��ɫ��Դ�ļ��أ���̬��
	void loadMonsterRes();										//������Դ�ļ��أ���̬
	void loadItemRes();											//����item��Դ�ļ��أ���̬
	void loadFloorBuffRes();									//����buff��Դ����
	void loadTrapRes();											//������Դ����

protected:
	//init���
	void initSceneItem();										//�ؿ���ͼitem
	void initFloorBuff();										//�ر�buff
	void initTrap();											//����
	void initMapUI();											//�ؿ�UI
	void initFriend();											//�Ѿ�
	void initMonster();											//��ʼ����ǰ�׶εĹ��Ԥ������һ�׶εĹ�����Դ
	virtual void initFightData();								//��ʼ��ս����ص�����

	void initSleepMonsters();									//��ʼ�����ߵĹ��������̨��

	//�����¼�
	void btnTouchEvent(CCObject *pSender, TouchEventType type);

	//update���
	void updateInstnaceByAI();									//����AI
	void brushMonsterLogic(float dt);							//ˢ���߼�
	void handlePassLogic();										//�����߼�
	void handleGiveUpLogic(CCObject* pstObj);

	//�ӽ�update
	void setViewpointCenter(const CCPoint& position);			//�ӽǾ���
	void updateBgViewCenter(float foreX);						//���µ�ͼǰ����Զ�����ӽ�(���ݵ�ͼ��ͬ����д)

	void handleGameOver();										//���ͽ�������

protected:
	//����������δ����ս��ʧ��
	void doHeroDieFail();		

	//���ǻ��Ѿ��¼�
	void handleFriendDieEvent(const string& stEvent, CCObject* pstEventObj);		//�Ѿ�����
	void handleFriendHurtValueEvent(const string& stEvent, CCObject* pstEventObj);	//�Ѿ���Ѫ
	void hanldeWinCompleteFun(const string& stEvent, CCObject* pstEventObj);		//ʤ�������������

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

private:
	void checkShowGoTip();										//����Ƿ���ʾ��ǰ�ߣ�����̨���޹�������׶�δ���أ���ʾ��ǰ
	void showGoTip();											//ǰ����ʾ
	void cancelGoTip();											//ȡ����ʾ

	void checkBrushSceneItem();									//����Ƿ񰴴�����ˢ��� OR BUFF
	void brushSceneItemByTouch(unsigned int touchID);			//���ݴ�����ˢ���
	void brushFloorBuffByTouch(unsigned int touchID);			//���ݴ�����ˢfloor buff
	void brushTrapByTouch(unsigned int touchID);				//���ݴ�����ˢ����

	void checkShowStage();										//��ʾ��ǰ�׶�

	void brushMonster();										//ˢ�־����߼�

	void handleFailLogic();										//ʧ���߼�

	void doNormalWarHideAward();								//��ͨ�ؿ����ؽ����ж�
	bool IsAchieveHideAward(unsigned int uType, unsigned int uValue);	//����Ƿ�ﵽ��������

	void doDailyInstanceOver();

	void setAtkBtnEnable(bool isEnable);
	void checkNextStage();										
	void checkArrivedEdge();
	void walkToNextStage();
	void removeStageTip();
	void nextStage();											//��һ�׶�
	void setNextStagePos();
	void handleTransitionMidEvent(const string& stEvent, CCObject* pstEventObj);
	void handleTransitionComplete(const string& stEvent, CCObject* pstEventObj);

	void passStageViewChange(float dt);

	//�����߼�
	void handleSuccessLogic();									//�ɹ��߼�
	bool checkWinLogic(unsigned int cfgID);						//���ͨ������

	//�ؿ�boss��������Ұ��ʱ������boss�������
	void checkBossInView();
	void showBossEmergePanel();

	CCRect getStageRect() const;								//��ȡ��ǰ�׶εĿ���������

protected:
	CFightTipLayout* m_pnextStageTip;							//��һ�׶���ʾ
	CNextStageTipLayer* m_pTipLayer;							//��һ�׶���ʾ

protected:
	unsigned int m_uTotalStage;									//�ܽ���
	unsigned int m_uCurStageIdx;								//��ǰ�׶Σ���1��ʼ(�ӵ��Ĳ߻�˵��)

	unsigned int m_uCurStageMonNum;								//��ǰ�׶ι�������

	float m_fStageStartTime;									//ÿ�׶ο�ʼ��ʱ��

	float m_fStartWalkTime;										//���׶�ʱ���ǿ�ʼ����ʱ��

	bool m_bStartedBrush;										//���׶��Ƿ�ʼˢ��
	bool m_bHasShowStage;										//�Ƿ���ʾ����ǰ�׶�

	bool m_bIsAutoWalking;		
	bool m_bIsArrivedEdge;

	bool m_bIsIntroduced;										//boss�Ƿ��Ѿ����ܹ�

	unsigned int m_uLastKillID;									//ʤ����ʱ��ɱ�������һ������id
	bool m_bIsWinComplete;										//ʤ����Ч�������Ƿ񲥷����

	map<int, vector<CEnemyData*> > m_stIDXToMonsters;			//��¼ÿ���׶ε�ˢ�ֵ㣬<int, CCArray*>�׶�������Ӧ����
	map<int, int> m_stIDXToMonsterNum;							//��¼ÿ���׶εĹ�������
	map<int, vector<CItemObj*> > m_stIDXToItems;				//��¼ÿ���׶ε�item��<int, CCArray*>�׶�������Ӧitem
	map<int, vector<CFloorBuffObj*> > m_stIDXToFloorBuffs;		//��¼ÿ���׶ε�floorbuff��<int, CCArray*>�׶�������Ӧfloorbuff
	map<int, vector<CTrapObj*> > m_stIDXToTraps;				//��¼ÿ���׶ε����壬<int, CCArray*>�׶�������Ӧ����

	vector<CEnemyData*> m_stCurStageMonsters;					//��ǰ�׶�ˢ�ֵ����Ϣ

	vector<CEnemyData*> m_stBrushAfterDieArr;					//��ǰ�׶ΰ���������cdˢ�ֵ�����
	vector<unsigned int> m_stDieMonsterData;					//�������������
};

#endif // FightMapScene_h__
