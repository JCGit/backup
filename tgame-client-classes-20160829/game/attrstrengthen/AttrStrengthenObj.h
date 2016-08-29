#ifndef AttrStrengthenObj_h__
#define AttrStrengthenObj_h__

#include "CSPlayerMsg.pb.h"

class AttrStrengthenObj 
{
public:
	AttrStrengthenObj();
	~AttrStrengthenObj();

	const vmsg::CSAttrStrengthenInfo& getAttrStrengthenInfo();

	void setAttrStrengthenInfo(const vmsg::CSAttrStrengthenInfo &stAttrStrengthenInfo);

private:
	vmsg::CSAttrStrengthenInfo m_stAttrStrengthenInfo;

};

#endif // AttrStrengthenObj_h__