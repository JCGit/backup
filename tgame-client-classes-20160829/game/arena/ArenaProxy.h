#ifndef ArenaProxy_h__
#define ArenaProxy_h__

#include "QBaseProxy.h"
#include "BasicTypes.h"

class CArenaProxy : public QBaseProxy
{
public:
	CArenaProxy();
	virtual ~CArenaProxy();

	virtual void DoRsp(vmsg::VLogicSvr2CliMsg &stMsg);

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
	void sendCleanCDRqst();													//清除cd

	void sendQueryArenaArmyRankRqst(unsigned int start, unsigned int stop, unsigned int cnt, const UIN_t uin = 0);		//查询竞技场排行榜 
protected:
private:
};

#endif // ArenaProxy_h__
