#ifndef CityScene_h__
#define CityScene_h__

#include "basic/BaseOSDScene.h"
#include "SceneEffect.h"
#include "plot/PlotControl.h"
#include "role/NPCRole.h"

const int EFFECT_INTER = 20;		//������Ч���ܴ����ļ��ʱ��

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

	virtual void updateRoleToScene(float dt);		//ÿ��һ��ʱ���򳡾����ͬ������
	virtual void addRoleToSceneInter();		//�򳡾����ͬ������

	virtual void setWalkHight();

	// CBaseMapScene
	virtual void loadHeroRes();
	virtual void initMap();
	virtual void buildMap();
	virtual void updateBgViewCenter(float foreX);
	virtual void loadItemRes();

	//������Ч����
	void loadEffRes();			//���س�����Ч��Դ
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

	void addInstanceTransGate(const CCPoint& stPos);        //��ָ����λ�����һ���������͵�

	void checkArrivedEdge(); // �Ƿ񵽴��Ե

	void prevCity(); // ��һ��ͬ����

	void nextCity(); // ��һ��ͬ����
	
    void setActEntryLayoutStat(const bool bIsShow); //��ʾ�ճ�����

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

	void checkTransitGate();    //����Ƿ���Ҫ�򿪸��������
private:
	CMainHero *m_pPlayer; // ����

	CCDictionary *m_pNpcsDict; // �����е�NPC�б� <NPCID, CMainHero>

	CCLayer *m_pGameLayer;

	UIButton *m_pAnyEntryBtn; // �������

	CCPoint m_stLastDirection;

	CCPoint m_stTransitPos;

	//CSceneEffect* m_pEffect;		//������Ч
	unsigned int m_uEffStartTime;			//��ǰ������Ч��ʼ��ʱ��

	//�����еĸ������͵�
	CCArmature* m_pstInstanceTransGate;

	bool m_bIsArrivedEdge; // �Ƿ񵽴��Ե

	bool m_bIsEnterBattlePortal;

	bool m_bIsHasSceneEff;			//�Ƿ��г�����Ч

	float m_fMidPos;
};

#define WALL_WIDTH 4.0f // ������ײ����ǽ��

#endif // CityScene_h__
