#include "NPageView.h"

NPageView::NPageView()
{
}

NPageView::~NPageView()
{
}

NPageView* NPageView::create() {
	NPageView *pRet = new NPageView();

	if(pRet != NULL && pRet->init()) {
		pRet->autorelease();
	} else {
		CC_SAFE_DELETE(pRet);
	}

	return pRet;
}

bool NPageView::init() {
	bool bRet = false;

	do {
		CC_BREAK_IF(!ScrollView::init());

		_autoScroll = true;

		bRet = true;
	} while(0);

	return bRet;
}

void NPageView::scrollToDestination(const CCPoint& dst, float time, bool attenuated) {
	startAutoScrollChildrenWithDestination(dst, time, attenuated);
}
