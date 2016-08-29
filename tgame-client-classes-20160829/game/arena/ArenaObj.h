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
        //����ʱ��Ҫ�ϱ�
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

	//���������ɶ�����ϸ��Ϣ
	void genRandOpDetailInfo(CSPlayerDetailInfo& stDetailInfo);

protected:
private:
	CSArenaQueryRsp m_stArenaInfo;					//�Լ���������Ϣ
	CSArenaQueryRecordRsp m_stRecordInfo;			//�Լ�ս����¼
	CSArenaFindOpponentRsp m_stFindInfo;			//ƥ����Ϣ
	CSArenaBuyNumRsp m_stBuyNumInfo;				//�����������
	CSArenaStartFightRsp m_stFightStartInfo;		//ս����ʼ��Ϣ
	CSArenaEndFightRsp m_stFightEndInfo;			//ս��������Ϣ
	CSArenaStroeInfo m_stStoreInfo;					//�̵���Ϣ
	CSRankQueryArenaArmyRankRsp m_stRankInfo;		//������Ϣ

	CC_SYNTHESIZE(int, _myRank, MyRank);

	CC_SYNTHESIZE(UIN_t, _opponentUin, OpponentUin);		//ƥ��Ķ���uin
	CC_SYNTHESIZE(unsigned int, _isMyWin, IsMyWin);			//PVP���

    int m_iFightBackRecTime; //����ʱ��Ҫ�ϱ��ڼ�¼���ʱ��
};

#endif // ArenaObj_h__
