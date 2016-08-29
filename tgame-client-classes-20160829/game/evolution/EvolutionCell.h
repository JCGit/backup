#ifndef EvolutionCell_h__
#define EvolutionCell_h__

#include "basic/BaseLayout.h"

class EvolutionCell: public UIWidget
{
public:
	enum CellStat {
		CELL_DARK,
		CELL_NEXT,
		CELL_LIGHT
	};
public:
	EvolutionCell();
	virtual ~EvolutionCell();

	virtual void setStat(CellStat stat);

protected:
	CellStat m_eStat;

};

class EvolutionStarCell: public EvolutionCell
{
public:
	enum StarColor {
		STAR_GREEN, 
		STAR_BLUE, 
		STAR_PURPLE, 
		STAR_YELLOW, 
		STAR_RED, 
	};
public:
	EvolutionStarCell();
	virtual ~EvolutionStarCell();

	/*static EvolutionStarCell* create(UILayout *pRefLayout);

	bool initWithLayout(UILayout *pRefLayout);*/

	static EvolutionStarCell* create();

	virtual bool init();

	// override start

	virtual void setStat(CellStat stat);

	// override end

	void setColor(StarColor color);

private:
	UIImageView *m_pStarImg;

	unsigned int m_uColor;

};

class EvolutionSkillCell: public EvolutionCell
{
public:
	EvolutionSkillCell();
	virtual ~EvolutionSkillCell();

	/*static EvolutionSkillCell* create(UILayout *pRefLayout);

	bool initWithLayout(UILayout *pRefLayout);*/

	static EvolutionSkillCell* create();

	virtual bool init();

	// override start

	virtual void setStat(CellStat stat);

	// override end

	void setSkill(unsigned int uSkillID);

private:
	UIImageView *m_pBgIconImg;

	UIImageView *m_pSkillIconImg;

};

#endif // EvolutionCell_h__