#ifndef EliteWarPage_h__
#define EliteWarPage_h__

#include "basic/BaseLayout.h"

class EliteWarPage: public BaseLayout
{
public:
	EliteWarPage();
	virtual ~EliteWarPage();

	static EliteWarPage* create();

	// override start

	virtual bool init();

	// override end

	unsigned int getEliteWarChapterID();

	void setEliteWarChapterID(unsigned int uEliteWarChapterID);

	void updateUI();

	UIWidget* getCurWarMapCell();

private:
	void touchEventHandler(CCObject *pSender, TouchEventType type);

	void onDialog();

private:
	UIImageView *m_pChapterMapImg;

	CCArray *m_pEliteWarCells;

	unsigned int m_uEliteWarChapterID;

};

#endif // EliteWarPage_h__