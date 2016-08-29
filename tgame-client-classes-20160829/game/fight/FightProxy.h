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

	//查询关卡信息
	void sendQueryWarMapInfoRqst(unsigned int fromID, unsigned int toID);

	//关卡开始
	void sendWarMapStartRqst(unsigned int warMapID);

	//关卡结束
	void sendWarMapEndRqst(vmsg::CSWarMapEndRqst& stWarEndRqst);

	//抽卡
	void sendWarMapCardRqst(unsigned int warMapID, unsigned int isFree);

	//查询详细信息
	void sendQueryPlayerFightInfo();

	//邀请好友参战
	void sendInviteFriendFightRqst(UIN_t friendUin);

	//扫荡请求
	void sendSweepWarRqst(unsigned int warMapID, unsigned int round);

	void sendQueryEliteWarMapRqst(unsigned int uEliteWarChapterID); // 查询精英副本

	void sendStartEliteWarMapRqst(unsigned int uEliteWarMapID); // 开始精英副本
	
	void sendEndEliteWarMapRqst(unsigned int uEliteWarMapID, unsigned int uEvolutionUseCnt, unsigned int uScore); // 结束精英副本

	void sendResetEliteWarMapRqst(unsigned int uEliteWarChapterID); // 重置精英副本

	void sendSweepEliteWarMapRqst(unsigned int uEliteWarChapterID); // 扫荡精英副本

	//购买子弹
	void sendBuyBulletRqst(unsigned int uIsUseItem, unsigned int uItemID = 0, int uBagIdx = -1);

	//战斗中复活,
	void sendReliveRqst(unsigned int uIsUseItem, unsigned int uItemID = 0, int uBagIdx = -1);

	//购买关卡挑战次数
	void sendBuyInstanceRqst(unsigned int uWarID);

	void sendGetStarAwardRqst(unsigned int uWarID, unsigned int uStarCnt);

};

#endif // FightProxy_h__
