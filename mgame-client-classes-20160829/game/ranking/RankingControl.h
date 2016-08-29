#pragma once

#include "Constant.h"
#include "Singleton.h"
#include "RankingProxy.h"
#include "net/NetPack.h"

class RankingProxy;
class RankingDialog;
class RankingControl
{
public:
	struct PlayerData
	{
		PlayerData() : id(0), rankingId(0), point(0), winRate(0), score(0), vipLevel(0)
		{

		}

		~PlayerData()
		{

		}

		uint64 id;
		int rankingId;
		std::string name;
		int point;
		float winRate;
		int score;
		int vipLevel;
	};

	struct PvpPlayerResultData
	{
		PvpPlayerResultData() : uin(0), addPoint(0), addScore(0)
		{

		}

		uint64 uin;
		std::string name;
		int addPoint; //���ӵķ���
		int addScore; // ���ӵ�����
	};

	struct PvpResultData
	{
		PvpResultData() : winUin(0)
		{

		}

		uint64 winUin;
		std::vector<PvpPlayerResultData*> resultList;
	};

	struct stuRankWeekAwardinfo
	{
		bool Status;
		int PvpCount;
		int GroupId;
		int Rank;

		stuRankWeekAwardinfo()
		{
			Status = false;
			PvpCount = 0;
			GroupId = 0;
			Rank = 0;
		}
	};
	stuRankWeekAwardinfo mRankWeekAwardinfo;

	struct stuRankWeekAward
	{
		bool Success;
		int  Error;

		stuRankWeekAward()
		{
			Success = false;
			Error = 0;
		}
	};
	stuRankWeekAward mRankWeekAward;

	// ����map�������Ψһ��
	typedef std::map<int, PlayerData*> RankingList;
	typedef std::map<eRankingGroupType, RankingList> GroupList;
	typedef std::list<PvpResultData*> PvpRoundResultList;

	bool getStatus(){ return mRankWeekAwardinfo.Status; };
	int getPvpCount(){ return mRankWeekAwardinfo.PvpCount; };
	int getGroupId(){ return mRankWeekAwardinfo.GroupId; };
	int getRank(){ return mRankWeekAwardinfo.Rank; };

	void setPvpCount(int count){ mRankWeekAwardinfo.PvpCount = count; };
	void setGroupId(int groupId){ mRankWeekAwardinfo.GroupId = groupId; };
	void setRank(int rank){ mRankWeekAwardinfo.Rank = rank; };


	bool getSuccess(){ return mRankWeekAward.Success; };
	int getError(){ return mRankWeekAward.Error; };
	
public:

	RankingControl();
	~RankingControl();

	void cleanup();

	void setView(RankingDialog* dialog) { mView = dialog; }
	RankingDialog* getView() const { return mView; }

	RankingProxy *	getProxy()const  { return mProxy; }

	// ����ָ����Top 50����
	void requestRankingList(eRankingGroupType groupType);
	// ���������Ϣ
	void requestPlayerRankingInfo();
	// �������pvp��ս�б�
	void requestPlayerPvpResultInfo();
	// ������������а��е���������
	void requestPlayerSelfRankingList();
	// �������н�����Ϣ
	void requestRankWeekAwardInfo();
	// ������ȡ���н���
	void requestRankWeekAward();

	void doNetGetRankingListRsp(const fgame::CSPvpRankingGetListResp& stMsg);
	void doNetPlayerRankingDataRsp(const fgame::CSPvpRankingPlayerInfoResp& stMsg);
	void doNetPlayerPvpResultRsp(const fgame::CSPvpResultResp& stMsg);
	void doNetGetSelfRankingResp(const fgame::CSPvpSelfRankListResp& stMsg);
	void doNetRankWeekAwardInfoResp(const fgame::CSRankWeekRewardInfoResp& stMsg);
	void doNetRankWeekAwardResp(const fgame::CSRankWeekRewardRewardResp stMsg);

	void addPvpResult(const fgame::CSPvpEndResp& pvpResult);
	void giveUp(int point);
	
	const PvpRoundResultList& getRoundResultList() const { return mPvpResultList; }

	//unsigned int getPlayerRoundCount() const { return mPlayerRoundCount; }
	//eRankingGroupType getPlayerGroupType() const { return mPlayerGroupType; }
	//int getPlayerPoint() const { return mPlayerRankingData.point; }
	//float getPlayerWinRate() const { return mPlayerRankingData.winRate; }

	int getPlayerCurrentRanking() const { return mPlayerRankingData.rankingId; }
	int getPlayerPrevRanking() const { return mPlayerPrevRankingId; }

	int getPlayerScore() const { return mPlayerRankingData.score; }

	bool mbLockDialogClose;
	bool getbLockDialogClose(){ return mbLockDialogClose; };
	void setbLockDialogClose(bool b){ mbLockDialogClose = b; };

	double mAwardTime;
	double getAwardTime(){ return mAwardTime; };
	void   setAwardTime(double time){ mAwardTime = time; };

protected:
	static eRankingGroupType PointToGroup(int point);
	static void CleanRankingList(RankingList& rankingList);

	void cleanRoundList();

	void rebuildRakingList(eRankingGroupType groupType);

	void onScheduleCleanGroupTopNCache(float dt, eRankingGroupType groupType);
	void onScheduleRequestPlayerResultInfo(float dt);

protected:
	static const int TopNSize = 50;

	RankingProxy* mProxy;
	RankingDialog* mView;

	GroupList mGroupsCache; // �������а�ֻ���𱣴����ݣ����������ݣ� ��mGroupsTopNCache��mSelfRankingList��mPlayerRankingData���������������
	GroupList mGroupsTopNCache; // ���������������ǰN������ 
	RankingList mSelfRankingList; // �����Լ������ܱߵ������������

	//eRankingGroupType mPlayerGroupType;
	//unsigned int mPlayerRoundCount;
	int mPlayerPrevRankingId;

	PlayerData mPlayerRankingData; // ����һ���Լ����ص����а񡰼١�����

	PvpRoundResultList mPvpResultList;
};

#define sgRankingControl() Singleton<RankingControl>::Get()