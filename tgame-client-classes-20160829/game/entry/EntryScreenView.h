#ifndef EntryScreenView_h__
#define EntryScreenView_h__

#include "basic/BaseLayout.h"

class EntryScreenView: public BaseLayout
{
public:
	EntryScreenView();
	virtual ~EntryScreenView();

	static EntryScreenView* create();

	// override start

	virtual bool init();

	// override end

	void initUI();

private:
	void touchEventHandler(CCObject *pSender, TouchEventType type);

	void scrollEventHandler(CCObject *pSender, ScrollviewEventType type);

private:
	UIButton *m_pBackBtn;

	ScrollView *m_pScreenScrollView;

	ImageView *m_pBgImg;

	UILayout *m_pEntryLayout;

	int m_nScrollType;

	UIButton *m_pEquipEntryBtn;

	UIButton *m_pWeaponEntryBtn;

	UIButton *m_pEvolutionEntryBtn;

	UIButton *m_pArenaEntryBtn;

	UIButton *m_pInfinitEntryBtn;

};

#endif // EntryScreenView_h__