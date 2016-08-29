#ifndef EndlessFightScene_h__
#define EndlessFightScene_h__

#include "fight/BaseFightScene.h"
#include "vlib/CVArray.h"
#include "fight/PauseLayer.h"
#include "FightEndlessScoreUI.h"
#include "FightEndlessWaveUI.h"

#include "MapCfgMaster.h"

const int m_stMonsterMark[4] = {20, 30, 50, 80};   //�����Ӧ�ķ���

class CEndlessFightScene : public CBaseFightScene
{
public:
	CEndlessFightScene();
	virtual ~CEndlessFightScene();

	CREATE_FUNC(CEndlessFightScene);

	virtual bool init();

	virtual void onEnterTransitionDidFinish();

	void update(float delta);

	void loadMapBgRes();										//��ͼ������Դ�ļ��أ���̬
	void loadHeroRes();											//��ɫ��Դ�ļ��أ���̬��
	void loadMonsterRes();										//������Դ�ļ��أ���̬
	void loadItemRes();											//����item��Դ�ļ��أ���̬

	void initMap();

protected:
	//init���
	void initSceneItem();										//�ؿ���ͼitem
	void initMapUI();											//�ؿ�UI
	void initMainHero();										//���
	void initMonster();											//��ʼ����ǰ�׶εĹ��Ԥ������һ�׶εĹ�����Դ
	void initFightData();										//��ʼ��ս����ص�����

	void setMapRes();											//ͨ����ͼ��������ÿ�����Դ��
	void setWalkHight();										//���ÿ���������

	//update���
	//void brushMonsterLogic(float dt);							//ˢ���߼�
	void handlePassLogic();										//�����߼�

	//�ӽ�update
	void updateBgViewCenter(float foreX);						//���µ�ͼǰ����Զ�����ӽ�(���ݵ�ͼ��ͬ����д)

	//����ս��
	virtual void handleGiveUpLogic(CCObject* pstObj);			//��;�����˳��ؿ�

protected:
	//����¼�
	void handleHeroKilledEvent(const string& stEvent, CCObject* pstEventObj);

	//�����¼�
	void handleEnemyHarmValueEvent(const string& stEvent, CCObject* pstEventObj);
	void handleEnemyDieCompleteEvent(const string& stEvent, CCObject* pstEventObj);
	void handleHeroKillValueChange(const string& stEvent, CCObject* pstEventObj);

private:
	void startFightCountDown();									//��ʼս������ʱ		
	void startEndless(const string& stEvent, CCObject* pstEventObj);//�޾�ģʽ��ʼ

	void prepareMonsterResByStatge(unsigned int stageIdx);		//׼���׶��й������Դ

	void prepareInitMonster(float dt);							//��ÿ��֮�䣬���һ��ʱ�乹��һ�������Ĺ������������̨

	void prepareInitOneMonster(unsigned int stageIdx);

	void genMonsterCfg(string avatar, MonsterCfg& cfg, int level = 1);			//����avatar���ɹ������ýṹ��

	void initMonsterLogic(bool isAddToStage = true);

	void initOneMosterByAvatar(string avatar, CCPoint stPos, int level = 1, bool isAddToStage = true);	//��ʼ���������ʱֹͣAI

	void activeMonsterByTime(float dt);							//����������AI

	void activeMonster();										//����

	void updateScore();											//���»���

	void nextStageTip();										//��һ�׶���ʾ

	void nextStage();											//��һ�׶�

	void handleGameOver();										//��������

	int rollPackage(unsigned int prob);							//�������
private:
	CFightEndlessScoreUI* m_pScoreUI;
	CFightEndlessWaveUI* m_pWaveUI;	
	CPauseLayer* m_pPauseLayer;

private:
	MapCfg m_stMapCfg;											//����ĵ�ͼ����
	CCArray* m_stNextMonsters;									//��һ�׶εĹ����ʱδ�ӵ���̨��

	float m_fLastTime;											//�޾�ģʽ��ʼ��������ʱ��
	float m_fStageStartTime;									//��ǰ�׶ο�ʼʱ��

	unsigned int m_uCurStageIdx;								//��ǰ��������1��ʼ(�ӵ��Ĳ߻�˵��)
	unsigned int m_uCurStageMonNum;								//��ǰ��̨�ϵĹ�������
	unsigned int m_uCurMonsterLev;								//��ǰ����ȼ�
	unsigned int m_uCurLimitTime;								//��ǰ�׶�����ʱ��
	unsigned int m_uCurTotalNum;								//��ǰ�׶ιֵ�����

	unsigned int m_uCurLeftNum;									//��ǰ��߹�������
	unsigned int m_uCurRightNum;								//��ǰ�ұ߹�������

	//��ǰ��������Ĳ���
	unsigned int m_uPreInitInter;								//��ǰ��������ļ��
	unsigned int m_uPreInitNum;									//��ǰ�������������

	pair<string, string> m_stCurAvatar;							//��ǰ�׶ε�2��avatar

	//ͳ��
	unsigned int m_uPackageCnt;									//��������
	unsigned int m_uCoin;										//������������
	unsigned int m_uCurScore;									//��ǰ����
	CVArray<int, 4> m_stMonsterCnt;								//����ɱ��������4������
};



#endif // EndlessFightScene_h__
