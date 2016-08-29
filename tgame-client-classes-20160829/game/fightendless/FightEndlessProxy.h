#ifndef FightEndlessProxy_h__
#define FightEndlessProxy_h__

#include "QBaseProxy.h"
#include "BasicTypes.h"
#include "vlib/CVArray.h"

class CFightEndlessProxy: public QBaseProxy
{
public:
	CFightEndlessProxy();
	virtual ~CFightEndlessProxy();

	// override rsp handler
	virtual void DoRsp(vmsg::VLogicSvr2CliMsg &stMsg);

	//基因强化
	void sendInfiniteGenInfoRqst();
	void sendInfiniteBuyGenRqst(unsigned int idx);

	void sendEndlessStartRqst();
	void sendEndlessEndRqst(const UNIT64_t score, const unsigned int packageCnt, const unsigned int waveCnt, 
		unsigned int coin, CVArray<int, 4> stMonsterCnt, unsigned int evolutionCnt);
};

#endif // FightEndlessProxy_h__