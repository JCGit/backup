#pragma once
#include "cocos2d.h"
#include "net/proto/ClientSvrMsg.pb.h"

enum ACTTYPE
{
	ACT_NULL = -1,
	ACT_LOGIN,//ǩ��
	ACT_FIRST_CHARGE = 1,//�׳�
	ACT_DAILY_CHARGE = 2,//ÿ�ճ�
	ACT_RECHARGE = 3,//�ۼƳ�
//	ACT_MAIL = 4,//�ʼ�(Ĭ�ϴ�)
	ACT_NEWER_GIFT = 5,//�������
	ACT_CRAZY_WEEK = 6, //�����ĩ
	ACT_CHRISTMAS  = 7, // ʥ���
	ACT_NEW_YEAR = 8,//Ԫ���
	ACT_VALENTINE = 14,//���˽ڻ
	ACT_TOP_RANK_STAR = 16,	//�۷����У�pve��������
	ACT_TOP_RANK_WIN = 17,	//�۷����У�pvpʤ����������
	ACT_TOP_RANK_WINSTREAK = 18,	//�۷����У�pvp��ʤ��������
};

class ActLayout :public cocos2d::Layer
{
public:
	ActLayout(void);
	~ActLayout(void);

	virtual void updateInfo(){}
	
	virtual void delCell(std::vector<int>){}

	virtual bool tableViewMove(bool isRigth){ return true; }
};

