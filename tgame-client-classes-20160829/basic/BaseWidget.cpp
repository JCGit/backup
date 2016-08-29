#include "BaseWidget.h"

BaseWidget::BaseWidget()
{
}

BaseWidget::~BaseWidget()
{
}

bool BaseWidget::init() {
	bool bRet = false;

	do {
		CC_BREAK_IF(!UIWidget::init());

		bRet = true;
	} while(0);

	return bRet;
}

std::string BaseWidget::getDescription() const {
	return "BaseWidget";
}
