#ifndef LevelEliteEntryPanel_h__
#define LevelEliteEntryPanel_h__

#include "basic/BasePanel.h"
#include "game/guide/GuideControl.h"

class EliteWarEntryPanel: public BasePanel, public GuideContainer
{
public:
	EliteWarEntryPanel();
	virtual ~EliteWarEntryPanel();

	static EliteWarEntryPanel* create();

	// override start

	virtual bool init();

	virtual void open();

	virtual void close();

	CCNode* getGuideNodeByName(const std::string &name);

	// override end

	void openWithEliteWarMapID(unsigned int uEliteWarMapID);

	void openWithQuery(unsigned int uEliteWarMapID);

	bool isOpenWithQuery();

private:
	void initUI();

	void touchEventHandler(CCObject *pSender, TouchEventType type);

private:
	UIButton *m_pCloseBtn;

	UIImageView *m_pBgInteriorImg;

	UIImageView *m_pTextChapterImg;

	UILabel *m_pWarMapTxt;

	UILabel *m_pBetterLevelValTxt;

	UILabel *m_pWinCondValTxt;

	UIButton *m_pEnterBtn;

	CCArray *m_pAwardCells;

	unsigned int m_uEliteWarMapID;

	bool m_bOpenWithQuery;

};

#endif // LevelEliteEntryPanel_h__