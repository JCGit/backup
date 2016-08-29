#pragma once
#include "cocos2d.h"
#include "Constant.h"
#include "BaseProxy.h"

class CrazyWeekProxy : public BaseProxy
{
public:
	CrazyWeekProxy();
	~CrazyWeekProxy();

	virtual void DoRsp(NetPack& stRsp);
	virtual bool fault(const fgame::CSErrorMsg &err);

	void sendGetCrazyWeekInfoReq();
	void sendGetRewardReq(int rewardId = 1);
};

class ActCrazyWeekControl
{
public:
	ActCrazyWeekControl();
	virtual ~ActCrazyWeekControl();

	// ��������ʾ��ܿ�ʼ
	void	showCrazyWeekIcon(bool flag = false);
	// ���������ʾ���
	void	showCrazyWeekTip(bool flag = false);

	// �����������
	void	sendGetCrazyWeekInfoReq();
	// ��ȡ���ܽ���
	void	sendGetRewardReq();

	// ͬ����������
	void	syncCrazyWeekInfo(const fgame::CSActivityCrazyDtlsResp &dtlsResp);
	// ���¿������ݣ������������·���
	void	updateCrazyWeekInfo(const fgame::CSActivityCrazyUsableResp &usableResp);
	//	��ȡ���ܽ���
	void	doReward(const fgame::CSActivityCrazyUsageResp &usageResp);

	CC_SYNTHESIZE(int,	mCurrDay, Currday);
	CC_SYNTHESIZE(int,	mRewardId, RewardId);
	CC_SYNTHESIZE(int,	mCrazyPoint, CrazyPoint);
	CC_SYNTHESIZE(int,	mExtraHonourPoint, ExtraHonourPoint);
	CC_SYNTHESIZE(int,  mConditionCompleted, ConditionCompleted);
	CC_SYNTHESIZE(int,  mSubConditionCompleted, SubConditionCompleted);
	CC_SYNTHESIZE(int,  mRewardState, RewardState);   // 0 �����죬 1 ���죬 2 ����
	CC_SYNTHESIZE(std::string, mTimeString, TimeString);
	CC_SYNTHESIZE(bool, mIsCrazyWeekOpen, IsCrazyWeekOpen);
protected:
	CrazyWeekProxy mCrazyWeekProxy;

private:
	//int		mCurrDay;
	//int		mRewardId;
	//int		mCrazyPoint;
	//int		mExtraHonourPoint;
	//bool		mConditionCompleted;
	//bool		mSubConditionCompleted;
	//bool		mIsGetReward;
};

#define sgActCrazyWeekControl() Singleton<ActCrazyWeekControl>::Get()