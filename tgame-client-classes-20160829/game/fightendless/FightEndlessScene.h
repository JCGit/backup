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
const int stMonsterMark[4] = {20, 30, 50, 80};   //怪物对应的分数

class CFightEndlessScene : public CBaseFightScene
{
public:
	CFightEndlessScene();
	virtual ~CFightEndlessScene();

	CREATE_FUNC(CFightEndlessScene);

	virtual bool init();

	virtual void onEnterTransitionDidFinish();

	void update(float delta);

	void loadMapBgRes();										//地图背景资源的加载，动态
	void loadHeroRes();											//角色资源的加载，动态的
	void loadMonsterRes();										//怪物资源的加载，动态
	void loadItemRes();											//场景item资源的加载，动态

	void initMap();

	virtual void cleanup();
protected:
	//init相关
	void initSceneItem();										//关卡地图item
	void initMapUI();											//关卡UI
	void initFriend();											//友军
	void initMonster();											//初始化当前阶段的怪物，预加载下一阶段的怪物资源
	void initFightData();										//初始化战斗相关的数据

	void setMapRes();											//通过地图类型设置每层的资源名
	void setWalkHight();										//设置可行走区域

	//update相关
	void brushMonsterLogic(float dt);							//刷怪逻辑
	void handlePassLogic();										//过关逻辑

	//视角update
	void updateBgViewCenter(float foreX);						//更新地图前景、远景的视角(根据地图不同，改写)

	//放弃战斗
	virtual void handleGiveUpLogic(CCObject* pstObj);			//中途放弃退出关卡
protected:
	//猪脚事件
	void handleHeroKilledEvent(const string& stEvent, CCObject* pstEventObj);

	//怪物事件
	void handleEnemyHarmValueEvent(const string& stEvent, CCObject* pstEventObj);
	void handleEnemyDieCompleteEvent(const string& stEvent, CCObject* pstEventObj);
	void handleHeroKillValueChange(const string& stEvent, CCObject* pstEventObj);

	//场景物件事件
	void handleSceneItemFallEvent(const string& stEvent, CCObject* pstEventObj);
private:
	void beginTip();											//开始倒计时
	void startEndless(const string& stEvent, CCObject* pstEventObj);//无尽模式开始

	void prepareNextStage();									//预先加载下一个阶段怪的资源
	void nextStageTip();										//下一阶段提示
	void nextStage();											//下一阶段

	void brushMonster();

	void handleGameOver();										//结束结算

	//make key
	unsigned int makeKey(int x, int y);							//由刷怪点的x，y构成key

	void genMonsterCfg(string avatar, MonsterCfg& cfg);			//根据avatar生成怪物配置结构体
	void genMonsterCfg(int level, string avatar, MonsterCfg& cfg);//根据等级和avatar生成怪物配置结构体
	void updateSkillRes(MonsterShowCfg* cfg, bool isAdd = false);//更新技能资源

	int rollPackage(unsigned int prob);							//随机掉落

	void updateMonsterNum(float dt);							//更新当前舞台上的怪物总数

	void updateScore();											//更新积分

	int checkMonsterNumOnStage();								//检查舞台上的怪物数量				
private:
	unsigned int m_uCurStageIdx;								//当前轮数，从1开始(坑爹的策划说的)
	unsigned int m_uCurStageMonNum;								//当前阶段怪物数量
	unsigned int m_uCurLimitTime;								//当前阶段限制时间
	unsigned int m_uCurTotalNum;								//当前阶段怪的总数
	unsigned int m_uCurLiveMonNum;								//当前舞台上的怪物数量
	unsigned int m_uHasBrushNum;								//当前阶段已经刷出的怪物数量
	unsigned int m_uCurMonsterLev;								//当前怪物等级
	int m_uCurStartTime;										//当前阶段开始时间
	bool m_bHasTips;											//当前阶段是否提示过

	float m_fScoreTime;											//积分按照时间更新，间隔60s更新一次


	vector<CCPoint> m_stBrushPoint;								//刷怪点位置
	map<unsigned int, int> m_stMonKeyToTime;					//每个刷怪点对应的起始刷怪时间(key有id，x，y合成，value为时间)

	bool m_bStartBrush;											//开始刷怪
	float m_fStartBrushTime;									//开始刷怪时间
	bool m_bIsGameOver;											//是否结束

	CFightEndlessScoreUI* m_pScoreUI;
	CFightEndlessWaveUI* m_pWaveUI;	
	CPauseLayer* m_pPauseLayer;

	pair<string, string> m_stCurAvatar;							//当前阶段的2个avatar
	pair<string, string> m_stNextAvatar;						//下一阶段的2个avatar

	//统计
	map<int, int> m_stTypeToMonNum;								//怪物类型对应杀怪的数量
	unsigned int m_uPackageCnt;									//掉落数量
	unsigned int m_uCoin;										//获得联邦币数量
	unsigned int m_uCurScore;									//当前积分
	CVArray<int, 4> m_stMonsterCnt;								//最终杀怪数量，4种类型

	unsigned int m_uNum;

	bool m_bIsInBrush;											//是否正在刷怪中
	unsigned int m_uBrushNum;									//此次刷怪的数量
	unsigned int m_uNowHasBrushNum;								//此次已经刷怪的数量
	int m_iFrameCnt;											//帧数
	int m_iCheckInterval;										//隔多少帧刷一只怪

	MapCfg m_stMapCfg;											//随机的地图配置
};

#endif // FightEndlessScene_h__
