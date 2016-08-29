#pragma once

#include "stdafx.h"
#include "cocos2d.h"
#include "cocos-ext.h"
#include "StateMachine.h"
#include "MainFrame.h"
#include "MessageManager.h"
#include "GameMessages.h"
//#include "PackageLogic.h"
#include "CCBManager.h"
#include "CustomPage.h"
#include "StringConverter.h"
#include "BlackBoard.h"

USING_NS_CC;
USING_NS_CC_EXT;

////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
#define qer_new					new
#define qer_delete(p)			{if ((p) != 0) {delete (p); (p) = 0;}}
#define qer_new_array			new
#define qer_delete_array(p)		{if ((p) != 0) {delete [] (p); (p) = 0;}}
#define qer_assert(x)			do{}while(0)//CCAssert((x), "")

static const unsigned int		k_art_anim_fps				= 30;
static const float				k_art_anim_frame_ms			= 1000.f/k_art_anim_fps;


#define BASE_RATE_VAL 10000.0f

enum AttrType
{
	AttrType_Physique = 10,				//����
	AttrType_Stength = 11,				//����
	AttrType_Armor = 12,				//����
	AttrType_HP = 13,					//����ֵ
	AttrType_Attack = 14,				//����
	AttrType_PhysicsDefence = 15,		//���
	AttrType_MagicDefence = 16,			//����
	AttrType_HPRate = 17,				//����ֵ�ٷֱ� 
	AttrType_AttackRate = 18,			//�����ٷֱ�
	AttrType_PhysicsDefenceRate = 19,	//����ٷֱ�
	AttrType_MagicDefenceRate = 20,		//�����ٷֱ�
	AttrType_HitRate = 21,				//������
	AttrType_DodgeRate = 22,			//������
	AttrType_ViolentRate = 23,			//������
	AttrType_ViolentAddRate = 24,		//�����ӳ���
	AttrType_ResistanceViolentRate = 25,//������
	AttrType_ParryRate = 26,			//���� 
	AttrType_WrecKRate = 27,			//�ƻ���
	AttrType_ExtraHurt = 28,			//�����˺�
	AttrType_HurtExempt = 29,			//�˺�����
	AttrType_ExtraHurtRate = 30,		//�����˺��ٷֱ�
};
#define QUALITY_COLOR_A (ccc3(255,0,255))
#define QUALITY_COLOR_B (ccc3(58,188,255))
#define QUALITY_COLOR_C (ccc3(0,255,0))
#define QUALITY_COLOR_D (ccc3(235,235,235))
#define QUALITY_COLOR_S (ccc3(205,127,50))

//���������������ͣ�1��������2��������3������
#define REFINE_TYPE_KEY "refine_type_key"
//����������������id
#define REFINE_ITEM_ID	"refine_item_id"
#define REFINE_CHOOSE_TYPE "refine_choose_type"
#define REFINE_SUCCESS_FLAG "refine_success_flag"

//Ĭ�ϵ�timeline
#define DEFAULT_TIMELINE "Default Timeline"

//������ɫ
#define FONT_COLOR_RED	(ccc3(255,0,0))
#define FONT_COLOR_GREEN (ccc3(0,255,0))


//����ǿ�����а�key
#define TREASURE_ENHANCE_UPLEVEL "TreasureEnhanceUpLevel"
#define TREASURE_ENHANCE_UPEXP	"TreasureEnhanceUpExp"

//�����ɹ����
#define DESTINY_SUCCESS_FLAG "DestinySuccessFlag"

enum BlurType
{
	BlurType_None = 0,				//û��ģ��
	BlurType_PushSingle = 1,		//����pushҳ��
	BlueType_PushMulti = 2			//���pushҳ�棬��Ծ��ŵ�
};