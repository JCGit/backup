#ifndef UFOCell_h__
#define UFOCell_h__

#include "basic/BaseLayout.h"

class ShowRole;

class UFOCell: public BaseLayout
{
public:
	UFOCell();
	virtual ~UFOCell();

	static UFOCell* create();

	// override start

	virtual bool init();

	virtual void onEnter();

	// override end

	void updateRole(unsigned int uPlayerID);

	unsigned int getPlayerID();

	bool isSelected();

	void selected(bool bSelected);

private:
	CCArmature *m_pTrailFlameArmture;

	CCParticleSystemQuad *m_pBackParticleEff;

	UIImageView *m_pUFOImg;

	CCParticleSystemQuad *m_pFrontParticleEff;

	ShowRole *m_pRole;

	bool m_bSelected;

};

#endif // UFOCell_h__