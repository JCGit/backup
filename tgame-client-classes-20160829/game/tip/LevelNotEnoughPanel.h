#ifndef LevelNotEnoughPanel_h__
#define LevelNotEnoughPanel_h__

#include "basic/BasePanel.h"

class LevelNotEnoughPanel: public BasePanel
{
public:
	LevelNotEnoughPanel();
	virtual ~LevelNotEnoughPanel();

	static LevelNotEnoughPanel* create();

	// override start

	virtual bool init();

	// override end

	void openWithLimitLevel(unsigned int uLimitLevel);

private:
	void initUI();

	void touchEventHandler(CCObject *pSender, TouchEventType type);

private:
	UIButton *m_pCloseBtn;

	UILabel *m_pPromptTxt;

	UILayout *m_pEntrysLayout;

};

#endif // LevelNotEnoughPanel_h__