#ifndef BaseLayout_h__
#define BaseLayout_h__

#include "cocos2d.h"
#include "cocos-ext.h"

using namespace std;
USING_NS_CC;
USING_NS_CC_EXT;
using namespace cocos2d::gui;

class BaseLayout: public UILayout
{
public:
	BaseLayout();
	virtual ~BaseLayout();

	// override start

	virtual bool init();

	virtual std::string getDescription() const;

	// override end

	UILayout* getRootLayout();

	void setRootLayout(UILayout *pRootLayout);

	bool isUIInited();

protected:
	virtual void initLayoutFromJsonFile(const char *pszFileName); // init the root layout from json file

	virtual void initLayoutWithUI(UILayout *pTargetLayout); // init the root layout with a layout child of UI

	virtual void initLayoutWithUICopy(UILayout *pRefLayout); // init the root layout with a layout copy

protected:
	UILayout *m_pRootLayout; // the root layout from loaded gui resources

	bool m_bUIInited; // UI是否已初始化

	bool m_bUIDelayed; // UI是否滞后，即操作UI时其尚未完成初始化

};

#endif // BaseLayout_h__
