#ifndef _WEAPON_H_
#define _WEAPON_H_


USING_NS_CC_EXT;
USING_NS_CC;
#include "vlib/Common.h"
class CWeapon: public CCObject{
public:
	CC_SYNTHESIZE(unsigned int, m_uiCfgID, CfgID);    //配置id
	CC_SYNTHESIZE(unsigned int, m_uAtkType, AtkType);   //攻击类型
	CC_SYNTHESIZE(unsigned int, m_uAtkMagicType, AtkMagicType);  //攻击属性类型
	CC_SYNTHESIZE(uint64, m_uWeaponID, WeaponID);   //武器ID

};


#endif