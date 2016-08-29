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

	void sendArenaQueryRqst();												//��ѯ��������Ϣ
	void sendArenaQueryRecordRqst();										//��ѯս����¼
	void sendArenaFindOpponentRqst();										//ƥ�����
	void sendArenaGetAwardRqst();											//��ȡÿ�ս���
	void sendArenaBuyFightNumRqst();										//���򾺼�������
	void sendArenaStartFightRqst(const UIN_t uin);						//��ʼս��
	void sendArenaEndFightRqst(const UIN_t uin, unsigned int isMyWin);	//����ս��
	void sendStoreQueryRqst();												//��ѯ�̵�
	void sendStoreBuyRqst(unsigned int itemIdx, unsigned int itemCfgID);	//�һ���Ʒ
	void sendStoreRefreshRqst();											//ˢ����Ʒ
	void sendQueryArenaArmyRankRqst(unsigned int uLv, const UIN_t uin = 0);		//��ѯ���������а� 
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

	bool m_bHasNewRecord; // ���µ���ս��¼

	CC_SYNTHESIZE(bool, m_bIsSendMsg, IsSendMsg);		//�Ƿ��Ѿ������˽�����Ϣ
};

#define P_ARENA_CTRL CVLib::CVSingleton<CArenaControl>::Instance()

#endif // ArenaControl_h__
