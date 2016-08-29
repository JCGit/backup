#pragma once
#include "cocos2d.h"
#include "Constant.h"
#include "WinnerDialog.h"
#include "WinnerProxy.h"

enum WINNERTYPE
{
	WINNER_ROAD = 0,//����֮·
	WINNER_STAR,//����֮��
	WINNER_FIGHT,//����֮��
};

class WinnerSubActivity{
public:
	WinnerSubActivity()
	{
		Act_Id = 0;
		Sub_Id = 0;
		GiftId = 0;
		Request = 0;
		Value = 0;
		Completed = false;
		Submid = false;
	}
	 int Act_Id; //�ID
	 int Sub_Id; //��ID
	 int GiftId; 
	 int Request; //����ֵ
	 int Value;   //��ǰֵ
	 bool   Completed; //�Ƿ����
	 bool   Submid;   //�Ƿ��ύ
};

class WinnerActivityData{
public:
	WinnerActivityData()
	{
		Act_type = 0;
		Cur_SubId = 0;
		DaysRemain = 0;
		IsCanGet = false;
	}
	 int Act_type;  //����� 
	 int Cur_SubId; //��ǰ���ID
	 int DaysRemain; //�ʣ������
	 bool IsCanGet; //true ����ȡ
	 std::map<int,WinnerSubActivity> SubList;
};

class WinnerControl
{
public:
	WinnerControl(void);
	~WinnerControl(void);

	CC_SYNTHESIZE(WinnerDialog*, mWinnerDialog, WinnerDialogPtr);

	CC_SYNTHESIZE(bool, m_IsGet, IsGet);

	CC_SYNTHESIZE_READONLY(int, m_WinnerCount, WinnerCount);

	CC_SYNTHESIZE(int, m_GetCount, GetWinnerCount);

	CC_SYNTHESIZE(bool,mBtnRefresh,BtnRefresh);

	void sendGetRqst();

	void sendWinnerValueRqst();

	void setWinnerCount(int val);

	bool getCanGet();

	void sendTaskComplete(int actId);

	void sendWinnerTip(int cout,int gif);

	bool checkCanOpen(int type);
	
	WinnerProxy* getWinnerProxy() { return &mWinnerProxy ;}

	bool getWinnerActOpen(int actID);

	WinnerActivityData* getWinnerActList(int actId);

	WinnerSubActivity* getWinnerSubID(int actId,int subId);

	std::map<int, WinnerActivityData>  mWinnerActList;

	//�Ƿ�����ʤ����
	bool getWinnerRoadOpen() { return mWinnerRoadOpen; }
	void setWinnerRoadOpen(unsigned long long _time);
private:
	WinnerProxy mWinnerProxy;
	bool mWinnerRoadOpen;
};

#define sgWinnerControl() Singleton<WinnerControl>::Get()