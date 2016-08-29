#ifndef PowerListTab_h__
#define PowerListTab_h__

#include "BaseTabLayer.h"

class CPowerListTab : public BaseTabLayer
{
public:
	CPowerListTab();
	virtual ~CPowerListTab();

	virtual void initUI(UILayout *rootLayer, const char *name);
	void updateUI();
protected:
private:
};

#endif // PowerListTab_h__
