#ifndef AttrStrengthenProxy_h__
#define AttrStrengthenProxy_h__

#include "basic/QBaseProxy.h"

class AttrStrengthenProxy: public QBaseProxy
{
public:
	AttrStrengthenProxy();
	virtual ~AttrStrengthenProxy();

	// override start

	virtual void DoRsp(vmsg::VLogicSvr2CliMsg& stRsp);

	// override end

	void sendQueryAttrStrengthenInfoRqst(); // ��ѯ����ǿ��

	void sendAttrStrengthenRqst(unsigned int uType, unsigned int uMagicType); // ǿ������

private:
	
};

#endif // AttrStrengthenProxy_h__