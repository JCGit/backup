#ifndef _SKILL_H_
#define _SKILL_H_
#include <cocos2d.h>
#include <string>

using namespace std;
USING_NS_CC;


class CSkillEff{
public: 
	unsigned char m_ucEffLayerCnt;  //���ܵ�����Ч������
	unsigned char m_ucEffRepeatCnt;
	unsigned int m_uiEffVal; //����Ч���̶�ֵ
	unsigned int m_uiEffRatio; //Ч������
};

class CSkill:public CCObject{
public:
	unsigned int m_uiSkillID;
	string m_stSkillName;
	string m_stSkillDesc;
	string m_stAtkDist;
	string m_stCastActID;  //�ü��ܶ�Ӧ���ͷŶ���
	string m_stFlyEffID;   //�ü��ܶ�Ӧ�ķ�����Ч
	string m_stHurtEffID;  //�ü��ܶ�Ӧ���ܻ���Ч
	string m_stFlySound;   //���ܵķ�����Ч
	string m_stHurtSound;  //���ܻ�����Ч
	unsigned char m_ucEffPlayCnt; //���ܵ�Ч�����ô���	
	CSkillEff m_astSkillEff[2];  //���ܵ�Ч������
	unsigned int m_uNextCD;      //���ܵ���һ���ͷ�ʱ��
	unsigned int m_uCD;          //�ü��ܵ�cd
	unsigned int m_uHitBackDist; //�����е��˵�λ��
};


#endif