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
	unsigned int m_uEvolutionID; // ��ǰ����id

	std::vector<vmsg::CSEvolutionSkill> m_stSkills; // ��ӵ�еĽ�����

	unsigned int m_uUsedSkillIdx; // ��ǰ���õĽ�����������
};

#endif // EvolutionObj_h__