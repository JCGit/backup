#ifndef ArenaControl_h__
#define ArenaControl_h__

#include "ArenaProxy.h"
#include "ArenaObj.h"
#include "ArenaPanel.h"
#include "MatchPanel.h"
#include "RecordPanel.h"
#include "ExchangePanel.h"
#include "ExchangeInfoPanel.h"
#include "ArenaRankPanel.h"
#include "ArenaRulePanel.h"
#include "ArenaResultPanel.h"
#include "ArenaStartPanel.h"

class CArenaControl
{
public:
	CArenaControl();
	~CArenaControl();

	void sendArenaQueryRqst();												//查询竞技场信息
	void sendArenaQueryRecordRqst();										//查询战斗记录
	void sendArenaFindOpponentRqst();										//匹配对手
	void sendArenaGetAwardRqst();											//领取每日奖励
	void sendArenaBuyFightNumRqst();										//购买竞技场次数
	void sendArenaStartFightRqst(const UIN_t uin);						//开始战斗
	void sendArenaEndFightRqst(const UIN_t uin, unsigned int isMyWin);	//结束战斗
	void sendStoreQueryRqst();												//查询商店
	void sendStoreBuyRqst(unsigned int itemIdx, unsigned int itemCfgID);	//兑换物品
	void sendStoreRefreshRqst();											//刷新物品
	void sendQueryArenaArmyRankRqst(unsigned int uLv, const UIN_t uin = 0);		//查询竞技场排行榜 
	void sendCleanCDRqst();

	CArenaObj* getArenaObj() {
		return &m_stArenaObj;
	}

	CArenaPanel* getArenaPanel();
	CMatchPanel* getMatchPanel();
	CRecordPanel* getRecordPanel();
	CExchangePanel* getExchangePanel();
	CArenaRankPanel* getRankPanel();
	CArenaRulePanel* getRulePanel();
	CArenaResultPanel* getResultPanel();
	CArenaStartPanel* getStartPanel();

	void openExchangeInfoPanel(unsigned int cfgID, unsigned int cfgType, unsigned int cfgCnt, unsigned int idx, unsigned int coin);

	void setHasNewRecord(bool bHasNewRecord);

	void notifyArenaNewRecord();

	bool hasFreeChallenge();

	void notifyArenaFreeChallenge();

	void doArenaQueryRsp(const CSArenaQueryRsp& stMsg);
	void doRecordRsp(const CSArenaQueryRecordRsp& stMsg);
	void doBuyFightNumRsp(const CSArenaBuyNumRsp& stMsg);
	void doFindOpponentRsp(const CSArenaFindOpponentRsp& stMsg);
	void doGetAwardRsp();
	void doStoreInfoRsp(const CSArenaStroeInfo& stMsg);
	void doArenaFightStartRsp(const CSArenaStartFightRsp& stMsg);
	void doArenaFightOverRsp(const CSArenaEndFightRsp& stMsg);
	void doArenaRankRsp(const CSRankQueryArenaArmyRankRsp& stMsg);

	void updateDetailInfoByWeapon(CSPlayerDetailInfo* stInfo, const vmsg::CSWeapon& stWeapon);

	void doCleanCDRsp();
protected:
private:
	CArenaProxy m_stArenaProxy;
	CArenaObj m_stArenaObj;

	CArenaPanel m_stArenaPanel;
	CMatchPanel m_stMatchPanel;
	CRecordPanel m_stRecordPanel;
	CExchangePanel m_stExchangePanel;
	CExchangeInfoPanel m_stExchangeInfoPanel;
	CArenaRankPanel m_stRankPanel;
	CArenaRulePanel m_stRulePanel;
	CArenaResultPanel m_stResultPanel;
	CArenaStartPanel m_stStartPanel;

	bool m_bHasNewRecord; // 有新的挑战记录

	CC_SYNTHESIZE(bool, m_bIsSendMsg, IsSendMsg);		//是否已经发送了结算信息
};

#define P_ARENA_CTRL CVLib::CVSingleton<CArenaControl>::Instance()

#endif // ArenaControl_h__
