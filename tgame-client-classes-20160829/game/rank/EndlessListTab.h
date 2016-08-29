#ifndef EndlessListTab_h__
#define EndlessListTab_h__

#include "BaseTabLayer.h"

class CEndlessListTab : public BaseTabLayer
{
public:
	CEndlessListTab();
	virtual ~CEndlessListTab();

	virtual void initUI(UILayout *rootLayer, const char *name);
	void updateUI();
protected:
private:
};

#endif // EndlessListTab_h__
