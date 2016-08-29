#ifndef AutoPopControl_h__
#define AutoPopControl_h__

#include "vlib/base/CVSingleton.h"

#include "basic/BasePanel.h"

class AutoPopControl
{
public:
	AutoPopControl();
	~AutoPopControl();

	void addPopPanel(BasePanel *pPanel);

	void popPanel();

	void onClosePanel(BasePanel* pPanel);

private:
	list<BasePanel*> m_stPanels;

	BasePanel *m_pCurPopPanel;

};

#define P_AUTO_POP_CTRL CVLib::CVSingleton<AutoPopControl>::Instance()

#endif // AutoPopControl_h__