#ifndef NihilityControl_h__
#define NihilityControl_h__

#include "NihilityProxy.h"
#include "CSInfiniteMsg.pb.h"
#include "NihilityShowPanel.h"
#include "NihilityResultPanel.h"
#include "NihilityPanel.h"
#include "NihilityItemPanel.h"
#include "NihilityGenPanel.h"
#include "NihilityContinuePanel.h"

using namespace vmsg;

class CNihilityControl
{
public:
	CNihilityControl();
	virtual ~CNihilityControl();

	//基因强化
	void sendInfiniteGenInfoRqst();
	void sendInfiniteBuyGenRqst(unsigned int idx);

	//查询上次是否正常退出
	void sendNihilityUnfinishedQueryRqst();

	//每过一层上报一次
	void sendNihilityMissonReportRqst(unsigned int evolutioncnt);

	void sendNihilityFightStartRqst();		
	void sendNihilityFightEndRqst(unsigned int evolutioncnt);

	//历史记录
	void doNihilityHistoryRsp(const CSNothingLandHistoryRsp& stRsp);
	const CSNothingLandHistoryRsp& getHistoryInfo(){
		return m_stHistoryInfo;
	}

	void doNihilityStartRsp();
	void doNihilityEndRsp(CSNothingLandEndRsp* pstRsp);

	void addAwarFloor(unsigned int floor);
	unsigned int getAwardFloorCount(){
		return m_stAwardFloor.Size();
	}

	void resetNihility();

	void addCoin(unsigned int addCnt);
	void addScore(unsigned int addCnt);

	unsigned int getCoin(){
		return _coin;
	}

	unsigned int getScore(){
		return _score;
	}

	CNihilityShowPanel* getShowPanel();
	CNihilityResultPanel* getResultPanel();
	CNihilityPanel* getNihilityPanel();
	CNihilityItemPanel* getItemPanel();
	CNihilityGenPanel* getGenPanel();
	CNihilityContinuePanel* getContinuePanel();

	const CSNothingLandEndRsp& getNihilityEndInfo(){
		return m_stEndInfo;
	}

	void doGenInfoRsp(CSInfiniteGenInfo* pstRsp);
	void doBuyGenRsp(CSInfiniteGenInfo* pstRsp);

	const CSInfiniteGenInfo& getGenInfo(){
		return m_stGenInfo;
	}

	const string getDescByGenInfo(const CSGen& info);

	void updateUserInfoByGenLst();
	void updateUserInfoByGenInfo(const CSGen& stGen);
	void updateTempWeapon(const CSGen& stGen);

	CVArray<float, 6> getAllWeaponBulletVal(){
		return m_stAllWeaponBulletVal;
	}

	void updateAllWeaponBulletVal(CVArray<float, 6> stArr);

private:
	bool checkData(unsigned int uPreData, unsigned int uMd5Data);
	void updateData(unsigned int uPreData, unsigned int& uMd5Data);
	void resumeData(unsigned int& uPreData, unsigned int uMd5Data);

private:
	CNihilityShowPanel m_stShowPanel;
	CNihilityResultPanel m_stResultPanel;
	CNihilityPanel m_stNihilityPanel;
	CNihilityItemPanel m_stItemPanel;
	CNihilityGenPanel m_stGenPanel;
	CNihilityContinuePanel m_stContinuePanel;

	CNihilityProxy m_stProxy;
	CSNothingLandEndRsp m_stEndInfo;
	CSInfiniteGenInfo m_stGenInfo;		//基因数据
	CSNothingLandHistoryRsp m_stHistoryInfo;

	CVArray<float, 6> m_stAllWeaponBulletVal;		//记录进入金币场景时，各种枪械的弹药量(下标为0的代表临时枪械的弹药)

	unsigned int _score;
	unsigned int _coin;

	//经过算法后的分数和金币数
	unsigned int m_uScoreMd5;
	unsigned int m_uCoinMd5;

	unsigned int m_uRandData;

	CC_SYNTHESIZE(unsigned int, _scoreAdd, ScoreAdd);  //分数加成
	CC_SYNTHESIZE(unsigned int, _coinAdd, CoinAdd);		//联邦币加成
	//CC_SYNTHESIZE(unsigned int, _score, Score);
	//CC_SYNTHESIZE(unsigned int, _maxScore, MaxScore);
	//CC_SYNTHESIZE(unsigned int, _coin, Coin);
	CC_SYNTHESIZE(unsigned int, _floor, Floor);
	//CC_SYNTHESIZE(unsigned int, _maxFloor, MaxFloor);
	CC_SYNTHESIZE(unsigned int, _needRush, NeedRush);	//是否扫荡

	CC_SYNTHESIZE(unsigned int, _floorScore, FloorScore);
		
	CC_SYNTHESIZE(bool, _bIsContinue, bIsContinue);		//是否为继续上次挑战

	CC_SYNTHESIZE(unsigned int, _lastEvolutionCnt, LastEvolutionCnt);		//历史的技能使用次数

	CC_SYNTHESIZE(bool, _isForceQuit, IsForceQuit);		//是否强制退出

	CVArray<int, 100> m_stAwardFloor;		//奖励的层数索引

	CC_SYNTHESIZE(bool, m_bIsAutoFight, bIsAutoFight);
};

#define P_NIHILITY_CTRL CVLib::CVSingleton<CNihilityControl>::Instance()

#endif // NihilityControl_h__
