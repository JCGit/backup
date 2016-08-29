#ifndef NextStageTipLayer_h__
#define NextStageTipLayer_h__

#include "BaseLayout.h"
#include "vlib/QEventDispatcher.h"

class CFightTipLayout;

class CNextStageTipLayer : public BaseLayout, public CQEventDispatcher
{
public:
	CNextStageTipLayer();
	virtual ~CNextStageTipLayer();

	CREATE_FUNC(CNextStageTipLayer);

	bool init();

	void startAct();
protected:
private:
	CFightTipLayout* m_pNextStageTip;		//��һ�׶���ʾ

	UILayout* m_pLayer;
};

#endif // NextStageTipLayer_h__
