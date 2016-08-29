#ifndef ArenaResultPanel_h__
#define ArenaResultPanel_h__

#include "BasePanel.h"

class CArenaResultPanel : public BasePanel
{
public:
	CArenaResultPanel();
	virtual ~CArenaResultPanel();

	CREATE_FUNC(CArenaResultPanel);

	bool init();

	void updateUI();
protected:
private:
	void touchEventHandler(CCObject *pSender, TouchEventType type);

	void updateRole();

	void End();
private:  
	UIButton* m_pSureBtn;
	UILabel* m_pScoreTxt;
	UILabel* m_pCoinTxt;
	UILabel* m_pArenaCoinTxt;

	ImageView* m_pResultImg;

};

#endif // ArenaResultPanel_h__
