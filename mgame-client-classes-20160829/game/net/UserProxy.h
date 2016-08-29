#pragma once

//�����Ϣͨ����

#include "BaseProxy.h"

class UserProxy : public BaseProxy
{
public:
	UserProxy(void);
	~UserProxy(void);

	virtual void DoRsp(NetPack& stRsp);
	virtual bool fault(const fgame::CSErrorMsg &err);

	//�ؿ���Ϣ
	void sendUpdateCheckPoint(int id, unsigned star,int passTime,bool isWin, int openCheckPt = 0);//ͨ����Ϣ
	void sendGetGift(int id, int type);//�ؿ������ȡ��Ϣ 1ͨ����� 2�������
	void sendGetNewBieReward();			//����ѵ��Ӫ������
	void sendGetCheckPtData();
	void sendEnterPoint(int pointId);

	//����������Ϣ
	void sendGetNewBieList();
	void sendFinishGuideID(int id, bool isComplete);
	void sendFinishGuideIDList(std::vector<int > idList, bool isComplete);
	
	 
	void sendGuideDiamonRqst(int count); 

	void sendGuideGlyphStuffRqst();

private:
};

