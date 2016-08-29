#ifndef RoleInfoPanel_h__
#define RoleInfoPanel_h__

#include "basic/BasePanel.h"
#include "game/guide/GuideControl.h"

class EquipCell;
class CMainHero;
class CStarProgressBar;
class BagLayout;
class DetailInfoLayout;

class RoleInfoPanel: public BasePanel, public GuideContainer
{
public:
	RoleInfoPanel();
	virtual ~RoleInfoPanel();

	static RoleInfoPanel* create();

	// override start

	virtual bool init();

	virtual void open();

	virtual void close();

	virtual CCNode* getGuideNodeByName(const std::string &name);

	// override end

	void initUI();

	void updateUI();

	void updateRoleInfo();

	void updateDetailInfo();

	void updateBag();

private:
	void updateRole();

	void touchEventHandler(CCObject *pSender, TouchEventType type);

private:
	UIButton *m_pBackBtn;

	UILabel *m_pRoleNameTxt;

	UILayout *m_pRoleLayout;

	CMainHero *m_pRole; // ½ÇÉ«

	EquipCell *m_pWeaponCell;

	CStarProgressBar *m_pWeaponStarProgress;

	EquipCell *m_pArmorCell;

	EquipCell *m_pAmuletCell;

	EquipCell *m_pShoesCell;

	EquipCell *m_pCloakCell;

	UILabel *m_pLevelValTxt;

	UILabel *m_pHpValTxt;

	UILabel *m_pAtkValTxt;

	UILabel *m_pDefValTxt;

	UILabel *m_pHitValTxt;

	UILabel *m_pHpAddValTxt;

	UILabel *m_pAtkAddValTxt;

	UILabel *m_pDefAddValTxt;

	UILabel *m_pHitAddValTxt;

	UIButton *m_pBagOrDetailBtn;

	BagLayout *m_pBagLayout;

	DetailInfoLayout *m_pDetailInfoLayout;

};

#endif //RoleInfoPanel_h__