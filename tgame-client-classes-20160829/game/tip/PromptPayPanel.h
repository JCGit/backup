#ifndef PromptPayPanel_h__
#define PromptPayPanel_h__

#include "basic/BasePanel.h"

class ImageNumber;

class PromptPayPanel: public BasePanel
{
public:
	enum PromptType {
		PROMPT_TYPE_GOLD,
		PROMPT_TYPE_VIP_PERM,
	};

public:
	PromptPayPanel();
	virtual ~PromptPayPanel();

	static PromptPayPanel* create();

	// override start

	virtual bool init();

	virtual void open();

	// override end

	void openWithDiffGold(unsigned int uDiffGoldVal);

	void openWithVIPModule(unsigned int uVIPModuleID);

	void updateUI();

private:
	void initUI();

	void touchEventHandler(CCObject *pSender, TouchEventType type);

private:
	UIButton *m_pCloseBtn;

	UIImageView *m_pTitleImg;

	UILayout *m_pVIPIntroLayout;

	UILabel *m_pCurVipTxt;

	ImageNumber *m_pCurVIPImgTxt;

	UILabel *m_pChargeTxt;

	UILabel *m_pGoldValTxt;

	UILabel *m_pNextVIPTxt;

	ImageNumber *m_pNextVIPImgTxt;

	UIButton *m_pSeePrivilegeBtn;

	UILabel *m_pPromptTxt;

	UILabel *m_pVIPPackageTitleTxt;

	UILayout *m_pAwardsLayout;

	UIButton *m_pChargeBtn;

	UILayout *m_pVIPTopLayout;

	UILabel *m_pCallTxt;

	ImageNumber *m_pTopVIPTxt;

	UILabel *m_pUserTxt;

	UILabel *m_pTopVIPPromptTxt;

	unsigned int m_uPromptType;

	unsigned int m_uDiffGoldVal;

	unsigned int m_uVIPModuleID;

};

#endif // PromptPayPanel_h__