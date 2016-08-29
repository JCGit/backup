#ifndef __GUARD_ACT_SCENE_h__
#define __GUARD_ACT_SCENE_h__


#include "fight/BaseFightScene.h"
#include "OSDSceneInf.h"
#include "BasicTypes.h"

using namespace vmsg;

class CGuardActRankUI;
class CGuardActPrizeUI;
class DummyHero;
class DummyEnemy;
class CEnemyData;
class CReviveLayer;

class GuardActScene : public CBaseFightScene, public OSDSceneInf
{
public:
    //����һЩ����
    enum {
        PATH_LINE_NUM       = 5, //���ﰴ����·������
        PATH_LINE_HEIGHT    = 50, //һ��·�ߵĸ߶�
        ENEMY_UIN_MAX       = 1000, //����Uin�����ֵ
        BASE_STAT_LOSE      = 0, //�ʧ��
        BASE_STAT_DIE       = 1, //���ع���
        BASE_STAT_LIVE      = 2, //���ػ���
        SCREEN_NUM          = 2, //
    };

public:
    GuardActScene();
    virtual ~GuardActScene();

    CREATE_FUNC(GuardActScene);
    bool init();
    virtual void onEnterTransitionDidFinish();
    void update(float delta);
    virtual void cleanup();

    void initMap();
    void buildMap();

    void loadMonsterRes();										//������Դ�ļ��أ���̬
    void loadItemRes();											//����item��Դ�ļ��أ���̬

    void brushMonsterLogic(float dt); //��BaseFightScene֡��update�ﴥ��

    //osd
	void addRoleQueue(const vmsg::CSPlayerPosInScene& stInfo);
    void addRole(const vmsg::CSPlayerCommInScene& playerInfo, const CCPoint &pos, const CCPoint &direction); // ���һ����ɫ��������
    void removeRole(UIN_t uin); // �ӳ������Ƴ�һ����ɫ
    void setRolePos(UIN_t uin, const CCPoint &pos, const CCPoint &direction); // ����һ����ɫ��λ�ã��ƶ���ָ��λ��
    void moveRoleByDirection(UIN_t uin, const CCPoint &startPos, const CCPoint &direction); // ͨ�������ƶ���ɫ
    void moveRoleToDestination(UIN_t uin, const CCPoint &endPos, const CCPoint &direction); // �ƶ���ɫ��Ŀ�ĵ�
    virtual void setPlayerStartAtkStat(const UIN_t uin, const vmsg::CSPlayerStartAtk& stStartAtk); // ���ý�ɫ���״̬
    virtual void setPlayerStopAtkStat(const UIN_t uin, const vmsg::CSPlayerStopAtk& stStopAtk); // ���ý�ɫֹͣ���״̬
    virtual void addMonster(const vmsg::CSOSDMonstersInfo& monsterInfo); // ���һ�����ﵽ������
    virtual void setPlayerSkillInScene(const UIN_t uin, const vmsg::CSPlayerSkillInScene& stSkillInScene); // ���ý�ɫ�ͷż���

    // hud layer delegate
    virtual void didChangeDirectionTo(HudLayer *hud, CCPoint direction);
    virtual void isHoldingDirection(HudLayer *hud, CCPoint direction);
    virtual void simpleDPadTouchEnded(HudLayer *hud);

    virtual void mainHeroShoot(); //��д���
    virtual void mainHeroCeaseShoot();

    //����Э��
    void setRoomReady();
    void doControlNotify(const CSCVZControlNotify& stCtrlNotify); //���¿��ƹ�����Ϣ
    void doHitMonsterNotify(const CSCVZHitMonsterNotify& stHitMonsterNotify); //ͬ������Ѫ��
    void doHitedNotify(const CSCVZHitedNotify& stHitedNotify); //Role������
    void doDestroyBaseNotify(const CSCVZDestroyBaseNotify& stDestroyBaseNotify); //�ݻٻ���
    void doFail(); //�ʧ��
    void doMonstersInfoNotify(const CSCVZMonsters& stMonstersInfo); //���¹���״̬��λ��
    void doRankInfoNotify(const CSCVZRankInfo& stMonstersInfo); //��������
    void doWaveTimeNotify(const int iTime); //����ˢ��ʱ����ʾ
    void doActOver(); //�����
    void doTotalContNotify(const int iTotalCont); //�ҵĹ���ֵ֪ͨ
    void doMyContAwardCoinNotify(const int iCoin); //�ҵ�Coin֪ͨ

protected:
    void updateInstnaceByAI();
    virtual void setWalkHight();								//���ÿ���������
	virtual void handleHeroEmergeCompleteEvent(const string& stEvent, CCObject* pstEventObj);	//���ǳ����������
    virtual void handleHeroKilledEvent(const string& stEvent, CCObject* pstEventObj);
    virtual void handleFriendHurtValueEvent(const string& stEvent, CCObject* pstEventObj); //�Ѿ���Ѫ
    virtual void handleEnemyHarmValueEvent(const string& stEvent, CCObject* pstEventObj);
    virtual void handleEnemyDieCompleteEvent(const std::string& stEvent, CCObject* pstEventObj); //��������
	virtual void handleEnemyUpDataEvent(const std::string& stEvent, CCObject* pstEventObj);		//����ǿ��һ��Ѫʱ�ϱ�����

    virtual void initMapUI();											//�ؿ�UI
	virtual void initMonster();
    virtual void handlePassLogic();										//�����߼�
    void doGiveUP();													//�����˳���ť

    virtual void setMainHeroAsTheOne() {
    }

    //���ܰ�ť�¼�
    virtual void handleSkillClickEvent(const string& stEvent, CCObject* pstEventObj);
    virtual void handleGiveUpLogic(CCObject* pstObj);

	virtual void btnTouchEvent(CCObject *pSender, TouchEventType type);
private:
    DummyHero * initOneDummyHero(const UIN_t uin, const CCPoint& pos, unsigned int showID, int weaponCfgID); //��ʼ��һ����������
    DummyEnemy* initOneDummyEnemy(const UIN_t uin, const CCPoint& pos, const int level, const int wave); //��ʼ��һ����������
    int genMonsterCfg(int level, int iWave, MonsterCfg& cfg);

    void doUploadHitMonster(); //�ϱ��ҵ�ɱ������
    void doUploadMonsterInfo(); //�ϱ��ҿ��ƵĹ���λ������
     
    inline bool isEnemyUin(const UIN_t uUin) {
        return (uUin <= ENEMY_UIN_MAX);
    }
    CQStageEntity* getStageEntity(const UIN_t uUin);


    void showReviveLayer();
    void onReviveDone(const std::string& stEvent, CCObject* pstEventObj);


    void showWaitingRoomLayer(); //��ʾ�ȴ�ƥ�䷿��

    CCPoint randMonsterPos(const int iChannel); //roll һ��λ��

	void haneldBossArmCallBack(CCArmature* armature, MovementEventType type, const char* stName);
	void addBossByIdx(int iIdx);	
	void bossArmBoom(int iIdx);  //���ر�ը
	void clearBossArm(CCArmature* armature);
private:
    CGuardActRankUI *m_pRankUI; //����UI
    CGuardActPrizeUI *m_pPrizeUI; //����UI

    CCDictionary *m_pRolesDict; // �����еĶ��ѽ�ɫ�б� <UIN_t, DummyHero>
    CCDictionary *m_pEnemysDict; // �����еĹ����б� <UIN_t, DummyEnemy>

    CCArray* m_stHeroRes;			//������Դ
    CCPoint m_stLastDirection;

    std::map<UIN_t, unsigned int> m_stPlayerHurtData; //<����UIN, �˺�ֵ>����¼�����ϱ�����ڼ����ǶԸ���������˺�

    bool m_bIsRoomReady; //�Ƿ�ƥ����˷���

    CCPoint m_stMainHeroInitPos; //���ǵĳ�ʼλ�ã�����ʱ���ã�
    CReviveLayer* m_pReviveLayer;

    std::set<UIN_t> m_stMyCtrlEnemy; //����AI�����Ĺ���
    int m_stBaseStat[PATH_LINE_NUM]; //0��ʾ���ݻ٣�1��ʾ����

	CCArmature* m_stBossArm[PATH_LINE_NUM];		//5��������

    bool m_bIsActOver; //��Ƿ����
	
	map<int, CCPoint> m_stID2Point;		//id��Ӧλ��
};


#endif // __GUARD_ACT_SCENE_h__
