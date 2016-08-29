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
	void sendCleanCDRqst();													//���cd

	void sendQueryArenaArmyRankRqst(unsigned int start, unsigned int stop, unsigned int cnt, const UIN_t uin = 0);		//��ѯ���������а� 
protected:
private:
};

#endif // ArenaProxy_h__
