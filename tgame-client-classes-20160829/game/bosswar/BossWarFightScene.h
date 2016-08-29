#ifndef BossWarFightScene_h__
#define BossWarFightScene_h__

#include "fight/BaseFightScene.h"
#include "DummyBoss.h"
#include "CSBossChallengeMsg.pb.h"

class CBossWarWaitLayer;
class CHeroReviveLayer;
class CBossWarInfoLayout;

using namespace vmsg;

class CBossWarFightScene : public CBaseFightScene
{
public:
	enum {
		WORLD_BOSS_SCENE_TYPE = 100001,
	};

public:
	CBossWarFightScene();
	virtual ~CBossWarFightScene();

	CREATE_FUNC(CBossWarFightScene);

	bool init();

	void onEnterTransitionDidFinish();

	void update(float delta);

	void loadMapBgRes();										//��ͼ������Դ�ļ��أ���̬
	void loadHeroRes();											//��ɫ��Դ�ļ��أ���̬��
	void loadMonsterRes();										//������Դ�ļ��أ���̬
	void loadItemRes();											//����item��Դ�ļ��أ���̬
	void loadFloorBuffRes();									//����buff��Դ����
	void loadTrapRes();											//������Դ����

	void initMap();

	//Э�鴦��
	void onBuyReviveCallBack();									//����Э�鷵�ش���
	void doDamageNotify(const CSBossChallengeDamageNotification& stRsp);	//�˺�ͬ��

	void doWarOver();

protected:
	void setWalkHight();										//���ÿ���������
	void initSceneItem();										//�ؿ���ͼitem
	void initFloorBuff();										//�ر�buff
	void initTrap();											//����
	void initMapUI();											//�ؿ�UI
	void initMainHero();
	void initMonster();											//��ʼ����ǰ�׶εĹ��Ԥ������һ�׶εĹ�����Դ
	void initFightData();										//��ʼ��ս����ص�����

	void startUpdate();											//��ʼ����

	void brushMonsterLogic(float dt);							//ˢ���߼�
	void handlePassLogic();										//�����߼�
	void handleGiveUpLogic(CCObject* pstObj);

	void setViewpointCenter(const CCPoint& position);			//�ӽǾ���

	void handleGameOver();										//���ͽ�������

	void btnTouchEvent(CCObject *pSender, TouchEventType type);
protected:
	//���ǻ��Ѿ��¼�
	void handleHeroKilledEvent(const string& stEvent, CCObject* pstEventObj);
	void handleHeroEmergeCompleteEvent(const string& stEvent, CCObject* pstEventObj);	//���ǳ����������

	//�����¼�
	void handleEnemyUpDataEvent(const string& stEvent, CCObject* pstEventObj);		//����ǿ��һ��Ѫʱ�ϱ�����
	void handleEnemyHurtEvent(const string& stEvent, CCObject* pstEventObj);
	void handleEnemyHarmValueEvent(const string& stEvent, CCObject* pstEventObj);
	void handleEnemyDieCompleteEvent(const string& stEvent, CCObject* pstEventObj);
	void handleHeroKillValueChange(const string& stEvent, CCObject* pstEventObj);
	void handleEnemyDieMovementEvent(const string& stEvent, CCObject* pstEventObj);	//��������˲������ݼ�¼

	void handleCloseAtkKeyFrameEvent(const string& stEvent, CCObject* pstEventObj);
	void handleSkillKeyFrameEvent(const string& stEvent, CCObject* pstEventObj);
	void handleFireABulletEvent(const string& stEvent, CCObject* pstEventObj);
	void handleDieEffKeyFrameEvent(const string& stEvent, CCObject* pstEventObj);
	void handleEnemySkillActionEvent(const string& stEvent, CCObject* pstEventObj);
	void handleEnemyDashCompleteEvent(const string& stEvent, CCObject* pstEventObj);
	void handleEnemyDieEffectEvent(const string& stEvent, CCObject* pstEventObj);
	void handleHeroEPAddEvent(const string& stEvent, CCObject* pstEventObj);

	//����
	void handleSkillClickEvent(const string& stEvent, CCObject* pstEventObj);
private:
	void doUploadHitMonster();		//�ϱ��ҵ��˺�

	CDummyBoss* initOneDummyBoss(unsigned int uCfgID, unsigned int uLv); //��ʼ��һ����������

	//��ʾ׼��
	void showWaitLayer();
	void onWaitDone(const string& stEvent, CCObject* pstEventObj);

	//��ɫ����
	void showReviveLayer();
	void onReviveDone(const string& stEvent, CCObject* pstEventObj);

	void showDamageEff(UINT64_t uDamge);
	void showDamageEffCallBack(CCObject* pstObj);

private:
	CDummyBoss* m_pBoss;

	CBossWarWaitLayer* m_pWaitLayer;
	CHeroReviveLayer* m_pReviveLayer;
	CBossWarInfoLayout* m_pWarInfoLayout;

private:
	CCPoint m_stBossInitPos;		//bossλ��
	CCPoint m_stMainHeroInitPos;	//���ǵĳ�ʼλ�ã�����ʱ���ã�

	bool m_bIsShowWait;				//�Ƿ���ʾ׼��ʱ��

	unsigned int m_uBossShowID;

	bool m_bIsUpSkillID;			//�Ƿ��ϱ�ʹ�þ���
	unsigned int m_uUpHarmValue;	//�ϱ��˺�ֵ

	UINT64_t m_uInitBossHp;
	UINT64_t m_uPreBossHp;		
};

#endif // BossWarFightScene_h__
