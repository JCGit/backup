#ifndef BaseMapScene_h__
#define BaseMapScene_h__

#include "cocos2d.h"
#include "BaseScene.h"
#include "fight/BgTextureNode.h"
#include "SceneItem.h"
//#include "AStar.h"

#include "QPathFinder.h"

USING_NS_CC;

using namespace std;

#define FIGHT_STAGE_NUM 1.5f

//static const double FIGHT_STAGE_NUM = 1.5f;

const int MAP_RES_NUM = 3;			//��ͼ��Դ��Ϊ3

enum { //�������Դ��ʽ��ͼƬ or ����
	SCENE_ITEM_PICTURE = 1,	
	SCENE_ITEM_ARMATURE
};

enum RESTYPE{
	ROLE_TYPE = 1,				//��ɫ��Դ����
	MONSTER_TYPE = 2,			//������Դ
	ITEM_TYPE = 3,				//item��Դ����
	SKILL_TYPE = 4				//������Դ����
};

//��ʼ�߽ṹ��
struct BeginData{
	int stage;					//�׶�
	int x;
	int condition;				//ʤ������
	int targetType;				//Ŀ������
	int targetID;				//Ŀ��ID
	int num;					//��������
	int time;					//ʱ��
};

class CBaseMapScene : public BaseScene
{
public:
	CBaseMapScene();
	virtual ~CBaseMapScene();

	// override start

	virtual bool init();

	virtual void onEnterTransitionDidFinish();

	virtual void loadRes();
    virtual bool loadNextRes();

	virtual void loadMapRes();						//��ȡ��ͼ��Դ��Ϣ

	//need override
	virtual void loadMapBgRes();					//��ͼ������Դ�ļ��أ���̬
	virtual void loadHeroRes();						//��ɫ��Դ�ļ��أ���̬��
	virtual void loadMonsterRes();					//������Դ�ļ��أ���̬
	virtual void loadItemRes();						//����item��Դ�ļ��أ���̬
	virtual void loadFloorBuffRes();				//����buff��Դ�ļ��أ���̬
	virtual void loadTrapRes();						//����������Դ�ļ��أ���̬

	virtual void build();

	// override end

	virtual void update(float delta);				//ͬ����Դ���ͷ�����

	virtual void initMap();					//��ʼ����ͼ��
	virtual void buildMap();				//������ͼԪ��

	void updateMapBlocks();					//���µ�ͼ�ϵ��赲��Ϣ

	bool getIsCached() const;
	virtual void cleanRes();				//�����Դ
	void moveEntity(CQStageEntity* pEntity, const CCPoint& stDestPoint);

protected:
	virtual void onEachMapResLoadedCallback(CCObject *pObject);	//��ɵ��ε�ͼ���ػص�����
	virtual void onEachArmatureLoaderCallback(float dt);		//��ɵ��ζ������ػص�����
	virtual void onLoaderCallback();							//loading�ص�
	virtual void analyseMapInfo();								//������ͼ��Ϣ
	virtual void setWalkHight();								//���ÿ���������

	virtual void setMapRes();									//ͨ����ͼ��������ÿ�����Դ��
	virtual void initSound();									//���汳��������Դ

protected:
	bool m_bIsCached;					// �Ƿ��ѻ���
	unsigned int m_uResTotalNum;		//��ͼ��ԴͼƬ����
	unsigned int m_uResLoadedNum;		//�Ѽ��ص�ͼƬ��
	unsigned int _gridWNum;				//�����������
	unsigned int _gridHNum;				//�����������
	unsigned int m_uWalkHight;			//�����߸߶�

	string m_stFrontStr;				//ǰ����Դ��
	string m_stForeStr;					//�ر���Դ��
	string m_stMidStr;					//�о�1��Դ��
	string m_stBackStr;					//������Դ��
	string m_stMid2Str;					//�о���2��Դ��

	vector<CCRect> m_stRects;			//��¼�赲�ľ���

public:
	map<int, BeginData> m_stBeginData;		//�׶�id ��Ӧ ��ʼ������
	map<int, int> m_stTouchData;			//touch id��Ӧx
	map<int, CCPoint> m_stNPCData;			//npc id��Ӧ(x,y)

private:
	CC_SYNTHESIZE(string, _cfgFileName, CfgFileName);					//��ͼ�����ļ���
	CC_SYNTHESIZE(string, _fileName, FileName);

	CC_SYNTHESIZE(int, _mapW, MapW);			//��ͼ���
	CC_SYNTHESIZE(int, _mapH, MapH);			//��ͼ�߶�
	CC_SYNTHESIZE(int, _screenW, ScreenW);		//����
	CC_SYNTHESIZE(int, _screenNum, ScreenNum);	//����
	CC_SYNTHESIZE(int, _gridW, GridW);			//���ӿ��
	CC_SYNTHESIZE(int, _gridH, GridH);			//���Ӹ߶�
	CC_SYNTHESIZE(int, _midResNum, MidResNum);	//�м����Դ����

	CC_SYNTHESIZE(CCArray*, _monsterData, MonsterData);	//������Ϣ
	CC_SYNTHESIZE(CCArray*, _itemData, ItemData);		//��ͼ����Ʒ��Ϣ���ӵ�ͼ�ĵ�����
	CC_SYNTHESIZE(CCArray*, _floorBuffData, FloorBuffData);	//��ͼ�ϵ�buff��Ϣ
	CC_SYNTHESIZE(CCArray*, _trapData, TrapData);		//��ͼ�ϵ�������Ϣ

	CC_SYNTHESIZE(CBgTextureNode*, _sceneBackground, SceneBackground);		//Զ����
	CC_SYNTHESIZE(CBgTextureNode*, _sceneForegound, SceneForeground);		//�ر��
	CC_SYNTHESIZE(CBgTextureNode*, _sceneFrontgound, SceneFrontgound);		//������	
	CC_SYNTHESIZE(CBgTextureNode*, _sceneMidground, SceneMidground);		//�о���1, �����ŵر�
	CC_SYNTHESIZE(CBgTextureNode*, _sceneMid2ground, SceneMid2ground);		//�о���2, �о���1,Զ����ǰһ��


	CC_SYNTHESIZE(string, _bgSoundName, BGSoundName);						//������������

    set<int> _blocks; //��¼�赲�ĸ�������

    set<string> m_stPrepareLoadRes; //�ȴ��������Դ

	
public:
	CCPoint xyToGrid(const CCPoint& pst);					//����ת���ɸ�������	
	CCPoint gridToXY(const CCPoint& pst);					//��������ת��������

	int gridToIdx(CCPoint& pst);							//��������ת��Ϊidx
	void getBlocksInSize(const CCSize& size, const CCPoint& pst, bool bIsItem = false);	//����Ŀ���Ϊ���ĵ�size��С�ĸ�������Ϊ��������
	void removeBlocksInSize(const CCSize& size, const CCPoint& pst, bool bIsItem = false);  //����Ŀ���Ϊ���ĵ�size��С�ĸ�������Ϊ������
	void removeAllBlocks();

	CCRect getPointInRect(const CCPoint& pst);				//�õ������ڵ��赲����
    bool isPointCanWalk(const CCPoint& pst);				//�жϵ��Ƿ��������(��ͼ����) 

    //todo:
    //CAStar m_stAStar;
    QPathFinder m_stPathFinder;

	unsigned int getWalkHight() const{
		return m_uWalkHight;
	}

protected:
	map<string, int> m_stResToNum;							//��¼ʹ��ĳ��Դ�Ĵ�����<string, num>��Դ����Ӧ����(���item��monster������)

	CCRect walkableRect;									//����������
protected:
	virtual void setViewpointCenter(const CCPoint& position);		//�ӽǾ���
	virtual void updateBgViewCenter(float foreX);					//���µ�ͼǰ����Զ�����ӽ�(���ݵ�ͼ��ͬ����д)
	virtual void updateBgTextureByRect(CBgTextureNode* pstBgTexture, bool bIsTopAlign = false);  //���ݿ�����������µ�ͼ��������Դ

	void updateResToNum(int id, RESTYPE type, bool isAdd = false, int num = 1);	
	void updateResToNumByStr(string str, bool isAdd, int num);
private:

    float m_iCheckCleanInterval;
};

#endif // BaseMapScene_h__
