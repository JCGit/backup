#ifndef FightEndlessResultPanel_h__
#define FightEndlessResultPanel_h__

#include "BasePanel.h"
#include "role/MainHero.h"
#include "RollNumber.h"

class CFightEndlessResultPanel : public BasePanel
{
public:
	CFightEndlessResultPanel();
	virtual ~CFightEndlessResultPanel();

	CREATE_FUNC(CFightEndlessResultPanel);

	bool init();

	void updateUI();

	void updateRole();
protected:
private:
	void initUI();
	void touchEventHandler(CCObject *pSender, TouchEventType type);

	UIButton* m_pSureBtn;
	UIButton* m_pShareBtn;
	UIImageView* m_pRoleImg;
	UILabel* m_pScoreTxt;
	UILabel* m_pCoinTxt;
	UILabel* m_pMaxTxt;

	CRollNumber* m_pScoreLabel;

	CMainHero* m_pRole;			//½ÇÉ«¶¯»­

	CCArray *m_pUICellArray;
	CCArray *m_pCellArray;
};

#endif // FightEndlessResultPanel_h__
