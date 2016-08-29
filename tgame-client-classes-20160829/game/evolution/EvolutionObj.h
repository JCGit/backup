#ifndef EvolutionObj_h__
#define EvolutionObj_h__

#include "CSPlayerMsg.pb.h"

class EvolutionObj 
{
public:
	EvolutionObj();
	~EvolutionObj();

	unsigned int getEvolutionID();

	void setEvolutionID(unsigned int uEvolutionID);

	const std::vector<vmsg::CSEvolutionSkill>& getSkills();

	std::vector<vmsg::CSEvolutionSkill>& mutableSkills();

	unsigned int getUsedSkillIdx();

	void setUsedSkillIdx(unsigned int uUsedSkillIdx);

private:
	unsigned int m_uEvolutionID; // 当前进化id

	std::vector<vmsg::CSEvolutionSkill> m_stSkills; // 已拥有的进化技

	unsigned int m_uUsedSkillIdx; // 当前设置的进化技的索引
};

#endif // EvolutionObj_h__