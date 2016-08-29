#ifndef FightEndlessScene_h__
#define FightEndlessScene_h__

#include "fight/BaseFightScene.h"
#include "role/Enemy.h"
#include "MapCfgMaster.h"
#include "fight/PauseLayer.h"
#include "MonsterShowCfgMaster.h"
#include "vlib/CVArray.h"
#include "FightEndlessScoreUI.h"
#include "FightEndlessWaveUI.h"

const unsigned int INTER = 2.0f;
const int stMonsterMark[4] = {20, 30, 50, 80};   //�����Ӧ�ķ���

class CFightEndlessScene : public CBaseFightScene
{
public:
	CFightEndlessScene();
	virtual ~CFightEndlessScene();

	CREATE_FUNC(CFightEndlessScene);

	virtual bool init();

	virtual void onEnterTransitionDidFinish();

	void update(float delta);

	void loadMapBgRes();										//��ͼ������Դ�ļ��أ���̬
	void loadHeroRes();											//��ɫ��Դ�ļ��أ���̬��
	void loadMonsterRes();										//������Դ�ļ��أ���̬
	void loadItemRes();											//����item��Դ�ļ��أ���̬

	void initMap();

	virtual void cleanup();
protected:
	//init���
	void initSceneItem();										//�ؿ���ͼitem
	void initMapUI();											//�ؿ�UI
	void initFriend();											//�Ѿ�
	void initMonster();											//��ʼ����ǰ�׶εĹ��Ԥ������һ�׶εĹ�����Դ
	void initFightData();										//��ʼ��ս����ص�����

	void setMapRes();											//ͨ����ͼ��������ÿ�����Դ��
	void setWalkHight();										//���ÿ���������

	//update���
	void brushMonsterLogic(float dt);							//ˢ���߼�
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

	//��������¼�
	void handleSceneItemFallEvent(const string& stEvent, CCObject* pstEventObj);
private:
	void beginTip();											//��ʼ����ʱ
	void startEndless(const string& stEvent, CCObject* pstEventObj);//�޾�ģʽ��ʼ

	void prepareNextStage();									//Ԥ�ȼ�����һ���׶ιֵ���Դ
	void nextStageTip();										//��һ�׶���ʾ
	void nextStage();											//��һ�׶�

	void brushMonster();

	void handleGameOver();										//��������

	//make key
	unsigned int makeKey(int x, int y);							//��ˢ�ֵ��x��y����key

	void genMonsterCfg(string avatar, MonsterCfg& cfg);			//����avatar���ɹ������ýṹ��
	void genMonsterCfg(int level, string avatar, MonsterCfg& cfg);//���ݵȼ���avatar���ɹ������ýṹ��
	void updateSkillRes(MonsterShowCfg* cfg, bool isAdd = false);//���¼�����Դ

	int rollPackage(unsigned int prob);							//�������

	void updateMonsterNum(float dt);							//���µ�ǰ��̨�ϵĹ�������

	void updateScore();											//���»���

	int checkMonsterNumOnStage();								//�����̨�ϵĹ�������				
private:
	unsigned int m_uCurStageIdx;								//��ǰ��������1��ʼ(�ӵ��Ĳ߻�˵��)
	unsigned int m_uCurStageMonNum;								//��ǰ�׶ι�������
	unsigned int m_uCurLimitTime;								//��ǰ�׶�����ʱ��
	unsigned int m_uCurTotalNum;								//��ǰ�׶ιֵ�����
	unsigned int m_uCurLiveMonNum;								//��ǰ��̨�ϵĹ�������
	unsigned int m_uHasBrushNum;								//��ǰ�׶��Ѿ�ˢ���Ĺ�������
	unsigned int m_uCurMonsterLev;								//��ǰ����ȼ�
	int m_uCurStartTime;										//��ǰ�׶ο�ʼʱ��
	bool m_bHasTips;											//��ǰ�׶��Ƿ���ʾ��

	float m_fScoreTime;											//���ְ���ʱ����£����60s����һ��


	vector<CCPoint> m_stBrushPoint;								//ˢ�ֵ�λ��
	map<unsigned int, int> m_stMonKeyToTime;					//ÿ��ˢ�ֵ��Ӧ����ʼˢ��ʱ��(key��id��x��y�ϳɣ�valueΪʱ��)

	bool m_bStartBrush;											//��ʼˢ��
	float m_fStartBrushTime;									//��ʼˢ��ʱ��
	bool m_bIsGameOver;											//�Ƿ����

	CFightEndlessScoreUI* m_pScoreUI;
	CFightEndlessWaveUI* m_pWaveUI;	
	CPauseLayer* m_pPauseLayer;

	pair<string, string> m_stCurAvatar;							//��ǰ�׶ε�2��avatar
	pair<string, string> m_stNextAvatar;						//��һ�׶ε�2��avatar

	//ͳ��
	map<int, int> m_stTypeToMonNum;								//�������Ͷ�Ӧɱ�ֵ�����
	unsigned int m_uPackageCnt;									//��������
	unsigned int m_uCoin;										//������������
	unsigned int m_uCurScore;									//��ǰ����
	CVArray<int, 4> m_stMonsterCnt;								//����ɱ��������4������

	unsigned int m_uNum;

	bool m_bIsInBrush;											//�Ƿ�����ˢ����
	unsigned int m_uBrushNum;									//�˴�ˢ�ֵ�����
	unsigned int m_uNowHasBrushNum;								//�˴��Ѿ�ˢ�ֵ�����
	int m_iFrameCnt;											//֡��
	int m_iCheckInterval;										//������֡ˢһֻ��

	MapCfg m_stMapCfg;											//����ĵ�ͼ����
};

#endif // FightEndlessScene_h__
