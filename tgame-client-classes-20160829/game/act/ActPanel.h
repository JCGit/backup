#ifndef ActPanel_h__
#define ActPanel_h__

#include "BasePanel.h"

class ActLayout;

class CActPanel : public BasePanel
{
public:
	CActPanel();
	virtual ~CActPanel();

	CREATE_FUNC(CActPanel);

	bool init();

	void open();

	void close();

	void refresh();

	void updateUI();

	void setTypeIdx(unsigned int idx);
	unsigned int getTypeIdx();

	void updateContent();

	void updateCurType();

	void setActSelectType(unsigned int type);
private:
	void initUI();

	void touchEventHandler(CCObject *pSender, TouchEventType type);

	void listViewEventHandler(CCObject *pSender, ListViewEventType type);

	ActLayout* getActLayout(unsigned int uActType);

	ActLayout* createActLayout(unsigned int uActType);

private:
	Button* m_pCloseBtn;

	ListView* m_pActList;

	UILayout* m_pActLayout;

	map<unsigned int, ActLayout*> m_stActLayoutMap; // <ActType, ActLayout*>

	int m_nActSelectedIdx;

	int m_actSelectType;//选中的活动类型
};

#endif // ActPanel_h__
