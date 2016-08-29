#ifndef GuildActCell_h__
#define GuildActCell_h__

#include "BaseLayout.h"

class GuildActCell: public BaseLayout
{
public:
	GuildActCell();
	virtual ~GuildActCell();
	
	static GuildActCell* create();

	// override start

	virtual bool init();

	// override end

	void updateUI();

	bool isSelected();

	void selected(bool bSelected);

private:
	UIImageView *m_pBgSelectedImg;

	UIImageView *m_pIconImg;

	UILabel *m_pActNameTxt;

	bool m_bSelected;

};

#endif // GuildActCell_h__