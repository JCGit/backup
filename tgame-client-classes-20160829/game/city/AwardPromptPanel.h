#ifndef AwardPromptPanel_h__
#define AwardPromptPanel_h__

#include "basic/BasePanel.h"

class AwardPromptPanel: public BasePanel
{
public:
	AwardPromptPanel();
	virtual ~AwardPromptPanel();

	static AwardPromptPanel* create();

	// override start

	virtual bool init();

	virtual void open();

	// override end

private:
	void initUI();

	void touchEventHandler(CCObject *pSender, TouchEventType type);

private:
	UIButton *m_pCloseBtn;

	UIImageView *m_pItemFrameImg;

	UILabel *m_pItemNameTxt;

};

#endif // AwardPromptPanel_h__