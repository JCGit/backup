#ifndef NihilityItemPanel_h__
#define NihilityItemPanel_h__

#include "BasePanel.h"

class CNihilityItemLayout;

class CNihilityItemPanel : public BasePanel
{
public:
	CNihilityItemPanel();
	virtual ~CNihilityItemPanel();

	CREATE_FUNC(CNihilityItemPanel);

	bool init();

	void updateUI();

	void open();
	void close();
protected:
private:
	void initUI();
	void touchEventHandler(CCObject *pSender, TouchEventType type);
	void listEventHandle(CCObject* pSender, ListViewEventType type);

	void setOtherEnable(CNihilityItemLayout* stItem, bool bEnable);

	void sendStart();

	Button* m_pCloseBtn;
	Button* m_pEnterBtn;

	ListView* m_pList;

	CNihilityItemLayout* m_pItemDemo;

	unsigned int m_pSelectItemID;
};

#endif // NihilityItemPanel_h__
