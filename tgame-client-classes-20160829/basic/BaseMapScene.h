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

const int MAP_RES_NUM = 3;			//地图资源数为3

enum { //物件的资源格式，图片 or 动画
	SCENE_ITEM_PICTURE = 1,	
	SCENE_ITEM_ARMATURE
};

enum RESTYPE{
	ROLE_TYPE = 1,				//角色资源类型
	MONSTER_TYPE = 2,			//怪物资源
	ITEM_TYPE = 3,				//item资源类型
	SKILL_TYPE = 4				//技能资源类型
};

//起始线结构体
struct BeginData{
	int stage;					//阶段
	int x;
	int condition;				//胜利条件
	int targetType;				//目标类型
	int targetID;				//目标ID
	int num;					//消灭数量
	int time;					//时间
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

	virtual void loadMapRes();						//读取地图资源信息

	//need override
	virtual void loadMapBgRes();					//地图背景资源的加载，动态
	virtual void loadHeroRes();						//角色资源的加载，动态的
	virtual void loadMonsterRes();					//怪物资源的加载，动态
	virtual void loadItemRes();						//场景item资源的加载，动态
	virtual void loadFloorBuffRes();				//场景buff资源的加载，动态
	virtual void loadTrapRes();						//场景陷阱资源的加载，动态

	virtual void build();

	// override end

	virtual void update(float delta);				//同步资源的释放问题

	virtual void initMap();					//初始化地图中
	virtual void buildMap();				//构建地图元素

	void updateMapBlocks();					//更新地图上的阻挡信息

	bool getIsCached() const;
	virtual void cleanRes();				//清除资源
	void moveEntity(CQStageEntity* pEntity, const CCPoint& stDestPoint);

protected:
	virtual void onEachMapResLoadedCallback(CCObject *pObject);	//完成单次地图加载回调函数
	virtual void onEachArmatureLoaderCallback(float dt);		//完成单次动画加载回调函数
	virtual void onLoaderCallback();							//loading回调
	virtual void analyseMapInfo();								//解析地图信息
	virtual void setWalkHight();								//设置可行走区域

	virtual void setMapRes();									//通过地图类型设置每层的资源名
	virtual void initSound();									//缓存背景音乐资源

protected:
	bool m_bIsCached;					// 是否已缓存
	unsigned int m_uResTotalNum;		//地图资源图片总数
	unsigned int m_uResLoadedNum;		//已加载的图片数
	unsigned int _gridWNum;				//横向格子总数
	unsigned int _gridHNum;				//纵向格子总数
	unsigned int m_uWalkHight;			//可行走高度

	string m_stFrontStr;				//前景资源名
	string m_stForeStr;					//地表资源名
	string m_stMidStr;					//中景1资源名
	string m_stBackStr;					//背景资源名
	string m_stMid2Str;					//中景层2资源名

	vector<CCRect> m_stRects;			//记录阻挡的矩形

public:
	map<int, BeginData> m_stBeginData;		//阶段id 对应 起始线数据
	map<int, int> m_stTouchData;			//touch id对应x
	map<int, CCPoint> m_stNPCData;			//npc id对应(x,y)

private:
	CC_SYNTHESIZE(string, _cfgFileName, CfgFileName);					//地图配置文件名
	CC_SYNTHESIZE(string, _fileName, FileName);

	CC_SYNTHESIZE(int, _mapW, MapW);			//地图宽度
	CC_SYNTHESIZE(int, _mapH, MapH);			//地图高度
	CC_SYNTHESIZE(int, _screenW, ScreenW);		//屏宽
	CC_SYNTHESIZE(int, _screenNum, ScreenNum);	//屏数
	CC_SYNTHESIZE(int, _gridW, GridW);			//格子宽度
	CC_SYNTHESIZE(int, _gridH, GridH);			//格子高度
	CC_SYNTHESIZE(int, _midResNum, MidResNum);	//中间层资源屏数

	CC_SYNTHESIZE(CCArray*, _monsterData, MonsterData);	//怪物信息
	CC_SYNTHESIZE(CCArray*, _itemData, ItemData);		//地图上物品信息，从地图文档读出
	CC_SYNTHESIZE(CCArray*, _floorBuffData, FloorBuffData);	//地图上的buff信息
	CC_SYNTHESIZE(CCArray*, _trapData, TrapData);		//地图上的陷阱信息

	CC_SYNTHESIZE(CBgTextureNode*, _sceneBackground, SceneBackground);		//远景层
	CC_SYNTHESIZE(CBgTextureNode*, _sceneForegound, SceneForeground);		//地表层
	CC_SYNTHESIZE(CBgTextureNode*, _sceneFrontgound, SceneFrontgound);		//近景层	
	CC_SYNTHESIZE(CBgTextureNode*, _sceneMidground, SceneMidground);		//中景层1, 紧挨着地表
	CC_SYNTHESIZE(CBgTextureNode*, _sceneMid2ground, SceneMid2ground);		//中景层2, 中景层1,远景层前一层


	CC_SYNTHESIZE(string, _bgSoundName, BGSoundName);						//背景音乐名称

    set<int> _blocks; //记录阻挡的格子索引

    set<string> m_stPrepareLoadRes; //等待载入的资源

	
public:
	CCPoint xyToGrid(const CCPoint& pst);					//坐标转换成格子索引	
	CCPoint gridToXY(const CCPoint& pst);					//格子索引转换成坐标

	int gridToIdx(CCPoint& pst);							//格子索引转换为idx
	void getBlocksInSize(const CCSize& size, const CCPoint& pst, bool bIsItem = false);	//将以目标点为中心的size大小的格子设置为不可行走
	void removeBlocksInSize(const CCSize& size, const CCPoint& pst, bool bIsItem = false);  //将以目标点为中心的size大小的格子设置为可行走
	void removeAllBlocks();

	CCRect getPointInRect(const CCPoint& pst);				//得到点所在的阻挡矩形
    bool isPointCanWalk(const CCPoint& pst);				//判断点是否可以行走(地图坐标) 

    //todo:
    //CAStar m_stAStar;
    QPathFinder m_stPathFinder;

	unsigned int getWalkHight() const{
		return m_uWalkHight;
	}

protected:
	map<string, int> m_stResToNum;							//记录使用某资源的次数，<string, num>资源名对应数量(针对item、monster、技能)

	CCRect walkableRect;									//可行走区域
protected:
	virtual void setViewpointCenter(const CCPoint& position);		//视角居中
	virtual void updateBgViewCenter(float foreX);					//更新地图前景、远景的视角(根据地图不同，改写)
	virtual void updateBgTextureByRect(CBgTextureNode* pstBgTexture, bool bIsTopAlign = false);  //根据可行走区域更新地图背景的资源

	void updateResToNum(int id, RESTYPE type, bool isAdd = false, int num = 1);	
	void updateResToNumByStr(string str, bool isAdd, int num);
private:

    float m_iCheckCleanInterval;
};

#endif // BaseMapScene_h__
