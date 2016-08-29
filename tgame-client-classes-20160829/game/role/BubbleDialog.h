#ifndef BubbleDialog_h__
#define BubbleDialog_h__

#include "BaseLayout.h"

using namespace std;

typedef enum {
	BUBBLE_OVER, // 结束
} BubbleEventType;

typedef void (CCObject::*SEL_BUBBLE)(CCObject*, BubbleEventType);
#define bubble_selector(_SELECTOR) (SEL_BUBBLE)(&_SELECTOR)

class CBubbleDialog : public BaseLayout
{
public:
	CBubbleDialog();
	virtual ~CBubbleDialog();

	CREATE_FUNC(CBubbleDialog);

	bool init();

	void setString(string str);

	void showTxtByStep();

	void stepUpdate(float dt);

	void hideDialog();

	void sizeChange();

	void addBubbleEventListener(CCObject *pListener, SEL_BUBBLE pSelector);

	void dispatchEvent(BubbleEventType type);
protected:
private:
	ImageView* m_pUpImg;
	ImageView* m_pMidImg;
	ImageView* m_pDownImg;
	UILabel* m_pTxt;

	string m_stTxt;
	unsigned int m_uIdx;

	vector<string> m_stWords; // 解析后的字符数组

	CCObject *m_pListener; 

	SEL_BUBBLE m_pfnSelector;
};

#endif // BubbleDialog_h__
