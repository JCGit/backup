#ifndef DialogLayout_h__
#define DialogLayout_h__

#include "basic/BaseLayout.h"

typedef void (CCObject::*SEL_DIALOG)(CCObject*, unsigned int);
#define dialog_selector(_SELECTOR) (SEL_DIALOG)(&_SELECTOR)

class AnimSprite;

class DialogLayout: public BaseLayout
{
public:
	DialogLayout();
	virtual ~DialogLayout();

	static DialogLayout* create();

	// override start

	virtual bool init();

	// override end

	void setStoryID(unsigned int uStoryID);

	void showDialog();

	void hideDialog();

	void addDialogEventListener(CCObject *pListener, SEL_DIALOG pSelector);

private:
	void playDialog();

	void onHideDialog(float dt);

	void literallyShow(); // 逐字显示

	void touchEventHandler(CCObject *pSender, TouchEventType type);

private:
	UILayout* m_pBgLayout;

	UIImageView *m_pBgWordsImg;

	UIImageView *m_pRoleHeadImg;

	UIImageView* m_pNameBgImg;

	UIImageView* m_pLightImg;

	UILabel *m_pRoleNameTxt;

	UILabel *m_pWordsTxt;

	AnimSprite *m_pNextAnim;

	unsigned int m_uStoryID;

	std::vector<std::string> m_stWords; // 解析后的字符数组

	int m_uDialogIdx;

	unsigned int m_uShowIdx;

	CCObject *m_pListener; 

	SEL_DIALOG m_pfnSelector;

	bool m_bActivate;

};

#endif // DialogLayout_h__