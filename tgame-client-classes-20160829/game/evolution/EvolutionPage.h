#ifndef EvolutionPage_h__
#define EvolutionPage_h__

#include "basic/BaseLayout.h"

class EvolutionPage: public BaseLayout
{
public:
	EvolutionPage();
	virtual ~EvolutionPage();
	
	/*static EvolutionPage* create(UILayout *pRefLayout);

	bool initWithLayout(UILayout *pRefLayout);*/

	static EvolutionPage* create();

	virtual bool init();

private:
	void touchEventHandler(CCObject *pSender, TouchEventType type);
	
};

#endif // EvolutionPage_h__