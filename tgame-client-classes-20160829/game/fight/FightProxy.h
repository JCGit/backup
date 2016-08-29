#ifndef FightProxy_h__
#define FightProxy_h__

#include "QBaseProxy.h"
#include "CSWarMapMsg.pb.h"
#include "BasicTypes.h"

class CFightProxy : public QBaseProxy
{
public:
	CFightProxy();
	virtual ~CFightProxy();

	virtual void DoRsp(vmsg::VLogicSvr2CliMsg& stRsp);

	//��ѯ�ؿ���Ϣ
	void sendQueryWarMapInfoRqst(unsigned int fromID, unsigned int toID);

	//�ؿ���ʼ
	void sendWarMapStartRqst(unsigned int warMapID);

	//�ؿ�����
	void sendWarMapEndRqst(vmsg::CSWarMapEndRqst& stWarEndRqst);

	//�鿨
	void sendWarMapCardRqst(unsigned int warMapID, unsigned int isFree);

	//��ѯ��ϸ��Ϣ
	void sendQueryPlayerFightInfo();

	//������Ѳ�ս
	void sendInviteFriendFightRqst(UIN_t friendUin);

	//ɨ������
	void sendSweepWarRqst(unsigned int warMapID, unsigned int round);

	void sendQueryEliteWarMapRqst(unsigned int uEliteWarChapterID); // ��ѯ��Ӣ����

	void sendStartEliteWarMapRqst(unsigned int uEliteWarMapID); // ��ʼ��Ӣ����
	
	void sendEndEliteWarMapRqst(unsigned int uEliteWarMapID, unsigned int uEvolutionUseCnt, unsigned int uScore); // ������Ӣ����

	void sendResetEliteWarMapRqst(unsigned int uEliteWarChapterID); // ���þ�Ӣ����

	void sendSweepEliteWarMapRqst(unsigned int uEliteWarChapterID); // ɨ����Ӣ����

	//�����ӵ�
	void sendBuyBulletRqst(unsigned int uIsUseItem, unsigned int uItemID = 0, int uBagIdx = -1);

	//ս���и���,
	void sendReliveRqst(unsigned int uIsUseItem, unsigned int uItemID = 0, int uBagIdx = -1);

	//����ؿ���ս����
	void sendBuyInstanceRqst(unsigned int uWarID);

	void sendGetStarAwardRqst(unsigned int uWarID, unsigned int uStarCnt);

};

#endif // FightProxy_h__
