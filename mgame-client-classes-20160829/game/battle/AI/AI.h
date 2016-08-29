/*
* jacey
* AI��� ������ÿ����Ӫ��λ

@brief һ����AI�жϡ������������������Ӧһ����Ϊ

1. class AI. ��ӪAI������ڣ�ӵ�о�����Policy��Ա��
2. class PolicyManager. Ŀǰ����AI״̬����Ϊ�Ĺ���
3. Policy�������ǰAI������Ӫ�����н�����˳��ִ�н���ӵ�е�״̬��Ϊ������ִ��AI�жϳɹ������˳��������к���AI�жϣ���

*/
#pragma once

#include "basic/Policy.h"
#include "Constant.h"
#include "Message.h"

class Camp;
class AI : public MessageHandler
{
public:
	AI();
	~AI();

	bool init(eCampType etype);
	void update(float deta);
	static AI* create(eCampType etype);

	virtual void onReceiveMassage(GameMessage * message);

	eCampType	getCampType(){ return mCampType; }


protected:

	// AI ����
	Policy	mPolicy;

	bool	mPauseAI;


	/* ��Ϸ�߼��������
	*/
	eCampType	mCampType;

};