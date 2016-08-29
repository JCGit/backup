#ifndef EndlessFightScene_h__
#define EndlessFightScene_h__

#include "fight/BaseFightScene.h"
#include "vlib/CVArray.h"
#include "fight/PauseLayer.h"
#include "FightEndlessScoreUI.h"
#include "FightEndlessWaveUI.h"

#include "MapCfgMaster.h"

const int m_stMonsterMark[4] = {20, 30, 50, 80};   //怪物对应的分数

class CEndlessFightScene : public CBaseFightScene
{
public:
	CEndlessFightScene();
	virtual ~CEndlessFightScene();

	CREATE_FUNC(CEndlessFightScene);

	virtual bool init();

	virtual void onEnterTransitionDidFinish();

	void update(float delta);

	void loadMapBgRes();										//地图背景资源的加载，动态
	void loadHeroRes();											//角色资源的加载，动态的
	void loadMonsterRes();										//怪物资源的加载，动态
	void loadItemRes();											//场景item资源的加载，动态

	void initMap();

protected:
	//init相关
	void initSceneItem();										//关卡地图item
	void initMapUI();											//关卡UI
	void initMainHero();										//猪脚
	void initMonster();											//初始化当前阶段的怪物，预加载下一阶段的怪物资源
	void initFightData();										//初始化战斗相关的数据

	void setMapRes();											//通过地图类型设置每层的资源名
	void setWalkHight();										//设置可行走区域

	//update相关
	//void brushMonsterLogic(float dt);							//刷怪逻辑
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

private:
	void startFightCountDown();									//开始战斗倒计时		
	void startEndless(const string& stEvent, CCObject* pstEventObj);//无尽模式开始

	void prepareMonsterResByStatge(unsigned int stageIdx);		//准备阶段中怪物的资源

	void prepareInitMonster(float dt);							//在每波之间，间隔一段时间构建一定数量的怪物，但不加入舞台

	void prepareInitOneMonster(unsigned int stageIdx);

	void genMonsterCfg(string avatar, MonsterCfg& cfg, int level = 1);			//根据avatar生成怪物配置结构体

	void initMonsterLogic(bool isAddToStage = true);

	void initOneMosterByAvatar(string avatar, CCPoint stPos, int level = 1, bool isAddToStage = true);	//初始化怪物，先暂时停止AI

	void activeMonsterByTime(float dt);							//激活怪物，激活AI

	void activeMonster();										//激活

	void updateScore();											//更新积分

	void nextStageTip();										//下一阶段提示

	void nextStage();											//下一阶段

	void handleGameOver();										//结束结算

	int rollPackage(unsigned int prob);							//随机掉落
private:
	CFightEndlessScoreUI* m_pScoreUI;
	CFightEndlessWaveUI* m_pWaveUI;	
	CPauseLayer* m_pPauseLayer;

private:
	MapCfg m_stMapCfg;											//随机的地图配置
	CCArray* m_stNextMonsters;									//下一阶段的怪物，暂时未加到舞台上

	float m_fLastTime;											//无尽模式开始到结束的时间
	float m_fStageStartTime;									//当前阶段开始时间

	unsigned int m_uCurStageIdx;								//当前轮数，从1开始(坑爹的策划说的)
	unsigned int m_uCurStageMonNum;								//当前舞台上的怪物数量
	unsigned int m_uCurMonsterLev;								//当前怪物等级
	unsigned int m_uCurLimitTime;								//当前阶段限制时间
	unsigned int m_uCurTotalNum;								//当前阶段怪的总数

	unsigned int m_uCurLeftNum;									//当前左边怪物数量
	unsigned int m_uCurRightNum;								//当前右边怪物数量

	//提前构建怪物的参数
	unsigned int m_uPreInitInter;								//提前构建怪物的间隔
	unsigned int m_uPreInitNum;									//提前构建怪物的数量

	pair<string, string> m_stCurAvatar;							//当前阶段的2个avatar

	//统计
	unsigned int m_uPackageCnt;									//掉落数量
	unsigned int m_uCoin;										//获得联邦币数量
	unsigned int m_uCurScore;									//当前积分
	CVArray<int, 4> m_stMonsterCnt;								//最终杀怪数量，4种类型
};



#endif // EndlessFightScene_h__
