#ifndef NihilityProxy_h__
#define NihilityProxy_h__

#include "QBaseProxy.h"
#include "BasicTypes.h"
#include "vlib/CVArray.h"

class CNihilityProxy : public QBaseProxy
{
public:
	CNihilityProxy();
	virtual ~CNihilityProxy();

	// override rsp handler
	virtual void DoRsp(vmsg::VLogicSvr2CliMsg &stMsg);

	//基因强化
	void sendInfiniteGenInfoRqst();
	void sendInfiniteBuyGenRqst(unsigned int idx);

	//1表示继续挑战没完成的，0表示重新开始
	void sendNihilityFightStartRqst(unsigned int isContinue);			

	//每过一层上报一次
	void sendNihilityMissonReportRqst(unsigned int evolutioncnt, unsigned int floorScore, unsigned int coin, unsigned int floor, unsigned int score, CVArray<int, 100> stAwardFloor);	

	//查询上次是否正常退出
	void sendNihilityUnfinishedQueryRqst();	

	//stAwardFloor:宝箱掉落的层数索引
	void sendNihilityFightEndRqst(unsigned int evolutioncnt, unsigned int floorScore, unsigned int coin, unsigned int floor, unsigned int score, CVArray<int, 100> stAwardFloor);
protected:
private:
};

#endif // NihilityProxy_h__
