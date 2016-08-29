#ifndef ArenaObj_h__
#define ArenaObj_h__

#include "CSArenaMsg.pb.h"
#include "CSRankMsg.pb.h"
#include "BasicTypes.h"

using namespace vmsg;

class CArenaObj
{
public:
	CArenaObj();
	~CArenaObj();

	void setArenaInfo(const CSArenaQueryRsp& stMsg);
	const CSArenaQueryRsp& getArenaInfo(){
		return m_stArenaInfo;
	}

	void setRecordInfo(const CSArenaQueryRecordRsp& stMsg);
	const CSArenaQueryRecordRsp& getRecordInfo(){
		return m_stRecordInfo;
	}

	void setFindInfo(const CSArenaFindOpponentRsp& stMsg);
	const CSArenaFindOpponentRsp& getFindInfo(){
		return m_stFindInfo;
	}

	void setBuyNumInfo(const CSArenaBuyNumRsp& stMsg);
	const CSArenaBuyNumRsp& getBuyNumInfo(){
		return m_stBuyNumInfo;
	}

	void setFightStartInfo(const CSArenaStartFightRsp& stMsg);
	const CSArenaStartFightRsp& getFightStartInfo(){
		return m_stFightStartInfo;
	}

	void setFightEndInfo(const CSArenaEndFightRsp& stMsg);
	const CSArenaEndFightRsp& getFightEndInfo(){
		return m_stFightEndInfo;
	}

	void setStoreInfo(const CSArenaStroeInfo& stMsg);
	const CSArenaStroeInfo& getStoreInfo(){
		return m_stStoreInfo;
	}

	void doGetAwardInfo();

    void setFightBackInfo(const int iRecordTime){
        //反击时需要上报
        m_iFightBackRecTime = iRecordTime;
    }
    int getFightBackInfo() {
        return m_iFightBackRecTime;
    }

	void setRankInfo(const CSRankQueryArenaArmyRankRsp& stMsg){
		m_stRankInfo = stMsg;

		if(stMsg.has_myarmyrank() && stMsg.myarmyrank() >= 0){
			_myRank = stMsg.myarmyrank();
		}
	}
	const CSRankQueryArenaArmyRankRsp& getRankInfo(){
		return m_stRankInfo;
	}

	//从配置生成对手详细信息
	void genRandOpDetailInfo(CSPlayerDetailInfo& stDetailInfo);

protected:
private:
	CSArenaQueryRsp m_stArenaInfo;					//自己竞技场信息
	CSArenaQueryRecordRsp m_stRecordInfo;			//自己战斗记录
	CSArenaFindOpponentRsp m_stFindInfo;			//匹配信息
	CSArenaBuyNumRsp m_stBuyNumInfo;				//购买次数返回
	CSArenaStartFightRsp m_stFightStartInfo;		//战斗开始信息
	CSArenaEndFightRsp m_stFightEndInfo;			//战斗结束信息
	CSArenaStroeInfo m_stStoreInfo;					//商店信息
	CSRankQueryArenaArmyRankRsp m_stRankInfo;		//排行信息

	CC_SYNTHESIZE(int, _myRank, MyRank);

	CC_SYNTHESIZE(UIN_t, _opponentUin, OpponentUin);		//匹配的对手uin
	CC_SYNTHESIZE(unsigned int, _isMyWin, IsMyWin);			//PVP结果

    int m_iFightBackRecTime; //反击时需要上报在记录里的时间
};

#endif // ArenaObj_h__
