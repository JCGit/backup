/**
* @file     ClickGuideLayer.h
* @brief    �����������������
* @details  
* @author   Alvin
* @date     2015-8-13
*/
#ifndef __CLICKGUIDELAYER_H__
#define __CLICKGUIDELAYER_H__
#include "BaseGuideLayer.h"
class ClickGuideLayer: public BaseGuideLayer
{
public:
	ClickGuideLayer();
	~ClickGuideLayer();
	void onDoGuide(float dt); 
	void onGuideCallback(bool isFinish); //�ص� 
	bool onTouchBegan(Touch *touch, Event *unused_event); 
	void initAnimation(int type);
	CREATE_FUNC(ClickGuideLayer);
protected:
	void animationCallback(Armature * pAr, MovementEventType type, const std::string& aniName); 
private:
	void initUI(); 
	void updateFogLayer(float dt); 
	bool drawClipNode(float dt, int drawType, Point pos);
	void playAnimation(GuideData* pData);
	void drawCircle(float x, float y, float r, bool atOnce = false);
	void drawRect(float x, float y, float width, float height, bool atOnce = false);
	void startGuide();
	bool checkTouchSwallow(Point touchPos);		//��ⴥ�����Ƿ��ڹ涨��Χ��
	void createClickLayer(GuideData* tData);	//����������
	void resetAnimation(GuideData* pData);		//�������ö���
	void resetClipRange(GuideData* pData);		//����ü���Χ
	void resetClipNode(GuideData* pData);		//����ü��ڵ�
	void resetRectBorder(Point pos);			//�������þ��α߿�
	void resetCircleBorder(Point pos);			//��������Բ�α߿�
	void resetNpcUI(GuideData* pData);			//���������Լ��Ի��� 

	void doStartAction(float delta);
	void doDoingAction(float delta);
	void doEndAction(float delta);
	void doFinalAction(float delta);  

protected: 	
	 
	
	//���ýڵ�
	ClippingNode* m_ClippingNode;	
	DrawNode* m_Stencil; 
	 
	float m_VisitRange;
	float m_VisitRangeEx;
	bool m_StartFog;  
	Size m_FogRange;

	extension::Scale9Sprite * m_GuideRectBorderSp;
	Size m_BaseRectSize;
	extension::Scale9Sprite* m_GuideCircleBorderSp;
	Size m_BaseCircleSize;


	bool mPlayingAni;
};  
#endif