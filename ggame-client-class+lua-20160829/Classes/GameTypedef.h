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
	AttrType_Physique = 10,				//体质
	AttrType_Stength = 11,				//武力
	AttrType_Armor = 12,				//护甲
	AttrType_HP = 13,					//生命值
	AttrType_Attack = 14,				//攻击
	AttrType_PhysicsDefence = 15,		//物防
	AttrType_MagicDefence = 16,			//法防
	AttrType_HPRate = 17,				//生命值百分比 
	AttrType_AttackRate = 18,			//攻击百分比
	AttrType_PhysicsDefenceRate = 19,	//物防百分比
	AttrType_MagicDefenceRate = 20,		//法防百分比
	AttrType_HitRate = 21,				//命中率
	AttrType_DodgeRate = 22,			//闪避率
	AttrType_ViolentRate = 23,			//暴击率
	AttrType_ViolentAddRate = 24,		//暴击加成率
	AttrType_ResistanceViolentRate = 25,//抗暴率
	AttrType_ParryRate = 26,			//格挡率 
	AttrType_WrecKRate = 27,			//破击率
	AttrType_ExtraHurt = 28,			//额外伤害
	AttrType_HurtExempt = 29,			//伤害减免
	AttrType_ExtraHurtRate = 30,		//额外伤害百分比
};
#define QUALITY_COLOR_A (ccc3(255,0,255))
#define QUALITY_COLOR_B (ccc3(58,188,255))
#define QUALITY_COLOR_C (ccc3(0,255,0))
#define QUALITY_COLOR_D (ccc3(235,235,235))
#define QUALITY_COLOR_S (ccc3(205,127,50))

//炼化（重生）类型；1：炼化；2：重生；3：升星
#define REFINE_TYPE_KEY "refine_type_key"
//炼化（重生）道具id
#define REFINE_ITEM_ID	"refine_item_id"
#define REFINE_CHOOSE_TYPE "refine_choose_type"
#define REFINE_SUCCESS_FLAG "refine_success_flag"

//默认的timeline
#define DEFAULT_TIMELINE "Default Timeline"

//字体颜色
#define FONT_COLOR_RED	(ccc3(255,0,0))
#define FONT_COLOR_GREEN (ccc3(0,255,0))


//宝物强化剪切板key
#define TREASURE_ENHANCE_UPLEVEL "TreasureEnhanceUpLevel"
#define TREASURE_ENHANCE_UPEXP	"TreasureEnhanceUpExp"

//天命成功标记
#define DESTINY_SUCCESS_FLAG "DestinySuccessFlag"

enum BlurType
{
	BlurType_None = 0,				//没有模糊
	BlurType_PushSingle = 1,		//单个push页面
	BlueType_PushMulti = 2			//多个push页面，针对军团等
};