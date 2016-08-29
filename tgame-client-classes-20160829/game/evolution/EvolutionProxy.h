#ifndef EvolutionProxy_h__
#define EvolutionProxy_h__

#include "basic/QBaseProxy.h"

class EvolutionProxy: public QBaseProxy
{
public:
	EvolutionProxy();
	virtual ~EvolutionProxy();

	// override start

	virtual void DoRsp(vmsg::VLogicSvr2CliMsg& stRsp);

	// override end

	void sendEvolutionSkillRqst();

	void sendChangeEvolutionSkillRqst(unsigned int uSkillID);

	void sendQueryEvolutionInfoRqst();

	void sendQueryEvolutionSkillInfoRqst();

private:
	
};

#endif // EvolutionProxy_h__