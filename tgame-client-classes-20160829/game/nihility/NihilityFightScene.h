#ifndef NihilityFightScene_h__
#define NihilityFightScene_h__

#include "fight/BaseFightScene.h"

class CNextStageTipLayer;
class CBossWarning;
class CopterEffLayer;
class CFightEndlessScoreUI;

class CNihilityFightScene : public CBaseFightScene
{
public:
	CNihilityFightScene();
	virtual ~CNihilityFightScene();

	CREATE_FUNC(CNihilityFightScene);

	virtual bool init();

	virtual void onEnterTransitionDidFinish();

	void update(float delta);

	void loadMapBgRes();										//��ͼ������Դ�ļ��أ���̬
	void loadHeroRes();											//��ɫ��Դ�ļ��أ���̬��
	void loadMonsterRes();										//������Դ�ļ��أ���̬
	void loadBrushRes();										//ֱ����Ч��Ҫ����Դ
	void loadItemRes();											//����item��Դ�ļ��أ���̬
	void loadFloorBuffRes();									//����buff��Դ����
	void loadTrapRes();											//������Դ����

	void initMap();

	CEnemy* initOneMonster(CEnemyData* monsterObj);						

	void updateCoin();
	void updateScore();
	void updatePackage(bool bIsShowEff = false);

	void setAtkBtnEnable(bool isEnable);

	string getEmergeByData(CEnemyData* pstObj);
protected:
	void startUpdate();											//��ʼ����

	//init���
	void initSceneItem();										//�ؿ���ͼitem
	void initFloorBuff();										//�ر�buff
	void initTrap();											//����
	void initMapUI();											//�ؿ�UI
	void initMainHero();										//���
	void initMonster();											//��ʼ����ǰ�׶εĹ��Ԥ������һ�׶εĹ�����Դ
	void initFightData();										//��ʼ��ս����ص�����

	void setMapRes();											//ͨ����ͼ��������ÿ�����Դ��
	void setWalkHight();										//���ÿ���������

	void brushMonsterLogic(float dt);							//ˢ���߼�

	//update���
	void handlePassLogic();										//�����߼�

	//����ս��
	void handleGiveUpLogic(CCObject* pstObj);					//��;�����˳��ؿ�

	void handleGameOver();										//���ͽ�������

	void handleCoinEndEvent();

	void autoFightHandle();
protected:
	void handleCancelAutoEvent(const string& stEvent, CCObject* pstEventObj);

	//����������δ����ս��ʧ��
	void doHeroDieFail();

	//�����¼�
	void handleHeroEmergeCompleteEvent(const string& stEvent, CCObject* pstEventObj);	//���ǳ����������

	//�����¼�
	void handleEnemyDieMovementEvent(const string& stEvent, CCObject* pstEventObj);	//��������˲������ݼ�¼
	void handleEnemyDieCompleteEvent(const string& stEvent, CCObject* pstEventObj);
	void handleHeroKillValueChange(const string& stEvent, CCObject* pstEventObj);

	//��������¼�
	void handleSceneItemFallEvent(const string& stEvent, CCObject* pstEventObj);
	void handleSceneItemDisappearEvent(const string& stEvent, CCObject* pstEventObj);	//�����ʧ

	//boss���沥�����
	void handleBossWarningCompleteEvent(const string& stEvent, CCObject* pstEventObj);

	//void btnTouchEvent(CCObject *pSender, TouchEventType type);
private:
	void startNihility();
	void startCopterEff();
	void handleCopterEffOverEvent(const string& stEvent, CCObject* pstEventObj);
	void startCopterFly();
	void copterFlyOut(CCArmature* armature);
	void copterFlyEndFun(CCObject* pstObject);
	void copterEffCallFun(CCArmature* armature, MovementEventType type, const char* nameStr);

	void packEndFun(CCNode* pNode);
	void handlePackageCallBack(CCArmature* armature, MovementEventType type, const char* nameStr);

	void countRushAward();										//����ɨ���Ľ���

	void prepAllStageRes();										//׼�����н׶ε���Դ

	void setMonsterNum();										//���õ�ǰ�׶ι�������
	void brushMonsters();

	void initSleepMonsters();									//���ߵĹ���
	void brushMonsterByTouch(CEnemyData* monsterObj);			//���ݴ�����ˢ��

	void checkBrushSceneItem();									//����Ƿ񰴴�����ˢ��� OR BUFF
	void brushSceneItemByTouch(unsigned int touchID);			//���ݴ�����ˢ���
	void brushFloorBuffByTouch(unsigned int touchID);			//���ݴ�����ˢfloor buff
	void brushTrapByTouch(unsigned int touchID);				//���ݴ�����ˢ����

	void showBossWarning();										//boss���־���

	bool checkWinLogic(unsigned int cfgID);						//�ж��Ƿ�ͨ��

	void addNextStageTip();										//������һ�׶���ʾ
	void removeStageTip();										//�Ƴ����͵�
	void checkArrivedEdge();									//����Ƿ���һ�׶�

	unsigned int getStageMapID(unsigned int uStageIdx);			//��õ�ǰ�׶εĵ�ͼID

	void resetStage();											//���ý׶�
	void clearPreStage();										//�����һ�׶�
	void nextStage();											//��һ�׶�

	void handleTransitionMidEvent(const string& stEvent, CCObject* pstEventObj);
	void handleTransitionComplete(const string& stEvent, CCObject* pstEventObj);

	int rollPackage(unsigned int uLv);							//�������
private:
	//����
	float m_fObjectTime;					//��¼ʱ��,��ʱ��
	float m_fStageTime;						//ÿ�׶ε�ʱ�䣬���׶�ʱ����
	float m_fStartWalkTime;					//���׶�ʱ���ǿ�ʼ����ʱ��

	unsigned int m_uCurLevel;				//��ǰ����
	unsigned int m_uCurStage;				//��ǰ�׶�������1��ʼ����3���׶�
	unsigned int m_uCurMonsterLev;			//����ȼ�
	unsigned int m_uCurMonsterNum;			//��ǰ�׶ι������� 

	//ÿ��3���ؿ���id
	unsigned int m_uNormalID;
	unsigned int m_uEliteID;
	unsigned int m_uBossID;

	bool m_bIsArrivedEdge;					//�Ƿ��ڵ������ұ�
	bool m_bIsAutoWalking;					//�Ƿ��Զ�������һ�׶�

	CCPoint m_stInitPos;
	CCArray* m_stSleepMonsters;

	CBossWarning* m_pBossWarning;			//boss���־���
	CNextStageTipLayer* m_pTipLayer;		//��һ�׶���ʾ
	CFightEndlessScoreUI* m_pScoreUI;
	CopterEffLayer* m_pCopterEff;
};

#endif // NihilityFightScene_h__
