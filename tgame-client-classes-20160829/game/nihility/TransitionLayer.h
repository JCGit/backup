#ifndef TransitionLayer_h__
#define TransitionLayer_h__

#include "BaseLayout.h"
#include "vlib/QEventDispatcher.h"

USING_NS_CC;
USING_NS_CC_EXT;

using namespace gui;
using namespace std;

class CTransitionLayer : public BaseLayout, public CQEventDispatcher
{
public:
	static const string TRANSITION_MID_EVENT;
	static const string TRANSITION_COMPLETE;
public:
	CTransitionLayer();
	virtual ~CTransitionLayer();

	CREATE_FUNC(CTransitionLayer);

	bool init();

	void startAct();
	void midAct();
	void finishAct();
protected:
private:
	UILayout* m_pLayer;
};


#endif // TransitionLayer_h__
