#ifndef ChooseRolePanel_h__
#define ChooseRolePanel_h__

#include "basic/BasePanel.h"

//class UFOCell;
class CMainHero;

class ChooseRolePanel: public BasePanel
{
public:
	ChooseRolePanel();
	virtual ~ChooseRolePanel();

	static ChooseRolePanel* create();

	// override start

	virtual bool init();

	virtual void open();

	virtual void close();

	virtual void onExit();

	// override end

	void initUI();

	void updateUI();

	void updateRoles();

	void updateRole(unsigned int uShowID, unsigned int uWeaponCfgID);

	void selectRole(int nSelectedIdx);

private:
	void touchEventHandler(CCObject *pSender, TouchEventType type);

	void listViewEventHandler(CCObject *pSender, ListViewEventType type);

    int getTagByIndex(const int iIdx);

    int getIndexByTag(const int iTag);

    void selectRoleByTag(const unsigned int iSelChildTag);

private:
	CCArmature *m_pBgArmture;

	CCArmature *m_pFrontBgArmture;

	UIImageView *m_pBgImg;

	UIImageView *m_pLightingMaskImg;

	UIImageView *m_pRoleLightingImg;

	UILabel *m_pSvrNameValTxt;

	UILabel *m_pRoleNameTxt;

	UIButton *m_pEnterBtn;

	UILayout *m_pRolesLayout;

	UIListView *m_pRolesListView;

	int m_nRoleSelectedTag;

	CCNode *m_pAircraftBody;

	CCArmature *m_pAircraftArmture;

	UIImageView *m_pAircraftImg;

	CMainHero *m_pRole;

	int m_nRoleSelectedIdx;

};

#endif // ChooseRolePanel_h__