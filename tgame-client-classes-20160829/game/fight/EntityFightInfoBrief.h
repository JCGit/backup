#ifndef _ENTITY_FIGHT_INFO_BRIEF_H_
#define _ENTITY_FIGHT_INFO_BRIEF_H_
#include "vlib/Common.h"

class CEntityFightInfoBrief{
public:	
	unsigned int m_uAttackType;                               //攻击类型
	unsigned int m_uArmorType;								//护甲类型
	unsigned int m_uAttackMagicType;							//攻击的属性类型
	//当前有效的各种抗性
	int m_stMagicAtkRatio[MAX_MAGIC_CNT];	//所有属性加成,结算顺序应该是：(自身+装备),然后是系统buff，然后是战斗buff等
	int m_stMagicDefRatio[MAX_MAGIC_CNT];    //所有属性抗性


	int m_uDefAdjustRatio;					//防御调整系数
	int m_uAtkAdjustRatio;					//攻击力调整系数

	uint64 m_uEffAtk;                         //目前的有效攻击，已经算了自己的基础数值、加上枪械的以及其他绝对值加成的 
	uint64 m_uEffDef;							//目前的有效防御，已经算了自己的基础数值和枪械的绝对值加成项
	int m_iLevel;
};


#endif