#ifndef AttrStrengthenInfoPanel_h__
#define AttrStrengthenInfoPanel_h__

#include "basic/BasePanel.h"
#include "NPageView.h"

class AttrStrengthenInfoPanel: public BasePanel
{
public:
	AttrStrengthenInfoPanel();
	virtual ~AttrStrengthenInfoPanel();
	
	static AttrStrengthenInfoPanel* create();

	// override start

	virtual bool init();

	// override end

	void updateUI();

	void prevAttr();

	void nextAttr();

	CCPoint calcPoint(int nSelectedIdx);

private:
	void touchEventHandler(CCObject *pSender, TouchEventType type);

	void listViewEventHandler(CCObject *pSender, ListViewEventType type);

	void scrollViewEventHandler(CCObject *pSender, ScrollviewEventType type);

	void checkFlipBtnStatus();

	void updateAttrInfo();

	void strengthen();

private:
	UIImageView *m_pBgImg;

	UILabel *m_pCurAttrNameTxt;

	UIImageView *m_pCurAttrLevelImg;

	UILabelAtlas *m_pCurAttrLevelTxt;

	UILabel *m_pCurAttrLimitLevelTxt;

	UILabel *m_pCurAttrLimitLevelValTxt;

	UIButton *m_pStrengthenBtn;

	UIButton *m_pLeftFlipBtn;

	UIButton *m_pRightFlipBtn;

	UIScrollView *m_pAttrScrollView;

	NPageView *m_pAttrPage;

	UILayout *m_pAttrLayout;

	UIListView *m_pAttrListView;

	UILabel *m_pFireAtkAddValTxt;

	UILabel *m_pIceAtkAddValTxt;

	UILabel *m_pPoisonAtkAddValTxt;

	UILabel *m_pThunderAtkAddValTxt;

	UILabel *m_pBiochemistryAtkAddValTxt;

	UILabel *m_pFireDefAddValTxt;

	UILabel *m_pIceDefAddValTxt;

	UILabel *m_pPoisonDefAddValTxt;

	UILabel *m_pThunderDefAddValTxt;

	UILabel *m_pBiochemistryDefAddValTxt;

	int m_nCellSelectedIdx;

	int m_nScrollDirection;

};

#endif // AttrStrengthenInfoPanel_h__