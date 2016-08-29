#ifndef FireButton_h__
#define FireButton_h__

#include "cocos2d.h"
#include "cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;

using namespace gui;
using namespace std;

class FireButton : public Button
{
public:
	FireButton();
	virtual ~FireButton();

	CREATE_FUNC(FireButton);

	virtual string getDescription() const;
protected:
private:
};

#endif // FireButton_h__
