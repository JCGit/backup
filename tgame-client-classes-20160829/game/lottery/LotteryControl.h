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

	void onLoginCheckTime(); // ��ʼ��ʱ��ѯһ��

	void onHeartBeat(); // ����ʱ����Ƿ���ʣ�����

	void lottery(unsigned int uLotteryType = 0);

	void onRecvMsg(vmsg::VLogicSvr2CliMsg &stMsg);

	void sendQueryLotteryRqst(); // ��ѯ�鿨��Ϣ

	void sendLotteryRqst(unsigned int uLotteryType); // �鿨

private:
	bool m_bInitialized;

	LotteryProxy m_stLotteryProxy;

	LotteryPanel *m_pLotteryPanel;

	LotteryStartPanel *m_pLotteryStartPanel;

	LotteryVIPTipPanel *m_pLotteryVIPTipPanel;

	CoinGetWayPanel *m_pCoinGetWayPanel;

	WeaponLotResultPanel *m_pLotteryResultPanel;

	unsigned int m_uCoinLotFreeTime; // ��ѽ�ҳ鿨ʣ��ʱ��

	unsigned int m_uGoldLotFreeTime; // �����ʯ�鿨ʣ��ʱ��

	unsigned int m_uGoldLotCertainlyCnt; // ��ʯ�鿨�ض�����ʣ�����

	unsigned int m_uVIPLotTodayCnt; // VIP�鿨����ʣ�����

	unsigned int m_uVIPLotCertainlyCnt; // VIP�鿨�ض�����ʣ�����

	unsigned int m_uCoinLotTotalTime; // ��ҳ鿨��ǰʱ��

	unsigned int m_uGoldLotTotalTime; // ��ʯ�鿨��ǰʱ��

	unsigned int m_uLastLotteryType;

};

#define P_LOTTERY_CTRL CVLib::CVSingleton<LotteryControl>::Instance()

#endif // LotteryControl_h__
