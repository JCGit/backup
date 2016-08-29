#pragma once
#include "cocos2d.h"


#include "Singleton.h"
#include <string>
#include "CCBContainer.h"
#include "GameMaths.h"

USING_NS_CC;
USING_NS_CC_EXT;

/*
*UI���ö���������
*@author liu longfei
*/

#define ANIM_MOVE_TIME 0.25
#define ANIM_MOVE_UNIT 0.1
#define ANIM_DELAY_TIME 0.03
#define ANIM_SCALE_TIME 0.3

#define SCROLL_MOVE_IN_TAG 100232

class AnimMgr
	:public Singleton<AnimMgr>
{
public:
	AnimMgr();
	~AnimMgr();

	//��˸Ч�����������֣�������ʧ 
	void fadeInAndOut(CCNode* node,float time);	

	//scrollview ��content���ο��ٳ���
	void scrollViewMoveIn(CCScrollView* scrollView);
	void nodeMoveIn(CCNode* node,bool order = false);

	//���������Ž���
	void scaleToShow(CCNode* node,float start,float end,CCCallFuncND* func);
	//������������ʧ
	void scaleToHide(CCNode* node,float start,CCCallFuncND* func);

	//for lua
	static AnimMgr* getInstance();
};


namespace InverseColorEffect
{
	CCSprite* generateInverseColorSpriteByNode(CCNode* node,CCPoint  anchorPoin);
}


