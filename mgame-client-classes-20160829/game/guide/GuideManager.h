/**
* @file     GuideManager.h
* @brief    ��������������
* @details  
* @author   Alvin
* @date     2015-7-28
*/
#ifndef __GUIDEMANAGER_H__
#define __GUIDEMANAGER_H__
#include "cocos2d.h" 
#include "cocos-ext.h" 
#include "battle/ArrowWidget.h"
#include "cocostudio/CCArmature.h"
#include "BaseGuideLayer.h" 
#include "Message.h"
#include "GuideProxy.h"
USING_NS_CC;
USING_NS_CC_EXT;
using namespace cocostudio;

 
typedef void (CCObject::*SEL_CallFuncByBool)(bool isOk);
#define callfuncByBool_selector(_SELECTOR) (SEL_CallFuncByBool)(&_SELECTOR)
 
enum GuideEspecialID
{
	GuideID_PveProp = 1000,			//PVE��������ID 
	GuideID_Morale = 900,			//ʿ������ID
	GuideID_PveSoldierBar = 830,		//����������ID	
	GuideID_RookieSoldierBar = 330, //ѵ��Ӫ����������
	GuideID_Glyph = 68,				//��������ID  
	GuideID_Institute = 1300,			//�鿨����

};

enum GuideEspecialGroup
{
	GuideGroup_Glyph = 20,//����������
	GuideGroup_SkipRookieCamp = 33,  //����ѵ��Ӫ�Ƿ����������� 
	GuideGroup_SelectRookieCamp = 34,	//ѡ������ѵ��Ӫ
	GuideGroup_RookieSoldierNum = 50, //ѵ��Ӫ����������������
	GuideGroup_PveSoldierNum = 90, //PVE������������
	GuideGroup_RookieProp = 70,	 //ѵ��Ӫ����������  
	GuideGroup_PveProp = 110,      //PVE���������� 
	GuideGroup_RookieStable = 120,  //ѵ��Ӫ�������
	GuideGroup_PveStable = 130,  //PVE������� 
	
};



class GuideAchiveCondition
{
public: 
	GuideAchiveCondition();
	~GuideAchiveCondition();
	void setSceneNode(Node* pNode);
	Node* getSceneNode(){ return m_SceneNode; }
	void setParentNode(Node* pNode);
	Node* getParentNode(){ return m_ParentNode; }
	void releaseNode();
private:
	CC_SYNTHESIZE(int, m_GateID, GateID);					//�����ؿ�
	CC_SYNTHESIZE(int, m_LevelID, LevelID);					//�����ȼ�
	CC_SYNTHESIZE(int, m_GuideLocation, GuideLocation);		//�������ڽ��� 
	CC_SYNTHESIZE(int, m_GuideID, GuideID);				    //�����������
	CC_SYNTHESIZE(bool, m_HighestLayer, HighestLayer);		//�Ƿ�ǰ������߲�
	CC_SYNTHESIZE(Point, m_GuideLayerPos, GuideLayerPos);
	Node* m_ParentNode;										//�����㸸�ڵ�
	Node* m_SceneNode;										//�����ڵ�
};



class GuideManager: public MessageHandler
{
public:
	GuideManager();
	~GuideManager();
	
	bool registGuideMessage(
		int gateID, int levelID, int guideLocation, 
		Node* parentNode, Node* sceneNode, 
		bool highestLayer = true, Point guideLayerPos = ccp(0,0)); 
	bool unRegistGuideMessage(bool force = false, int operatorType = 0); 

	const GuideData* getGuideDataByGuideID(int guideID);
	const GuideData* getGuideDataByGroup(int groupID);

	void saveGuideData(int id);
	
	void rollbackGuide(bool gateGuide = true);

	static void cleanGuideManager();

	void clearGuideData();

	void onReceiveMassage(GameMessage * message);

	bool init();
	
	void update(float delta); 
	
	bool getIsRunning() { return m_IsRunning;}

	
	bool checkGuideFinishByID(int id);		 //��ѯ���ID�Ƿ����
	bool checkGuideFinishByGroup(int group); //��ѯ������Ƿ����

	bool checkOpeartor(int operatorType);

	bool checkDragOperator();

	Layer* getGuideLayer();
	bool getGuideLock();
	void setGuideLock(bool lock);
	 

	bool checkNeedPower(int gateID); //��ѯ�Ƿ���Ҫ��������  

	bool checkRookieFlag(); //��ȡ���ֱ�ʶ

	bool checkGuideTypeFinished(int type); //��ѯ�����������Ƿ����

	void setCurGuideGroupFinish();
	void setGroupFinishByID(int group, bool serverSave = true); 

	const GuideData* getCurGuideData();

	void rollbackRookieGuide(); //�ع�����ѵ��Ӫ����״̬

	bool checkIsReady();

	void resetGuideDragFlag(int gateID);

	bool checkOpenSoldierControl(int gateID); //�Ƿ���������

	bool checkOpenPropControl(int gateID); //�Ƿ���������

	bool checkHaveGateGuide(int gateID); //�ж��Ƿ����������ȥ�����������

	bool getStopDragFlag();
	void setStopDragFlag(bool flag);
private:
	//ִ������--����Ϣ����
	bool startGuide(GuideAchiveCondition* conditionNode);

	GuideData* getGuideActiveID(GuideAchiveCondition* conditionNode);

	//ִ������--���ڲ���������
	void doGuide(GuideAchiveCondition* conditionNode);

	//����������ʾ����
	BaseGuideLayer* initGuideLayer(GuideData* tData, GuideAchiveCondition* conditionNode);

	//��鴥������
	bool checkGuideCondition(GuideData* tData, GuideAchiveCondition* conditionNode); 
	//������С�ķ���������������Ϣ
	GuideData* searchGuideData(GuideAchiveCondition* condition);
	
	//��ת��һ������
	void gotoNextGuide(GuideAchiveCondition* conditionNode);
	//�������
	bool setCurGuideFinish(bool force = false, int operatorType = 0); 
	 
	void resetGroupState(int group, bool state);

	void addGuideTimer(float time);
	bool checkGuideActive();
	void removeGuideTimer();

	void sendGuideMessage(int gateID, int locationID, int state); 
	
private:
	BaseGuideLayer* mGuideLayer;

	CC_SYNTHESIZE(int, m_CurGuideGroup, CurGuideGroup);
	CC_SYNTHESIZE(int, m_CurGuideID, CurGuideID);
	CC_SYNTHESIZE(bool, m_IsRookie, IsRookie);
	CC_SYNTHESIZE(int, m_ExtraCondition, ExtraCondition);
	CC_SYNTHESIZE(int, m_ExtraCondition2, ExtraCondition2);
	CC_SYNTHESIZE(int, m_DragCondition, DragCondition);
	CC_SYNTHESIZE(bool, m_OpenRookieCamp, OpenRookieCamp);
	 
	CC_SYNTHESIZE(bool, m_CloseGuide, CloseGuide);
	bool m_GuideLock;

	bool m_StopDragFlag;

	std::vector<GuideData* > m_TotalGuideDataVec;  

	void initGuideData(); 
	bool m_IsRunning;

	
	std::queue<GuideAchiveCondition* > m_GuideAchiveCoditionQueue;

	GuideProxy m_GuideProxy;

	std::map<int, bool> m_GuideGroupStateMap; 
 };


 #define sgGuideManager() Singleton<GuideManager>::Get()

 
#endif