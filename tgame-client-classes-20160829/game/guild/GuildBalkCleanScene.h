#ifndef GuildBalkCleanScene_h__
#define GuildBalkCleanScene_h__

#include "fight/BaseFightScene.h"
#include "fight/PauseLayer.h"

#include "MapCfgMaster.h"

class CGuildBalkCleanScene : public CBaseFightScene
{
public:
	CGuildBalkCleanScene();
	virtual ~CGuildBalkCleanScene();

	CREATE_FUNC(CGuildBalkCleanScene);

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

	//update���
	void brushMonsterLogic(float dt);							//ˢ���߼�
	void handlePassLogic();										//�����߼�

	//�ӽ�update
	void updateBgViewCenter(float foreX);						//���µ�ͼǰ����Զ�����ӽ�(���ݵ�ͼ��ͬ����д)

	void updateInstnaceByAI();

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
	unsigned int makeKey(int x, int y);							//��ˢ�ֵ��x��y����key

	void startFight(const string& stEvent, CCObject* pstEventObj);//��ʼ

	void genMonsterCfg(int level, string avatar, MonsterCfg& cfg);

private:
	MonsterCfg m_stMonsterCfg;

	map<unsigned int, int> m_stMonKeyToTime;					//ÿ��ˢ�ֵ��Ӧ����ʼˢ��ʱ��(key��x��y�ϳɣ�valueΪʱ��)
	map<unsigned int, bool> m_stMonKeyToBool;					//ÿ��ˢ�ֵ��Ӧ�Ƿ�ˢ��(key��x��y�ϳ�)

private:
	CPauseLayer* m_pPauseLayer;
};

#endif // GuildBalkCleanScene_h__
