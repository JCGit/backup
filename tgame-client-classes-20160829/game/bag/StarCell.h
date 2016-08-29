#ifndef StarCell_h__
#define StarCell_h__

#include "basic/BaseLayout.h"

class StarCell: public BaseLayout
{
public:
	enum {
		STATE_DARK,
		STATE_LIGHT,
	};

public:
	StarCell();
	virtual ~StarCell();
	
	static StarCell* create();

	static StarCell* create(UILayout *pRefLayout);

	// override start

	virtual bool init();

	// override end

	virtual bool initWithLayout(UILayout *pRefLayout);

	unsigned int getState();

	void setState(unsigned int uState);

	void setLevel(unsigned int uLevel);

private:
	UIImageView *m_pDarkImg;

	UIImageView *m_pNormalImg;

	UIImageView *m_pLightImg;

	UILabel *m_pLevelTxt;

	unsigned int m_uState;

	unsigned int m_uLevel;

};

#endif // StarCell_h__