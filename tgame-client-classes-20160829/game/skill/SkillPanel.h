#ifndef SkillPanel_h__
#define SkillPanel_h__

#include "BasePanel.h"

class CSkillPanel : public BasePanel
{
public:
	CSkillPanel();
	virtual ~CSkillPanel();

	CREATE_FUNC(CSkillPanel);

	bool init();

	void updateUI();

	void open();
	void close();
private:
	void initUI();
	void touchEventHandler(CCObject* pSender, TouchEventType type);

private:
	Button* m_pCloseBtn;
	Button* m_pTabWpSkillBtn;
	Button* m_pTabEvSkillBtn;
	Button* m_pSetBtn;

	Label* m_pTimeTxt;
	Label* m_pRemainTxt;
	Label* m_pSkillEffTxt;
	Label* m_pUpEffTxt;
	Label* m_pCoinTxt;
	Label* m_pCostPointTxt;

	Layout* m_pUpLayout;

	ImageView* m_pPointImg;
	ImageView* m_pListTitleImg;
	ImageView* m_pSkillEffImg;


};

#endif // SkillPanel_h__
