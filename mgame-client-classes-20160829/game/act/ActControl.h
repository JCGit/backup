#pragma once
#include "ActPanel.h"
#include "ActProxy.h"
#include "ActLoginPanel.h"
#include "Constant.h"

class ActControl
{
public:
	ActControl(void);
	~ActControl(void);

	CC_SYNTHESIZE(ActPanel*, mActPanel, ActPanel);
	ActProxy* getActProxy();

	void sendQueryActLogin();
	void sendGetActLogin();

	void doRueryActLogin(const fgame::CSSevenDaySignListResp&);
	void doGetActLogin(const fgame::CSSevenDaySignResp&);

	std::map<int,bool>* getActTypeEff() { return &mActTypeEff; }

	void doRechargeCheck();

	bool getCurrRechargeData(int type);


	bool checkCurrActCanGet();

	bool checkActCanGetByType(int type);

	void setActCanGetByType(int type,bool _get);
	
	void sendGetActNewerCharge();
	void doGetActNewerCharge();
	
	void clearActTypeEff();

	void updateFirstChargeView();

	void setIsNewYearIconOpen(bool isOpen);
	void setIsValentineIconOpen(bool isOpen);
private:
	bool isSameDay(int day);
private:
	ActProxy mActProxy;
	ActLoginPanel* mActLoginPanel;
	CC_SYNTHESIZE(bool ,mIsCanAutoShow, isCanAutoShow);


	//��ֵ��Ϣ
	std::map<int,bool> mActTypeEff;//���ʾ��ǩ
	CC_SYNTHESIZE(int ,mRechargeCount, RechargeCount);//�ۼƳ�ֵ
	CC_SYNTHESIZE(int ,mRechargeId, RechargeId);//�ۼ�ID

	CC_SYNTHESIZE(int ,mDailyCount, DailyCount);//ÿ���ۼƳ�ֵ
	CC_SYNTHESIZE(int ,mDailyId, DailyId);//ÿ��ID

	CC_SYNTHESIZE(int ,mFirstId, FirstId);//�׳�ID
	CC_SYNTHESIZE(bool, mIsShowFirstCharge, IsShowFirstCharge); // �Ƿ���ʾ�׳�

	CC_SYNTHESIZE(bool, mIsNewerGiftOpened, IsNewerGiftOpened);//�׳�ID

	CC_SYNTHESIZE(bool, mIsChristmasActOpen, IsChristmasActOpen); // ����
	
	CC_SYNTHESIZE(bool, mIsTopRankActOpen, IsTopRankActOpen); // ����

	std::vector<int> mDelMailVec;

	CC_SYNTHESIZE(int, mQueryLoginDay, QueryLoginDay);;//�ϴβ�ѯǩ��ʱ��day

	CC_SYNTHESIZE(bool,mBtnRefresh,BtnRefresh);
};
#define sgActControl() Singleton<ActControl>::Get()

