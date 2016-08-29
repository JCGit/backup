#ifndef FightGuideDialog_h__
#define FightGuideDialog_h__

#include "BaseLayout.h"

class CFightGuideDialog : public BaseLayout
{
public:
	CFightGuideDialog();
	virtual ~CFightGuideDialog();

	CREATE_FUNC(CFightGuideDialog);

	bool init();

	void showDialog();
protected:

private:
	void touchEventHandler(CCObject *pSender, TouchEventType type);

private:
	UILayout* m_pBgLayout;
	UIImageView *m_pBgWordsImg;
	UIImageView *m_pRoleHeadImg;
	UIImageView* m_pNameBgImg;
	UIImageView* m_pLightImg;
	ImageView* m_pTipBg;
	UILabel *m_pRoleNameTxt;
	UILabel *m_pWordsTxt;
};

#endif // FightGuideDialog_h__
