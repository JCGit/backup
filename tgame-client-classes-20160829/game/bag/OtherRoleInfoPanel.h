#ifndef OtherRoleInfoPanel_h__
#define OtherRoleInfoPanel_h__

#include "basic/BasePanel.h"

class StarCell;
class EquipCell;
class CMainHero;
class CStarProgressBar;
class ImageNumber;

class OtherRoleInfoPanel: public BasePanel , public CCTouchDelegate
{
public:
	OtherRoleInfoPanel();
	virtual ~OtherRoleInfoPanel();

	static OtherRoleInfoPanel* create();

	// override start

	virtual bool init();

	virtual void open();

	virtual void close();

	virtual void onExit();

	// override end

	void initUI();

	void updateUI();

	void updateRole();

	void updateDetailInfo();

	//为了屏蔽下层的触摸事件
	virtual void registerWithTouchDispatcher(); 
	virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent); 
	virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent); 
	virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent); 
private:
	void touchEventHandler(CCObject *pSender, TouchEventType type);

	void loadHeroRes();

private:
	UIButton *m_pBackBtn;

	UILabel *m_pInfoRoleNameTxt;

	UILayout *m_pRoleLayout;

	CMainHero *m_pRole; // 角色

	EquipCell *m_pWeaponCell;

	CStarProgressBar *m_pWeaponStarProgress;

	EquipCell *m_pArmorCell;

	EquipCell *m_pAmuletCell;

	EquipCell *m_pShoesCell;

	EquipCell *m_pCloakCell;

	UILabel *m_pInfoLevelValTxt;

	UILabel *m_pInfoHpValTxt;

	UILabel *m_pInfoAttackValTxt;

	UILabel *m_pInfoDefenseValTxt;

	UILabel *m_pInfoHitValTxt;

	UILabel *m_pInfoHpAddValTxt;

	UILabel *m_pInfoAttackAddValTxt;

	UILabel *m_pInfoDefenseAddValTxt;

	UILabel *m_pInfoHitAddValTxt;

	UIButton *m_pOptionBtn;

	// detail
	UILabel *m_pRoleNameTxt;

	ImageNumber *m_pVIPTxt;

	UILabel *m_pLevelValTxt;

	UILabel *m_pHpValTxt;

	UILoadingBar *m_pExpProgress;

	UILabel *m_pExpProgressTxt;

	UILabel *m_pFightPowerValTxt;

	UILabel *m_pUINValTxt;

	UILabel *m_pDefValTxt;

	UILabel *m_pAtkValTxt;

	UILabel *m_pHitValTxt;

	UILabel *m_pDefTypeValTxt;

	UILabel *m_pAtkTypeValTxt;

	UILabel *m_pThunderAtkValTxt;

	UILabel *m_pPoisonAtkValTxt;

	UILabel *m_pFireAtkValTxt;

	UILabel *m_pIceAtkValTxt;

	UILabel *m_pBiochemistryAtkValTxt;

	UILabel *m_pThunderDefValTxt;

	UILabel *m_pPoisonDefValTxt;

	UILabel *m_pFireDefValTxt;

	UILabel *m_pIceDefValTxt;

	UILabel *m_pBiochemistryDefValTxt;

};

#endif // OtherRoleInfoPanel_h__