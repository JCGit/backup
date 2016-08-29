#ifndef AutoFightLayer_h__
#define AutoFightLayer_h__

#include "BaseLayout.h"
#include "vlib/QEventDispatcher.h"

class CAutoFightLayer : public BaseLayout, public CQEventDispatcher
{
public:
	static const string CANCEL_AUTO_EVENT;
public:
	CAutoFightLayer();
	virtual ~CAutoFightLayer();

	CREATE_FUNC(CAutoFightLayer);

	bool init();

private:
	void touchEventHandler(CCObject *pSender, TouchEventType type);

	UILayout* m_pLayer;
	CCArmature* m_pArm;
	Button* m_pCancelBtn;
};

#endif // AutoFightLayer_h__
