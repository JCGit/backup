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

	//update相关
	void brushMonsterLogic(float dt);							//刷怪逻辑
	void handlePassLogic();										//过关逻辑

	//视角update
	void updateBgViewCenter(float foreX);						//更新地图前景、远景的视角(根据地图不同，改写)

	void updateInstnaceByAI();

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
	unsigned int makeKey(int x, int y);							//由刷怪点的x，y构成key

	void startFight(const string& stEvent, CCObject* pstEventObj);//开始

	void genMonsterCfg(int level, string avatar, MonsterCfg& cfg);

private:
	MonsterCfg m_stMonsterCfg;

	map<unsigned int, int> m_stMonKeyToTime;					//每个刷怪点对应的起始刷怪时间(key有x，y合成，value为时间)
	map<unsigned int, bool> m_stMonKeyToBool;					//每个刷怪点对应是否刷怪(key有x，y合成)

private:
	CPauseLayer* m_pPauseLayer;
};

#endif // GuildBalkCleanScene_h__
