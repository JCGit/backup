#ifndef FightEndlessControl_h__
#define FightEndlessControl_h__

#include "FightEndlessProxy.h"
#include "FightEndlessRankPanel.h"
#include "StrengthenPanel.h"
#include "FightEndlessResultPanel.h"
#include "FightEndlessShowPanel.h"

using namespace vmsg;

class CFightEndlessControl
{
public:
	CFightEndlessControl();
	~CFightEndlessControl();

	void sendInfiniteGenInfoRqst();
	void sendInfiniteBuyGenRqst(unsigned int idx);

	void sendEndlessStartRqst();
	void sendEndlessEndRqst(const UNIT64_t score, const unsigned int packageCnt, const unsigned int waveCnt, 
		unsigned int coin, CVArray<int, 4> stMonsterCnt, unsigned int evolutionCnt);

	void doGenInfoRsp(CSInfiniteGenInfo* pstRsp);
	void doBuyGenRsp(CSInfiniteGenInfo* pstRsp);
	void doInfiniteEnd(CSInfiniteEndRsp* pstRsp);

	void updateUserInfoByGenLst();
	void updateUserInfoByGenInfo(const CSGen& stGen);

	const CSInfiniteGenInfo& getGenInfo(){
		return m_stGenInfo;
	}

	const CSInfiniteEndRsp& getEndInfo(){
		return m_stEndInfo;
	}

	CFightEndlessRankPanel* getEndlessRankPanel();
	CStrengthenPanel* getStrengthenPanel();
	CFightEndlessResultPanel* getResultPanel();
	CFightEndlessShowPanel* getShowPanel();

	const string getDescByGenInfo(const CSGen& info);
private:
	CFightEndlessProxy m_stEndlessProxy;
	CFightEndlessRankPanel m_stRankPanel;
	CStrengthenPanel m_stStrengthenPanel;
	CFightEndlessResultPanel m_stResultPanel;
	CFightEndlessShowPanel m_stShowPanel;

	CSInfiniteGenInfo m_stGenInfo;		//基因数据
	CSInfiniteEndRsp m_stEndInfo;		//结算奖励数据

	CC_SYNTHESIZE(unsigned int, _score, Score);
	CC_SYNTHESIZE(unsigned int, _maxScore, MaxScore);
	CC_SYNTHESIZE(unsigned int, _coin, Coin);

	CC_SYNTHESIZE(unsigned int, _scoreAdd, ScoreAdd);  //分数加成
	CC_SYNTHESIZE(unsigned int, _coinAdd, CoinAdd);		//联邦币加成

	CSWeapon m_stRandWeapon;
};

#define P_ENDLESS_CTRL CVLib::CVSingleton<CFightEndlessControl>::Instance()

#endif // FightEndlessControl_h__