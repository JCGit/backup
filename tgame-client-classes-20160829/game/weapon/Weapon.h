#ifndef _WEAPON_H_
#define _WEAPON_H_


USING_NS_CC_EXT;
USING_NS_CC;
#include "vlib/Common.h"
class CWeapon: public CCObject{
public:
	CC_SYNTHESIZE(unsigned int, m_uiCfgID, CfgID);    //����id
	CC_SYNTHESIZE(unsigned int, m_uAtkType, AtkType);   //��������
	CC_SYNTHESIZE(unsigned int, m_uAtkMagicType, AtkMagicType);  //������������
	CC_SYNTHESIZE(uint64, m_uWeaponID, WeaponID);   //����ID

};


#endif