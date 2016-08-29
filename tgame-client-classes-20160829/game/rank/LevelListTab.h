#ifndef LevelListTab_h__
#define LevelListTab_h__

#include "BaseTabLayer.h"

class CLevelListTab : public BaseTabLayer
{
public:
	CLevelListTab();
	virtual ~CLevelListTab();

	virtual void initUI(UILayout *rootLayer, const char *name);
	void updateUI();
protected:
private:
};

#endif // LevelListTab_h__
