#ifndef GuildMemberPanel_h__
#define GuildMemberPanel_h__

#include "basic/BasePanel.h"
#include "CSPlayerMsg.pb.h"

class EquipCell;

class GuildMemberPanel: public BasePanel
{
public:	
	enum {
		TYPE_MEMBER_GUILD = 1,
		TYPE_MEMBER_RANK = 2,
	};
public:
	GuildMemberPanel();
	virtual ~GuildMemberPanel();

	static GuildMemberPanel* create();

	// override start

	virtual bool init();

	// override end

	void updateUI();

	void updateOptionBtns();

private:
	void touchEventHandler(CCObject *pSender, TouchEventType type);

	void listViewEventHandler(CCObject *pSender, ListViewEventType type);

private:
	CC_SYNTHESIZE(unsigned int, _panelYype, PanelType);

	UIButton *m_pCloseBtn;

	EquipCell *m_pArmorCell;

	EquipCell *m_pRingCell;

	EquipCell *m_pShoesCell;

	EquipCell *m_pCloakCell;

	UIListView *m_pOptionsListView;

	UILabel *m_pHpValTxt;

	UILabel *m_pAttackValTxt;

	UILabel *m_pDefenseValTxt;

	UILabel *m_pAtkSpeedValTxt;

	UILabel *m_pMoveSpeedValTxt;

	UILabel *m_pAddValTxt;

	UILabel *m_pAtkTypeValTxt;

	UILabel *m_pDefTypeValTxt;

	UIButton *m_pDetailBtn;

	vmsg::CSPlayerInfo m_stPlayerInfo;

};

#endif // GuildMemberPanel_h__
