#include "GuideCallFunc.h"

#include "GuideControl.h"

GuideCallFunc::GuideCallFunc()
{
}

GuideCallFunc::~GuideCallFunc()
{
}

GuideCallFunc* GuideCallFunc::create() {
	GuideCallFunc *pRet = new GuideCallFunc();

	if(pRet != NULL) {
		pRet->autorelease();
	} else {
		CC_SAFE_DELETE(pRet);
	}

	return pRet;
}

void GuideCallFunc::checkGuideNodeExit(float dt) {
	P_GUIDE_CTRL->checkGuideNodeExit();
}
