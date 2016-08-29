#ifndef RankProxy_h__
#define RankProxy_h__

#include "QBaseProxy.h"
#include "BasicTypes.h"

class CRankProxy: public QBaseProxy
{
public:
	CRankProxy();
	virtual ~CRankProxy();

	// override rsp handler
	virtual void DoRsp(vmsg::VLogicSvr2CliMsg &stMsg);

	void sendRankQueryRqst(unsigned int rankType, int startPos, int endPos);
	void sendRankQueryMyRankRqst(unsigned int rankType);

	void sendQueryAwardRankRqst();
	void sendFetchRankAwardRqst(unsigned int rankType);
};

#endif // RankProxy_h__