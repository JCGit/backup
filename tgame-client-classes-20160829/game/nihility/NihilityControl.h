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

	//����ǿ��
	void sendInfiniteGenInfoRqst();
	void sendInfiniteBuyGenRqst(unsigned int idx);

	//��ѯ�ϴ��Ƿ������˳�
	void sendNihilityUnfinishedQueryRqst();

	//ÿ��һ���ϱ�һ��
	void sendNihilityMissonReportRqst(unsigned int evolutioncnt);

	void sendNihilityFightStartRqst();		
	void sendNihilityFightEndRqst(unsigned int evolutioncnt);

	//��ʷ��¼
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
	CSInfiniteGenInfo m_stGenInfo;		//��������
	CSNothingLandHistoryRsp m_stHistoryInfo;

	CVArray<float, 6> m_stAllWeaponBulletVal;		//��¼�����ҳ���ʱ������ǹе�ĵ�ҩ��(�±�Ϊ0�Ĵ�����ʱǹе�ĵ�ҩ)

	unsigned int _score;
	unsigned int _coin;

	//�����㷨��ķ����ͽ����
	unsigned int m_uScoreMd5;
	unsigned int m_uCoinMd5;

	unsigned int m_uRandData;

	CC_SYNTHESIZE(unsigned int, _scoreAdd, ScoreAdd);  //�����ӳ�
	CC_SYNTHESIZE(unsigned int, _coinAdd, CoinAdd);		//����Ҽӳ�
	//CC_SYNTHESIZE(unsigned int, _score, Score);
	//CC_SYNTHESIZE(unsigned int, _maxScore, MaxScore);
	//CC_SYNTHESIZE(unsigned int, _coin, Coin);
	CC_SYNTHESIZE(unsigned int, _floor, Floor);
	//CC_SYNTHESIZE(unsigned int, _maxFloor, MaxFloor);
	CC_SYNTHESIZE(unsigned int, _needRush, NeedRush);	//�Ƿ�ɨ��

	CC_SYNTHESIZE(unsigned int, _floorScore, FloorScore);
		
	CC_SYNTHESIZE(bool, _bIsContinue, bIsContinue);		//�Ƿ�Ϊ�����ϴ���ս

	CC_SYNTHESIZE(unsigned int, _lastEvolutionCnt, LastEvolutionCnt);		//��ʷ�ļ���ʹ�ô���

	CC_SYNTHESIZE(bool, _isForceQuit, IsForceQuit);		//�Ƿ�ǿ���˳�

	CVArray<int, 100> m_stAwardFloor;		//�����Ĳ�������

	CC_SYNTHESIZE(bool, m_bIsAutoFight, bIsAutoFight);
};

#define P_NIHILITY_CTRL CVLib::CVSingleton<CNihilityControl>::Instance()

#endif // NihilityControl_h__
