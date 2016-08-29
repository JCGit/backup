#ifndef LotteryControl_h__
#define LotteryControl_h__

#include "vlib/base/CVSingleton.h"

#include "LotteryProxy.h"
#include "LotteryPanel.h"
#include "LotteryStartPanel.h"
#include "LotteryVIPTipPanel.h"
#include "CoinGetWayPanel.h"
#include "WeaponLotResultPanel.h"

class LotteryControl
{
public:
	LotteryControl();
	~LotteryControl();

	void init();

	void finalize();

	LotteryPanel* getLotteryPanel();

	LotteryStartPanel* getLotteryStartPanel();

	LotteryVIPTipPanel* getLotteryVIPTipPanel();

	CoinGetWayPanel* getCoinGetWayPanel();

	WeaponLotResultPanel* getLotteryResultPanel();

	unsigned int getCoinLotFreeTime();

	unsigned int getGoldLotFreeTime();

	unsigned int getGoldLotCertainlyCnt();

	unsigned int getVIPLotTodayCnt();

	unsigned int getVIPLotCertainlyCnt();

	bool hasFreeLottery();

	void notifyFreeLottery();

	void onLoginCheckTime(); // 初始化时查询一下

	void onHeartBeat(); // 心跳时检查是否有剩余次数

	void lottery(unsigned int uLotteryType = 0);

	void onRecvMsg(vmsg::VLogicSvr2CliMsg &stMsg);

	void sendQueryLotteryRqst(); // 查询抽卡信息

	void sendLotteryRqst(unsigned int uLotteryType); // 抽卡

private:
	bool m_bInitialized;

	LotteryProxy m_stLotteryProxy;

	LotteryPanel *m_pLotteryPanel;

	LotteryStartPanel *m_pLotteryStartPanel;

	LotteryVIPTipPanel *m_pLotteryVIPTipPanel;

	CoinGetWayPanel *m_pCoinGetWayPanel;

	WeaponLotResultPanel *m_pLotteryResultPanel;

	unsigned int m_uCoinLotFreeTime; // 免费金币抽卡剩余时间

	unsigned int m_uGoldLotFreeTime; // 免费钻石抽卡剩余时间

	unsigned int m_uGoldLotCertainlyCnt; // 钻石抽卡必定抽中剩余次数

	unsigned int m_uVIPLotTodayCnt; // VIP抽卡今天剩余次数

	unsigned int m_uVIPLotCertainlyCnt; // VIP抽卡必定抽中剩余次数

	unsigned int m_uCoinLotTotalTime; // 金币抽卡当前时间

	unsigned int m_uGoldLotTotalTime; // 钻石抽卡当前时间

	unsigned int m_uLastLotteryType;

};

#define P_LOTTERY_CTRL CVLib::CVSingleton<LotteryControl>::Instance()

#endif // LotteryControl_h__
