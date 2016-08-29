#ifndef _ENTITY_FIGHT_INFO_BRIEF_H_
#define _ENTITY_FIGHT_INFO_BRIEF_H_
#include "vlib/Common.h"

class CEntityFightInfoBrief{
public:	
	unsigned int m_uAttackType;                               //��������
	unsigned int m_uArmorType;								//��������
	unsigned int m_uAttackMagicType;							//��������������
	//��ǰ��Ч�ĸ��ֿ���
	int m_stMagicAtkRatio[MAX_MAGIC_CNT];	//�������Լӳ�,����˳��Ӧ���ǣ�(����+װ��),Ȼ����ϵͳbuff��Ȼ����ս��buff��
	int m_stMagicDefRatio[MAX_MAGIC_CNT];    //�������Կ���


	int m_uDefAdjustRatio;					//��������ϵ��
	int m_uAtkAdjustRatio;					//����������ϵ��

	uint64 m_uEffAtk;                         //Ŀǰ����Ч�������Ѿ������Լ��Ļ�����ֵ������ǹе���Լ���������ֵ�ӳɵ� 
	uint64 m_uEffDef;							//Ŀǰ����Ч�������Ѿ������Լ��Ļ�����ֵ��ǹе�ľ���ֵ�ӳ���
	int m_iLevel;
};


#endif