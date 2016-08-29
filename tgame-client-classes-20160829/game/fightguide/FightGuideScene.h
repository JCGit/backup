#ifndef FightGuideScene_h__
#define FightGuideScene_h__

#include "fight/BaseFightScene.h"

class CFightTipLayout;
class CFightGuideHud;

class CFightGuideScene : public CBaseFightScene
{
public:
	CFightGuideScene();
	virtual ~CFightGuideScene();

	CREATE_FUNC(CFightGuideScene);

	bool init();

	void onEnterTransitionDidFinish();

	void update(float delta);

	void initMap();

	void loadMonsterRes();										//������Դ�ļ��أ���̬

	CEnemy* initOneMonster(CEnemyData* monsterObj);				//��ʼ��һ������

	void setAttackOpBtnVisible(bool bIsVisible);				//���ù��������ܰ�ť�Ƿ�ɼ�

	void showFireGuide();										//ָ��ս��

	void showMoveFireGuide(int direct);

	void removeGuide();

	void removeFireBtns();

	FireButton* getFireBtn(){
		return fireBtn;
	}

	SkillButton* getSkillBtn(){
		return m_trickBtn;
	}

	bool checkInFireGuide();		

	bool checkInMoveFireGuide();

	void showMoveToGuide();
	void moveHero();

	void showSkillGuide();

	void moveToEnd();
	void checkArrivedEdge();									//����Ƿ���һ�׶�

	void unSetMainHeroAsTheOne();
protected:
	void initMapUI();											//�ؿ�UI
	void initMainHero();										//���
	void initMonster();											//��ʼ����ǰ�׶εĹ��Ԥ������һ�׶εĹ�����Դ
	void initFightData();										//��ʼ��ս����ص�����

	void updateMonsterArrow();									//������̨�Ϲ���ָ����ͷ
	void brushMonsterLogic(float dt);							//ˢ���߼�

	void handleGiveUpLogic(CCObject* pstObj);					

	//�����¼�
	void btnTouchEvent(CCObject *pSender, TouchEventType type);

	//�ӽ�update
	void updateBgViewCenter(float foreX);						//���µ�ͼǰ����Զ�����ӽ�(���ݵ�ͼ��ͬ����д)

protected:
	//�����¼�
	void handleEnemyDieCompleteEvent(const string& stEvent, CCObject* pstEventObj);
	void handleHeroKillValueChange(const string& stEvent, CCObject* pstEventObj);
	void handleEnemyDieMovementEvent(const string& stEvent, CCObject* pstEventObj);	//��������˲������ݼ�¼

	//���ܰ�ť�¼�
	void handleSkillClickEvent(const string& stEvent, CCObject* pstEventObj);
private:
	void nextStageTip();										//��һ�׶���ʾ
	void cancelTip();											//ȡ����ʾ

	void moveToDst(CQStageEntity* pstEntity, CCPoint stPos);	//�ƶ���Ŀ�ĵ�
	void onArrived(const string &evt, CCObject *pSender);	    //����Ŀ�ĵ�

	unsigned int getNowStepType();

	void handleTransitionMidEvent(const string& stEvent, CCObject* pstEventObj);
	void handleTransitionComplete(const string& stEvent, CCObject* pstEventObj);
private: 
	CFightTipLayout* m_pNextStageTip;							//��һ�׶���ʾ
	CFightGuideHud* m_pGuideHud;
	ImageView* m_pTipImg;

	float m_fObjectTime;										//ʱ��

	unsigned int m_uGuideStep;									//ָ������
	unsigned int m_uArriveNum;									//�����ƶ���Ŀ�ĵ������
	unsigned int m_uCurStageMonNum;								//��ǰ�׶ι�������
	unsigned int m_uBrushNum;
	unsigned int m_uLastMonUID;

	bool m_bIsArrivedEdge;
	CC_SYNTHESIZE(bool, m_bCanBrushMonster, CanBrushMonster);	//�Ƿ����ˢ��

	vector<CEnemyData*> m_stCurStageMonsters;					//��ǰ�׶�ˢ�ֵ����Ϣ
};

#endif // FightGuideScene_h__
