#include "AttrStrengthenObj.h"

AttrStrengthenObj::AttrStrengthenObj()
{
}

AttrStrengthenObj::~AttrStrengthenObj()
{
}

const vmsg::CSAttrStrengthenInfo& AttrStrengthenObj::getAttrStrengthenInfo() {
	return m_stAttrStrengthenInfo;
}

void AttrStrengthenObj::setAttrStrengthenInfo(const vmsg::CSAttrStrengthenInfo &stAttrStrengthenInfo) {
	m_stAttrStrengthenInfo = stAttrStrengthenInfo;
}
