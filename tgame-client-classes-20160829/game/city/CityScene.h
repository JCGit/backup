#ifndef CityScene_h__
#define CityScene_h__

#include "basic/BaseOSDScene.h"
#include "SceneEffect.h"
#include "plot/PlotControl.h"
#include "role/NPCRole.h"

const int EFFECT_INTER = 20;		//场景特效可能触发的间隔时间

class CityScene: public BaseOSDScene, public HudLayerDelegate {
public:
	CityScene();
	virtual ~CityScene();

	static CityScene *create();

	// override start

	virtual bool init();
	virtual void build();

	virtual void onEnterTransitionDidFinish();
	virtual void onExit();
	virtual void update(float dt);

	virtual void updateRoleToScene(float dt);		//每隔一段时间向场景添加同屏人物
	virtual void addRoleToSceneInter();		//向场景添加同屏人物

	virtual void setWalkHight();

	// CBaseMapScene
	virtual void loadHeroRes();
	virtual void initMap();
	virtual void buildMap();
	virtual void updateBgViewCenter(float foreX);
	virtual void loadItemRes();

	//场景特效处理
	void loadEffRes();			//加载场景特效资源
	void updateSceneEff();

	// hud
	virtual void didChangeDirectionTo(HudLayer *hud, CCPoint direction);
	virtual void isHoldingDirection(HudLayer *hud, CCPoint direction);
	virtual void simpleDPadTouchEnded(HudLayer *hud);

	virtual void cleanRes();

	// override end

	NPCRole* getNPC(unsigned int uNPCID); // get the npc from scene

	void addNPC(unsigned int uNPCID, const CCPoint &pos); // add the npc to scene

	void removeNPC(unsigned int uNPCID); // remove the npc from scene

	void moveToNPC(unsigned int uNPCID); // move player to the npc

	void onArrivedNPC(const std::string &evt, CCObject *pSender);

	void updateNPCTaskState(unsigned int uNPCID);

	void checkRolePos(CQStageEntity *pRole);

	void addInstanceTransGate(const CCPoint& stPos);        //在指定的位置添加一个副本传送点

	void checkArrivedEdge(); // 是否到达边缘

	void prevCity(); // 上一个同屏点

	void nextCity(); // 下一个同屏点
	
    void setActEntryLayoutStat(const bool bIsShow); //显示日常活动面板

	void moveToBattlePortal();

	void moveToScene(unsigned int uSceneID); // move to edge and enter the scene

	void showPlotDialog();

	void moduleOpen();

private:
	void touchEventHandler(CCObject *pSender, TouchEventType type);

	void dialogEventHandler(CCObject *pSender, unsigned int uStoryID);

	void gameLayerHitEventHandler(CCObject *pSender, CCPoint pos);

	void handleShakeKeyFrameEvent(const string& stEvent, CCObject* pstEventObj);
	void handleDustKeyFrameEvent(const string& stEvent, CCObject* pstEventObj);
	void handleEffSoundKeyFrameEvent(const string& stEvent, CCObject* pstEventObj);

	NPCRole *createNPC(unsigned int uNPCID); // create a npc

	void checkTransitGate();    //检查是否需要打开副本传面板
private:
	CMainHero *m_pPlayer; // 主角

	CCDictionary *m_pNpcsDict; // 场景中的NPC列表 <NPCID, CMainHero>

	CCLayer *m_pGameLayer;

	UIButton *m_pAnyEntryBtn; // 功能入口

	CCPoint m_stLastDirection;

	CCPoint m_stTransitPos;

	//CSceneEffect* m_pEffect;		//场景特效
	unsigned int m_uEffStartTime;			//当前场景特效开始的时间

	//场景中的副本传送点
	CCArmature* m_pstInstanceTransGate;

	bool m_bIsArrivedEdge; // 是否到达边缘

	bool m_bIsEnterBattlePortal;

	bool m_bIsHasSceneEff;			//是否有场景特效

	float m_fMidPos;
};

#define WALL_WIDTH 4.0f // 用于碰撞检测的墙宽

#endif // CityScene_h__
