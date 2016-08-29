#include "EvolutionObj.h"


EvolutionObj::EvolutionObj()
:m_uEvolutionID(0)
,m_uUsedSkillIdx(0)
{
}

EvolutionObj::~EvolutionObj()
{
}

unsigned int EvolutionObj::getEvolutionID() {
	return m_uEvolutionID;
}

void EvolutionObj::setEvolutionID(unsigned int uEvolutionID) {
	m_uEvolutionID = uEvolutionID;
}

const std::vector<vmsg::CSEvolutionSkill>& EvolutionObj::getSkills() {
	return m_stSkills;
}

std::vector<vmsg::CSEvolutionSkill>& EvolutionObj::mutableSkills() {
	return m_stSkills;
}

unsigned int EvolutionObj::getUsedSkillIdx() {
	return m_uUsedSkillIdx;
}

void EvolutionObj::setUsedSkillIdx(unsigned int uUsedSkillIdx) {
	m_uUsedSkillIdx = uUsedSkillIdx;
}
