#ifndef LevelUpPanel_h__
#define LevelUpPanel_h__

#include "basic/BasePanel.h"

class ImageNumber;

class LevelUpPanel: public BasePanel
{
public:
	LevelUpPanel();
	virtual ~LevelUpPanel();

	static LevelUpPanel* create();

	// override start

	virtual bool init();

	virtual void open();

	virtual void close();

	virtual bool isMeetAutoOpen();

	// override end

	void levelup(unsigned int uPreLevel, unsigned int uLevel);

private:
	void initUI();

	void touchEventHandler(CCObject *pSender, TouchEventType type);

	vector<unsigned int> getWillOpenEntry();
private:
	UIButton *m_pCloseBtn;

	UIImageView *m_pUpToImg;

	ImageNumber *m_pLvImgTxt;

	UIImageView *m_pLvImg;

	UILabel *m_pLastHpTxt;

	UILabel *m_pLastAtkTxt;

	UILabel *m_pLastDefTxt;

	UILabel *m_pHpTxt;

	UILabel *m_pAtkTxt;

	UILabel *m_pDefTxt;

	UIImageView *m_pNewModuleOpenImg;

	UILayout *m_pEntrysLayout;

	bool m_bIsLevelUp; // ÊÇ·ñÉý¼¶ÁË

};

#endif // LevelUpPanel_h__