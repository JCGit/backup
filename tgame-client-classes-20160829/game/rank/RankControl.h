#ifndef RankControl_h__
#define RankControl_h__

#include "RankProxy.h"
#include "RankPanel.h"

using namespace vmsg;

class CRankControl
{
public:
	CRankControl();
	~CRankControl();

	void sendRankQueryRqst(unsigned int rankType, int startPos, int endPos);
	void sendRankQueryMyRankRqst(unsigned int rankType);

	void sendQueryAwardRankRqst();
	void sendFetchRankAwardRqst(unsigned int rankType);

	void doRankQueryRsp(const CSRankQueryRsp* pstRsp);
	void doRankQueryMyRankRsp(const CSRankQueryMyRankRsp* pstRsp);

	void doAwardRankRsp(const CSAwardRankRsp * pstRsp);

	CSRankQueryRsp& getRankInfo(){
		return m_stRankInfo;
	}

	CSRankQueryMyRankRsp& getMyRankInfo(){
		return m_stMyRankInfo;
	}

	CSAwardRankRsp& getRankAwardInfo(){
		return m_stRankAwardInfo;
	}

	CRankPanel* getRankPanel();
private:
	CC_SYNTHESIZE(bool, _isRankPanel, IsRankPanel);			//是否是同一排行榜

	CRankProxy m_stProxy;
	CRankPanel m_stRankPanel;

	CSRankQueryRsp m_stRankInfo;
	CSRankQueryMyRankRsp m_stMyRankInfo;
	CSAwardRankRsp m_stRankAwardInfo;
};

#define P_RANK_CTL CVLib::CVSingleton<CRankControl>::Instance()

#endif // RankControl_h__
