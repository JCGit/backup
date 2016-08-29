#ifndef EliteWarCell_h__
#define EliteWarCell_h__

#include "basic/BaseLayout.h"

class EliteWarCell: public BaseLayout
{
public:
	enum EliteWarStat {
		ELITE_WAR_LOCKED,
		ELITE_WAR_OPENED,
		ELITE_WAR_PASSED,
	};

public:
	EliteWarCell();
	virtual ~EliteWarCell();

	static EliteWarCell* create();

	// override start

	virtual bool init();

	// override end

	unsigned int getWarMapID();

	void setWarMapID(unsigned int uWarMapID);

	void setStat(EliteWarStat stat);

	void setStarNum(unsigned int num);

private:
	UIImageView *m_pEliteWarLockedImg;

	UIImageView *m_pEliteWarOpenedImg;

	UIImageView *m_pEliteWarPassedImg;

	UIImageView *m_pEliteWarBossImg;

	UIImageView *m_pStar1Img;

	UIImageView *m_pStar2Img;

	UIImageView *m_pStar3Img;

	UIImageView *m_pStampImg;

	unsigned int m_uWarMapID;

	unsigned int m_uStat;

};

#endif // EliteWarCell_h__