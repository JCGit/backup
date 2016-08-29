#ifndef _SKILL_H_
#define _SKILL_H_
#include <cocos2d.h>
#include <string>

using namespace std;
USING_NS_CC;


class CSkillEff{
public: 
	unsigned char m_ucEffLayerCnt;  //技能的作用效果层数
	unsigned char m_ucEffRepeatCnt;
	unsigned int m_uiEffVal; //技能效果固定值
	unsigned int m_uiEffRatio; //效果比例
};

class CSkill:public CCObject{
public:
	unsigned int m_uiSkillID;
	string m_stSkillName;
	string m_stSkillDesc;
	string m_stAtkDist;
	string m_stCastActID;  //该技能对应的释放动作
	string m_stFlyEffID;   //该技能对应的飞行特效
	string m_stHurtEffID;  //该技能对应的受击特效
	string m_stFlySound;   //技能的飞行音效
	string m_stHurtSound;  //技能击中音效
	unsigned char m_ucEffPlayCnt; //技能的效果作用次数	
	CSkillEff m_astSkillEff[2];  //技能的效果类型
	unsigned int m_uNextCD;      //技能的下一次释放时间
	unsigned int m_uCD;          //该技能的cd
	unsigned int m_uHitBackDist; //被击中的人的位移
};


#endif