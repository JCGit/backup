#ifndef PrestigeListTab_h__
#define PrestigeListTab_h__

#include "BaseTabLayer.h"

class CPrestigeListTab : public BaseTabLayer
{
public:
	CPrestigeListTab();
	virtual ~CPrestigeListTab();

	virtual void initUI(UILayout *rootLayer, const char *name);
	void updateUI();
protected:
private:
};

#endif // PrestigeListTab_h__
