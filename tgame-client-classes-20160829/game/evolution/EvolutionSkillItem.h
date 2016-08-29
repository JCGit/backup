#ifndef EvolutionSkillItem_h__
#define EvolutionSkillItem_h__

#include "CSPlayerMsg.pb.h"

#include "basic/BaseLayout.h"

class EvolutionSkillItem: public BaseLayout
{
public:
	EvolutionSkillItem();
	virtual ~EvolutionSkillItem();

	static EvolutionSkillItem* create();

	// override start

	virtual bool init();

	// override end

	bool isSelected();

	void selected(bool bSelected);

	void updateUI(const vmsg::CSEvolutionSkill& stSkill);

private:
	void initUI();

private:
	bool m_bSelected;

	UIImageView *m_pBgImg;

	UIImageView *m_pSkillIconImg;

	UILabel *m_pSkillNameTxt;

	UILabel *m_pSkillLevelTxt;

};

#endif // EvolutionSkillItem_h__