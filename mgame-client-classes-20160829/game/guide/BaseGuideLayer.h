/**
* @file     BaseGuideLayer.h
* @brief    �������������
* @details  
* @author   Alvin
* @date     2015-7-28
*/
#ifndef __GUIDELAYER_H__
#define __GUIDELAYER_H__
#include "cocos2d.h"
#include "cocos-ext.h"
#include "cocostudio/CCArmature.h" 
#include "battle/ArrowWidget.h"
#include "data/TableManager.h"
#include "data/Tables.h"
#include "utils/Comm.h" 
#include "MessageManager.h"
#include "core/ExRichText.h"
USING_NS_CC;
USING_NS_CC_EXT;
using namespace cocostudio;
using namespace ui;
enum E_GUIDE_TYPE
{
	E_GUIDE_NONE = 0,
	E_GUIDE_CLICK = 1,		//���
	E_GUIDE_DRAG = 2,		//��ҷ
	E_GUIDE_FOCUS = 3,		//�۽�
	E_GUIDE_DIALOG = 4,		//˵����
	E_GUIDE_NOMAL =  5,		//���Ի��޶Խ�
	E_GUIDE_ANIMATION = 6,  //���⶯��
	E_GUIDE_STORY = 7,      //����Ի���
};

enum E_GuideActionState
{
	E_GuideActionState0 = 0,
	E_GuideActionState1,
	E_GuideActionState2 ,
	E_GuideActionState3 ,
	E_GuideActionState4
};

enum E_GuideState
{
	E_GuideStart = 0,
	E_GuideDoing = 1,
	E_GuideEnd = 2,
	E_GuideFinal = 3,
	E_GuideWait = 4,
};
class BaseGuideLayer: public Layer
{
public:
	BaseGuideLayer();
	~BaseGuideLayer();
	virtual bool init();
	virtual void onEnter();
	virtual void onExit();	
	virtual void update(float delta);
	virtual void onDoGuide(float dt){};
	virtual void onReadyGuide();
	virtual void onGuideCallback(bool isFinish);								//�ص�
	virtual void addGuideData(GuideData* tData);								//���ϵ����������
	virtual bool onTouchBegan(Touch *touch, Event *unused_event);
	virtual void initAnimation(int type){};
	virtual void initDialogUI(int type, float angle, Point pos);
	virtual void initGuideInfo(int guideID, int isPause, Node* sceneNode, Node* parentNode, Point pos);
	CREATE_FUNC(BaseGuideLayer);
protected:
	virtual void initUI();
	void addListener();
	virtual void animationCallback(Armature * pAr, MovementEventType type, const std::string& aniName){};  
	virtual void adjustNpcUI(GuideData* tData);										//��������Ի���λ��
	virtual void doNextState(E_GuideState state);
	virtual void doExtraAction();													//ִ���Զ��巽��
	virtual bool checkTouchSwallow(Point touchPos){ return true;};					//����������
	virtual void onGuideOver();
	virtual void initMask();

protected: 	
	Node* m_TextNode;						//���ֽڵ�
	ExRichText* m_CurrGuideLabel;			//����������
	ui::Text* m_GuideTipsLable;				//��ʾ����
	Sprite* m_LabelBg;						//������
	Sprite* m_NpcBg;						//����
	ArrowWidget* m_Painting;				//��ͷ
	bool m_AnimationOver;					//�����Ƿ����
	Armature* m_GuideAnimation;				//ָ����������
	EventListenerTouchOneByOne* m_TouchListener;	
	
	
	float m_WaitTime;						//ѭ���ȴ�ʱ��	
	int m_AnimationState;					//����ִ�в��衢״̬ 
	E_GuideState m_GuideState;

	
	Node* m_UINode;	
	Node* m_GuideNode;											//�����
	Node* m_DragNode;											//��ҷ��
	Point m_BaseLabelPos;
	Point m_BaseNpcBgPos;
	Point m_BaseLabelBgPos; 

	int  m_CurGuideDataIndex;  //��ǰ�����ڵ�����
	std::vector<GuideData* > m_GuideDataVec;

	//�����������
	CC_SYNTHESIZE(int, m_GuideType, GuideType);
	CC_SYNTHESIZE(int, m_GuideID, GuideID);	
	CC_SYNTHESIZE(Node*, m_SceneNode, SceneNode);
	CC_SYNTHESIZE(Node*, m_GuideParentNode, GuideParentNode);	 
	CC_SYNTHESIZE(bool, m_IsEnablePause, IsEnablePause);				//ս���Ƿ���ͣ
	CC_SYNTHESIZE(bool, m_IsAdjust, IsAdjust);							//�Ƿ����λ��
	CC_SYNTHESIZE(int, m_GuideAnimationType, GuideAnimationType);		//������������
	CC_SYNTHESIZE(Point, m_GuideAnimationPos, GuideAnimationPos);		//��ͷλ��
	CC_SYNTHESIZE(float, m_GuideAnimationAngle, GuideAnimationAngle);	//��ͷ�Ƕ� 
	CC_SYNTHESIZE(bool, m_IsReady, IsReady);
}; 
#endif